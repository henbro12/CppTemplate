# SPDX-License-Identifier: Apache-2.0
# @copyright (C) 2025 Henbro12

add_library(CppTemplateCore STATIC
    "src/core/utils/logger.cpp"
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

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(CppTemplateCore PUBLIC
        TEMPLATE_ENABLE_ASSERTS
    )
endif()

add_library(template::core ALIAS CppTemplateCore)
