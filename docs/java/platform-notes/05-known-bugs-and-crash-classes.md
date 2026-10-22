# 05 — Known Bugs and Crash Classes (Old Port → New Build)

## Overview

The OLD monolith (`MinecraftAlpha/MinecraftAlpha/`, ~379 `.cpp`) is a hand-debugged,
direct-D3D9/Xbox-360 port of Minecraft Alpha. It was crash-prone enough that the author
built permanent heap-corruption tripwires into `Common.h` and littered the source with
**145** crash-class guard comments across 60 files. This doc catalogs those hard-won
lessons so the NEW transpiler build avoids re-hitting them.

Two facts frame everything below:

1. **The Xbox 360 console hard-crashes (IP `0x0` / `0xC0000018` / `RtlValidateHeap`
   "Invalid Address") where the JVM would silently tolerate the same code.** Java's
   guarantees — zero-initialized arrays, GC, NPE-as-exception, bounds-checked arrays —
   are exactly what C++ does *not* give you. Most crash classes are a Java guarantee that
   evaporated in translation.
2. **The current source is "clean" only because every bug was already chased down by
   hand.** All retained build logs (`build_alpha48`..`91`) report `0 Warning(s) 0 Error(s)`.
   The bugs live in the *comments*, not in active warnings. The one captured *failure*
   is `build_alpha69.log` (a self-inflicted linker error, see below). The whole point of
   the new converter is to catch these *by construction* instead of by 40+ debug builds.

The codebase's distress vocabulary is **not** the usual `// HACK` / `// BUG` / `// FIXME`
(zero of those exist). It is:

| Marker | Meaning | Count |
|---|---|---|
| `// PORT(...)` | a deliberate Java→C++ semantic divergence | 233 total `// PORT*` lines |
| `// PORT(null guard)` / `PORT(★ ...)` | a defensive fix for a crash class | ~145 |
| `// PORT(★★ ROOT-CAUSE FIX)` | a fix for a bug that broke the whole game | a handful (the worst ones) |
| `// PORT-BLOCKED` | code that could **not** be ported and is stubbed/dead | 14 across 7 files |

---

## Heap corruption (the [HEAPCHK] machinery)

The author wired a permanent heap tripwire into `Common.h` because the 360's real process
heap was being silently corrupted and only blowing up much later.

**The probe** — `Common.h:60-72`:

```cpp
// [HEAPCHK] diagnostic: poke the REAL process heap (the one RtlValidateHeap@40000000 guards)
// with a malloc/free; the 360 heap auto-validates on free, so the LAST "[HEAPCHK] ck: X"
// printed right before an "Invalid Address ... RtlValidateHeap" message names the phase X
// that corrupted it. (_heapchk() checks a DIFFERENT/CRT heap and is blind to this corruption.)
inline void DbgHeapChk(const char* where) {
    static int _n = 0;
    if (_n >= 4000) return;   // PORT(diag): raised so in-world per-frame probes work (was 70)
    ++_n;
    DbgPrintF("[HEAPCHK] ck: %s\n", where);
    void* p = malloc(64); if (p) free(p);
}
```

Key technique insight: `_heapchk()` (CRT) is **blind** to this corruption because the 360's
real allocator is a *different* heap that auto-validates on `free`. The probe forces a
`malloc`/`free` against the real heap so the **last `[HEAPCHK] ck:` line before the
`RtlValidateHeap` crash names the phase that did the damage.** `DbgPrintF` (`Common.h:49`)
is the channel that survives to the Nexia360 log via the raw `DbgPrint` import.

**Phases that were bracketed** (where the probes were planted):

- **Per-frame render** — `Minecraft.cpp:469/471`: `DbgHeapChk("pre-func_4136_b (render)")`
  / `"post-func_4136_b (render)"`. The render path was a prime suspect.
- **World generation** — `ChunkProviderGenerate.cpp:16-18`: the gen probes were eventually
  turned into a **no-op** with the verdict: *"[HEAPCHK] gen probes confirmed the real heap is
  CLEAN through generation -> no-op now ... the corruption is in the post-gen player/render
  setup."* So generation was **cleared**; corruption was localized to post-gen player/render
  init.
- Dense `[GEN]/[POP]/[R]/[RI]` trace probes (`ChunkProviderGenerate.cpp`, `EntityRenderer.cpp`,
  `ItemRenderer.cpp`) bisected the gen/populate/render pipeline to find the crashing step.

