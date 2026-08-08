# MSVC 2.0 C2.EXE diagnostic switches — probe results

Goal: find a white-box view of MSVC 2.0's backend (C2.EXE) — ideally an IR/quad or
register-allocation/scheduling trace — so the rung-3 forward/inverse model can learn the
below-C decisions (the esp/ebp base-flip, the two-register reassociation, `pop` scheduling)
without reverse-engineering C2.EXE in a disassembler. This records what the undocumented
`-d1`/`-d2` switches actually expose, established by empirical brute-force against the vendored
`msvc20/VC/BIN` compiler.

## The mechanism

`CL.EXE` forwards debug switches to its passes: **`-d1<x>` → C1.EXE** (front end, "p1"),
**`-d2<x>` → C2.EXE** (back end, "p2"). An unknown one is reported verbatim, e.g.
`fatal error C1007: unrecognized flag '-X' in 'p2'` — which is exactly what makes brute-force
enumeration possible. `-Bt` prints per-pass timings. C2's internal build path leaks in its ICE
text: `l:\b_bld\c2\P2\main.c`.

## What C2 accepts (single-char `-d2` flags)

| flag | effect |
|---|---|
| **`-d2S`** | **dumps C2's full symbolic assembly listing to stdout** — named arg/local offsets (`_a$ = 8`), decorated proc names (`_f@12 PROC`), COMDAT/segment scaffolding. Identical to the `/Fa` `.cod` listing. |
| `-d2G…` | option-group = C2's internal spellings of the codegen knobs (`Gf/Gh/Gy`, `G3/G4/G5`). Control, not dump. `-d2Gs`/`-d2Gt` ICE (need args). |
| `-d2O…` | option-group = internal optimization knobs (`Oa/Og/Op/Os/Ot/Ow/Oy`). Control, not dump. |
| `-d2f`, `-d2A` | accepted, silent (internal mode bits). |
| `-d2G`/`-d2H`/`-d2O`/`-d2W` (bare) | ICE at `main.c:374` (expect a sub-option). |

## The finding

**There is no IR/quad or scheduler-reasoning dump in MSVC 2.0's C2.** The only observability
switch, `-d2S`, emits the *result* (the symbolic asm) — the same instructions capstone already
recovers, plus C2's own symbol names. It shows the esp/ebp base-flip and the two-register
reassociation **symbolically** (e.g. `sub eax, _c$[esp]` then `sub eax, _b$[ebp]`) but does **not
explain why** C2 chose that base or that schedule.

Practical consequences:
- The white-box symbolic listing is **already available in the tool** as `cc32.compile_c(...)["cod"]`
  (the `/Fa` listing). Useful for symbol/offset recovery and as a cleaner oracle output than raw
  bytes; not a new capability to build.
- The dump-flag shortcut **partially paid off**: it gave the symbolic listing and mapped C2's
  switch surface, but it did **not** hand us the scheduler's internal state.
- Therefore the still-declined shapes (esp/ebp base-flip in `a-b-c`/`(a+b)*c`, n≥4 `pop`
  scheduling, two-product `lea`) must be closed either by **inference from more black-box probing**
  or by **targeted RE of C2.EXE's codegen** (the `compute_stack_frame` / scheduler routines) —
  the dump switches won't reveal them.

## Reproduce

```
CL /nologo /c /O1 /Oy- -d2S t.c            # C2 symbolic listing to stdout
CL /nologo /c /O1 -Bt   t.c                # per-pass timings
CL /nologo /c /O1 -d2<x> t.c               # forward <x> to C2 (unknown -> C1007 names it)
```
