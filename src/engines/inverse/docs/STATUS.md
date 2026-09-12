# Lattice C RE — Current Project Status

*Snapshot as of 2026-08-07. For the project overview see [CONTEXT.md](CONTEXT.md); for the
full bounded task list see [ROADMAP.md](ROADMAP.md).*

---

## TL;DR

Both directions of the Lattice/MC2 **code generator are modeled deterministically and
validated byte-exact against the compiler itself**, with zero search. The engine now
reverses not just the codegen templates but MC2's **optimizer** — register value-tracking,
copy-driven offset swaps, instruction scheduling, and dead-store elimination.

- **Local-variable round-trip: `19/19` byte-exact** (`src/engines/inverse/rtlocals.py`)
- **Real LC2 functions regression: `10/10` byte-exact** (`raiser16.py --batch`)
- **Frame-allocator model: `25/25` byte-exact** (`framemodel.py` self-test)
- **Forward model (quad→bytes): `48/48` byte-exact**

---

## What's proven (byte-exact, oracle-verified)

### Expressions & types
- int / long / float `+ − × ÷ %`, all six relationals, `& | ^ << >>`, unary `− ! ~`
- globals, pointers, single struct-field access, assignment
- `long`/`float` via the `CX*` helper ABI, with **CX-symbol resolution** (mul vs div, etc.)

### Control flow
- `if` / `else` / nesting, `while`, `do…while`, function calls (void / args / returned /
  in-expression), empty bodies

### Local variables & the frame (the recent deep work)
- **Stack-slot layout** — MC2's allocator modeled deterministically: `[temps][locals in
  declaration order]`, char-filler for unused-local gaps, framesize rounded even, params at
  `bp+framesize+4`.
- **Register value-tracking** — MC2 keeps values live in registers and reuses them,
  reordering stores; the inverse reverses this (backward alias + forward store-hoist).
  Fixes `c=a+b`, `c=a*b`, `return a+b`, `b=a+a`, …
- **Copy-reversal slot-ordering** — a live copy `dest=src` swaps their frame offsets
  (per-chain endpoint swap); the inverse pre-compensates declaration order so the recompile
  lands each variable back at its own offset.
- **Instruction-scheduling reversal** — MC2 defers a reused constant's *store* while its
  *load* stays at the source position; the inverse ranks each statement by its load index
  and reorders straight-line blocks.
- **Dead-store recovery** — `a=11; b=a; a=99`: MC2 elides the dead `a=11` store but routes
  `b=a` through the register; the inverse detects the unjustified register-routing and
  reconstructs the dead store + copy.

### Reconstruction backend
`raiser16.reconstruct(image, off, size)` drives the inverse to produce C and verifies it
byte-exact via the MC1/MC2 oracle. **No search in the loop.** Out-of-class functions are
cleanly **declined**, never mis-guessed.

---

## Known remaining edges (small, characterized)
These are the residual local-variable edge cases still to close; each is understood:
- **leading-unused-local with no temps** — region-base ambiguity (temp size not visible from
  bytes alone)
- **chained assignment** `a=b=c=0`
- **inc/dec** off-by-one disambiguation in a couple of forms

---

## The bigger remaining work (from the roadmap)

The engine core is done; "fully done" is dominated by two efforts:

1. **Language breadth (§1)** — each item bounded (gather ground truth → template →
   validate): `switch`/`case`, `goto`/labels, `for`, `break`/`continue`; arrays &
   multi-dim, `char`, `double`, unsigned; compound lvalues `a[i]`/`p->a->b`; pre/post
   `++`/`--`, compound assign; `&x`, casts, `sizeof`, `?:`, `&&`/`||`; functions returning
   long/float/struct/pointer.
2. **Whole-module rebuild (§3–§4)** — the data segment (globals + exact DGROUP offsets,
   initialized data, string literals, jump/ctype tables) and whole-module byte-exact
   assembly (link order, OMF structure, startup module → identical `LC1.EXE`/`LC2.EXE`).
   *The data-segment offset layout reuses the same "model the allocator" mechanism already
   built for the frame.* An **asm fallback** (`mzasmrecon.py`) means a byte-exact *rebuild*
   is reachable before the C is 100% — anything not yet C-reconstructed stays as verified
   asm and still links identically.

**Suggested next step:** §1 language breadth (highest incremental coverage per unit effort)
or §3 the data-segment layout (the linchpin for a whole-module rebuild).

---

## Recent milestones (this stretch of work)
1. Modeled MC2's **frame allocator** deterministically (`framemodel.py`, 25/25).
2. Wired it into `mc2inv`/`raiser16` — local functions reconstruct byte-exact.
3. **Register value-tracking** upgrade to the inverse.
4. **Copy-reversal slot-ordering** rule cracked (per-chain endpoint swap) + pre-compensation.
5. **Instruction-scheduling reversal** (load-hoist / store-defer un-scheduling).
6. **Dead-store recovery** — closed the last local-variable round-trip case → **19/19**.

Throughout: **10/10 real LC2 regression held** at every step.

---

## Environment status
The shared `objextract` dependency now lives in the user's **CodeBreaker** workbench
(`C:\Users\mrtit\Documents\GitHub\CodeBreaker`, package import). Harness path blocks updated;
older `WinTranspiler\win1.0` and `XboxTranspiler\tools` locations kept as fallbacks. See the
Environment section of [CONTEXT.md](CONTEXT.md) if imports break.
