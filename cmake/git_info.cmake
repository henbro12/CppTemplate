# SPDX-License-Identifier: MIT
# @copyright (C) 2025 Henbro12
#
# Writes the current git state into a header. Runs at configure time and, via the `template_git_info`
# target, before every build (cmake -P). configure_file only rewrites the header when its content changes,
# so the code that uses it is only recompiled when the commit or dirty state changes.
#
# Inputs: SOURCE_DIR, TEMPLATE_FILE, OUTPUT_FILE

set(TEMPLATE_GIT_DESCRIBE "unknown")
set(TEMPLATE_GIT_COMMIT "unknown")
set(TEMPLATE_GIT_DIRTY "false")

find_package(Git QUIET)
if (GIT_FOUND)
  # e.g. "v1.2.0-5-g1a2b3c4d-dirty", or just "1a2b3c4d" when there are no tags yet
  execute_process(
    COMMAND "${GIT_EXECUTABLE}" describe --tags --always --dirty --abbrev=8
    WORKING_DIRECTORY "${SOURCE_DIR}"
    RESULT_VARIABLE describe_result
    OUTPUT_VARIABLE describe_output
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
  )
  execute_process(
    COMMAND "${GIT_EXECUTABLE}" rev-parse --short=8 HEAD
    WORKING_DIRECTORY "${SOURCE_DIR}"
    RESULT_VARIABLE commit_result
    OUTPUT_VARIABLE commit_output
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
  )

  if (describe_result EQUAL 0 AND commit_result EQUAL 0)
    set(TEMPLATE_GIT_DESCRIBE "${describe_output}")
    set(TEMPLATE_GIT_COMMIT "${commit_output}")
    if (describe_output MATCHES "-dirty$")
      set(TEMPLATE_GIT_DIRTY "true")
    endif()
  endif()
endif()

configure_file("${TEMPLATE_FILE}" "${OUTPUT_FILE}" @ONLY)
