# SPDX-License-Identifier: MIT
# Copyright (c) 2023 Myvas Foundation

cmake_minimum_required(VERSION 3.20)
project(cmdout_tests LANGUAGES CXX)

if(NOT TARGET cmdout)
    # Stand-alone build
    find_package(cmdout REQUIRED)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
#include(GTest)
include(Catch2)
include(TestUtils)

if(CMDOUT_BUILD_TESTING)
    cmdout_add_catch2(test_std_system src/test_std_system.cpp)
    cmdout_add_catch2(test_std_system_out src/test_std_system_out.cpp DEPENDS cmdout)
    cmdout_add_catch2(test_myvas_system src/test_myvas_system.cpp DEPENDS cmdout)
    cmdout_add_catch2(test_myvas_cmdout src/test_myvas_cmdout.cpp DEPENDS cmdout)
endif()
