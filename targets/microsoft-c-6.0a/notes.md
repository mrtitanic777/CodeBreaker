# Microsoft C 6.0a — Reverse Engineering Notes

Living log for the RE of the **Microsoft C Professional Development System 6.0a** toolchain.
Companion files: [`signatures.md`](signatures.md) (fingerprint/signature catalog) and
[`MAP.md`](MAP.md) (the full end-to-end project plan).

- **Started:** 2026-09-14
- **Analyst:** mrtitanic777@gmail.com
- **Tool:** CodeBreaker (`C:\Users\mrtit\Documents\GitHub\CodeBreaker`)
- **Goal:** Recover readable, faithful source/structure for the C 6.0a compiler passes
  (front end, code generator, optimizer, driver) and supporting toolchain, verified
  byte-exact where the code class allows.

---

## 1. Legal / scope note
Analysis is on binaries the user possesses (WinWorld preservation copy). Output is a derivative
of Microsoft's copyrighted binaries — kept local, not distributed. CodeBreaker ships as tool-only;
no Microsoft code lives in either repo. Unpacked/extracted binaries are git-ignored.

## 2. Target inventory

Source media: 7 × 5.25" 1.2 MB FAT12 floppy images (`Disk1..Disk7`), MS-compressed (`.EX$`/`.OB$`/`.LI$`
= SZDD). The **compiler passes are already decompressed** into the CodeBreaker period toolchain:
`CodeBreaker\toolchains\period\compilers\msc-6.0a\BIN`.

### Primary RE targets — the compiler itself (decompressed, in BIN/)
| File | Size | Role | Notes |
|---|---:|---|---|
| `CL.EXE`  | 28,858  | Compiler **driver** — parses cmdline, chains passes | smallest, best warm-up target |
| `C1.EXE`  | 148,060 | **Front end** (preprocess + parse + semantic), default mem model | |
| `C1L.EXE` | 189,952 | Front end, **large**-model build | diff against C1 = model-portability insight |
| `C2.EXE`  | 354,338 | **Code generator** — the heart | file ≫ MZ image ⇒ has trailing overlay/data |
| `C3.EXE`  | 150,076 | **Optimizer / post-pass** | |

Error/message data files (plain text/near-text, ship next to passes):
`C1.ERR`, `C23.ERR`, `CL.ERR`, `CL.MSG`.

### Secondary targets — linker, librarian, utilities (still `.EX$` on disks, need extraction)
- Disk4: `LINK.EX$`, `ILINK.EX$`, `ILINKSTB.OV$`, `LIB`(via CL), `IMPLIB.EX$`, `CVPACK.EX$`,
  `EXEHDR.EX$`, `NMAKE.EX$`, PWB (`PWB.CO$`, `PWBED.EX$`, …)
- Disk1: `SETUP.EXE` (plain), `QCC.EX$`, `LIB.EX$`, `C2L/C3L`
- Disk2: `CV.EX$` (CodeView), `QH.EX$`
- Disk7: `BIND.EX$`, `RC.EX$`, `RCPP.EX$`, `C3.EX$`, `NMAKE.EX$`
- Libraries (`.LI$`, Disk3/5/7): `?LIBC*` CRT variants, `GRAPHICS`, `EM` (emulator),
  `87` (8087), `PGCHART`, `LLIBCMT`, OS/2 libs. → ground truth for CRT/lib **signatures**.
