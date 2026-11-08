#!/usr/bin/env bash
set -euo pipefail

# Usage:
#   ./scripts/run.sh [-t TOOLCHAIN] [-c CONFIG] [-- APP_ARGS...]
# Examples:
#   ./scripts/run.sh -t gcc
#   ./scripts/run.sh -t clang -c RelWithDebInfo -- --input foo.txt

SCRIPT_DIR="$(cd -- "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
# shellcheck source=common.sh
source "$SCRIPT_DIR/common.sh"
cd "$SCRIPT_DIR/.."

# ---------- parse flags (stop at --) ----------
HOST_OS="$(detect_host_os)"
TOOLCHAIN="$(default_toolchain "$HOST_OS")"
CONFIG="Debug"
APP_ARGS=()

while [[ $# -gt 0 ]]; do
  case "$1" in
    -t|--toolchain) require_value "$@"; TOOLCHAIN="$2"; shift 2 ;;
    -c|--config)    require_value "$@"; CONFIG="$2";    shift 2 ;;
    --)             shift; APP_ARGS=("$@"); break ;;
    *)              echo "[ERROR] Unknown option: $1"; exit 1 ;;
  esac
done

# ---------- build first (validates toolchain/config, delegates to presets) ----------
"$SCRIPT_DIR/build.sh" -t "$TOOLCHAIN" -c "$CONFIG"

# Presets put each build in build/<configure-preset>
BUILDDIR="build/$(configure_preset_for "$HOST_OS" "$TOOLCHAIN")"

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

if [[ -z "$EXE_PATH" ]]; then
  echo "[ERROR] Executable not found. Looked for:"
  printf '  - %s\n' "${CANDIDATES[@]}"
  echo "Set APP_NAME if you renamed the app target."
  exit 1
fi

echo "[INFO] Running: $EXE_PATH ${APP_ARGS[*]:-}"
echo "----------------------------------------"
# Branch instead of "${APP_ARGS[@]:-}", which passes one empty argument when there are none
# (and plain "${APP_ARGS[@]}" trips `set -u` on macOS' bash 3.2 when the array is empty)
if [[ ${#APP_ARGS[@]} -gt 0 ]]; then
  "$EXE_PATH" "${APP_ARGS[@]}"
else
  "$EXE_PATH"
fi
echo "----------------------------------------"
echo "[SUCCESS] Execution complete."
