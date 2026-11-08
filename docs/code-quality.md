# Code Quality

The [Linter CI](ci.md) runs clang-format, clang-tidy and cppcheck, and fails on any finding. To run all of them plus
the Debug and Release tests locally:

```bash
./scripts/check.sh           # everything (on Windows: from a Developer shell for VS 2022)
./scripts/check.sh --quick   # formatting + Debug tests only
```

It runs every step even when one fails and prints a summary. Steps whose tool isn't installed are skipped with a
warning; add `--strict` to treat that as a failure. The sections below show how to run each tool on its own.

The project uses **LLVM 20** everywhere it controls the version: clang-format, clang-tidy, the Linux Clang and
sanitizer builds, and clang-cl on Windows. CI installs it explicitly (`LLVM_VERSION` in the workflows). Use LLVM 20
locally too, e.g. from [releases.llvm.org](https://releases.llvm.org) or `pip install clang-format==20.1.8`.
AppleClang on macOS follows the installed Xcode and can't be pinned this way.

## clang-format

Style is defined in [`.clang-format`](../.clang-format).

```bash
clang-format --dry-run --Werror $(git ls-files '*.cpp' '*.h' '*.hpp')   # check
clang-format -i $(git ls-files '*.cpp' '*.h' '*.hpp')                   # fix
```

## clang-tidy

Rules are defined in [`.clang-tidy`](../.clang-tidy). It runs on a dedicated tidy build (Clang, no precompiled header):

```bash
./scripts/build.sh -t tidy
run-clang-tidy -p build/<win|linux|mac>-tidy core app tests
```

## cppcheck

cppcheck uses the tidy build's compile database, so it sees the same include paths and defines as the compiler:

```bash
./scripts/build.sh -t tidy
cppcheck --project=build/<win|linux|mac>-tidy/compile_commands.json \
  --enable=warning,style,performance,portability \
  --inline-suppr --suppressions-list=.cppcheck-suppressions \
  --error-exitcode=1 -q
```

Suppress a finding with `// cppcheck-suppress <id>` on the line before it, or project-wide in
[`.cppcheck-suppressions`](../.cppcheck-suppressions). CI uses Ubuntu 24.04's `cppcheck` package; other versions may
report slightly different findings.

## Sanitizers and coverage

The `asan` toolchain (Linux, macOS) builds the project's own targets with `-fsanitize=address,undefined`;
dependencies from vcpkg are not instrumented:

```bash
./scripts/build.sh -t asan --test
```

Other sanitizers can be selected with the `TEMPLATE_SANITIZERS` cache variable, e.g.
`cmake --preset linux-asan -DTEMPLATE_SANITIZERS=thread`.

The `coverage` toolchain (Linux, GCC) builds with `--coverage`. Generate a report after the tests, e.g. with
[gcovr](https://gcovr.com):

```bash
./scripts/build.sh -t coverage --test
gcovr --root . --filter core/ --filter app/ build/linux-coverage
```
