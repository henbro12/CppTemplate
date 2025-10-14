#!/usr/bin/env bash
set -euo pipefail

# Usage: ./scripts/build.sh [toolchain] [config] [--test]
# toolchain: msvc  | gcc     | clang         (auto-detected if omitted)
# config   : Debug | Release | RelWithDebInfo   (default: Debug)
# --test   : run ctest for matching test preset

# --- detect platform default ---
uname_s=$(uname -s || echo "")
default_toolchain="gcc"
case "$uname_s" in
  MINGW*|MSYS*|CYGWIN*) default_toolchain="msvc" ;;  # Git Bash on Windows
  Darwin*)              default_toolchain="clang" ;; # Xcode/Apple Clang later
  Linux*)               default_toolchain="gcc" ;;
esac

TOOLCHAIN="${1:-$default_toolchain}"
CONFIG="${2:-Debug}"
RUN_TESTS="false"
if [[ "${3:-}" == "--test" ]]; then RUN_TESTS="true"; fi

CONFIGURE_PRESET="$TOOLCHAIN"
BUILD_PRESET="$TOOLCHAIN-$CONFIG"

echo "[INFO] Toolchain       : $TOOLCHAIN"
echo "[INFO] Configure preset: $CONFIGURE_PRESET"
echo "[INFO] Build preset    : $BUILD_PRESET"
echo "[INFO] Configuration   : $CONFIG"

# Configure
cmake --preset "$CONFIGURE_PRESET"

# Build (always builds the whole project)
cmake --build --preset "$BUILD_PRESET"

echo "[SUCCESS] Build complete."

# Run tests if requested AND in Debug
if [[ "$RUN_TESTS" == "true" && "$CONFIG" == "Debug" ]]; then
  echo "[INFO] Running tests    : $TOOLCHAIN"
  case "$TOOLCHAIN" in
    msvc)  ctest --preset msvc-tests  --output-on-failure ;;
    gcc)   ctest --preset gcc-tests   --output-on-failure ;;
    clang) ctest --preset clang-tests --output-on-failure ;;
    *)     echo "[WARN] Unknown toolchain '$TOOLCHAIN' for tests; skipping." ;;
  esac
  echo "[SUCCESS] Tests complete."
fi
