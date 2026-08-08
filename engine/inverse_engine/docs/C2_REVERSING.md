# Reverse-engineering C2.EXE (MSVC 2.0 backend) — the map

The rung-3 residual (esp/ebp base-flip, two-register reassociation, `pop`/instruction
scheduling) is not exposed by dump switches (see [C2_DIAGNOSTICS.md](C2_DIAGNOSTICS.md)), so the
only way to model it faithfully is to reverse-engineer C2's codegen. This is the map that makes
that tractable — recovered without a disassembler GUI, from C2's own embedded debug data.

C2.EXE: 517,632 bytes, PE32 i386, image_base 0x400000. `.text` holds the codegen; it is itself
frame-pointer-omitted and optimized, so functions do **not** start with `push ebp; mov ebp,esp`.

## C2's internal source-file inventory (59 files, from embedded ICE paths)

Every `fatal error C1001` embeds `compiler file 'l:\b_bld\c2\<dir>\<file>', line N`. The full set
is C2's module map. The ones that own the rung-3 residual:

| file | role | .text range (from xref clustering) |
|---|---|---|
| **`P2\386\sched.c`** | **instruction scheduler** (ordering, `pop` placement) | ~0x42e686 – 0x4327d9 |
| **`P2\386\schedp5u.c`** | Pentium (P5) U/V-pipe scheduler | ~0x4327d9 – 0x43axxx |
| **`P2\386\regasg.c`** | register assignment | ~0x41f4bc – … |
| **`P2\386\color.c`** | graph-coloring register allocator | ~0x41734d – 0x41a8c7 |
| **`P2\nglobreg.c`** | global register allocation | ~0x459c3e – … |
| **`P2\canon.c`** | canonicalization (operand reorder / reassociation) | ~0x444512 – 0x446105 |
| **`P2\cgcanon.c`** | codegen canonicalization | ~0x461f6a |
| **`P2\trees.c`** | expression trees (the tree evaluator) | ~0x442d9d – … |
| `P2\386\code.c`, `codegen.c` | code generation / emit | ~0x412k / 0x401k (codegen.c biggest, 50 asserts) |
| `P2\386\peeps.c` | peephole | — |
| `P2\emit.c`, `coffemit.c`, `omf.c`, `coff.c` | object emission | — |
| `P2\386\range.c`, `spl.c`, `flow.c`, `alias.c` | live ranges / spill / flow / aliasing | — |
| `Common\il.c`, `initil.c`, `tables.c` | the IL (intermediate language) C1↔C2 speaks | — |

(Full list also: confold.c, optimize.c, inline.c, unroll.c, fltopt.c, fltreg.c, double.c,
p2symtab.c, typer.c, getattr.c, sortnode.c, bitv.c, pril.c, main.c, misc.c, …)

## Anchoring technique — the ICE convention

C2 raises an ICE with **`mov ecx, <file-string VA>; mov edx, <line>; call 0x46ea50`** (a
fastcall-style error handler; the filename goes in ecx, the line in edx — *not* pushed). So:
- scanning `.text` for `mov ecx, <VA of any b_bld path>` recovers **every** assert site with its
  exact `(source-file, line)` — a dense function→file map across all of C2.
- each file's assert sites cluster into that file's contiguous code range (link order), which is
  how the ranges above were derived.

## Scheduler — first read (sched.c, around the 0x42e686 assert = sched.c:378)

The code walks a list of instruction nodes with `esi` = current node; observed field layout:
`[esi+8]` flags (OR'd with 0x800000, 0x80…), `[esi+0x18]` a size/width (masked `&0xfffc`, `>>2`),
`[esi+0x1c]`, `[esi+0x20]` (masked `&3` — a pipe/kind tag). Globals act as accumulators, e.g.
`[0x47b284]` is added/subtracted by `(size&0xfffc)>>2` (a running cycle/slot counter), `[0x47b258]`
/`[0x475de4]` are mode/state flags. This is the scheduler's per-instruction bookkeeping — the
machinery that decides ordering (and, downstream, where the epilogue `pop` lands).

## Status & next steps (honest scope)

This is **phase 1 (the map)** of a genuinely multi-week RE effort — done: C2's module inventory,
source→address ranges, the ICE/function-mapping convention, and a first read into the scheduler's
data structures. **Not** done: the scheduler's actual ordering algorithm, or `regasg.c`/`color.c`'s
esp-vs-ebp base decision — reproducing those byte-exact is the remaining work.

Recommended path from here:
1. Build the full function→(file,line) map from the `mov ecx,<path>` convention (mechanical).
2. Read `regasg.c`/`color.c` for the esp/ebp base-selection rule (the `a-b-c`/`(a+b)*c` flip).
3. Read `sched.c`'s node-ordering loop for the `pop`/final-op scheduling (the n≥4 additive case).
4. Fold each recovered rule back into `msvc2model` and re-validate byte-exact against the oracle.
