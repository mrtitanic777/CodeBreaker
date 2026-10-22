# Microsoft C 6.0a — Signature Catalog

Fingerprints and signatures for identifying binaries, the packer, the toolchain that built them,
and (as we build them) CRT/library/compiler-idiom signatures for **origin triage**.
Companion: [`notes.md`](notes.md), [`MAP.md`](MAP.md).

> Naming: the request said `signatures.me`; using the conventional `signatures.md`. Say the word to rename.

---

## 1. Packer signature (VERIFIED — applies to all 5 passes)

**Microsoft `LINK /EXEPACK`.** Every compiler pass is EXEPACK-compressed.

- Detector: ASCII string **`Packed file is corrupt`** present in the file (the EXEPACK
  in-stub error handler).
- Corroborating: MZ `e_crlc` (relocation count) = **0**, and entry `CS:IP` resolves to an image
  offset at/near the **end** of the MZ image (the decompressor stub runs first).
- Action: **must be unpacked** before disassembly. Record the unpacked header here once done.
- Non-signatures (checked, absent): `LZ91` (LZEXE), `PKLITE`, `DIET`, `aPACK`.

## 2. MZ header fingerprints (VERIFIED — packed images, as shipped)

First 16 bytes and key header fields, exactly as on disk (all in BIN/):

| File | Magic+first16 (hex) | File size | MZ image size | Entry CS:IP (img off) | Relocs |
|---|---|---:|---:|---|---:|
| `CL.EXE`  | `4d5a ba00 3900 0000 2000 9701 9701 2c07` | 28,858  | 28,346 (0x6eba)   | 06d6:0010 (0x6d70) | 0 |
| `C1.EXE`  | `4d5a 5c00 2201 0000 2000 dc03 ffff 7625` | 148,060 | 147,548 (0x2405c) | 2387:0010 (0x23880) | 0 |
| `C1L.EXE` | `4d5a 0000 7301 0000 2000 8e04 ffff c632` | 189,952 | 189,440 (0x2e400) | 2ce9:0010 (0x2cea0) | 0 |
| `C2.EXE`  | `4d5a 1200 8101 0000 2000 f91a ffff 5049` | 354,338 | 196,114 (0x2fe12) | 2ea0:0010 (0x2ea10) | 0 |
| `C3.EXE`  | `4d5a 3c00 2601 0000 2000 2c04 ffff e725` | 150,076 | 149,564 (0x2483c) | 23b3:0010 (0x23b40) | 0 |

Notes:
- `C2.EXE`: file (354,338) ≫ MZ image (196,114) ⇒ **~158 KB trailing overlay/data** past the packed image.
- All headers: `e_cparhdr` = 0x20 (512-byte header). Stack SS:SP = ssss:0080 across passes (EXEPACK stub convention).

## 3. Unpacked-image fingerprints (DONE — Phase 1)

Unpacked with `tools/unexepack.py` (our EXEPACK reverser — see §1a). Each verified: the image at
its real `CS:IP` disassembles to a genuine MSC real-mode C startup (`int 21h`/AH=30h DOS-version
check → CRT stack init → `call __cinit`/`main` → `int 21h`/AX=4Cxx exit). Outputs in `work/unpacked/`.

| Unpacked | real CS:IP | real SS:SP | relocs (rebuilt) | image size | SHA-256 (of `work/unpacked/*.exe`) |
|---|---|---|---:|---:|---|
| `CL.exe`  | 0000:3fba | 0783:1000 | 4     | 29,024 (0x7160)  | `b63489532f50c64e…edcc1bff` |
| `C1.exe`  | 1624:26d4 | —         | 845   | 151,408 (0x24f70)| `6691298dec82c925…c158fa1` |
| `C1L.exe` | 1d12:10de | —         | 2,575 | 202,480 (0x316f0)| `e6e117ce10de3244…9b6553c2` |
| `C2.exe`  | 0000:5645 | —         | 2,400 | 295,136 (0x480e0)| `d660d3798809d112…00f08d73` |
| `C3.exe`  | 0ef8:a960 | —         | 1,501 | 151,904 (0x25160)| `130fda6a6b2b785a…c2c84dc8` |

- CL & C2 have `real_cs=0` (single/near code group at base); C1/C1L/C3 have a non-zero real CS
  (code group offset within the image) — expected for the larger, multi-segment passes.
- C2's real-entry startup is the **large-model** variant (`push ds; mov ax,SEG; mov ds,ax`), fitting
  the biggest pass; CL/C1's are the standard variant.
- CRT banner in every image: `MS Run-Time Library - Copyright (c) 1990, Microsoft Corp` → confirms
  genuine 1990-era MSC binaries with the CRT statically linked.
- **`C2.EXE` overlay:** the ~158 KB tail past the MZ image is NOT covered by unexepack (it processes
  the MZ image only); unpacked image is 295 KB. The overlay still needs separate handling (Phase 1/5).

