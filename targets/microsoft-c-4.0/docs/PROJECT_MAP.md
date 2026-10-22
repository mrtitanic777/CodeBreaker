# Reverse Engineering Microsoft C 4.0 — End-to-End Project Map

The full route from shipped floppies to readable, verified source, using **CodeBreaker** as the
workbench. Doctrine (inherited from CodeBreaker): **evidence only; byte-exact or flagged, never
faked; triage by origin first; prove every claim by rebuilding against the original.**

- **Corpus & findings:** [../NOTES.md](../NOTES.md)
- **Signature DB:** [../SIGNATURES.md](../SIGNATURES.md)
- **Legal:** these are 1986 Microsoft binaries. We analyze copies the user is entitled to analyze;
  reconstructed output inherits the input's copyright. Tool-only, no MS code committed.

---

## The goal, stated precisely

Recover the **authored source** of the Microsoft C 4.0 toolchain — primarily the three compiler
passes `C1/C2/C3` and the driver `MSC/CL`, then the utilities — as source a person can read, with
each function **triaged by origin** (app code vs CRT vs library vs compiler glue) and, where the
code class allows, **recompilable to the exact original bytes** using C 4.0 itself as the oracle.

**Definition of done, per binary:** (1) unpacked & disassembled 100% (byte-exact ASM floor);
(2) every function origin-classified; (3) CRT/library functions *identified & named* from the
shipped source/.LIB; (4) app functions lifted to readable C at a labelled confidence tier;
(5) a byte-exact rebuild demonstrated for the tier that supports it.

---

## Target routing (what each input becomes)

```
                 ┌─ disk06 CRT .ASM/.INC ─┐
                 │  disk03 headers .H     │──▶  GROUND TRUTH  (answer key, not a target)
                 │  disk07 .C↔.EXE pairs  │      → calibrates the C-4.0 codegen model
                 └────────────────────────┘        → verifies libsig / CRT sigs

  MZ (EXEPACK)   MSC C1 C2 C3 C3L CL      ─▶  Phase 1 unpack ─▶ Phase 2 disasm ─▶
  CV LINK LIB EXEMOD EXEPACK SETENV DOSSEG      Phase 3 triage ─▶ Phase 4 lift/recon ─▶ Phase 5 verify

  MZ (plain)     MAKE.EXE  LINK4.EXE      ─▶  skip Phase 1; warm-up disasm targets

  OMF archives   *.LIB  *.OBJ             ─▶  libsig source (build FLIRT sigs) + link-verify

  Resources      HELLO.* (disk08)         ─▶  16-bit resource/.RC track (secondary)
```

---

## Phase 0 — Environment & corpus  ✅ DONE (session 1)

- [x] Extract all 8 FAT12 images → `extracted/diskNN/` (pure-Python walker).
- [x] Classify the corpus (Tier A compiler / B utilities / C ground-truth). See NOTES §1.
- [x] `git init`, workspace tree, `wt.py doctor` green, DOSBox-X present.
- [x] Identify the packing wall (**EXEPACK**, NOTES §2) and the ground-truth assets.
- **Exit criteria met.** Copy the ground-truth source into `workspace/source_gt/` for easy diffing.

---

## Phase 1 — Unpack (defeat EXEPACK)

Recover the true image (real CS:IP, relocation table, unpacked code+data) for every packed tool.

1. **Write a deterministic Python EXEPACK unpacker** in `workspace/scripts/unexepack.py`
   (parse the packed header, run the copy-up + RLE-ish unpack, rebuild the reloc table, re-emit a
   plain MZ). Deterministic + inspectable = fits the doctrine.
2. **Cross-check** each unpack a second, independent way: load in **DOSBox-X debugger**, break at
   the transfer to OEP, dump the resident image; compare with the Python result.
3. Emit to `workspace/unpacked/<NAME>.exe` + a small JSON manifest (OEP, seg map, reloc count).

