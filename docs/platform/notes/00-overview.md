# Platform layer — overview & buildout plan

**Goal:** the NEW `j2x` converter output (clean, faithful, crash-class-guarded game logic) replaces the OLD monolith game code in the Xbox 360 project, **reusing/adapting the old port's proven platform layer** to back the new converter's no-op platform stubs. The old monolith is buggy game code, but its *platform* infrastructure largely works and is the reference (and in several cases directly reusable).

Projects:
- **NEW** transpiler + game logic: `ConversionTool/` (stubs in `j2x/runtime.py`).
- **OLD** Xbox 360 project (reference): `../MinecraftAlpha/` (`MinecraftAlpha.sln`, builds on the XDK).
- XDK: `C:/Program Files (x86)/Microsoft Xbox 360 SDK`.

## The big realization

The old port has **far more real, reusable platform code than "just a reference"**:

| Component | Old-port file | Reusable? |
|---|---|---|
| D3D9 device + present | `Main.cpp`, `Common.h` | Directly |
| **HLSL shader pipeline** (vs/ps 3.0; Xenos has NO fixed-function) | `Shaders.cpp` | Directly — this IS the GL emulator's backend |
| **Tesselator** → `DrawPrimitiveUP` | `Tesselator.cpp` | As the geometry-submit core |
| **XInput** + full control mapping | inline in `Minecraft.cpp`/`EntityPlayerSP.cpp`/`GuiScreen.cpp` | As a behavior spec (needs a wrapper) |
| **XACT3 audio** + shipped `sounds.xwb`/`music.xwb` banks | `XactAudio.cpp`, `SoundManager.cpp` | Directly (Route A) |
| Texture loader (`D3DXCreateTextureFromFileExA`, int-handle registry) | `Textures.cpp`, `RenderEngine.cpp` | Directly |
| Assets: loose PNGs under `game:\Content\…` | on-disc | Directly |
| NBT save/load + **STFS save container** (`Save:\`) | `Save360.cpp`, `CompressedStreamTools.cpp` | Directly |
| `java.io` → Win32 bridge | `JavaIO.h`, `DataInput.h`/`DataOutput.h` | Directly |

So the buildout is mostly **"adapt these behind the new converter's stub interfaces"**, not "write from scratch."

See the per-subsystem docs:
- [01 — Architecture & lifecycle](01-architecture-and-lifecycle.md)
- [02 — Rendering: GL → D3D9](02-rendering-gl-to-d3d9.md) ← biggest piece
- [03 — Input & audio](03-input-and-audio.md)
- [04 — Textures, resources, file I/O](04-textures-resources-fileio.md)
- [05 — Known bugs & crash classes](05-known-bugs-and-crash-classes.md)

## Critical facts that change the plan

1. **Rendering is shader-based, not fixed-function.** Xbox 360 (Xenos) has no FFP — a draw with no vertex shader bound is rejected. The old port runtime-compiles 4 HLSL shaders (textured/untextured × vs/ps 3.0) in `Shaders.cpp`. The GL emulator must own a shader pair; reuse `Shaders.cpp`.

2. **The new code uses the vertex-ARRAY path, not `glBegin`/`glVertex`.** The faithful Java `Tessellator` calls `glVertexPointer/glColorPointer/glTexCoordPointer/glDrawArrays` (interleaved 32B stride). So the emulator hot path is *vertex-array → `DrawPrimitiveUP`*, NOT an immediate-mode `glBegin` accumulator. (~52 distinct `GL11` funcs used; top: glEnable/glDisable, glRotatef, glBindTexture, glTranslatef, glColor4f.)

3. **`initStatics()` must be driven in order from Main.** The converter EMITS static initializers but C++ gives no cross-TU init ORDER guarantee — the old port hand-calls ~13 `initStatics()` in dependency order (`Main.cpp:93-105`); skip them and every Block/registry is NULL. The new build's Main must do the same (or the converter must emit an ordered init driver). **This is a converter/runtime gap to close.**

4. **Present/BeginScene/EndScene are scattered in the renderer** (`EntityRenderer.cpp` etc.), not the main loop. The new `Display::update`/`swapBuffers` must own EndScene+Present (platform owns BeginScene). Dropping Present was the original "blue screen on boot" bug.

5. **Color byte order: Java packs `0xAABBGGRR`, D3DCOLOR is `0xAARRGGBB`** → the emulator must swap R↔B when building vertex color.

6. **Alpha test (cutout blocks: leaves/glass/crops) is a shader `clip()`** — Xenos ignores `D3DRS_ALPHATESTENABLE`. Map `glAlphaFunc`/`glEnable(GL_ALPHA_TEST)` → `Shaders::setAlphaRef`. Fog is shader-based too. Lighting/normals are no-ops (MC bakes light into vertex color).

7. **Big-endian PowerPC.** All NBT/binary I/O is assembled byte-at-a-time MSB-first (endian-safe, Java-faithful). The trap: any `memcpy`/`reinterpret_cast`/`*(int*)ptr` over a serialized buffer byte-swaps on the 360 and corrupts saves. Keep the manual byte assembly.

8. **Audio is solved (Route A).** A complete XACT3 backend + shipped banks already exist; bridge via a `SoundManager` override and leave the paulscode `SoundSystem`/`CodecJOrbis` stubs no-op. (Route B = real OGG decoder, only if 3D/pitch is needed.)

9. **Random divergence:** old `Common.h` used a non-Java 64-bit LCG; the new runtime uses the faithful Java 48-bit LCG. The new one is *correct* (same seed → same world as PC Alpha), but worlds differ from the old port. Intended.

## Recommended buildout order

0. **MSVC/XDK compile gate** — get the new output + `runtime.py` headers compiling under `cl.exe`/the XDK (only g++ tested). Watch JObject conversion operators + templates. *Prerequisite for everything.*
1. **App skeleton** — new `Main.cpp`: D3D9 init (reuse old) → ordered `initStatics()` → `new Minecraft()` → run loop; `Display`/`Sys`/`BufferUtils`/`GLContext` stubs backed.
2. **GL → D3D9 emulator** (the big one) — vertex-array path → `DrawPrimitiveUP` via reused `Shaders.cpp`; matrix stacks; texture binding; state cache; alpha-ref/fog; 3D-perspective vs 2D-ortho. Gets the world on screen.
3. **Textures** — back `ImageIO`/`BufferedImage`/`RenderEngine` with the `Textures.cpp` loader + `game:\Content\` PNGs.
4. **Input** — one XInput poll/frame → back `Keyboard::isKeyDown`/`Mouse::getDX/getDY`/`isButtonDown` (right stick → mouse-look is the live look path now). Reuse the documented control map.
5. **File I/O + saves** — back `File`/streams with `JavaIO.h`; mount the STFS `Save:\` container (`Save360.cpp`).
6. **Audio (Route A)** — `SoundManager` → XACT backend.
7. Skip: networking (single-player), AWT/dev-tools (IsomPreview).

## Converter-improvement opportunities (crash classes the old port hit that `j2x/rules.py` does NOT yet guard)

From [05](05-known-bugs-and-crash-classes.md) — fold these back into the converter so the new build is correct by construction:
- **OOB fixed-array indexing** (e.g. `m_blocks[x<<11|z<<7|y]` with y≥128 → real-heap corruption / `RtlValidateHeap`). Highest-value missing rule.
- **Static-init-order across TUs** (see fact #3 — ordered init driver).
- **Virtual call in constructor** (dispatches to base → wrong data recorded).
- **Endianness / raw cast over a serialized buffer.**
- Already guarded: uninit-array, ctor-stub, rule-of-three, value-copy-of-owner; null-deref via `converter_lint` (verify still wired).

## Open decisions for the user
- **Static init**: have the new converter emit an *ordered* init driver, or hand-write the call list in the new Main.cpp (like the old port)?
- **Projection handedness**: keep the old port's LH + `_11`-negate X-mirror + cull-flip hack, or move to RH (cleaner, but flips all winding)?
- **Chunk geometry**: the Java uses display lists (`glNewList/glCallList`) for world chunks — re-tessellate per frame, or implement record/replay into D3D9 vertex buffers? (Needs a RenderGlobal/WorldRenderer deep-dive.)
- **Saves**: keep raw/uncompressed NBT (not PC-compatible), or implement real gzip for interop?
- **Audio**: Route A (reuse XACT) — confirm.
