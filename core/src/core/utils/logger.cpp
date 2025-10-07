/**
 * SPDX-License-Identifier: Apache-2.0
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

#include <spdlog/sinks/stdout_color_sinks.h>

#include "utils.h"

namespace core
{
    constexpr const char* ANSI_WHITE = "\033[37m";
    constexpr const char* ANSI_GREY = "\033[90m";
    constexpr const char* ANSI_RESET = "\033[0m";

    std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> Logger::s_loggers;

    void Logger::init()
    {
        auto create = [](const std::string& name, const char* color)
        {
            auto logger = spdlog::stdout_color_mt(name);
            logger->set_level(spdlog::level::trace);
            logger->set_pattern(fmt::format("[%T] [%^%l%$] {}%n: %v{}", color, ANSI_RESET));
            s_loggers[name] = logger;
        };

        create("Core", ANSI_GREY);
        create("App", ANSI_GREY);

        TB_CORE_INFO("Loggers initialized.");
    }

    void Logger::shutdown()
    {
        TB_CORE_INFO("Shutting down logger.");

        for (auto& [name, logger] : s_loggers)
        {
            logger->flush();
            spdlog::drop(name);
        }
        spdlog::shutdown();
    }

    void Logger::setLogLevel(spdlog::level::level_enum level)
    {
        for (auto& [name, logger] : s_loggers)
        {
            logger->set_level(level);
        }

        TB_CORE_INFO("Log level set to {}", spdlog::level::to_string_view(level));
    }

    void Logger::setLogLevel(std::string_view name, spdlog::level::level_enum level)
    {
        auto it = s_loggers.find(std::string(name));
        if (it != s_loggers.end())
        {
            it->second->set_level(level);
            it->second->info("Log level set to {}", spdlog::level::to_string_view(level));
        }
        else
        {
            TB_CORE_ERROR("Logger '{}' not found", name);
        }
    }

    std::shared_ptr<spdlog::logger> Logger::get(const std::string& name)
    {
        auto it = s_loggers.find(name);
        if (it != s_loggers.end()) return it->second;

        TB_CORE_ERROR("Logger '{}' not found", name);
        return nullptr;
    }

} // namespace core