- **Verify:** unpacked file loads in DOSBox-X and *runs* (e.g. `C1 -?`), and its entry matches the
  debugger OEP. **Deliverable:** `workspace/unpacked/*` + `SIG-PACK-001` promoted with real offsets.
- **Risk:** nested/edge EXEPACK variants; if the Python path stalls, fall back to `unp`/`unexepack`
  in DOSBox and document the exact tool + version in NOTES.

---

## Phase 2 — Disassembly & carving (byte-exact ASM floor)

Establish the *guaranteed-correct* baseline: every byte accounted for.

1. `python wt.py inverse mzdis workspace/unpacked/<NAME>.exe` → recursive-descent 16-bit disasm
   (Capstone) with code/data classification; segment & far-call aware.
2. `python wt.py inverse mzasmrecon <NAME>` → MASM-able ASM with a `db` fallback that **guarantees
   byte-identity** (CodeBreaker's recon floor). This is the 100%-coverage floor: nothing faked.
3. Build the **call graph** and segment/section map; carve functions; separate CODE vs DATA vs
   the DGROUP.

- **Verify:** reassemble the emitted ASM and byte-compare to the unpacked image (must be 0 diff).
- **Deliverable:** `workspace/disasm/<NAME>/` — listings, call graph, function inventory.
- **Start with MAKE.EXE / LINK4.EXE** (unpacked already) to shake out the 16-bit MZ path before
  the big C1/C2 images.

---

## Phase 3 — Origin triage (identify before reconstructing)

The most important phase for signal-to-noise: separate the compiler authors' code from everything
they linked in.

1. **CRT identification** — match against disk06 source. Assemble the shipped `.ASM` with period
   MASM to get reference bytes, fingerprint, and label matches (`SIG-CRT-*`). Name `__astart`,
   `_chkstk`, `_setargv`, `_setenvp`, FP init, message helpers.
2. **Library identification (`libsig`)** — `python wt.py inverse omf <LIB>` to extract every
   PUBDEF/module from `SLIBC/CLIBC/MLIBC/LLIBC` + float/helper libs; build FLIRT-style byte sigs
   (FIXUPP-masked) and mark hits as *library, named, not reconstructed*. The loose `.OBJ`s are the
   cleanest seeds. (`SIG-LIB-*`)
3. **Compiler glue** — `_chkstk`, long-arithmetic helpers (`__aFldiv` …), FP thunks: identify via
   `crtsig`/`idioms`, mark toolchain.
4. **Determine memory model** per binary from the startup/segment fingerprint.

- **Verify:** a named CRT/library function's bytes match the assembled/`.LIB` bytes exactly.
- **Deliverable:** per-binary triage ledger (`app` / `crt` / `lib` / `glue` counts); only `app`
  functions carry forward as reconstruction targets. Populates SIGNATURES §3–4.

---

## Phase 4 — Reconstruction of the compiler's own code

Now lift the *authored* (app-tier) functions of MSC/C1/C2/C3 to readable C. Two complementary tracks.

### 4a. Static, byte-exact (the inverse track)
- **Author a C-4.0 codegen model.** CodeBreaker ships MS-C **2.03** and MSC **6.0a** models;
  4.0 sits between. Calibrate a 4.0 model on **known source↔binary pairs**: the disk07 samples
  (`CIRCLE/COUNT/PI.C` ↔ `.EXE`) and the disk06 CRT (`.ASM` ↔ its `.OBJ` in the libs). Capture
  prologue/epilogue, switch tables, calling conventions, long/FP helper calls (`SIG-CG-*`).
- Reuse `raiser16` / `mzasmrecon` / `idioms`; reverse the optimizer where 4.0's `-O` fires.
- **Best oracle = C 4.0 itself**, run in DOSBox-X: recover C for a function, recompile with the
  shipped `C1/C2/C3`, and byte-compare. This is the same "period toolchain as oracle" method
  CodeBreaker uses for Win95, applied self-referentially.

### 4b. Dynamic, behavioral (the oracle track)
- If CodeBreaker's `behavior` (Unicorn) engine can host **real-mode DOS + INT 21h** (open TODO in
  NOTES §5), drive individual functions as an execution oracle to recover pure input→output
  expressions, struct layouts, and global effects — then differential-test the synthesized C back
  against the binary. Otherwise, scope this track to a thin DOS shim or defer.

