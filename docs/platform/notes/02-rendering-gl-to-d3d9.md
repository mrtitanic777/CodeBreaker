# 02 — Rendering: GL → D3D9 translation layer (Xbox 360)

> Scope: how the NEW transpiler's faithful LWJGL GL calls (`GL11::*`, `GL12::*`, `GLU::*`,
> declared as symbol-only stubs in `J2CTool/j2ctool/runtime.py`) get implemented as a
> real Direct3D 9 immediate-mode emulator backed by `g_pd3dDevice`.
>
> All "OLD port" file:line references are to
> `C:/Users/USERNAME/Documents/Visual Studio 2010/Projects/MinecraftAlpha/MinecraftAlpha/`.
> The OLD port is the proven-on-hardware reference: its render classes were translated
> directly to D3D9 (GL was pre-inlined), so it shows the *answer* for every GL idiom.
> The NEW code keeps the GL calls, so we must build the emulator that the OLD port baked in by hand.

---

## Overview

The OLD port and the NEW code solve the same problem two different ways:

| | OLD port (reference) | NEW transpiler |
|---|---|---|
| Geometry | `Tesselator` class emits `DrawPrimitiveUP` directly | faithful `Tessellator` (Java) batches into a `rawBuffer`, calls `glDrawArrays` |
| Matrices | converter inlined GL stack → `Shaders::s_world/view/projMatrix` + `D3DXMatrix*` | code still calls `glPushMatrix/glTranslatef/glRotatef/...` |
| Pipeline | **vs_3_0 / ps_3_0 shaders** (Xenos requires a VS; fixed-function is gone) | code assumes GL fixed-function |
| Textures | int-handle registry (`Textures`) over `D3DXCreateTextureFromFileExA` | code calls `glGenTextures`/`glBindTexture`/`glTexImage2D` |

Key architectural fact discovered: **the Xbox 360 (Xenos) D3D9 has no fixed-function pipeline.**
The OLD port proves this — every batch must have a vertex shader bound or the runtime rejects
the draw ("ERR[D3D]: A vertex shader must be set", `Tesselator.cpp:40-50`). So the new GL
emulator cannot be a thin `SetRenderState` shim; it must own a small shader pair plus a
manual matrix stack and feed the WVP as a shader constant.

Second key fact: **the NEW code's geometry hot path is the vertex-array path, not glBegin/glVertex.**
A histogram of the NEW project's GL calls shows **no `glBegin/glVertex/glColor3f-in-immediate/glEnd`**
at all — Minecraft Alpha funnels all geometry through its `Tessellator`, which uses
`glVertexPointer` / `glColorPointer` / `glTexCoordPointer` / `glEnableClientState` /
`glDrawArrays(GL_TRIANGLES, …)` (`overrides/Tessellator.cpp:82-156`). The GL-call surface the
emulator must actually implement is ~52 distinct functions (see mapping table); the immediate-mode
emulator is really a **vertex-array → DrawPrimitiveUP** emulator.

---

## Immediate mode (glBegin/glVertex/...) → D3D9

### What the OLD port does (the model to copy)

`Tesselator` (`Tesselator.h`, `Tesselator.cpp`) is the heart. Vertex format
(`Shaders.h:6-10`):

```cpp
struct TesselatorVertex { float x, y, z; DWORD color; float u, v; };  // 24 bytes
```

- Accumulates vertices into `m_vertices[MAX_VERTICES]` (`MAX_VERTICES = 262144`),
  one `vertex()`/`vertexUV()` call at a time, carrying the current color + UV
  (`Tesselator.cpp:115-127`). This is exactly glColor/glTexCoord "current state" + glVertex.
- `flush()` (`Tesselator.cpp:38-86`) is the draw:
  1. Bind shader by content: `Shaders::useTexturedShader()` if any UV was set, else
     `useUntexturedShader()` — each also sets the vertex declaration.
  2. `Shaders::updateShaderConstants()` uploads the current WVP.
  3. **Quads → triangle list, by hand**, with *reversed winding* (`0,2,1` and `0,3,2`)
     for D3D CW culling (`Tesselator.cpp:53-69`).
  4. `g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, out/3, triVerts, sizeof(TesselatorVertex))`.
- Color packs to `D3DCOLOR_ARGB` immediately (`Tesselator.cpp:90-111`).
- `normal()` is a **no-op** (`Tesselator.cpp:112`) — Minecraft bakes lighting/AO into
  vertex color, so normals/lighting are unused.

The 2D GUI path uses the exact same Tesselator: see `Gui::drawRect` / `drawGradientRect` /
`drawTexturedModalRect` (`Gui.cpp:20-89`) — `init()` … `vertex()`/`vertexUV()` … `flush()`.

### Vertex declaration (the FVF, but as a decl because shaders)

`Shaders::init()` (`Shaders.cpp:191-198`):

```cpp
D3DVERTEXELEMENT9 decl[] = {
  { 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
  { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
  { 0, 16, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
  D3DDECL_END()
};
```

