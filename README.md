# C++ CMake Template

[![GitHub Release][releases-shield]][releases]
[![License][license-shield]](LICENSE)

A modern C++ project template using **CMake presets**, **vcpkg**, and **Ninja** — designed to quickly bootstrap new projects with support for **MSVC**, **GCC**, and **Clang**.

---

## 💡 Coding Guidelines

We follow a set of internal C++ guidelines to ensure code quality and consistency.
They’re based on [Jan Wilmans' C++ Guidelines](https://github.com/janwilmans/guidelines) (MIT licensed), with modifications for this project.

👉 See [guidelines.md](./guidelines.md) for the full list.

---

## 🛠️ Getting Started

### 1. Install dependencies

Before you start, make sure the following tools are installed:

- **CMake ≥ 3.25**
- **Ninja** (build system)
- **vcpkg** (bootstrapped automatically by `setup.sh`)
- **Visual Studio Build Tools** — (for MSVC builds on Windows)
- **LLVM/Clang** *(optional)* — (for Clang builds)
- **MSYS2** *(optional)* — (for GCC/MinGW builds)

Then set up the project:

```bash
./scripts/setup.sh
```

---

## ⚙️ Building the Project

Three toolchains are supported:

| Toolchain    | Preset name   | Description                                       |
|--------------|---------------|---------------------------------------------------|
| **MSVC**     | `msvc`        | Microsoft Visual C++ compiler                     |
| **GCC**      | `gcc`         | GCC from MSYS2 / MinGW                            |
| **Clang**    | `clang`       | LLVM Clang (installed to `C:\Program Files\LLVM`) |

And three build configurations:

- `Debug` — Debug symbols, assertions enabled, `_DEBUG` defined
- `Release` — Optimized, assertions disabled
- `RelWithDebInfo` — Release build with debug info

---

### 🧪 Build with Scripts (recommended)

Use the provided build script to configure & build the project easily:

```bash
./scripts/build.sh <toolchain> <configuration> [target]
```

Examples:

```bash
# Build the full project with MSVC in Debug mode
./scripts/build.sh msvc Debug

# Build with GCC in Release mode
./scripts/build.sh gcc Release

# Build only the unit tests with Clang in Debug mode
./scripts/build.sh clang Debug unit-tests
```

If you omit arguments, it defaults to:

```bash
./scripts/build.sh gcc Debug
```

---

## ▶️ Running the Application

You can run the main application with:

```bash
./scripts/run.sh <toolchain> <configuration>
```

Examples:

```bash
# Run the app built with MSVC Debug
./scripts/run.sh msvc Debug

# Run the app built with GCC Release
./scripts/run.sh gcc Release
```

---

## 🧪 Running Tests

To build and run unit tests:

```bash
./scripts/build.sh gcc Debug unit-tests
ctest --preset gcc-tests
```

Or simply run the test executable directly:

```bash
./build/gcc/tests/Debug/unit-tests.exe
```

---

## 📁 Output Structure

Each toolchain/configuration combination has its own build folder:

```
build/
├─ msvc/
│   └─ app/Debug/CppTemplateApp.exe
├─ gcc/
│   └─ app/Debug/CppTemplateApp.exe
└─ clang/
    └─ app/Release/CppTemplateApp.exe
```

This ensures different compiler builds never clash.

---

## 📦 Dependencies

Dependencies are managed via [vcpkg](https://github.com/microsoft/vcpkg):

- [**spdlog**](https://github.com/gabime/spdlog) — high-performance logging
- [**Catch2**](https://github.com/catchorg/Catch2) — unit testing framework

---

## 📜 License

This project is licensed under the [MIT License](./LICENSE).

---

✅ **Tip:** You can also build and run directly inside VS Code by selecting a **Configure Preset** (toolchain) and **Build Preset** (configuration) from the status bar — no scripts needed.

---

[releases-shield]: https://img.shields.io/github/v/release/henbro12/CppTemplate?style=for-the-badge
[releases]: https://github.com/henbro12/CppTemplate/releases
[license-shield]: https://img.shields.io/github/license/henbro12/CppTemplate?style=for-the-badge
