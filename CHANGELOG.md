# Changelog

What changed in each release of CodeBreaker. Newest first; [docs/RELEASING.md](docs/RELEASING.md) says how a
release is made. Each entry lists what a user notices: new commands and lanes, fixes, changes in behaviour.

## Unreleased

### Added
- The PlayStation lane (MIPS R3000A): `info` and `reach` take a PS-X EXE; the program comes back as GNU as source (every function, the
  header field by field, strings, tables and hardware registers by name) that CodeBreaker's own MIPS assembler builds back into the
  same executable, byte for byte, and its functions are judged in CodeBreaker's own R3000A interpreter (delay slots, the BIOS calls,
  the GTE's registers and its commands that need no division). 58 open-source PS1 programs rebuild identical (docs/TESTED_SOFTWARE.md).

### Fixed
- A fresh clone builds and passes the gate: `tools\build_native.cmd` builds every engine the program calls (the
  decompiler, XEX and NASM included); tests that need copyrighted or out-of-repo inputs find them in
  `binaries/test-inputs/` or in checkouts beside this one, and are skipped where those are absent.
- `dos-build` and `driver-build` write a link script that names the program relative to its folder, so a build
  leaves the source folder the same on every machine.
- `tidy-state` knows the `origin`, `oss` and `setup` folders as CodeBreaker's own.

### Changed
- CodeBreaker builds with any modern compiler on Windows: MSVC 2022 and 2026, Clang (clang-cl) and GCC (MinGW-w64).
  Compiler flags are chosen per compiler, Windows libraries are linked by name, MSVC-only intrinsics and CRT calls
  have portable forms, and CI builds and tests with all three compilers.
- Builds need no Python: the engines take Capstone's and Unicorn's headers from `native/third_party/include`.
- Python-era design notes moved to `docs/history/`; the current docs give `codebreaker <command>` and the native
  source paths.

## 1.0.0 — 2026-11-14

The first official release: CodeBreaker as one native C++20 program.

- Every command (75) runs in `codebreaker.exe`; the only Python left is the terminal interface (`codebreaker ui`).
- Lanes: 32- and 16-bit Windows (C, or assembly where it was written in assembly), VxD drivers and DOS programs
  (whole-file assembly, rebuilt byte for byte), Xbox 360 (C per function, or whole titles recompiled to C++), XNA
  (C#, content, HLSL), Java (C++03), NES / Master System / Game Gear / Game Boy / Genesis / ZX Spectrum / MSX
  (assembly source), and the universal translator (Java, Python, C# to C / C++17 / C++03).
- Proven: `tools\gate.cmd` (5,450 unit tests, 155 command cases, the interface's tests); `codebreaker regress`
  all15 2/2, setup 2/2, install 2/2 (Windows 95 to the desktop on 22 rebuilt modules), slow (Minecraft Xbox 360
  Edition recompiled and run headless).
- Release package `CodeBreaker-1.0.0-win64.zip`: the program, its engines and data, no tests.
