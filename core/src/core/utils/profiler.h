/**
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file profiler.h
 * @brief Simple static profiler to measure named code sections.
 * @date 2025-10-08
 *
 * Lightweight utility for quick performance comparisons. Use Profiler::start("name")
 * and Profiler::end("name") or the RAII helper ScopedTimer.
 */

#pragma once

#include <chrono>
#include <string>

namespace core::utils
{
    class Profiler
    {
      public:
        static void start(std::string_view name);
        static void end(std::string_view name);

        // Convenience: returns elapsed microseconds without logging
        static long long elapsed_us(std::string_view name);
    };

    // RAII helper
    class ScopedTimer
    {
      public:
        explicit ScopedTimer(std::string_view name);
        ~ScopedTimer();

        ScopedTimer(const ScopedTimer&) = delete;
        ScopedTimer& operator=(const ScopedTimer&) = delete;

      private:
        std::string m_name;
        std::chrono::steady_clock::time_point m_start;
    };

} // namespace core::utils

// Convenience macros: enable by defining ENABLE_PROFILER (e.g., in Debug builds)
#if !defined(ENABLE_PROFILER)
#if defined(_DEBUG) || (defined(DEBUG) && DEBUG)
#define ENABLE_PROFILER 1
#else
#define ENABLE_PROFILER 0
#endif
#endif

#if ENABLE_PROFILER
#define PROFILE_START(name) ::core::utils::Profiler::start(name)
#define PROFILE_END(name) ::core::utils::Profiler::end(name)
#define PROFILE_SCOPE(name) ::core::utils::ScopedTimer CONCAT_SCOPE_TIMER(__LINE__)(name)

// helper to generate unique variable names for the scope timer
#define CONCAT_SCOPE_TIMER_INTERNAL(line) scope_timer_##line
#define CONCAT_SCOPE_TIMER(line) CONCAT_SCOPE_TIMER_INTERNAL(line)
#else
#define PROFILE_START(name) ((void)0)
#define PROFILE_END(name) ((void)0)
#define PROFILE_SCOPE(name) ((void)0)
#endif
