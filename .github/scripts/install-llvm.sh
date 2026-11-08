#!/usr/bin/env bash
# Installs a pinned LLVM major version on the Ubuntu CI runners from the official LLVM apt repository
# (https://apt.llvm.org) and makes it the default `clang`/`clang++`, so the Clang builds, clang-tidy
# and clang-format all use the same version as local development.
#
# Usage: .github/scripts/install-llvm.sh <major-version>
set -euo pipefail

VERSION="${1:?usage: install-llvm.sh <major-version>}"
CODENAME="$(. /etc/os-release && echo "$VERSION_CODENAME")"

wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key | sudo tee /etc/apt/trusted.gpg.d/apt.llvm.org.asc >/dev/null
echo "deb http://apt.llvm.org/${CODENAME}/ llvm-toolchain-${CODENAME}-${VERSION} main" \
  | sudo tee /etc/apt/sources.list.d/llvm-${VERSION}.list >/dev/null

sudo apt-get update
# libclang-rt provides the sanitizer runtimes used by the asan preset
sudo apt-get install -y \
  "clang-${VERSION}" "clang-tidy-${VERSION}" "clang-format-${VERSION}" "libclang-rt-${VERSION}-dev"

# The presets call plain clang/clang++; /usr/local/bin comes before /usr/bin on PATH
for tool in clang clang++ clang-tidy clang-format run-clang-tidy; do
  sudo ln -sf "/usr/bin/${tool}-${VERSION}" "/usr/local/bin/${tool}"
done

clang++ --version
clang-format --version
