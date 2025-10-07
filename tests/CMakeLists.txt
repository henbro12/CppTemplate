# SPDX-License-Identifier: Apache-2.0
# @copyright (C) 2025 Henbro12

find_package(Catch2 REQUIRED)

add_executable(unit-tests
    test_logger.cpp
)

target_link_libraries(unit-tests PRIVATE
    Catch2::Catch2WithMain
    template::core
)

# Automatically register with CTest
include(CTest)

# Enable Catch2 tests
add_test(NAME unit-tests COMMAND unit-tests)