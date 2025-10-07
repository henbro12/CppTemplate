#!/usr/bin/env bash
set -euo pipefail

# -------------------
# Configuration
# -------------------
VCPKG_DIR="external/vcpkg"
BUILD_DIR="build"

# -------------------
# Functions
# -------------------
function install_ninja_if_missing_unix {
    if ! command -v ninja &> /dev/null; then
        echo "[INFO] Ninja not found. Installing..."
        if command -v apt &> /dev/null; then
            sudo apt update && sudo apt install -y ninja-build
        elif command -v brew &> /dev/null; then
            brew install ninja
        else
            echo "[ERROR] Unsupported package manager. Install Ninja manually."
            exit 1
        fi
    fi
}

# -------------------
# Platform detection
# -------------------
UNAME=$(uname -s)
case "$UNAME" in
    Linux*)     PLATFORM="linux";;
    Darwin*)    PLATFORM="osx";;
    MINGW*|MSYS*|CYGWIN*) PLATFORM="windows";;
    *)          echo "[ERROR] Unsupported platform: $UNAME"; exit 1;;
esac

echo "[INFO] Platform detected: $PLATFORM"

# -------------------
# Install Ninja if needed
# -------------------
if [[ "$PLATFORM" != "windows" ]]; then
    install_ninja_if_missing_unix
fi

# -------------------
# Clone vcpkg
# -------------------
if [ ! -d "$VCPKG_DIR" ]; then
    echo "[INFO] Cloning vcpkg into $VCPKG_DIR..."
    git clone https://github.com/microsoft/vcpkg.git "$VCPKG_DIR"
fi

# -------------------
# Bootstrap vcpkg
# -------------------
if [[ "$PLATFORM" == "windows" ]]; then
    if [ ! -f "$VCPKG_DIR/vcpkg.exe" ]; then
        echo "[INFO] Bootstrapping vcpkg (Windows)..."
        WIN_VCPKG_DIR=$(cd "$VCPKG_DIR" && pwd -W)
        cmd.exe /c "$WIN_VCPKG_DIR\\bootstrap-vcpkg.bat"
    fi
else
    if [ ! -f "$VCPKG_DIR/vcpkg" ]; then
        echo "[INFO] Bootstrapping vcpkg (Unix)..."
        "$VCPKG_DIR/bootstrap-vcpkg.sh"
    fi
fi

# -------------------
# Set environment variable
# -------------------
export VCPKG_ROOT=$(cd "$VCPKG_DIR" && pwd)
echo "[INFO] VCPKG_ROOT set to $VCPKG_ROOT"

# -------------------
# Configure project
# -------------------
echo "[INFO] Configuring project using CMake Presets..."
cmake --preset=debug

echo "[SUCCESS] Setup complete. You can now build with:"
echo "  ./scripts/build.sh"
echo "Or run the project with:"
echo "  ./scripts/run.sh"
