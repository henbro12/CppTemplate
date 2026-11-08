# SPDX-License-Identifier: MIT
# @copyright (C) 2025 Henbro12

cmake_minimum_required(VERSION 3.25)

# Project configuration
# The version is available in code through core/version.h (see core::versionInfo())
project(CppTemplate
  VERSION 0.1.0
  LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json for IDEs (e.g. VS Code with clangd) and clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Position independent code for all targets by default (static libs on *nix)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(GNUInstallDirs)

# Instrumentation for the project's own targets (dependencies from vcpkg are not instrumented)
set(TEMPLATE_SANITIZERS "" CACHE STRING "Comma-separated -fsanitize= list for GCC/Clang, e.g. 'address,undefined'")
option(TEMPLATE_ENABLE_COVERAGE "Build the project's own targets with --coverage (GCC/Clang)" OFF)

if (MSVC AND (TEMPLATE_SANITIZERS OR TEMPLATE_ENABLE_COVERAGE))
  message(FATAL_ERROR "TEMPLATE_SANITIZERS and TEMPLATE_ENABLE_COVERAGE are only supported with GCC/Clang")
endif()

# Common settings for every target in this project: warnings as errors, plus optional sanitizers and coverage.
# Override warnings-as-errors locally with `cmake --preset <preset> --compile-no-warning-as-error`.
function(template_configure_target target)
  set_target_properties(${target} PROPERTIES COMPILE_WARNING_AS_ERROR ON)
  if (MSVC)
    target_compile_options(${target} PRIVATE /W4 /permissive-)
  else()
    target_compile_options(${target} PRIVATE
      -Wall
      -Wextra
      -pedantic
      -Wcast-align
      -Wformat=2
      -Wmisleading-indentation
      -Wmissing-include-dirs
      -Wnull-dereference
      -Wswitch-enum
      -Wunused
      -Wunused-variable
      -Wunused-parameter
      -Wvla
      -Wnon-virtual-dtor
      -Woverloaded-virtual
    )
  endif()

  # MinGW's own runtime (libgcc, libstdc++, winpthread) is not part of Windows: link it statically so the
  # executables run without MSYS2 on PATH, like the other toolchains (see "Linking" in the README)
  if (MINGW)
    target_link_options(${target} PRIVATE -static)
  endif()

  if (TEMPLATE_SANITIZERS)
    target_compile_options(${target} PRIVATE -fsanitize=${TEMPLATE_SANITIZERS} -fno-omit-frame-pointer -fno-sanitize-recover=all)
    target_link_options(${target} PRIVATE -fsanitize=${TEMPLATE_SANITIZERS})
  endif()

  if (TEMPLATE_ENABLE_COVERAGE)
    target_compile_options(${target} PRIVATE --coverage)
    target_link_options(${target} PRIVATE --coverage)
  endif()
endfunction()

# Add subdirectories
add_subdirectory(core)
add_subdirectory(app)

# Add testing support
include(CTest)
if (BUILD_TESTING)
  add_subdirectory(tests)
endif()
