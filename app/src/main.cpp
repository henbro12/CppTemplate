/**
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file main.cpp
 * @brief Initial starting point for the application.
 * @date 2025-06-27
 *
 * @copyright (C) 2025 Henbro12
 * @author Henrico Brom <henricobrom@gmail.com>
 */

#include <spdlog/common.h>

#include "core/template/template.h"
#include "core_inc.h"

int main(int argc, char** argv)
{
    (void)argc; // Unused parameter
    (void)argv; // Unused parameter

    core::Logger::init();
    core::Logger::setLogLevel(spdlog::level::trace);

    TB_APP_INFO("Template Application initialized successfully.");

    core::boot();

    core::Logger::shutdown();

    return 0;
}
