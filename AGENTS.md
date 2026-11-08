# AGENTS.md

Instructions for AI coding agents working in this repository. Humans: see [README.md](README.md) and [docs/](docs/).

## Project

Cross-platform C++23 template: CMake presets + vcpkg (pinned, in `external/vcpkg`) + Ninja, for Windows (MSVC,
clang-cl, MinGW GCC), Linux (GCC, Clang) and macOS (AppleClang).

- `core/` - static library `template::core` (logger, assert/profiler macros, version info); headers under `core/src/core/`
- `app/` - executable linking `core`
- `tests/` - Catch2 unit tests (`unit_tests`)
- `scripts/` - `setup.sh`, `build.sh`, `run.sh`, `check.sh` (bash; on Windows run them from Git Bash)
- `docs/` - building, code quality, CI and core library details

## Commands

Run everything from the repository root (the scripts also work from elsewhere).

```bash
./scripts/setup.sh                          # once: fetch + bootstrap the pinned vcpkg
./scripts/build.sh --test                   # build (OS default toolchain, Debug) and run the tests
./scripts/build.sh -t clang -c Release --test
./scripts/check.sh                          # everything CI checks: format, Debug+Release tests, clang-tidy, cppcheck
./scripts/check.sh --quick                  # format + Debug tests only
```

Toolchains (`-t`): Windows `msvc` (default), `clang`, `gcc`; Linux `gcc` (default), `clang`, `asan`, `coverage`;
macOS `appleclang` (default), `asan`; all `tidy`. Configs (`-c`): `Debug`, `Release`, `RelWithDebInfo`.

**Before finishing a change, run `./scripts/check.sh`** (or at least `--quick`) and fix every failure.

## Rules that break the build or CI

- Warnings are errors on every compiler (`/W4 /permissive-`, `-Wall -Wextra -pedantic ...`).
- clang-tidy (`.clang-tidy`) and cppcheck fail on any finding. Fix the code; only suppress with an inline
  `// NOLINT(<check>)` or `// cppcheck-suppress <id>` plus a comment explaining why.
- Formatting must match clang-format **20** (the LLVM version used everywhere) with `.clang-format`: 4 spaces,
  140 columns, Stroustrup braces.
- Tests must pass in **Debug and Release**: `TEMPLATE_ASSERT` and the `TEMPLATE_PROFILE_*` macros are compiled out
  outside Debug, so never put required side effects in an assert.
- Code must build with MSVC, GCC and Clang. Avoid compiler-specific extensions and C++23 library features that
  MSVC or AppleClang's libc++ don't support yet.

## Conventions

- Follow [guidelines.md](guidelines.md). Naming (enforced by clang-tidy): classes `CamelCase`, functions/methods
  `camelBack`, private members `m_name`, static members `s_name`.
- Every macro starts with `TEMPLATE_`. Code lives in `namespace core` (or `core::<area>`).
- New source files: add them to the target's `CMakeLists.txt`, start with the SPDX/file header used by existing files,
  and add tests in `tests/` (register the file in `tests/CMakeLists.txt`).
- Dependencies: add them to `vcpkg.json` and use `find_package(... CONFIG REQUIRED)`; they are linked statically.
- Configure new targets with `template_configure_target(<target>)` (warnings, sanitizers, coverage).
- Log with `TEMPLATE_CORE_*` / `TEMPLATE_APP_*` (fmt syntax), not `std::cout`, except for CLI output like `--version`.

## Don'ts

- Don't edit `external/`, `build/` or generated files (`build/**/generated/core/git_info.h`).
- Don't change `VCPKG_COMMIT` in `scripts/setup.sh` without changing `builtin-baseline` in `vcpkg.json` to match.
- Don't weaken warnings, `.clang-tidy`, `.cppcheck-suppressions` or CI to make a change pass.

## Environment notes

- Windows: clang-cl presets (`-t clang`, `-t tidy`, so also `check.sh`) need a *Developer PowerShell for VS 2022*;
  MinGW (`-t gcc`) needs an MSYS2 MINGW64 shell.
- The first configure of a preset downloads and builds dependencies (several minutes); later ones use vcpkg's cache.
