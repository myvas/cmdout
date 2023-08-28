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
#include <stdexcept>

namespace myvas {

int cmdout::status() const { return status_; }

void cmdout::status(int value) { status_ = value; }

std::string_view cmdout::out() const { return out_; }

void cmdout::out(std::string_view value) { out_ = value; }

std::string_view cmdout::cmd() const { return cmd_; }

void cmdout::cmd(std::string_view value) { cmd_ = value; }

cmdout::cmdout()
	: status_(ENOTSUP), out_("Not supported"), cmd_("")
{
}

cmdout::cmdout(std::string_view cmd)
	: status_(ENOTSUP), out_("Not supported"), cmd_(cmd)
{
}

cmdout::cmdout(int status, std::string_view out, std::string_view cmd)
	: status_(status), out_(out), cmd_(cmd)
{
}

cmdout::cmdout(const cmdout&) = default;
cmdout& cmdout::operator=(const cmdout&) = default;
cmdout::cmdout(cmdout&&) = default;
cmdout& cmdout::operator=(cmdout&&) = default;
cmdout::~cmdout() = default;

} // namespace myvas