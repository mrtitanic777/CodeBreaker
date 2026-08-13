# MERGE1.md — Chat status & knowledge dump (chat 1 of 3)

> **Purpose.** This is a self-contained status + knowledge snapshot of ONE Claude
> chat that has been working on the Xbox 360 Minecraft Alpha port + `j2x` converter.
> Three such chats exist; each produces a `MERGE<N>.md`. Together they get merged
> into one up-to-date project state. **Over-include rather than under-include.**
> A reusable prompt to generate the matching file in the other chats is at the bottom.

---

## 1. Chat identity

- **This chat:** the **ConversionTool / in-world port-debugging** chat. Its work centered on
  getting the *converted game* running on the Nexia360 emulator: multiplayer, in-world
  crash fixes, rendering/brightness, and **parallelizing world generation**.
- **Project dir it operated on:** `C:\Users\mrtit\Documents\Visual Studio 2010\Projects\MinecraftAlpha\`
  - Converter (git-tracked): `…\MinecraftAlpha\ConversionTool\` (the `j2x` Python package)
  - Live C++ game tree (NOT git-tracked): `…\MinecraftAlpha\MinecraftAlpha\` (the VS2010 Xbox360 project)
  - Emulator: Nexia360 at `C:\Nexia360`, log at `C:\Nexia360\nexia.log`
- **Git HEAD last seen:** `c8ee21d` (commits from other sessions have landed on top of this chat's last commit `fc9b10f`).
- **Session id:** `f110fbe3-cc63-45df-955b-df3d89b97ab5` (transcript on disk under `.claude/projects/…-ConversionTool/`).

## 2. Current committed project state (as of HEAD `c8ee21d`)

### Converter (`j2x`) — ship-ready
- Deterministic, **modular** Java→C++03 transpiler, **correct-by-construction**: the crash
  classes (uninit arrays, rule-of-three double-frees, NULL-deref virtual calls, dropped ctor
  registration, OOB array access) are prevented in the generator, not patched after.
- `j2x/` package: `parse → ir → pipeline → emit` (`typemap`/`stubs`/`rules`) + `analyze`/`report`/`ledger`/`worklist`/`signatures`/`vcxproj`/`platforms`.
- **GUI + standalone EXE**: `dist/j2x.exe` (PySide6 + PyInstaller) covers every CLI command, opens on double-click — no terminal needed.
- **Ship-ready restructure** (`ada2d1a`): deleted `legacy/`; folded `signatures`/`worklist`/`vcxproj` tools into the `j2x` CLI; `overrides/` applied automatically by default; output → `out/`; multi-profile (`C++03`/`C++98` under `dist/`, `profiles/`, `PlatformSDKs/`).
- Reported **link-clean (444/444)**. Newest converter fix: chained constructor delegation was being dropped (`c8ee21d`).
- Universal type model: every Java class = pointer; only primitives, `String`→`JavaString`, and containers by value; arrays = `JArray<T>`; `Object`→`JObject`/`void*` dispatch.

### The Alpha port (the game) — playable
- Boots to menu, generates/loads a singleplayer world, renders, HUD, controller input, audio (XACT3).
- Extensive in-world crash + rendering fixes landed (see §3). **Parallel world-gen verified working.**
- Known **stubbed/unwired** subsystems: world **save/persistence** (storage stubbed → placed blocks/chests don't survive reload/chunk-unload), texture-pack *usage* (loader exists, application deferred), compression LZX / OSK / net partly stubbed. See memory `xdk-subsystem-integration`.

## 3. What THIS chat implemented (commit hashes)

In-world / multiplayer / rendering run (most recent first):
- `fc9b10f` **Mesher Stage A (1/2)**: `Tessellator::active()` accessor + redirect RenderBlocks' 24 `s_instance` sites. Behavior-identical (no-op) foundation for off-thread meshing. **INCOMPLETE — see §4.**
- `0540ea7` Fix C4150 (include `MapGenBase.h` so per-worker `caveGenerator` deletes via virtual dtor).
- `28757ab` **Multithread spawn-area world generation** across idle HW cores (j2x_pool 2&4). ✅ VERIFIED: "way faster," 289 chunks / 2 workers, world enters clean.
- `fe22a13` Debug chests: place on the surface (`getHeightValue`, were buried at fixed y=64) + skip the gap/unregistered block ids they were stuffed with.
- `0d337cd` Gamma pass on the "Generating level" loading screen (separate dirt draw).
- `eb0c8f9` Fix chest IP-0 crashes: NULL-guard the whole `ItemStack`/`itemsList` render class (open + mine).
- `126f316` Restore per-pass gamma-correction scheme (world 0.80 / 2D 1.0 / menu dirt 1.19 + overlays) ported from `Minecraft.Client`.
- `f2f5dc5` Fix SMP spawn IP-0: NULL-guard `Block::s_blocksList[id]` in the chunk mesh path (+ ChunkCache/World getBlockMaterial + handleMobSpawn).
- `cf88ffd` Fix nether login crash: `Packet1Login.dimension` is a SIGNED Java byte (was `unsigned char` → -1 read as 255 → NULL provider).
- `07ce1cf` Item/world lighting: feed glColor as a global PS tint (g_Tint, register c2) — shader-only 360 dropped glColor on array draws.
- `81dae81` Render the SMP local player (alias `EntityClientPlayerMP`→RenderPlayer). `b07a5ff` cap per-frame chunk rebuilds. `6899d27` gate GUI cursor to container screens. Plus (earlier in session): DEFLATE/zlib/gzip inflate + gzip compressor, gamertag via XUserGetName, other-player render, signed Packet30Entity deltas, shadow/furnace/chest guards, ZipFile reader, GUI cursor (left-stick, container-only).

## 4. Open / pending threads (resume points)

1. **In-world chunk mesher parallelization — MID-IMPLEMENTATION.** This is the big one left.
   - The in-world FPS lockup on heavy chunk rebuilds is still handled only by a ~8-rebuilds/frame cap (`RenderGlobal::updateRenderers`, RenderGlobal.cpp:975).
   - ✅ **Stage A part 1** done (`fc9b10f`): `Tessellator::active()` (returns `s_active ? s_active : s_instance`) + RenderBlocks redirect. No functional change yet.
   - ⬜ **Stage A part 2**: build a per-chunk **block snapshot** (id/meta/skylight/blocklight for the ~20×128×20 region, border=2 to cover stair/farmland light recursion) + a snapshot-backed `IBlockAccess` (`SnapshotBlockAccess`) with (a) the ported light logic (ChunkCache `addPendingConnection` stair-max + `Chunk::getBlockLightValue` skylight/blocklight max, ChunkCache.cpp:106-150 / Chunk.cpp:422-434), (b) a **per-worker `WorldChunkManager`** for grass/leaf color (`BlockGrass`/`BlockLeaves::colorMultiplier` call `getWorldChunkManager()->getBiomesGrid()`+`temperature[0]`/`humidity[0]` — same hazard the gen fix solved), (c) a per-instance `litFlag` replacing global `Chunk::s_isLit`. Rewrite `WorldRenderer::updateRenderer` (WorldRenderer.cpp:221-306) to mesh from the snapshot via a dedicated mesh Tessellator (`s_active`), tile-entity collection stays main-thread. Run it INLINE (single-threaded) first — must be byte-identical.
   - ⬜ **Stage B**: make `active()` **TLS-backed** (explicit `TlsAlloc`/`TlsSetValue`/`TlsGetValue` — NOT `__declspec(thread)`; the pool uses raw `CreateThread`). Split `updateRenderer` into beginMeshSnapshot(main)/`_meshJobFn`(worker: mesh→`extractTris`)/installMesh(main: `glNewList`+matrices+`GL11::recordTris`+`glEndList`). Convert `updateRenderers` to async submit + **non-blocking per-frame drain** (Interlocked `done` flag), `m_meshInFlight`, INFLIGHT/INSTALL caps, **double-buffer** (chunk keeps old list until install), stale-job rejection via a per-WorldRenderer `meshGen`. `g_asyncMesh` kill-switch + sync fallback.
   - The full staged plan (with file:line anchors, the `MeshJob`/`ChunkMeshSnapshot` structs, and 6 named race risks incl. "verify grass/leaf color biome read") lives in this chat's transcript — the `Plan` agent output right before `fc9b10f`.
   - The already-shipped B1 primitive: `Tessellator::extractTris(std::vector<TesselatorVertex>&)` + `GL11::recordTris(vec, textured)` already separate geometry-gen from GL-recording.
2. Texture-pack **usage** (application to rendering) — loader + menu exist, deferred by user request; notated.
3. World **save/persistence** — storage subsystem stubbed; placed blocks/chests don't persist. `ContentStorage.cpp` (Save360/XContent) exists but is unwired. Would fix "chests despawned on reload."
4. Live-only DbgPrint **diagnostics cleanup** — `[world]`/`[hud]`/`[de]`/`[rp]`/`[op]`/`[mp]`/`[rd]` markers; most are one-shot/capped now but should be stripped once stable (⚠ per-tick/per-entity DbgPrint floods the synchronous Nexia link → ~0fps "hang").

## 5. Key technical knowledge held by this chat (must survive the merge)

- **j2x_pool** (`ThreadPool.h`): header-only worker pool pinned to idle HW threads 2&4 (main=0, XACT audio=5). API: `submit(JobFn,void*)`, `wait()` (barrier), `pending()`, `isStarted()`, `workerCount()`. `submit` runs inline if not started / ring full (built-in fallback). Jobs must touch only owned data or snapshots. Heap `new` is thread-safe.
- **Parallel-gen pattern (proven, `28757ab`)**: per-worker *owned* objects (own `ChunkProviderGenerate` + own `WorldChunkManager`), seeded from the world seed → deterministic/identical output → race-free by OWNERSHIP not locks. Two new `ChunkProviderGenerate` fields: `genWCM` (per-worker biome mgr, avoids the shared temperature/humidity scratch race) and `genDeferSpawn` (skip `performWorldGenSpawning` on the worker; main thread does it after install). Install into `ChunkProviderLoadOrGenerate->chunks[(x&31)+(z&31)*32]` + `generateBlocklightMap`+`onChunkLoad`; reset `lastQueriedChunk=NULL`. Verified no mutable file-static state in NoiseGenerator*/MapGen*/WorldChunkManager.
- **Shader-only D3D9 on 360**: NO fixed-function pipeline (`D3DRS_LIGHTING` etc. absent). Two pixel-shader brightness passes: **g_Tint (PS c2)** = current glColor fed in (dims items/mobs/hand by block-light; terrain/GUI use white glColor); **g_Gamma (PS c3)** = `pow(rgb, g_Gamma.x)` display gamma. Per-pass gamma VALUES: **world 0.80** (brighter), **2D/HUD/GUI 1.0**, **menu dirt 1.19** + gamma-compensated tint `0x6C6C6C` + white-wash(α25)/dark(α51) overlays. Source of truth for the gamma scheme = the user's `Projects/Minecraft.Client/` port. This scheme was SILENTLY LOST once in a regen — it now lives in override-managed `EntityRenderer`/`GuiScreen`/`LoadingScreenRenderer`.
- **Recurring 360 crash class = IP 0x0** (0xC0000005, "referenced memory at 0x0") = call through a NULL vtable/function pointer. Top offenders: `Block::s_blocksList[id]->` / `Item::s_itemsList[id]->` where `id` comes from world/network/item data and may be unregistered (**this Alpha v1.2.6 has NO blocks at ids 21-34 and 36 — "gap ids"** — s_blocksList NULL there). Guard every such virtual call. `ItemStack::getItem()` returns `s_itemsList[itemID]` (NULL for unregistered) — the chest crash class. Java `byte` is SIGNED but the converter maps it to `unsigned char`; scalar byte fields that can be negative (dimension, signed deltas) MUST be `signed char` (`typemap.py` caveat, fixed via overrides per-packet).
- **SMP** = Alpha protocol 6 (the user's `minecraft_server` tree; their Beta 1.7.3 server.zip is incompatible). No window-click packets; containers client-side; tile entities via Packet59 (gzip NBT). NBT=gzip, MapChunk=zlib. `Inflate.{h,cpp}`: rawInflate/zlibInflate/gzipInflate/gzipDeflateStored (all big-endian-safe).
- **Dual-write discipline**: fix the live tree AND the durable source (converted-class → `overrides/<Class>.{h,cpp}`; platform helpers → `docs/platform/*.reference.*`). Converter only applies an override if a matching Java class parses (no `Debug.java` → `overrides/Debug.cpp` is git-backup only). Live tree is NOT git-tracked; only `ConversionTool/` is. Big-endian PowerPC, `char` is UNSIGNED by default, C++03 only.
- **Nexia DbgPrint cost** ⚠: per-tick/per-entity DbgPrint floods the synchronous debug link → apparent ~0fps "hang". Cap/throttle; strip after diagnosis; suspect first when a hang appears right after adding markers.
- **Verify every 360 API/const against the actual SDK headers** at `C:\Program Files (x86)\Microsoft Xbox 360 SDK\include\xbox\` before use. Controller = xinput2.h/xinput2.lib (NOT PC XInputGetState). XDK API docs indexed in `docs/platform/xdk-api-index.tsv` (11,906 entries).

## 6. Memory files this chat maintains
Dir: `…\.claude\projects\C--Users-…-ConversionTool\memory\` (index = `MEMORY.md`). Key entries: `parallel-worldgen`, `render-brightness-passes`, `inworld-crash-playbook`, `nexia-dbgprint-cost`, `multiplayer-buildout`, `alpha-360-crash-classes`, `xbox360-d3d9-shader-only`, `xbox360-input-xinput2`, `xbox-swap-buildout`, `override-phase`, `converter-redesign`, `xdk-subsystem-integration`, `indev-render-reference`, `indev-audio-input-reference`, `retromcp-deobf-audit`, `xdk-docs-index`, `user-profile`, `clean-repo-preference`.

## 7. Possible divergence to reconcile at merge time
- **`inworld-crash-playbook`** points to a SECOND Alpha chat (**project `MinecraftAlpha` (no ConversionTool), session `c1df73f4`**) that root-caused in-world bugs on an OLDER monolith converter — likely one of the other two chats. Its memory dir is separate (`…-Projects-MinecraftAlpha\memory\`). Expect overlapping crash-class knowledge; reconcile which fixes are already in the current `ConversionTool` code vs only in that chat's tree.
- This chat's mesher work (`fc9b10f`) predates the **ship-ready restructure** (`ada2d1a`) that "baked in overrides" — verify where the durable `Tessellator` override lives now (top-level `overrides/` appears gone; the running port is unaffected since Stage A1 is a no-op).
- Converter dates in commit messages show Sonnet 4.6 authored the restructure/GUI commits — those are from a DIFFERENT chat than this one. Treat the GUI/EXE/restructure as another chat's contribution.

---

## PROMPT TO GENERATE THE MATCHING FILE IN THE OTHER CHATS

> Paste the block below into each of the other two converter chats. Tell each one its number (MERGE2, MERGE3).

```
You are one of three Claude chats that have worked on the same project: an unofficial
Xbox 360 (RGH homebrew) port of Minecraft Alpha v1.2.6, built with a deterministic
Java→C++03 transpiler (the "j2x" converter). Project root:
C:\Users\mrtit\Documents\Visual Studio 2010\Projects\MinecraftAlpha\  (converter under
…\ConversionTool\ ; live C++ game tree under …\MinecraftAlpha\ ; emulator log at
C:\Nexia360\nexia.log). NOTE: there may be a SECOND project dir at
…\Projects\MinecraftAlpha\ without ConversionTool — if that's the tree YOU worked in, say so.

I am collecting one status+knowledge file per chat so I can merge all three into a single
up-to-date project state. Produce a file named MERGE<N>.md (I will tell you N) at the project
root, with EXACTLY these sections — ground every claim in the actual repo (run `git log
--oneline -25`, read files) rather than memory alone, and OVER-include rather than under-include:

1. Chat identity — which chat this is (its topic/focus), the exact project dir it operated in,
   its session id if visible, and the git HEAD you last saw.
2. Current committed project state — what's in the repo now (converter state, port state,
   what builds, what's stubbed), grounded in git + a look at the tree.
3. What THIS chat implemented — concrete tasks/fixes/features, with commit hashes where known.
4. Open / pending threads — anything unfinished, mid-implementation, or deferred, with enough
   detail (file:line, the plan, the resume point) to pick it back up.
5. Key technical knowledge — durable facts, decisions, gotchas, APIs, platform constraints,
   design patterns, and specific values this chat holds that a merged view must not lose.
6. Memory files — list the persistent memory files you maintain (path + one-line each).
7. Possible divergence — anything you believe may CONFLICT with the other chats or with the
   current committed code (e.g., you remember a file/decision one way but git shows another).
   Flag it explicitly so it can be reconciled.

Do NOT change any code — just report. Match the structure of MERGE1.md so the three files line up.
```

## HOW TO MERGE (for whoever runs the final pass)
Feed MERGE1/2/3 to one chat and ask it to produce a single `PROJECT_STATE.md`: (a) reconcile
§2 across all three against current git HEAD as the source of truth for *committed* code; (b)
union §3 (dedupe by commit hash); (c) union §4 open threads, flag any a later commit already
closed; (d) union §5 knowledge, resolve §7 conflicts explicitly; (e) note which chat owns which
memory dir. The newest committed code wins over any chat's remembered state.
