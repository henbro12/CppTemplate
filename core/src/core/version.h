/**
 * SPDX-License-Identifier: MIT
 *
 * @file version.h
 * @brief Project version and build identification (for --version, logs and crash reports).
 * @date 2026-11-08
 *
 * The version comes from project(VERSION ...) in the top-level CMakeLists.txt, the git details are
 * refreshed on every build. Without git (e.g. a source tarball) they read "unknown".
 *
 * @copyright (C) 2025 Henbro12
 * @author Henrico Brom <henricobrom@gmail.com>
 */

#pragma once

#include <string>
#include <string_view>

namespace core {

struct VersionInfo {
    std::string_view project; // e.g. "CppTemplate"
    std::string_view version; // e.g. "0.1.0"
    int version_major{};
    int version_minor{};
    int version_patch{};
    std::string_view git_describe; // e.g. "v0.1.0-3-g1a2b3c4d-dirty"
    std::string_view git_commit;   // e.g. "1a2b3c4d"
    bool git_dirty{};              // uncommitted changes when built
    std::string_view build_config; // e.g. "Debug", "Release"
};

const VersionInfo& versionInfo();

// One-line summary, e.g. "CppTemplate 0.1.0 (v0.1.0-3-g1a2b3c4d-dirty, Debug)"
std::string versionString();

} // namespace core
