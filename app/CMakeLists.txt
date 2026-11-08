# SPDX-License-Identifier: MIT
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

template_configure_target(CppTemplateApp)

add_executable(template::app ALIAS CppTemplateApp)

# `cmake --install build/<preset> --config <Config> --prefix <dir>`
# Dependencies are linked statically on every platform, so the executable is all that needs installing.
install(TARGETS CppTemplateApp RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
