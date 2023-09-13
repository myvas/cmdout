/**
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 Myvas Foundation
 *
 * @file  test_std_system.cpp
 * @brief Unit test for function std::system().
 */
#include <catch2/catch_test_macros.hpp>

TEST_CASE("std::system")
{
    SECTION("NULL")
    {
        auto result = std::system(NULL);
        REQUIRE(result == EXIT_FAILURE);
    }

    SECTION("exit 0")
    {
        auto result = std::system("exit 0");
        REQUIRE(result == EXIT_SUCCESS);
    }

    SECTION("exit 1")
    {
        auto result = std::system("exit 1");
        REQUIRE(WEXITSTATUS(result) == EXIT_FAILURE);
    }

    SECTION("stdout only, without stderr")
    {
        auto result = std::system("ls ~/");
        REQUIRE(result == EXIT_SUCCESS);
    }

    SECTION("with stderr")
    {
        auto result = std::system("ls not-exist 2>&1");
        REQUIRE(WEXITSTATUS(result) == ENOENT);
    }

    SECTION("with default arguments to >1ms call")
    {
        auto result = std::system("ls / -l");
        REQUIRE(result == EXIT_SUCCESS);
    }
}
