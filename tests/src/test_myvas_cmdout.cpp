/**
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 Myvas Foundation
 *
 * @file  test_myvas_cmdout.cpp
 * @brief Unit test for class myvas::cmdout.
 */

#include <cmdout/cmdout.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("myvas::cmdout")
{
    SECTION("with default arguments")
    {
        auto result = myvas::cmdout().exec();
        REQUIRE(result.status() == EXIT_SUCCESS);
    }

    SECTION("exit 0")
    {
        auto result = myvas::cmdout("exit 0").exec();
        REQUIRE(result.status() == EXIT_SUCCESS);
    }

    SECTION("exit 1")
    {
        auto result = myvas::cmdout("exit 1").exec();
        REQUIRE(result.status() == EXIT_FAILURE);
    }

    SECTION("stdout only, without stderr")
    {
        auto result = myvas::cmdout("ls ~/").exec();
        REQUIRE(result.status() == EXIT_SUCCESS);
    }

    SECTION("with stderr")
    {
        auto result = myvas::cmdout("ls not-exist 2>&1").exec();
        REQUIRE(result.status() == ENOENT);
    }

    SECTION("with default arguments to >1ms call")
    {
        auto result = myvas::cmdout("ls / -l").exec();
        REQUIRE(result.status() == EXIT_SUCCESS);
    }

    SECTION("with timeout 0 to >1ms call")
    {
        auto result = myvas::cmdout("ls / -l", 0).exec();
        REQUIRE(result.status() == EXIT_SUCCESS);

        auto result2 = myvas::cmdout("ls / -l", std::chrono::milliseconds(0)).exec();
        REQUIRE(result2.status() == EXIT_SUCCESS);
    }

    SECTION("with timeout 1ms to >2ms call")
    {
        auto result = myvas::cmdout("ls / -l", 1).exec();
        REQUIRE(result.status() == ETIME);

        auto result2 = myvas::cmdout("ls / -l", std::chrono::milliseconds(1)).exec();
        REQUIRE(result2.status() == ETIME);
    }

    SECTION("with timeout 9ms to <4ms call")
    {
        auto result = myvas::cmdout("ls / -l", 10000).exec();
        REQUIRE(result.status() == EXIT_SUCCESS);

        auto result2 = myvas::cmdout("ls / -l", std::chrono::milliseconds(10000)).exec();
        REQUIRE(result2.status() == EXIT_SUCCESS);
    }

    SECTION("with stderr and timeout 9ms")
    {
        auto result = myvas::cmdout("ls not-exist 2>&1", 10000).exec();
        REQUIRE(result.status() == ENOENT);

        auto result2 = myvas::cmdout("ls not-exist 2>&1", std::chrono::milliseconds(10000)).exec();
        REQUIRE(result2.status() == ENOENT);
    }
}
