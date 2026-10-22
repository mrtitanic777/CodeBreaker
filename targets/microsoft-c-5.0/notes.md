# Microsoft C Compiler 5.0 (1987) — Reverse Engineering Notebook

> The scripts this log mentions (`*.py`) are gone: their work is CodeBreaker's own now. Which command does what: [targets/README.md](../README.md).

Goal: recover readable, byte-exact-verifiable source for the Microsoft C 5.0 DOS
toolchain (the compiler passes, linker, and utilities) from the shipped floppy
images, using **CodeBreaker** as the workbench.

- **Target**: `Microsoft C Compiler 5.0 (1987) (5.25-360k)/` — 9 × 360K floppies (`.IMG` + ImageDisk `.IMD`).
- **Tool**: `C:\Users\mrtit\Documents\GitHub\CodeBreaker` — `wt.py` (16-bit MZ path: `mzdis`, `mzasmrecon`, `omf`, `libmatch`, `inverse` pipeline, Unicorn `behavior` oracle).
- **Everything here is 16-bit real-mode DOS MZ** — CodeBreaker's `inverse`/`msc6` era, not PE.

---

## Status board

| Phase | What | State |
|---|---|---|
| 0 | Extract files from floppy images (incl. subdirs) | ✅ done → `extracted/` |
| 1 | Unpack EXEPACK compression | ✅ done → `unpacked/` (19/19 validated) |
| 2 | Build MS C 5.0 compile oracle (DOSBox-X) | ✅ done → `tools/oracle5.py` |
| 3 | Disassemble + carve functions | ✅ done → `tools/carve.py` (~5,700 fns) |
| 4 | Origin triage: CRT/lib vs. app code | ✅ done → `tools/idlib.py` |
| 5 | Byte-exact recovery cascade | 🔄 loop proven (`inset` verified); scaling |
| 6 | Attribute functions to original `.c` modules (SCCS map) | 🔄 map built (80 modules) |
| 7 | Verify by recompile → byte-diff | ✅ primitive works → `tools/objfn.py` |

**The full pipeline is proven end to end.** All tooling is in `tools/`. The remaining
work (Phase 5/6) is *scale*: driving the recovery loop across the ~3,000 app functions
and, where the codegen isn't yet trivially matched, extending an MS C 5.0 codegen model.

---

## Corpus inventory (what's on the disks)

Extracted with `tools/fat12_extract.py` (raw FAT12, no external deps). All disks
are standard 360K FAT12 (`bytes/sec=512, sec/clus=2, root=112`).

### Reconstruction targets — the compiler & tools (16-bit DOS MZ)

| Binary | Disk | Role | Notes |
|---|---|---|---|
| **C1.EXE** (110K) | SETUP | Compiler **front end** (parse, small model) | pass 1 |
| **C1L.EXE** (141K) | ADDNUTIL | Front end, **large model** | pass 1 (large) |
| **C2.EXE** (199K) | COMPILER | **Code generator** | pass 2 |
| **C3.EXE** (117K) | COMPILER | **Optimizer / back end** | pass 3 |
| **CL.EXE** (33K) | SETUP | Compiler **driver** (`cc_main.c`) | orchestrator |
| **QCL.EXE** (30K) | SETUP | QuickC command-line driver | |
| LINK.EXE (64K) | UTILITES | Overlay Linker **v3.61** | |
| LIB.EXE (35K) | UTILITES | Library manager | OMF |
| MAKE.EXE (24K) | UTILITES | Build tool | |
| EXEPACK.EXE (15K) | UTILITES | The packer itself | see §Unpacking |
| EXEMOD.EXE (12K) | UTILITES | EXE header patcher | |
| ERROUT.EXE (10K) | UTILITES | stderr redirector | |
| SETENV.EXE (11K) | UTILITES | environment sizer | **not packed** |
| QLIB.EXE (25K) | UTILITES | QuickLib | |
| CVPACK.EXE (38K) | ADDNUTIL | CodeView info packer | |
| LIBBUILD.EXE (27K) | ADDNUTIL | library builder | |
| CV.EXE (220K) | CODEVIEW | **CodeView** debugger | overlaid; distinct startup |
| QC.EXE (327K) | QUICK_C | **QuickC** IDE | **not packed**, 224 relocs |
| SETUP.EXE (28K) | SETUP | installer | |

