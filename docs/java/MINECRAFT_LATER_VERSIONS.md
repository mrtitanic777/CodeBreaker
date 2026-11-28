# Minecraft past Beta 1.9: notes for a later session

Where the Java lane stands (2026-11-27) and what converting the release versions would take.

## Where it stands

- All 87 Indev, Alpha and Beta versions (Indev 2009-12-23 to Beta 1.9-pre5) convert from scratch, build and play
  off-screen on all seven PC graphics options: OpenGL, Direct3D 7, 8, 9, 10, 11 and 12 (`codebreaker java port
  <workspace> --platform pc --graphics opengl|d3d7|d3d8|d3d9|d3d10|d3d11|d3d12`).
- Direct3D 7 and 8 are 32-bit programs (Windows has them for 32-bit only), built with a 32-bit MinGW-w64
  (`C:\msys64\mingw32`, with i686 SDL2 and libvorbis) or Visual Studio's Win32 platform.
- The GL layer (`config/java/platform/reference/GL11.reference.cpp`) is OpenGL 1.x fixed function, with display
  lists, texgen, two lights, fog, alpha test and two texture units, on any backend.

## The versions past Beta 1.9

The version list the prep uses (mcphackers' versions JSON, `work/allversions/versions.json`) has mappings for six
releases: **1.0.0, 1.1, 1.2.3, 1.2.4, 1.2.5, 1.5.2**. 1.3, 1.4 and 1.6+ have none there: they would convert with
obfuscated names (`a`, `b`, `c`) unless mappings are found elsewhere (MCP's own for 1.3 to 1.12, for example).

| Releases | Expected work |
|---|---|
| 1.0 to 1.5.2 | Same LWJGL 2 and fixed-function GL the layer covers, so it should mostly just work. Each brings new converter and runtime paths: villages, strongholds, Nether fortresses, the End, 1.3's integrated server (the client starts a server thread and talks to it over sockets), 1.4's new items and mobs, and 1.5's texture changes (the stitched texture atlas, which needs `glTexSubImage2D` and mip levels in the atlas). |
| 1.6 to 1.12 | A new launcher and asset system (an assets index with hashed objects), resource packs, more GL (framebuffer objects, VBOs, `glDrawArrays`), Java 6/7 features. |
| 1.13 and up | Data-driven blocks and a newer LWJGL. From 1.15 the renderer moves to buffers and shaders, and from 1.17 it needs OpenGL 3.2 core with GLSL shaders: a GL 3 layer, not the 1.x one. |

## How to run them

1. Prep one: `build\codebreaker.exe java decompile 1.2.5 work\allversions\ws\1.2.5`.
2. Use the all-versions harness in `work/allversions`:
   - `harness.ps1 -Version <id> -Graphics <api>` for one version;
   - `runlane.ps1 -Lane <file of ids> -Graphics <api> -Jobs 3` for a list;
   - results go to `results[-api]/<id>/result.txt`, with screenshots.
3. Start on OpenGL, then the Direct3D APIs. The scripted menu clicks (`Get-EraScript` in `runlane.ps1`) are set for
   Indev to Beta 1.9: the release menus (1.0's Singleplayer, Create New World, the world-type buttons from 1.1)
   need their own clicks at 854x480. Check `menu2.bmp` and `menu3.bmp` in each result to find them.

## Things learned the hard way (worth knowing before starting)

- **32-bit crashes that a Debug build doesn't show:** suspect the compiler before the code. GCC 16's identical-code
  folding merged a static method into a thiscall member (fixed: `-fno-ipa-icf` in the generated CMake for 32-bit).
- **Memory:** the runtime's arrays were never freed until 9b8a19fa3 (Indev's level generation went to 3.6 GB). A
  second leak, objects the runtime never deletes (`JObject`, `JavaRT.h`), is still open: in-20091231-2255 peaks near
  3 GB. Bigger releases will feel it more, so watch peak memory, above all in 32-bit builds.
- **Timeouts under load are not bugs:** with five runners on 16 cores the game runs at about 11 frames per second.
  Rerun a TIMEOUT on a quiet machine before chasing it.
- **A stale workspace hides remap fixes:** after a mapping or remap change, delete `ws\<id>` and prep again.
