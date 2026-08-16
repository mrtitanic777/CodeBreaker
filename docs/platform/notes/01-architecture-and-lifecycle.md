# Platform Architecture & Lifecycle (OLD project reference)

> Scope: how the OLD hand/old-converter Xbox 360 port boots, runs, and shuts down, so the
> NEW transpiler can re-implement the platform layer behind its LWJGL-style stub headers.
> All paths are absolute; line refs are against the OLD project unless noted.
> OLD project root: `C:/Users/USERNAME/Documents/Visual Studio 2010/Projects/MinecraftAlpha/MinecraftAlpha/`

## Overview

The OLD port replaces Java/LWJGL's `Display`/AWT entry path with a bare Xbox 360 `void main()`
that creates a Direct3D 9 device up front, runs the Java `static {}` initializers by hand, then
constructs `Minecraft` and calls `run()`. The game code calls **D3D9 directly** (`g_pd3dDevice->...`)
rather than through any GL abstraction — every `GL11.*`/`Display.*` call from the original Java was
hand-rewritten into D3D9 + a small `Shaders`/`Tesselator` helper layer at conversion time. The
frame loop lives inside `Minecraft::run()`, and the actual `BeginScene`/`EndScene`/`Present` swap is
buried in the renderer (`EntityRenderer::func_4136_b`), not in `Main.cpp`. The NEW transpiler emits
faithful `GL11::glBegin`-style code instead, so the NEW platform layer must implement
`Display`/`Sys`/`GLContext`/`BufferUtils` (+ the GL namespaces) on top of D3D9 and own the
device + present itself, rather than scattering D3D calls through game code.

## Entry point & main loop

Entry: `Main.cpp` (`MinecraftAlpha/Main.cpp`). There is **no `WinMain`** — the XDK uses a plain
`void __cdecl main()` (`Main.cpp:76`). Console title comment (`Main.cpp:1-6`) notes this file is the
`handwritten_alpha` override that replaced the converter's placeholder Main (which only blue-cleared
the screen — the "blue screen on boot" bug).

Startup sequence (`Main.cpp:76-114`):
1. `InitD3D()` (`Main.cpp:40-63`, called at `:77`) — creates `g_pD3D` + `g_pd3dDevice`. On failure
   it `DbgPrintF`s and returns (no game).
2. `ApplyDefaultRenderState()` (`Main.cpp:65-74`, called `:78`) — Z-test LEQUAL, ZWRITE on,
   `D3DCULL_CW`, alpha-blend off, point sampling (nearest-neighbour, for the pixel-art textures).
3. `Shaders::init()` (`:80`) — compiles the textured/untextured VS+PS (the GL fixed-function
   replacement). Returns on failure.
4. Textures: `Textures::init()` + `Textures::loadAll()` (`:82-83`), then explicit loads of
   `terrain.png`, `gui/gui.png`, `mob/char.png`, `default.png` from `game:\Content\...`
   (`:84-87`) into the global texture handles.
5. **Java `static {}` initializers run by hand** (`:93-105`) — the converter emits these as
   `Class::initStatics()` but never calls them; without them every enum singleton / `Block` /
   registry is NULL and the first use jumps a null vtable. Order matters: enums first (no deps),
   then `MathHelper`, `Material`, `Block`, `EntityList`, `TileEntity`, `MobSpawnerBase`, `OsMap`.
6. `g_minecraft = new Minecraft()` (`:111`) — default ctor (its init list builds `m_sndManager`,
   renderers, timer, etc.).
7. Set `m_displayWidth/m_displayHeight = SCREEN_WIDTH/HEIGHT` (`:112-113`) — substituting what the
   parameterized AWT ctor would have set.
8. `g_minecraft->run()` (`:114`) — owns the rest of init + the loop until BACK.
9. Shutdown (`:117-120`): `Shaders::shutdown()`, `Textures::shutdown()`, `g_pd3dDevice->Release()`,
   `g_pD3D->Release()`.

