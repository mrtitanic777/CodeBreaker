# Step 6 — Audio (paulscode SoundSystem → XACT3): deliverables & integration checklist

Reference implementation written (Xbox-side; not built by the CodeBreaker repo — it needs the XDK's
`<xact3.h>` + the platform layer's `XactAudio.cpp/.h`, so it's verified by building the Xbox
project, not the converter):

- `Audio.reference.h` / `Audio.reference.cpp` — a **real `SoundSystem`** that REPLACES the
  converter's no-op `SoundSystem` stub and routes the new game's paulscode sound calls onto the
  **existing, shipped XACT3 backend** (`XactAudio` + `sounds.xwb`/`music.xwb`). This is **Route A**:
  reuse the proven 360 audio path; leave `CodecJOrbis` a no-op (no OGG decode needed).

## Which sound API the new game actually calls (so the bridge targets the right seam)

This is the key finding and the reason the bridge differs from the old port.

- **Old port:** `SoundManager.cpp` was a *hand override* that bridged straight to XACT
  (`toCue(name)` → `XaPlayCue`). The paulscode `SoundSystem`/`SoundPool` classes were effectively
  unused.
- **New port:** `SoundManager.cpp` is **generated and faithful** to the Java sound state machine
  (`C:\Users\USERNAME\AppData\Local\Temp\j2xv7\SoundManager.cpp`). It funnels *every* sound through a
  `static SoundSystem* s_sndSystem`, calling:
  `newSource` / `newStreamingSource` / `play` / `stop` / `setVolume` / `setPitch` /
  `setListenerPosition` / `setListenerOrientation` / `playing` / `cleanup`.
  The sound's bank-relevant identity (e.g. `"step.grass3"`, `"random.click1"`) is carried in
  `SoundPoolEntry::soundName` and handed to `newSource(.., identifier=soundName, ..)`.

So **the correct Route-A seam is `SoundSystem` itself**, not a `SoundManager` override. We leave
`SoundManager.cpp` generated and make the `SoundSystem` it talks to real.

> **UPDATE (2026-08-20):** correct but not sufficient on its own. The generated `SoundManager` NEVER reaches
> `SoundSystem` on console, because it gates every `newSource`/`play` on the OGG sound pool — EMPTY here
> (sound lives in XACT banks) — so `getRandomSoundFromSoundPool()==NULL` skips the whole block: all SFX
> silent, and `play("BgMusic")` (which starts the XACT playlist) never fires. The missing prerequisite is the
> **`sound_fix.py` generator pass** (`sfx-bypass` drops the guard + routes the name to the cue; `music-bypass`
> makes `play("BgMusic")` unconditional). With that pass, "leave `SoundManager` generated + make `SoundSystem`
> real" holds. It replaced the per-game `SoundManager` overlays (Beta's deleted).

Call sites confirmed in the generated output:
- `Minecraft.cpp:1032` `new SoundManager()`; `:302/:1033` `loadSoundSettings`; `:533` `setListener`;
  `:1102` `playStreaming(NULL,…)`; `:1184-1200` `installResource → addSound/addStreaming/addMusic`.
- `RenderGlobal.cpp:1136` `playStreaming` (records/ambient), `:1146` `playSound` (positional SFX).
- `PlayerController*.cpp` `playSound(stepSound->getStepSound()/getBreakSound(), …)`,
  `playRandomMusic()`.
- `GuiScreen.cpp:184` `playSoundFX("random.click",…)`; `EntityPlayerSP.cpp:156/162`
  `playSoundFX("portal.trigger"/"portal.travel",…)`.

Name flow (so the cue map keys are right): `installResource("sound/step/grass1.ogg", …)` →
`addSound("step/grass1.ogg")` → `SoundPool` key = text before first `.`, `/`→`.`, **trailing digits
stripped** = `"step.grass"`. At play time `playSound("step.grass", …)` →
`getRandomSoundFromSoundPool("step.grass")` returns an entry whose `soundName` is the full variant
(`"step.grass3"`). That `soundName` is what reaches `SoundSystem::newSource(identifier=…)` and what
`toCue()` maps to a bank cue.

## The cue-map approach

