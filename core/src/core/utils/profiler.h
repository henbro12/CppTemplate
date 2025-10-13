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

#include "core/utils/logger.h"

namespace core::utils {

class Profiler {
  public:
    using clock = std::chrono::steady_clock;

    static void startTimer(std::string_view name);
    static void endTimer(std::string_view name);
    static long long elapsed_us(std::string_view name);

    template <class F>
    static decltype(auto) exprTimer(std::string_view name, F&& f);

  private:
    static std::unordered_map<std::string, std::chrono::steady_clock::time_point> s_starts;
    static std::mutex s_mutex;
};

// RAII helper
class ScopedTimer {
  public:
    explicit ScopedTimer(std::string_view name);
    ~ScopedTimer();

    ScopedTimer(const ScopedTimer&) = delete;
    ScopedTimer& operator=(const ScopedTimer&) = delete;

  private:
    std::string m_name;
    std::chrono::steady_clock::time_point m_start;
};

// Template implementation
template <class F>
decltype(auto) Profiler::exprTimer(std::string_view name, F&& f)
{
    const auto start = clock::now();
    try {
        if constexpr (std::is_void_v<std::invoke_result_t<F&>>) {
            std::invoke(f);
            const auto us = std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - start).count();
            TB_CORE_TRACE("Profiler '{}' took {} us", name, us);
            return;
        }
        else {
            auto ret = std::invoke(f);
            const auto us = std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - start).count();
            TB_CORE_TRACE("Profiler '{}' took {} us", name, us);
            return ret;
        }
    }
    catch (...) {
        const auto us = std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - start).count();
        TB_CORE_TRACE("Profiler '{}' threw after {} us", name, us);
        throw;
    }
}

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
#define PROFILE_START(name) ::core::utils::Profiler::startTimer(name)
#define PROFILE_END(name) ::core::utils::Profiler::endTimer(name)
#define PROFILE_CALL(name, expr) ::core::utils::Profiler::exprTimer((name), [&]() -> decltype(expr) { return (expr); })
#define PROFILE_CALL_V(name, expr) ::core::utils::Profiler::exprTimer((name), [&]() { (expr); })
#define PROFILE_SCOPE(name) ::core::utils::ScopedTimer CONCAT_SCOPE_TIMER(__LINE__)(name)

// helper to generate unique variable names for the scope timer
#define CONCAT_SCOPE_TIMER_INTERNAL(line) scope_timer_##line
#define CONCAT_SCOPE_TIMER(line) CONCAT_SCOPE_TIMER_INTERNAL(line)
#else
#define PROFILE_START(name) ((void)0)
#define PROFILE_END(name) ((void)0)
#define PROFILE_CALL(name) ((void)0)
#define PROFILE_CALL_V(name) ((void)0)
#define PROFILE_SCOPE(name) ((void)0)
#endif
