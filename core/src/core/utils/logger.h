/**
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file logger.h
 * @brief Basic logger class for the project, providing various logging levels.
 * @date 2025-06-27
 *
 * @copyright (C) 2025 Henbro12
 * @author Henrico Brom <henricobrom@gmail.com>
 */

#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <string>
#include <unordered_map>

namespace core
{
    class Logger
    {
      public:
        static void init();
        static void shutdown();

        static void setLogLevel(spdlog::level::level_enum level);
        static void setLogLevel(const std::string_view name, spdlog::level::level_enum level);

        // Returns a shared_ptr to a logger; if it does not exist it will be created.
        static std::shared_ptr<spdlog::logger> get(const std::string& name);

      private:
        static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> s_loggers;
    };

} // namespace core

// Define the logging macros for different levels
#define TB_CORE_TRACE(...) ::core::Logger::get("Core")->trace(__VA_ARGS__)
#define TB_CORE_DEBUG(...) ::core::Logger::get("Core")->debug(__VA_ARGS__)
#define TB_CORE_INFO(...) ::core::Logger::get("Core")->info(__VA_ARGS__)
#define TB_CORE_WARN(...) ::core::Logger::get("Core")->warn(__VA_ARGS__)
#define TB_CORE_ERROR(...) ::core::Logger::get("Core")->error(__VA_ARGS__)
#define TB_CORE_CRITICAL(...) ::core::Logger::get("Core")->critical(__VA_ARGS__)

#define TB_APP_TRACE(...) ::core::Logger::get("App")->trace(__VA_ARGS__)
#define TB_APP_DEBUG(...) ::core::Logger::get("App")->debug(__VA_ARGS__)
#define TB_APP_INFO(...) ::core::Logger::get("App")->info(__VA_ARGS__)
#define TB_APP_WARN(...) ::core::Logger::get("App")->warn(__VA_ARGS__)
#define TB_APP_ERROR(...) ::core::Logger::get("App")->error(__VA_ARGS__)
#define TB_APP_CRITICAL(...) ::core::Logger::get("App")->critical(__VA_ARGS__)
