# Minecraft Indev → Xbox 360 — Port Roadmap

Status as of 2026-06-18. Companion to `REMAINING_PORT_STUBS.md` (which covers the
design-decision PORT stubs). This file tracks getting the converted C++ to **compile,
link, and run**.

> Note: the game is **Indev**, not Alpha — the folder name is historical. The converter
> (`java2xbox360.py`) is fully deterministic (no LLM/network), so output is reproducible
> and every fix is verifiable by recompiling.

---

## Where it stands

| Metric | Session start | Now |
|---|---|---|
| Pure-logic `.cpp` compiling (XDK `cl.exe`, syntax) | 71 / ~210 (35%) | **132 / ~210 (63%)** |
| Total compiler errors across failing files | (unmeasured) | ~1,150 |

All progress is in the converter itself, so it survives regeneration. The top-level
`MinecraftAlpha.vcxproj` still builds only the XDK spinning-triangle sample — the
converted code is **not yet integrated** into it.

### How to measure / iterate
- **Build harness:** `pwsh -File AlphaCode\syntaxcheck.ps1` — compiles every `output\*.cpp`
  with the real Xbox 360 `cl.exe` flags, writes `_syntaxbuild\summary.txt` (PASS/FAIL +
  error tally + failing files) and `_syntaxbuild\errors.log`. ~2-4 min.
  - INCLUDE order matters: `include\xbox` BEFORE `include\win32` (else `xnamath.h` errors).
- **Regenerate:** `python java2xbox360.py Java output --version indev`.
  - Postfix-only change (a `fix_*` pass): ~4s, applies to all files in place.
  - Generation change (type map, `generate_*`, profile config): delete the affected
    `output/*.{h,cpp}` so the skip-logic regenerates them (~1-2 min); a from-scratch
    regen (delete all) is ~27 min.
- **Always** diff the failing-file set after each change to catch regressions.

---

## Converter hardening already done (~25 passes/fixes)

- Value types: struct `.cpp` kept (no link errors); `ItemStack` reclassified value→reference
  (nullable; `ItemStack[]`→`ItemStack**`, `new ItemStack[n]`→`new ItemStack*[n]()`).
- Real subsystems: `DataInput`/`DataOutput` big-endian stream classes; **HashMap/std::map**
  layer (types, `put`/`get`/`containsKey`/`remove`/`isEmpty`, `values()`/`keySet()` →
  `std::map::iterator` loops); `toStr()` Java string-concat helpers.
- Type-aware `.`→`->` passes (pointer fields/methods, value-locals incl. fixpoint and
  pointer-field RHS, static pointers `Block::s_X.`); `private:`+`protected:`→`public:`;
  auto-`#include` detection.
- Mechanical: `T[] x`→`T* x`; `new Vec3D(..)`→value; `>>>` unsigned shift; `::rand()`;
  `Material/Block/Item.X`→`::s_X` (+ static-pointer `::s_X.`→`->`); raw `std::map` templating;
  `super(new M(),x)` nested-paren fix; iterator blank-line fix; parent-field configs for 9
  class hierarchies; `private:`+`protected:`→`public:`.
- **Static-init blocks**: Java `static { }` (was misparsed as fields → bogus `m_varN`) now
  captured and emitted as `ClassName::initStatics()`. Fixed Item.cpp (25→1 errors).
  TODO (runtime): call `Block::initStatics()`/`Item::initStatics()` at startup, Block first.
- **Reference-type arrays**: `ItemStack`/`Item`/`Block` arrays → `**` (nullable/polymorphic),
  `new T[n]`→`new T*[n]()`, element `[i].`→`[i]->`; pointer-array element locals promoted.
- **World.length** field (`m_length`) disambiguated from Java array `.length`.

---

## Remaining work (prioritized)

### Phase 1 — finish compiling (converter work)
The pass count has plateaued because each remaining file is blocked by **multiple
independent** issues. Broad passes now lower error counts without flipping files; the
remaining wins are per-file and bottom out in a few hard structural problems:

1. **Array + length modeling.** Java arrays carry `.length`; the port uses raw pointers
   with `_count` companion vars that are often referenced-but-undeclared (C2065) or fed
   to APIs as the array (`writeShort(bytes)` instead of `bytes.length`). The faithful fix
   is `std::vector` (or a `{ptr,len}` struct) for tracked arrays. **Blocks the NBT
   serialization files** (`readFully(arr)`, `write(arr)`), inventory, recipes.
2. **Static-init blocks.** `Item`/`Block`/`Recipes*` emit decompiler temporaries as member
   initializers referencing undeclared `varN` (biggest single C2065 cluster; ~25 in
   `Item.cpp`). Needs proper static-field init (definitions in an `initItems()`/`initBlocks()`
   function in the `.cpp`, not constructor member-init lists).
3. **Java String API.** `getBytes`, `substring`, `charAt`, `indexOf`, `startsWith` have no
   `std::string` equivalents — need a small helper layer.
4. **NBTBase value/pointer + abstract-class** consistency (mostly resolved by the `::`
   static-call pointer pass; a few remain).
5. **Per-file long tail.** Misc single-error files (value/pointer, missing includes).

### Phase 2 — link
Compiling ≠ linking. Expect unresolved-externals (missing out-of-line definitions, the
value-struct methods, statics). New error class; handle after Phase 1.

### Phase 3 — PORT stubs (design decisions; see REMAINING_PORT_STUBS.md)
XInput input loop, animated textures (LockRect), display-list→vertex-buffer chunk
rendering, level save/load file I/O.

### Phase 4 — integrate & run
Add the converted sources to `MinecraftAlpha.vcxproj` (Xbox 360), build the `.xex`,
deploy, and bring up gameplay incrementally: clear a frame → render world → player →
input → save/load.

---

## Honest assessment

The broad, mechanical converter-hardening phase is essentially complete (71→127, ~25
passes). Getting from here to a fully compiling port requires the **array-length** and
**static-init** structural work (Phase 1, items 1-2) — these are focused sub-projects, not
regex passes, and resist the line-based approach. After that, linking and the deferred
PORT stubs are the remaining gates before hardware bring-up.