The main loop is **inside `Minecraft::run()`** (`Minecraft.cpp:392-518`), not Main.cpp:
- `run()` calls `startGame()` (`Minecraft.cpp:396` -> body `185-244`), which does the
  display/render-engine/font/main-menu init that LWJGL would have triggered. `startGame()`
  replaces `Display.create()` with a comment (`Minecraft.cpp:198-199`) since the device already
  exists, and sets the fixed 1280x720 backbuffer.
- Loop condition: `while(running && (m_mcApplet == NULL))` (`Minecraft.cpp:408`).
- Tick cadence: a `Timer` is updated (`m_timer->updateTimer()`, `:417/:420`), then ticks are run
  `for(var6 = 0; var6 < m_timer->m_elapsedTicks; ++var6) runTick()` (`:425-435`) — the classic
  fixed-20-TPS catch-up loop. `m_renderPartialTicks` carries the fractional remainder to the renderer.
- Render: `m_field_9243_r->func_4136_b(m_timer->m_renderPartialTicks)` (`:470`) — `m_field_9243_r`
  is the `EntityRenderer`; this call owns the actual scene + present.
- Framerate limiter: `if(m_gameSettings->m_limitFramerate) Sleep(5)` (`:458-460`), plus `Sleep(0)`
  (`:484`); FPS string built each second (`:504-508`).
- Dead Java `try/catch`/`Display`-poll branches are kept as `if(false){...}` (e.g. `:397`, `:411`,
  `:475`, `:510-511`) because `/EH` (exceptions) is off on the 360.
- Shutdown: `Minecraft::shutdown()` (`Minecraft.cpp:600-602`) just sets `running = false`; the
  actual D3D teardown is back in `Main.cpp:117-120` after `run()` returns.

### Present / swap location (important)
There is **no Present in the main loop body or Main.cpp**. The swap is inside the renderer:
- `EntityRenderer::func_4136_b` brackets the frame: `g_pd3dDevice->BeginScene()`
  (`EntityRenderer.cpp:384`) ... `EndScene()` + `Present(NULL,NULL,NULL,NULL)`
  (`EntityRenderer.cpp:459-460`). Comment at `:378-383` records the original-converter bug: it
  dropped both BeginScene and EndScene+Present, so frames rendered to the back buffer but were
  never flipped (screen frozen on the last loadScreen present = blue clear).
- Other manual present sites: `Minecraft::loadScreen` (`Minecraft.cpp:293`),
  `Minecraft::func_6238_a` FPS-graph debug (`:534`/uses present pattern around `:746`), and
  `LoadingScreenRenderer.cpp:145`. These do `EndScene(); Present(...); BeginScene();`.

## Global platform state

`Common.h` (`MinecraftAlpha/Common.h`) is the shared platform header; it `#include`s `stdafx.h`
and declares the globals every TU sees:
- D3D globals (`extern`, defined in `Main.cpp:30-35`): `g_pD3D` (`Common.h:7`), `g_pd3dDevice`
  (`:8`), and texture handles `g_terrainTexture`, `g_guiTexture`, `g_charTexture`,
  `g_defaultTexture` (`:9-12`). `stdafx.h:39-40` re-declares `g_pD3D`/`g_pd3dDevice` so even TUs
  that only include `stdafx.h` can reach the device.
- Screen size macros: `SCREEN_WIDTH 1280`, `SCREEN_HEIGHT 720` (`Common.h:16-17`); `DEG_TO_RAD`
  (`:15`).
- `Random` (`Common.h:20-37`) — NOTE: this is a **64-bit LCG, NOT Java's `java.util.Random`**
  (constants `6364136223846793005`/`1442695040888963407`). It does not match Java sequences. The
  NEW converter's `runtime.py RANDOM_H` (`j2ctool/runtime.py:12-43`) is a faithful 48-bit Java LCG —
  these two will generate **different worlds** (see Pitfalls).
