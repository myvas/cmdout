# Copyright (c) 2023 Myvas Foundation
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.19)
project(cmdout_examples CXX)

if(NOT TARGET cmdout)
    # Stand-alone build
    find_package(cmdout REQUIRED)
endif()

add_executable(example_std_system)
target_sources(example_std_system PRIVATE "example_std_system.cpp")
target_link_libraries(example_std_system PRIVATE myvas::cmdout)

add_executable(example_myvas_system)
target_sources(example_myvas_system PRIVATE "example_myvas_system.cpp")
target_link_libraries(example_myvas_system PRIVATE myvas::cmdout)

add_executable(example_myvas_cmdout)
target_sources(example_myvas_cmdout PRIVATE "example_myvas_cmdout.cpp")
target_link_libraries(example_myvas_cmdout PRIVATE myvas::cmdout)