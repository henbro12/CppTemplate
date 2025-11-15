# SPDX-License-Identifier: Apache-2.0
# @copyright (C) 2025 Henbro12

cmake_minimum_required(VERSION 3.25)

# Project configuration
project(CppTemplate
  VERSION 0.1
  LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json for IDEs (e.g. VS Code with clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Position independent code for all targets by default (static libs on *nix)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Enable warnings and strictness
include(CheckCXXCompilerFlag)
function(enable_common_warnings target)
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
      # Warnings that are not allowed in C compilation units
      $<$<COMPILE_LANGUAGE:CXX>:-Wnon-virtual-dtor>
      $<$<COMPILE_LANGUAGE:CXX>:-Woverloaded-virtual>
    )
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