### Reference material (ground truth for signatures / typing)

- **CRT libraries (OMF `.LIB`)** — memory-model matrix S/M/C/L × {C, FP, FA}:
  `SLIBC/SLIBFP/SLIBFA`, `MLIBC/MLIBFP/MLIBFA`, `CLIBC/CLIBFP/CLIBFA`, `LLIBC/LLIBFP/LLIBFA`
  (INCLIBSM, LIB_M_C, LIB_L disks). These are the **library-signature source** — feed to
  `wt inverse omf` / `libmatch` to build FLIRT-style sigs so CRT code is *identified, not reconstructed*.
- `*VARSTCK.OBJ` (S/M/C/L) — stack-checking startup objects (OMF `.OBJ`, ground-truth codegen).
- `C1.ERR`, `C23.ERR`, `CL.ERR` — compiler diagnostic tables (string DB → cross-ref error paths).
- `.DOC` / `.HLP` / `PACKING.LST` — docs; `PACKING.LST` is the authoritative file manifest.

---

## §Unpacking — EXEPACK (the phase-1 unblocker)

17 of 19 EXEs are **Microsoft EXEPACK**-compressed (0 header relocs, `"Packed file is
corrupt!"` stub, CS:IP into a decompressor). CodeBreaker has no EXEPACK unpacker, so I
built `tools/unexepack.py` (algorithm read directly out of the C2.EXE stub disassembly).

Key finding — **partial packing**: the larger images (C1, C1L, C2, SETUP, ERROUT…) store
their **code segment uncompressed at the front** and EXEPACK-compress **only the trailing
data**. EXEPACK decompresses **in place**, so the fix was to seed the output buffer with
the original load module (not zeros) before expanding the tail top-down. With that, all
17 unpack cleanly:

- 16/17 show the canonical MS C startup at their real CS:IP: `mov ah,30h / int 21h / cmp al,2`
  (`__astart` → DOS get-version). CV.EXE has its own valid overlaid startup.
- Full relocation tables rebuilt (per-16-segment packed reloc format), e.g. C2 = 3280 relocs.
- QC.EXE and SETENV.EXE ship **unpacked** already (copied as-is).

Unpacked, relocatable, disassemble-ready binaries are in `unpacked/`.

Re-run: `python tools/unexepack.py extracted/<disk>/<name>.EXE unpacked/<name>.EXE`

---

## §Toolchain fingerprint

The binaries carry SCCS `@(#)` version stamps that name their **original source files** —
this drives module attribution (Phase 6). Confirmed:

- `CL.EXE` / `C3.EXE`: **"Microsoft C Compiler Version 5.00"**; `LINK.EXE`: **"Overlay Linker Version 3.61"**, "Microsoft Corp 1983-1987".
- **CL.EXE** source modules: `cc_main.c` (1.125), `cc_msdos.c`, `error.c`, `flags.c`, `getflags.c`, `get_err.c` …
- **C2/C3** shared backend modules: `MD.c`, `MDmisc.c`, `ail.c`, `andor.c`, `bitfields.c`,
  `canonical.c`, `code.c`, `csize.c`, `dbil.c`, `check_mode.c` … (C2 and C3 share `MD.c`/`ail.c`).
- C3 also references a **FORTRAN 4.0** back end string — C2/C3 are the shared MS C/FORTRAN codegen.

Running catalog of module names → `signatures.me`.

---

## §Environment

- Python 3.14, Capstone 5.0.7, pycdlib — all present.
- CodeBreaker `wt doctor`: DOSBox-X configured (`toolchains/dosbox-x/...`); MASM **not** yet
  configured (needed for the recompile/verify floor — can use MASM 5.1 or the bundled `nasm-masm`).
- Directory layout of this project:
  ```
  MicrosoftC5.0/
    Microsoft C Compiler 5.0 (1987) .../   original floppy images (read-only)
    extracted/    files pulled from the FAT12 disks
    unpacked/     EXEPACK-decompressed, relocatable MZ binaries  ← RE input
    work/         per-binary disasm / asm / recovered C
    tools/        fat12_extract.py, unexepack.py
    notes.md      this notebook
    signatures.me signature catalog
  ```