**Suspected root causes of heap corruption** (confirmed in the guard comments):

1. **OOB writes past fixed arrays.** `Chunk.cpp:368-372`: `m_blocks` is a 32768-byte array
   indexed `x<<11 | z<<7 | y`, valid only for `y ∈ [0,128)`. *"decoration/gen can ask for
   y>=128 (e.g. a tree near the height limit) -> OOB read/write past the 32768-byte array ->
   heap corruption (RtlValidateHeap). Guard y."* Fixed by `if (var2 < 0 || var2 >= 128) return`
   in `getBlockID` / `setBlockIDWithMetadata` (`Chunk.cpp:371,376,418`).
2. **Rule-of-Three violations / value-copies of owning types.** `AxisAlignedBB.cpp:10-12`
   notes AABB is a value type that Java *pooled by reference*; the pool stores heap copies.
   Copying an owning object double-frees on the second destructor → the intermittent
   `0xC0000018` class.
3. **The self-inflicted linker error** — `build_alpha69.log:389`:
   `ChunkProviderGenerate.obj : error LNK2001: unresolved external symbol RtlValidateHeap` →
   `fatal error LNK1120`. An attempt to call `RtlValidateHeap` *directly* (rather than via the
   `malloc`/`free` probe) failed to link on the 360 toolchain. Lesson: the indirect
   `DbgHeapChk` poke is the only portable way to exercise that validator.

---

## Recurring distress markers (themes, with examples)

The 145 crash-class guards cluster into a handful of recurring patterns. Highest-density
files: `World.cpp` (23), `ItemStack.cpp` (11), `Chunk.cpp` (8), `Block.cpp`, `ChunkCache.cpp`,
`EntityRenderer.cpp` (52 `PORT*` lines, mostly render-matrix notes).

### Theme 1 — `blocksList[id] == NULL` for unregistered block IDs (the dominant null-deref)
The single most repeated guard. `Block** blocksList = new Block*[256]()` has live entries only
for *registered* IDs; every other slot is `NULL`. Java would NPE-and-recover; C++ derefs a null
vtable → IP `0x0`. Guards appear in at least a dozen files:
- `BlockDoor.cpp:172,193`, `BlockFlowing.cpp:145,250`, `BlockSand.cpp:66`, `BlockSnow.cpp:47`,
  `BlockStationary.cpp:63`, `BlockTNT.cpp:31`, `BlockPumpkin.cpp:55`, `BlockRedstoneWire.cpp:324`,
  `Block.cpp:583`, `Chunk.cpp:386,406,428,445`.
- Representative: `BlockDoor.cpp:172` *"PORT(null guard): blocksList[var5] is NULL for
  unregistered neighbor block ids."*

### Theme 2 — uninitialized arrays (Java zero-fills, C++ does not) — **root-cause class**
`Block.cpp:88-99` is the canonical `★★ ROOT-CAUSE FIX`:
> *"Java zero-initializes arrays; C++ `new T[N]` does NOT. These per-block-id lookup tables
> were full of GARBAGE for UNREGISTERED ids ... Garbage `s_lightOpacity[0]` made air count as
> 'opaque', so the height-map scan stopped at the top: m_heightMap stuck at 128 -> skylight
> propagation exploded (164s world gen) AND the player spawned 'buried' -> suffocation 1
> dmg/tick ('hurt every frame'). Value-init with () so air/unregistered ids are 0/false."*

Fix: `new bool[256]()` / `new int[256]()` / `new unsigned char[256]()` everywhere
(`Block.cpp:88,95-100`, `Chunk.cpp:100,138`). This is the exact `new bool[10]` "phantom-input"
class the new converter's `rule_uninit_array` targets.

### Theme 3 — constructor delegation dropped (`this(...)`) → registration skipped — **root-cause**
`Block.cpp:267` `★★ ctor-delegation fix — THE empty-world bug`:
> *"Java was `this(var1, var3)` then `blockIndexInTexture = var2`. The converter dropped the
> `this(...)` delegation, so this ctor only set blockIndexInTexture and left blockID=0,
> m_blockMaterial=NULL, and the block UNREGISTERED ... Every block ended up with blockID 0.
> generateTerrain then wrote s_stone->blockID (==0 = AIR) for the whole column -> chunks
> generated completely empty."*

