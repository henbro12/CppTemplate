# SPDX-License-Identifier: MIT
# @copyright (C) 2025 Henbro12

add_library(CppTemplateCore STATIC
  "src/core/utils/logger.cpp"
  "src/core/utils/profiler.cpp"
  "src/core/template/template.cpp"
)

target_include_directories(CppTemplateCore PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

find_package(spdlog CONFIG REQUIRED)

target_link_libraries(CppTemplateCore PUBLIC
  spdlog::spdlog
)

target_precompile_headers(CppTemplateCore PRIVATE src/pch.h)

enable_common_warnings(CppTemplateCore)

# Generator expression so this also works with multi-config generators (Ninja Multi-Config, Visual Studio)
target_compile_definitions(CppTemplateCore PUBLIC
  $<$<CONFIG:Debug>:TEMPLATE_ENABLE_ASSERTS>
)

target_compile_definitions(CppTemplateCore PRIVATE
  $<$<CONFIG:Debug>:_DEBUG;DEBUG>
  $<$<CONFIG:Release>:_RELEASE;RELEASE>
)

add_library(template::core ALIAS CppTemplateCore)