- Timing helpers: `GetNanoTime()` via `QueryPerformanceCounter` (`Common.h:40-45`).
- Debug logging: `DbgPrintF` -> `OutputDebugStringA` + `DbgPrint` (the Nexia360 log channel)
  (`Common.h:48-58`); `DbgHeapChk` (`Common.h:65-72`) pokes the real process heap with a
  malloc/free to bracket `RtlValidateHeap` corruption (capped at 4000 calls). These confirm an
  active heap-corruption hunt.
- Java-shim helpers used everywhere: `toStr(...)` overloads (`:79-88`), `javaStartsWith`/`EndsWith`/
  `IndexOf`/`ToLowerCase`/`Trim` (`:91-114`), `_vecRemoveAt`/`_vecRemoveObj` (`:121-130`),
  `signumf` (`:75`). (The NEW converter folds these into `JavaString`/`JList`/`JavaRT` instead.)

`stdafx.h` (`MinecraftAlpha/stdafx.h`) is the precompiled-header root. Includes: `<xtl.h>`,
`<xboxmath.h>`, `<d3d9.h>`, `<d3dx9.h>`, `<xaudio2.h>`, `<xgraphics.h>` (`:10-15`), plus C/C++ std
headers (`:17-28`). Defines `typedef unsigned char byte;` and `#define null NULL` (`:35-36`) — the
Java-token shims. Silences signed/unsigned + float-truncation warnings (`:8`, `:30-32`).

## D3D9 device setup / present (concrete API calls)

Device creation — `InitD3D()` (`Main.cpp:40-63`):
```
g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);              // :41
D3DPRESENT_PARAMETERS d3dpp; ZeroMemory(...);           // :44-45
  BackBufferWidth/Height      = 1280 / 720              // :46-47  (SCREEN_WIDTH/HEIGHT)
  BackBufferFormat            = D3DFMT_X8R8G8B8         // :48
  BackBufferCount             = 1                       // :49
  MultiSampleType             = D3DMULTISAMPLE_NONE     // :50
  SwapEffect                  = D3DSWAPEFFECT_DISCARD   // :51
  hDeviceWindow               = NULL                    // :52  (no HWND on 360)
  Windowed                    = FALSE                   // :53  (always fullscreen)
  EnableAutoDepthStencil      = TRUE                    // :54
  AutoDepthStencilFormat      = D3DFMT_D24S8            // :55  (24-bit depth + 8 stencil)
  FullScreen_RefreshRateInHz  = 60                      // :57
  PresentationInterval        = D3DPRESENT_INTERVAL_IMMEDIATE  // :58  (no vsync)
g_pD3D->CreateDevice(0, D3DDEVTYPE_HAL, NULL,
    D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice);   // :60-61
```
Adapter 0, HAL, hardware vertex processing. No `D3DCREATE_BUFFER_2_FRAMES` or multi-buffering tweaks.

Default render state — `ApplyDefaultRenderState()` (`Main.cpp:65-74`): `D3DRS_ZENABLE`/`ZWRITEENABLE`
TRUE, `D3DRS_ZFUNC = D3DCMP_LESSEQUAL`, `D3DRS_CULLMODE = D3DCULL_CW`, `D3DRS_ALPHABLENDENABLE`
FALSE, sampler 0 MIN/MAG = `D3DTEXF_POINT`, MIP = `D3DTEXF_NONE`.

Present — `g_pd3dDevice->Present(NULL, NULL, NULL, NULL)` paired with `BeginScene()`/`EndScene()`,
located in the renderer (`EntityRenderer.cpp:384, 459-460`), loadScreen (`Minecraft.cpp:293`),
FPS graph (`Minecraft.cpp:534`+`746`), and `LoadingScreenRenderer.cpp:145`.

Clears use `g_pd3dDevice->Clear(...)` with the Minecraft sky color `D3DCOLOR_XRGB(128,204,255)`
(e.g. `Minecraft.cpp:250, 534`; `EntityRenderer.cpp:444, 453`).

There is **no device-lost / reset (`D3DERR_DEVICELOST` / `TestCooperativeLevel` / `Reset`) handling
anywhere** — acceptable on a dedicated console (no alt-tab) but worth noting.

