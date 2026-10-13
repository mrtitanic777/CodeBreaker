# Windows 95 Source Beautification — Plan & Methodology

**Goal:** take the transliterated, *compiling-and-running* Windows 95 corpus and progressively
turn it into source that reads like the original — real names, real types/structs, structured
control flow, C where it was C, C++ where it was C++, ASM where Microsoft wrote ASM — **without
ever breaking the "still builds, still boots" invariant.**

This is a layered, verifiable, incremental effort. Every change is gated by the differential
oracle (must stay bit-identical to Microsoft's shipped machine code) so beautification can never
silently diverge from real Windows 95 behavior.

---

## 1. Where we are (the starting material)

Three tiers of "control" over the corpus, from widest to narrowest:

| Tier | Meaning | Scope |
|------|---------|-------|
| **Compiles** | transliterates to buildable C+ASM, 0 cc-fails | **1,191 modules / 224,773 functions** (633 PE + 335 NE-16bit + 223 VxD/LE) |
| **Boots & runs** | executes live in Windows 95 (VMware), edit→rebuild→boot | **~254 of 261** install system DLLs |
| **Clean readable C today** | promoted to real C, oracle-verified bit-identical | `<<PROMOTE_COUNT>>` functions *(filled by the corpus promote pass)* |

What the *un-beautified* generated C looks like today (correct but machine-shaped):
- Functions named `sub_<hex>`; only exports carry real names.
- CPU registers modeled as module-global variables (`eax`, `esi`, …); flags as `__cf`/`__zf` temps.
- Memory as raw casts: `*(u32*)(_imgbase + 0x1234)`, `*(u8*)(ebp + 8)`.
- Types are almost all `u32`; no structs, no meaningful locals.
- Control flow: recovered `if` where provable, `goto L_xxxx` elsewhere; cross-function tails stay ASM.

**Nothing below is allowed to change observable behavior.** The lift is faithful; beautification is
a *rename/retype/restructure* pass over faithful code, continuously re-verified.

---

## 2. What "beautiful" means (the target)

Ordered by increasing difficulty and decreasing automatability:

1. **Language fidelity** — each function in its ORIGINAL language: clean C for C, editable ASM for
   the parts Microsoft wrote in ASM (shared tails, jump-into-middle, hand-optimized routines), C++
   for the COM/OLE object code. *This is the milestone that matters for a real, modifiable OS.*
2. **Real symbol names** — `sub_1cef` → `SHELL32_ILGetDisplayName`, exports already named.
3. **Types & structs** — `*(u32*)(p+0xc)` → `pidl->mkid.cb`; recover the data layouts.
4. **Structured control flow** — `goto` → `if`/`while`/`for`/`switch`.
5. **Idiomatic locals & style** — named locals instead of register-globals; Hungarian notation to
   match the era; comments; the code *reads* like a Microsoft engineer wrote it.
6. **C++ class reconstruction** — vtable-dispatch C → `class CShellFolder : public IShellFolder`.

---

## 3. Automatability — honest tiers of effort

**Automatable now (tooling + oracle time — weeks/months):**
- Promote every oracle-provable function to clean C (`promote.py` + `difftest`/`memdifftest`/`ne_oracle`).
- Apply export & import names (already recovered from the export/import tables and VxD symbol maps).
- Symbolize all absolute addresses (done, 99.9%).
- Recover the tractable branch conditions (`recover_flags`).

**Semi-automatable (months, tool-assisted, needs judgment):**
- Struct/type inference from access patterns (offsets, sizes, cross-refs).
- Register-global → real-local data-flow reconstruction.
- Deeper control-flow structuring.

**Largely manual (the multi-year, human part):**
- Real *internal* function names — Win95 shipped no symbols; these are reconstructed by understanding
  behavior, not recovered.
- C++/COM class reconstruction.
- Idiomatic style + comments.

> Reality check: reaching Tier-1 (language-faithful, editable, still boots) is *weeks of grinding the
> existing pipeline*. Reaching "reads like the leaked NT source" is a from-scratch-scale clean-room
> decompilation — years, mostly human, with semantic names as educated reconstructions.

---

## 4. Tooling (all in `src/engines/recompile/tools/`)

| Tool | Role in beautification |
|------|------------------------|
| `hybrid_emit.py` | ASM → per-function compilable C+ASM (the raw material) |
| `symbolize.py` | absolute addresses → `_imgbase + rva` (relocatable, editable) |
| `recover_flags.py` | `if(1/*not recovered*/)` → real comparisons from the preceding cmp/test |
| `difftest.py` / `memdifftest.py` / `ne_oracle.py` | **the oracle** — prove a function bit-identical to real Win95 before/after any change |
| `promote.py` | replace an oracle-proven function's naked ASM with its actual clean C |
| `certify_fn.py` | which functions/exports are defect-free (safe to promote & swap) |
| `package_ff.py` | assemble a module into a drop-in DLL (resources via `embed_rsrc.py`) |

**The invariant:** *no beautification edit ships unless the oracle still says bit-identical.* This is
what separates this from a lossy decompiler dump.

---

## 5. Per-module beautification workflow

For a target module (e.g. `shell32.dll`):

1. **Inventory** — `certify_fn.py <mod> --list`: how many exports/functions are clean vs defective.
2. **Promote** — run the oracle over its functions; `promote.py` converts every proven one to clean C.
3. **Name** — apply export names; then name internal functions by behavior (manual, prioritized by
   call frequency / centrality).
4. **Retype** — infer structs from access patterns; introduce typed pointers.
5. **Restructure** — `recover_flags` + manual control-flow cleanup.
6. **Verify** — oracle re-run after each step; `package_ff.py` → drop-in → boot in VMware.
7. **Commit** — the module now reads better AND still boots byte-identically.

---

## 6. Priorities & phasing

- **Phase A — corpus-wide promote (automatable):** run `promote.py` across all 1,191 modules; publish
  the clean-C-vs-ASM ratio. *(This is the number that quantifies "how much is already beautiful.")*
- **Phase B — names & types on the DLLs you actually read:** shell32, kernel32, user32, gdi32, ole32 —
  the ones a person opens first.
- **Phase C — struct/COM reconstruction:** the OLE/COM object code, class by class.
- **Phase D — idiomatic pass:** style, comments, the "reads-like-Microsoft" polish.

Each phase keeps the OS booting; you can stop at any phase with a working, more-readable Windows 95.

---

## 7. Metrics to track (fill after each promote pass)

- Functions promoted to clean C: `<<PROMOTE_COUNT>>` / 224,773
- Functions still faithful-ASM (C has no clean form — cross-fn tails etc.): `<<ASM_COUNT>>`
- Functions oracle-verified bit-identical: `<<VERIFIED_COUNT>>`
- Modules booting as our source: **~254 / 261**

*(Counts populated by the corpus-wide `promote.py --manifest` run.)*
