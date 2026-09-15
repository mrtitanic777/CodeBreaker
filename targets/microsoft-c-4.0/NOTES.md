# Microsoft C Compiler 4.0 — Reverse Engineering Lab Notebook

> Running log of findings, decisions, and dead-ends. Newest dated entry at the bottom
> of each section. Companion files: [SIGNATURES.md](SIGNATURES.md) (the signature DB),
> [docs/PROJECT_MAP.md](docs/PROJECT_MAP.md) (the end-to-end plan).

---

## 0. Target at a glance

- **Product:** Microsoft C Compiler **4.0**, released 1986, for **MS-DOS** (16-bit real mode).
- **Media:** eight 360 KB 5.25" floppies, FAT12, OEM `IBM  3.2` (DOS 3.2 formatter).
- **Historical note:** 4.0 introduced **CodeView** (source debugger), **MAKE**, the compact
  memory model, and shipped **C run-time startup source**. This is the direct ancestor of the
  MSC 5/6 line that later became Visual C++.
- **Source input:** `Microsoft C Compiler 4.0 (5.25-360k)/disk01..08.img` (byte-for-byte floppy images).

### Disk map
| Disk | Volume | Part # | Contents |
|---|---|---|---|
| 01 | C COMPILER | 048-014-052 | MSC, C1, C2, C3 |
| 02 | C UTILITIES | 048-014-053 | CL, CV, EXEMOD, EXEPACK, LIB, MAKE, SETENV, README |
| 03 | C INC LIB S | 048-014-054 | LINK, small-model libs, **headers (.H)**, EMOEM.ASM, DEMO/SIEVE.C |
| 04 | C LIB M C | 048-014-055 | medium + compact model libs |
| 05 | C LIB L | 048-014-056 | large model libs, **C3L.EXE**, MOUSE.COM |
| 06 | C STARTUP | 048-014-060 | **CRT startup source** (.ASM/.C/.INC) + MAKEFILE |
| 07 | CV LEARN | 048-014-058 | Learning CodeView (CVR.EXE, tutorial scripts, sample .C/.EXE) |
| 08 | C WINDOWS | 048-014-063 | Windows 1.x dev kit update: LINK4, Windows libs, HELLO sample |

Everything extracted to `extracted/diskNN/` (148 files) via a pure-Python FAT12 walker
(`workspace/scripts/` — see §Environment).

---

## 1. Binary classification (the reverse-engineering corpus)

**All primary targets are 16-bit real-mode DOS MZ executables.** No NE, no PE. This puts them
outside CodeBreaker's NE/PE mainline and squarely on the **`wt inverse` 16-bit MZ track**
(`mzdis`, `mzasmrecon`, `raiser16`, MS-C 2.03 oracle) plus custom work.

### Tier A — the compiler itself (primary goal)
| File | Disk | Size | Role (hypothesis) |
|---|---|---:|---|
| `MSC.EXE` | 01 | 27,269 | Compiler **driver** (older `msc` front end; pre-`cl`) |
| `CL.EXE` | 02 | 27,381 | Newer **`cl` driver** (near-twin of MSC.EXE by size/entry) |
| `C1.EXE` | 01 | 130,757 | **Pass 1** — preprocessor + parser + semantic (front end) |
| `C2.EXE` | 01 | 116,991 | **Pass 2** — code generator |
| `C3.EXE` | 01 | 83,481 | **Pass 3** — optimizer / final code emit (small/med/compact) |
| `C3L.EXE` | 05 | 116,909 | **Pass 3, large-model** build variant |

