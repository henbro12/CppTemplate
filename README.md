# C++ Cross-Platform CMake Template

[![GitHub Release][releases-shield]][releases]
[![License][license-shield]](LICENSE)

A **modern, cross-platform C++23 project template** using **CMake presets**, **vcpkg** and **Ninja**, to quickly
bootstrap new projects on **Windows**, **Linux** and **macOS** with **MSVC**, **GCC** and **Clang**. It comes with unit
tests, static analysis, sanitizers and CI for every platform.

| OS | Default toolchain | Also supported | Architectures |
|----|-------------------|----------------|---------------|
| **Windows** | MSVC | clang-cl, MinGW GCC (MSYS2) | x64 |
| **Linux** | GCC | Clang | x64, arm64 |
| **macOS** | AppleClang | - | arm64, x64 |

---

## Quick Start

You need CMake ≥ 3.25, Ninja, Git and a compiler (see [requirements](docs/building.md#requirements)). On Windows, run
the scripts from Git Bash.

```bash
./scripts/setup.sh             # once: fetches and bootstraps the pinned vcpkg
./scripts/build.sh             # build with your OS's default toolchain (Debug)
./scripts/build.sh --test      # build and run the unit tests
./scripts/run.sh               # build and run the app
```

Pick another toolchain or configuration with `-t` and `-c`:

```bash
./scripts/build.sh -t clang -c Release --test
./scripts/run.sh -- --version
```

> **Windows:** clang-cl needs a *Developer PowerShell for VS 2022* (start `bash` from there); MinGW GCC needs an
> **MSYS2 MINGW64** shell. See [Windows shells](docs/building.md#windows-shells).

You can also open the folder in **VS Code** and pick a configure/build preset in CMake Tools.

---

## Using This Repository as a Template

1. Click **“Use this template”** on GitHub.
2. Set your project name and version in `CMakeLists.txt`: `project(MyProject VERSION 0.1.0 LANGUAGES CXX)`.
3. Replace the `core/` and `app/` sources with your own.
4. List your dependencies in `vcpkg.json`.

---

## Documentation

- [Building](docs/building.md): requirements, presets, vcpkg, linking and installing
- [Code quality](docs/code-quality.md): clang-format, clang-tidy, cppcheck, sanitizers and coverage
- [CI](docs/ci.md): the GitHub Actions workflows
- [Core library](docs/core.md): logging, asserts, profiling and version info
- [Coding guidelines](guidelines.md): based on [Jan Wilmans' C++ Guidelines](https://github.com/janwilmans/guidelines)

---

## License

This project is licensed under the [MIT License](./LICENSE).

[releases-shield]: https://img.shields.io/github/v/release/henbro12/CppTemplate?style=for-the-badge
[releases]: https://github.com/henbro12/CppTemplate/releases
[license-shield]: https://img.shields.io/github/license/henbro12/CppTemplate?style=for-the-badge
