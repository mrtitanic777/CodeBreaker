# Track 1: Windows 95 from authored source, the CodeBreaker side (plan)

Plan of 2026-11-28, from the audit (docs/WIN95_AUDIT.md, section 9). The Windows95 tree is not changed by this work: CodeBreaker
is changed so that what `codebreaker build` writes for a boot module is source worth keeping, and section 4 lists what would then
go into the tree (a proposal for Logan, not done here).

## What is CodeBreaker's to fix, in the audit's order

| gap | what CodeBreaker changes | how it is proved |
|---|---|---|
| A1 + C3: the boot form is not kept, the recipes are Microsoft C with dead paths | `build` writes, beside its working files, the module's **source form**: one folder that holds only the module's sources (C per function, the assembly pieces, the data, the header, the definition and resource files) and a recipe for OpenWatcom's own `wmake` (wcc, wasm, wlink, every flag the build used, no absolute path) | the source form, copied alone to another folder and made there with `wmake`, links an image byte-identical to the build's (so it is the image `regress` boots) |
| C1: KERNEL and other imports called by ordinal (`KERNEL_18`) | the source form calls each import by the name its exporting module gives it, spelled as the SDK headers spell it (`GlobalLock`); the import directive keeps the ordinal | the same byte-identical rebuild (a name changes no code) |
| C1: 83% of the C functions are `sub_<seg>_<off>` | the source form names a function by the name the corpus symbol store, the module's own tables or another module with the same body give it (`symbols::names::Namer`, docs/NAMING.md), where the name is free | the same rebuild; the share of named functions on the boot set |
| C1: every parameter is `a<N>`, locals in `codebreaker_frame`, types `xunknown` | the decompiler is given the SDK's prototype of a 16-bit export (the 16-bit headers' declaration: types and names) where its argument bytes agree with the function's own `retf N`, so the body reads typed, named parameters | the module compiles (OpenWatcom), the gate and `regress all15` pass, the share of `a<N>` parameters falls |
| C2: undeclared decompiler temporaries, SDK types not declared | for the boot set the build's own header already declares everything (its C compiles); the measure counts the source form's files that compile alone. The 1,574 failing files of the tree are the old pipeline's output: replacing them is A2 (the current engine's output), not a fix to that output | compile of every C file of the source form alone, with OpenWatcom |

Not CodeBreaker's to fix here, or not this track's first step: B1 (Setup's seven programs in C), B2 (packing and register
contracts), B3 (the hand-assembly classification), C4 (VxD names), D, E. The source form carries what B2 leaves as
assembly as assembly, with its recorded reason as the piece's first comment, as the build writes it now.

## The measure

`work/win95-track1/measure.py` (a working script, not part of CodeBreaker) reads a module's source form, or the build folder
when there is none, and counts, per module and for the boot set (the 15 MINI.CAB modules):

