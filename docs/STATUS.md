# CodeBreaker — where it stands, and what is next

Written 2026-10-13. Numbers are from runs on this machine against the Windows 95 corpus and
twelve PowerPC sample images; each says which run it is from.

## What the tool does today

One path for every kind of code: decompile, rebuild, run the rebuild beside the original on the
same inputs, compare what both did, record a verdict per function, and write the source.

| Kind of code | Output | How it is judged |
|---|---|---|
| 32-bit Windows | C, struct fields as fields, C++ classes described | behaviour, function by function |
| 16-bit Windows | C | behaviour, function by function |
| PowerPC | C with real import names, tables of functions described | behaviour, function by function |
| Device drivers | assembly source that builds back | byte for byte, and behaviour of each entry point |
| DOS programs | assembly source that builds back | byte for byte |

Beside that path:

- **A judge that is itself tested** (`teeth`): functions it passed are broken on purpose and it
  is asked again.
- **Toward the authored source** (`match`, `repair`): three compilers of the time are wired in
  (Microsoft's 32-bit C of 1994, Microsoft C 6.0a for 16-bit code, the XENIX C compiler of the
  1984 DOS tools), with a search over rewrites that change no meaning.
- **Speed**: nothing is compiled or decompiled twice; a verdict holds until the function or
  something it calls changes; the 32-bit emulator runs about six times faster than it did.

## The numbers

| Kind | Modules | Functions | Verified | Verified (weak) | Unverified | Not judged | Does not compile |
|---|---|---|---|---|---|---|---|
| DOS programs | 127 of 128 | 17,058 | 16,295 (96%) | — | 0 | — | 763 (two programs) |
| Device drivers | 214 of 214 | 1,292 | 1,113 (86%) | 159 (12%) | 4 | — | 0 |
| 16-bit Windows | 317 of 326 | 36,758 | 6,139 (17%) | 7,224 (20%) | 19,468 (53%) | — | 3,611 (10%) |
| 32-bit Windows, first pass | 558 of 559 | 151,556 | 28,642 (19%) | 28,678 (19%) | 34,968 (23%) | 16,105 (11%) | 42,794 (28%) |
| PowerPC, 12 images | 12 | 3,597 | 1,077 (30%) | 1,724 (48%) | 728 (20%) | — | 67 |

Notes on reading these:

- **32-bit is stale.** That pass ran before today's fixes to five causes that accounted for most
  of its "does not compile" and a large share of its "unverified". On the modules checked by
  hand the change is large (calc.exe: nothing judged → 97 passing; wordpad: 632 unverified →
  369). A second full pass is running; its numbers replace this row.
- **16-bit** is after the 64K fix (before it, 44% did not compile) but before the far-pointer
  fix; a third pass is queued.
- **"Verified (weak)"** means it matched with a stated limit — most often that under half of
  the function's instructions ran on any input.
- **DOS and drivers** are judged by rebuilding the whole file from assembly source, so a module
  is all-or-nothing; that is why DOS has no "unverified".

## How far to trust "verified"

On advapi32 the judge catches 13 of 16 functions deliberately broken after it had called them
verified (it was 11 of 16 before two holes were closed). The three that survive are breaks
nothing outside the function can observe. This has been run on one module only.

## Toward the authored source

- **1984 DOS tools (the ground truth):** the search brings back 289 of 289 functions knocked
  away from their known original source.
- **32-bit Windows (comctl32, first 200 functions):** 37 come out byte-identical with the 1994
  compiler. It was 4 before the compiler was given the declarations an author would have had
  and the switches the module was built with. The rewrites that reorder or rephrase (operand
  order, declarations, `switch`, early returns, folded temporaries) add nothing on their own:
  this compiler normalises them away.
- **16-bit Windows:** the compiler runs and compares; no function matches yet on the two
  modules tried.

## Known gaps

- **16-bit behaviour (53% unverified).** The main cause is the decompiler showing far pointers
  as two unrelated numbers. A first fix reads the pointer loads from the original's code;
  functions that reach near into another segment or take their own stack's address still fail.
- **32-bit.** References to a function's own stack address do not compile; rebuilt whole DLLs
  are not yet tested running in place of the original.
- **PowerPC.** Images carry no class names; the remaining faults are mostly calls through
  pointers that are null in the test setup.
- **DOS.** `command.com` and `dosx.exe` each have one block of code over 64K, which the
  assembler cannot emit as one segment. The kernel file (`winboot.sys`) is not attempted.
- **Drivers.** Four entry points in two drivers differ; 16 have no source; 16-bit code inside
  drivers is written as bytes.
- **C++.** Classes are described in C++ syntax beside the C; methods are not compiled as
  member functions.
- **Struct fields** are written for accesses through arguments only, with positional names.

## Roadmap

1. **Finish measuring.** Let the second 32-bit pass and the third 16-bit pass finish; replace
   the stale rows above. Run `teeth` on five or six large modules for a trust figure that does
   not rest on one module.
2. **16-bit far pointers, properly.** Tell the decompiler which argument pairs are far pointers
   (from the same loads the text fix reads) so it types them itself, instead of repairing its
   text afterwards. This is the largest single block of unverified functions in the corpus.
3. **32-bit: the next causes.** After the second pass, rank again. Expected next: functions
   that take the address of their own stack, and calling conventions that pass in registers.
4. **Whole modules that run.** A rebuilt DLL loaded in place of the original is the real test
   of "behaves as the original"; today only version.dll-sized modules are built whole.
5. **Authored source.** The rewrites a normalising compiler does respond to are about types
   and declarations, not statement shape: parameter types, structure types in place of
   offsets, the real headers' prototypes. That is where the next exact matches are.
6. **C++ as C++.** Compile recovered classes as classes for modules where the binary names
   them (MFC programs first).
7. **Other products.** Windows 1.0, 3.1 and 98 are configured and unmeasured.

## Housekeeping decisions (the owner's)

- The Windows95 repository has uncommitted generated files (module sources, SCOREBOARD.md).
- This repository has no remote: all history is local.
