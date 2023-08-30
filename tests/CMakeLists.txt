# Copyright (c) 2023 Myvas Foundation
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.19)
project(cmdout_tests CXX)

if(NOT TARGET cmdout)
    # Stand-alone build
    find_package(cmdout REQUIRED)
endif()

include(../cmake/utils.cmake)

find_package(Catch2 3 QUIET)
if(Catch2_FOUND)
    message(STATUS "Packaged version of Catch will be used.")
else()
    message(STATUS "Bundled version of Catch will be downloaded and used.")
    include(FetchContent)
    #FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v3.3.2)
    FetchContent_Declare(Catch2 URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.3.2.tar.gz)
    FetchContent_MakeAvailable(Catch2)
endif()

#########################
# tests sources
#########################
set(CMDOUT_TESTS_SRCS
    test_std_system.cpp
    test_myvas_system.cpp
    test_myvas_cmdout.cpp
)

enable_testing()

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
    cmdout_prepare_test(tests myvas::cmdout)
endif()