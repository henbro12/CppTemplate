/**
 * SPDX-License-Identifier: MIT
 *
 * @file test_version.cpp
 * @brief Unit tests for the version and build info.
 * @date 2026-11-08
 *
 * @copyright (C) 2025 Henbro12
 * @author Henrico Brom <henricobrom@gmail.com>
 */

#include <catch2/catch_test_macros.hpp>
#include <spdlog/fmt/fmt.h>
#include <string>

#include "core/version.h"


TEST_CASE("Version info matches project(VERSION) from CMake", "[version]")
{
    const auto& info = core::versionInfo();

    CHECK(info.project == TEMPLATE_EXPECTED_PROJECT_NAME);
    CHECK(info.version == TEMPLATE_EXPECTED_VERSION);
    CHECK(fmt::format("{}.{}.{}", info.version_major, info.version_minor, info.version_patch) == info.version);
}

TEST_CASE("Version info includes git and build details", "[version]")
{
    const auto& info = core::versionInfo();

    // "unknown" when built without git, otherwise a commit hash
    CHECK_FALSE(info.git_commit.empty());
    CHECK_FALSE(info.git_describe.empty());
    CHECK(info.git_dirty == info.git_describe.ends_with("-dirty"));

    const std::string summary = core::versionString();
    CHECK(summary.starts_with(fmt::format("{} {}", info.project, info.version)));
    CHECK(summary.find(info.git_describe) != std::string::npos);
}
