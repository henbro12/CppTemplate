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

# Output directories
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable warnings and strictness
include(CheckCXXCompilerFlag)
function(enable_common_warnings target)
    if (MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive-)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
endfunction()

# Add subdirectories
add_subdirectory(core)
add_subdirectory(app)

# Add testing support
option(ENABLE_TESTING "Enable unit test targets" ON)
if(ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()