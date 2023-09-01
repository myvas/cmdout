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
include(cmake/benchmark.cmake)

###########################
# target benchmarks
###########################
add_executable(benchmarks "benchmarks.cpp")
cmdout_enable_warnings(benchmarks)
target_link_libraries(benchmarks PRIVATE myvas::cmdout benchmark::benchmark)