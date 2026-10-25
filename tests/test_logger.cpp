/**
 * SPDX-License-Identifier: MIT
 *
 * @file test_logger.cpp
 * @brief Unit tests for the Logger wrapper.
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

    REQUIRE_NOTHROW(TEMPLATE_CORE_TRACE("Trace step {}", 7));
    REQUIRE_NOTHROW(TEMPLATE_CORE_DEBUG("Debug: {:.2f}", std::numbers::pi));
    REQUIRE_NOTHROW(TEMPLATE_CORE_INFO("Info message: {}", 123));
    REQUIRE_NOTHROW(TEMPLATE_CORE_WARN("Warning: {}", "check this"));
    REQUIRE_NOTHROW(TEMPLATE_CORE_ERROR("Error code {}", -1));
    REQUIRE_NOTHROW(TEMPLATE_CORE_CRITICAL("Critical error occurred! ({})", -42));
    REQUIRE_NOTHROW(TEMPLATE_APP_INFO("App message: {}", "hello"));

    core::Logger::shutdown();
}

TEST_CASE("Logger tolerates logging before init and repeated init", "[logger]")
{
    REQUIRE_NOTHROW(TEMPLATE_CORE_INFO("Logged before init"));
    REQUIRE_NOTHROW(core::Logger::init());
    REQUIRE_NOTHROW(core::Logger::init());

    core::Logger::shutdown();
}

TEST_CASE("Logger levels can be changed globally and per logger", "[logger]")
{
    core::Logger::init();

    core::Logger::setLogLevel(spdlog::level::warn);
    CHECK(core::Logger::get("Core")->level() == spdlog::level::warn);
    CHECK(core::Logger::get("App")->level() == spdlog::level::warn);

    core::Logger::setLogLevel("App", spdlog::level::err);
    CHECK(core::Logger::get("Core")->level() == spdlog::level::warn);
    CHECK(core::Logger::get("App")->level() == spdlog::level::err);

    REQUIRE_NOTHROW(core::Logger::setLogLevel("DoesNotExist", spdlog::level::info));

    core::Logger::setLogLevel(spdlog::level::info);
    core::Logger::shutdown();
}

TEST_CASE("Logger::get creates unknown loggers on demand", "[logger]")
{
    auto logger = core::Logger::get("OnDemand");
    REQUIRE(logger != nullptr);
    CHECK(logger->name() == "OnDemand");
    CHECK(core::Logger::get("OnDemand") == logger);

    core::Logger::shutdown();
}
