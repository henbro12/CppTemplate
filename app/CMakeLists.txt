# SPDX-License-Identifier: Apache-2.0
# @copyright (C) 2025 Henbro12

add_executable(CppTemplateApp
  src/main.cpp
)

target_include_directories(CppTemplateApp PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

target_link_libraries(CppTemplateApp PRIVATE
  template::core
)

enable_common_warnings(CppTemplateApp)

target_compile_definitions(CppTemplateApp PRIVATE
  $<$<CONFIG:Debug>:_DEBUG;DEBUG>
  $<$<CONFIG:Release>:_RELEASE;RELEASE>
)

add_executable(template::app ALIAS CppTemplateApp)
