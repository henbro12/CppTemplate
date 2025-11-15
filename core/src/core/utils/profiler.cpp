/**
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file profiler.cpp
 * @brief Implementation of lightweight profiler utilities.
 * @date 2025-10-08
 */

#include "profiler.h"

#include <mutex>
#include <string>
#include <unordered_map>

#include "logger.h"


namespace core::utils {

std::unordered_map<std::string, Profiler::clock::time_point> Profiler::s_starts{};
std::mutex Profiler::s_mutex{};

void Profiler::startTimer(std::string_view name)
{
    const std::lock_guard lk(s_mutex);
    s_starts[std::string{name}] = clock::now();
}

void Profiler::endTimer(std::string_view name)
{
    const auto key = std::string{name};
    Profiler::clock::time_point start{};
    {
        const std::lock_guard lk(s_mutex);
        auto it = s_starts.find(key);
        if (it == s_starts.end()) {
            TB_CORE_WARN("Profiler end called for '{}' without a matching start", name);
            return;
        }
        start = it->second;
        s_starts.erase(it);
    }
    const auto us = std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - start).count();
    TB_CORE_TRACE("Profiler '{}' took {} us", name, us);
}

long long Profiler::elapsedUs(std::string_view name)
{
    const std::lock_guard lk(s_mutex);
    auto it = s_starts.find(std::string{name});
    if (it == s_starts.end()) {
        return 0;
    }
    return std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - it->second).count();
}


ScopedTimer::ScopedTimer(std::string_view name) : m_name{name}, m_start{std::chrono::steady_clock::now()} {}

ScopedTimer::~ScopedTimer()
{
    const auto us = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - m_start).count();
    TB_CORE_TRACE("Profiler '{}' took {} us", m_name, us);
}

} // namespace core::utils
