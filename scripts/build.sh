#!/usr/bin/env bash
set -euo pipefail

# Usage: ./scripts/build.sh [toolchain] [config] [target]
# toolchain: msvc  | gcc     | clang            (defaults to mingw-gcc)
# config   : Debug | Release | RelWithDebInfo   (defaults to Debug)
# target   : optional cmake target to build     (defaults to ALL)

TOOLCHAIN="${1:-gcc}"
CONFIG="${2:-Debug}"
TARGET="${3:-}"

# Map to CMake configure/build presets
CONFIGURE_PRESET="$TOOLCHAIN"
BUILD_PRESET="$TOOLCHAIN-$CONFIG"

echo "[INFO] Configure preset: $CONFIGURE_PRESET"
echo "[INFO] Build preset    : $BUILD_PRESET"
echo "[INFO] Configuration   : $CONFIG"
if [[ -n "$TARGET" ]]; then echo "[INFO] Target          : $TARGET"; fi

# Always (re)configure via preset; CMake will reuse the cache safely.
cmake --preset "$CONFIGURE_PRESET"

# Build via build preset, optionally a single target
if [[ -n "$TARGET" ]]; then
  cmake --build --preset "$BUILD_PRESET" --target "$TARGET"
else
  cmake --build --preset "$BUILD_PRESET"
fi

echo "[SUCCESS] Build complete."