### 1a. EXEPACK format notes (as reversed here — for reproducibility)
16-byte header at `CS:0` of the load module: `real_ip, real_cs, mem_start, exepack_size, real_sp,
real_ss, dest_len(paras), signature=0x4252 "RB"`. Decompression is **in-place, backward**: only the
**upper** region is compressed; the **lower** region is stored uncompressed at the start of the load
module and copied through unchanged (this was the key gotcha — see `tools/unexepack.py`). RLE opcodes:
`0xB0/0xB1`=fill(count,byte), `0xB2/0xB3`=copy(count); **bit0 set = last command**; count word sits
directly below the opcode (low byte lower). Trailing `0xFF` padding is skipped first. The packed
**relocation table** (16 segment-groups of `count,offset…`) sits **after** the embedded
`"Packed file is corrupt"` string, then zero padding to the block end.

## 4. Toolchain self-fingerprint (TO CONFIRM)

Hypothesis: the C 6.0a passes were themselves built with **Microsoft C 6.0 + MASM 5.1** and linked
with the period LINK (EXEPACK). Evidence to collect:
- CRT startup idiom / `_astart` shape in the unpacked image (matches Disk4 `STARTUP\CRT0*.ASM`).
- Version/copyright strings (`Microsoft (R) C ...`, build banners) — extract with strings post-unpack.
- Memory model per pass (segment count + far-call idioms) and which CRT library variant is linked.
- Cross-check against CodeBreaker's `msc6` idiom model (`src/engines/msc6/IDIOMS.md`).

## 5. CRT / library signatures (TO BUILD — ground truth on the disks)

The distribution ships the exact `.LIB`/`.OBJ` these binaries statically linked — perfect FLIRT-style
ground truth. Build signatures from them and use to mark library code as *toolchain, not authored*:

| Library (disk) | Covers |
|---|---|
| `SLIBC*/MLIBC*/LLIBC*/CLIBC*` (Disk3/5) | C runtime, per model (S/M/C/L) × (A/P/…) variants |
| `87.LIB`, `EM.LIB` (Disk3) | 8087 math / floating-point emulator |
| `GRAPHICS.LIB`, `*GRAPH*`, `PGCHART.LIB` | graphics / presentation-graphics |
| `LIBH.LIB` | helper library |
| `STARTUP\*.ASM` (Disk4) | authored CRT startup — known-answer for `crtsig` |

Plan: `wt inverse omf` to read each `.LIB` → per-function byte patterns → signature DB →
apply during triage (MAP Phase 3). Record coverage stats here as built.

**Done (first pass, `tools/triage.py` + `wt inverse libmatch`):** matched each pass against its
model's CRT + math (`*FA`/`*FP`) + `LIBH` + `EM` + `87`. Library functions **identified** (named):
CL 41 · C1 56 · C1L 55 · C2 34 · C3 40. Per-function origin tags in `work/<pass>/origin.json`;
counts in `work/triage_summary.md`. CL's spawn/tempfile hits (`_spawnve`, `__dospawn`, `__cenvarg`,
`_mktemp`) prove the driver chains passes via child processes + temp files. Recall is a floor, not a
ceiling (sweep-leader alignment + compile-flag variance) — improvable with exact-entry alignment.

## 6. Compiler code-idiom signatures (measured)

**Dominant prologue = the near-frame `55 8B EC`** (`push bp; mov bp,sp`), then the local-frame /
saved-reg byte. Measured over authored functions:

| First 4 bytes | Meaning | CL | C2 |
|---|---|---:|---:|
| `55 8B EC 83` | push bp; mov bp,sp; **sub sp,imm8** (locals) | 7 | **200** |
| `55 8B EC 57` | …; push di (reg var) | — | 121 |
| `55 8B EC 56` | …; push si (reg var) | 2 | 115 |
| `55 8B EC 8B` | …; mov … | 3 | 52 |
| `55 8B EC 81` | …; sub sp,imm16 (big frame) | — | 36 |
| `55 8B EC B8` | …; mov ax,imm (e.g. `__chkstk` size) | 56 | 13 |
| `55 8B EC 33` | …; xor … | 33 | — |

**Key Phase-4 finding:** CodeBreaker's `msc6dec.invert` declines **100%** of our functions with
*"prologue miss"* — it models only the **far-loadds** prologue (`1E 58 90 45 55 8BEC 1E 8ED8…`) of
the Win95 medium-model DLLs it was built against. Our passes keep DS fixed (single DGROUP) and use
the **near-frame** prologue above. So Phase 4 needs a near-frame prologue template added to the MSC6
inverse (tractable — near-frame is simpler than far-loadds). This is *the* gating template.

Still to model: epilogues (`8B E5 5D C3` / `…CA nn nn` far ret), `__chkstk`/`__aNchkstk` size-in-AX
call, far-call thunks, switch/jump tables. Seed from `src/engines/msc6/IDIOMS.md`.