### Tier B — the toolchain utilities
| File | Disk | Size | Role |
|---|---|---:|---|
| `LINK.EXE` | 03 | 47,547 | DOS segmented linker |
| `LINK4.EXE` | 08 | 60,006 | Windows 1.x linker (**NOT packed**) |
| `LIB.EXE` | 02 | 29,359 | OMF library manager |
| `MAKE.EXE` | 02 | 23,418 | Build tool (**NOT packed**) |
| `CV.EXE` / `CVR.EXE` | 02/07 | ~141 K | CodeView debugger (CVR = "learning" restricted build) |
| `EXEMOD.EXE` | 02 | 11,034 | EXE header modifier |
| `EXEPACK.EXE` | 02 | 11,684 | The **packer** used on the other tools (self-packed) |
| `SETENV.EXE` | 02 | 9,012 | Environment-size setter |
| `DOSSEG.EXE` | 06 | 4,088 | Segment-order helper |
| `RESPOND.COM` | 07 | 124 | Tiny .COM (tutorial input driver) |

### Tier C — ground-truth artifacts (NOT reverse targets — they are the *answer key*)
- **CRT startup SOURCE (disk06):** `CRT0.ASM`, `CRT0DAT.ASM`, `CHKSTK.ASM`, `STDARGV.ASM`,
  `STDENVP.ASM`, `SETARGV.ASM`, `STDALLOC.ASM`, `NMSGHDR.ASM`, `CHKSUM.ASM`, `EXECMSG.ASM`,
  `CRT0FP.ASM`, `CRT0MSG.ASM`, `_WILD.ASM`, `WILD.C`, plus `CMACROS.INC`, `MSDOS.INC`,
  `BRKCTL.INC`, `REGISTER.H`, `MAKEFILE`. → **These let us verify** libsig hits against the
  actual authored source of the CRT.
- **Headers (disk03):** full C 4.0 `.H` set (STDIO, STDLIB, STRING, MATH, DOS, …) →
  the type/prototype ground truth for `fmtargs` / type propagation.
- **OMF libraries (disk03/04/05/08):** `?LIBC.LIB`, `?LIBFA/FP.LIB`, `EM.LIB`, `87.LIB`,
  `LIBH.LIB`, Windows `?LIBW.LIB`, `?WINLIBC.LIB`, and `.OBJ` files (SETARGV/VARSTCK/BINMODE).
  → FLIRT-style **library signatures built from the shipped .LIB itself** (CodeBreaker `libsig`).
- **Sample apps (disk07):** CIRCLE/COUNT/PI `.C` + matching `.EXE` — a self-contained
  *source ↔ binary* pair set, i.e. a **calibration corpus** for the C-4.0 codegen model.

---

## 2. Packing — the first wall (SOLVED: identified)

