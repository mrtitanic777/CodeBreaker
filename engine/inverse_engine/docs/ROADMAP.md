# Roadmap — what's left until the tool is "fully done"

**Definition of done:** the compiler-inverse engine reconstructs the *entire* `LC1.EXE`
+ `LC2.EXE` (and, by extension, any Lattice/MSC-family binary) into a **complete,
byte-exact, compilable C source tree** — one that recompiles to the identical binaries.

Status legend: ✅ done · 🟡 partial · ⬜ todo. Effort: S (hours) · M (a day) · L (multi-day).

---

## 1. Inverse-engine language/codegen coverage  (the biggest bucket)

This is per-construct: gather ground truth → add template → validate round-trip. Each
item is bounded; there are just many of them.

**Expressions**
- ✅ int/long/float `+ − × ÷ %`, all 6 relationals, `& | ^ << >>`, unary `- ! ~`
- 🟡 operator recovery *exactness* — `mul` vs `div`, float `add` vs `mul` are byte-identical
  modulo the masked `CX*` call; needs **CX-symbol resolution** (S)
- ✅ **register value-tracking** — MC2 keeps values live in regs & reuses them, reordering
  stores; `mc2inv` now reverses this (backward alias + forward store-hoist), fixing
  `c=a+b`/`c=a*b`/`return a+b`/`b=a+a`. Round-trip 11/12; 10/10 LC2 regression clean.
- ⬜ multi-op temp chains `a+b+c`, precedence/associativity reconstruction (M)
- ⬜ compound lvalues: `a[i]`, `p[i]`, `*(p+i)`, `s.a.b`, `p->a->b` (M)
- ⬜ pre/post `++`/`--` in expressions, compound assign `+= -= *= …` (M)
- ⬜ `&x` address-of, casts, `sizeof`, comma operator, ternary `?:` (M)
- ⬜ short-circuit `&&` / `||` (branch structure, we saw the shape) (M)
- ⬜ constant expressions / folded constants back to source form (S)

**Statements & control flow**
- ✅ `if`, `while`, function-call statements, empty body, assignment
- ⬜ `else` / `else if` (S) ← *doing next*
- ⬜ nested / compound blocks, multiple statements with locals (M)
- ⬜ `for`, `do…while`, `switch`/`case`/`default`, `break`, `continue`, `goto`/labels (L)
- ✅ local variable declarations + **stack-slot layout** — `framemodel.py` models MC2's
  allocator deterministically (25/25 battery byte-exact): `[temps][locals-decl-order]` from
  bp+0, char-filler for unused-local gaps, framesize rounded even, params at `bp+fs+4`.
  Wired into `mc2inv`/`raiser16`; round-trip byte-exact incl. unused-local gaps + temp-shift.
  Remaining local edges (separate): leading-unused-local (no-temp base ambiguity),
  chained assign `a=b=c=0`.
- ✅ **copy-reversal slot-ordering** — MC2 swaps frame offsets for live copies (`dest=src`,
  src not overwritten after); per-chain ENDPOINT swap (forward model 7/7 incl. chains).
  `framemodel.infer_decl_order` pre-compensates declaration order; round-trip byte-exact
  for all single/chain/independent copy cases.
- ✅ **instruction-scheduling reversal** — MC2 defers a reused const's store while its load
  stays at the source position; mc2inv now ranks each statement by its load index (reused
  consts) vs store index and reorders straight-line blocks (`recover`).
- ✅ **dead-store recovery** — `a=11;b=a;a=99`: MC2 elides the dead `a=11` store but routes
  `b=a` through the register; mc2inv detects the unjustified register-routing and
  reconstructs the dead store + copy (`_dead_store_recover`, gated on a unique host).
  Local-variable round-trip now **19/19 byte-exact**; 10/10 LC2 regression clean.

