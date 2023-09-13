/**
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 Myvas Foundation
 *
 * @file  test_myvas_system.cpp
 * @brief Unit test for function myvas::system().
 */

#include <cmdout/cmdout.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("myvas::system")
{
    SECTION("with exit 0")
    {
        auto result = myvas::system("exit 0");
        REQUIRE(result.status() == EXIT_SUCCESS);
    }

    SECTION("with exit 1")
    {
        auto result = myvas::system("exit 1");
        REQUIRE(result.status() == EXIT_FAILURE);
    }

    SECTION("stdout only, without stderr")
    {
        auto result = myvas::system("ls ~/");
        REQUIRE(result.status() == EXIT_SUCCESS);
    }

    SECTION("with stderr")
    {
        auto result = myvas::system("ls not-exist 2>&1");
        REQUIRE(result.status() == ENOENT);
    }

    SECTION("with default arguments to >1ms call")
    {
        auto result = myvas::system("ls / -l");
        REQUIRE(result.status() == EXIT_SUCCESS);
    }

    SECTION("with timeout 0 to >1ms call")
    {
        auto result = myvas::system_timeout("ls / -l", std::chrono::milliseconds(0));
        REQUIRE(result.status() == EXIT_SUCCESS);

        auto result2 = myvas::system_timeout_ms("ls / -l", 0);
        REQUIRE(result2.status() == EXIT_SUCCESS);
    }

    SECTION("with timeout 1ms to >2ms call")
    {
        auto result = myvas::system_timeout("ls / -l", std::chrono::milliseconds(1));
        REQUIRE(result.status() == ETIME);

        auto result2 = myvas::system_timeout_ms("ls / -l", 1);
        REQUIRE(result2.status() == ETIME);
    }

    SECTION("with timeout 9ms to <4ms call")
    {
        auto result = myvas::system_timeout("ls / -l", std::chrono::milliseconds(10000));
        REQUIRE(result.status() == EXIT_SUCCESS);

        auto result2 = myvas::system_timeout_ms("ls / -l", 10000);
        REQUIRE(result2.status() == EXIT_SUCCESS);
    }

    SECTION("with stderr and timeout 9ms")
    {
        auto result = myvas::system_timeout("ls not-exist 2>&1", std::chrono::milliseconds(10000));
        REQUIRE(result.status() == ENOENT);

        auto result2 = myvas::system_timeout_ms("ls not-exist 2>&1", 10000);
        REQUIRE(result2.status() == ENOENT);
    }
}
