/**
 * SPDX-License-Identifier: MIT
 *
 * @file test_utils.cpp
 * @brief Unit tests for the assert/verify macros.
 * @date 2026-10-25
 *
 * @copyright (C) 2025 Henbro12
 * @author Henrico Brom <henricobrom@gmail.com>
 */

#include <catch2/catch_test_macros.hpp>

#include "core/utils/utils.h"

namespace {

// Runtime value so the conditions below aren't constant expressions (MSVC C4127 under /W4 /WX)
int runtimeValue()
{
    return 3;
}

} // namespace


TEST_CASE("Assert macros compile and pass on true conditions", "[utils]")
{
    const int value = runtimeValue();

    // Must be usable as a single statement, e.g. in an unbraced if/else
    // NOLINTBEGIN(readability-braces-around-statements)
    if (value > 0)
        TEMPLATE_ASSERT(value == 3, TEMPLATE_CORE_ERROR, "value was {}", value);
    else
        TEMPLATE_ASSERT(false, TEMPLATE_CORE_ERROR, "unreachable");
    // NOLINTEND(readability-braces-around-statements)

    int evaluations = 0;
    TEMPLATE_VERIFY(++evaluations == 1, TEMPLATE_CORE_ERROR, "evaluated {} times", evaluations);
    CHECK(evaluations == 1);
}
