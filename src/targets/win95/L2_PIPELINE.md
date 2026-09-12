# Layer-2 reconstruction pipeline (win95, 32-bit PE)

**Goal:** drive every function of every 32-bit PE module to **buildable, byte-exact
source** unattended — clean C / C++ where a function is mechanical, a byte-exact
ASM floor for everything else, so a module is *always* 100% covered and nothing is
ever faked. Runs under `wt --target win95`; each stage is a standalone tool too.

This is the automated scale-up of the hand pilot: `classify → emit → verify →
floor → report`. Where `conduct.py` (Track A) drove a single module to byte-exact
ASM, the L2 pipeline drives readable SOURCE across the whole corpus.

## The oracle discipline

Nothing is claimed that isn't proven. Every reconstructed function is compiled by
a **period MSVC** (2.0 preferred; 4.2/5.0 fallback — `cc32.py`) and the emitted
machine code is compared **mask-aware** against the shipped bytes (`segdiff.py`:
call/branch displacements + base-relocs masked, opcode stream compared). A
byte-exact match lands the function; a mismatch is a *draft* (compiles, calls the
right target, but codegen differs — a human starting point), never auto-landed.

## Stages

| # | Stage | Module | What it does |
|---|---|---|---|
| 1 | classify | `classify32.py` | Route every carved function: `thunk` / `asm` / `c` / `human` (+ a category). One disasm, feature-extracted. |
| 2a | clean C | `emit_c.py` | Model the mechanical C classes (const-return, forwarder, wrapper, leaf, global-load) as `__cdecl/__stdcall/__fastcall` free functions; confirm byte-exact via `cc32`. |
| 2b | clean C++ | `emit_cpp.py` | Model the mechanical `__thiscall` **method** classes (getter/setter/field-address/return-this) `emit_c` can't reach; confirm byte-exact via `cc32` compiled as **C++**. See [WIN95_MAP.md](WIN95_MAP.md). |
| 3 | ASM floor | `ml32.py` + `asmrecon32.py` (via `l2pipe.reconcile_ml`) | Byte-exact ASM for everything not landed as clean C/C++: asmrecon32's emitter + native ML 6.11c, db-ing the first diverging row each pass. Worst case: raw `db` of the shipped bytes — byte-identity by construction. This is the **coverage floor**. |
| 4 | conduct | `l2pipe.py` | Per-module conductor: runs 1→2→3, writes a per-function **ledger** + `.auto.c` / `.auto.cpp` + `.review.c` drafts. A per-module wall-clock **budget** defers the (slow) floor so one huge module can't wedge a sweep. |
| 5 | sweep | `l2sweep.py` | Run `l2pipe` across all 32-bit PE modules of the corpus; aggregate coverage; evict caches between modules to keep memory flat. |
| 6 | report | `l2report.py` | Aggregate every `<mod>.l2ledger.json` into corpus totals, per-category counts, modules ranked by yield. Run any time (mid-sweep or after). |
| 7 | predict | `l2predict.py` | Classify-only triage: tag each module with its architectural tier, measure route mix + emit-reachable fraction, flag modules underperforming their tier. The "where to aim next" list. |

Supporting: `cc32.py` (the MSVC oracle: compile C/C++ → parse COFF → extract
function bytes+mask), `segdiff.py` (mask-aware diff), `pe32util.py` (carve /
disasm / named-funcs / imports helpers, memoized).

## Commands

```bash
# one module, full pipeline (clean C/C++ + byte-exact ASM floor), writing source + ledger
wt --target win95 l2pipe MODULE.dll --emit src/reconstructed/auto

# fast: clean C/C++ only, skip the (slow, emitter-independent) ASM floor -- for measuring
# emitter yield after a modeler change
wt --target win95 l2pipe MODULE.dll --no-floor --emit DIR
wt --target win95 l2pipe MODULE.dll --budget 240    # per-module floor wall-clock cap (s)

# whole corpus
wt --target win95 l2sweep [--no-floor] [--limit N] [--start K] --emit DIR
wt --target win95 l2sweep --list                    # just list the 32-bit PE modules

# reporting + triage
wt --target win95 l2report [emit_dir]               # corpus coverage report
wt --target win95 l2predict [--emit DIR]            # route-mix prediction / triage

# stage tools standalone
wt --target win95 classify32 MODULE.dll             # route/category report
wt --target win95 emit_c    MODULE.dll              # clean-C yield for one module
wt --target win95 emit_cpp  MODULE.dll              # clean-C++ (__thiscall) yield
```

## Output of a run (`--emit DIR`)

- `<mod>.auto.c` — clean C, every function verified byte-exact (internal targets as
  `sub_<rva>`, imports by name), stitched into one clean translation unit.
- `<mod>.auto.cpp` — clean C++ `__thiscall` methods, each verified byte-exact; one
  per-function `struct C_<rva>` with `this`-relative field access.
- `<mod>.review.c` — clean-C **drafts**: compile + call the right target but aren't
  byte-identical. Already covered byte-exact on the floor; this is the readability
  upgrade for a human to confirm.
- `<mod>.l2ledger.json` — per-function record: name, rva, cat, route, kind
  (`clean-c` / `clean-cpp` / `asm` / `asm-pending` / `escalate`), verify status,
  flags/db counts. The audit trail; `l2report` aggregates it.

## Current state (measured)

- **Corpus:** 549 32-bit PE modules, ~24,562 named functions (per `l2predict`).
- **Clean C++ (`emit_cpp`)** reaches the `__thiscall` leaf classes — ~1–2% of MFC
  functions, ~418 methods corpus-wide; the call-bearing bulk is `human` and needs
  full expression reconstruction (out of scope for template emit — the behavior
  engine's territory).
- **Clean C (`emit_c`)** is the volume on the 32-bit C API DLLs; `l2predict`
  quantifies the per-tier yield and the modules that underperform their peers.
- Every non-clean function is taken to a byte-exact ASM floor, so **coverage is
  total by construction** — the open question is always *readability* (how much is
  clean C/C++ vs floored ASM), never *correctness*.

See [WIN95_MAP.md](WIN95_MAP.md) for how a module's tier predicts its route mix,
and why each route lands where it does.
