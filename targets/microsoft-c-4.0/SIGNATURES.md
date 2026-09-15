# Microsoft C 4.0 — Signature Database

> Living catalogue of byte/behavior signatures used to *identify* code before reconstructing it,
> in CodeBreaker's origin-triage spirit: **compiler/CRT/library code is identified, not
> reconstructed.** Companion: [NOTES.md](NOTES.md), [docs/PROJECT_MAP.md](docs/PROJECT_MAP.md).
>
> `.me` in the request is honored here as `.md` so it renders. Each entry records: what it is,
> how to detect it (bytes/regex/structure), confidence, and where it was confirmed.
>
> **Confidence tiers:** `CONFIRMED` (byte-verified on this corpus) · `STRONG` (canonical pattern,
> matched here) · `HYP` (hypothesis, not yet verified). Never promote HYP→CONFIRMED without proof.

---

## 1. Container / packer signatures

### SIG-PACK-001 — Microsoft EXEPACK loader  `[CONFIRMED]`
The wrapper on every Tier-A/B tool (MSC, C1, C2, C3, C3L, CL, CV, LINK, LIB).
- **MZ header tell:** `e_crlc (relocs) == 0`, `e_cparhdr == 0x20` (512-byte header),
  `e_ip == 0x0010`, `e_sp == 0x0080`.
- **Decompressor entry stub** (at `512 + CS*16 + IP`):
  ```
  8C C0        mov  ax, es
  05 xx xx     add  ax, imm16        ; + (exepack_size>>4) style fixup
  0E           push cs
  1F           pop  ds
  ...          (copy-up + unpack loop; reloc rebuild)
  ```
- **Trailer strings:** ASCII `RB` signature word + `"Packed file is corrupt"` near loader end.
- **Action:** unpack to recover real OEP + relocation table before any disassembly.
- **Confirmed:** MSC.EXE header @image 0x66d0 = `c030 0000 0000 b501 0008 9807 bb06 5242`
  → real_ip=0x30c0, real_cs=0, exepack_size=0x1b5, real_sp=0x800, real_ss=0x798, dest_len=0x6bb,
  sig=`RB`@+0x0E. **Decompressor stub byte-identical across all 10 packed tools** (same EXEPACK
  version): `8CC0 0510 00 0E 1F A304 00 0306 0C00 8EC0 …`.
- **Unpacker:** `workspace/scripts/unexepack.py` (deterministic, no deps). Decompress downward,
  skip `0xFF` padding, `0xB0`=FILL / `0xB2`=COPY, bit0 = last-block. Verified on all 10.

### SIG-PACK-002 — EXEPACK uncompressed leading region `[CONFIRMED]`
Sub-variant: some images leave the first *K* bytes uncompressed in place; the command stream only
regenerates `image[K:]`, so decompression ends with a `K`-byte residual that is filled verbatim
from the packed module's own low bytes. Observed K: C1=76, LINK=32, CV=8271, CVR=8378; K=0 for
MSC/CL/LIB/C2/C3/C3L.

### SIG-PACK-000 — Unpacked MZ (control) `[CONFIRMED]`
`MAKE.EXE` (relocs=8) and `LINK4.EXE` (relocs=1) are **plain MZ**, not packed — useful as
warm-up targets and as a control for the disassembler.

---

## 2. MZ / DOS structure signatures

### SIG-MZ-001 — real-mode segmented MZ `[CONFIRMED]`
All targets: `4D 5A` (`MZ`) magic, 16-bit real-mode, segmented (far calls, seg relocations).
Memory model per binary TBD from CRT startup fingerprint (see SIG-CRT-*).

---

## 3. CRT / startup signatures (ground truth = disk06 source)

> These identify the C run-time so it is **named, not reconstructed**. Bytes to be filled in
> from the *unpacked* binaries; the authored source on disk06 is the answer key. Placeholders
> below name the target functions; status starts `HYP` until matched on an unpacked image.

| ID | Function | Source file (disk06) | Detect by | Status |
|---|---|---|---|---|
| SIG-CRT-001 | `__astart` / entry | `CRT0.ASM` | entry prologue **`B4 30 CD 21 3C 02 73`** = `mov ah,30h/int 21h/cmp al,2/jae` (DOS-2.0 gate), then DGROUP/stack setup. Small model → near `call` to msg printer; large model → `9A` far `lcall`. | **CONFIRMED** — at OEP of MSC (0x30c0, near), C1 (0x2382, far), CV (0x417b, far). Matches disk06 `CRT0.ASM`. |
| SIG-CRT-002 | `_chkstk` | `CHKSTK.ASM` | stack-probe idiom (`sub sp` loop touching pages) | HYP |
| SIG-CRT-003 | `_setargv` | `STDARGV.ASM`/`SETARGV.ASM` | command-tail parse from PSP:0x80 | HYP |
| SIG-CRT-004 | `_setenvp` | `STDENVP.ASM` | environment-segment copy loop | HYP |
| SIG-CRT-005 | `__cintDIV`/FP init | `CRT0FP.ASM` | 8087/emulator vector patch | HYP |
| SIG-CRT-006 | msg/abort | `CRT0MSG.ASM`/`EXECMSG.ASM`/`NMSGHDR.ASM` | writes fixed abort strings via INT 21h AH=40h | HYP |