- CRT sources (Disk4 `STARTUP\`): `CRT0*.AS$`, `CHKSTK.AS$`, `STDARGV.AS$`, … — authored ASM
  for the startup/CRT, i.e. *known-answer* material for signature building & verification.

## 3. Format findings (verified)

- All five passes are **real-mode DOS MZ** (no NE/PE; `e_lfanew`=0).
- **All five are Microsoft `LINK /EXEPACK`-compressed.** Confirmed by the unpacker error string
  `"Packed file is corrupt"` in each, `Relocations = 0`, and entry point sitting at the very end
  of the MZ image (the decompressor stub). **They MUST be unpacked before real disassembly.**
- `C2.EXE`: file 354,338 B vs MZ image 196,114 B ⇒ ~158 KB trailing **overlay** after the packed
  image (investigate: overlay code the pass pages in, vs. packed-data tail).
- No LZEXE/PKLITE/DIET — only EXEPACK. Good: EXEPACK is well-understood and losslessly reversible.

See [`signatures.md`](signatures.md) §"MZ header fingerprints" for exact per-file header values.

## 4. Environment

| Component | Path / value | Status |
|---|---|---|
| CodeBreaker repo | `C:\Users\mrtit\Documents\GitHub\CodeBreaker` | OK |
| Python | 3.14.0 (repo also has 3.12 pyc) | OK |
| `wt.py` entry | `python wt.py <cmd>` | OK |
| DOSBox-X | `toolchains\dosbox-x\...\dosbox-x.exe` | OK (doctor) |
| 7-Zip CLI | `C:\Program Files\7-Zip\7z.exe` | OK (reads FAT12 imgs) |
| Period MSC 6.0a | `toolchains\period\compilers\msc-6.0a` | OK (BIN/INCLUDE/LIB) |
| MASM | 5.1 + 6.13 under `toolchains\period\assemblers` | OK |
| Ghidra | `...\ghidra_12.1.3_PUBLIC` (optional cross-check) | configured |

### CodeBreaker capabilities relevant to this target (real-mode / MSC6)
- `wt inverse mzdis <f> header|dis|relocs` — MZ loader + 16-bit disassembler (verified on all 5).
- `wt inverse mzasmrecon` — MZ → byte-exact ASM reconstruction.
- `wt inverse omf` — OMF `.OBJ`/`.LIB` reader (for library signature ground truth).
- `wt inverse msc6{dec,fn,inv,reg,walk,_oracle}` — MSC-6.0a compiler-**inverse** suite
  (bytes→byte-exact C for code the *period MSC6 emitted*). NB: built to recognize MSC6 *output*;
  the compiler passes themselves were likely built with MSC6 → these idioms should apply to them.
- `wt msc6 …` — MSC-6.0a byte-exact 16-bit NE rebuild engine (`src/engines/msc6/`, has IDIOMS.md,
  SPEC.md, `win95_dllmap.json`) — reuse its MSC6 code-idiom model.
- `wt behavior …` — Unicorn oracle (dynamic recovery). Usable on unpacked real-mode segments.
- `src/disasm/disasm16.py` — 16-bit disassembly core.

## 5. Working conventions
- Outputs go under `work/` (git-ignored): `unpacked/`, `extracted/`, `disasm/`, `oracle/`.
- Never disassemble the packed image — always the unpacked one. Record unpack method + resulting
  header in `signatures.md` so results are reproducible.
- Triage every function by **origin first** (CRT/startup/lib vs. authored compiler code) — see MAP Phase 3.
- Faithfulness over coverage; flag anything unproven; verify by re-assembly/rebuild where possible.

## 5a. Phase 1 result — unpacked, and a free architecture map

All five passes **unpacked and verified** (`tools/unexepack.py`; fingerprints/hashes in
`signatures.md` §3). Each disassembles to a real MSC startup at its real entry.

**Major find:** the passes carry SCCS `@(#)<file.c>:<rev>` *what-strings* — the compiler's own
internal **source-module names**. Full list in [`work/module_manifest.txt`](work/module_manifest.txt).
This is Phase-5 architecture recovery handed to us for free. Summary:

- **`CL.EXE`** — driver (8 modules): `cc_main.c`, `cc_msdos.c`, `flags.c`, `targetcode.c`,
  `strings.c`, `error.c`, `get_err.c`, `VERSION.c`. Banner: *"C Optimizing Compiler Version"*.
- **`C2.EXE`** — the big pass, **parser + codegen + global optimizer** (~55 modules). Front-end/IL:
  `grammar.c`, `trees.c`, `typer.c`, `getattr.c`, `il.c`/`sil.c`/`pril.c`/`initil.c` (intermediate
  language), `p2symtab.c`, `pragma.c`/`p2pragma.c`, `mactab.c`. Codegen/opt: `expand.c`, `gencode.c`,
  `emit.c`, `codegen.x`, `optimize.c`, `confold.c` (const-fold), `flow.c`, `newcse.c` (CSE),
  `globregs.c` (global reg alloc), `canon.c`/`cgcanon.c`, `genswtch.c`, `intrinsc.c`/`intrinfo.c`,
  `ndp.c` (8087). Machine-dependent (`*MD`): `regMD.c`, `globregsMD.c`, `suMD.c`, `switchMD.c`.
- **`C3.EXE`** — back end: **OMF object emitter + post-optimizer + listings/debug** (~35 modules).
  OMF: `omf.c`, `newomf_ms.c`, `omfMD.c`. Optimizer: `pgo.c`/`pgo2.c`/`pgoMD.c`/`pgoMD2.c`.
  Listings/disasm: `disasm2.c`, `disasm3.c`, `srclist.c`/`srclist2.c`. Debug/CodeView:
  `dbil.c`, `dbread.c`. Also `newcode.c`, `labels.c`, `p3io.c`, `p3symtab.c`. Banner:
  *"Microsoft C Compiler Version"*.
- **`C1.EXE` / `C1L.EXE`** — **no `@(#)` what-strings found** (open Q#6). Front-end pass 1
  (preprocess/parse → intermediate file). C1 vs C1L = compiler's own near vs large model build.

**Revised pass-role model (evidence-based, supersedes the initial guess):** CL drives; **C1/C1L =
pass 1 front end** (→ binary intermediate); **C2 = pass 2** (re-reads IL, does the heavy
parsing-into-trees, codegen, and global optimization); **C3 = pass 3** back end (OMF emission,
post/peephole optimization, listings, CodeView debug info). The `il/sil/pril` modules in C2 and the
`p2symtab`/`p3symtab` split confirm a multi-pass **intermediate-language** design — the C1↔C2↔C3 IL
format is the headline artifact to recover (MAP Phase 5).

## 6. Open questions / to-investigate
1. ~~Best EXEPACK unpack path~~ **RESOLVED**: wrote `tools/unexepack.py` (in-place backward RLE +
   reloc rebuild), verified on all 5 passes. Could contribute it to CodeBreaker `wt inverse`.
2. `C2.EXE` overlay: structure and loader mechanism (MS overlay manager? `_OVERLAY`?).
3. Memory model of each pass (tiny/small/…/large) and which CRT variant they link — pins signatures.
4. Do CodeBreaker's `msc6` idioms match code emitted for these passes? (self-hosting hypothesis:
   C 6.0a compiler built with C 6.0 / MASM). Validate on unpacked CL.EXE first.
5. Segment map of each pass (how many code/data segments once unpacked; overlay segments).
6. **Why do C1/C1L carry no `@(#)` what-strings** when C2/C3/CL do? Grep C1 for other version/source
   markers (different build convention in pass 1?).

## 7. Log
- 2026-09-14 — Env verified (`wt doctor`), inventory built, all 5 passes confirmed EXEPACK-packed,
  MZ headers captured, workspace + git initialized. Deliverables `notes.md`/`signatures.md`/`MAP.md` written.
- 2026-09-14 — **Phase 1 done.** Wrote & validated `tools/unexepack.py`; unpacked all 5 passes to
  `work/unpacked/` (real entries verified as MSC startups; hashes in `signatures.md` §3). Harvested
  SCCS what-strings → `work/module_manifest.txt`; recovered the internal source-module map of C2/C3/CL
  and revised the pass-role model (§5a). Open: C1/C1L have no what-strings; C2 overlay tail unhandled.
- 2026-09-14 — **Phase 2 done.** Wrote `tools/carve.py` (Capstone 16-bit; recursive descent + linear
  sweep). Learned the segmented passes hide most code behind **indirect dispatch tables** (a codegen's
  IL-node handlers) — pure recursive descent found only ~19 funcs in C2, so leaders are seeded by
  sweep (near/far call targets + `55 8B EC` prologues). Result: `work/<pass>/{functions.json,floor.asm}`
  + `work/phase2_summary.md` — **~4,150 functions**, 81–91% code, 96–99% byte-decoded. ASM floor is
  faithful (decode of real bytes; reloc/data → `db`). Round-trip re-assembly verify not yet wired
  (MASM/nasm-masm config) → Phase 6. mzasmrecon's own verify path needs MASM-under-DOSBox (absent here).
- 2026-09-14 — **Phase 3 first pass.** Period `.LIB`s already decompressed in `.../msc-6.0a/LIB`;
  `wt inverse omf` reads them fine. Ran `wt inverse libmatch` (FLIRT-style) on all 5 → `work/triage/`
  + `work/triage_summary.md`. CRT identified: CL 41 / C1 45 / C1L 43 / C2 22 / C3 24; the rest are
  authored compiler code. CL's CRT hits (`_spawnve`, `__dospawn`, `__cenvarg`, `_mktemp`) confirm the
  driver chains passes via spawn + temp files. **Edited CodeBreaker** `src/engines/inverse/libmatch.py`:
  its module walker only accepted Lattice's `PROG` code segment → 0 hits on MSC; added `_TEXT`/`*_TEXT`
  (one line, backward-compatible). This is a real fix for any MSC target — offer to upstream it.