Shaders (the GL fixed-function replacement) — `Shaders.h` (`MinecraftAlpha/Shaders.h`): textured +
untextured VS/PS (`s_texturedVS/PS`, `s_untexturedVS/PS`), a `TesselatorVertex {x,y,z; DWORD color;
u,v}` (`:6-10`), world/view/proj matrix setters, fog, gamma, and a 360-specific `setAlphaRef`
(Xenos ignores `D3DRS_ALPHATESTENABLE`, so alpha cutout is done with `clip()` in the PS — `:28-34`).
`Shaders::init()` is called from `Main.cpp:80`.

## Build / project config

Solution: `MinecraftAlpha.sln` (`MinecraftAlpha.sln`). One project,
`MinecraftAlpha\MinecraftAlpha.vcxproj`, GUID `{61D6C58E-65BB-4CEC-9DCF-AA2C3C1FCE0D}`. VS 2010
format (`.sln:2-3`). All platforms are **`Xbox 360`** (`.sln:7-14`).

Six configurations (`.sln:8-13`): `CodeAnalysis`, `Debug`, `Profile`, `Profile_FastCap`,
`Release`, `Release_LTCG` — all `|Xbox 360`. `ConfigurationType` = `Application` for all
(`.vcxproj` `<ConfigurationType>Application</ConfigurationType>`).

Preprocessor defines (`.vcxproj`):
- Debug / CodeAnalysis: `_DEBUG;_XBOX`
- Profile: `NDEBUG;_XBOX;PROFILE`
- Profile_FastCap: `NDEBUG;_XBOX;PROFILE;FASTCAP`
- Release: `NDEBUG;_XBOX`
- Release_LTCG: `NDEBUG;_XBOX;LTCG`

Optimization: Debug/CodeAnalysis = `Disabled`; Profile = `Full`; Release/Release_LTCG = `Full`.
`WholeProgramOptimization` true (LTCG). `RuntimeLibrary`: `MultiThreadedDebug` (debug),
`MultiThreaded` (release/profile). `PrecompiledHeader` = `Use` for all configs (PCH =
`stdafx.h`; output `$(OutDir)$(ProjectName).pch`). `DebugInformationFormat` = `ProgramDatabase`,
`GenerateDebugInformation` true (even in release).

Link libraries (`<AdditionalDependencies>`), per config (debug `*d`, instr `*i`, ltcg `*ltcg`):
- Debug: `xapilibd.lib; d3d9d.lib; d3dx9d.lib; xgraphicsd.lib; xboxkrnl.lib; xnetd.lib;
  xaudiod2.lib; xactd3.lib; x3daudiod.lib; xmcored.lib; xbdm.lib; vcompd.lib`
- Release: `xapilib.lib; d3d9.lib; d3dx9.lib; xgraphics.lib; xboxkrnl.lib; xnet.lib; xaudio2.lib;
  xact3.lib; x3daudio.lib; xmcore.lib; vcomp.lib; xbdm.lib`
- Release_LTCG swaps in `d3d9ltcg.lib; xact3ltcg.lib; x3daudioltcg.lib; xmcoreltcg.lib`.
- `vcomp(d).lib` = OpenMP; `xbdm.lib` = debug monitor; `xnet` present but networking is stubbed.

XDK paths: **no `<AdditionalIncludeDirectories>` / `<AdditionalLibraryDirectories>` are set** — the
project relies on the XDK's default include/lib environment (SDK installed at
`C:/Program Files (x86)/Microsoft Xbox 360 SDK`; `XEDK` env var). The libs above resolve from the
XDK lib path automatically.

