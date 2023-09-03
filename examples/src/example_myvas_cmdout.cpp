/**
 * Copyright (c) 2023 Myvas Foundation
 * SPDX-License-Identifier: MIT
 *
 * @file  example_myvas_cmdout.cpp
 * @brief example for class myvas::cmdout.
 */
#include <cmdout/cmdout_ext.hpp>

#include <iostream>
#include <cassert>

void example_myvas_cmdout() {
	std::cout << "******************************" << std::endl;
	std::cout << "*    class myvas::cmdout     *" << std::endl;
	std::cout << "******************************" << std::endl;
	{
		auto result = myvas::cmdout().exec();
		std::cout << result << std::endl;
		assert(result.status() == EXIT_SUCCESS);
	}
	{
		std::cout << std::endl;
		auto result = myvas::cmdout("ls ~/ -C").exec();
		std::cout << result << std::endl;
		assert(result.status() == EXIT_SUCCESS);
	}
	{
		std::cout << std::endl;
		auto result = myvas::cmdout("ls not-exist 2>&1").exec();
		std::cout << result << std::endl;
		assert(result.status() == ENOENT);
	}
	{
		std::cout << std::endl;
		auto result = myvas::cmdout("ls / -C").exec();
		std::cout << result << std::endl;
		assert(result.status() == EXIT_SUCCESS);
	}
	{
		std::cout << std::endl;
		auto result = myvas::cmdout("ls / -C", std::chrono::milliseconds(0)).exec();
		std::cout << result << std::endl;
		assert(result.status() == EXIT_SUCCESS);
	}
	{
		std::cout << std::endl;
		auto result = myvas::cmdout("ls / -C", std::chrono::milliseconds(1)).exec();
		std::cout << result << std::endl;
		assert(result.status() == ETIME);
	}
	{
		std::cout << std::endl;
		auto result = myvas::cmdout("ls / -C", std::chrono::milliseconds(9)).exec();
		std::cout << result << std::endl;
		assert(result.status() == EXIT_SUCCESS);
	}
	{
		std::cout << std::endl;
		auto result = myvas::cmdout("ls not-exist 2>&1", std::chrono::milliseconds(9)).exec();
		std::cout << result << std::endl;
		assert(result.status() == ENOENT);
	}
}

int main() {
	example_myvas_cmdout();
}
