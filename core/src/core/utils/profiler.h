/**
 * SPDX-License-Identifier: MIT
 *
 * @file profiler.h
 * @brief Simple static profiler to measure named code sections.
 * @date 2025-10-08
 *
 * Lightweight utility for quick performance comparisons. Use Profiler::startTimer("name")
 * and Profiler::endTimer("name"), the RAII helper ScopedTimer, or the TEMPLATE_PROFILE_* macros.
 *
 * @copyright (C) 2025 Henbro12
 * @author Henrico Brom <henricobrom@gmail.com>
 */

#pragma once

#include <chrono>
#include <functional>
#include <mutex>
#include <string>
#include <string_view>
#include <unordered_map>

#include "core/utils/logger.h"

namespace core::utils {

class Profiler {
  public:
    using clock = std::chrono::steady_clock;

    // Timers are keyed by name only, so the same name must not be timed concurrently from several threads.
    static void startTimer(std::string_view name);
    static void endTimer(std::string_view name);
    static long long elapsedUs(std::string_view name);

    // Times a call to `f` and returns exactly what `f` returns (values, references and move-only types alike).
    template <class F>
    static decltype(auto) exprTimer(std::string_view name, F&& f);

  private:
    static std::unordered_map<std::string, clock::time_point> s_starts;
    static std::mutex s_mutex;
};

// RAII helper: logs the time between construction and destruction, noting when the scope was left by an exception.
class ScopedTimer {
  public:
    explicit ScopedTimer(std::string_view name);
    ~ScopedTimer();

    ScopedTimer(const ScopedTimer&) = delete;
    ScopedTimer& operator=(const ScopedTimer&) = delete;

    ScopedTimer(ScopedTimer&&) = delete;
    ScopedTimer& operator=(ScopedTimer&&) = delete;

  private:
    std::string m_name;
    Profiler::clock::time_point m_start;
    int m_uncaught_exceptions;
};

// Template implementation
template <class F>
decltype(auto) Profiler::exprTimer(std::string_view name, F&& f)
{
    // The timer logs when it goes out of scope, after the result has been produced, so the
    // result can be returned directly and keeps its exact type and value category.
    const ScopedTimer timer{name};
    return std::invoke(std::forward<F>(f));
}

} // namespace core::utils

// Convenience macros, enabled when TEMPLATE_ENABLE_PROFILER is defined. template::core defines it for Debug
// builds of everything that links it; define it yourself to profile other configurations.
#ifdef TEMPLATE_ENABLE_PROFILER
#define TEMPLATE_PROFILE_START(name) ::core::utils::Profiler::startTimer(name)
#define TEMPLATE_PROFILE_END(name) ::core::utils::Profiler::endTimer(name)
#define TEMPLATE_PROFILE_CALL(name, expr) ::core::utils::Profiler::exprTimer((name), [&]() -> decltype(expr) { return (expr); })
#define TEMPLATE_PROFILE_CALL_V(name, expr) ::core::utils::Profiler::exprTimer((name), [&]() { (expr); })
#define TEMPLATE_PROFILE_SCOPE(name) const ::core::utils::ScopedTimer TEMPLATE_PROFILE_CONCAT(template_scope_timer_, __LINE__)(name)

// Helpers to generate a unique variable name for the scope timer
#define TEMPLATE_PROFILE_CONCAT_INNER(a, b) a##b
#define TEMPLATE_PROFILE_CONCAT(a, b) TEMPLATE_PROFILE_CONCAT_INNER(a, b)
#else
// Disabled variants keep the same signatures, and TEMPLATE_PROFILE_CALL/_CALL_V still evaluate `expr`
#define TEMPLATE_PROFILE_START(name) ((void)0)
#define TEMPLATE_PROFILE_END(name) ((void)0)
#define TEMPLATE_PROFILE_CALL(name, expr) (expr)
#define TEMPLATE_PROFILE_CALL_V(name, expr) ((void)(expr))
#define TEMPLATE_PROFILE_SCOPE(name) ((void)0)
#endif