File organization: the ~379 `.cpp` are flat (no folders) in `MinecraftAlpha\`, listed as
`<ClCompile Include="..."/>` and `<ClInclude Include="..."/>` item groups (e.g.
`.vcxproj:16384002+` for headers, `Main.cpp` at `.vcxproj:16450169`, `Minecraft.cpp` at `:16450180`).
NOTE: the `.vcxproj` is ~512KB and padded with huge runs of blank lines between sections
(an artifact, not content) — grep for the tag, don't read sequentially.

## Mapping to the new converter

The NEW game code calls LWJGL-style symbols (stubs in `j2ctool/runtime.py`). For app startup the NEW
platform layer must turn these from no-ops into a real D3D9 implementation:

- **`Display`** (`runtime.py:839-848`, `_ns_stub`) — the big one. NEW `Display::create()` must do
  what OLD `InitD3D()`+`ApplyDefaultRenderState()`+`startGame()` do today: create `g_pD3D`/
  `g_pd3dDevice` (1280x720, `D3DFMT_X8R8G8B8`, `D3DFMT_D24S8`, fullscreen, immediate present),
  set the default render state. `Display::update()`/`swapBuffers()` must become the
  `EndScene()`+`Present()` (and the NEW layer should own `BeginScene` at frame top — game code
  won't call it). `Display::setDisplayMode`/`setFullscreen`/`setTitle` are no-ops on 360.
  `isCloseRequested()` should return false (or map to the Guide/BACK button). `getWidth/getHeight`
  return 1280/720. `getDesktopDisplayMode`/`getDisplayMode` return a `DisplayMode` reporting
  1280x720x32@60.
- **`DisplayMode`** (`runtime.py:836-838`) — make `getWidth/getHeight/getFrequency/getBitsPerPixel`
  return the real backbuffer values (currently `0`).
- **`Sys`** (`runtime.py:977-979`) — `Sys::getTime()` must return a real tick (OLD uses
  `GetTickCount()`/`GetNanoTime()` via `QueryPerformanceCounter`, `Common.h:40-45`) and
  `getTimerResolution()` the matching frequency; `Timer`/the tick loop depend on these.
- **`GLContext`** (`runtime.py:980-1000`) — `getCapabilities()` returns a `ContextCapabilities`
  with all flags false today. For 360, report the caps the game branches on (e.g. leave
  `GL_ARB_occlusion_query`/`GL_ARB_vertex_buffer_object` false so the game takes the simple paths,
  or wire VBO to D3D vertex buffers if desired). The OLD code sets `m_glCapabilities = NULL`
  (`Minecraft.cpp:225`) and side-steps this.
- **`BufferUtils`** (`runtime.py:864-866`) + `FloatBuffer/IntBuffer/ByteBuffer`
  (`runtime.py:767-787`) — currently return `0`/no-op. The `Tesselator` and texture upload paths
  need real backing memory (OLD uses a `TesselatorVertex` struct + D3D dynamic vertex buffer via
  `Shaders`). `createFloatBuffer/IntBuffer/ByteBuffer` must return buffers whose `operator void*`
  hands the D3D draw path a real pointer.
- **`GL11`/`GL12`/`GL15`/`GLU`** (`runtime.py:538-674, 849-857`) — out of immediate startup scope
  but these are where the OLD direct-D3D calls have to live in the NEW design: each `glBegin/glEnd/
  glVertex*/glTexCoord*` -> Tesselator accumulation; `glClear` -> `Clear`; matrix calls -> the
  `Shaders` matrix stack; `glBindTexture` -> `SetTexture`. The OLD `Shaders` + `Tesselator` classes
  are the reusable target for this.

Driving the NEW `Minecraft` class from a NEW `Main.cpp`: the OLD pattern (`Main.cpp:76-121`) is the
template, but the NEW Main should be thinner. Recommended NEW flow:
1. `void main()` (no `WinMain`).
2. `Display::create()` (does device + render-state setup internally).
3. `Sys`/timer init (or fold into `Display::create`).
4. Call the `initStatics()` initializers in the same dependency order (`Main.cpp:93-105`) — the NEW
   converter still emits `initStatics()` and (per OLD comment, `Main.cpp:89-92`) does not call them,
   so Main must. **Open question: confirm the NEW converter doesn't already auto-invoke these.**
5. `new Minecraft(); set displayWidth/Height; run()` — but in the faithful build, `run()` should
   call `Display.update()`/`swapBuffers()` itself (the present moves out of `EntityRenderer` and
   into the platform layer), so the NEW Main does NOT need the buried-Present workaround.
6. On `run()` return: `Display::destroy()` (releases device + shaders + textures).

## Known bugs / pitfalls

- **`Random` mismatch (world-gen divergence):** OLD `Common.h:20-37` is a 64-bit LCG, NOT
  `java.util.Random`. The NEW `runtime.py:12-43` is the faithful 48-bit Java LCG. Same seed ->
  different worlds between OLD and NEW. Expected and arguably a NEW improvement, but flag it when
  comparing behavior.
- **Active heap-corruption hunt:** `DbgHeapChk` (`Common.h:65-72`) and the `[HEAPCHK]`/`[FRAME]`/
  `[R]` `DbgPrintF` probes peppered through `Minecraft.cpp`/`EntityRenderer.cpp` (e.g.
  `Minecraft.cpp:469-472`, `EntityRenderer.cpp:416-427`) show an unresolved `RtlValidateHeap`
  corruption was being chased. The corruption root cause may still be live.
- **No device-lost/reset handling:** no `TestCooperativeLevel`/`Reset` anywhere. Fine for a fixed
  console, but the NEW layer inherits the same gap.
- **Unbounded->bounded lighting drain hack:** `Minecraft.cpp:441-452` caps the per-frame skylight
  drain to 8 batches/frame to avoid a hard freeze ("0 fps, 0 chunk updates"). This diverges from
  Java semantics (lighting lags a few frames). The NEW faithful port will reintroduce Java's
  unbounded drain and hit the same freeze unless this cap is re-applied in the platform/world layer.
- **Present scattered through game code (anti-pattern):** OLD puts `BeginScene`/`EndScene`/`Present`
  in `EntityRenderer`, `loadScreen`, the FPS graph, and `LoadingScreenRenderer` — 4+ sites
  (`EntityRenderer.cpp:384/459-460`, `Minecraft.cpp:293/534`, `LoadingScreenRenderer.cpp:145`).
  The NEW design must consolidate this into `Display.update()`/`swapBuffers()` or it will desync.
- **`Minecraft::func_4007_a` was de-pure-virtualized:** the AWT/applet subclass normally implemented
  it; OLD made it concrete (`Minecraft.h:80`) so `new Minecraft()` is constructible. NEW Main must
  do the same or supply a concrete subclass.
- **`/EH` (exceptions) is off:** all Java `try/catch` became `if(false){...}` dead branches
  (`Minecraft.cpp:397, 430, 510-511`). Any NEW code relying on thrown exceptions for control flow
  will silently not work; the converter must keep lowering them to status checks.
- **Vcxproj is null/blank-padded (~512KB):** tooling-hostile; if regenerating the NEW project,
  produce a clean vcxproj.

## Open questions

1. Does the NEW converter already call the `initStatics()` blocks during static init, or must the
   NEW `Main.cpp` call them by hand (as OLD does at `Main.cpp:93-105`)? Confirm the full dependency
   order is still correct for the alpha corpus.
2. Should the NEW platform layer own `BeginScene` at frame top automatically, or expose it (the
   faithful Java has no concept of it — leaning toward "platform owns it" inside `Display.update`)?
3. Vsync: OLD uses `D3DPRESENT_INTERVAL_IMMEDIATE` + `Sleep(5)` framerate limiter. Should the NEW
   layer honor `Display.setVSyncEnabled` / `Display.sync(fps)` instead?
4. `GLContext` capabilities: which ARB/EXT flags should report `true` on 360 (e.g. should VBO map to
   D3D vertex buffers, or stay false to force the simple immediate path)?
5. Asset path convention: OLD loads from `game:\Content\...` (`Main.cpp:84-87`). Is that the same
   deployment root the NEW build will use, and should the NEW `Texture`/`File` layer normalize the
   Java `/title/mojang.png`-style paths to `game:\Content\...`?
6. Confirm the OLD `Random` divergence is acceptable (NEW uses faithful Java RNG) for whatever
   save/seed compatibility the user cares about.
