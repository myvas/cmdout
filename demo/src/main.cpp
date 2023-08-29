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
	std::cout << std::string("*****************************") << std::endl;
	std::cout << std::string("*       std::system()       *") << std::endl;
	std::cout << std::string("*****************************") << std::endl;
	// std::system()
	auto result10 = std::system(NULL);
	std::cout << result10 << std::endl;

	auto result11 = std::system("ls ~/");
	std::cout << result11 << std::endl;

	auto result12 = std::system("ls not-exist 2>&1");
	std::cout << result12 << std::endl;

	std::cout << std::endl;
	std::cout << std::string("*****************************") << std::endl;
	std::cout << std::string("*      myvas::system()      *") << std::endl;
	std::cout << std::string("*****************************") << std::endl;
	// myvas::system()
	auto result30 = myvas::system("exit 0");
	std::cout << result30 << std::endl;

	auto result31 = myvas::system("ls ~/");
	std::cout << result31 << std::endl;

	auto result32 = myvas::system("ls not-exist 2>&1");
	std::cout << result32 << std::endl;

	auto result330 = myvas::system_timeout_ms("ls / -l", 0);
	std::cout << result330 << std::endl;

	auto result331 = myvas::system_timeout_ms("ls / -l", 1);
	std::cout << result331 << std::endl;

	auto result340 = myvas::system_timeout("ls / -l", std::chrono::milliseconds(0)).exec();
	std::cout << result340 << std::endl;

	auto result341 = myvas::system_timeout("ls / -l", std::chrono::milliseconds(1)).exec();
	std::cout << result341 << std::endl;

	std::cout << std::endl;
	std::cout << std::string("*****************************") << std::endl;
	std::cout << std::string("*      myvas::cmdout()      *") << std::endl;
	std::cout << std::string("*****************************") << std::endl;
	// myvas::cmdout
	auto result20 = myvas::cmdout().exec();
	std::cout << result20 << std::endl;

	auto result21 = myvas::cmdout("ls ~/").exec();
	std::cout << result21 << std::endl;

	auto result22 = myvas::cmdout("ls not-exist 2>&1").exec();
	std::cout << result22 << std::endl;

	auto result23 = myvas::cmdout("ls / -l").exec();
	std::cout << result23 << std::endl;

	auto result40 = myvas::cmdout("ls / -l", std::chrono::milliseconds(0)).exec();
	std::cout << result40 << std::endl;

	auto result41 = myvas::cmdout("ls / -l", std::chrono::milliseconds(1)).exec();
	std::cout << result41 << std::endl;

	auto result42 = myvas::cmdout("ls / -l", std::chrono::milliseconds(9)).exec();
	std::cout << result42 << std::endl;

	auto result5 = myvas::cmdout("ls not-exist 2>&1", std::chrono::milliseconds(9)).exec();
	std::cout << result5 << std::endl;
}