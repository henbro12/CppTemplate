/**
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file template.cpp
 * @brief Example of a core module.
 * @date 2025-06-27
 *
 * @copyright (C) 2025 Henbro12
 * @author Henrico Brom <henricobrom@gmail.com>
 */

#include "core/template/template.h"

#include "core/utils/logger.h"


namespace core {

void boot()
{
    TB_CORE_INFO("Boot sequence initialized.");
}

} // namespace core
