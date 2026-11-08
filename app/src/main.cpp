/**
 * SPDX-License-Identifier: MIT
 *
 * @file main.cpp
 * @brief Initial starting point for the application.
 * @date 2025-06-27
 *
 * @copyright (C) 2025 Henbro12
 * @author Henrico Brom <henricobrom@gmail.com>
 */

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <span>
#include <spdlog/common.h>
#include <string_view>

#include "core_inc.h"

namespace {

int run(std::span<char*> args)
{
    for (const std::string_view arg : args.subspan(1)) {
        if (arg == "--version") {
            std::cout << core::versionString() << '\n';
            return EXIT_SUCCESS;
        }
        if (arg == "--help") {
            std::cout << "Usage: " << core::versionInfo().project << " [--version] [--help]\n";
            return EXIT_SUCCESS;
        }
    }

    core::Logger::init();
    core::Logger::setLogLevel(spdlog::level::trace);

    TEMPLATE_APP_INFO("{} starting", core::versionString());

    core::boot();

    core::Logger::shutdown();

    return EXIT_SUCCESS;
}

} // namespace

int main(int argc, char** argv)
{
    try {
        return run(std::span(argv, static_cast<std::size_t>(argc)));
    }
    catch (const std::exception& e) {
        // C stdio here: it doesn't throw, unlike iostreams
        (void)std::fputs("Fatal error: ", stderr);
        (void)std::fputs(e.what(), stderr);
        (void)std::fputs("\n", stderr);
    }
    catch (...) {
        (void)std::fputs("Fatal error: unknown exception\n", stderr);
    }
    return EXIT_FAILURE;
}
