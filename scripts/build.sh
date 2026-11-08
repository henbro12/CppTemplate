#!/usr/bin/env bash
set -euo pipefail

# Usage:
#   ./scripts/build.sh [-t TOOLCHAIN] [-c CONFIG] [--test]
#   TOOLCHAIN (Windows): msvc | gcc | clang | tidy
#   TOOLCHAIN (Linux)  : gcc  | clang | tidy | asan | coverage
#   TOOLCHAIN (macOS)  : appleclang | tidy | asan
#   CONFIG: Debug | Release | RelWithDebInfo  (default: Debug; tidy/asan/coverage are Debug only)

SCRIPT_DIR="$(cd -- "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
# shellcheck source=common.sh
source "$SCRIPT_DIR/common.sh"
cd "$SCRIPT_DIR/.."

HOST_OS="$(detect_host_os)"
TOOLCHAIN="$(default_toolchain "$HOST_OS")"
CONFIG="Debug"
RUN_TESTS="false"

# parse flags
while [[ $# -gt 0 ]]; do
  case "$1" in
    -t|--toolchain) require_value "$@"; TOOLCHAIN="$2"; shift 2 ;;
    -c|--config)    require_value "$@"; CONFIG="$2"; shift 2 ;;
    --test)         RUN_TESTS="true"; shift ;;
    *) echo "[ERROR] Unknown option: $1"; exit 1 ;;
  esac
done

CONFIGURE_PRESET="$(configure_preset_for "$HOST_OS" "$TOOLCHAIN")"
if [[ -z "$CONFIGURE_PRESET" ]]; then
  echo "[ERROR] Unsupported host/toolchain combo: $HOST_OS / $TOOLCHAIN"
  exit 1
fi

case "$CONFIG" in
  Debug|Release|RelWithDebInfo) ;;
  *) echo "[ERROR] Unsupported config: $CONFIG (expected Debug | Release | RelWithDebInfo)"; exit 1 ;;
esac

if is_debug_only_toolchain "$TOOLCHAIN" && [[ "$CONFIG" != "Debug" ]]; then
  echo "[ERROR] The $TOOLCHAIN toolchain only supports the Debug config"
  exit 1
fi

# Tidy builds compile the tests (so clang-tidy sees them) but are not meant to be run
if [[ "$TOOLCHAIN" == "tidy" && "$RUN_TESTS" == "true" ]]; then
  echo "[ERROR] --test is not supported with the tidy toolchain"
  exit 1
fi

BUILD_PRESET="${CONFIGURE_PRESET}-${CONFIG}"
TEST_PRESET="${BUILD_PRESET}-tests"

echo "[INFO] Host OS          : $HOST_OS"
echo "[INFO] Toolchain        : $TOOLCHAIN"
echo "[INFO] Configure preset : $CONFIGURE_PRESET"
echo "[INFO] Build preset     : $BUILD_PRESET"
echo "[INFO] Configuration    : $CONFIG"

cmake --preset "$CONFIGURE_PRESET"
cmake --build --preset "$BUILD_PRESET"

echo "[SUCCESS] Build complete."

if [[ "$RUN_TESTS" == "true" ]]; then
  echo "[INFO] Running tests: $TEST_PRESET"
  ctest --preset "$TEST_PRESET"
  echo "[SUCCESS] Tests complete."
fi
