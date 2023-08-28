/**
 * Copyright (c) 2023 Myvas Foundation
 * SPDX-License-Identifier: MIT
 *
 * @file  main.cpp
 * @brief Demonstrate the usage of `myvas::system()`.
 */
#include <cmdout_ext.hpp>
#include <iostream>

int main()
{
	// std::system(cmd)
	auto result10 = std::system(NULL);
	std::cout << result10 << std::endl;

	auto result11 = std::system("ls ~/");
	std::cout << result11 << std::endl;

	auto result12 = std::system("ls not-exist 2>&1");
	std::cout << result12 << std::endl;

	// myvas::system(cmd)
	auto result20 = myvas::system(NULL);
	std::cout << result20 << std::endl;

	auto result21 = myvas::system("ls ~/");
	std::cout << result21 << std::endl;

	auto result22 = myvas::system("ls not-exist 2>&1");
	std::cout << result22 << std::endl;

	auto result23 = myvas::system("ls / -l");
	std::cout << result23 << std::endl;

	// myvas::system(cmd, timeout)
	auto result40 = myvas::system("ls / -l", std::chrono::milliseconds(0));
	std::cout << result40 << std::endl;

	auto result41 = myvas::system("ls / -l", std::chrono::milliseconds(1));
	std::cout << result41 << std::endl;

	auto result42 = myvas::system("ls / -l", std::chrono::milliseconds(9));
	std::cout << result42 << std::endl;

	auto result5 = myvas::system("ls not-exist 2>&1", std::chrono::milliseconds(9));
	std::cout << result5 << std::endl;
}