# Byte-Exact Reconstruction of Windows 95 System DLLs — End-to-End Specification

> **Relocation note.** This spec predates the merge of the standalone `w95recon` tool into
> CodeBreaker; its `tools/w95recon/<file>.py` path references are historical. The capability
> now lives in the CodeBreaker engine: NE parsing → `codebreaker/formats`, 16-bit disassembly
> → `codebreaker/disasm`, OMF parsing → `codebreaker/engines/inverse/omf.py`, byte-exact
> verify/segdiff → `codebreaker/engines/recon`, and the MSC-6.0a-specific steps (recipe detection, NE
> normalization, brute-force axis search, component-tree filing) → `codebreaker/engines/msc6` (run via
> `wt msc6 …`). See [`MERGED_INTO_CODEBREAKER.md`](MERGED_INTO_CODEBREAKER.md) for the full map.

**Purpose.** This document specifies the complete, repeatable process for taking an
*originally shipped* Windows 95 binary DLL and reproducing it as a set of faithful,
compilable source files (C + assembler) that, driven through the authentic period
toolchain, re-link into a byte-identical (or provably equivalent) DLL. It is written
so the process can be encoded into an automatic tool that sweeps every DLL shipped in
Windows 95.

It is the generalization of the fully worked reference case, **LZEXPAND.DLL** (16-bit
NE, 9936 bytes, md5 `c7f03833`), for which 8/12 segments were driven to byte-identical
and the remaining 4 differ only in non-deterministic compiler register allocation.

---

## 0. Vocabulary and ground rules

- **Shipped DLL / retail** — the original Microsoft binary from `data/files/WINDOWS/SYSTEM/`.
  This is the oracle. Never modify it.
- **Reconstruction** — our source tree + build that reproduces it.
- **Authored code** — functions Microsoft's developers wrote (the DLL's actual logic).
- **Runtime code** — the C run-time library + DLL startup that the *linker* pulled in
  (not authored; comes from a library). Must be reproduced by linking the same library,
  not by hand-writing it.
- **Byte-identical (masked)** — code bytes match once the operand bytes that relocations
  overwrite at load time are masked out. Proves the *instructions* match.
- **Truly file-identical** — every byte matches, including the relocation records. This
  is the real goal per segment.
- **Residual** — a byte difference that is not a reconstruction error: compiler
  register-allocation tie-breaks, linker record ordering, inter-segment padding. These
  are properties of not having Microsoft's exact build machine and are not fixable from
  source. See §11.

