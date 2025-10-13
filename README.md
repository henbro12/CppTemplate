# C++ Cmake Template

[![GitHub Release][releases-shield]][releases]
[![License][license-shield]](LICENSE)

A simple C++ CMake template with some basic features to easily get started on new projects.

---

## 💡 Coding Guidelines

We follow a set of internal C++ guidelines to ensure code quality and consistency.
Our guidelines are based on [Jan Wilmans' C++ Guidelines](https://github.com/janwilmans/guidelines) (MIT Licensed), with modifications for this project.

👉 See [guidelines.md](./guidelines.md) for the full list.

---

## 🛠️ Getting Started

To set up the development environment (install `vcpkg`, dependencies, and configure the project):

```bash
./scripts/setup.sh
```

If you're using Windows, open the `x64 Native Tools Command Prompt for VS 2022` and run:

```bash
code .
```

There are multyple build modes available for building the project in debug, release, dist or test mode.
The project can be build with the desired build mode using the following commands:

```bash
./scripts/build.sh <debug/release/dist/test>
./scripts/run.sh <debug/release/dist/test>
```

---

## ⚙️ Requirements

- **CMake ≥ 3.25**
- **Ninja**
- **VSCode**
- **Visual Studio Build Tools** (on Windows)
- **vcpkg** (automatically bootstrapped)
- *(Linux/macOS only)* `bash`, `git`, optionally `clang`, `lldb`, etc.

---

## 📦 Dependencies

Managed via [vcpkg](https://github.com/microsoft/vcpkg):

- [**spdlog**](https://github.com/gabime/spdlog) — high-performance logging
- [**Catch2**](https://github.com/catchorg/Catch2) — unit testing framework

---

## License
This project is licensed under the [MIT License](./LICENSE).

---
