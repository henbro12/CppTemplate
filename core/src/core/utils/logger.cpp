/**
 * SPDX-License-Identifier: MIT
 *
 * @file logger.cpp
 * @brief Logger class for the engine, providing various logging levels.
 * @date 2025-06-27
 *
 * @copyright (C) 2025 Henbro12
 * @author Henrico Brom <henricobrom@gmail.com>
 */
#include "pch.h"

#include "logger.h"

#include <mutex>
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <string>

namespace core {

namespace {

constexpr const char* ANSI_WHITE = "\033[37m";
constexpr const char* ANSI_GREY = "\033[90m";
constexpr const char* ANSI_RESET = "\033[0m";

// Looks the logger up in the spdlog registry, creating it if needed. Creation is serialised so two
// threads asking for the same new logger can't both try to register it (spdlog would throw).
std::shared_ptr<spdlog::logger> getOrCreate(const std::string& name)
{
    if (auto logger = spdlog::get(name)) {
        return logger;
    }

    static std::mutex s_create_mutex;
    const std::scoped_lock lock(s_create_mutex);
    if (auto logger = spdlog::get(name)) {
        return logger;
    }

    // New loggers pick up the registry's global level (see setLogLevel)
    auto logger = spdlog::stdout_color_mt(name);
    logger->set_pattern("[%T] [%^%l%$] %n: %v");
    return logger;
}

} // namespace

void Logger::init()
{
    // Reconfigures loggers that already exist (e.g. created by logging before init) instead of re-creating them
    auto configure = [](const std::string& name, const char* color) {
        auto logger = getOrCreate(name);
        logger->set_level(spdlog::level::trace);
        logger->set_pattern(fmt::format("[%T] [%^%l%$] {}%n: %v{}", color, ANSI_RESET));
    };

    configure("Core", ANSI_GREY);
    configure("App", ANSI_WHITE);

    TEMPLATE_CORE_INFO("Loggers initialized.");
}

void Logger::shutdown()
{
    TEMPLATE_CORE_INFO("Shutting down logger.");

    spdlog::apply_all([](const std::shared_ptr<spdlog::logger>& logger) { logger->flush(); });
    spdlog::shutdown();
}

void Logger::setLogLevel(spdlog::level::level_enum level)
{
    // Applies to all registered loggers and becomes the default for loggers created later
    spdlog::set_level(level);

    TEMPLATE_CORE_INFO("Log level set to {}", spdlog::level::to_string_view(level));
}

void Logger::setLogLevel(std::string_view name, spdlog::level::level_enum level)
{
    if (auto logger = spdlog::get(std::string(name))) {
        logger->set_level(level);
        logger->info("Log level set to {}", spdlog::level::to_string_view(level));
    }
    else {
        TEMPLATE_CORE_ERROR("Logger '{}' not found", name);
    }
}

std::shared_ptr<spdlog::logger> Logger::get(const std::string& name)
{
    return getOrCreate(name);
}

} // namespace core
