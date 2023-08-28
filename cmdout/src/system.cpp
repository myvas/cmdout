/**
 * Copyright (c) 2023 Myvas Foundation
 * SPDX-License-Identifier: MIT
 *
 * @file  system.cpp
 * @brief Implementation of function `myvas::system()`.
 */
#include "cmdout.hpp"

#include <array>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <future>
#include <stdexcept>
#include <thread>
#include <chrono>

namespace myvas {

constexpr std::chrono::milliseconds CMDOUT_TIMEOUT = std::chrono::seconds(10);

/**
 * @brief Execute a shell command specified in `cmd`.
 */
cmdout do_system(const char* cmd)
{
	assert(cmd != nullptr);

	cmdout result(cmd);
	std::string out;
	std::array<char, 4080> buff{};

	FILE* fp = popen(cmd, "r");
	if (fp == NULL)
	{
		result.status(EXIT_FAILURE);
		result.out("popen() failed!");
		return result;
	}

	try
	{
		size_t n;
		while (n = fread(buff.data(), sizeof(buff.at(0)), sizeof(buff), fp), n != 0)
		{
			out += std::string(buff.data(), n);
		}
	}
	catch (const std::exception& ex)
	{
		out += ex.what();
	}

	auto res = pclose(fp);
	auto status = WEXITSTATUS(res);

	result.out(out);
	result.status(status);
	return result;
}

cmdout do_system_timeout(const char* cmd, std::chrono::milliseconds timeout_ms)
{
	assert(cmd != nullptr);

	if (timeout_ms.count() < 1)
		timeout_ms = CMDOUT_TIMEOUT;

	std::future<cmdout> future = std::async(std::launch::async, [&cmd]() {
		return myvas::do_system(cmd);
	});

	std::future_status status;
	do
	{
		switch (status = future.wait_for(timeout_ms); status)
		{
		case std::future_status::timeout:
		{
			auto s = "Timeout: The command did not return before specified timeout duration ("
				+ std::to_string(timeout_ms.count()) + " ms) has passed.";
			return cmdout(ETIME, s, cmd); // #include <cerrno>, ECANCELED or ETIME
		} break;
		case std::future_status::ready:
		{
			return future.get();
		} break;
		default:
			break;
		}
	} while (status != std::future_status::ready &&
		status != std::future_status::timeout);

	return cmdout(cmd);
}

cmdout system(const char* cmd, std::chrono::milliseconds timeout_ms)
{
	if (cmd == nullptr)
	{
		return do_system("exit 0");
	}
	return do_system_timeout(cmd, timeout_ms);
}

cmdout system(const char* cmd, unsigned timeout_ms) { return system(cmd, std::chrono::milliseconds(timeout_ms)); }

cmdout system(const char* cmd) { return system(cmd, CMDOUT_TIMEOUT); }

} // namespace myvas