**Doctrine: evidence, never guess.** Every claim ("this segment matches", "this function
is X") must be backed by a differential check against the shipped bytes. A masked-code
match is *not* sufficient on its own — see §10.3 for the trap it hides.

**No-fabrication rule (binding on the automatic tool).** The tool must never emit a
plausible-but-unverified value, name, type, boundary, or code body as if it were known.
Anything not *proven* — by the byte differential, the reloc-resolved differential, the
entry/name tables, the import ground truth, or the functional oracle — must be **marked**
as unknown/assumed and surfaced for human review, not silently invented. Where the tool
cannot produce verified source for a region, it emits that region's *raw shipped bytes*
(as a `DB` blob or `#pragma`/`_asm` sequence) behind a marker — never a guessed C body.
The output must be honest about exactly what is and is not known. See §12.2.

---

## 1. Phase 0 — Triage / classify the target

Before any work, classify the DLL. The tool must branch here.

1. **Container format.** Read the MZ header; follow `e_lfanew`.
   - `NE` → 16-bit segmented executable (Win 3.x / Win95 16-bit core). This spec's main path.
   - `PE` → 32-bit. Different toolchain (MSVC 2.x, not covered by the 16-bit path); the
     RE/verify methodology still applies but codegen and CRT differ.
2. **Thunk vs self-contained.** Decompile and inspect imports.
   - A **thunk** DLL (e.g. `LZ32.DLL`) merely forwards to another module (imports
     `SYSTHUNK.DLL` `QT_Thunk`/`SMapLS_*`, calls a 16-bit twin). Its input→output
     behavior depends on the far side, so an input/output oracle *cannot* verify it in
     isolation. Flag and defer; verify only end-to-end.
   - A **self-contained** DLL holds the real algorithm. Proceed.
3. **Record the fingerprint:** file size, md5, segment count, linker version stamp
   (NE header offset `0x02`), align shift (`0x32`), imported module set. These are the
   acceptance targets.

Output of Phase 0: `{format, is_thunk, nseg, size, md5, linkver, align, imports[]}`.

---

## 2. Phase 1 — Parse the NE container

Deterministic. Tooling: `tools/w95recon/ne.py`.

Extract, from the NE header (at `e_lfanew`) and its tables:

- **Segment table** (`ne+0x22`, `nseg` × 8 bytes): per segment `{sector, length, flags,
  minalloc}`. File offset = `sector << align_shift`. `flags & 0x0100` = has relocations.
- **Per-segment relocation records** (2-byte count + N×8 immediately after each segment's
  data): `{address_type, reloc_type, offset, target1, target2}`.
  - `reloc_type & 3`: 0 = internal ref, 1 = import by ordinal, 2 = import by name,
    3 = OS fixup.
  - `address_type`: 3 = far pointer (segment:offset, used by `CALLF`/far data),
    5 = 16-bit offset.
  - **Chains:** if several code sites share one target, the NE stores *one* reloc record
    whose `offset` points to the first site; each site's operand word points to the next
    site; `0xFFFF` ends the chain. The tool must walk chains to enumerate every patched
    site (needed in §10).
  - **Internal target `segment == 0xFF`** means "movable — resolve through the entry
    table": `target2` is an *entry ordinal*, not a segment offset.
- **Entry table** (`ne+0x04`, len `ne+0x06`): bundles mapping ordinal → `(segment,
  offset, flags)`. `flags & 1` = exported. This is the authoritative map of *addressable*
  functions and their exact segment:offset.
- **Resident + nonresident name tables**: name ↔ ordinal (module name is the first
  resident entry; exported names live here). Note: the **nonresident-names table offset**
  (`ne+0x2c`, DWORD) is *file-absolute*, unlike every other table offset which is
  NE-relative — this matters in §9.
- **Module-reference + imported-name tables** (`ne+0x28`, `ne+0x2a`): resolve import
  module ordinals (mod 1, mod 2 …) to names (`KERNEL`, `USER`, …).
- **Fast-load / gangload area** (`ne+0x38` sector offset, `ne+0x3a` length): absolute.

Output: a structured model of every segment, its bytes, its relocations (with resolved
targets), and the ordinal→segment:offset map.

---

## 3. Phase 2 — Segment→module map and function discovery

### 3.1 Segments are source modules
In 16-bit **medium model**, each C source module compiles to its own `<module>_TEXT`
code segment, and the linker (with `/NOPACKCODE`) keeps them separate. Therefore:

> **The shipped DLL's code-segment count reveals the number of authored source modules.**
> N code segments = (N−1) authored modules + 1 runtime segment (the CRT/startup the
> linker pulled). The last code segment, containing the C run-time and the exported
> stub, is the runtime segment; the rest are authored, in source order.

For LZEXPAND: 12 segments = 10 authored `.c` modules + 1 runtime `_TEXT` + 1 DATA.

### 3.2 Complete function discovery (critical — Ghidra alone is insufficient)
Tooling: `tools/w95recon/disasm.py` (16-bit capstone). Discover functions as the **union**
of three sources, because any one misses functions:

1. **Entry-table targets** — every ordinal's `segment:offset`.
2. **Prologue scan** — byte signatures of the medium-model far prologue:
   `(1e 58 90 | b8 .. ..) 45 55 8b ec` (`push ds; pop ax; nop` *or* `mov ax,imm` for the
   loadds fixup, then `inc bp; push bp; mov bp,sp`).
3. **Relocation targets** — internal reloc targets that land inside a segment.

Ghidra reliably finds the exported/referenced functions but **drops functions that are
only reached by near calls within a segment** (verified: seg8 had 4 functions, Ghidra
found 2; seg10 had 6, Ghidra found 5). The prologue scan recovers those.

**A "function" is not always what the entry table suggests.** Some entry-table entries are
*basic blocks inside a larger function* reached via the movable-entry mechanism, and some
call sites that look like separate helpers are inline blocks. Always confirm boundaries by
prologue + single-epilogue analysis (§3.3), not by the entry table alone. (In LZEXPAND
this caused an entire spurious segment: four "functions" turned out to be one Driver's
inline blocks — see §7.4.)

### 3.3 Boundary rule
A function runs from a prologue to its matching epilogue
(`pop..; mov sp,bp; pop ds; pop bp; dec bp; retf`, often preceded by `dec bp; dec bp`
from the `inc bp` loadds convention). One prologue + one terminal epilogue = one function,
regardless of how many entry-table ordinals point into it.

Output: per authored segment, an ordered list of `{name?, offset, length, entry_ordinal?}`.

---

## 4. Phase 3 — Reverse each function to faithful period C

For each authored function, produce C that compiles (via §6's recipe) to bytes matching
the shipped segment. Method: Ghidra decompilation (headless `DecompileAll.py`) as a first
draft, then hand-refine against the disassembly using the **idiom library**
(`tools/w95recon/IDIOMS.md`) and iterate compile→diff.

### 4.1 The compile→diff loop (per function)
1. Write/refine C.
2. Compile with the exact recipe (§6).
3. Parse our `.obj` (`tools/w95recon/omf.py`: LEDATA `0xA0/0xA1` + FIXUPP) to get code
   bytes + fixup positions.
4. Diff against the shipped segment's function slice, **masking both our fixups and the
   shipped relocations** (`cmp_seg`/`segdiff`). Zero diff = done.
5. Non-zero → read the `/Fc` `.cod` listing, identify the idiom, fix, repeat.

### 4.2 Brute force the bounded source-form space (the accelerator)
Because compile+diff is free (no human, no tokens), most "compiler-inherent" residuals are
actually **source-form-crackable**. Tooling: `tools/w95recon/bruteforce.py`.
- Author one base with `@{ optA | optB | optC }@` choice markers at uncertain spots
  (loop form, signed/unsigned, `goto` vs structured, temp vs inline, ternary vs if).
- `expand()` cross-products; `batch_compile()` compiles *all* variants in one DOSBox launch
  (amortizes startup); `crack()` returns the zero-diff winner.
- Proven: cracked seg6's `INC`-vs-`CMP` "residual" with `return di==-1 ? 0xfffe : 0xfffc;`
  (ternary → MSC emits `CMP`); cracked Refill's cold-block placement.

### 4.3 Codegen idioms that matter (non-exhaustive; keep appending to IDIOMS.md)
- `cl /c /AM /G2 /Gsw /Ox` is the recipe (medium model, 286 codegen, Windows loadds, max opt).
- Far-pointer buffer globals stored handle-in-hi-word: `gp = (DWORD)GlobalAlloc(...) << 16;`
  peepholes to immediate stores, no shift.
- Alloc-shrink retry loops must be `do { if(x==0) break; ...; x-=0x400; } while(y!=0);`
  to match MSC's top-CMP + flag-driven back-edge.
- Signed `int` loop counters give `JL`(7c); unsigned give `JB`(72).
- `shift += 8` on a byte must go through a temp (`t=shift; t+=8; shift=t;`) to avoid
  `add byte[mem],imm`.
- Shared exits need explicit `goto ret1;` labels (MSC shares one `MOV AX,1` that early
  paths jump to).
- Intra-segment far call = `PUSH CS; CALL near`; inter-segment = `CALLF` (reloc).
- `register` on a far-ptr walker forces `LES/SI`.
- Statement/eval order is literal; a result that must sit between two stores needs the
  far-ptr/DWORD idiom, not a named temp (named temp → register var → spurious `PUSH SI`).

---

## 5. Phase 4 — Reconstruct the DATA segment (structure, not splice)

The DATA segment is a *layout* problem: get every global at its shipped DGROUP offset by
declaration order and type, **naturally** (no `ORG` pinning). Tooling: MASM 5.1 for the
data module (`segdata.asm`) so byte-exact control is possible.

### 5.1 Read the shipped layout
Slice the DATA segment. Map every region: control block, initialized strings/tables,
signatures, CRT startup block, `_BSS`. The **global names encode their offsets** in a
good reconstruction (`gp4c`@0x4c, `gD280`@0x280, …).

### 5.2 The "block M" insight
A block of zero-initialized words near the top of DGROUP (0x10–0x5f in LZEXPAND) is easily
mistaken for CRT data. It is usually **the DLL's own control globals** (handle tables,
state, buffer pointers). Put them in `_DATA` (zero-init, in-file) *before* the initialized
strings, so the strings and tables land at their shipped offsets. Getting this wrong slides
everything after it.

### 5.3 DGROUP composition and link order
The combined `_DATA` is the concatenation of each object's `_DATA` contributions **in link
order**. Therefore:
- The data module links **first** → control block at the base.
- The CRT data object (e.g. `crt0dat`) links **explicitly right after** → its
  `_C_FILE_INFO`/argc/argv block lands at the shipped offset. (Pulled implicitly from the
  lib, it lands wrong.)
- Startup/`aDBswp` fragments link **last** so their DGROUP slots fall where the shipped
  file has them.

### 5.4 `_BSS` ordering and communal vs explicit
`_BSS` is where two subtle rules decide byte-exactness:
- **The named `_BSS` segment sorts first within class BSS; other BSS segments (the CRT's
  `XO`/`XOB`/`XOE` on-exit tables, etc.) sort after.** To place an app `_BSS` block *after*
  those tables, put it in a *distinct* BSS segment and add it to the DGROUP `GROUP` line
  **after** the table segments — DGROUP membership order drives placement.
- **Communal (`COMM`) vars are size-sorted by LINK (DWORDs first), which scrambles offsets.**
  If the shipped globals are in natural declaration order, they are *explicit* `_BSS`, not
  communal. Do not use `COMM` to place ordered globals.
- Reserved gaps matter: unnamed decoder/CRT scratch between named globals must be
  reproduced (`DW n DUP(?)`) or everything downstream shifts by the gap size.

### 5.5 Avoid dragging CRT data
If an authored function calls a CRT wrapper that sets `errno` (e.g. `_dos_getftime`), the
linker pulls that wrapper *and its data* (e.g. a DOS→errno table), bloating `_DATA`. The
shipped DLL used the leaner path. Provide the function yourself (inline `_asm INT 21h`,
or a hand-written asm module) so the lib version — and its data — is never pulled.

---

## 6. Phase 5 — The authentic toolchain (the single most important input)

Byte-exactness is impossible without the *exact* period tools. Identify them by
fingerprint, not by name. The tool locates each one through `toolchain/toolchain.py`
`resolve(role, version)` against `toolchain/MANIFEST.json` (§13) — never a hardcoded path;
the `Location` column below is indicative, the manifest is authoritative.

| Role | Tool | How identified | Location |
|------|------|----------------|----------|
| C compiler | **MSC 6.0a** (`cl` + `c1`/`c2`/`c3`) | reproduces the shipped WEP byte-exact; old `PUSH DS;POP AX;NOP` prologue; `SUB SP,2 + NOP` at `/Ox` | `toolchain/msc60dos/BIN` (decompressed from the 6.00a floppies with `DECOMP.EXE`) |
| Assembler | **MASM 5.1** (real-mode `MASM.EXE`) | real-mode DOS; MASM 6.x `ML.EXE` is Win32 PE, useless in DOSBox | `toolchain/masm51/` |
| Linker | **LINK 5.20** | matches the shipped NE linker stamp (5.20); does *not* hang (MSC 6.0's LINK 5.10 hangs) | `toolchain/link520/` (from the Win3.1 DDK `win16ddk.iso`) |
| DLL C runtime | **MDLLCEW.LIB** (medium-model Windows DLL CRT) | the shipped runtime segment's source; crt0dat 82-byte `_DATA` starting `_C_FILE_INFO=` | Win3.1 SDK `C600LIBS/LIB/` |
| Windows imports | **LIBW.LIB** | period 16-bit USER/KERNEL/GDI import library | Win3.1 SDK / `toolchain/winkit/` |
| DLL entry | **LIBENTRY.OBJ** | provides `LibEntry` that calls the DLL's `LibMain` | Win3.1 SDK |
| Resource compiler | **RC.EXE** | stamps NE as Windows executable | Win3.1 SDK / `msvc152` |

**Compiler identification protocol.** Pick the smallest authored function with a clean
fingerprint (both LZEXPAND and VER used their ~24-byte `WEP`). Reproduce it byte-exact by
sweeping compiler candidates **and `/O` levels**. The candidate + flags that reproduce it
*are* the toolchain for that DLL. **The `/O` level is PER-DLL, not global** — verified:
LZEXPAND was built `/Ox` (its `WEP {int i;}` needs the `SUB SP,2` empty frame), but VER was
built **`/Os`** (its `WEP {return 1;}` has *no* frame — `/Ox` wrongly emits a spurious
`SUB SP,2`, `/Os` matches 23/23). So every DLL must have its `/O` level pinned from its own
smallest function before reconstructing the rest; do not assume the previous DLL's flags.

**This is AUTOMATED — `detect_recipe.py <dll>`.** The tool locates the DLL's `WEP` export
(a tiny, near-universal Windows Exit Procedure whose source space is a handful of known
forms), brute-forces `{WEP source forms} x {model /AS,/AM,/AC,/AL} x {/O /Ox,/Os,/O,/Ot,/Og}`
in one DOSBox launch, and pins the flag set(s) that reproduce WEP's exact bytes -- writing
`_work/recipe/<dll>/recipe.json`. It never guesses: WEP is small so the *model* is usually
ambiguous (no data refs) and is reported as such (confirm it on a data-referencing function);
if the DLL has no WEP, or no WEP form matches (different compiler / MASM-authored), it emits
`@REVIEW` rather than a fabricated recipe. Verified: auto-detects LZEXPAND=/Ox, VER=/Os,
hpcolor=/O(ambiguous), and honestly `@REVIEW`s slenh (no WEP)/cards/olesvr. When a DLL's opt
is ambiguous from WEP, disambiguate by extending the probe to its next-smallest function.

**Everything runs in DOSBox-X** (`refs/tools/dosbox-x/...`); Phar-Lap-hosted tools hit
"Out of memory" on bare Win64 (no DOS DPMI). Mount the toolchain dirs as drives; drive the
build from a `.conf` `[autoexec]` that `call`s a batch and `exit`s (self-exiting — never
`taskkill`, it kills sibling builds).

**DOS gotchas the tool must encode:** no `2>&1` (redirect each tool to its own file; a bare
`2>&1` creates a junk `&1` file); a driver batch must **not** be named `MD.BAT` (collides
with DOS `mkdir`); `cl` writes banner/errors to STDERR which DOS can't capture — screenshot
the DOSBox window if you need them.

---

## 7. Phase 6 — Build assembly (per-segment modules → NE)

### 7.1 Module tree
One `.c` per authored segment, named for its role, functions in *shipped source order*
(so intra-segment near-call displacements match). A shared internal header carries types,
structs, constants, far-pointer macros, import externs, and cross-module far prototypes.
Globals are DEFINED once in a data-only module (→ no code segment). Cross-module-called
helpers must drop `static` (external linkage) so the linker resolves inter-segment far
calls; module-local helpers stay `static`.

### 7.2 Link recipe
`/NOD /NOE /NOPACKCODE /DOSSEG /ALIGN:16 /MAP` with `mdllcew.lib libw.lib`.
- `/NOD` — no default libs (we name them).
- `/NOE` — no extended dictionary (resolves multiply-defined `WEP` etc.; lib ships a default WEP).
- **`/NOPACKCODE`** — mandatory; medium model emits distinct `<module>_TEXT` segments but
  LINK's default `/PACKCODE` folds them into one physical segment. `/NOPACKCODE` keeps all
  N separate, reproducing the shipped segment architecture.
- `/DOSSEG` — canonical segment ordering.
- LINK 5.20 is Phar-Lap-hosted → `link obj1+obj2+...` overflows the arg list; use a
  **response file** `link @lz.lnk` (fields: opts+objs / dll / map / libs / def; objects
  continue across lines with a trailing `+`). Call the linker by **explicit path** (a bare
  `link` on PATH may find the hanging LINK 5.10).

### 7.3 Segment/link order
`datamodule + [runtime-startup module] + authored1..N + [glue] + [aDBswp module last]`,
matching §5.3. The startup/runtime modules' `_TEXT` must lead the runtime segment so their
internal offsets (e.g. an `aDB` RET stub) land where the shipped file references them.

### 7.4 Reproduce the runtime segment correctly (do not invent glue)
The runtime segment is the linker's CRT + `LibEntry` + exported stub. Reproduce it by
*linking the right library*, not by hand-writing stubs.
- **`LibMain`** is usually the CRT's own (the `LibEntry`→`LibMain` internal call resolves
  into the runtime segment). Do **not** add an app `LibMain` — it creates a spurious extra
  code segment.
- The shipped DLL may contain a small **authored** helper compiled into the runtime `_TEXT`
  region (LZEXPAND had a Windows-dispatched DOS date/time helper: `TEST cs:[__wflags],1 /
  CALLF DOS3Call` else `INT 21h`). Reconstruct these into an asm module that links into the
  runtime segment at the right offset.
- **STKHQQ placement:** `globalW _STKHQQ,STACKSLOP`(→`_DATA`) vs `globalW _STKHQQ,0`
  (→`_BSS` via CMACROS `?dd` zero-init). The shipped DLL used the `_BSS`/0 form.
- **Do not factor authored inline blocks into separate helper functions.** If a driver
  function inlines calls to real functions in other segments, reconstruct it as one
  function calling those real functions — inventing stub sub-functions creates extra
  entry-table entries *and an extra code segment*. (This was the LZHELP_TEXT 13th-segment
  bug; folding it out restored the shipped 12-segment count.)

Output of Phase 6: a linked NE with the shipped segment architecture.

---

## 8. Phase 7 — Control DATA/BSS layout to shipped offsets

This is §5 realized in the build. Iterate: build → dump the `.map` and compare global
offsets to the shipped DGROUP offsets → adjust module link order and DGROUP `GROUP`
membership order until every global lands on its shipped offset and `_BSS` starts at the
shipped offset. The empty-segment-in-an-earlier-module trick (declare an empty
`CDATA`/`XIF*`/`XO*`/app-BSS segment in an early-linked module and list it in that module's
DGROUP line) moves table/BSS segments to their shipped positions via appearance order.

---

## 9. Phase 8 — NE metadata normalization (post-link)

LINK 5.20 (this build) forces the NE header to a **0x400 file boundary**, padding the DOS
stub area; shipped DLLs carry a minimal **128-byte** stub with the NE header at 0x80. This
is pure boilerplate before the NE and affects only file size, not code — normalize it.

Tooling: `tools/w95recon/ne_normalize.py`. It replaces the stub with a reference 128-byte
stub (extract the shipped DLL's own `[:e_lfanew]`) and fixes every **absolute** field so it
still loads:
- MZ `e_lfanew` → new NE offset (stub length).
- Each segment-table **sector** number → `-= shift >> align_shift`.
- **Nonresident-names table offset** (`ne+0x2c`, file-absolute) → `-= shift`.
- **Fast-load/gangload sector offset** (`ne+0x38`) → `-= shift >> align_shift`.
- All other NE table offsets are NE-relative → copied verbatim.

Validate the result: module name, exports at shipped ordinals, imports, and every segment's
file range in bounds.

---

## 10. Phase 9 — Verification (the acceptance gate)

Three independent checks; a segment/DLL is accepted only when all pass.

### 10.1 Structural
Segment count, order, and classes match the shipped DLL; entry table has the same ordinal
count and the exported ordinals map to the same roles; imports match; NE stamps match.

### 10.2 Byte differential (masked) — per segment
Compare each segment's code with relocation-patched bytes masked (mask our fixups ∪ shipped
relocs; for cross-linker comparisons also mask far-call operands `9a/ea` + 4 bytes).
Zero masked diff = instructions identical. Tooling: `segdiff.py` / `cmp_seg.py`.

### 10.3 Relocation-resolved differential — MANDATORY, do not skip
**A masked-code match hides real differences.** The masked comparison zeroes the operand of
every relocated call — so two segments can be "byte-identical (masked)" while their calls
resolve to *different imported functions*. Always additionally compare the **relocation
records** (targets + order), and resolve masked call relocs to their actual targets
(import ordinal or entry-table segment:offset).

> Reference incident: LZEXPAND seg10 was "byte-identical (masked)" but two `lcall`s bound
> to swapped USER ordinals. Resolving them exposed a genuine **bug in our source** —
> `RestoreName` tested `IsCharUpper`/`IsCharLower` in the wrong order, inverting the case
> of a restored filename character. Ground truth came from dumping the real
> `WINDOWS/SYSTEM/USER.EXE` exports (`USER.435=IsCharUpper`, `USER.436=IsCharLower`). Fixing
> the source both corrected behavior and made the segment truly byte-identical. The tool
> must resolve every masked call reloc and diff the resolved target against the shipped
> reloc; a mismatch is a finding, not noise.

### 10.4 Functional regression net
Independent of byte-exactness: build a stub harness that `#include`s the reconstructed
source with the Windows imports replaced by host/DOS equivalents, and round-trips a corpus
through the DLL's real entry points. For LZEXPAND: an SZDD corpus (empty/one/runs/text/
buffer-edge/random/binary) through both `LZCopy` and `LZOpenFile→LZRead`, verified against
a known-good decompressor. This catches semantic bugs that byte tests may miss **and** vice
versa (§10.3) — run both. Gate: `N passed, 0 failed`.

