#!/usr/bin/env bash
# Shared helpers for build.sh and run.sh. Sourced, not executed. Keep it bash 3.2 compatible (macOS).

# Prints Windows | Darwin | Linux, or the raw `uname -s` value for anything else
detect_host_os() {
  local uname_s
  uname_s=$(uname -s 2>/dev/null || echo "")
  case "$uname_s" in
    MINGW*|MSYS*|CYGWIN*) echo "Windows" ;;
    Darwin*)              echo "Darwin"  ;;
    Linux*)               echo "Linux"   ;;
    *)                    echo "$uname_s" ;;
  esac
}

default_toolchain() {
  case "$1" in
    Windows) echo "msvc" ;;
    Darwin)  echo "appleclang" ;;
    *)       echo "gcc" ;;
  esac
}

# Prints the configure preset for <host-os> <toolchain>, or nothing when the combination is unsupported
configure_preset_for() {
  case "$1:$2" in
    Windows:msvc)      echo "win-msvc" ;;
    Windows:gcc)       echo "win-mingw-gcc" ;;
    Windows:clang)     echo "win-clangcl" ;;
    Windows:tidy)      echo "win-tidy" ;;
    Linux:gcc)         echo "linux-gcc" ;;
    Linux:clang)       echo "linux-clang" ;;
    Linux:tidy)        echo "linux-tidy" ;;
    Linux:asan)        echo "linux-asan" ;;
    Linux:coverage)    echo "linux-coverage" ;;
    Darwin:appleclang) echo "mac-appleclang" ;;
    Darwin:tidy)       echo "mac-tidy" ;;
    Darwin:asan)       echo "mac-asan" ;;
  esac
}

# The analysis presets (clang-tidy, sanitizers, coverage) are single-config Debug builds
is_debug_only_toolchain() {
  case "$1" in
    tidy|asan|coverage) return 0 ;;
    *)                  return 1 ;;
  esac
}

# Usage: require_value "$@"  (inside an option-parsing loop, before reading $2)
require_value() {
  if [[ $# -lt 2 || -z "$2" ]]; then
    echo "[ERROR] Option $1 requires a value"
    exit 1
  fi
}
