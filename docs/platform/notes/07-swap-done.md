# Code swap — DONE (in-place). Build it in VS2010.

The working Xbox project `../MinecraftAlpha/` was swapped in-place to the new j2x codebase
(2026-06-27). Source backup: `../MinecraftAlpha_src_backup_20260627_preswap.tgz` (restore with
`tar xzf` from the parent dir if needed — the project is NOT git-tracked).

## What changed
- **Removed** 785 old-monolith game `.cpp/.h` (incl. old-version leftovers: Player/Zombie/Map/
  IAnimals/GuiLoadLevel…) and the old `Enum*.cpp` (enums are header-only now).
- **Kept** the platform layer (18 files): Shaders, Tesselator, Textures, XactAudio, Save360,
  ContentStorage, Debug, stdafx, Common.h, JavaIO.h.
- **Added** the 1018 new j2x files (444 game `.cpp` + headers + runtime: JavaRT/JavaString/JArray/
  JList/Random… + `j2x_init.{h,cpp}`). The platform `stdafx.h` was kept (the stub one not copied).
- **Wired the platform layer** over the no-op stubs:
  - GL11.h/.cpp, GLU.h/.cpp (real GL→D3D9), GL12.h kept (one enum token).
  - Input.h/.cpp; `Mouse.h`/`Keyboard.h`/`Controllers.h` are now `#include "Input.h"` shims.
  - NIOBuffers.h/.cpp; `ByteBuffer/FloatBuffer/IntBuffer.h` → `#include "NIOBuffers.h"` shims.
  - FileIO.h/.cpp; `File/InputStream/OutputStream/FileInputStream/FileOutputStream/DataInput(Stream)/
    DataOutput/BufferedReader.h` → `#include "FileIO.h"` shims.
  - ImageIO.h/.cpp + real BufferedImage.h.
  - Audio.h/.cpp; `SoundSystem.h` → `#include "Audio.h"` shim. CodecJOrbis.h kept (no-op).
  - Textures_ext.cpp + 4 method decls added to Textures.h (genId/uploadRGBA/…).
  - New `Main.cpp` (calls `j2x_initAll()`; new `Minecraft(Component*,Canvas*,…,w,h,bool)` ctor).
- **Reconciled** `Common.h`: removed its `class Random` (clashed with the runtime's) → `#include
  "Random.h"`.
- **Regenerated** `MinecraftAlpha.vcxproj`: was 50 MB of blank-line bloat → 64 KB, listing the 461
  `.cpp` + 588 `.h`, `stdafx.cpp` PCH block + all 6 configs preserved. (Tool:
  ``python -m j2x vcxproj`` — re-run after any future swap.)
- **Cleaned** ~430 MB of stale Debug/Release/LTCG obj + the 29 MB `.sdf`.

## Build it (VS2010 + Xbox 360 XDK — can't be built from the converter repo)
1. Open `MinecraftAlpha.sln` in VS2010, config `Debug|Xbox 360`. **Ensure `/GR` (RTTI) is ON** — the
   new code relies on `typeid` (Class identity, JObject, the registries). Exceptions: `/EHsc` (the
   code uses try/catch) or strip — your call.
2. Build. Expect a shake-out pass (this swap was assembled, not compiled here — the bare XDK
   command line can't build it: `d3dx9.h` needs the Windows-SDK `ObjBase.h` that the VS IDE supplies).
3. Likely first-build items, in order of probability:
   - **LWJGL overload stragglers** — the GL/Input/buffer real impls cover the overloads the game
     uses; the compiler may flag a few more. Add thin wrappers (see GL11.reference.h pattern).
   - **Method-coverage gaps** — a real class (ByteBuffer/File/SoundSystem/…) missing a method the
     game calls that the old no-op `(...)` stub silently accepted. Add it (no-op or real).
   - **PCH**: every `.cpp` already `#include "stdafx.h"` first; if a runtime header needs something
     from stdafx, include it.
4. Then bring up on a 360 and walk the `// HARDWARE-VERIFY` notes across docs/platform (projection
   handedness/winding, color swizzle, GUI y-flip, audio cues, save durability).

## Crash classes — prevented by construction
null-deref, uninit arrays, rule-of-three, static-init order (j2x_initAll), virtual-call-in-ctor,
endianness — all guarded by the converter or flagged every build (j2x/rules.py).
