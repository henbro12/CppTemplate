/**
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file profiler.cpp
 * @brief Implementation of lightweight profiler utilities.
 * @date 2025-10-08
 */

#include "profiler.h"

#include <mutex>
#include <unordered_map>

#include "logger.h"


namespace core::utils
{
    using clock = std::chrono::steady_clock;

    static std::unordered_map<std::string, clock::time_point> s_starts;
    static std::mutex s_mutex;

    void Profiler::start(std::string_view name)
    {
        std::lock_guard lock(s_mutex);
        s_starts[std::string(name)] = clock::now();
    }

    void Profiler::end(std::string_view name)
    {
        std::lock_guard lock(s_mutex);
        auto it = s_starts.find(std::string(name));
        if (it == s_starts.end())
        {
            TB_CORE_WARN("Profiler end called for '{}' without a matching start", name);
            return;
        }

        auto dur = std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - it->second).count();
        s_starts.erase(it);

        TB_CORE_TRACE("Profiler '{}' took {} us", name, dur);
    }

    long long Profiler::elapsed_us(std::string_view name)
    {
        std::lock_guard lock(s_mutex);
        auto it = s_starts.find(std::string(name));
        if (it == s_starts.end()) return -1;
        return std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - it->second).count();
    }

    ScopedTimer::ScopedTimer(std::string_view name) : m_name(name), m_start(clock::now()) {}

    ScopedTimer::~ScopedTimer()
    {
        auto dur = std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - m_start).count();
        TB_CORE_TRACE("ScopedTimer '{}' took {} us", m_name, dur);
    }

} // namespace core::utils
