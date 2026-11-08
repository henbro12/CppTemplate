# SPDX-License-Identifier: MIT
# @copyright (C) 2025 Henbro12

add_library(CppTemplateCore STATIC
  "src/core/utils/logger.cpp"
  "src/core/utils/profiler.cpp"
  "src/core/template/template.cpp"
  "src/core/version.cpp"
)

target_include_directories(CppTemplateCore PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

find_package(spdlog CONFIG REQUIRED)

target_link_libraries(CppTemplateCore PUBLIC
  spdlog::spdlog
)

# CMake force-includes this header into every source of the target; sources don't include it themselves
target_precompile_headers(CppTemplateCore PRIVATE src/pch.h)

template_configure_target(CppTemplateCore)

# Debug-only features, exported to everything that links template::core so they switch on the same way
# with every compiler. Generator expression so this works with multi-config generators too.
target_compile_definitions(CppTemplateCore PUBLIC
  $<$<CONFIG:Debug>:TEMPLATE_ENABLE_ASSERTS>
  $<$<CONFIG:Debug>:TEMPLATE_ENABLE_PROFILER>
)

# ---------- Version and git build info (core/version.h) ----------
set(TEMPLATE_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(TEMPLATE_GIT_INFO_HEADER "${TEMPLATE_GENERATED_DIR}/core/git_info.h")
set(TEMPLATE_GIT_INFO_ARGS
  -DSOURCE_DIR=${PROJECT_SOURCE_DIR}
  -DTEMPLATE_FILE=${PROJECT_SOURCE_DIR}/cmake/git_info.h.in
  -DOUTPUT_FILE=${TEMPLATE_GIT_INFO_HEADER}
  -P ${PROJECT_SOURCE_DIR}/cmake/git_info.cmake
)

# Generate once now so the header exists for IDEs and clang-tidy, then refresh it before every build
execute_process(COMMAND ${CMAKE_COMMAND} ${TEMPLATE_GIT_INFO_ARGS})
add_custom_target(template_git_info
  COMMAND ${CMAKE_COMMAND} ${TEMPLATE_GIT_INFO_ARGS}
  BYPRODUCTS ${TEMPLATE_GIT_INFO_HEADER}
  COMMENT "Updating git build info"
  VERBATIM
)
add_dependencies(CppTemplateCore template_git_info)

target_include_directories(CppTemplateCore PRIVATE ${TEMPLATE_GENERATED_DIR})

# Only version.cpp sees these, so a version bump or config switch recompiles one file. It skips the PCH because
# its definitions differ from the ones the PCH was built with (MSVC warns about that, and warnings are errors).
set_source_files_properties(src/core/version.cpp PROPERTIES
  SKIP_PRECOMPILE_HEADERS ON
  COMPILE_DEFINITIONS "TEMPLATE_PROJECT_NAME=\"${PROJECT_NAME}\";TEMPLATE_PROJECT_VERSION=\"${PROJECT_VERSION}\";TEMPLATE_PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR};TEMPLATE_PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR};TEMPLATE_PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH};TEMPLATE_BUILD_CONFIG=\"$<CONFIG>\""
)

add_library(template::core ALIAS CppTemplateCore)
