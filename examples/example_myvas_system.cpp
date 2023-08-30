/**
 * Copyright (c) 2023 Myvas Foundation
 * SPDX-License-Identifier: MIT
 *
 * @file  example_myvas_system.cpp
 * @brief example for function `myvas::system()`.
 */
#include <cmdout_ext.hpp>

#include <iostream>
#include <cassert>

void example_myvas_system() {
	std::cout << "***********************************" << std::endl;
	std::cout << "*    function myvas::system()     *" << std::endl;
	std::cout << "***********************************" << std::endl;
	{
		auto result = myvas::system("exit 0");
		std::cout << result << std::endl;
		assert(result.status() == EXIT_SUCCESS);
	}
	{
		auto result = myvas::system("exit 1");
		std::cout << result << std::endl;
		assert(result.status() == EXIT_FAILURE);
	}
	{
		auto result = myvas::system("ls not-exist 2>&1");
		std::cout << result << std::endl;
		assert(result.status() == ENOENT);
	}
	{
		auto result = myvas::system("ls ~/");
		std::cout << result << std::endl;
		assert(result.status() == EXIT_SUCCESS);
	}
	{
		auto result = myvas::system("ls not-exist 2>&1");
		std::cout << result << std::endl;
		assert(result.status() == ENOENT);
	}
	{
		auto result = myvas::system_timeout_ms("ls / -l", 0);
		std::cout << result << std::endl;
		assert(result.status() == EXIT_SUCCESS);
	}
	{
		auto result = myvas::system_timeout("ls / -l", std::chrono::milliseconds(0)).exec();
		std::cout << result << std::endl;
		assert(result.status() == EXIT_SUCCESS);
	}
	{
		auto result = myvas::system_timeout_ms("ls / -l", 1);
		std::cout << result << std::endl;
		assert(result.status() == ETIME);
	}
	{
		auto result = myvas::system_timeout("ls / -l", std::chrono::milliseconds(1)).exec();
		std::cout << result << std::endl;
		assert(result.status() == ETIME);
	}
}

int main() {
	example_myvas_system();
}
