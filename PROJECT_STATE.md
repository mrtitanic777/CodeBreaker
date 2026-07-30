# PROJECT_STATE.md — consolidated source of truth

> Merged from MERGE1 (in-world port / parallel world-gen chat), MERGE2 (rendering / HUD /
> hand chat), and MERGE3 (converter / networking / XDK chat). **This file supersedes the
> three MERGE\*.md** for going-forward development. Every "✅ VERIFIED this pass" claim was
> checked against the actual repo/live-tree on the date below; everything else is carried
> from the merge files and marked as *reported* (trust, but re-verify before acting).
>
> **Reconciled:** 2026-07-30 · **Repo git HEAD:** `c8ee21d` (all three chats agree; unchanged) ·
> **Reconciler note:** git HEAD is the source of truth for *committed converter* code; the
> live C++ game tree is **not git-tracked**, so its state is verified by reading files directly.

---

## 0. What this project is

An unofficial **Xbox 360 (RGH homebrew)** port of **Minecraft Alpha v1.2.6**, produced by a
bespoke **deterministic Java→C++03 transpiler** (`j2x`, formerly `java2xbox360.py`). Converted
C++ is swapped into a real **VS2010 + Xbox 360 XDK** project, and runs on the **Nexia360
emulator** and **real RGH hardware**. The game boots, generates/loads a singleplayer world,
renders terrain/entities/mobs/HUD, does combat/containers, has controller input and XACT3
audio, and has **working end-to-end SMP** (client speaks **Alpha protocol 6**). Java is ground
truth; the "correct by construction" converter prevents the crash classes rather than patching.

### Key paths

