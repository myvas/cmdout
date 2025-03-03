/**
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 Myvas Foundation
 *
 * @file  benchmark_std_system.cpp
 * @brief benchmark for function std::system() with output.
 */
#include <benchmark/benchmark.h>

#include <cmdout/cmdout.h>
#include <cmdout/experimental/std_system_out.h>

static void BM_std_system(benchmark::State &state)
{
    // Perform setup here
    for (auto _ : state)
    {
        // This code gets timed
        std::system("exit 0");
    }
}

static void BM_myvas_system(benchmark::State &state)
{
    // Perform setup here
    for (auto _ : state)
    {
        // This code gets timed
        myvas::system("exit 0");
    }
}

static void BM_myvas_cmdout(benchmark::State &state)
{
    // Perform setup here
    for (auto _ : state)
    {
        // This code gets timed
        myvas::cmdout("exit 0").exec();
    }
}

static void BM_std_system_out(benchmark::State &state)
{
    // Perform setup here
    for (auto _ : state)
    {
        // This code gets timed
        myvas::experimental::std_system_out("exit 0");
    }
}

// Register the function as a benchmark
BENCHMARK(BM_std_system);
BENCHMARK(BM_std_system_out);
BENCHMARK(BM_myvas_system);
BENCHMARK(BM_myvas_cmdout);

// Run the benchmark
BENCHMARK_MAIN();