Cross-check against the vendor tool where one exists (e.g. Microsoft's `expand.exe` for
LZEXPAND) for an external ground truth.

---

## 10.5 The per-function synthesizer — automating "write C" + "diagnose" (steps 2 & 5)

The RE loop's manual steps are (2) write faithful C from the disassembly and (5) diagnose a
byte-diff and adjust the source. Both become **search with the compiler as oracle** — no
comprehension required, because compile+diff is free. Tool: `synth.py <dll> <seg> <func>`,
built on the existing `verify.py` (compile+diff), `bruteforce.py` (free batch compile+diff),
and `autoaxis.py` (source-form variation). Loop:

```
skeleton = auto_skeleton(func)          # STEP 2: deterministic disasm -> compilable C
recipe   = detect_recipe(dll)           # per-DLL flags (P5) -- NOT hardcoded
best = skeleton
while diff(compile(best, recipe), target) > 0:
    off   = first_diff_offset(best)     # STEP 5a: where it diverges
    stmt  = map_offset_to_source(off)   # via the /Fc .cod line<->address listing
    cands = axes.variants_at(best, stmt)# STEP 5b: transform ONLY that construct
    best  = argmin_diff(batch_compile(cands, recipe))   # oracle picks the winner
    if no_improvement: break            # -> classify residual (§11) or widen axes
emit(best) or emit_raw_bytes(func)      # @RECON-RAW if unreconstructed (never fabricate)
```