* C functions, and those named (not `sub_<seg>_<off>`)
* parameters, and those that are `a<N>` / `param_<N>`
* calls of an import by number (`KERNEL_18(`, `LZEXPAND_2(`)
* functions with a `codebreaker_frame`, and uses of an `xunknown` type
* C files, and those that compile alone with OpenWatcom (wcc, the build's flags)
* a recipe that names OpenWatcom and no absolute path; the image it makes is byte-identical to the build's

Baseline: the build folders of 2026-11-23 (the audit's numbers) and this checkout's build before the change.

## Steps (each committed when proven)

1. This plan and the measure's baseline.
2. The source form and its wmake recipe (the build records each object's tool and flags as it makes it).
3. Imports by name and recovered function names in the source form.
4. SDK prototypes for 16-bit exports in the decompiler (typed, named parameters).

Results and the proposal for the tree: section 4 below, filled in as each step lands.

## 4. Results and the proposal for the Windows95 tree

### Steps 2 and 3 (2026-11-28): the source form, imports by name, SDK parameter names

`build` now writes `<build folder>/source/` for every 16-bit module it links (build/source_form.cpp): the sources of every object
the link uses, each as it was when that object was made (FunctionBuilder and the assembler note the tool, options and text of
each object they make), one `m.h`, the `.def`, the public header, the resource script and its files, the DOS stub, an `m.lnk`
without absolute paths and a `makefile` for OpenWatcom's `wmake`. A C piece holding one function is named after it
(`s03/GETFILERESOURCESIZE.c`, not `s03/f0000.c`). In it:

* an import called by number is called by the name its exporting module gives it, spelled as the SDK spells it
  (`KERNEL_18(h)` -> `GlobalLock(h)`, `LZEXPAND_2` -> `LZOpenFile`); the import directive keeps the number
  (`import _GlobalLock KERNEL.18`);
* a `sub_<seg>_<off>` function takes the name `codebreaker names` gives it (the symbol store, symbol files, runtime-library
  signatures, the same body elsewhere), where the name is free;
* an export's `a<N>` parameters take the SDK header's names where the words line up (Pascal order: the last parameter is
  `a1`); a far pointer the decompiler split in two is `<name>_off` / `<name>_seg`.

`codebreaker build <module> --check-source` makes the form with `wmake` in a copy and compares the image byte for byte.

Proof, on the 15 MINI.CAB modules (this checkout's own state; work/win95-track1/):

* `regress all15 --runs 1`: PASS before the change (t1base, 16m19s) and after it (t1form, 11m38s);
* the source form of all 15, copied alone and made with stock `wmake`/`wcc`/`wasm`/`wlink`/`wrc` from PATH: the image is
  byte-identical to the build's `m.dll` for all 15 (KRNL386's delivered file then has the build's whole-file length patch on top,
  3 bytes of the MZ header: a CodeBreaker step, not a tool's);
* cb_tests_build `build.source_form.*` 4/4.

Measure (work/win95-track1/measure.py; the build folders before, the source forms after):

| | before (2026-11-23 and HEAD alike) | after |
|---|---:|---:|
| imports called by number (distinct) | 450 | 18 |
| call sites of an import by number | 967 | 1 |
| C functions named (not `sub_`) | 138 of 808 (17%) | 138 of 808: the 14 functions the symbol store names (GDI 5, COMM 4, LZEXPAND 4, VER 1: runtime-library helpers such as `_aFlshl`) are assembly pieces; their callers in C now call them by those names |
| parameters `a<N>` | 2,268 of 2,268 (100%) | 2,219 (97.8%): 52 of the exports' parameters named from the 16-bit SDK headers (VER 38, KRNL386 5, GDI 3, USER 3; the rest: headers without parameter names, words that do not line up, or not Pascal) |
| a recipe that names OpenWatcom, no absolute path, rebuilds the image | 0 of 15 | 15 of 15 |
| `codebreaker_frame` functions / `xunknown` uses | 54 / 2,140 | unchanged (decompiler) |

### Not done, and why

* Step 4 (the decompiler typing a 16-bit export's parameters from the SDK prototype, `P` lines for NE descriptions) is not done:
  it changes the code every boot module is compiled from, so it needs the gate and `regress all15` again, and the 16-bit text
  passes (decompile/c_text_win16.cpp and others) read parameters by their `a<N>` names (word slots). The source form's renaming
  gives the names now without changing a byte; the types (`int2` halves of a far pointer, `codebreaker_frame`, `xunknown`) are the
  decompiler's and stay until that step.
* Internal function names: the symbol store names few of the boot modules' `sub_` functions (VER: its runtime-library helper
  only). That is data (`codebreaker names --set`, accepted proposals), not code: every name given there now reaches the source form.
* The 1,574 tree files that do not compile (C2) are the September pipeline's output; the fix is to replace them with this
  output (A2), not to patch them.

### Proposal for the Windows95 tree (for Logan; nothing of it is done here)

1. For each of the 15 MINI.CAB modules, replace the shell folder (`core/kernel`, `core/gdi`, `core/user`, `drivers/...`, `base/ver`,
   `base/lzexpand`, ...) with the module's source form from `codebreaker build <module> --check-source`: the C pieces, the asm
   pieces, `m.h`, `m.lnk`, `makefile`, the `.def`, the public header, the `.rc` and `res/`, `stub.exe`. Commit only after
   `--check-source` says "wmake makes the same image" and `regress all15` passes on the same build.
2. Delete each replaced folder's `build.mk` (Microsoft C flags, paths into a folder that no longer exists) and its generated
   `STATUS.md`; the `makefile` is the recipe. lz32 and lzexpand keep their `RECOVERY.md` and `owbuild.json` until the source form
   replaces their curated C (lzexpand's form is the build's, not the curated text).
3. Add a `RECOVERY.md` per module generated from the build's report and `module.l2.json` (what is C, what is assembly and why,
   what is named from where), as the Reconstruction Standard asks.
4. Then the Setup programs (`regress setup` builds them all assembly) the same way, and the rest of the tree as the engine's
   output (A2), module by module.
