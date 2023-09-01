# Copyright (c) 2023 Myvas Foundation
# SPDX-License-Identifier: MIT

###################################
# Find or fetch a CMake package
#
# benchmark >=1.8.3
# benchmark::benchmark
###################################

find_package(benchmark QUIET)
if(benchmark_FOUND)
    message(STATUS "Found benchmark: ${benchmark_VERSION}")
else()
    message(STATUS "CMake package benchmark >=1.8.3 not found! Please wait for fetching from github.com...")

    include(FetchContent)
    ###############################################
    # Do not build and run googlebenchmark tests!
    ###############################################
    set(BENCHMARK_ENABLE_TESTING OFF CACHE INTERNAL "")
    FetchContent_Declare(benchmark
        URL https://github.com/google/benchmark/archive/refs/tags/v1.8.3.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP ON
    )
    FetchContent_MakeAvailable(benchmark)
endif()