Macro/ABI context: `CMACROS.INC` defines the `cProc/cBegin/cEnd`, `save`, `parmB/W/D` calling
conventions used throughout — the shape of every CRT prologue/epilogue comes from here.

---

## 4. Library signatures (ground truth = shipped .LIB, disk03/04/05/08)

FLIRT-style, built **from the .LIB itself** (CodeBreaker `wt inverse omf` + `libmatch`/`libsig`).
Each model's C library: `SLIBC/CLIBC/MLIBC/LLIBC` (small/compact/medium/large), float packs
`?LIBFA` (alt math) / `?LIBFP` (emulator), `EM.LIB` (emulator), `87.LIB` (8087), `LIBH.LIB` (helpers).

| ID | Scope | Source | Method | Status |
|---|---|---|---|---|
| SIG-LIB-001 | small-model C RTL | `SLIBC.LIB` | OMF module/pubdef extraction → per-function byte sig w/ FIXUPP mask | TODO |
| SIG-LIB-002 | float (emulator) | `SLIBFP.LIB` etc. | same | TODO |
| SIG-LIB-003 | float (8087/alt) | `SLIBFA.LIB`,`87.LIB` | same | TODO |
| SIG-LIB-004 | Windows glue | `?LIBW.LIB`,`?WINLIBC.LIB` | same (disk08) | TODO |

The `.OBJ`s shipped loose (`SSETARGV.OBJ`, `SVARSTCK.OBJ`, `BINMODE.OBJ`, model variants) give
single-function, self-labelled signatures — the cleanest FLIRT seeds.

---

## 5. Compiler codegen idioms (C 4.0 code generator — C2/C3 output shape)

> The fingerprints of *how C 4.0 compiles C*, learned from the disk07 source↔EXE sample pairs
> (CIRCLE/COUNT/PI) and the disk06 CRT. These let the inverse engine attribute a code sequence to
> a specific C construct. Populate as calibration proceeds.

| ID | Idiom | Pattern (to capture) | Status |
|---|---|---|---|
| SIG-CG-001 | function prologue | `push bp / mov bp,sp / sub sp,N` (+ far variant, + `__chkstk` when frame large) | TODO |
| SIG-CG-002 | epilogue | `mov sp,bp / pop bp / ret`(near) or `retf` | TODO |
| SIG-CG-003 | switch/jump-table | table-in-CS indexed `jmp` layout | TODO |
| SIG-CG-004 | far-call thunking | model-dependent `call far`/`push cs;call near` | TODO |
| SIG-CG-005 | printf-family varargs | argument push order + stack cleanup (cdecl) | TODO |
| SIG-CG-006 | 32-bit long ops | `__aFldiv`/`__aFuldiv`/helper calls into LIBH | TODO |

---

## 6. Tool version / banner strings (to harvest from unpacked images)

Record the exact version/copyright strings once unpacked (they pin the exact build and often the
internal component versions of C1/C2/C3/LINK). Table seeded, values TBD.

| Binary | String | Status |
|---|---|---|
| MSC/CL | `Microsoft (R) C Compiler  Version 4.00` (@MSC image 0x5222) | **CONFIRMED** |
| MSC/CL | `Copyright (C) Microsoft Corp 1984, 1985, 1986.  All rights reserved.` | **CONFIRMED** |
| MSC | internal build stamp `4.02 Mon Jun 23 10:13:47 PDT 1986` (@0x638c) | **CONFIRMED** |
| MSC/CL | driver spawns `c0.exe c1.exe c2.exe c3.exe masm.exe link.exe` (@0x4d01) | **CONFIRMED** |

---

## 7. Source-module attribution — SCCS `@(#)` markers `[CONFIRMED]`

### SIG-SRC-001 — `@(#)what` version strings
Every tool embeds one `@(#)<file>.c:<rev>` string per translation unit (regex `@\(#\)([ -~]{3,60})`).
Ground-truth module names — used to cluster carved functions by owning source file and to identify
a module shared across tools (cross-pass libsig). Full inventory (11 tools; C1=40, C2=48, C3=35
units) in **[docs/SOURCE_MAP.md](docs/SOURCE_MAP.md)**. Anchors (MSC image): `cc_main.c:1.28`
@0x4cd6, `getflags.c:1.30` @0x6718, `error.c:1.4` @0x6264. CV/CVR/DOSSEG carry none (stripped).

---

_Add new signatures with the next free ID in each section. Cite the file + offset where confirmed._