`AudioBridge::toCue()` in `Audio.reference.cpp` is **ported verbatim from the old
`SoundManager::toCue`** (`MinecraftAlpha/SoundManager.cpp:62-166`). It converts dotted/variant Java
names → the shipped bank's **short cue names** (`step_grass`, `step_grass 2`, `dig_stone3`,
`random_click`, `glass1`, `pig1`, …), strips the trailing `" 2"` variant token, lowercases, handles
the `dig.*`/`break.*`/`step.*`/`glass`/mob families, and returns `""` for anything not in the bank
(caller skips). Resolution happens in `SoundSystem::play()`: look up the channel's recorded
`soundName`, `toCue()` it, and `XaPlayCue()` it through the existing audio thread.

## Integration steps (in the Xbox VS project)

1. **Drop `Audio.h`/`Audio.cpp` in, REPLACING the converter's no-op `SoundSystem.h` stub** for the
   `SoundSystem` name. Keep the generated `SoundManager.{h,cpp}`, `SoundPool.{h,cpp}`,
   `SoundPoolEntry.{h,cpp}` as-is — they are the Java sound model and now drive the real bridge.
2. **Prerequisite — `XactAudio.cpp/.h` must be in the project.** Carry over the old port's XACT3
   engine unchanged (engine init, streaming wave banks, the audio thread pinned to HW thread 5, the
   music playlist, and the free functions `InitAudio`/`UpdateAudio`/`ShutdownAudio`/`StartMusic`/
   `StopMusic`/`XaPlayCue`/`XaSetSoundEnabled`/`XaSetMusicEnabled`). The bridge calls these.
