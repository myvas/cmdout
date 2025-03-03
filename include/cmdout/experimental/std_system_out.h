/**
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 Myvas Foundation
 *
 * @file  std_system_out.h
 * @brief Experimental functions for the benchmark control group.
 */
#pragma once

#include <string>
#include <tuple>

namespace myvas::experimental {

/**
 * @brief A simple implementation of executing a shell command and gets its output,
 * via calling function std::system() and reading log files.
 */
std::tuple<int, std::string> std_system_out(const std::string &cmd);

}
