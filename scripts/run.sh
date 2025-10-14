#!/usr/bin/env bash
set -euo pipefail

# Usage: ./scripts/run.sh [toolchain] [config]
# toolchain: msvc  | gcc     | clang            (defaults to gcc)
# config   : Debug | Release | RelWithDebInfo   (defaults to Debug)

TOOLCHAIN="${1:-gcc}"
CONFIG="${2:-Debug}"

# Build first
SCRIPT_DIR="$(cd -- "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
"$SCRIPT_DIR/build.sh" "$TOOLCHAIN" "$CONFIG"

# Executable naming/location (matches typical CMake + presets layout)
APP_NAME="CppTemplateApp"
EXE="$APP_NAME"
if [[ "${OS:-}" == "Windows_NT" ]]; then EXE="${APP_NAME}.exe"; fi

# For multi-config generators, CMake places config as a subdir
# Our presets use:
#   build/msvc/app/<Config>/<exe>
#   build/gcc/app/<Config>/<exe>
#   build/clang/app/<Config>/<exe>
BUILD_DIR="build/${TOOLCHAIN}/app/${CONFIG}"
EXE_PATH="${BUILD_DIR}/${EXE}"

if [[ ! -f "$EXE_PATH" ]]; then
  echo "[ERROR] Executable not found at: $EXE_PATH"
  echo "       If your output directories differ, adjust run.sh paths."
  exit 1
fi

echo "[INFO] Running: $EXE_PATH"
echo "----------------------------------------"
"$EXE_PATH"
echo "----------------------------------------"
echo "[SUCCESS] Execution complete."
