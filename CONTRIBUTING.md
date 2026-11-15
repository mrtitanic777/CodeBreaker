# Contributing to CodeBreaker

Thank you for wanting to help. CodeBreaker takes programs back to readable source and proves the source right by
running it beside the original; every change is held to the same standard: it builds, it is tested, and it does not
change what the judge decides unless that is the point of the change.

## Before you start

- Everyone taking part is expected to follow the [Code of Conduct](CODE_OF_CONDUCT.md).
- Bugs and ideas go in the issue tracker; the bug and feature templates ask for what helps (the file, the command,
  what you expected).

- Read the [README](README.md) for what CodeBreaker does and how it is laid out, and
  [docs/ROADMAP.md](docs/ROADMAP.md) for where it is going. If you plan something large, open an issue first so it
  can fit the roadmap.
- New platforms: [docs/LANES.md](docs/LANES.md) is the lane interface, with worked examples and a checklist; a lane is one registration object
  (`register_lane({...})` in its own `engine/machine_<name>.cpp`) and one line in `engine/lane_list.cpp`.
- C++ style: [docs/CPP_DESIGN.md](docs/CPP_DESIGN.md).

## Setting up

You need Windows 10 or 11 (x64), Git, CMake and Ninja, and a C++20 compiler: MSVC (Visual Studio 2022 or 2026),
Clang (`clang-cl`) or GCC (MinGW-w64). The engines' NASM is built with MinGW-w64 GCC (MSYS2). The Java lane needs JDK 21;
the terminal interface needs Python 3.10+ with `textual`.

```bash
tools\build_native.cmd
```

```bash
tools\build_codebreaker_exe.cmd
```

Copy `settings.example.json` to `settings.json` and point it at your tools; `codebreaker settings` shows what was
found. Copyrighted inputs (original binaries, period compilers) are never in the repository: see
[docs/NOT_IN_REPO.md](docs/NOT_IN_REPO.md). Tests that need them look in `binaries/test-inputs/` or in checkouts beside
this one, and skip themselves when they are not there.

CodeBreaker's changes to OpenWatcom are on the `codebreaker` branch of
[mrtitanic777/open-watcom-v2](https://github.com/mrtitanic777/open-watcom-v2); build OpenWatcom from that branch.

## Making a change

- **C++ only, idiomatic and typed.** CodeBreaker is C++20 in `native/cb`; the terminal interface in `codebreaker/ui`
  is the only Python. New code uses the portable layer in `native/cb/core` (`cb::fs`, `cb::process`, `cb::dynlib`,
  ...), not the Windows API directly, so it can build on Linux too.
- **Every compiler.** Code must compile with MSVC (warnings are errors), clang-cl and GCC. CI builds all three.
- **Tests beside the code.** A change comes with `*_test.cpp` tests next to the code it changes; inputs go in a
  `testdata/` folder beside them. Tests that need a file that is not in the repository skip themselves without it.
- **Comments say what the code does and why,** not its history.
- **Behaviour is proven, not assumed.** A change to the decompiler, the builder or the judge must leave verdicts the
  same unless changing them is the point; say which and show it (the same `measure` output before and after, or the
  milestone regression for the Windows 95 lane).
- **Fix bugs where they are.** A wrong behaviour is fixed, not preserved for compatibility.

## Before you open a pull request

```bash
tools\gate.cmd
```

The gate builds everything and runs the unit tests, every command case against its reviewed golden output
(`tests/exe_gate`), and the interface's tests. It must pass. If your change rightly changes a command's output,
re-record that case's golden with `tools\gate.cmd --update --filter <case>` and review the diff before committing it.
Changes to the Windows 95 modules also need `codebreaker regress all15 --runs 2` (and `setup` / `install` where they
are touched).

Add a line to `CHANGELOG.md` under **Unreleased** for anything a user would notice. Keep a commit to one change, with a
message that says what changed and why.

## Platforms and test programs

A new platform is proven with real programs: open-source software with published source and a licence that allows it
(homebrew for consoles), kept outside the repository and listed in
[docs/TESTED_SOFTWARE.md](docs/TESTED_SOFTWARE.md) with its source and licence. Never add retail games or other
copyrighted binaries to the repository.

## Licence

By contributing, you agree that your contribution is licensed under the [Apache License 2.0](LICENSE), as the rest of
CodeBreaker is.
