# Continuous Integration

Two GitHub Actions workflows run on pull requests, pushes to `main` and manual runs (`workflow_dispatch`).

## Build (`build.yml`)

Builds every toolchain with the same scripts you use locally (`setup.sh`, then `build.sh --test`) and uploads the app
for each job. Pull requests run a reduced matrix to stay fast:

| | Pull requests | Pushes to `main`, manual runs |
|---|---|---|
| Windows | MSVC Debug + Release, clang-cl Debug | MSVC, clang-cl, MinGW × Debug, Release |
| Linux | x64: GCC Debug + Release, Clang Debug, ASan | x64 + arm64: GCC, Clang × Debug, Release, plus ASan |
| macOS | AppleClang Debug | AppleClang Debug, Release |

Change this with the `exclude:` lines in [`build.yml`](../.github/workflows/build.yml). If you use branch protection,
only require checks that also run on pull requests.

Built dependencies are cached (vcpkg binary cache in `actions/cache`), so they are only rebuilt when `vcpkg.json` or
the vcpkg pin changes.

## Linter (`linter.yml`)

One job that checks formatting, builds the tidy preset, then runs clang-tidy and cppcheck on it. Each tool fails the run on
any finding. See [code-quality.md](code-quality.md) to run the same checks locally.
