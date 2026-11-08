# Building

Everything is driven by [`CMakePresets.json`](../CMakePresets.json). The scripts in `scripts/` pick the right preset for
your OS, but you can use the presets directly with `cmake`/`ctest` or from your IDE (e.g. VS Code CMake Tools).

## Requirements

| | Windows | Linux | macOS |
|---|---|---|---|
| Compiler | Visual Studio Build Tools 2022; optional LLVM 20 (clang-cl), MSYS2 (MinGW GCC) | GCC or Clang 20 | Xcode Command Line Tools |
| Tools | CMake ≥ 3.25, Ninja, Git (Git Bash runs the scripts) | CMake ≥ 3.25, Ninja, Git | CMake ≥ 3.25, Ninja, Git |

### Windows shells

- **MSVC** (`win-msvc`) uses the Visual Studio generator and works from any shell.
- **clang-cl** (`win-clangcl`, `win-tidy`) uses Ninja, which needs the MSVC environment: run the scripts from a
  *Developer PowerShell / Command Prompt for VS 2022* (start `bash` from there), or call `vcvars64.bat` first.
- **MinGW GCC** (`win-mingw-gcc`) uses the `g++` and `ninja` on `PATH`: run the scripts from an **MSYS2 MINGW64** shell,
  or put `<msys2>/mingw64/bin` first on `PATH`. vcpkg builds the dependencies with the same compiler, so make sure no
  other GCC comes first.

## vcpkg

`./scripts/setup.sh` checks out a pinned vcpkg under `external/vcpkg` and bootstraps it. Dependencies from
[`vcpkg.json`](../vcpkg.json) are installed into `build/<preset>/vcpkg_installed` on the first configure.

- The vcpkg commit (`VCPKG_COMMIT` in `scripts/setup.sh`) and `builtin-baseline` in `vcpkg.json` pin all dependency
  versions; update both together to upgrade.
- The presets always use this checkout. A `VCPKG_ROOT` in your environment (Visual Studio developer shells set one) is
  ignored, so every machine builds with the same vcpkg. To use another one, add a `CMakeUserPresets.json` that inherits a
  preset and overrides `toolchainFile`.

## Presets

| OS | `-t` toolchain | Configure preset | Notes |
|----|----------------|------------------|-------|
| Windows | `msvc` (default) | `win-msvc` | Visual Studio 17 2022 |
| Windows | `clang` | `win-clangcl` | clang-cl, MSVC ABI and linker |
| Windows | `gcc` | `win-mingw-gcc` | MSYS2 MinGW GCC |
| Linux | `gcc` (default) | `linux-gcc` | |
| Linux | `clang` | `linux-clang` | |
| macOS | `appleclang` (default) | `mac-appleclang` | |
| All | `tidy` | `win-tidy`, `linux-tidy`, `mac-tidy` | Clang without PCH, for [clang-tidy](code-quality.md) |
| Linux, macOS | `asan` | `linux-asan`, `mac-asan` | [AddressSanitizer + UBSan](code-quality.md#sanitizers-and-coverage) |
| Linux | `coverage` | `linux-coverage` | GCC with `--coverage` |

The first six are multi-config (`Debug`, `Release`, `RelWithDebInfo`); `tidy`, `asan` and `coverage` are Debug-only.
Linux and macOS build for the host architecture (x64 or arm64); Windows builds x64.

- **Build presets** are `<configure-preset>-<Config>`, e.g. `cmake --build --preset linux-gcc-Release`.
- **Test presets** are `<configure-preset>-<Config>-tests`, e.g. `ctest --preset win-msvc-Debug-tests`.
- **Output** goes to `build/<configure-preset>/`, e.g. `build/win-msvc/app/Release/CppTemplateApp.exe`, or
  `build/linux-asan/app/CppTemplateApp` for the single-config presets.

`Debug` enables the [assert and profiler macros](core.md); `Release` and `RelWithDebInfo` disable them. Tests run in
every configuration.

Warnings are errors for the project's own targets. To relax this locally:
`cmake --preset <preset> --compile-no-warning-as-error`.

## Scripts

```bash
./scripts/build.sh [-t <toolchain>] [-c <config>] [--test]
./scripts/run.sh   [-t <toolchain>] [-c <config>] [-- <app args>]
```

Both default to your OS's toolchain and `Debug`, and work from any directory. `run.sh` builds first.
[`check.sh`](code-quality.md) runs the full set of CI checks.

## Linking

Dependencies are linked **statically on every platform**, so the app is a single executable:

| Platform | vcpkg triplet | C/C++ runtime |
|----------|---------------|---------------|
| Windows (MSVC, clang-cl) | `x64-windows-static-md` | shared MSVC runtime (`/MD`), part of Windows / the VC++ redistributable |
| Windows (MinGW) | `x64-mingw-static` | linked statically (`-static`), since MinGW's runtime isn't part of Windows |
| Linux | `x64-linux` / `arm64-linux` | system `libstdc++` and glibc |
| macOS | `arm64-osx` / `x64-osx` | system `libc++` |

To use shared libraries instead (e.g. for plugins or LGPL dependencies), override `VCPKG_TARGET_TRIPLET` in a
`CMakeUserPresets.json` (e.g. `x64-windows`, `x64-linux-dynamic`) and ship the libraries with the app.

## Installing

```bash
cmake --install build/<preset> --config Release --prefix dist
```
