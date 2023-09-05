/**
 * Copyright (c) 2023 Myvas Foundation
 * SPDX-License-Identifier: MIT
 *
 * @file  example_std_system_out.cpp
 * @brief example for function std::system() to execute a shell command and get its output.
 */
#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <tuple>

/**
 * @brief A simple implementation of executing a shell command and gets its output,
 * via calling function std::system() and reading log files.
 */
std::tuple<int, std::string> std_system_out(const std::string& cmd)
{
	// warning: the use of `tmpnam' is dangerous, better use `mkstemp'
	auto tempfilepath = std::tmpnam(nullptr);

	// execute a shell command, and log the stdout and stderr to a temp file
	auto cmd_stdout = cmd + std::string(" >> ") + tempfilepath + std::string(" 2>&1");
	auto exit_code = std::system(cmd_stdout.c_str());
	auto status = WEXITSTATUS(exit_code);

	// read the temp file
	std::ifstream stdout_file(tempfilepath);
	if (!stdout_file.is_open()) {
		status = EXIT_FAILURE;
	}
	auto out = std::string((std::istreambuf_iterator<char>(stdout_file)), std::istreambuf_iterator<char>());

	// cleanup the temp file
	std::remove(tempfilepath);

	return std::make_tuple(status, out);
}

int main() {
	{
		auto cmd = "ls not-exist";
		auto result = std_system_out(cmd);
		std::cout << "status: " << std::get<0>(result) << std::endl;
		std::cout << "out: \n" << std::get<1>(result) << std::endl;
	}
	{
		auto cmd = "ls ~ -C";
		auto result = std_system_out(cmd);
		std::cout << "status: " << std::get<0>(result) << std::endl;
		std::cout << "out: \n" << std::get<1>(result) << std::endl;
	}
	return 0;
}
