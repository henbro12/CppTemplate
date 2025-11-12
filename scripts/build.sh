#!/usr/bin/env bash
set -euo pipefail

# Usage:
#   ./scripts/build.sh [-t TOOLCHAIN] [-c CONFIG] [--test]
#   TOOLCHAIN (Windows): msvc | gcc | clang
#   TOOLCHAIN (Linux)  : gcc  | clang
#   TOOLCHAIN (macOS)  : appleclang
#   CONFIG: Debug | Release | RelWithDebInfo  (default: Debug)

uname_s=$(uname -s || echo "")
case "$uname_s" in
  MINGW*|MSYS*|CYGWIN*) HOST_OS="Windows" ;;
  Darwin*)              HOST_OS="Darwin"  ;;
  Linux*)               HOST_OS="Linux"   ;;
  *)                    HOST_OS="$uname_s" ;;
esac

# defaults per OS
case "$HOST_OS" in
  Windows) DEFAULT_TOOLCHAIN="msvc" ;;
  Linux)   DEFAULT_TOOLCHAIN="gcc"  ;;
  Darwin)  DEFAULT_TOOLCHAIN="appleclang" ;;
  *)       DEFAULT_TOOLCHAIN="gcc"  ;;
esac

TOOLCHAIN="$DEFAULT_TOOLCHAIN"
CONFIG="Debug"
RUN_TESTS="false"

# parse flags
while [[ $# -gt 0 ]]; do
  case "$1" in
    -t|--toolchain) TOOLCHAIN="$2"; shift 2 ;;
    -c|--config)    CONFIG="$2"; shift 2 ;;
    --test)         RUN_TESTS="true"; shift ;;
    *) echo "[ERROR] Unknown option: $1"; exit 1 ;;
  esac
done

CONFIGURE_PRESET=""
TEST_PRESET=""

case "$HOST_OS:$TOOLCHAIN" in
  Windows:msvc)        CONFIGURE_PRESET="win-msvc";       TEST_PRESET="win-msvc-tests" ;;
  Windows:gcc)         CONFIGURE_PRESET="win-mingw-gcc";  TEST_PRESET="win-mingw-gcc-tests" ;;
  Windows:clang)       CONFIGURE_PRESET="win-clangcl";    TEST_PRESET="win-clangcl-tests" ;;
  Linux:gcc)           CONFIGURE_PRESET="linux-gcc";      TEST_PRESET="linux-gcc-tests" ;;
  Linux:clang)         CONFIGURE_PRESET="linux-clang";    TEST_PRESET="linux-clang-tests" ;;
  Darwin:appleclang)   CONFIGURE_PRESET="mac-appleclang"; TEST_PRESET="mac-appleclang-tests" ;;
  *)
    echo "[ERROR] Unsupported host/toolchain combo: $HOST_OS / $TOOLCHAIN"
    exit 1
    ;;
esac

BUILD_PRESET="${CONFIGURE_PRESET}-${CONFIG}"

echo "[INFO] Host OS          : $HOST_OS"
echo "[INFO] Toolchain        : $TOOLCHAIN"
echo "[INFO] Configure preset : $CONFIGURE_PRESET"
echo "[INFO] Build preset     : $BUILD_PRESET"
echo "[INFO] Configuration    : $CONFIG"

cmake --preset "$CONFIGURE_PRESET"
cmake --build --preset "$BUILD_PRESET"

echo "[SUCCESS] Build complete."

if [[ "$RUN_TESTS" == "true" && "$CONFIG" == "Debug" ]]; then
  echo "[INFO] Running tests: $TEST_PRESET"
  ctest --preset "$TEST_PRESET" --output-on-failure
  echo "[SUCCESS] Tests complete."
fi
