#!/usr/bin/env bash
set -euo pipefail

# Default to Debug build if no argument is provided
BUILD_TYPE="${1:-debug}"

# Setup CMake config for different modes
case "$BUILD_TYPE" in
  debug|release|dist)
    BUILD_DIR="build/$BUILD_TYPE"
    TARGET=""
    ;;
  test)
    BUILD_DIR="build/test"
    TARGET="unit-tests"
    ;;
  *)
    echo "[ERROR] Unknown build type: $BUILD_TYPE"
    echo "Usage: $0 [debug|release|dist|test]"
    exit 1
    ;;
esac

# Configure the project if needed
if [ ! -d "$BUILD_DIR" ]; then
  echo "[INFO] Configuring CMake in $BUILD_TYPE mode..."
  cmake -S . -B "$BUILD_DIR" -G Ninja \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_TOOLCHAIN_FILE="external/vcpkg/scripts/buildsystems/vcpkg.cmake"
fi

# Build project or test target
if [ -n "$TARGET" ]; then
  echo "[INFO] Building target: $TARGET with preset: $BUILD_TYPE"
  cmake --preset "$BUILD_TYPE"
  cmake --build --preset "$BUILD_TYPE" --target "$TARGET"
else
  echo "[INFO] Building full project in $BUILD_TYPE mode with preset: $BUILD_TYPE"
  cmake --preset "$BUILD_TYPE"
  cmake --build --preset "$BUILD_TYPE"
fi

echo "[SUCCESS] Build complete."
