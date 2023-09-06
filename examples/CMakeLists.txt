# Copyright (c) 2023 Myvas Foundation
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.19)
project(cmdout_examples LANGUAGES CXX)

if(NOT TARGET cmdout)
    # Stand-alone build
    find_package(cmdout REQUIRED)
endif()

add_executable(example_std_system)
target_sources(example_std_system PRIVATE "src/example_std_system.cpp")
cmdout_enable_warnings(example_std_system)

add_executable(example_std_system_out)
target_sources(example_std_system_out PRIVATE "src/example_std_system_out.cpp")
target_link_libraries(example_std_system_out PRIVATE cmdout)
cmdout_enable_warnings(example_std_system_out)

add_executable(example_myvas_system)
target_sources(example_myvas_system PRIVATE "src/example_myvas_system.cpp")
target_link_libraries(example_myvas_system PRIVATE cmdout)
cmdout_enable_warnings(example_myvas_system)

add_executable(example_myvas_cmdout)
target_sources(example_myvas_cmdout PRIVATE "src/example_myvas_cmdout.cpp")
target_link_libraries(example_myvas_cmdout PRIVATE cmdout)
cmdout_enable_warnings(example_myvas_cmdout)
