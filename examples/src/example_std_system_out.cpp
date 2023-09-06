/**
 * Copyright (c) 2023 Myvas Foundation
 * SPDX-License-Identifier: MIT
 *
 * @file  example_std_system_out.cpp
 * @brief example for function std::system() to execute a shell command and get its output.
 */
#include <cmdout/experimental/std_system_out.hpp>

#include <iostream>
#include <cstdio>
#include <fstream>

int main() {
	{
		auto cmd = "ls not-exist";
		auto result = myvas::experimental::std_system_out(cmd);
		std::cout << "status: " << std::get<0>(result) << std::endl;
		std::cout << "out: \n" << std::get<1>(result) << std::endl;
	}
	{
		auto cmd = "ls ~ -C";
                auto result = myvas::experimental::std_system_out(cmd);
		std::cout << "status: " << std::get<0>(result) << std::endl;
		std::cout << "out: \n" << std::get<1>(result) << std::endl;
	}
	return 0;
}