Three components, in order of what's already built:

- **STEP 3/4 (compile, diff)** — DONE (`verify.py`). Fix needed: it hardcodes `/Ox`; it must
  take the per-DLL recipe from `detect_recipe.py` (else the search optimizes against the wrong
  compiler — e.g. VER=/Os fails under /Ox).
- **STEP 5 (diagnose+adjust)** — PARTIAL. `autoaxis.py` does blind greedy coordinate-descent
  (every axis everywhere) + `recon.py autocrack` runs it hands-off. Upgrade: **diff-guided
  targeting** — compile with `/Fc`, parse the `.cod` source-line↔address map, map the first
  diff offset back to its source statement, and apply axes to *that* construct first. Turns an
  O(axes×sites) blind sweep into a targeted, convergent search. Grow the axis library; every
  cracked residual adds one (ternary, positive-test flip, loop-form, signed/unsigned,
  temp-vs-inline, do-while-vs-while, store-order — see `IDIOMS.md`).
- **STEP 2 (write the base C)** — THE linchpin, least built. A deterministic disasm→compilable-C
  skeleton so the search has a starting point with no LLM. Path: Ghidra headless
  (`DecompileAll.py` exists) → a **normalization pass** that makes the decompile MSC-6.0a-compilable
  (K&R sigs, `far`/`pascal` conventions, the auto-derived extern globals/structs/import protos
  from the DGROUP layout (§5) and import tables (§2)), → hand it to the search. Ghidra need not
  be *faithful*, only close enough for the axis search to converge; where it can't produce a
  compilable skeleton, that function falls back to `@RECON-RAW` + `@REVIEW` (never a guess).

