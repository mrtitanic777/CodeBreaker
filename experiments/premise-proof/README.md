# premise-proof

An end-to-end test of the project's core premise:

> Make **readable, maintainable, modifiable, integral, and compilable** source
> files from the binaries — **not** byte-exact compilations. (Byte-exact is
> trivial — Xcopy.)

The experiment takes one **integral** Win95 component all the way through the
CodeBreaker pipeline and proves it compiles + runs with a **maintainable,
non-Microsoft toolchain** — no `thunk.exe`, no `ml`, no period `cl` required.

## Pipeline under test

1. **binary → readable source** — CodeBreaker's own disassembler/decompiler
   (`native/xexmagic` → `wt --target win95 decompile`), names/types from the
   demangler + value inference (SDK-free).
2. **source → object/module** — compiled by a modern/maintainable compiler:
   **OpenWatcom v2** for the 16-bit segmented (NE) side, clang/gcc/OpenWatcom
   for any 32-bit side. (A CodeBreaker `.thk`→C/C++ converter replaces
   `thunk.exe` for the thunk class — planned.)
3. **verify — behaviorally, not by byte diff**: it compiles, and it loads +
   functions in the Win95 VM. Byte-diff is at most a weak optional signal.

## Success criteria (the five properties)

- **readable / maintainable / modifiable** — the recovered `.c/.h` read as
  authored C a human can edit.
- **integral** — behaves as the original (dynamic check on the oracle / VM).
- **compilable** — builds with OpenWatcom v2 (and/or clang/gcc), no MS tools.

## Step 0 (current): build the compiler

Build **OpenWatcom v2** from source — the maintainable open toolchain that
compiles 16-bit segmented C and emits Win16 NE. Clone lives at
`open-watcom-v2/` (git-ignored; third-party). Once built, it compiles the
reconstructed component in step 2.

Source: https://github.com/open-watcom/open-watcom-v2

## Notes

- This folder is **staging/experiment only** — NOT the canonical Win95 source
  tree. A validated result is promoted into the `Windows95` repo by curated
  placement; nothing here competes with the one source tree.
- Revert anchor for the whole experiment: commit `746c310`.