Same class in `World.cpp:134` (`★ constructor delegation -- THE world-gen crash`) and
`World.cpp:371`, and `BlockContainer.cpp:25` (the 3-arg ctor forgot to set `s_isBlockContainer`).
C++03 has no delegating constructors, so the real ctor body must be *replicated*.

### Theme 4 — static-init-order fiasco across translation units
`Block.cpp:101-106`: Material statics and block defs live in different TUs with undefined
cross-TU CRT init order; a block could capture a `NULL` Material* → null vtable call in
world-gen (IP `0x0`). Fixed with a forced `_ForceMaterialInit` object declared before `s_stone`.

### Theme 5 — virtual-call-in-constructor
`Block.cpp:663` `★ virtual-in-ctor fix`: the Block ctor records `s_opaqueCubeLookup` /
`s_lightOpacity` from `isOpaqueCube()` — a virtual that in C++ dispatches to the *base*, not
the subclass, during construction (unlike Java). Had to be reworked.

### Theme 6 — `PORT-BLOCKED` (un-portable code, stubbed out) — latent behavior gaps
14 sites, concentrated in `EntityRenderer.cpp` (7), `GuiTexturePacks.cpp` (2). e.g.
`EntityRenderer.cpp:229-231` comments out F1-key hide-HUD logic (`// PORT-BLOCKED: var12 +=
180.0F;`). These are not crashes but **silent feature holes** — input (`Keyboard.isKeyDown`),
texture-pack enumeration, reflection. `CodecMus.cpp:5` flags entire dead audio classes.

### Theme 7 — no RTTI / reflection
`instanceof`, `Class.newInstance()`, `isAssignableFrom` had no equivalent and were
hand-replaced: `ChunkCoordinates.cpp:27`, `ChunkPosition.cpp:30`, `BlockSign.cpp:90`
(reflection→explicit `new`), `BlockPressurePlate.cpp:98`, `Chunk.cpp:681`.

---

## Build-log warnings worth heeding

All retained logs build clean (`0 Warning(s) 0 Error(s)`, confirmed `build_alpha48`..`91`),
so there is no live warning corpus to mine — the warnings were fixed as they appeared. What
the logs *do* teach:

- **`build_alpha69.log:389-397`** — `LNK2001 unresolved external symbol RtlValidateHeap` →
  `LNK1120`. Calling the NT heap validator directly does not link on the 360 SDK; use the
  `malloc`/`free` probe instead. **The only captured build failure.** (Builds 60, 63, 76 are
  *absent* from the log set entirely — likely failed builds that were deleted.)
- The toolchain runs **FxCop `RunCodeAnalysis`** on every build (`build_alpha91.log` tail:
  *"Code Analysis Complete -- 0 error(s), 0 warning(s)"*). Keep that gate green in the new build.
- Build is **`Release_LTCG`** (link-time code gen) — whole-program optimization can reorder/
  inline aggressively, which interacts badly with UB (uninit reads, aliasing). Categories to
  treat as errors in the new build, because LTCG will punish them: **C4700/C4701** (uninitialized
  local use), **C4244/C4267** (truncation, `size_t`→`int`), **C4018/C4389** (signed/unsigned
  compare), **C4311/C4312** (pointer⇄int truncation — critical, see endianness), **C4146**
  (unsigned negation). None are *present* now; the lesson is to compile `/W4 /WX` from day one
  so they never accumulate the way they did across 40 manual builds.

---

## Endianness (big-endian PowerPC) hazards

The Xbox 360 CPU (Xenon) is **big-endian PowerPC**. The old port handled this *correctly* in
the I/O layer, and the new build must preserve that discipline.

**What was done right — manual byte assembly (endian-agnostic):**
`DataInput.h` / `DataOutput.h` implement `java.io.DataInput/DataOutput` by reading/writing
**one byte at a time, MSB first**, never by casting a struct over the buffer:
- `DataOutput.h:16`: `writeInt(int v) { writeByte(v>>24); writeByte(v>>16); writeByte(v>>8); writeByte(v); }`
- `DataInput.h:20`: `readShort() { int h=readUnsignedByte(), l=readUnsignedByte(); return (short)((h<<8)|l); }`
- `DataInput.h:22`: `readInt()` assembles 4 bytes big-endian.