i.e. the FVF-equivalent is `XYZ | DIFFUSE | TEX1` at offsets 0/12/16. (FontRenderer even sets
the literal FVF `D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1`, `FontRenderer.cpp:135`, though the
shader path uses the declaration.)

### How the NEW Tessellator differs (and what that means for the emulator)

The NEW `Tessellator` (`overrides/Tessellator.cpp`) is faithful Java: it packs an **interleaved
8-int (32-byte) stride** into `rawBuffer` per vertex (`addVertex`, `:246-285`):

| ints | bytes | field |
|---|---|---|
| 0,1,2 | 0..11 | x, y, z (floats, via `Float::floatToRawIntBits`) |
| 3,4 | 12..19 | u, v (floats) |
| 5 | 20..23 | color (RGBA packed `a<<24|b<<16|g<<8|r`) |
| 6 | 24..27 | normal (packed bytes) |
| 7 | 28..31 | (pad) |

`draw()` (`:82-156`) sets the GL pointers at these byte offsets (`glTexCoordPointer` @ float pos 3,
`glColorPointer` @ byte 20, `glVertexPointer` @ float pos 0, stride 32) and calls
`glDrawArrays(GL_TRIANGLES, 0, vertexCount)`. It also does the **quad→triangle expansion in
`addVertex` itself** when `drawMode==7` (GL_QUADS) and `s_convertQuadsToTriangles` (`:249-265`),
so by the time `glDrawArrays` is called, geometry is already a triangle list.

**Emulator implication:** the heavy path is "interpret these three client-array pointers + stride
+ offsets, gather into a `TesselatorVertex`-shaped buffer, `DrawPrimitiveUP`." Two practical
options:

1. **Per-call gather** in `glDrawArrays`: read the bound vertex/color/texcoord pointers (saved by
   `glVertexPointer`/etc.), walk `count` verts at the given stride, build a temp
   `{xyz, D3DCOLOR, uv}` array, bind textured/untextured shader, upload WVP, `DrawPrimitiveUP`.
   This mirrors `Tesselator::flush` exactly and reuses the OLD port's decl + shaders verbatim.
2. Note the NEW color packing is `0xAABBGGRR` (R in low byte) whereas D3DCOLOR is `0xAARRGGBB`.
   The emulator's gather must **swizzle R↔B** (or declare the color element as
   `D3DDECLTYPE_UBYTE4` and swizzle in the VS). The OLD `Tesselator` sidesteps this by packing
   `D3DCOLOR_ARGB` directly; the faithful path does not, so handle it.

A classic immediate-mode `glBegin/glVertex` accumulator is still worth providing for completeness
(some hand overrides like FontRenderer build quads directly), but it is **not** on the Tessellator
hot path for Alpha.

---

## Matrix stack → D3D9 matrices

There is no D3D fixed-function matrix stack on Xenos; the OLD port keeps **three D3DXMATRIX
statics** in `Shaders` and multiplies them into a WVP each draw.

`Shaders.cpp`:
- State: `s_worldMatrix`, `s_viewMatrix`, `s_projMatrix` (`:178-180`), all identity at init
  (`:206-208`).
- `updateShaderConstants()` (`:232-251`):
  ```cpp
  D3DXMatrixMultiply(&wvp, &s_world, &s_view);
  D3DXMatrixMultiply(&wvp, &wvp, &s_proj);
  D3DXMatrixTranspose(&wvp, &wvp);              // HLSL expects column-major
  g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&wvp, 4);   // c0..c3
  ```
- The VS does `o.pos = mul(float4(i.pos,1), g_WorldViewProj)` (`Shaders.cpp:21`).

### How GL stack ops were inlined (the pattern to reproduce)

The converter turned each GL matrix op into a `D3DXMatrix*` build + multiply into the world
matrix. From `EntityRenderer.cpp`:

- `glLoadIdentity` → `D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id);` (`:293`, `:341`)
- `glTranslatef` → `D3DXMatrixTranslation(&_t,…); D3DXMatrixMultiply(&_w,&_t,&getWorldMatrix()); setWorldMatrix(_w);` (`:204`, `:263`, `:296`)
- `glRotatef` → `D3DXMatrixRotationAxis(&_r,&axis,deg*DEG_TO_RAD); … multiply …` (`:176`, `:257-271`)
- `glScalef` → `D3DXMatrixScaling(&_s,…); … multiply …` (`:329`)
- `glPushMatrix`/`glPopMatrix` → save/restore via a local `D3DXMATRIX _saved = getWorldMatrix();`
  … `setWorldMatrix(_saved);` (`:348`, `:364`)

Note the **pre-multiply** order (`_t * world`), commented "camera/view composes in view frame"
(`:205` etc.) — this matches GL's right-multiply semantics where `glTranslate` post-multiplies the
current matrix.

