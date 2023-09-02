/**
 * Copyright (c) 2023 Myvas Foundation
 * SPDX-License-Identifier: MIT
 *
 * @file  system.cpp
 * @brief Implementation of function `myvas::system()`.
 */
#include "cmdout.hpp"

#include <array>
#include <future>
#include <thread>

namespace myvas {

cmdout system(const std::string& cmd)
{
	cmdout result(cmd);
	std::string out;
	std::array<char, 4080> buff{};

	FILE* fp = popen(cmd.c_str(), "r");
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
	result.status(status);
	result.out(out);

	return result;
}

cmdout system_timeout_ms(const std::string& cmd, int64_t timeout_ms)
{
	return system_timeout(cmd, std::chrono::milliseconds(timeout_ms));
}

cmdout system_timeout(const std::string& cmd, std::chrono::milliseconds timeout)
{
	auto timeout_ms = timeout.count();
	if (timeout_ms < 1)
	{
		timeout = std::chrono::milliseconds(CMDOUT_TIMEOUT_MILLISECONDS);
		timeout_ms = timeout.count();
	}

	std::future<cmdout> future = std::async(std::launch::async, [&cmd, &timeout_ms]() {
		return cmdout(system(cmd).timeout(timeout_ms));
	});

	std::future_status fstatus;
	do
	{
		switch (fstatus = future.wait_for(timeout); fstatus)
		{
		case std::future_status::timeout:
		{
			auto status = ETIME; // #include <cerrno>, ECANCELED or ETIME
			auto out = "Timeout: The command did not return before time runs out. (" + std::to_string(timeout_ms) + " ms)";
			return cmdout(cmd, timeout_ms, status, out);
		} break;
		case std::future_status::ready:
		{
			return future.get();
		} break;
		default:
			break;
		}
	} while (fstatus != std::future_status::ready &&
		fstatus != std::future_status::timeout);

	return cmdout(cmd, timeout_ms);
}

} // namespace myvas