Because it shifts bytes explicitly, this produces the **same big-endian wire format on any
host**, matching Java NBT. NBT tags route all their I/O through these (`NBTTagInt.cpp:27/31`,
`NBTTagShort.cpp:31`, `NBTTagList.cpp:29/39`, `NBTTagByteArray.cpp:28/33`). **Do NOT** "optimize"
these to `memcpy`/`reinterpret_cast` over the buffer — that would byte-swap on the 360 and
corrupt every save file.

**The one bit-pattern trap — float/double over the wire:**
`DataOutput.h:18` `writeFloat` does `memcpy(&i, &v, 4); writeInt(i);` and `DataInput.h:31`
reverses it. The `memcpy` grabs the float's host bit pattern (fine — IEEE-754 layout is the
same), and `writeInt` then emits those bits big-endian. Correct, but fragile: anyone touching
this must keep the `writeInt`/`readInt` wrapper, not raw `memcpy` of the float to the stream.

**Color packing — ARGB, consistent:**
Colors are packed `0xAARRGGBB` via `D3DCOLOR_ARGB(a,r,g,b)` everywhere
(`Tesselator.cpp:95,105,109-111`, `FontRenderer.cpp:62,120`) and textures forced to
`D3DFMT_A8R8G8B8` (`Textures.cpp:47`). Java MC also uses `0xAARRGGBB` ints, so
`colorI(int rgb)` (`Tesselator.cpp:111`) unpacks `(rgb>>16)&0xFF` etc. correctly. **Hazard for
the new build:** the GPU's in-memory pixel byte order vs the `D3DCOLOR` DWORD is a place where a
naive `unsigned char[4]` cast would swap R and B on big-endian. Keep using `D3DCOLOR_ARGB`
macros, never hand-rolled byte arrays for color.

**Shader microcode** is stored "in native DWORD byte order" (`shaders/*.h:51-77`) — already
target-correct; regenerate, don't byte-flip.

---

## Crash classes: old port vs new converter guards

| Crash class | Old port hit it? | `codebreaker/java/rules.py` guards it? | Gap? |
|---|---|---|---|
| **Uninit primitive/pointer array** (`new T[n]` no `()`) | ✅ root-cause (`Block.cpp:88-99`, empty world + suffocation) | ✅ `rule_uninit_array` (HIGH for `bool`, ptr arrays) | Covered |
| **Empty/delegation-only ctor → missing registration** | ✅ root-cause (`Block.cpp:267`, `World.cpp:134`, blockID=0) | ✅ `rule_ctor_stub` (names blockID=0 / s_isBlockContainer) | Covered |
| **Rule of Three** (owns heap, no dtor/copy ctrl) → double-free | ✅ (`AxisAlignedBB.cpp:10`, intermittent `0xC0000018`) | ✅ `rule_rule_of_three` | Covered |
| **Heavy/owning type by value** (Tesselator/NBT copy) | ✅ (Tesselator MB buffer) | ✅ `rule_value_copy_owner` | Covered |
| **Null deref** (`blocksList[id]==NULL`, tile entity NULL) | ✅ **dominant** (~12+ files) | ➖ in `converter_lint`, not `rules.py` (by design, to avoid double-report) | Covered elsewhere — verify lint still active |
| **OOB array index** (`m_blocks[y]`, y≥128 → heap corruption) | ✅ (`Chunk.cpp:368`, `RtlValidateHeap`) | ❌ **NOT guarded** | **GAP** |
| **Static-init-order across TUs** (NULL Material* captured) | ✅ (`Block.cpp:101`, null vtable in gen) | ❌ **NOT guarded** | **GAP** |
| **Virtual call in constructor** (base dispatch, not subclass) | ✅ (`Block.cpp:663`, wrong opacity recorded) | ❌ **NOT guarded** | **GAP** |
| **Dropped `this(...)` delegation specifically** | ✅ (same as ctor-stub but distinct mechanism) | ⚠️ partial — `rule_ctor_stub` catches *empty* bodies, but a ctor that does *some* work yet still drops delegation may slip | Partial |
| **Endianness / raw-cast over byte buffer** | ⚠️ avoided by hand (`DataInput/Output.h`) | ❌ **NOT guarded** | **GAP** (no rule flags `reinterpret_cast`/`memcpy` over I/O buffers) |
| **`PORT-BLOCKED` un-portable feature holes** | ✅ 14 sites (input, texture packs, reflection) | ❌ not a crash, not modeled | **GAP** (silent behavior loss) |