What stays human/LLM after this: functions whose source-form space is too large for the axis
search to converge, genuinely novel idioms (added once to the library, then automatic), and
the residual-vs-real-bug judgement (§10.3). The goal is to shrink the LLM to those, not to
claim 100% automation — a function the tool cannot synthesize is emitted as raw bytes and
flagged, per the no-fabrication rule.

## 11. What will NOT match, and why (residuals)

These are not reconstruction errors and are not fixable from source. The tool must
recognize and *classify* them, not chase them:

1. **Register-allocation tie-breaks.** MSC 6.0a picks a different register (`si` vs `ax`)
   with identical instructions/CFG/calls. The DOS-hosted `c2` codegen pass on hand is one
   specific build; the alternate is the OS/2 build ("cannot run in DOS mode"). Differs by a
   register number, occasionally ±2 bytes. Detect: same instruction stream, differing only
   in modrm register fields.
2. **Linker record ordering.** Two relocation records for different targets emitted in a
   different order (same set). Cosmetic; the code is identical.
3. **Inter-segment padding / fast-load layout.** Our `/ALIGN:16` packing can differ from
   the shipped file's by a few bytes per segment.

A whole-file md5 match is therefore generally **not** achievable from pure source, and a
"splice" of the shipped bytes for residual functions would just *be* the shipped file (the
identical segments are already identical) — so it is not a reconstruction. The meaningful
deliverable is **maximal from-source fidelity**: every authored segment byte-identical
except the register residuals, plus byte-identical data, runtime, and DOS stub, plus the
exact segment architecture, plus a functional pass, plus faithful compilable source.

