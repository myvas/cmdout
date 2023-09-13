/**
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 Myvas Foundation
 *
 * @file  cmdout.cpp
 * @brief Implementation of class myvas::cmdout.
 */
#include <cmdout/cmdout.h>

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

			auto t = system(cmd_);
			status_ = t.status();
			out_ = t.out();
			return *this;
		}

		auto t = system_timeout_ms(cmd_, timeout_ms_);
		status_ = t.status();
		out_ = t.out();
		return *this;
	}

} // namespace myvas
