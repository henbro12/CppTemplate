#!/usr/bin/env bash
set -euo pipefail

# Default to Debug build if not specified
BUILD_TYPE="${1:-debug}"

# Determine the executable
if [[ "$BUILD_TYPE" == "test" ]]; then
    BUILD_DIR="build/test/tests"
    EXE_NAME="unit-tests"
else
    BUILD_DIR="build/$BUILD_TYPE/app"
    EXE_NAME="CppTemplateApp"
fi

[[ "$OS" == "Windows_NT" ]] && EXE_NAME="$EXE_NAME.exe"

# Build
./scripts/build.sh "$BUILD_TYPE"

# Run the binary
EXE_PATH="$BUILD_DIR/$EXE_NAME"
if [[ -f "$EXE_PATH" ]]; then
    echo "[INFO] Running $EXE_PATH"
    echo "----------------------------------------"
    "$EXE_PATH"
else
    echo "[ERROR] Executable not found: $EXE_PATH"
    exit 1
fi

echo "----------------------------------------"
echo "[SUCCESS] Execution complete."