---

## 12. Automation architecture (applying this to every Win95 DLL)

### 12.1 Generated outputs — the deliverable set (the tool WRITES all of these)

For each DLL the tool must generate a complete, buildable source project — not just a
report. Nothing in this set is optional; a DLL is "done" only when the set is present and
the build reproduces the target:

- **`<module>.c`** — one C file per authored code segment (§3.1), functions in shipped
  source order, named for their role.
- **`<name>.h`** — the shared internal header(s): types, structs, constants, far-pointer
  macros, import `extern`s, cross-module far prototypes. Prefer the original Microsoft
  header when one is recovered from an SDK (e.g. `LZEXPAND.H`); otherwise generate one and
  mark synthesized declarations (§12.2).
- **`<datamodule>.asm`** and any **runtime/startup `.asm`** — the MASM 5.1 data module and
  any authored asm that lands in the runtime segment (§7.4). ASM is generated **only where
  appropriate**: the DATA segment (for byte-exact layout control) and hand-authored
  asm-in-runtime helpers. Authored *code* segments stay C unless the shipped bytes cannot
  be reproduced from C — then the specific function is emitted as an `_asm`/`DB` blob behind
  a marker (§12.2), not rewritten wholesale in asm.
- **`<module>.def`** — the module-definition file: `LIBRARY`/`DESCRIPTION`/`EXETYPE`/
  `STUB`, `CODE`/`DATA` attributes, `HEAPSIZE`, and an `EXPORTS` section reproducing the
  **shipped export ordinals exactly** (read from the entry + name tables in Phase 1, not
  invented — a mis-ordinal breaks every binary that imports by number). Emit explicit
  `IMPORTS` ordinal bindings when the differential shows the local import library disagrees
  with the shipped file (the §10.3 `IsCharUpper` case).
- **`<name>.lnk`** — the LINK 5.20 response file (opts+objs / dll / map / libs / def), with
  the object order that achieves the shipped DATA/segment layout (§7.3, §8).
- **`makefile`** — the build recipe: the exact `cl` line (`/c /AM /G2 /Gsw /Ox`), the MASM
  invocations, `link @<name>.lnk`, `rc`, and the host-side `ne_normalize.py` post-step. It
  must be runnable end-to-end (under the DOSBox harness for the DOS tools + host Python for
  normalization) with no manual steps.
