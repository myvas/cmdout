# Copyright (c) 2023 Myvas Foundation
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.19)
project(cmdout_benchmarks LANGUAGES CXX)

if(NOT TARGET cmdout)
    # Stand-alone build
    find_package(cmdout REQUIRED)
endif()

include(cmake/benchmark.cmake)

###########################
# target benchmarks
###########################
add_executable(benchmarks "src/benchmarks.cpp")
cmdout_enable_warnings(benchmarks)
target_link_libraries(benchmarks PRIVATE cmdout benchmark::benchmark)