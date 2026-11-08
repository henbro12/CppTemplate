/**
 * SPDX-License-Identifier: MIT
 *
 * @file version.cpp
 * @brief Project version and build identification.
 * @date 2026-11-08
 *
 * @copyright (C) 2025 Henbro12
 * @author Henrico Brom <henricobrom@gmail.com>
 */

#include "core/version.h"

#include <spdlog/fmt/fmt.h>
#include <string>

#include "core/git_info.h" // generated in the build directory

// The TEMPLATE_PROJECT_* and TEMPLATE_BUILD_CONFIG definitions are set on this file only (see core/CMakeLists.txt),
// so changing the version or configuration recompiles just this file.

namespace core {

const VersionInfo& versionInfo()
{
    static const VersionInfo info{
        .project = TEMPLATE_PROJECT_NAME,
        .version = TEMPLATE_PROJECT_VERSION,
        .version_major = TEMPLATE_PROJECT_VERSION_MAJOR,
        .version_minor = TEMPLATE_PROJECT_VERSION_MINOR,
        .version_patch = TEMPLATE_PROJECT_VERSION_PATCH,
        .git_describe = detail::GIT_DESCRIBE,
        .git_commit = detail::GIT_COMMIT,
        .git_dirty = detail::GIT_DIRTY,
        .build_config = TEMPLATE_BUILD_CONFIG,
    };
    return info;
}

std::string versionString()
{
    const auto& info = versionInfo();
    if (info.build_config.empty()) {
        return fmt::format("{} {} ({})", info.project, info.version, info.git_describe);
    }
    return fmt::format("{} {} ({}, {})", info.project, info.version, info.git_describe, info.build_config);
}

} // namespace core