**Types**
- ✅ int, long, float (params); pointers, single struct-field access, globals
- 🟡 struct pointers (`p->fN` recovered; field *types* synthetic)
- ⬜ unsigned int/char/long (idiv→div, sar→shr, movzx signals), `char`, `double` (M)
- ⬜ arrays (1-D, multi-D), struct-by-value, nested structs, unions, `char*`/strings (L)
- ⬜ pointer-to-pointer, function pointers, `register`/`static` locals (M)
- ⬜ int↔long↔float↔double conversions (the `CXV*` helper family) (M)

**Functions**
- 🟡 K&R signature + return-type inference (return type currently forced `int`)
- ⬜ functions returning long/float/double/struct/pointer (return-value conventions) (M)
- ⬜ multiple return paths, varargs (M)

---

## 2. Symbol / naming recovery  🟡

Byte-exactness doesn't need real names, but a usable source tree does.
- ✅ synthetic names: `g_<addr>` globals, `sub_<addr>` callees, `fN` fields
- ⬜ resolve `CX*` helper calls to real names via the library map (S) ← *doing next*
- ⬜ resolve calls to sibling compiler functions (intra-module target → `sub_<off>`) (S)
- ⬜ recover meaningful names — impossible from the stripped binary; best-effort from
  the shipped diagnostic strings / manual annotation (L, optional)

---

## 3. Data-segment reconstruction  ⬜ (L)

Currently we reconstruct **code only**. A byte-exact rebuild needs the DATA segment too:
- ⬜ enumerate globals + their exact DGROUP offsets (drives the code fixups)
- ⬜ initialized data, string literals, `switch` jump tables, the ctype/`_iob` tables
- ⬜ emit as C declarations with initializers that link to the same layout

---

## 4. Whole-module byte-exact assembly  ⬜ (L)

Turning per-function C into the identical `.EXE`:
- ⬜ reconstruct **link order** (functions + data in original sequence)
- ⬜ match the full OMF object structure (segments/groups/pubdefs/extdefs/fixups)
- ⬜ the startup module (`C.ASM`) + exact `LINK` invocation → identical `LC1.EXE`/`LC2.EXE`
- ✅ fallback path exists: `mzasmrecon` gives byte-exact **asm** for anything the C
  path can't yet reach (guarantees convergence)

---

## 5. Scale & robustness  🟡

- 🟡 run over all ~340 compiler-original functions (LC1 ~135 + LC2 ~165); 129 are library
- ⬜ the large functions (parsers/codegen, 400–2000 B) with deep nested control flow (L)
- ⬜ embedded data / jump tables inside code segments (M)
- ⬜ forward model (`mc2model`) symmetry: handle call quads + long/float returns (S)
- ✅ principled decline: out-of-class functions are flagged, never mis-guessed

---

## 6. Integration & polish  🟡

- ✅ `mc2inv` wired as the `raiser16` byte-exact-C backend, MC-oracle verified
- ⬜ formal WinTranspiler `latticec` target registration (`wt --target latticec …`) (S)
- ⬜ automated batch runner + coverage ledger (which fns byte-exact vs asm-fallback) (S)
- ✅ design writeup (`re/PROJECT_SUMMARY.md`), this roadmap

---

## Honest bottom line

- **The engine core is proven** — both directions of the code generator, validated
  byte-exact against the compiler, across a real (if still narrow) language class.
- **"Fully done" is dominated by two efforts:** (a) *breadth* — grinding the ~40 language
  features in §1 (each bounded, S–M), and (b) *the whole-module rebuild* — §3 data segment
  + §4 assembly (each L), which are what actually produce identical `LC1.EXE`/`LC2.EXE`.
- **Realistic sizing:** the language-coverage crank is weeks; data-segment + whole-module
  assembly is the harder, more novel chunk. The asm-fallback (§4) means a byte-exact
  *rebuild* is reachable before the C is 100% — every function not yet C-reconstructed
  stays as verified asm, and the whole thing still links identically.

**Immediate next (this session):** `else`/nesting (§1) + `CX*`-symbol resolution (§2).