- **`RECON_NOTES.md` + `unknowns.json`** — the uncertainty manifest (§12.2).
- **`scoreboard.json`** — per-segment/per-function status: `identical | masked-identical |
  residual(<class>) | UNVERIFIED`, with the byte-diff count and, for residuals, the
  classification (register | link-order | padding).

The tool generates these into a per-DLL `_work/<name>/` tree and copies the accepted
source set to a `recovered/<name>/` deliverable tree.

**Source-tree assembly (final filing step).** `win95tree.py` files each accepted
`recovered/<dll>/` set into a **plausible Microsoft "Chicago" source tree** under `WIN95/`,
driven by the editable map `win95_dllmap.json` (component layout + `exact_map` + family
`rules`). Honesty rules, same doctrine as §12.2: the layout is a *reconstruction*, not
verified Microsoft ground truth (component names are real; the hierarchy is reasoned and
correctable by editing the map). A DLL is filed only via a high-confidence `exact_map` entry
or a transparent family rule; **anything unclassified lands in `WIN95/UNSORTED/` with a
`@REVIEW` marker, never force-fit**. Every filed DLL carries a `PLACEMENT.md` recording how
it was classified, so the filing is auditable. Out-of-scope branches (MS-DOS real-mode base,
ring-0 VMM/VxDs, 32-bit/VxD network stacks) are scaffolded with a `SCOPE.md` so the tree
mirrors Microsoft's shape but are never populated by the 16-bit-NE pipeline. Commands:
`win95tree.py init | classify [--all] | place <dll> <srcdir> | status`. Current auto-
classification of the 189 NE16 corpus: ~139 confident (exact/rule) / ~50 `UNSORTED` for
review.

### 12.2 No-fabrication doctrine and uncertainty notation (how "we don't know" is recorded)

The tool must make every gap **visible and reviewable**, never paper over it. Concretely:

**Marker syntax** (uniform, greppable, carries a stable id so the manifest can index it):

- C / H: `/* @RECON-UNKNOWN[id] category: what is uncertain and why */`
- ASM:   `; @RECON-UNKNOWN[id] category: ...`
- For a value the tool had to choose to proceed but has **not** proven:
  `/* @RECON-ASSUME[id] category: chose X because Y — UNVERIFIED */`
- For a region emitted as raw shipped bytes because no verified source was produced:
  `/* @RECON-RAW[id]: <n> bytes at seg<k>:0x<off>, not reconstructed */` immediately above
  the `DB`/`_asm` blob of the actual shipped bytes.

**Categories** (each becomes a review lane): `symbol-name` (function/global name not
recoverable from tables — a placeholder like `Fn_1030_00f4` was used), `type` (parameter/
global type inferred, not proven), `boundary` (function extent uncertain), `undecoded`
(bytes that did not disassemble cleanly), `layout` (a global/segment that did not land on
its shipped offset), `import-ordinal` (local lib disagrees with shipped — see §10.3),
`residual` (a byte diff classified as non-reconstruction; still logged so a human can
confirm the classification), `runtime` (a runtime-segment fragment reproduced by
approximation rather than the exact library object).

**Rules the tool obeys:**
1. If a function cannot be reconstructed to a zero masked-diff **and** cannot be
   confidently classified as a known residual (§11), it is emitted as `@RECON-RAW` shipped
   bytes with a marker — **never** a fabricated C body that merely "looks right."
2. Any name not present in the shipped name/entry tables is a **placeholder**, tagged
   `@RECON-UNKNOWN symbol-name` — the tool never presents an invented name as the real one.
3. Any type/signature the tool inferred (rather than read from a recovered header or proved
   by the calling convention in the bytes) is tagged `@RECON-ASSUME type`.
4. Every marker is mirrored into `unknowns.json` (`{id, dll, file, line, category, text,
   confidence}`) and summarized in `RECON_NOTES.md` as a review worklist ordered by
   category and confidence. A DLL with open `@RECON-UNKNOWN` / `@RECON-RAW` markers is
   reported as **partially reconstructed**, with the exact count — it is never rounded up to
   "done."
5. The functional oracle (§10.4) still runs on a partial reconstruction (raw-byte regions
   execute as the shipped bytes), so behavior can be checked even while source gaps remain.

The purpose is a clean hand-review surface: a reviewer opens `RECON_NOTES.md`, works the
markers by lane, and every uncertain decision is exactly where the tool left it — with the
shipped bytes preserved so nothing is lost to a guess.

### 12.3 Pipeline

A driver that runs the pipeline per DLL:

```
for each DLL in WINDOWS/SYSTEM:
  P0 classify           -> {NE|PE, thunk?, fingerprint}         [ne.py]        deterministic
  if PE or thunk: record and defer (out of 16-bit scope)
  P1 parse NE           -> segments, relocs(resolved), entry map [ne.py]        deterministic
  P2 discover funcs     -> per-seg function list                 [disasm.py]    deterministic
  P3 RE to C            -> seg*.c + headers; mark unknowns/raw    [Ghidra + LLM/idioms]  assisted
     per function: compile->diff->brute-force to 0 masked diff   [omf.py, bruteforce.py]
     unreconstructed function -> @RECON-RAW shipped bytes (§12.2)
  P4 data layout        -> segdata.asm (+map-driven offset fit)  [MASM 5.1]     assisted
  P5 detect recipe      -> auto-pin compiler+/O from WEP bytes    [detect_recipe.py] deterministic
     no WEP / no match / ambiguous -> @REVIEW or disambiguate on 2nd fn (never guessed)
  P6 build              -> per-seg objs + LINK 5.20 @response    [DOSBox-X]     deterministic
  P7 layout fit         -> iterate link order/DGROUP vs .map      [recon.py]     assisted
  P8 normalize NE       -> compact stub, fix absolute fields      [ne_normalize.py] deterministic
  P9 verify             -> masked + reloc-resolved + functional   [segdiff.py, gate.sh] deterministic
     classify residuals; emit per-segment scoreboard
  emit deliverable set (§12.1): <module>.c, <name>.h, <datamodule>.asm[, runtime.asm],
       <module>.def (shipped ordinals), <name>.lnk, makefile, scoreboard.json,
       RECON_NOTES.md + unknowns.json (every @RECON-* marker indexed)
  status = done ONLY if zero open @RECON-UNKNOWN/@RECON-RAW; else "partial (k open)"
```

