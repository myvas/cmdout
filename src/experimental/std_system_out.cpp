/**
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 Myvas Foundation
 *
 * @file  std_system_out.cpp
 * @brief Implementation of Experimental functions for the benchmark control group,
 * via function std::system() to execute a shell command and get its output.
 */
#include "cmdout/experimental/std_system_out.h"

#include <cstdio>
#include <fstream>

namespace myvas::experimental
{

std::tuple<int, std::string> std_system_out(const std::string &cmd)
{
    // warning: the use of `tmpnam' is dangerous, better use `mkstemp'
    auto tempfilepath = std::tmpnam(nullptr);

    // execute a shell command, and log the stdout and stderr to a temp file
    auto cmd_stdout = cmd + std::string(" >> ") + tempfilepath + std::string(" 2>&1");
    auto exit_code = std::system(cmd_stdout.c_str());
    auto status = WEXITSTATUS(exit_code);

    // read the temp file
    std::ifstream stdout_file(tempfilepath);
    if (!stdout_file.is_open())
    {
        status = EXIT_FAILURE;
    }
    auto out = std::string((std::istreambuf_iterator<char>(stdout_file)), std::istreambuf_iterator<char>());

    // cleanup the temp file
    std::remove(tempfilepath);

    return std::make_tuple(status, out);
}

} // namespace myvas::deleted