### Gaps the new converter does NOT yet guard (priority order)
1. **OOB fixed-array indexing** — the `Chunk.cpp` y≥128 class caused actual *heap corruption*,
   the hardest bug to diagnose. No rule flags indexing a known-fixed-size array (`m_blocks[...]`,
   `[256]` lookup tables) without a bounds check. **Highest-value new rule.**
2. **Static-init-order across translation units** — a converter that emits cross-TU static
   `new`/`Material*` dependencies should flag them or force an init-order shim like
   `_ForceMaterialInit`.
3. **Virtual call in constructor** — detect virtual method calls inside a ctor body; C++ won't
   dispatch to the subclass.
4. **Endianness / raw struct-over-buffer** — flag `reinterpret_cast`/`memcpy` that reads
   multi-byte scalars directly out of a serialized buffer (must go byte-by-byte).

---

## Lessons for the new build (platform layer)

**DO:**
- **Value-initialize every array.** `new T[N]()` always; never bare `new T[N]`. Treat
  `s_lightOpacity[0]`-style "garbage made air opaque" as the canonical disaster.
- **Bounds-check every fixed-array access on the hot path** (`m_blocks`, the `[256]` lookup
  tables, `m_heightMap`). The 360 turns one OOB write into delayed heap corruption.
- **Keep the `DbgHeapChk`/`[HEAPCHK]` tripwire** (and `DbgPrintF` → `DbgPrint`) in the new
  runtime. The "last `ck:` line before `RtlValidateHeap`" bisection method is proven.
- **Force cross-TU static init order** with an explicit shim object (the `_ForceMaterialInit`
  pattern) wherever one TU's static captures a pointer to another TU's static.
- **Do all binary I/O byte-by-byte, MSB-first** (`DataInput/DataOutput`), so save format is
  big-endian and host-independent. Float/double go through `writeInt(bit pattern)`.
- **Pack colors only via `D3DCOLOR_ARGB` / `A8R8G8B8`** — never hand-rolled `unsigned char[4]`.
- **Compile `/W4 /WX` and keep FxCop green from build #1** so warnings can't accumulate across
  dozens of manual builds the way they did here.
- **Replicate full constructor bodies** when collapsing Java `this(...)` delegation (C++03 has
  no delegating ctors) — and re-verify the object actually *registers* itself.

**DON'T:**
- Don't call `_heapchk()` and trust it — it checks the wrong heap on the 360.
- Don't call `RtlValidateHeap` directly — it won't link (`build_alpha69`); use the probe.
- Don't `memcpy`/`reinterpret_cast` a struct or scalar over a serialized byte buffer — swaps on BE.
- Don't call virtual methods from constructors and expect subclass dispatch.
- Don't pass/return owning types (`Tesselator`, `NBTTag*`, pooled `AxisAlignedBB`) by value.
- Don't leave `PORT-BLOCKED` features as silent stubs without tracking them — they're behavior
  regressions, just not crashes.

---

## Open questions

1. **Where exactly is the post-gen heap corruption?** `ChunkProviderGenerate.cpp:16` cleared
   *generation* but pinned it to "post-gen player/render setup." Was that ever fully root-caused,
   or only mitigated by the `Chunk.cpp` y-bounds guard? If the latter, the same class could
   recur in any other fixed-array accessor.
2. **Are builds 60/63/76 missing because they failed?** Only `build_alpha69` is a captured
   failure; the gaps in the numbering suggest deleted failed builds whose errors we can't see.
3. **Does `converter_lint` still run the null-deref / label-goto rules?** `rules.py:14-21` says
   they live there "to avoid double-reporting" — the new build relies on that being wired into
   the pipeline. Confirm it executes in the same pass as `scan_rules`.
4. **Should the converter model `PORT-BLOCKED` as a first-class report category?** 14 silent
   feature holes (input, texture packs, reflection, audio) shipped in the old port. A
   "could-not-port" ledger entry would surface them instead of burying them in comments.
5. **AABB pooling:** the old port keeps a manual `s_boundingBoxes` pool of `AxisAlignedBB*`.
   Is the new build keeping the pool (and its Rule-of-Three exposure) or moving to value
   semantics with no pool?
