# Code Quality

The [Linter CI](ci.md) runs clang-format, clang-tidy and cppcheck, and fails on any finding. Run them locally before
pushing to avoid surprises.

CI uses **LLVM 18** (`clang-format-18`, `clang-tidy-18`). Use the same major version locally for identical results,
e.g. `pip install clang-format==18.1.8`.

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
