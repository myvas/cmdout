/**
 * Copyright (c) 2023 Myvas Foundation
 * SPDX-License-Identifier: MIT
 *
 * @file  test_myvas_system.cpp
 * @brief Unit test for function myvas::system().
 */
#include <catch2/catch_test_macros.hpp>

#include <cmdout/experimental/std_system_out.hpp>

TEST_CASE("std_system_out")
{
    SECTION("with exit 0")
    {
        auto result = myvas::experimental::std_system_out("exit 0");
        REQUIRE(std::get<0>(result) == EXIT_SUCCESS);
    }

    SECTION("with exit 1")
    {
        auto result = myvas::experimental::std_system_out("exit 1");
        REQUIRE(std::get<0>(result) == EXIT_FAILURE);
    }

    SECTION("stdout only, without stderr")
    {
        auto result = myvas::experimental::std_system_out("ls ~/");
        REQUIRE(std::get<0>(result) == EXIT_SUCCESS);
    }

    SECTION("with stderr")
    {
        auto result = myvas::experimental::std_system_out("ls not-exist 2>&1");
        REQUIRE(std::get<0>(result) == ENOENT);
    }

    SECTION("with default arguments to >1ms call")
    {
        auto result = myvas::experimental::std_system_out("ls / -l");
        REQUIRE(std::get<0>(result) == EXIT_SUCCESS);
    }
}
