/**
 * SPDX-License-Identifier: Apache-2.0
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

#ifdef TEMPLATE_ENABLE_ASSERTS
#define TEMPLATE_ASSERT(x, log_macro, ...)                                                                                                 \
    {                                                                                                                                      \
        if (!(x)) {                                                                                                                        \
            log_macro("Assertion Failed: {0}", __VA_ARGS__);                                                                               \
            __debugbreak();                                                                                                                \
        }                                                                                                                                  \
    }

#define TEMPLATE_VERIFY(x, log_macro, ...)                                                                                                 \
    {                                                                                                                                      \
        if (!(x)) {                                                                                                                        \
            log_macro("Verification Failed: {0}", __VA_ARGS__);                                                                            \
            __debugbreak();                                                                                                                \
        }                                                                                                                                  \
    }

#else
#define TEMPLATE_ASSERT(x, log_macro, ...)
#define TEMPLATE_VERIFY(x, log_macro, ...)
#endif