| What | Path |
|------|------|
| Converter repo (git-tracked) | `…\MinecraftAlpha\ConversionTool\` (the `j2x` package) |
| Live Xbox VS2010 project (**NOT git-tracked**) | `…\MinecraftAlpha\MinecraftAlpha\` |
| Durable platform-layer source of truth | `ConversionTool\docs\platform\reference\*.reference.{h,cpp}` |
| Alpha Java input (client, ground truth) | `C:\Users\mrtit\Desktop\MinecraftAlpha\minecraft\src\net\minecraft\src\` |
| Protocol-6 server (the RIGHT one) | `…\Xbox 360\Tools\minecraft_server\src\net\minecraft\` |
| Networking reference (proven 360 code) | `…\Projects\HttpProgram\HttpProgram\` |
| XDK docs (11,906 .htm) + index | `C:\Users\mrtit\Desktop\XDK Documentation\` → `docs/platform/xdk-*.tsv` |
| Emulator log (rewritten each launch) | `C:\Nexia360\nexia.log` |
| Codebase map | `ConversionTool\signatures.md` |

**Build:** VS2010 + Xbox 360 XDK IDE only (bare XDK CLI fails). User drives builds by pasting
`Build.txt`; expect `BUILD EXIT: 0 / ERRORS: 0`. **Nexia crashes on ANY guest C++ throw** even
with `/EHsc` (works on real HW) → neuter failure-path throws on hot/boot paths.
**Crash signature:** IP `0x0` / NTSTATUS `0xC0000018`/`0xC0000005` = call through NULL
vtable/function pointer **or** heap corruption.

---

## 1. Bases — status

> **Update 2026-07-30 (this session):** items 1a, 1b, 1c below are **RESOLVED**; 1d committed.
> - **1a RenderSorter crash → FIXED** by construction: `Collections::sort` now uses a stable,
>   comparator-tolerant merge sort (never `std::sort`) in `j2x/runtime.py` + mirrored to the live
>   tree `Collections.h`. No generated-source edit; stays Java-faithful.
> - **1b overrides model → RESOLVED converter-only:** signed-byte fix is now converter logic
>   (`typemap.SIGNED_BYTE_FIELDS` + `emit/header.py` → `signed char`); README corrected; the old
>   `overrides/` path is formally retired.
> - **1c test drift → FIXED:** 189/189 green (rules `plat` param + Alpha-corpus re-baselines +
>   a signed-byte test).

### 1a. 🔴→✅ RenderSorter invalid `std::sort` comparator — **FIXED** (was CONFIRMED STILL LIVE)
- `MinecraftAlpha\RenderSorter.cpp:74` returns:
  `var5 < var7 ? 1 : (var5 > var7 ? -1 : (var1->chunkIndex < var2->chunkIndex ? 1 : -1))`.
  For equal elements → `chunkIndex < chunkIndex` is false → **returns `-1`**, so `compare(a,a) == -1`.
- The runtime's `Collections::sort` uses **non-stable `std::sort`** with
  `_CmpLess(a,b) = compare(a,b) < 0` (`j2x/runtime.py:1270`). Thus `_CmpLess(a,a) == true` →
  **reflexivity violated → strict-weak-ordering broken → `std::sort` is UB** on the Xbox STL
  (OOB read → heap corruption `0xC0000018` / `IP 0x0`, or hang). Fires when the sorted list is
  **large** — and world entry marks all 625–5000 chunks dirty and sorts the whole dirty queue
  **every frame**. This is the single confirmed deadlock/crash landmine.
- Siblings are FINE: `EntitySorter` returns `1` for equal, `RecipeSorter` returns `0`.
- **Two-part fix (both needed):**
  1. **Source:** rewrite `RenderSorter::a` to return **0 for equal** (keep in-frustum-then-nearest intent).
  2. **Converter rule (whole latent class):** any `Comparator.compare` feeding `std::sort`/
     `Collections::sort` must emit a 0-for-equal case / guarantee `compare(a,a)` is not `< 0`.
     Consider *also* hardening `Collections::sort` itself (e.g. a strict-weak-safe adapter or
     `std::stable_sort` won't fix a non-SWO comparator — the comparator must be fixed).
- *This is the same bug the sibling in-world chat fixed in the OLD port; the new converter
  regenerated it.* **Do this first.**

### 1b. 🔴 The `overrides/` durable-fix mechanism is GONE — **CONFIRMED** ✅ VERIFIED this pass
- There is **no `overrides/` directory** (untracked and absent on disk), **no `--overrides` CLI
  flag**, and **no override-application code** in `pipeline.py`/`cli.py`. Only stale *comments*
  remain (`typemap.py:22,25`, `analyze.py:125`). The **README still documents** `overrides/` +
  `--overrides` as the escape hatch — that documentation is now **stale/wrong**.
- MERGE1/2/3 all describe the durable-fix discipline as "dual-write: live tree **and**
  `overrides/<Class>.{h,cpp}`." **That path no longer exists.** Implication: handwritten
  class-level fixes (RenderSorter, signed-byte packets, render-brightness EntityRenderer/
  GuiScreen, etc.) currently have **no durable home** unless they were absorbed into converter
  emit/rules during the "ship-ready restructure" (`ada2d1a`, per MERGE1 §7).
- **Must resolve:** decide the going-forward durable-fix model — (a) reinstate an `overrides/`
  dir + wiring, or (b) commit to "all durable fixes become converter rules/emit changes" and
  update the README. Until then, verify whether the render-brightness / signed-byte / other
  handwritten fixes survive a fresh reconvert or live **only** in the untracked live tree.

### 1c. 🟠 Test suite drift — **CONFIRMED 21 failing** ✅ VERIFIED this pass
- `python -m unittest discover -s tests -t .` → 189 run, **21 fail** (7 FAIL + 14 ERROR).
- **14 ERRORs:** `rule_*() missing 1 required positional argument: 'plat'` — rule fns gained a
  platform param (`platforms.py`); the `test_rules._fire()` helper still calls them with 3 args.
- **7 FAILs:** stale fixtures — `original_java/` corpus was swapped (commit `a1db028`), e.g.
  `Vec3D.java` is now `package net.minecraft.src` with 5 fields; tests hardcode the old package/3
  fields. Also `test_emit`/`test_emit_source`/`test_emit_statements` expectations lag the corpus.
- **Not a converter regression — pure test drift.** Fix = thread `plat` through the test helper +
  re-baseline parse/emit expectations to the new corpus. Restores the safety net before edits.

### 1d. 🟠 Uncommitted networking work in the repo — needs review + commit ✅ VERIFIED this pass
- `git status`: `docs/platform/reference/Socket.reference.cpp` modified (+59/−4); untracked
  `docs/platform/xdk-api-index.{md,tsv}`, `xdk-toc.{tsv,txt}`, `tools/gen_xdk_toc.py`.
- The Socket diff contains three real hardware fixes (see §4 Networking), matching MERGE3 §3.
  Decision needed: review + commit these, or hold.

---

## 2. Converter (`j2x`) — committed state (HEAD `c8ee21d`)

- **Modular, correct-by-construction**, no postfix passes:
  `parse → ir → typemap/types → emit/ (naming, header, source, statements, ownership) →
  pipeline → cli`, plus `platforms.py` (multi-target), `runtime.py`/`stubs.py` (prelude +
  external stubs: JavaString, JArray, JList, GL11, Comparator, Collections, …), `rules.py`
  (crash-class detectors), and `analyze`/`report`/`ledger`/`worklist`/`signatures`/`vcxproj`.
- **Universal type model:** every Minecraft class is a **pointer**; only primitives, `String`→
  `JavaString`, and containers are by value; `T[]`→`JArray<T>` (length-carrying, bounds-checked);
  `Object`→`JObject`/`void*` dispatch. Rule-of-Three synthesized for owning classes.
- **Crash-class defenses:** `new T[n]()` value-init, null-guards on registry arrays, ctor
  `this(...)`/`super(...)` delegation inlined, override ctor back-fill, MI-diamond forwarding.
- **Reported link-clean** (444/444). Newest converter fix (`c8ee21d`): chained constructor
  delegation was being dropped — now replicated.
- **GUI + EXE:** `dist/j2x.exe` (PySide6 + PyInstaller) covers all CLI commands, opens on
  double-click. CLI: `python -m j2x convert <java_dir> <out_dir>` (+ `report`/`signatures`/
  `worklist`/`vcxproj`). ⚠️ README's `--overrides` flag is stale (see §1b).

### Open converter items
1. **Comparator / strict-weak-ordering rule** — the RenderSorter class of bug (§1a). Whole latent class.
2. **Test drift** (§1c).
3. **Durable-fix model** decision (§1b).

---

## 3. The Alpha port (game) — reported playable state

Boots to menu → generates/loads a singleplayer world → renders terrain/trees/water/clouds with
correct depth, day/night lighting → full controller (move/look/jump/mine/place/hotbar/pause) →
clean HUD → combat/containers → **working SMP** (see §4). First-person arm renders.

### Reported-fixed highlights (from MERGE1/2 — live tree, not independently re-verified this pass)
- **World/gen/physics:** Block ctor delegation stub (empty world), collision-box merge (fall-
  through), uninit `MovementInput` bool[] (auto-jump), LH X-mirror (`proj._11` negate + cull
  CW→CCW), ZENABLE re-enable after 2D HUD, render-distance clamp raised.
- **The "black skin/terrain on camera" bug:** vignette `%blur%`/`%clamp%` path prefix never
  stripped → NULL texture → Xenos PS samples last-bound atlas + darkening blend paints it
  full-screen. Fix: strip prefix in `toContentPath` + basename fallback in `Textures::load` +
  vignette blend `INVSRCCOLOR`.
- **HUD/alpha (all reference-confirmed):** keep `ALPHABLENDENABLE` on through HUD loop; crosshair
  `INVDESTCOLOR`/`INVSRCCOLOR`; `Textures::load` color key `0` (not `0xFF000000`) so black
  outlines/detail holes survive; `Shaders::s_alphaRef`/`setAlphaRef` (writes `fogParams[2]`) for
  item/particle alpha cutout.
- **First-person hand:** `CULL_NONE`; flip base forward-Z to `+Z`; un-negate `proj._11` for the
  hand only. **[OPEN]** forearm orientation tuning (`[HAND]` diagnostic wired in `ItemRenderer`).
- **Crashes:** dungeon-gen `goto`→`break` + `BlockContainer` flag + null-guards; mob-spawner gate
  on `difficulty > 0`; chest/ItemStack IP-0 null-guards; SMP spawn null-guard `s_blocksList[id]`;
  nether login signed `dimension` byte; entity-vtable guard in `World::func_633_c`.

### Reported open blockers
- **[OPEN]** Intermittent **gen-phase heap-corruption crash** (`0xC0000018`/IP 0x0) during
  gen/spawn catch-up (MERGE2 #9). Mitigated by an entity-vtable guard, root cause not found —
  **likely the RenderSorter UB in §1a** (world entry sorts all chunks) and/or another
  rule-of-three / buffer overrun. **Re-test after §1a is fixed** before hunting further.
- **[OPEN]** Hand forearm orientation (§3, functional/cosmetic).
- **[OPEN]** `shadow.png` doesn't resolve (lives in `Content/misc/`, requested at root).

### Stubbed / unwired subsystems
- **World save/persistence** — storage stubbed; placed blocks/chests don't survive reload/unload.
  `ContentStorage.cpp` (Save360/XContent) exists but is unwired.
- Texture-pack **usage** (loader + menu exist; application to rendering deferred by user request).
- Compression **LZX** / **OSK** / parts of **net** partly stubbed (see `xdk-subsystem-integration`).

---

## 4. Networking (SMP) — reported working + this-pass verification

- **Protocol match is non-negotiable:** client = **Alpha protocol 6**. Only the `minecraft_server`
  tree matches; the user's Beta 1.7.3 `server.zip` does **not** negotiate.
- **Insecure socket bypass (authoritative, from HttpProgram):**
  `XNetStartup(XNET_STARTUP_BYPASS_SECURITY)` + `WSAStartup(2,2)` +
  `socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)` + `setsockopt(SOL_SOCKET, 0x5802 **then** 0x5801, TRUE)`
  **before** connect (order authoritative). `SO_ERROR` is **absent** in 360 `winsockx.h`.
- **Two fixes landed** in `Socket.cpp`, synced to `Socket.reference.cpp` ✅ VERIFIED present in the
  uncommitted diff this pass:
  1. **Bounded XNADDR wait** in `globalInit()` — poll `XNetGetTitleXnAddr` past `PENDING` (~5s,
     50×100ms) so we don't connect before DHCP finishes (the #1 cold-boot flaky-connect cause).
  2. **Bounded connect** in `_doConnect()` — non-blocking + `select()` writable/except with 10s
     timeout (bad IP fails in ≤10s instead of hanging the connect thread). Restore blocking after.
  3. (Also in the diff) the `0x5802`/`0x5801` insecure flags — the reference file had been **stale/
     missing** them.
- **Tested E2E (emulator):** connected to a remote server, pulled 196 map-chunk packets +
  block/mob/inventory updates + rendered the other player. `WSAENOTSOCK` on close = benign teardown race.
- **🔑 CONSOLE (real hardware) limit:** `connect() … 10065 (WSAEHOSTUNREACH)` immediately to a
  remote public IP even with correct IP/mask/gateway/DNS. Root cause: **XNet + BYPASS_SECURITY
  does NOT route off-subnet** — LAN-only. Options: (a) **portproxy relay** on the ICS host
  (`192.168.137.1`) — `netsh interface portproxy add v4tov4 …` (admin shell, `iphlpsvc` running),
  console connects to the on-subnet gateway; (b) **run the Java server on-subnet** and connect to
  its LAN IP. **STILL TO CONFIRM on hardware:** does the on-subnet relay connect (proves the fix).

---

## 5. Open threads / resume points (union, deduped)

| # | Item | Kind | From | Status |
|---|------|------|------|--------|
| 1 | RenderSorter 0-for-equal + converter comparator rule | crash / converter | M3 | 🔴 confirmed live (§1a) — **do first** |
| 2 | Decide durable-fix model (`overrides/` gone) | process | reconcile | 🔴 confirmed (§1b) |
| 3 | Green the test suite (`plat` param + fixtures) | converter tests | M3 | 🟠 confirmed (§1c) |
| 4 | Review + commit networking (Socket + XDK docs) | networking | M3 | 🟠 uncommitted (§1d) |
| 5 | Confirm on-hardware relay / LAN SMP | networking | M3 | 🟠 unverified on HW |
| 6 | **In-world chunk mesher parallelization** | perf | M1 | 🟡 mid-impl — Stage A part 1 only (`fc9b10f`) |
| 7 | Intermittent gen-phase heap crash | crash | M2 | 🟡 re-test after #1 |
| 8 | Hand forearm orientation tuning | render | M2 | 🟡 cosmetic |
| 9 | Stream in-game chunk gen on `j2x_pool` + guard populate cascade | perf | M3 | 🟡 |
| 10 | Lighting: cap by time / fix 100k silent `clear()` | perf | M3 | 🟡 |
| 11 | World save/persistence (wire `ContentStorage`) | feature | M1/M3 | 🟡 stubbed |
| 12 | Strip/gate hot-path `DbgPrintF` (#ifdef) | perf/measurement | M1/M2/M3 | 🟡 makes emulator FPS trustworthy |
| 13 | `shadow.png` path; texture-pack usage; LZX/OSK | misc | M1/M2 | 🟢 low |

**Mesher parallelization (#6) resume detail (from MERGE1 §4):** Stage A part 1 done (`fc9b10f`:
`Tessellator::active()` + RenderBlocks redirect, no-op). Remaining: Stage A part 2 = per-chunk
block snapshot + `SnapshotBlockAccess` (ported light logic + **per-worker `WorldChunkManager`** for
grass/leaf color + per-instance `litFlag`), rewrite `WorldRenderer::updateRenderer` to mesh from
snapshot via a dedicated mesh Tessellator (`s_active`) INLINE first (byte-identical); Stage B =
TLS-backed `active()` (explicit `TlsAlloc`, **not** `__declspec(thread)` — pool uses raw
`CreateThread`), async submit + non-blocking per-frame drain, double-buffer, stale-job rejection,
`g_asyncMesh` kill-switch. B1 primitive already shipped: `Tessellator::extractTris` + `GL11::recordTris`.

---

## 6. Durable technical knowledge (must survive) — union of all three

- **Parallel world-gen (proven, `28757ab`, ✅ verified working):** per-worker *owned* objects (own
  `ChunkProviderGenerate` + own `WorldChunkManager`), seeded from world seed → deterministic →
  **race-free by ownership, not locks**. New fields `genWCM` (per-worker biome mgr, avoids shared
  temperature/humidity scratch race) + `genDeferSpawn` (main thread does `performWorldGenSpawning`
  after install). `j2x_pool` (`ThreadPool.h`): header-only, pinned to idle HW threads 2&4
  (main=0, XACT audio=5); `submit` runs inline if not started / ring full; jobs touch only owned
  data/snapshots; heap `new` is thread-safe.
- **Shader-only D3D9 on 360:** NO fixed-function pipeline (`D3DRS_LIGHTING` etc. absent). Two PS
  brightness passes: **g_Tint (PS c2)** = glColor fed in (dims items/mobs/hand by block-light;
  terrain/GUI use white); **g_Gamma (PS c3)** = `pow(rgb, g_Gamma.x)`. Per-pass gamma: **world
  0.80**, **2D/HUD/GUI 1.0**, **menu dirt 1.19** (+ tint `0x6C6C6C`, white-wash α25 / dark α51).
  Source of truth = `Projects/Minecraft.Client/`. **This scheme was silently lost once in a regen**
  — now lives in EntityRenderer/GuiScreen/LoadingScreenRenderer (durability now uncertain, see §1b).
- **Xenos quirks:** `D3DRS_ALPHATESTENABLE` ignored — alpha clip is shader-side
  (`clip(texel.a - g_FogParams.z)`, threshold `Shaders::s_alphaRef`). `SetTexture(0, NULL)` does
  NOT blank the sampler (leaked-texture quads). GL is RH; port uses LH projection with `proj._11`
  negated for the world + cull CW→CCW; hand needs per-element handedness.
- **IP 0x0 crash class:** call through NULL vtable/fn ptr. Top offenders `Block::s_blocksList[id]->`
  / `Item::s_itemsList[id]->` where id may be **unregistered** — **Alpha v1.2.6 has NO blocks at
  ids 21–34 and 36 ("gap ids")** → NULL there. Guard every such virtual call. `ItemStack::getItem()`
  = `s_itemsList[itemID]` (NULL for unregistered) = the chest crash class.
- **Signed-byte hazard:** Java `byte` is SIGNED; the converter maps it to `unsigned char`. Scalar
  byte fields that can be negative (dimension, signed deltas) MUST be `signed char`. (`typemap.py`
  caveat — was previously fixed via `overrides/`, which is now gone — see §1b.)
- **Recurring converter bug families:** ctor-delegation stubs (`this(...)` dropped); post- vs
  pre-multiply (`_w = _t * world` for row-vector D3D); zero-init arrays `new T[N]()`;
  rule-of-three double-free; virtual-call-in-ctor; label-goto → wrong loop restart;
  **non-strict-weak comparators (§1a)**.
- **Nexia DbgPrint cost ⚠:** per-tick/per-entity `DbgPrintF` floods the synchronous debug link →
  apparent ~0 FPS "hang." Cap/throttle; strip after diagnosis; suspect first when a hang appears
  right after adding markers.
- **Dual-write discipline (⚠ now partly broken):** live-tree fix + durable source. Platform helpers
  → `docs/platform/reference/*.reference.*` (still valid). Handwritten class overrides →
  `overrides/` (**gone — §1b**). Converter bugs → `j2x/`. Live tree is NOT git-tracked.
- **Verify every 360 API/const against the actual SDK headers** at
  `C:\Program Files (x86)\Microsoft Xbox 360 SDK\include\xbox\` before use. Controller =
  `xinput2.h`/`xinput2.lib` (NOT PC `XInputGetState`). XDK docs indexed in
  `docs/platform/xdk-api-index.tsv` / `xdk-toc.tsv` (11,906 entries; `tools/gen_xdk_toc.py` regens).
- **Measurement:** WIPE the temp out dir before a converter regen (`convert()` doesn't delete stale files).

---

## 7. Memory-dir ownership & cross-references

- **Converter/networking chats (MERGE1, MERGE3)** memory dir:
  `…\.claude\projects\C--Users-…-ConversionTool\memory\` — keys: `parallel-worldgen`,
  `render-brightness-passes`, `inworld-crash-playbook`, `nexia-dbgprint-cost`,
  `multiplayer-buildout`, `alpha-360-crash-classes`, `xbox360-d3d9-shader-only`,
  `xbox360-input-xinput2`, `xbox-swap-buildout`, `override-phase`, `converter-redesign`,
  `xdk-subsystem-integration`, `indev-render-reference`, `indev-audio-input-reference`,
  `retromcp-*`, `xdk-docs-index`, `user-profile`, `clean-repo-preference`.
- **Sibling in-world chat (authoritative render/sim/gen/lighting/mob playbook):** project
  `MinecraftAlpha` (no ConversionTool), session `c1df73f4-…`, separate memory dir
  `…\C--Users-…-Projects-MinecraftAlpha\memory\`. It root-caused (on the OLD converter):
  RenderSorter std::sort UB, value-array vs ref-array draw disconnect, NoiseGenerator
  rule-of-three, block-array zero-init, VB-Release-while-bound, per-frame light-drain freeze,
  populate cascade. **MERGE2 appears to be this sibling chat's output.**
- **Session ids seen:** MERGE1 `f110fbe3-…`, MERGE3 `41666556-…`, converter-redesign origin
  `f110fbe3-…`, sibling in-world `c1df73f4-…`.

## 8. Divergences reconciled this pass

1. **`overrides/` dir:** MERGE1 flagged "appears gone"; **confirmed fully gone** (no dir, no flag,
   no wiring) — README is stale. Elevated to a §1 blocker (§1b).
2. **RenderSorter:** MERGE3 reported it as the confirmed landmine; **confirmed still live** in the
   current live tree + runtime (§1a). Not yet fixed. Likely the root of MERGE2 #9's gen crash.
3. **Test drift:** MERGE3 reported ~21 failing; **confirmed exactly 21** with the two root causes (§1c).
4. **Networking:** MERGE3's Socket fixes are **present but uncommitted** in the repo (§1d, §4).
5. **HEAD:** all three said `c8ee21d`; **confirmed unchanged** — committed converter state is as described.

---

## 9. Recommended first moves (when development resumes)

1. **Fix RenderSorter** (source: 0-for-equal) — smallest, highest-impact, kills the confirmed
   crash/deadlock. Then decide its durable home (§1b) and write the converter comparator rule.
2. **Decide the durable-fix model** (§1b) — this gates how #1 and every future fix persists.
3. **Green the tests** (§1c) — restores the safety net before touching the converter.
4. **Review + commit** the networking + XDK-docs work (§1d).
5. Re-test the gen-phase crash (#7) — it may already be gone once #1 lands.
6. Then resume the mesher parallelization (#6) / in-game gen streaming (#9) FPS work.
