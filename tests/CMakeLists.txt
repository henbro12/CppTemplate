# SPDX-License-Identifier: Apache-2.0
# @copyright (C) 2025 Henbro12

# Catch2 via vcpkg (v3):
find_package(Catch2 3 CONFIG REQUIRED)

add_executable(unit_tests
  test_logger.cpp
)

target_link_libraries(unit_tests PRIVATE
  Catch2::Catch2WithMain
  template::core
)

# Automatically register all TEST_CASEs with CTest
include(Catch)
catch_discover_tests(unit_tests
  TEST_PREFIX "unit."
)
