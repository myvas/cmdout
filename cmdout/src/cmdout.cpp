/**
 * Copyright (c) 2023 Myvas Foundation
 * SPDX-License-Identifier: MIT
 *
 * @file  cmdout.cpp
 * @brief Implementation of class `cmdout`.
 */
#include "cmdout.hpp"

#include <array>
#include <cstdio>
#include <cstring>
#include <string>
#include <string_view>
#include <stdexcept>
#include <cassert>
#include <future>
#include <thread>

namespace myvas {

int cmdout::status() const { return status_; }
cmdout& cmdout::status(int value) { status_ = value; return *this; }

std::string cmdout::out() const { return out_; }
cmdout& cmdout::out(const std::string& value) { out_ = value;  return *this; }

std::string cmdout::cmd() const { return cmd_; }
cmdout& cmdout::cmd(const std::string& value) { cmd_ = value; return *this; }

int64_t cmdout::timeout_ms() const { return timeout_ms_; }
std::chrono::milliseconds cmdout::timeout() const { return std::chrono::milliseconds(timeout_ms_); }
cmdout& cmdout::timeout(std::chrono::milliseconds value) { timeout_ms_ = value.count(); return *this; }
cmdout& cmdout::timeout(int64_t value) { timeout_ms_ = value; return *this; }

cmdout::cmdout()
{
}

cmdout::cmdout(const std::string& cmd)
	: cmd_(cmd)
{
}

cmdout::cmdout(const std::string& cmd, int64_t timeout_ms)
	: cmd_(cmd), timeout_ms_(timeout_ms)
{
}

cmdout::cmdout(const std::string& cmd, std::chrono::milliseconds timeout)
	:cmd_(cmd), timeout_ms_(timeout.count())
{
}

cmdout::cmdout(const std::string& cmd, int64_t timeout_ms, int status, const std::string& out)
	: cmd_(cmd), timeout_ms_(timeout_ms), status_(status), out_(out)
{
}

cmdout::cmdout(const std::string& cmd, std::chrono::milliseconds timeout, int status, const std::string& out)
	: cmd_(cmd), timeout_ms_(timeout.count()), status_(status), out_(out)
{
}

cmdout::cmdout(const cmdout&) = default;
cmdout& cmdout::operator=(const cmdout&) = default;
cmdout::cmdout(cmdout&&) = default;
cmdout& cmdout::operator=(cmdout&&) = default;
cmdout::~cmdout() = default;

cmdout& cmdout::exec()
{
	if (cmd_.empty())
	{
		cmd_ = "exit 0";

		auto t = do_system(cmd_);
		status_ = t.status();
		out_ = t.out();
		return *this;
	}

	auto t = do_system_timeout_ms(cmd_, timeout_ms_);
	status_ = t.status();
	out_ = t.out();
	return *this;
}
///////////////////
/// Private functions
///////////////////

/**
 * @brief Execute a shell command specified in `cmd`.
 *
 * @return Returns a `cmdout` object but no field `timeout`.
 */
cmdout cmdout::do_system(const std::string& cmd)
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

/**
 * @brief Execute a shell command specified in `cmd` and get its output before time runs out.
 */
cmdout cmdout::do_system_timeout_ms(const std::string& cmd, int64_t timeout_ms)
{
	return do_system_timeout(cmd, std::chrono::milliseconds(timeout_ms));
}

cmdout cmdout::do_system_timeout(const std::string& cmd, std::chrono::milliseconds timeout)
{
	auto timeout_ms = timeout.count();
	if (timeout_ms < 1)
	{
		timeout = std::chrono::milliseconds(CMDOUT_TIMEOUT_MILLISECONDS);
		timeout_ms = timeout.count();
	}

	std::future<cmdout> future = std::async(std::launch::async, [&cmd, &timeout_ms]() {
		return cmdout(do_system(cmd).timeout(timeout_ms));
	});

	std::future_status fstatus;
	do
	{
		switch (fstatus = future.wait_for(timeout); fstatus)
		{
		case std::future_status::timeout:
		{
			auto status = ETIME; // #include <cerrno>, ECANCELED or ETIME
			auto out = "Timeout: The command did not return before specified timeout duration ("
				+ std::to_string(timeout_ms) + " ms) has passed.";
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