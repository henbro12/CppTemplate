#!/usr/bin/env bash
set -euo pipefail

VCPKG_DIR="external/vcpkg"

# ---------- Helpers ----------
install_ninja_if_missing_unix() {
  if ! command -v ninja &> /dev/null; then
    echo "[INFO] Ninja not found. Installing..."
    if command -v apt &> /dev/null; then
      sudo apt update && sudo apt install -y ninja-build
    elif command -v dnf &> /dev/null; then
      sudo dnf install -y ninja-build
    elif command -v pacman &> /dev/null; then
      sudo pacman -Sy --noconfirm ninja
    elif command -v brew &> /dev/null; then
      brew install ninja
    else
      echo "[ERROR] Unsupported package manager. Install Ninja manually."
      exit 1
    fi
  fi
}

# ---------- Platform ----------
uname_s=$(uname -s || echo "")
case "$uname_s" in
  MINGW*|MSYS*|CYGWIN*) PLATFORM="windows" ;;
  Darwin*)              PLATFORM="osx"     ;;
  Linux*)               PLATFORM="linux"   ;;
  *) echo "[ERROR] Unsupported platform: $uname_s"; exit 1 ;;
esac
echo "[INFO] Platform detected: $PLATFORM"

# ---------- Tools ----------
if [[ "$PLATFORM" != "windows" ]]; then
  install_ninja_if_missing_unix
  if ! command -v cmake &> /dev/null; then
    echo "[WARN] cmake not found. Please install CMake >= 3.25."
  fi
  if [[ "$PLATFORM" == "osx" && ! -x "/usr/bin/clang" ]]; then
    echo "[WARN] Xcode command line tools missing. Run: xcode-select --install"
  fi
fi

# ---------- vcpkg clone ----------
if [ ! -d "$VCPKG_DIR/.git" ]; then
  echo "[INFO] Cloning vcpkg into $VCPKG_DIR..."
  git clone --depth 1 https://github.com/microsoft/vcpkg.git "$VCPKG_DIR"
fi

# ---------- vcpkg bootstrap ----------
if [[ "$PLATFORM" == "windows" ]]; then
  if [ ! -f "$VCPKG_DIR/vcpkg.exe" ]; then
    echo "[INFO] Bootstrapping vcpkg (Windows)..."
    WIN_VCPKG_DIR=$(cd "$VCPKG_DIR" && cygpath -w "$PWD")
    cmd.exe /c "$WIN_VCPKG_DIR\\bootstrap-vcpkg.bat -disableMetrics"
  fi
else
  if [ ! -f "$VCPKG_DIR/vcpkg" ]; then
    echo "[INFO] Bootstrapping vcpkg (Unix)..."
    "$VCPKG_DIR/bootstrap-vcpkg.sh" -disableMetrics
  fi
fi

export VCPKG_ROOT="$(cd "$VCPKG_DIR" && pwd)"
echo "[INFO] VCPKG_ROOT set to $VCPKG_ROOT"
echo ""
echo "[SUCCESS] Setup complete."
echo ""
echo "Build & Run Examples:"
echo "  ./scripts/build.sh                 # uses OS default toolchain"
echo "  ./scripts/build.sh clang Release   # pick toolchain+config"
echo "  ./scripts/run.sh -- --help         # run app with args"