3. **Ship the banks.** `sounds.xwb`/`sounds.xsb` + `music.xwb`/`music.xsb` in
   `game:\Content\resources\` (the paths `XactAudio::init()` opens). These already ship with the old
   port.
4. **Init at boot.** No extra wiring needed: the bridge calls `InitAudio()` from `SoundSystem`'s
   constructor, and the generated `SoundManager::tryToSetLibraryAndCodecs()` does `new SoundSystem()`
   exactly when audio is first wanted (mirrors the old override's `InitAudio()` seam). `InitAudio` is
   idempotent. *(Alternative: call `InitAudio()` once in `Main` after the device is up and make the
   `SoundSystem` ctor a no-op — pick one, don't double-init.)*
5. **Pump the engine.** The generated `SoundManager::setListener()` runs each frame from
   `Minecraft.cpp:533`; the music auto-advance lives in `UpdateAudio()`. Ensure `UpdateAudio()` is
   pumped every frame (either from `setListener` like the old override, or from the main loop). The
   XACT `DoWork()` itself is already pumped by the HW-thread-5 audio thread.
6. **Leave `CodecJOrbis` a no-op** (`runtime.py:1007-1011`). Route A never decodes OGG — XACT plays
   the prebuilt banks.
7. **Build the Xbox project (XDK).** Fix any `SoundSystem` call-shape mismatches the compiler flags;
   the bridge provides typed overloads for the exact shapes `SoundManager` uses plus a `(...)`
   catch-all so any other paulscode call still links as a no-op.

### Which stub methods are now real

| `SoundSystem` method | Route A behavior |
|---|---|
| `newSource` / `newStreamingSource` | record the channel's cue identity (`soundName`); no real voice |
| `play("BgMusic")` | `StartMusic()` — XACT shuffles `calm1/2/3`, auto-advances |
| `play("streaming"/"sound_N")` | `toCue(soundName)` → `XaPlayCue` (2D, fire-and-forget) |
| `stop("BgMusic")` | `StopMusic()` |
| `setVolume("BgMusic", v)` | `XaSetMusicEnabled(v>0)` (music mute works) |
| `setVolume("sound_N", v)` | **dropped** — no per-cue volume on the XACT path |
| `setPitch` | **dropped** — pitch disabled in old XACT path |
| `setListenerPosition/Orientation` | **ignored** — 2D mix |
| `playing("BgMusic")` | real (`isMusicCuePlaying`) so the music scheduler advances |
| `playing(other)` | `false` (SFX/streaming not tracked) |
| `cleanup` | `StopMusic` + `ShutdownAudio` |

## Route A vs Route B

- **Route A (delivered — recommended).** Reuse the shipped XACT3 backend. Restores sound
  immediately with assets that already ship and a path the old port proved works. Limits inherited
  from the old XACT path: **2D fire-and-forget** (no 3D attenuation), **no per-cue volume** (binary
  gate only; music mute works), **pitch disabled**, and **streaming ambience does not loop** unless
  the cue is authored looping in the bank.
- **Route B (only if 3D/pitch is a hard requirement).** Implement the paulscode `SoundSystem` for
  real over an Xbox audio engine (XAudio2 or XACT cue instances) with per-source voices, *and* back
  `CodecJOrbis::read/readAll` with a real OGG Vorbis decoder (stb_vorbis / libvorbis for 360) so the
  Java sound model runs unmodified — per-source pitch/volume, 3D positional sources, `.ogg` streaming
  music. Much more work (codec + per-source mixer + 3D listener), and only worth it because both
  pitch and 3D are exactly what Route A drops. The `setPitch`/`setListener*`/per-source `setVolume`
  bodies in `Audio.cpp` mark the seams a Route-B upgrade would fill in.

**Recommendation: ship Route A.** Defer Route B unless 3D audio / per-cue pitch becomes a
requirement.

## HARDWARE-VERIFY (can't be checked off-device — iterate on the 360)

- **Cue coverage.** `toCue()` can emit cues that may not be **authored** in `sounds.xwb/.xsb` —
  especially Alpha-new mobs/portals/doors (mapped at old `SoundManager.cpp:138-162`). When
  `GetCueIndex` fails the play silently no-ops. Confirm the bank contents against the cue list; add
  cues to the bank or accept silence for those sounds.
- **Music start/advance.** `playing("BgMusic")` must go `false` when a music cue ends so
  `playRandomMusic` queues the next track. Verify the 60-frame startup guard (`UpdateAudio`,
  `XactAudio.cpp:819/853`) doesn't race the first `isMusicCuePlaying()` check — symptom would be
  music never advancing or restarting instantly.
- **Init order / double-init.** Decide ONE init owner (the `SoundSystem` ctor *or* an explicit
  `InitAudio()` in `Main`). If both fire, confirm `XactAudio`'s guards make the second a no-op;
  otherwise banks/thread could be created twice.
- **Streaming ambience.** Water/lava/fire ambience comes through `playStreaming` → treated as a
  one-shot SFX here, so it won't loop. If looping ambient beds are wanted, author looping cues in
  the bank (or move ambience to Route B).
- **`setListener` as the frame pump.** If `UpdateAudio()` is driven from `setListener` (old-override
  style), confirm `setListener` is actually called every frame even when `soundVolume == 0` —
  otherwise the music state machine stalls while sound is muted. (Safer: pump `UpdateAudio` from the
  main loop unconditionally.)

## Status

`SoundSystem` bridge + ported `toCue` map delivered as reference. Remaining for audible sound:
`XactAudio.cpp/.h` + the four bank files in the project, building/iterating the Xbox project, and an
on-device pass over cue coverage + music advance. No converter (CodeBreaker) changes required — the new
`SoundManager`/`SoundPool`/`SoundPoolEntry` and the no-op `CodecJOrbis` stay generated as-is.

## Open questions

1. **Bank authoring for Alpha-new cues.** Are the mapped-but-maybe-unauthored cues (chicken, cow,
   creeper, zombie, skeleton, slime, spider, doors, portal) actually in the shipped `sounds.xwb`? If
   not, are they required for this build or acceptable as silent?
2. **Looping ambience.** Is non-looping water/lava/fire ambience acceptable under Route A, or does it
   need looping cues / Route B?
3. **Init owner.** Constructor-driven `InitAudio()` (delivered) vs explicit boot call in `Main` —
   which does the platform team prefer for ordering against the D3D device + file system mount?
4. **Frame pump source.** Pump `UpdateAudio()` from `setListener` (matches old override) or from the
   main loop (more robust while muted)? Affects music advance during sound-off.
5. **3D/pitch requirement.** Confirm Route A's dropped 3D attenuation + pitch are acceptable for
   ship; if not, scope Route B (real `SoundSystem` voices + `CodecJOrbis` OGG decode).