**Finding (2026-09-14):** every Tier-A/B executable except MAKE.EXE and LINK4.EXE is compressed
with **Microsoft EXEPACK**. Evidence, uniform across MSC/C1/C2/C3/C3L/CL/CV/LINK/LIB:
- MZ header shows `relocations = 0`, header size = 512, `IP = 0x0010`, `SP = 0x0080`
  (EXEPACK's fixed loader layout — the real relocations/entry are packed).
- Decompressor entry stub begins `8C C0  05 xx xx  0E  1F` = `mov ax,es / add ax,imm / push cs / pop ds`.
- The literal `RB` EXEPACK signature and the `"Packed file is corrupt"` stub string are present
  near the end of the loader (e.g. MSC.EXE: `RB` @0x68de-ish, message @file 0x69e1).

**Consequence:** disassembly of the raw file only shows the ~256-byte EXEPACK loader. **We must
unpack first** to recover the true code/data image, real CS:IP, and the relocation table.

**SOLVED (session 2): pure-Python unpacker `workspace/scripts/unexepack.py`.** Deterministic,
no deps. EXEPACK header is the 16-byte variant (`RB` at +0x0E); algorithm = read the compressed
stream downward, skipping 0xFF paragraph padding, opcodes `0xB0`=FILL / `0xB2`=COPY with
bit0 = last-block flag. **Two sub-variants seen, both handled:** some images leave the first *K*
bytes of the image **uncompressed in place** (the command stream regenerates only `image[K:]`),
so a nonzero decompress residual is filled from the packed module's own low bytes. The decompressor
**stub is byte-identical** across all packed tools (same EXEPACK version).

**Result — all 10 packed tools unpacked to coherent images (verified at OEP):**

| Tool | OEP CS:IP | SS:SP | image | relocs | notes |
|---|---|---|---:|---:|---|
| MSC | 0000:30c0 | 0798:0800 | 27,568 | 55 | `__astart`, small model (near call) |
| CL | 0000:2f1a | 0760:0800 | ~27.7K | — | `__astart` |
| LIB | 0000:42e6 | 0873:0800 | ~28.7K | — | `__astart` |
| C1 | 0172:0c62 | 222f:2000 | 131,680 | 2,951 | `__astart`, **large model** (`lcall`) |
| C2 | 0000:ea77 | 2025:2000 | ~127K | — | large model |
| C3 | 0000:c9bd | 163e:2000 | ~84K | — | |
| C3L | 0000:09ac | 1d64:2000 | ~126K | — | large-model pass-3 |
| CV | 0204:213b | 28b5:1000 | 139,088 | 2,260 | `__astart`, large model |
| CVR | 020b:2136 | 28c2:1000 | 139,296 | 2,270 | CodeView (learning build) |
| LINK | 0000:0120 | 10b4:0100 | 47,968 | 10 | **own** startup (older, not CRT `__astart`) |

Each unpacked file + a `.json` manifest (OEP/SS:SP/relocs) is in `workspace/unpacked/`.
OEP verified by disassembly: C1/CV/MSC land exactly on the MS-C `__astart` prologue
(`mov ah,30h / int 21h / cmp al,2 / jae`) that matches `CRT0.ASM` on disk06.

**Plain MZ (no unpack needed) — direct disasm targets:** `MAKE.EXE`, `LINK4.EXE`, `EXEMOD.EXE`,
`SETENV.EXE`, `DOSSEG.EXE`, and **`EXEPACK.EXE` itself** (relocs=5; it merely *contains* the
"Packed file is corrupt" string as its own message — it is not self-packed).

**Remaining verification (M1 close-out):** boot one unpacked tool in DOSBox-X (e.g. `C1 -?`) as an
independent runtime check, and optionally reproduce the EXEPACK wrapper to match the shipped file
byte-for-byte (stretch).

---

## 3. Toolchain provenance (hypotheses to confirm)

- The C 4.0 compiler is **largely self-hosted**: C1/C2/C3 are C programs compiled by an
  earlier MS C (4.0 or its 3.x bootstrap) and assembled/linked with MASM + LINK. The CRT they
  link is the very CRT whose source ships on disk06.
- Therefore the **best codegen oracle for these binaries is C 4.0 itself** once we can run it
  (in DOSBox-X). Strategy: recover source, recompile with the *shipped* C 4.0, and compare
  byte-for-byte — the same "period toolchain as oracle" doctrine CodeBreaker uses elsewhere.
- CodeBreaker's nearest built-in model is **MS-C 2.03** (`wt inverse msvc2*`, 16-bit); expect
  partial applicability. A **C 4.0 codegen model** likely needs to be authored (see MAP §Phase 4),
  calibrated on the disk07 source↔EXE sample pairs and the disk06 CRT source↔.OBJ pairs.

---

## 4. Environment setup (done)

- **Working repo:** `C:/Users/mrtit/Documents/GitHub/MicrosoftC4.0` — `git init` done, `core.autocrlf=false`.
- **Tool:** CodeBreaker at `C:/Users/mrtit/Documents/GitHub/CodeBreaker`, Python 3.14, `wt.py doctor` OK,
  DOSBox-X configured. (MASM path in config points at a `reference/masm` that isn't present here —
  fine; not needed until the recompile/verify phase.)
- **Layout created:**
  ```
  extracted/diskNN/        floppy contents (input, read-only)
  workspace/unpacked/      EXEPACK-unpacked executables (RE inputs)
  workspace/disasm/        disassembly listings
  workspace/recon/         reconstructed C/ASM
  workspace/source_gt/     ground-truth source copied from disk06/03/07 for reference
  workspace/scripts/       our helper scripts (FAT12 walker, unpacker, drivers)
  docs/                    PROJECT_MAP.md and supporting docs
  ```

### CodeBreaker command cheat-sheet for THIS target (16-bit MZ)
```
python wt.py inverse mzdis      <file>   # 16-bit MZ disassembler
python wt.py inverse mzasmrecon <file>   # byte-exact ASM reconstruction (MZ)
python wt.py inverse raiser16   <file>   # raise 16-bit asm -> C
python wt.py inverse omf        <lib>    # OMF .LIB/.OBJ parser (for libsig ground truth)
python wt.py inverse libmatch   ...      # match library functions
python wt.py inverse idioms     ...      # compiler idiom recognition
python wt.py behavior <cmd>     <file>   # Unicorn oracle (dynamic) — works on real-mode? verify
python wt.py doctor                      # dependency check
```
(Full inverse subcommand list captured in the map; `msc6`/`msvc2` families target NE/2.03 —
reuse what transfers, author a C-4.0 model for the rest.)

---

## 5. Open questions / TODO seeds
- [ ] Unpack all EXEPACK targets; verify OEP + relocations two independent ways.
- [ ] Does CodeBreaker's `behavior` (Unicorn) oracle support real-mode DOS + INT 21h? (needed for dynamic track)
- [ ] Confirm MSC.EXE vs CL.EXE relationship (two drivers? one legacy?).
- [ ] Build the disk07 source↔EXE calibration pairs; measure C-4.0 codegen signatures.
- [ ] Stand up C 4.0 running inside DOSBox-X to serve as its own recompile oracle.

---

_Log entries below, newest last._

### 2026-09-14 — session 3
Wrote `workspace/scripts/carve.py` (recursive-descent carve + call graph + strings, no assembler).
MSC.EXE: 151 fns, 528 edges, 0 far calls (small model confirmed), _TEXT≈[0,0x4bda), data after.
**Breakthrough:** every tool embeds SCCS `@(#)` markers → recovered the **entire original
source-file tree** of the toolchain (C1=40, C2=48, C3=35 units, etc.) with revisions — see
[docs/SOURCE_MAP.md](docs/SOURCE_MAP.md). Confirmed version/build strings (SIG-VER, SIG-SRC-001).
Pipeline = driver(cc_main)→c1(front end, incl. `grammar.y` yacc)→c2(optimizer)→c3(OMF emit), IL
temp files between. MSC≡CL except the DOS-glue unit. Reconstruction is now **per-module**, shared
modules identified across passes. Assembler-backend decision still deferred (per request).

### 2026-09-14 — session 2
Wrote `workspace/scripts/unexepack.py`. Unpacked **all 10** EXEPACK tools; OEPs verified against
`CRT0.ASM` (`__astart`). Found the EXEPACK "uncompressed leading region" sub-variant (SIG-PACK-002).
Classified the 4 non-EXEPACK files as plain MZ. **M1 essentially done** (pending a DOSBox run check).
Promoted SIG-PACK-001, added SIG-PACK-002, confirmed SIG-CRT-001. Next: shake out `mzdis` byte-exact
floor on MAKE/LINK4 (M2), then C1/C2/C3.

### 2026-09-14 — session 1
Extracted all 8 disks (FAT12 walker). Classified corpus. Identified universal **EXEPACK** on the
compiler/tools. Confirmed source & library ground-truth is on-disk (disks 03/06/07). Set up repo +
workspace. Wrote this notebook, SIGNATURES.md, and the end-to-end PROJECT_MAP.md.
