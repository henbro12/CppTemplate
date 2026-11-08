#!/usr/bin/env bash
set -uo pipefail

# Runs the same checks as CI, locally: formatting, build + tests (Debug and Release), clang-tidy and cppcheck.
# Every step runs even if an earlier one fails; a summary at the end lists the results.
#
# Usage:
#   ./scripts/check.sh [-t TOOLCHAIN] [--quick] [--strict]
#   -t TOOLCHAIN  toolchain for the build/test steps (default: your OS's default, see build.sh)
#   --quick       only formatting and a Debug build + tests
#   --strict      treat skipped steps (tool not installed) as failures
#
# On Windows the clang-tidy step builds with clang-cl, so run this from a Developer shell for VS 2022.

SCRIPT_DIR="$(cd -- "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
# shellcheck source=common.sh
source "$SCRIPT_DIR/common.sh"
cd "$SCRIPT_DIR/.."

HOST_OS="$(detect_host_os)"
TOOLCHAIN="$(default_toolchain "$HOST_OS")"
QUICK="false"
STRICT="false"
CI_LLVM_VERSION="20"

while [[ $# -gt 0 ]]; do
  case "$1" in
    -t|--toolchain) require_value "$@"; TOOLCHAIN="$2"; shift 2 ;;
    --quick)        QUICK="true"; shift ;;
    --strict)       STRICT="true"; shift ;;
    *) echo "[ERROR] Unknown option: $1"; exit 1 ;;
  esac
done

TIDY_PRESET="$(configure_preset_for "$HOST_OS" tidy)"
RESULTS=()
FAILED="false"

# Usage: record <step name> <PASS|FAIL|SKIP> [detail]
record() {
  RESULTS+=("$(printf '%-24s %s %s' "$1" "$2" "${3:-}")")
  if [[ "$2" == "FAIL" || ( "$2" == "SKIP" && "$STRICT" == "true" ) ]]; then
    FAILED="true"
  fi
}

step() {
  echo ""
  echo "==================== $1 ===================="
}

# First match of the given command names on PATH, e.g. find_tool clang-format-20 clang-format
find_tool() {
  local name
  for name in "$@"; do
    if command -v "$name" >/dev/null 2>&1; then
      command -v "$name"
      return 0
    fi
  done
  return 1
}

# Python scripts like run-clang-tidy can't be executed directly on Windows
run_python_tool() {
  if [[ "$HOST_OS" == "Windows" ]]; then
    "$(find_tool python3 python)" "$@"
  else
    "$@"
  fi
}

# ---------- clang-format ----------
step "clang-format"
if CLANG_FORMAT="$(find_tool "clang-format-$CI_LLVM_VERSION" clang-format)"; then
  version="$("$CLANG_FORMAT" --version | sed -E 's/.*version ([0-9]+).*/\1/')"
  if [[ "$version" != "$CI_LLVM_VERSION" ]]; then
    echo "[WARN] clang-format $version found; CI uses $CI_LLVM_VERSION, so results may differ"
  fi
  # shellcheck disable=SC2046 # word splitting of the file list is intended
  if "$CLANG_FORMAT" --dry-run --Werror $(git ls-files '*.cpp' '*.h' '*.hpp'); then
    record "clang-format" PASS "(v$version)"
  else
    record "clang-format" FAIL "fix with: clang-format -i \$(git ls-files '*.cpp' '*.h' '*.hpp')"
  fi
else
  record "clang-format" SKIP "clang-format not found"
fi

# ---------- build + tests ----------
CONFIGS=(Debug Release)
if [[ "$QUICK" == "true" ]]; then CONFIGS=(Debug); fi

for config in "${CONFIGS[@]}"; do
  step "build + tests ($TOOLCHAIN, $config)"
  if "$SCRIPT_DIR/build.sh" -t "$TOOLCHAIN" -c "$config" --test; then
    record "tests ($config)" PASS
  else
    record "tests ($config)" FAIL
  fi
done

if [[ "$QUICK" == "false" ]]; then
  # ---------- tidy build ----------
  step "tidy build ($TIDY_PRESET)"
  TIDY_BUILD_OK="false"
  if "$SCRIPT_DIR/build.sh" -t tidy; then
    TIDY_BUILD_OK="true"
  else
    record "tidy build" FAIL
  fi

  # ---------- clang-tidy ----------
  step "clang-tidy"
  if [[ "$TIDY_BUILD_OK" != "true" ]]; then
    record "clang-tidy" SKIP "tidy build failed"
  elif RUN_CLANG_TIDY="$(find_tool "run-clang-tidy-$CI_LLVM_VERSION" run-clang-tidy)"; then
    tidy_args=(-quiet -p "build/$TIDY_PRESET")
    if CLANG_TIDY="$(find_tool "clang-tidy-$CI_LLVM_VERSION" clang-tidy)"; then
      tidy_args+=(-clang-tidy-binary "$CLANG_TIDY")
    fi
    if run_python_tool "$RUN_CLANG_TIDY" "${tidy_args[@]}" core app tests; then
      record "clang-tidy" PASS
    else
      record "clang-tidy" FAIL
    fi
  else
    record "clang-tidy" SKIP "run-clang-tidy not found"
  fi

  # ---------- cppcheck ----------
  step "cppcheck"
  if [[ "$TIDY_BUILD_OK" != "true" ]]; then
    record "cppcheck" SKIP "tidy build failed"
  elif CPPCHECK="$(find_tool cppcheck)"; then
    if "$CPPCHECK" --project="build/$TIDY_PRESET/compile_commands.json" \
         --enable=warning,style,performance,portability \
         --inline-suppr --suppressions-list=.cppcheck-suppressions \
         --error-exitcode=1 -q; then
      record "cppcheck" PASS
    else
      record "cppcheck" FAIL
    fi
  else
    record "cppcheck" SKIP "cppcheck not found"
  fi
fi

# ---------- summary ----------
echo ""
echo "==================== summary ===================="
printf '%s\n' "${RESULTS[@]}"

if [[ "$FAILED" == "true" ]]; then
  echo "[FAILED] Some checks failed."
  exit 1
fi
echo "[SUCCESS] All checks passed."
