/**
 * SPDX-License-Identifier: MIT
 *
 * @file test_profiler.cpp
 * @brief Unit tests for the Profiler utilities.
 * @date 2026-10-25
 *
 * @copyright (C) 2025 Henbro12
 * @author Henrico Brom <henricobrom@gmail.com>
 */

#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <memory>
#include <stdexcept>
#include <thread>
#include <vector>

#include "core/utils/profiler.h"

namespace {

constexpr int MOVE_ONLY_VALUE = 7;

} // namespace

TEST_CASE("Profiler start/end timers", "[profiler]")
{
    using core::utils::Profiler;

    CHECK(Profiler::elapsedUs("unknown") == 0);

    Profiler::startTimer("sleep");
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    CHECK(Profiler::elapsedUs("sleep") >= 1000);

    REQUIRE_NOTHROW(Profiler::endTimer("sleep"));
    CHECK(Profiler::elapsedUs("sleep") == 0);

    // Ending a timer that was never started only warns
    REQUIRE_NOTHROW(Profiler::endTimer("never-started"));
}

TEST_CASE("Profiler::exprTimer forwards results and exceptions", "[profiler]")
{
    using core::utils::Profiler;

    CHECK(Profiler::exprTimer("value", [] { return 42; }) == 42);

    int calls = 0;
    Profiler::exprTimer("void", [&] { ++calls; });
    CHECK(calls == 1);

    CHECK_THROWS_AS(Profiler::exprTimer("throws", []() -> int { throw std::runtime_error("boom"); }), std::runtime_error);
}

TEST_CASE("Profiler::exprTimer keeps references and supports move-only results", "[profiler]")
{
    using core::utils::Profiler;

    int target = 1;
    const int& ref = Profiler::exprTimer("ref", [&]() -> int& { return target; });
    CHECK(&ref == &target);

    const auto ptr = Profiler::exprTimer("move-only", [] { return std::make_unique<int>(MOVE_ONLY_VALUE); });
    REQUIRE(ptr != nullptr);
    CHECK(*ptr == MOVE_ONLY_VALUE);
}

TEST_CASE("Profiler macros evaluate their expression whether enabled or not", "[profiler]")
{
    int calls = 0;
    auto f = [&] {
        return ++calls;
    };

    CHECK(TEMPLATE_PROFILE_CALL("macro", f()) == 1);
    TEMPLATE_PROFILE_CALL_V("macro_v", f());
    CHECK(calls == 2);

    // Reference results behave the same with the profiler enabled (Debug) and disabled (Release)
    constexpr int new_value = 20;
    std::vector<int> values{1, 2, 3};
    // cppcheck doesn't see that the macro yields a reference into `values`
    // cppcheck-suppress constStatement
    TEMPLATE_PROFILE_CALL("element", values[1]) = new_value;
    CHECK(values[1] == new_value);

    {
        TEMPLATE_PROFILE_SCOPE("scope");
    }

    TEMPLATE_PROFILE_START("macro_timer");
    TEMPLATE_PROFILE_END("macro_timer");
}
