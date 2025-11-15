#!/usr/bin/env bash
set -euo pipefail

# Usage:
#   ./scripts/run.sh [-t TOOLCHAIN] [-c CONFIG] [-- APP_ARGS...]
# Examples:
#   ./scripts/run.sh -t gcc
#   ./scripts/run.sh -t clang -c RelWithDebInfo -- --input foo.txt

# ---------- parse flags (stop at --) ----------
TOOLCHAIN=""
CONFIG="Debug"
APP_ARGS=()

while [[ $# -gt 0 ]]; do
  case "$1" in
    -t|--toolchain) TOOLCHAIN="$2"; shift 2 ;;
    -c|--config)    CONFIG="$2";    shift 2 ;;
    --)             shift; APP_ARGS=("$@"); break ;;
    *)              echo "[ERROR] Unknown option: $1"; exit 1 ;;
  esac
done

# ---------- detect host OS ----------
uname_s=$(uname -s || echo "")
case "$uname_s" in
  MINGW*|MSYS*|CYGWIN*) HOST_OS="Windows" ;;
  Darwin*)              HOST_OS="Darwin"  ;;
  Linux*)               HOST_OS="Linux"   ;;
  *)                    HOST_OS="$uname_s" ;;
esac

# ---------- defaults per OS ----------
if [[ -z "$TOOLCHAIN" ]]; then
  case "$HOST_OS" in
    Windows) TOOLCHAIN="msvc" ;;
    Linux)   TOOLCHAIN="gcc"  ;;
    Darwin)  TOOLCHAIN="appleclang" ;;
    *)       TOOLCHAIN="gcc"  ;;
  esac
fi

# ---------- build first (delegates to presets) ----------
SCRIPT_DIR="$(cd -- "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
"$SCRIPT_DIR/build.sh" -t "$TOOLCHAIN" -c "$CONFIG"

# ---------- map to build dir ----------
case "$HOST_OS:$TOOLCHAIN" in
  Windows:msvc)        BUILDDIR="build/win-msvc" ;;
  Windows:gcc)         BUILDDIR="build/win-mingw-gcc" ;;
  Windows:clang)       BUILDDIR="build/win-clangcl" ;;
  Linux:gcc)           BUILDDIR="build/linux-gcc" ;;
  Linux:clang)         BUILDDIR="build/linux-clang" ;;
  Darwin:appleclang)   BUILDDIR="build/mac-appleclang" ;;
  *)
    echo "[ERROR] Unknown toolchain mapping for $HOST_OS / $TOOLCHAIN"
    exit 1
    ;;
esac

APP_NAME="${APP_NAME:-CppTemplateApp}"
EXE="$APP_NAME"
if [[ "$HOST_OS" == "Windows" ]]; then EXE="${APP_NAME}.exe"; fi

# Prefer multi-config tree, then single-config
CANDIDATES=(
  "${BUILDDIR}/app/${CONFIG}/${EXE}"  # multi-config (Ninja Multi-Config/VS)
  "${BUILDDIR}/app/${EXE}"            # single-config (Ninja)
)

EXE_PATH=""
for p in "${CANDIDATES[@]}"; do
  if [[ -f "$p" ]]; then EXE_PATH="$p"; break; fi
done

# Fallback: search
if [[ -z "$EXE_PATH" && -d "${BUILDDIR}/app" ]]; then
  # shellcheck disable=SC2044
  for p in $(find "${BUILDDIR}/app" -type f -name "$EXE" 2>/dev/null); do
    EXE_PATH="$p"; break
  done
fi

if [[ -z "$EXE_PATH" ]]; then
  echo "[ERROR] Executable not found. Looked for:"
  printf '  - %s\n' "${CANDIDATES[@]}"
  echo "Consider setting RUNTIME_OUTPUT_DIRECTORY or check your target name."
  exit 1
fi

echo "[INFO] Running: $EXE_PATH ${APP_ARGS[*]:-}"
echo "----------------------------------------"
"$EXE_PATH" "${APP_ARGS[@]:-}"
echo "----------------------------------------"
echo "[SUCCESS] Execution complete."
