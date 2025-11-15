/**
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file test_logger.cpp
 * @brief
 * @date 2025-06-27
 *
 * @copyright (C) 2025 Henbro12
 * @author Henrico Brom <henricobrom@gmail.com>
 */

#include <catch2/catch_test_macros.hpp>
#include <numbers>

#include "core/utils/logger.h"


TEST_CASE("Logger runs without crashing", "[logger]")
{
    core::Logger::init();

    REQUIRE_NOTHROW(TB_CORE_TRACE("Trace step {}", 7));
    REQUIRE_NOTHROW(TB_CORE_TRACE("Debug: {:.2f}", std::numbers::pi));
    REQUIRE_NOTHROW(TB_CORE_TRACE("Info message: {}", 123));
    REQUIRE_NOTHROW(TB_CORE_TRACE("Warning: {}", "check this"));
    REQUIRE_NOTHROW(TB_CORE_TRACE("Error code {}", -1));
    REQUIRE_NOTHROW(TB_CORE_TRACE("Critical error occurred! ({})", -42));

    core::Logger::shutdown();
}
