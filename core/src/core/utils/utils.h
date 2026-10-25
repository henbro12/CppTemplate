/**
 * SPDX-License-Identifier: MIT
 *
 * @file utils.h
 * @brief Utility macros for the project.
 * @date 2025-07-06
 *
 * @copyright (C) 2025 Henbro12
 * @author Henrico Brom <henricobrom@gmail.com>
 */

#pragma once

#include "logger.h"

// Portable debugger break: MSVC/clang-cl, Clang/AppleClang, then POSIX (GCC/MinGW), else abort
#ifdef _MSC_VER
#define TEMPLATE_DEBUGBREAK() __debugbreak()
#endif

#ifdef __has_builtin
#if __has_builtin(__builtin_debugtrap) && !defined(TEMPLATE_DEBUGBREAK)
#define TEMPLATE_DEBUGBREAK() __builtin_debugtrap()
#endif
#endif

#ifndef TEMPLATE_DEBUGBREAK
#include <csignal>
#ifdef SIGTRAP
#define TEMPLATE_DEBUGBREAK() std::raise(SIGTRAP)
#else
#include <cstdlib>
#define TEMPLATE_DEBUGBREAK() std::abort()
#endif
#endif

// TEMPLATE_ASSERT(cond, log_macro, fmt, args...): checked in Debug only, `cond` is not evaluated otherwise.
// TEMPLATE_VERIFY(cond, log_macro, fmt, args...): checked in Debug only, but `cond` is always evaluated.
#ifdef TEMPLATE_ENABLE_ASSERTS
#define TEMPLATE_ASSERT(x, log_macro, ...)                                                                                                 \
    do {                                                                                                                                   \
        if (!(x)) {                                                                                                                        \
            log_macro("Assertion Failed: {}", fmt::format(__VA_ARGS__));                                                                   \
            TEMPLATE_DEBUGBREAK();                                                                                                         \
        }                                                                                                                                  \
    } while (0)

#define TEMPLATE_VERIFY(x, log_macro, ...)                                                                                                 \
    do {                                                                                                                                   \
        if (!(x)) {                                                                                                                        \
            log_macro("Verification Failed: {}", fmt::format(__VA_ARGS__));                                                                \
            TEMPLATE_DEBUGBREAK();                                                                                                         \
        }                                                                                                                                  \
    } while (0)

#else
#define TEMPLATE_ASSERT(x, log_macro, ...) ((void)0)
#define TEMPLATE_VERIFY(x, log_macro, ...) ((void)(x))
#endif