**Emulator implication:** implement a real `std::vector<D3DXMATRIX>` stack per matrix mode.
- `glMatrixMode(GL_MODELVIEW|GL_PROJECTION)` selects which stack is "current".
- `glLoadIdentity` → top = identity. `glPushMatrix`/`glPopMatrix` → push copy / pop.
- `glTranslatef/glRotatef/glScalef/glMultMatrix` → build a `D3DXMATRIX`, `top = build * top`
  (pre-multiply, GL order). `glRotatef` takes degrees → multiply by `DEG_TO_RAD`.
- On draw: `world = modelviewTop` (GL has no separate world/view; modelview = world*view),
  `proj = projectionTop`, build+transpose WVP, upload to `c0`. The OLD port keeps world & view
  separate but multiplies them anyway, so collapsing GL's modelview into `s_worldMatrix` and
  leaving `s_viewMatrix` identity is equivalent and simplest.

---

## Textures

### Registry + creation

OLD port `Textures` class (`Textures.cpp`) is an int-handle registry — exactly the model for
`glGenTextures`/`glBindTexture`:

- `load(path)` (`:25-95`): de-dupes by path, then
  `D3DXCreateTextureFromFileExA(dev, path, D3DX_DEFAULT, D3DX_DEFAULT, 1 /*1 mip*/, 0,
  D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0 /*no color key*/, …)`.
  - **1 mip level, no mips** — MC art is a pixel atlas; mips blur it.
  - **`D3DX_FILTER_NONE`** on load — no resampling.
  - **No color key (the `0` arg)** — transparency comes from each PNG's real alpha channel.
    `Textures.cpp:51-54` calls out the bug: a `0xFF000000` color key forced opaque-black texels
    (heart/hotbar outlines, lava) to alpha 0 → corrupted HUD.
- `getById(id)` / `bind(id)` → `SetTexture(0, …)` (`:97-108`).
- Animated textures (`TextureFX`) are ticked but the per-tile `LockRect` re-upload is deferred
  (`RenderEngine.cpp:101-107`) — i.e. `glTexSubImage2D` is effectively a no-op in the reference.

### Binding (glBindTexture → SetTexture)

`RenderEngine::bindTexture(id)` → `g_pd3dDevice->SetTexture(0, Textures::getById(id))`
(`RenderEngine.cpp:113-117`). All textures live on stage 0; Xenos's PS always samples `s0`.

> Pitfall the OLD port documents (`Textures.cpp:58-83`, `RenderEngine.cpp:26-36`): if a path
> fails to resolve, **do not leave `SetTexture(0, NULL)`** — the previous texture stays bound and
> the quad shows a leaked atlas (the infamous "Steve skin / terrain.png stuck on the vignette"
> bug). The OLD port retries with the basename under Content root. The emulator's
> `glBindTexture(0)`/unbind should bind a known 1×1 white texture, not NULL.

### Pixel-art filtering (the important one)

Set once globally in `Main.cpp::ApplyDefaultRenderState` (`:71-73`) and re-asserted in
FontRenderer (`:127-128`):

```cpp
SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);   // nearest
SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);   // nearest
SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);    // no mips
```

This is the D3D9 equivalent of MC's
`glTexParameteri(GL_TEXTURE_MIN/MAG_FILTER, GL_NEAREST)`. **Map `GL_NEAREST → D3DTEXF_POINT`,
`GL_LINEAR → D3DTEXF_LINEAR`** in `glTexParameteri`. Default everything to POINT.
Address mode (`GL_TEXTURE_WRAP_S/T` = `GL_REPEAT`/`GL_CLAMP`) → `D3DSAMP_ADDRESSU/V` =
`D3DTADDRESS_WRAP`/`CLAMP` (the OLD port leaves default WRAP; the GL2 `GL_CLAMP_TO_EDGE` enum
exists in the GL12 stub).

---

## Render states

All via `SetRenderState`. Defaults set in `Main.cpp::ApplyDefaultRenderState` (`:65-74`).

### Blend (glBlendFunc / glEnable(GL_BLEND))

`SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE/FALSE)` + `D3DRS_SRCBLEND`/`D3DRS_DESTBLEND`.
The dominant pair is `D3DBLEND_SRCALPHA` / `D3DBLEND_INVSRCALPHA`
(`Gui.cpp:28-29,53-54`, `EntityRenderer.cpp:525-526`, `FontRenderer.cpp:130-131`).
GL→D3D factor map: `GL_SRC_ALPHA→D3DBLEND_SRCALPHA`, `GL_ONE_MINUS_SRC_ALPHA→D3DBLEND_INVSRCALPHA`,
`GL_ONE→D3DBLEND_ONE`, `GL_DST_COLOR→D3DBLEND_DESTCOLOR`, etc.

### Depth (glDepthFunc / glDepthMask / glEnable(GL_DEPTH_TEST))

`D3DRS_ZENABLE`, `D3DRS_ZWRITEENABLE` (= glDepthMask), `D3DRS_ZFUNC` (= `D3DCMP_LESSEQUAL` for
`GL_LEQUAL`) (`Main.cpp:66-68`, `EntityRenderer.cpp:490-492`).

