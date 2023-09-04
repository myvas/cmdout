# Copyright (c) 2023 Myvas Foundation
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.23)
project(cmdout_tests LANGUAGES CXX)

if(NOT TARGET cmdout)
    # Stand-alone build
    find_package(cmdout REQUIRED)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(GTest)
include(Catch2)

#########################
# tests sources
#########################
set(CMDOUT_TESTS_SRCS
    src/test_std_system.cpp
    src/test_myvas_system.cpp
    src/test_myvas_cmdout.cpp
)

# Build target for test
function(cmdout_prepare_test test_target cmdout_lib)
    add_executable(${test_target} ${CMDOUT_TESTS_SRCS})
    cmdout_enable_warnings(${test_target})
    target_link_libraries(${test_target} PRIVATE ${cmdout_lib})
    target_link_libraries(${test_target} PRIVATE Catch2::Catch2WithMain)
    if(CMDOUT_SANITIZE_ADDRESS)
        cmdout_enable_sanitizer(${test_target})
    endif()
    add_test(NAME ${test_target} COMMAND ${test_target})
    set_tests_properties(${test_target} PROPERTIES RUN_SERIAL ON)
endfunction()

###########################
# target tests
###########################
if(CMDOUT_BUILD_TESTS OR CMDOUT_BUILD_ALL)
    cmdout_prepare_test(tests cmdout)
endif()