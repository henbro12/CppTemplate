# Core Library

`core` (`template::core`) is the example library the app and tests link against. Include
[`core_inc.h`](../core/src/core_inc.h) to get everything below.

## Logging

Thin wrapper around [spdlog](https://github.com/gabime/spdlog): call `core::Logger::init()` at startup and
`core::Logger::shutdown()` at exit, then log with `TEMPLATE_CORE_*` / `TEMPLATE_APP_*`
(`TRACE`, `DEBUG`, `INFO`, `WARN`, `ERROR`, `CRITICAL`), using `fmt` syntax:

```cpp
TEMPLATE_APP_INFO("Loaded {} items in {} ms", count, ms);
```

## Asserts and profiling

Active in **Debug** builds of every target that links `template::core`, which exports `TEMPLATE_ENABLE_ASSERTS` and
`TEMPLATE_ENABLE_PROFILER` for that configuration:

- `TEMPLATE_ASSERT(cond, log_macro, fmt, args...)` logs and breaks into the debugger; `cond` is not evaluated when disabled.
- `TEMPLATE_VERIFY(cond, log_macro, fmt, args...)` is the same, but `cond` is always evaluated.
- `TEMPLATE_PROFILE_SCOPE(name)`, `TEMPLATE_PROFILE_CALL(name, expr)`, `TEMPLATE_PROFILE_CALL_V(name, expr)` and
  `TEMPLATE_PROFILE_START(name)` / `TEMPLATE_PROFILE_END(name)` log timings at trace level; `expr` is always evaluated.

To profile another configuration, define `TEMPLATE_ENABLE_PROFILER` for your target.

## Version info

[`core/version.h`](../core/src/core/version.h) exposes the version from `project(... VERSION x.y.z ...)` and the git
state of the build:

```cpp
core::versionString();             // "CppTemplate 0.1.0 (v0.1.0-3-g1a2b3c4d-dirty, Release)"
core::versionInfo().git_commit;    // "1a2b3c4d"
core::versionInfo().version_major; // 0
```

The app prints it with `--version` and logs it at startup. The git details come from
`git describe --tags --always --dirty` and are refreshed on every build; only `version.cpp` is recompiled, and only when
they change. Without git (e.g. a source archive) they read `unknown`. Tag releases (`git tag v1.2.0`) for readable
describe strings.