> Pitfall (`EntityRenderer.cpp:486-492`): the 2D GUI passes leave `ZENABLE=FALSE`; the world pass
> must **re-enable Z test + Z write + LEQUAL every frame** or far geometry paints over near.

### Cull (glCullFace / glEnable(GL_CULL_FACE))

`D3DRS_CULLMODE`. Default `D3DCULL_CW` (`Main.cpp:69`). The world passes flip to
`D3DCULL_CCW` (`EntityRenderer.cpp:485,551`) and the 2D GUI uses `D3DCULL_NONE`
(`EntityRenderer.cpp:615`, `func_905_b`). This is tangled up with the X-mirror fix (below).

### Fog (glFog* → shader, NOT D3D9 fixed-function fog)

Xenos has no fixed-function fog. The OLD port computes fog **in the shaders**:
- `Shaders::setFogEnabled/setFogColor/setFogDensity` set statics
  (`Shaders.cpp:259-262`), uploaded as `g_FogParams`/`g_FogColor` constants
  (`Shaders.cpp:239-243`).
- VS computes `o.fog = saturate((end - pos.w)/(end-start))` (`Shaders.cpp:25`); PS does
  `lerp(fogColor, texel.rgb, vis)` (`Shaders.cpp:38-40`).
- `glFogf(GL_FOG_START/END/DENSITY)`, `glFogi(GL_FOG_MODE, GL_LINEAR/GL_EXP)`,
  `glFog(GL_FOG_COLOR, …)` therefore map to `Shaders::setFog*` calls, not render states.
  Caller examples: `EntityRenderer::func_4140_a` (`:675-728`, "Fog mode = GL_LINEAR — handled by
  shader"), `setFogEnabled(true/false)` toggled around world passes (`:500,560,565`).

### ALPHA TEST for cutouts (leaves/glass/crops/items) — special on Xenos

**Xenos ignores `D3DRS_ALPHATESTENABLE`.** The many `SetRenderState(D3DRS_ALPHATESTENABLE, …)`
calls in the OLD port (`EntityRenderer.cpp:519,522`, `Gui.cpp:52,67`, `RenderGlobal.cpp`,
`ItemRenderer.cpp`, `Minecraft.cpp:219-221`) are **vestigial converter output** — they don't do
anything on hardware. Real cutout is done in the **pixel shader** via `clip()`:

- PS: `clip(texel.a - g_FogParams.z)` (`Shaders.cpp:37`) where `.z = s_alphaRef`
  (`Shaders.cpp:184,238-239`).
- `Shaders::setAlphaRef(0.5f)` = hard cutout (discard transparent texels → leaves/items render
  with holes); `setAlphaRef(0.0f)` = keep everything (soft-blended water/smoke/overlays)
  (`Shaders.h:28-34`).
- Cutout draws bracket themselves: `setAlphaRef(0.5)…draw…setAlphaRef(0.0)`
  (`EffectRenderer.cpp:102-135`, `ItemRenderer.cpp:69,173`).

**Emulator implication:** implement `glAlphaFunc(GL_GREATER, ref)` + `glEnable(GL_ALPHA_TEST)` by
setting the PS clip threshold (`setAlphaRef(ref)`), and `glDisable(GL_ALPHA_TEST)` by
`setAlphaRef(0)`. The FontRenderer's `D3DRS_ALPHAREF=128, D3DRS_ALPHAFUNC=GREATER`
(`FontRenderer.cpp:133-134`) is the GL semantic to emulate (ref 0.5), even though on hardware it's
the shader clip that actually cuts.

### Lighting / color material

`glEnable(GL_LIGHTING)`, `glLight*`, `glColorMaterial`, `RenderHelper::enable/disableStandardItemLighting`
are all **no-ops** in the reference (`RenderHelper.cpp:20-48`, "Lighting is shader-based … ").
Minecraft bakes per-vertex brightness into the diffuse color, so the emulator can treat the entire
GL lighting API as a no-op and just pass vertex color through (the VS does `o.col = i.col`).
`glColorMask` → `D3DRS_COLORWRITEENABLE` (used for anaglyph + fancy-water depth pre-pass,
`EntityRenderer.cpp:475-477,532-540`). `glShadeModel` → no-op (shaders interpolate smoothly).

---

## Projection: 3D world vs 2D GUI/HUD/text

Both are just `s_projMatrix`; the OLD port swaps which D3DX builder fills it.

### 3D world (gluPerspective)

`EntityRenderer::setupCameraTransform` (`:286-338`):

```cpp
D3DXMatrixPerspectiveFovLH(&_persp, toRad(fov), aspect, 0.05f, farPlane);
_persp._11 = -_persp._11;                 // X-mirror fix (see below)
Shaders::setProjectionMatrix(_persp);
```

> **The X-mirror fix** (`EntityRenderer.cpp:301-308`): Java/GL is right-handed (camera looks −Z);
> `…LH` looks +Z, mirroring the scene horizontally (every texture left-right reversed). Negating
> `proj._11` re-mirrors it. This **flips triangle screen winding**, which is why the world passes
> use `D3DCULL_CCW` while everything starts at `CW`. An emulator that implements `gluPerspective`
> with a proper **right-handed** `D3DXMatrixPerspectiveFovRH` would avoid the negate *and* the
> CW/CCW dance — strongly recommended for the rewrite (the OLD port's negate is a workaround, not a
> requirement).

### 2D GUI/HUD/text (glOrtho)

`EntityRenderer::func_905_b` (`:607-631`) — the setupOverlayRendering equivalent:

```cpp
SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);   // Y-flipped ortho reverses winding
SetRenderState(D3DRS_ZENABLE, FALSE);
D3DXMatrixOrthoOffCenterLH(&_ortho, 0.5, w+0.5, h+0.5, 0.5, 1000.0, 3000.0); // note l,r,b,t order: top-left origin
Shaders::setProjectionMatrix(_ortho);
// world = identity, then translate Z by +2000 to land inside LH [near,far]=[1000,3000]
```

> Two documented gotchas: (1) MC's `glOrtho(0,w,h,0,…)` is **Y-flipped** (origin top-left) → pass
> `bottom=h, top=0` to OffCenter, and add a +0.5px half-texel. (2) The view Z translate must be
> **+2000, not −2000** for LH (−2000 puts GUI behind the near plane → blank screen)
> (`EntityRenderer.cpp:619-630`). GUI quads are authored at z=0.

`glViewport` → `g_pd3dDevice->SetViewport(&D3DVIEWPORT9{x,y,w,h,0,1})`
(`EntityRenderer.cpp:441-442,481-482`). `glClear` → `g_pd3dDevice->Clear(…)`,
`glClearColor` feeds the Clear color (`EntityRenderer.cpp:443-444,484`).

---

## Fixed-function vs shaders on Xbox 360

**Shaders, mandatory.** Xenos D3D9 has no fixed-function vertex pipeline; a draw with no VS bound
is rejected at runtime (`Tesselator.cpp:40-45`). The OLD port ships **four** runtime-compiled
HLSL shaders (`Shaders.cpp:15-63`, compiled with `D3DXCompileShader` to `vs_3_0`/`ps_3_0`,
`:67-96`):

- **textured VS/PS** — transform, pass color+UV, compute linear fog factor; PS samples `s0`,
  multiplies by vertex color, `clip()`s for alpha cutout, applies fog lerp, then a gamma `pow`.
- **untextured VS/PS** — transform + vertex color + gamma only (used by `Gui::drawRect` etc.).

Constant register layout the emulator must honor (`Shaders.cpp:232-251`):

| reg | shader | meaning |
|---|---|---|
| VS c0–c3 | both VS | `g_WorldViewProj` (transposed) |
| VS c8 | textured VS | `g_FogParams` = (density/start, enabled, **alphaRef**, fogScale) |
| PS c0 | textured PS | `g_FogColor` |
| PS c1 | textured PS | `g_FogParams` (same vector) |
| PS c3 | both PS | `g_Gamma` (exponent; **must upload every draw** or uninitialized → black scene, `:244-250`) |

There's an offline-precompiled Xenos microcode path embedded too (`Shaders.cpp:98-171`) but it's
disabled — runtime compile is the live path and is what the rewrite should match (precompiled
bytecode silently desyncs from edited HLSL).

> Note the precompiled-vs-runtime history in `Shaders.cpp:5-14`: the PS originally used `discard`
> which faulted the 360 D3DX compiler; switching to `clip()` made runtime compilation work. Use
> `clip()`, not `discard`.

---

## GL → D3D9 mapping table

GL symbols are those the NEW code actually calls (from `j2ctool/runtime.py` `GL11_H` / `GL12` / `GLU`
stubs; counts are usages across `J2CTool/`).

| GL call / enum (new code) | D3D9 implementation (from OLD port) | ref |
|---|---|---|
| `glVertexPointer` / `glColorPointer` / `glTexCoordPointer` | save client-array ptr+stride+offset for the gather in `glDrawArrays` | NEW `Tessellator.cpp:100-135` |
| `glEnableClientState`/`glDisableClientState` | set flags (has color/tex) for the gather | NEW `:105-151` |
| `glDrawArrays(GL_TRIANGLES, 0, n)` | gather verts → bind shader+decl → upload WVP → `DrawPrimitiveUP(D3DPT_TRIANGLELIST, n/3, …)` | `Tesselator.cpp:79-82` |
| (implicit glBegin/quad) `GL_QUADS` mode | quad→tri expand (winding `0,2,1`/`0,3,2`) | `Tesselator.cpp:53-69` |
| `glColor4f/glColor3f` | pack `D3DCOLOR_ARGB`; store as "current" (swizzle R↔B vs Java packing) | `Tesselator.cpp:90-111` |
| `glTexImage2D` / `glGenTextures` | `D3DXCreateTexture` / registry alloc (1 mip, A8R8G8B8) | `Textures.cpp:42-95` |
| `glBindTexture(GL_TEXTURE_2D, id)` | `SetTexture(0, Textures::getById(id))` (never bind NULL) | `RenderEngine.cpp:113-117` |
| `glDeleteTextures` | release + free registry slot | `Textures.cpp:15-23` |
| `glTexSubImage2D` | `LockRect` upload (OLD port defers; ok as no-op initially) | `RenderEngine.cpp:101-107` |
| `glTexParameteri(MIN/MAG_FILTER, GL_NEAREST)` | `SetSamplerState(0, MIN/MAGFILTER, D3DTEXF_POINT)` | `Main.cpp:71-72` |
| `glTexParameteri(WRAP_S/T, GL_REPEAT/CLAMP)` | `SetSamplerState(0, ADDRESSU/V, WRAP/CLAMP)` | (default WRAP) |
| `glMatrixMode(GL_MODELVIEW/PROJECTION)` | select active D3DXMATRIX stack | `EntityRenderer.cpp:292,309` |
| `glLoadIdentity` | `D3DXMatrixIdentity(&top)` | `EntityRenderer.cpp:293` |
| `glPushMatrix`/`glPopMatrix` | push/pop `D3DXMATRIX` stack | `EntityRenderer.cpp:348,364` |
| `glTranslatef` | `D3DXMatrixTranslation`; `top = T*top` | `EntityRenderer.cpp:204-206` |
| `glRotatef(deg,ax,ay,az)` | `D3DXMatrixRotationAxis(…, deg*DEG_TO_RAD)`; `top = R*top` | `EntityRenderer.cpp:257-271` |
| `glScalef` | `D3DXMatrixScaling`; `top = S*top` | `EntityRenderer.cpp:329-331` |
| `glMultMatrix` | `top = M*top` | (same pattern) |
| `gluPerspective(fov,asp,n,f)` | `D3DXMatrixPerspectiveFovLH` (+`_11` negate) → `setProjectionMatrix` | `EntityRenderer.cpp:301-308` |
| `glOrtho(l,r,b,t,n,f)` | `D3DXMatrixOrthoOffCenterLH` (Y-flip, +0.5px, Z+2000) | `EntityRenderer.cpp:619-630` |
| `glViewport` | `SetViewport(D3DVIEWPORT9)` | `EntityRenderer.cpp:441-442` |
| `glClear(COLOR/DEPTH_BUFFER_BIT)` | `Clear(0,NULL, D3DCLEAR_TARGET\|D3DCLEAR_ZBUFFER, color,1,0)` | `EntityRenderer.cpp:484` |
| `glClearColor` / `glClearDepth` | feed `Clear()` color / z | `EntityRenderer.cpp:443-444` |
| `glEnable/glDisable(GL_BLEND)` | `SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE/FALSE)` | `Gui.cpp:26,38` |
| `glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)` | `D3DRS_SRCBLEND=SRCALPHA, DESTBLEND=INVSRCALPHA` | `Gui.cpp:28-29` |
| `glEnable/glDisable(GL_DEPTH_TEST)` | `D3DRS_ZENABLE` | `EntityRenderer.cpp:490` |
| `glDepthMask(bool)` | `D3DRS_ZWRITEENABLE` | `EntityRenderer.cpp:491,550` |
| `glDepthFunc(GL_LEQUAL)` | `D3DRS_ZFUNC=D3DCMP_LESSEQUAL` | `Main.cpp:68` |
| `glEnable/glDisable(GL_CULL_FACE)` + `glCullFace` | `D3DRS_CULLMODE` (CW/CCW/NONE) | `Main.cpp:69`, `EntityRenderer.cpp:485,615` |
| `glColorMask(r,g,b,a)` | `D3DRS_COLORWRITEENABLE` (bitmask) | `EntityRenderer.cpp:475-477` |
| `glEnable/glDisable(GL_ALPHA_TEST)` + `glAlphaFunc(GL_GREATER, ref)` | **PS clip**: `Shaders::setAlphaRef(ref)` / `setAlphaRef(0)` (D3DRS_ALPHATEST* is ignored on Xenos) | `Shaders.cpp:37`, `ItemRenderer.cpp:69,173` |
| `glEnable/Disable(GL_FOG)` | `Shaders::setFogEnabled(bool)` | `EntityRenderer.cpp:500,560` |
| `glFogf(GL_FOG_START/END/DENSITY)` | `Shaders::setFogDensity` / start-end constants | `EntityRenderer.cpp:688,699` |
| `glFogi(GL_FOG_MODE, GL_LINEAR/GL_EXP)` | shader fog mode (constant) | `EntityRenderer.cpp:687,709` |
| `glFog(GL_FOG_COLOR, buf)` | `Shaders::setFogColor(r,g,b)` | `EntityRenderer.cpp:677` |
| `glEnable(GL_TEXTURE_2D)` | pick textured vs untextured shader at flush | `Tesselator.cpp:46-50` |
| `glEnable(GL_LIGHTING)`, `glLight*`, `glColorMaterial`, `glNormal3f`, `glShadeModel`, `glEnable(GL_COLOR_MATERIAL)` | **no-op** (lighting baked into vertex color) | `RenderHelper.cpp:20-48`, `Tesselator.cpp:112` |
| `glNewList/glEndList/glCallList/glGenLists/glDeleteLists` | display lists: simplest = no-op (re-tessellate each frame) or record+replay a `DrawPrimitiveUP` buffer | NEW FontRenderer/WorldRenderer callers |
| `glPolygonOffset` + `GL_POLYGON_OFFSET_FILL` | `D3DRS_DEPTHBIAS` / `SLOPESCALEDEPTHBIAS` | (selection box) |
| `glLineWidth` | no D3D9 equivalent; ignore (lines rare) | — |
| `glReadPixels` | `GetRenderTargetData` + lock (screenshots; can stub) | — |
| `glGetString(GL_VENDOR/RENDERER/VERSION)` | return constant strings | stub returns "" |
| `glGetError` | return `0` (`D3D_OK`) | stub |
| `gluErrorString` | return "" | stub |
| `GL12::GL_CLAMP_TO_EDGE` | `D3DTADDRESS_CLAMP` | — |
| `GL15` / `ARBVertexBufferObject` (VBO) | unused — `s_tryVBO=false`, non-VBO path only | NEW `Tessellator.cpp:37,71` |

GL-call surface to implement: **~52 distinct `GL11` functions** in use, **+1 `GL12` enum**,
**+2 `GLU` funcs** (`gluPerspective`, `gluErrorString`). The 5 highest-frequency are
`glEnable`/`glDisable` (55 each), `glRotatef` (25), `glBindTexture` (25), `glTranslatef` (24),
`glColor4f` (24).

---

## Implementation plan for the new GL11/GL12/GLU stub

Replace the symbol-only stubs in `j2ctool/runtime.py` with a real translation unit (e.g.
`overrides/GLEmu.cpp` + the existing `GL11.h`/`GL12.h`/`GLU.h` declaring the same signatures) that
holds this state and drives `g_pd3dDevice`. Reuse the OLD port's `Shaders` + `Textures` + decl
verbatim — they already work.

**1. Global emulator state (one struct):**
- Matrix stacks: `std::vector<D3DXMATRIX> mvStack, projStack;` + `int matrixMode;`
  (top of each = current). On any draw, `world=mvTop`, `proj=projTop`.
- Current vertex attribs (immediate-mode accumulator, for hand-built quads):
  `D3DCOLOR curColor; float curU, curV; std::vector<TesselatorVertex> imm; int beginMode;`
- Client-array pointers (the Tessellator hot path): `const void *vtxPtr,*colPtr,*uvPtr;`
  `int vtxStride,colStride,uvStride; bool vtxOn,colOn,uvOn;`
- State cache (avoid redundant `SetRenderState`): blend on/off + factors, depth on/off +
  func + mask, cull mode, colormask, current texture id, sampler filter, plus
  `bool texEnabled` to pick shader.

**2. The two draw entry points:**
- `glDrawArrays(mode, first, count)` — the real workhorse. Gather `count` verts from `vtxPtr`
  (+col/uv at their offsets/strides) into a temp `TesselatorVertex[]`, **swizzle color R↔B**
  (Java packs `0xAABBGGRR`), choose `Shaders::useTexturedShader()`/`useUntexturedShader()` from
  `texEnabled`, `Shaders::updateShaderConstants()` (after pushing `world=mvTop`, `proj=projTop`),
  `DrawPrimitiveUP(D3DPT_TRIANGLELIST, count/3, …)`. (Alpha's Tessellator already expands quads to
  tris, so `GL_TRIANGLES` is the only mode seen; handle `GL_TRIANGLE_FAN`/`GL_LINE_STRIP` if a
  hand path needs them.)
- `glBegin/glVertex3f/glColor*/glTexCoord2f/glEnd` accumulator — for any hand override that still
  builds geometry directly. `glEnd` flushes exactly like the OLD `Tesselator::flush`
  (`Tesselator.cpp:38-86`), including the quad→tri winding swap.

**3. Matrix ops** — pre-multiply into the active stack top, as in §"Matrix stack" (degrees→rad for
`glRotatef`). `glMatrixMode` switches the active stack pointer.

**4. State ops** — `glEnable/glDisable` dispatch on the enum to the right `SetRenderState`
(blend/depth/cull) or to `texEnabled` (GL_TEXTURE_2D) / `Shaders::setFogEnabled` (GL_FOG) /
`Shaders::setAlphaRef` (GL_ALPHA_TEST) / no-op (lighting). Cache to skip redundant device calls.

**5. Projection/viewport/clear** — `gluPerspective`→`PerspectiveFovRH` (preferred: kills the
X-mirror + CW/CCW hack) or `PerspectiveFovLH`+`_11` negate to bit-match the reference;
`glOrtho`→`OrthoOffCenterLH` with the Y-flip + Z+2000 + half-texel; `glViewport`→`SetViewport`;
`glClear`/`glClearColor`→`Clear`.

**6. Textures** — `glGenTextures`/`glTexImage2D` create via the `Textures` registry (or
`D3DXCreateTexture` + `LockRect` if the bytes come from `glTexImage2D` directly);
`glBindTexture`→`SetTexture(0,…)` (bind 1×1 white, never NULL); `glTexParameteri`→sampler state
(`GL_NEAREST→POINT`).

**7. Display lists** — start with no-op `glNewList/glEndList` and have `glCallList` re-run the
recorded calls, or simplest: ignore lists and let callers re-tessellate each frame (the OLD
FontRenderer/WorldRenderer already draw immediately, `FontRenderer.cpp:166-176`).

**8. BeginScene/EndScene/Present** are owned by the frame driver (EntityRenderer equiv,
`EntityRenderer.cpp:384,459-460`), not the GL emulator — keep that boundary.

---

## Known bugs / pitfalls (observed in the OLD port)

1. **DrawPrimitiveUP per batch = high draw-call count.** No persistent vertex/index buffers; every
   `flush` copies through `alloca` + a fresh `DrawPrimitiveUP` (`Tesselator.cpp:56,80`). Fine for
   GUI, a perf trap for chunk geometry. The new layer should consider a real dynamic VB +
   `DrawPrimitive`, or honor the NEW Tessellator's (disabled) VBO path.
2. **`alloca` for triangle expansion** (`Tesselator.cpp:56`) — at `MAX_VERTICES=262144`, a full
   batch is ~9 MB of stack via `alloca` → stack overflow risk. Use a static scratch buffer.
3. **Gamma constant must be uploaded every draw** or the scene goes black
   (`Shaders.cpp:244-250`) — uninitialized PS `c3`.
4. **Never `SetTexture(0, NULL)`** — leaves the previous atlas bound → leaked-texture quads
   ("Steve skin on vignette", `Textures.cpp:58-83`).
5. **Depth state leaks from GUI to world** — must re-enable Z every world frame
   (`EntityRenderer.cpp:486-492`).
6. **LH projection X-mirror** forces the whole CW↔CCW cull inversion
   (`EntityRenderer.cpp:301-308,485`); brittle. Use RH projection in the rewrite.
7. **GUI ortho sign traps** — Y-flip and the +2000 (not −2000) Z translate; wrong sign = blank
   blue screen (`EntityRenderer.cpp:619-630`).
8. **`D3DRS_ALPHATESTENABLE` does nothing on Xenos** — relying on it (instead of `setAlphaRef`)
   silently produces opaque black boxes where cutouts should be (`Shaders.h:28-34`).
9. **Animated textures don't actually update** — `glTexSubImage2D`/`TextureFX` LockRect upload is
   deferred (`RenderEngine.cpp:101-107`); water/lava/fire are static in the reference.
10. **Color packing mismatch** between the faithful NEW Tessellator (`0xAABBGGRR`) and D3DCOLOR
    (`0xAARRGGBB`) — must swizzle in the emulator's gather, or red/blue are swapped.
11. **`color3f`/`color4f` honor a `m_colorDisabled` flag** (`Tesselator.cpp:91,100`) — the
    `disableColor()` semantic (used so terrain keeps baked color) must be preserved in the emulator.

---

## Open questions

1. **Per-context gamma.** `s_gamma=1.19` is tuned for the menu; world wants its own value
   (`Shaders.cpp:185-188` flags a "per-context setGamma TODO"). What value does the world need,
   and should the emulator expose gamma at all (Java has none)?
2. **Display lists.** Alpha's chunk renderer (`WorldRenderer`/`RenderGlobal`) leans on
   `glNewList/glCallList`. Re-tessellate every frame (simple, slower) vs. record/replay into
   per-chunk dynamic VBs (faster, more code)? The OLD port's answer isn't fully visible from the
   files read — needs a `RenderGlobal.cpp`/`WorldRenderer.cpp` deep-dive.
3. **VBO path.** NEW Tessellator has a full ARB VBO path gated off (`s_tryVBO=false`). Worth
   enabling on 360 for chunk geometry, or stick to `DrawPrimitiveUP`?
4. **Normals/lighting.** Reference bakes light into vertex color and no-ops all lighting. Are there
   any Alpha draws (entities? items in 3D?) that actually expected GL lighting, or is baked-color
   universally sufficient?
5. **`glReadPixels` / screenshots** — needed for the 360 build, or safe to stub?
6. **RH vs LH projection decision** — adopting `PerspectiveFovRH` removes the X-mirror + CW/CCW
   hacks but changes every winding assumption inherited from the reference; confirm before
   committing.
7. **MSAA / render target format** — OLD port uses `D3DMULTISAMPLE_NONE`, `X8R8G8B8`, `D24S8`
   (`Main.cpp:48-55`). Keep, or upgrade for the rewrite?
