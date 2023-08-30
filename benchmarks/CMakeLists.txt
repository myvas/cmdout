# Copyright (c) 2023 Myvas Foundation
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.19)
project(cmdout_benchmarks CXX)

if(NOT TARGET cmdout)
    # Stand-alone build
    find_package(cmdout REQUIRED)
endif()

# cmdout already includes Threads
#find_package(Threads REQUIRED)

find_package(benchmark CONFIG)
if(NOT benchmark_FOUND)
    # fetch googlebenchmark (require CMake>=3.11.0)
    message(STATUS "Downloading GoogleBenchmark")
    include(FetchContent)

    ###############################################
    # Do not build and run googlebenchmark tests!
    ###############################################
    set(BENCHMARK_ENABLE_TESTING OFF CACHE INTERNAL "")
    #FetchContent_Declare(googlebenchmark GIT_REPOSITORY https://github.com/google/benchmark.git GIT_TAG v1.6.0)
    FetchContent_Declare(googlebenchmark URL https://github.com/google/benchmark/archive/refs/tags/v1.6.0.tar.gz)
    FetchContent_MakeAvailable(googlebenchmark)
endif()

###########################
# target benchmarks
###########################
add_executable(benchmarks "benchmarks.cpp")
cmdout_enable_warnings(benchmarks)
target_link_libraries(benchmarks PRIVATE myvas::cmdout)