---

## §Full end-to-end map

The plan, start to finish. CodeBreaker commands assume its repo as cwd; point it at
`../MicrosoftC5.0/unpacked/<bin>`.

### Phase 0 — Acquire & extract ✅
Raw FAT12 extraction of all 9 floppies → `extracted/`. `PACKING.LST` reconciled.

### Phase 1 — Normalize (unpack) ✅
EXEPACK-decompress every packed EXE → `unpacked/`, rebuilding relocations. Validate each
by disassembling at real CS:IP (startup signature present).

### Phase 2 — Stand up the oracle 🎯 *next*
The elegant twist: **the target is its own oracle.** Drive the extracted MS C 5.0
(`CL.EXE`/`C1.EXE`/`C2.EXE`) under DOSBox-X to (a) compile probe C for the byte-exact
recovery loop, and (b) serve as ground truth for its own codegen.
- Wire an MS C 5.0 oracle mirroring CodeBreaker's existing `oracle16` / `msvc2_oracle` pattern.
- Add `masm`/`link` to `config.json` for the recompile-verify floor (MASM 5.1 or `nasm-masm`).
- Smoke test: compile a trivial `.c` with the real `CL.EXE`, confirm OMF + MZ round-trips.

### Phase 3 — Disassemble & carve
For each binary: `wt inverse mzdis <bin> header|relocs|dis`, then function carving.
Produce per-binary `work/<bin>/` with linear disasm, the reloc map, and a function table
(entry points, xrefs). Start with the small drivers (CL, QCL, MAKE, ERROUT, EXEMOD) to
shake out the pipeline, then the big passes (C1/C2/C3).

### Phase 4 — Origin triage (identify, don't reconstruct)
Split app code from toolchain code *first* (CodeBreaker's prime directive):
- Build CRT signatures from the shipped `.LIB`s: `wt inverse omf <model>LIBC.LIB` → `libmatch`.
- Fingerprint startup/`_chkstk`/`__stkchk` from `*VARSTCK.OBJ`.
- Mark identified library/CRT functions as *toolchain*; only unmatched code is a recon target.
- Cross-check identical CRT bodies across C1/C2/C3/CL to confirm the sig set.

### Phase 5 — Recovery cascade
Route every app function to the engine that explains it:
- **Byte-exact inverse** (`wt inverse` pipeline) where the codegen is modeled — template
  match → instruction inverter → decline-with-gap-log. Extend the model toward MS C 5.0
  codegen using the Phase-2 oracle (probe → model → invert → verify).
- **Behavioral** (`wt behavior trace|synth|struct|cfg|interproc`) on the Unicorn oracle for
  the rest — recover expressions, structs, globals, call sequences with typed args.
- Floor everything at **verified byte-exact ASM** (`wt inverse mzasmrecon`) so no body is faked.

### Phase 6 — Attribution & readability
- Map recovered functions to original **`.c` modules** via the SCCS `@(#)` table (from
  `signatures.me`) + string/error-table cross-refs (`C1.ERR`/`C23.ERR`/`CL.ERR`).
- Type args from CRT/DOS call sites; resolve DOS `int 21h` calls to named services;
  name callers by call sequence; recover switch/jump tables and the structured CFG.

### Phase 7 — Verify & assemble the picture
- Reassemble/recompile recovered units and **byte-diff against the original** (the proof floor).
- Track the ratio (template : inverter : gap) across iterations as the progress metric.
- Deliverable: an annotated, module-attributed source map of the C 5.0 compiler pipeline,
  with each function tiered (byte-exact / behavioral / ASM-floor) and cited to its origin.

---

## §Open questions / risks
- MS C 5.0's own codegen isn't yet modeled in CodeBreaker's inverse engine (it models MS C
  2.03 / 6.0a / MSVC). Phase 2's oracle is what lets us *extend* the model to 5.0.
- CV.EXE and QC.EXE use **overlays** — need overlay-aware carving before their code is whole.
- MASM 5.1 not yet attached — required for the recompile-verify floor (Phase 2/7).

