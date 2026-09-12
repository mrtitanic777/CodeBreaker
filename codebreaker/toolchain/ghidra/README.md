# codebreaker/toolchain/ghidra — Ghidra decompilation pipeline (32-bit+ track)

Automated, high-accuracy Ghidra decompilation for the **32-bit-and-up** Win95 binaries
(PE user-mode DLLs/EXEs, and LE/PE VxD drivers). This is the *functional* reconstruction
track — readable, correct-logic C with real period types. **16-bit NE** binaries are **not**
handled here; those are CodeBreaker's byte-exact `msc6dec` + oracle track.

Ghidra was chosen over IDA/dogbolt after measuring: on 32-bit PE its function discovery and
decompiler match or beat Hex-Rays (advapi32: 141 vs 135; atim32: 6 vs dogbolt's 2), it runs
fully local/headless/unlimited, and — unlike Hex-Rays — it can be scripted and scaled.

## Pieces

| File | Role |
|---|---|
| `AggressivePre.py` | Ghidra **preScript** — enables the heavier analyzers (Decompiler Parameter ID, Aggressive Instruction Finder, Call Convention ID, Switch Analysis). |
| `DecompilePipeline.py` | Ghidra **postScript** — applies a period GDT, cross-checks for functions Ghidra's flow analysis missed (undefined call targets + unreferenced prologue scan), force-creates them, then decompiles every function to one `.c`. |
| `BuildGDT.py` | Ghidra **script** — parses period C headers into a `.gdt` type archive. Header-by-header (one bad header never kills the batch), with the full Windows-header define kit + `MIDL_PASS`, and include-order auto-recovery. |
| `ghidra_batch.py` | **Driver** (plain Python) — walks a folder, classifies each binary, picks the GDT, runs the pipeline, writes one `.c` per binary. Skips 16-bit NE. |

## Period type archives (the accuracy lever)

Ghidra's bundled `windows_vs12_32.gdt` is Visual Studio **2012** — it lacks Win16 and DDK/VxD
types entirely. `BuildGDT.py` builds period-correct archives from the **original Win95 SDK +
DDK headers** in the Win95 `design/reference/` arena (resolve them with `paths.reference(...)`):

| GDT (in `binaries/ghidra_gdt/`, gitignored) | Source headers | Types |
|---|---|---|
| `win32_win95sdk.gdt` | `WIN32SDK/MSTOOLS/INCLUDE/windows.h` | ~8150 |
| `ddk_win95_vxd.gdt`  | `DDK/INC32/*` (VMM, VWIN32, CONFIGMG, …) | ~5340 |
| `win16_win95sdk.gdt` | `DDK/INC16/*` | ~10250 |

Coverage note: a small fraction of declarations are skipped — a few include-order sub-headers,
plus VxD inline-asm service macros that aren't standard C (no C parser can take those). Every
type that matters (API structs, handles, common types) is captured. This is the same tradeoff
Hex-Rays/NSA made building their archive; it is **not** a Ghidra-parser bug to be fixed in source.

## Usage

Requires Ghidra 11.3.2 + JDK 21 (set `GHIDRA_HOME`/`JAVA_HOME`, or `ghidra_home`/`java_home`
in `config.json`).

```
# (re)build the type archives from the period headers
#   analyzeHeadless <proj> b -import <any.dll> -scriptPath codebreaker/toolchain/ghidra \
#       -postScript BuildGDT.py <out.gdt> <include_dir> <include_dir>   -deleteProject

# batch-decompile a folder of 32-bit binaries
python codebreaker/toolchain/ghidra/ghidra_batch.py <input_dir_or_file> <out_dir> [--limit N]
```

Output is one `<name>.ghidra.c` per binary. Re-runs skip binaries already done.