- **Tiering:** every function emitted with a confidence tier (byte-exact / partial / flagged).
  A module is only "100%" because the floor is verified ASM — never a fabricated body.
- **Deliverable:** `workspace/recon/<NAME>/` readable C + ASM floor, per-function tier.
- **Sequence the passes so the story is legible:** start with the smallest driver (`MSC.EXE` /
  `CL.EXE`, ~27 KB) → then `C3` (codegen/optimizer) → `C1` (front end) → `C2`. The driver teaches
  us the shared CRT/idiom vocabulary that the big passes reuse.

---

## Phase 5 — Verification & rebuild

The proof floor. Nothing is "recovered" until it rebuilds.

1. **ASM floor:** reassembled disasm == unpacked image, byte for byte (Phase 2 gate, re-run in CI).
2. **Optionally re-pack:** reproduce the EXEPACK wrapper to match the *shipped* file byte-for-byte
   (stretch goal — proves the unpacker + packer models are both exact).
3. **C rebuild:** recovered C compiled by the **shipped C 4.0** (in DOSBox-X) reproduces the target
   function's bytes; track a "% byte-exact" and "% live (real instrs vs `db`)" metric per module.
4. **Regression guard:** a `goldens` set so re-runs can't silently regress.

- **Deliverable:** a coverage/verification report per binary (bytes cited, functions carved vs
  written, % byte-exact, % live), mirroring CodeBreaker's `coverage ledger`.

---

## Phase 6 — Secondary tracks & write-up

- **Utilities:** apply Phases 1–5 to LINK, LIB, CV, EXEMOD, EXEPACK, MAKE, SETENV, DOSSEG.
- **Windows 1.x kit (disk08):** HELLO sample + `?LIBW`/`?WINLIBC` libs, LINK4 → a small NE/16-bit
  resource side-quest (CodeBreaker's `neresources`/`win16*` engines apply here).
- **Cross-version diffing:** `wt diff` C 4.0 functions against later MSC (5/6) or the shipped CRT
  source to carry names forward and study the toolchain's evolution.
- **Narrative:** fold discoveries back into NOTES/SIGNATURES; write a short "how MS C 4.0 is built"
  architecture note (pass boundaries, the C1↔C2↔C3 temp-file protocol, memory-model machinery).

---

## Milestone checklist (execution order)

| # | Milestone | Gate |
|---|---|---|
| M0 | Environment + corpus + classification | ✅ done |
| M1 | EXEPACK unpacker, all Tier-A/B unpacked & cross-verified | unpacked runs in DOSBox; OEP matches |
| M2 | 16-bit MZ disasm path proven on MAKE/LINK4 | reassemble == original, 0 diff |
| M3 | Byte-exact ASM floor for C1/C2/C3/MSC | reassemble == unpacked, 0 diff |
| M4 | Origin triage: CRT + libs identified from source/.LIB | named fns' bytes match ground truth |
| M5 | C-4.0 codegen model calibrated on disk07 pairs | model reproduces sample .EXE fns byte-exact |
| M6 | Readable C for MSC/CL driver (first full binary) | tiers labelled; app fns recompile-verified |
| M7 | C1/C2/C3 reconstruction | per-module coverage report |
| M8 | Utilities + Windows kit + write-up | reports + architecture note |

---

## Immediate next actions (start of session 2)
1. Copy ground-truth source into `workspace/source_gt/`.
2. Write & run `workspace/scripts/unexepack.py`; unpack `MSC.EXE` first, verify in DOSBox-X.
3. Prove `wt inverse mzdis` on the already-unpacked `MAKE.EXE` (M2 shakedown).
4. Harvest version/banner strings from unpacked C1/LINK/CV → SIGNATURES §6.