## §Tooling (all in `tools/`)
| Tool | Phase | Does |
|---|---|---|
| `fat12_extract.py` | 0 | recursive FAT12 extraction of the floppy images |
| `unexepack.py` | 1 | EXEPACK decompressor (in-place seed; rebuilds relocations) |
| `oracle5.py` | 2 | drives extracted CL→C1/C2/C3 headless under DOSBox-X (`init`/`compile`/`build`/`cc`) |
| `carve.py` | 3 | MS C prologue-based function carver → `work/<bin>/functions.json` |
| `idlib.py` | 4 | FLIRT-style CRT identification from the OMF libs → `work/<bin>/identified.json` |
| `objfn.py` | 7 | extract a function + fixup mask from an OMF `.OBJ`; masked byte-diff vs the image |

## §Phase-4 corpus triage (lib% = share of carved fns that are CRT/library)
Small utilities are 50–70% CRT (nearly done once named); the **compiler passes are the
real targets**: C1 6%, C1L 7%, C2 7%, C3 18%, CL 40% library — i.e. C1/C2 are ~93%
original authored code. Full table via `idlib.py`; per-binary lists in `work/<bin>/identified.json`.

## §Phase-5 method (proven)
The recovery loop: **carve → read disasm → hypothesize C → compile with the real oracle
→ extract OBJ bytes + fixup mask → masked byte-diff → refine.** First verified recovery:
`inset()` (the char-in-set search shared by ERROUT/SETENV/EXEMOD/MAKE/LIB). Two iterations
pinned the exact source idiom (`register char *p`; `*p == c` operand order; `/AS /Ox`) to
reproduce EXEMOD@0x1ade **byte-for-byte** (modulo the one `mov si,set` pointer fixup). See
`work/recover/inset.c`. This is the template for the rest of Phase 5.

Codegen notes learned: MS C 5.0 `for(p=x;*p;p++)` emits jump-into-loop with the increment
at the top (`jmp .test / nop / inc si / .test: cmp[si],0`); `register` pointers live in
si/di with no stack home; two `return`s → two full epilogues (no shared tail at /Ox).

**Methodological boundary (found on `err1`).** Self-contained, side-effect-poor functions
(`inset`) recover byte-exact by compiling one function in isolation. But many functions
depend on MS C 5.0 codegen heuristics that are **whole-module / context sensitive** — e.g.
`err1` leaves an inner call's argument on the stack (deferring cleanup, legal since `exit`
is noreturn), a "loose stack" optimization the compiler applies in-context but not for a
lone function; no standalone flag reproduces it. **Implication:** scaling byte-exact past
leaf functions needs either (a) an inverted MS C 5.0 **codegen model** (CodeBreaker's
`inverse` approach — recognize templates, undo the optimizer), or (b) **whole-module**
reconstruction (recover all functions of one .obj together and compile the module so the
context-sensitive heuristics match). Per-function isolation is a floor, not the ceiling.
This is the "years → weeks" problem CodeBreaker exists to attack; the environment here is
now ready to attack it.

## §Log
- **2026-09-14** — Phases 0–1 complete. Extracted 9 disks; built + validated
  `tools/unexepack.py` (partial-packing / in-place seed bug found & fixed). 19/19
  binaries unpacked & entry-validated. Toolchain fingerprinted (C 5.00, LINK 3.61).
- **2026-09-14** — Phases 2–5 online. Re-extracted with subdir recursion (headers,
  STARTUP CRT source). `oracle5.py`: extracted MS C 5.0 compiles+links+runs SIEVE.C
  correctly ("150 iterations: 1027 primes"). Carved ~5,700 functions corpus-wide.
  `idlib.py`: CRT triage across all 19 binaries. `objfn.py` + oracle: first **byte-exact**
  reconstruction (`inset`). Pipeline proven end to end.
- **2026-09-14** — Phase-5 scale-out started on ERROUT. Added `tools/sheet.py` (recovery
  worksheet) and `tools/asmid.py` (asm-helper ID: `__chkstk` named in 15/19 binaries).
  ERROUT mapped: ~6 genuine app functions (rest CRT); `inset`@0x12ce byte-exact,
  `err1`@0x174 source-recovered. Report: `work/recover/ERROUT.md`. Confirmed several
  "app" functions are unmatched CRT (`_output`/printf) — extend idlib, don't reconstruct.
