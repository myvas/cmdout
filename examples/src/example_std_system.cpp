/**
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 Myvas Foundation
 *
 * @file  example_std_system.cpp
 * @brief example for function std::system().
 */
#include <iostream>
#include <cassert>

/**
 * @brief example for function std::system().
 */
void example_std_system() {
	std::cout << "*********************************" << std::endl;
	std::cout << "*    function std::system()     *" << std::endl;
	std::cout << "*********************************" << std::endl;
	{
		std::cout << std::endl;
		auto result = std::system(nullptr);
		std::cout << "std::system(nullptr) returns " << result << std::endl;
		// https://en.cppreference.com/w/cpp/utility/program/system
		// If command is a null pointer, returns a nonzero value
		// if and only if the command processor exists.
		assert(result != 0);
		assert(result == 1);
		assert(WEXITSTATUS(result) == EXIT_SUCCESS);
	}
	{
		std::cout << std::endl;
		auto cmd = "exit 0";
		auto result = std::system(cmd);
		std::cout << "std::system(\"" << cmd << "\") returns " << result << std::endl;
		assert(WEXITSTATUS(result) == EXIT_SUCCESS);
	}
	{
		std::cout << std::endl;
		auto cmd = "exit 1";
		auto result = std::system("exit 1");
		std::cout << "std::system(\"" << cmd << "\") returns " << result << std::endl;
		assert(WEXITSTATUS(result) == EXIT_FAILURE);
	}
	{
		std::cout << std::endl;
		auto cmd = "ls ~/ -C";
		auto result = std::system(cmd);
		std::cout << "std::system(\"" << cmd << "\") returns " << result << std::endl;
		assert(WEXITSTATUS(result) == EXIT_SUCCESS);
	}
	{
		std::cout << std::endl;
		auto cmd = "ls not-exist 2>&1";
		auto result = std::system(cmd);
		std::cout << "std::system(\"" << cmd << "\") returns " << result << std::endl;
		assert(WEXITSTATUS(result) == ENOENT);
	}
}

int main() {
	example_std_system();
}
