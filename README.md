# C++ Cross-Platform CMake Template

[![GitHub Release][releases-shield]][releases]
[![License][license-shield]](LICENSE)

A **modern, cross-platform C++ project template** using **CMake presets**, **vcpkg**, and **Ninja** — designed to quickly bootstrap new projects on **Windows**, **Linux**, and **macOS** with support for **MSVC**, **GCC**, and **Clang**.

This template automatically detects your host OS and uses the matching compiler toolchain and preset when building via the provided scripts.

---

## Coding Guidelines

We follow internal C++ guidelines based on [Jan Wilmans' C++ Guidelines](https://github.com/janwilmans/guidelines) (MIT licensed), with minor modifications.

See [guidelines.md](./guidelines.md) for the full list.

---

## Cross-Platform Overview

| Operating System | Default Toolchain | Other Supported Toolchains |
|------------------|------------------|-----------------------------|
| **Windows** | MSVC (`cl.exe`) | LLVM **Clang-CL**, MSYS2 **MinGW-GCC** |
| **Linux** | GCC | LLVM **Clang** |
| **macOS (Apple Silicon)** | AppleClang (`clang`) | - |

> The `build.sh` and `run.sh` scripts **automatically detect your OS** and select the correct compiler preset.
> You can override it manually with `-t <toolchain>` if you want to test a specific compiler.

---

## Getting Started

### 1. Install dependencies

Make sure the following tools are installed on your system:

#### Windows
- **Visual Studio Build Tools 2022** (for MSVC)
- **LLVM/Clang** *(optional)* – for Clang builds
- **MSYS2** *(optional)* – for GCC/MinGW builds
- **CMake ≥ 3.25**
- **Ninja**
- **Git**

#### Linux
- **GCC** or **Clang**
- **CMake ≥ 3.25**
- **Ninja**
- **Git**

#### MacOS
- **Xcode Command Line Tools** (provides AppleClang)
- **CMake ≥ 3.25**
- **Ninja**
- **Git**
- *(Optional)* **Homebrew** for package installs

Then run once to set up **vcpkg** and dependencies:

```bash
./scripts/setup.sh
```

This bootstraps vcpkg under `external/vcpkg` and installs required dependencies defined in `vcpkg.json`.

---

## Building the Project

### Supported Toolchains

| OS | Toolchain | Preset | Description |
|----|-----------|--------|-------------|
| Windows | **msvc** | `win-msvc` | Visual Studio 17 2022 |
| Windows | **clang** | `win-clangcl` | LLVM Clang using MSVC ABI/linker |
| Windows | **gcc** | `win-mingw-gcc` | GCC via MSYS2/MinGW (static triplet) |
| Linux | **gcc** | `linux-gcc` | GNU Compiler Collection on Linux |
| Linux | **clang** | `linux-clang` | LLVM Clang on Linux |
| MAC | **appleclang** | `mac-appleclang` | AppleClang on macOS (arm64) |

### Supported Configurations
- `Debug` — full debug symbols, assertions enabled
- `Release` — optimized build, assertions disabled
- `RelWithDebInfo` — optimized with debug info

---

### Build with Scripts (Recommended)

The build scripts automatically detect your OS and compiler:

```bash
./scripts/build.sh [-t <toolchain>] [-c <config>] [--test]
```

Examples:

```bash
# Default build (auto-detected toolchain, Debug)
./scripts/build.sh

# Explicitly build MSVC Release
./scripts/build.sh -t msvc -c Release

# Build and run unit tests (Debug only)
./scripts/build.sh --test
```

---

## Running the Application

Use the run script to automatically build (if needed) and execute the app:

```bash
./scripts/run.sh [-t <toolchain>] [-c <config>] [-- <args>]
```

Examples:

```bash
# Run with auto-detected OS toolchain (Debug)
./scripts/run.sh

# Run Clang-CL build in Release mode
./scripts/run.sh -t clangcl -c Release

# Pass arguments to the app
./scripts/run.sh -- --help
```

**Tip:** You can also open this project in **VS Code** and use CMake Tools’ “Configure Preset” and “Build Preset” selectors to build and run directly — no scripts required.

---

## Running Tests

Unit tests use [Catch2](https://github.com/catchorg/Catch2) and are automatically built when `BUILD_TESTING` is enabled.

To build and run tests manually:

```bash
./scripts/build.sh --test
```

Or invoke directly:

```bash
ctest --preset <preset-name> --output-on-failure
```

Example:
```bash
ctest --preset linux-gcc-tests
```

---

## Code Quality Checks

The project includes built-in support for **clang-tidy** (static analysis) and **clang-format** (code style enforcement).
These are automatically executed in CI, but you can also run them locally.

### Running clang-tidy

`clang-tidy` performs static analysis and enforces modern C++ best practices using the rules defined in [`.clang-tidy`](./.clang-tidy).

To run it manually:

```bash
# Create a tidy build (auto-selects correct toolchain per OS)
./scripts/build.sh -t tidy

# Run clang-tidy on all source files
run-clang-tidy -p build/<win|linux|mac>-tidy core app tests
```

---

### Running clang-format

`clang-format` ensures all source files follow the style defined in [`.clang-format`](./.clang-format).

Check for formatting issues:

```bash
clang-format --dry-run --Werror \
  $(find core app tests -name '*.cpp' -o -name '*.hpp')
```

Automatically fix formatting:

```bash
clang-format -i \
  $(find core app tests -name '*.cpp' -o -name '*.hpp')
```

---

**Tip:** Run both tools before committing to ensure your code passes the **Linter CI** checks automatically.

---

## Output Structure

All builds are separated by toolchain and configuration to prevent conflicts:

```
build/
├─ win-msvc/
│   ├─ app/Debug/CppTemplateApp.exe
│   └─ app/Release/CppTemplateApp.exe
├─ win-clangcl/
│   ├─ app/Debug/CppTemplateApp.exe
│   └─ app/Release/CppTemplateApp.exe
├─ win-mingw-gcc/
│   ├─ app/Debug/CppTemplateApp.exe
│   └─ app/Release/CppTemplateApp.exe
├─ linux-gcc/
│   ├─ app/Debug/CppTemplateApp
│   └─ app/Release/CppTemplateApp
├─ linux-clang/
│   ├─ app/Debug/CppTemplateApp
│   └─ app/Release/CppTemplateApp
└─ mac-appleclang/
    ├─ app/Debug/CppTemplateApp
    └─ app/Release/CppTemplateApp
```

---

## Dependencies

Dependencies are managed through **vcpkg** and defined in [`vcpkg.json`](./vcpkg.json):

- [**spdlog**](https://github.com/gabime/spdlog) — high-performance logging
- [**Catch2**](https://github.com/catchorg/Catch2) — lightweight unit testing

Each toolchain has an isolated `vcpkg_installed` directory under its build folder, ensuring clean separation between compilers and architectures.

---

## Using This Repository as a Template

If you want to start a new project from this template:

1. Click **“Use this template”** on GitHub.
2. Update project metadata in `CMakeLists.txt`:
   ```cmake
   project(MyProject VERSION 0.1 LANGUAGES CXX)
   ```
3. Replace `core/` and `app/` sources with your own.
4. Update `vcpkg.json` for your dependencies.

Included configuration files:
- `.clang-format` — C++ code style
- `.editorconfig` — editor settings
- `.github/workflows/ci.yml` — GitHub Actions CI for all platforms
- `CMakePresets.json` — unified build configurations

---

## Continuous Integration (CI)

The provided **GitHub Actions** workflows ensure that every commit and pull request is automatically verified across compilers, platforms, and code quality tools.

### Build & Test Workflow (`ci.yml`)

This workflow:

* Builds **MSVC**, **Clang-CL**, and **MinGW (GCC)** on **Windows**
* Builds **GCC** and **Clang** on **Linux**
* Builds **AppleClang (arm64)** on **macOS**
* Runs **unit tests** for all toolchains in `Debug` mode
* Uploads **application artifacts** for every compiler/configuration combination

### Linter Workflow (`linter.yml`)

This workflow ensures code style and quality using multiple static analysis tools:

| Tool             | Purpose         | Description                                                                                                                     |
| ---------------- | --------------- | ------------------------------------------------------------------------------------------------------------------------------- |
| **clang-format** | Code style      | Checks source files against the `.clang-format` style; fails if formatting is incorrect (`--dry-run --Werror`).                 |
| **clang-tidy**   | Static analysis | Runs code analysis on a dedicated **tidy build** (no PCH, Debug); enforces modern C++ best practices (`WarningsAsErrors: '*'`). |
| **cppcheck**     | Static analysis | Performs an additional lightweight static code scan to catch issues missed by other tools.                                      |

The **Linter CI** runs automatically on every **push** and **pull request**, ensuring consistent style, clean code, and no regressions before merging.

---

## License

This project is licensed under the [MIT License](./LICENSE).

---

[releases-shield]: https://img.shields.io/github/v/release/henbro12/CppTemplate?style=for-the-badge
[releases]: https://github.com/henbro12/CppTemplate/releases
[license-shield]: https://img.shields.io/github/license/henbro12/CppTemplate?style=for-the-badge
