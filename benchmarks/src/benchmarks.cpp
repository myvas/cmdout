/**
 * Copyright (c) 2023 Myvas Foundation
 * SPDX-License-Identifier: MIT
 *
 * @file  benchmarks.cpp
 * @brief benchmarks for `myvas::cmdout`.
 */
#include "cmdout.hpp"

#include <iostream>

static const int default_iters = 10000;

void benchmark_std_system(int iters)
{
	using std::chrono::duration;
	using std::chrono::duration_cast;
	using std::chrono::high_resolution_clock;

	auto start = high_resolution_clock::now();
	for (auto i = 0; i < iters; ++i)
	{
		std::system("exit 0");
	}

	auto delta = high_resolution_clock::now() - start;
	auto delta_d = duration_cast<duration<double>>(delta).count();
	auto result = iters / delta_d;
	std::cout << "std::system(\"exit 0\"):          runs " << iters
		<< " times, elapsed " << delta.count()
		<< " ns, result: " << result << " x/s" << std::endl;
}

void benchmark_myvas_system(int iters)
{
	using std::chrono::duration;
	using std::chrono::duration_cast;
	using std::chrono::high_resolution_clock;

	auto start = high_resolution_clock::now();
	for (auto i = 0; i < iters; ++i)
	{
		myvas::system("exit 0");
	}

	auto delta = high_resolution_clock::now() - start;
	auto delta_d = duration_cast<duration<double>>(delta).count();
	auto result = iters / delta_d;
	std::cout << "myvas::system(\"exit 0\"):        runs " << iters
		<< " times, elapsed " << delta.count()
		<< " ns, result: " << result << " x/s" << std::endl;
}

void benchmark_myvas_cmdout(int iters)
{
	using std::chrono::duration;
	using std::chrono::duration_cast;
	using std::chrono::high_resolution_clock;

	auto start = high_resolution_clock::now();
	for (auto i = 0; i < iters; ++i)
	{
		myvas::cmdout("exit 0").exec();
	}

	auto delta = high_resolution_clock::now() - start;
	auto delta_d = duration_cast<duration<double>>(delta).count();
	auto result = iters / delta_d;
	std::cout << "myvas::cmdout(\"exit 0\").exec(): runs " << iters
		<< " times, elapsed " << delta.count()
		<< " ns, result: " << result << " x/s" << std::endl;
}

void benchmark(int iters) {
	benchmark_std_system(iters);
	benchmark_myvas_system(iters);
	benchmark_myvas_cmdout(iters);
}

int main(int argc, char* argv[]) {
	int iters = default_iters;
	try {
		if (argc > 1) {
			iters = std::stoi(argv[1]);
		}

		benchmark(iters);
	}
	catch (std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
