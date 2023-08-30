/**
 * Copyright (c) 2023 Myvas Foundation
 * SPDX-License-Identifier: MIT
 *
 * @file  example_std_system.cpp
 * @brief example for function `std::system()`.
 */
#include <iostream>
#include <cassert>

void example_std_system() {
	std::cout << "*********************************" << std::endl;
	std::cout << "*    function std::system()     *" << std::endl;
	std::cout << "*********************************" << std::endl;
	{
		auto result = std::system(nullptr);
		std::cout << result << std::endl;
		// https://en.cppreference.com/w/cpp/utility/program/system
		// If command is a null pointer, returns a nonzero value
		// if and only if the command processor exists.
		assert(result != 0);
	}
	{
		auto result = std::system("exit 0");
		std::cout << result << std::endl;
		assert(result == EXIT_SUCCESS);
	}
	{
		auto result = std::system("exit 1");
		std::cout << result << std::endl;
		assert(result == EXIT_FAILURE);
	}
	{
		auto result = std::system("ls ~/");
		std::cout << result << std::endl;
		assert(result == EXIT_SUCCESS);
	}
	{
		auto result = std::system("ls not-exist 2>&1");
		std::cout << result << std::endl;
		assert(WEXITSTATUS(result) == ENOENT);
	}
}

int main() {
	example_std_system();
}