**Design goals for the tool (already largely realized in `tools/w95recon/`):**
- Keep the LLM/human out of the compile-diff loop; ~95% deterministic Python (zero tokens
  per DLL once idioms are known).
- **Brute force is the engine**: compile+diff is free, so enumerate the bounded source-form
  space per function and pick the zero-diff winner automatically. Add an axis generator that
  injects the common toggles (leading `int i`, loop form, signed/unsigned, goto/dup-exit,
  ternary/if) without manual markers.
- **Idioms compound**: every new idiom appended to `IDIOMS.md` lowers the cost of the next
  DLL. The first DLL is expensive; later DLLs mostly reuse.
- **Per-DLL isolation**: each DLL/segment builds in its own `_work/<name>/` with its own
  self-exiting DOSBox (never `taskkill`).
- **Scoreboard as the contract**: `recon.py status` reports `k/n functions byte-exact`;
  the residual classifier explains every non-match as (register|link-order|padding) or
  flags it as a real finding to fix.

**Per-DLL human-in-the-loop budget** shrinks over the sweep: Phase 3 (RE) and Phase 4/7
(layout fit) are the only assisted steps; both are increasingly automatable as the idiom
and layout-heuristic libraries grow. Phases 0–2, 5, 6, 8, 9 are already deterministic.

---

## 13. Toolchain & artifact inventory (version-keyed layout + manifest)

**Resolve every tool through the manifest, never a hardcoded path.** The toolchain is
organized version-keyed under `toolchain/`, and `toolchain/MANIFEST.json` maps
`role+version → path`. The pipeline calls `toolchain/toolchain.py` — `resolve('linker',
'5.20')`, `resolve('compiler','msc-6.0a')`, `resolve('lib','win31-kit','mdllcew')` — which
returns an absolute path or raises `ToolchainGap` with the manifest's explanation for a
missing tool (it never invents a path — a gap is surfaced for review, per §0/§12.2).

```
toolchain/
  MANIFEST.json  toolchain.py          <- source of truth + resolver
  compilers/  msc-6.0a  msvc-1.52c  msvc-1.52c-cdk  msvc-2.0
  assemblers/ masm-5.1  masm-6.13
  linkers/    link-5.20 (5.20.034)  link-5.60 (5.60.339)   [5.10/5.30/5.50 = marked GAPS]
  rc/         rc-3.1
  libs/       win31-kit (LIBW/MDLLCEW/LIBENTRY/crt0*/WINSTUB)  crt-startup-msc60
  sdks/       nt31-win32-sdk  nt351-ddk  masm-5.1a-update  newfiles
  archives/   compressed kits + msc-6.0a floppy images + loose ISOs (win16ddk, win31 SDK)
```

**Linker reality (from the corpus triage):** of 189 shipped NE16 DLLs, ~163 stamp LINK
**5.60** (we have it: `linkers/link-5.60`, surfaced from the 1.52c suite — its banner
mislabels itself 5.40), ~8 stamp **5.20** (`linkers/link-5.20`, LZEXPAND's), and 5.10/5.30/
5.50 (~16 DLLs) are **not yet extracted** — real gaps the manifest records. Validate each
linker version's codegen (compiler-ID-by-smallest-function, §6) before trusting byte-exact
output from it; only 5.20 is validated so far.

Other fixed inputs:
- `data/files/WINDOWS/SYSTEM/` — the shipped DLLs (oracles) + `USER.EXE`/`KRNL386.EXE`/`GDI.EXE`
  (import ground truth; see §10.3).
- `refs/tools/dosbox-x/bin/x64/Release/dosbox-x.exe` — the DOS host for DOS-hosted tools.
- `tools/w95recon/` — the tool: `ne.py`, `disasm.py`, `omf.py`, `verify.py`, `recon.py`,
  `bruteforce.py`, `autoaxis.py`, `ne_normalize.py`, `triage.py`, `IDIOMS.md`, `README.md`,
  this spec. Corpus map: `_work/triage_system.{csv,json}`, `_work/ne16_worklist.csv`.

## 14. Reference case status (LZEXPAND.DLL)

`tools/w95recon/_work/treeA/` (byte-exact tree) + `tools/w95recon/recovered/` (clean
functional tree, 18/18 regression). Build: `_work/treeA/build.sh` (DOSBox link →
normalize). Result: 12-segment NE, 9664 B (shipped 9936; delta is stub/pad + 4 register
residuals), **8/12 segments truly byte-identical** (seg1, seg4, seg5, seg8, seg10, WEP,
runtime, DATA) + DOS stub byte-identical; seg2/6/7/9 differ only in register tie-breaks.
All 44 functions recovered and functionally verified bit-identical against `expand.exe`.
