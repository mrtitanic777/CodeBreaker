# Java-to-Xbox 360 Render Conversion Rules
## For RubyDung (and similar OpenGL-to-D3D9 Xbox 360 ports)

---

## Architecture: What the Old Working Code Does

The old code has a clear init-before-run pipeline:

```
main()
  1. Direct3DCreate9 + CreateDevice (D3DFMT_X8R8G8B8, IMMEDIATE)
  2. SetRenderState (ZENABLE, CULLMODE)
  3. SetSamplerState (POINT filtering)
  4. Shaders::init() — compile 4 HLSL shaders at startup
  5. LoadTextures() — D3DXCreateTextureFromFileExA with fallback placeholders
  6. game->init() + game->run()
```

Key globals: `g_pd3dDevice` (extern everywhere), `g_terrainTexture`, `g_charTexture`.

---

## Rule 1: D3D Device is a Global, Created in main()

**Java:** No explicit device — LWJGL/OpenGL manages context.  
**Xbox 360:** `IDirect3DDevice9* g_pd3dDevice` declared in RubyDung.cpp, externed everywhere else.

```cpp
// RubyDung.cpp — top of file
IDirect3D9*       g_pD3D       = NULL;
IDirect3DDevice9* g_pd3dDevice = NULL;

// Every other .cpp that needs the device:
extern IDirect3DDevice9* g_pd3dDevice;
```

**NEVER** wrap the device in a namespace or helper class. **NEVER** use `static` on these globals.

---

## Rule 2: Shaders Compiled at Startup, Not Lazily

**Java:** OpenGL fixed-function pipeline — no shaders needed.  
**Xbox 360:** GPU is shader-only. Must compile VS+PS before any draw call.

The old code uses `Shaders.cpp` with:
- `Shaders::init()` — compiles 4 shaders (textured VS/PS, untextured VS/PS)
- `Shaders::useTexturedShader()` — binds VS + PS + vertex declaration
- `Shaders::useUntexturedShader()` — binds untextured pair
- `Shaders::updateShaderConstants()` — uploads WVP matrix to c0-c3

**NEVER** compile shaders inside `Tesselator::flush()` or any per-frame code.

### Shader source (textured VS):
```hlsl
float4x4 g_WorldViewProj : register(c0);
float4 g_FogParams : register(c8);
struct VS_INPUT { float3 pos : POSITION; float4 color : COLOR0; float2 tex : TEXCOORD0; };
struct VS_OUTPUT { float4 pos : POSITION; float4 color : COLOR0; float2 tex : TEXCOORD0; float fog : TEXCOORD1; };
VS_OUTPUT main(VS_INPUT input) {
    VS_OUTPUT output;
    output.pos = mul(float4(input.pos, 1.0), g_WorldViewProj);
    output.color = input.color;
    output.tex = input.tex;
    float dist = output.pos.z;
    output.fog = saturate(exp(-g_FogParams.x * dist));
    return output;
}
```

### WVP matrix convention:
```cpp
D3DXMatrixMultiply(&wvp, &worldMatrix, &viewMatrix);
D3DXMatrixMultiply(&wvp, &wvp, &projMatrix);
D3DXMatrixTranspose(&wvp, &wvp);  // HLSL expects column-major
g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&wvp, 4);
```

Profiles: `vs_3_0` / `ps_3_0`.

---

## Rule 3: Camera Builds View Matrix with D3DX, Not MatrixStack

**Java:** `GL11.glTranslatef`, `GL11.glRotatef` — OpenGL matrix stack.  
**Xbox 360:** Build the view matrix explicitly with D3DX calls.

```cpp
// OLD WORKING moveCameraToPlayer:
D3DXMatrixTranslation(&matTrans, -px, -py, -pz);
D3DXMatrixRotationY(&matRotY, player->yRot * DEG_TO_RAD);
D3DXMatrixRotationX(&matRotX, player->xRot * DEG_TO_RAD);
D3DXMatrixTranslation(&matTransZ, 0.0f, 0.0f, -0.3f);
// Combine: Trans * RotY * RotX * TransZ
D3DXMatrixMultiply(&matView, &matTrans, &matRotY);
D3DXMatrixMultiply(&matView, &matView, &matRotX);
D3DXMatrixMultiply(&matView, &matView, &matTransZ);
Shaders::setViewMatrix(matView);
```

### Projection:
```cpp
D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(70.0f), aspect, 0.05f, 1000.0f);
Shaders::setProjectionMatrix(matProj);
```

**NEVER** use an OpenGL-style MatrixStack class. The multiply order differs between OpenGL (post-multiply) and D3D (pre-multiply), causing silent transform bugs.

---

## Rule 4: Textures Loaded at Startup with D3DXCreateTextureFromFileExA

**Java:** `ImageIO.read()` + `GL11.glTexImage2D`.  
**Xbox 360:** `D3DXCreateTextureFromFileExA` with explicit format.

```cpp
D3DXCreateTextureFromFileExA(
    g_pd3dDevice, fullPath,
    D3DX_DEFAULT, D3DX_DEFAULT,  // width, height from file
    1,                            // 1 mip level
    0,                            // usage
    D3DFMT_A8R8G8B8,             // FORCE 32-bit ARGB
    D3DPOOL_DEFAULT,
    D3DX_FILTER_NONE,            // no filter on load
    D3DX_FILTER_NONE,            // no mip filter
    0xFF000000,                  // color key
    NULL, NULL,
    &pTexture);
```

**NEVER** use `D3DXCreateTextureFromFileA` (the simple version) — it chokes on paletted PNGs.

Textures are loaded in `main()` before the game loop, stored as globals, and bound per-frame in `render()`:
```cpp
g_pd3dDevice->SetTexture(0, g_terrainTexture);
```

---

## Rule 5: Chunk Uses Vertex Buffer Cache, Not DrawPrimitiveUP Per Frame

**Java:** OpenGL display lists (`glNewList`/`glCallList`).  
**Xbox 360:** `IDirect3DVertexBuffer9` created once in `rebuild()`, drawn with `DrawPrimitive` in `render()`.

```cpp
// Chunk::rebuild(layer) — build geometry into VB once
g_pd3dDevice->CreateVertexBuffer(vertCount * sizeof(TesselatorVertex),
    D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &m_vertexBuffer[layer], NULL);
// Lock, fill with triangle data, unlock

// Chunk::render(layer) — draw cached VB every frame (fast)
g_pd3dDevice->SetVertexDeclaration(Shaders::s_vertexDecl);
g_pd3dDevice->SetStreamSource(0, m_vertexBuffer[layer], 0, sizeof(TesselatorVertex));
g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_vertexCount[layer] / 3);
```

The rebuild limit (4 per frame) only controls how many chunks REBUILD, not how many DRAW. All chunks with valid VBs draw every frame.

---

## Rule 6: Quad-to-Triangle Winding is REVERSED for D3D

**Java/OpenGL:** CCW winding, quads via `GL_QUADS`.  
**Xbox 360/D3D:** CW winding with `D3DCULL_CW`. Triangulate with reversed winding.

```cpp
// Quad verts: 0, 1, 2, 3
// Triangle 1: 0, 2, 1  (reversed)
// Triangle 2: 0, 3, 2  (reversed)
```

**NEVER** use 0,1,2 / 0,2,3 — that's OpenGL winding and gets 100% backface-culled.

---

## Rule 7: Render Loop Order

**Java:**
```java
GL11.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
setupCamera();
level.render(player, 0);  // with fog off
zombies.render();
level.render(player, 1);  // with fog on
Display.update();
```

**Xbox 360:**
```cpp
g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
    D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
g_pd3dDevice->BeginScene();

setupCamera();
Shaders::updateShaderConstants();

// Layer 0
Shaders::setFogEnabled(false);
Shaders::useTexturedShader();
g_pd3dDevice->SetTexture(0, g_terrainTexture);
Shaders::updateShaderConstants();
m_levelRenderer->render(m_player, 0);

// Zombies
for (...) m_zombies[i]->render(a);

// Layer 1
Shaders::setFogEnabled(true);
Shaders::useTexturedShader();
g_pd3dDevice->SetTexture(0, g_terrainTexture);
Shaders::updateShaderConstants();
m_levelRenderer->render(m_player, 1);

// Hit highlight (untextured shader)
Shaders::useUntexturedShader();
Shaders::updateShaderConstants();
renderHit();

g_pd3dDevice->EndScene();
g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
```

Key: **Bind shader + upload constants + bind texture BEFORE each render pass.** Never rely on shaders being bound inside Tesselator::flush().

---

## Rule 8: D3D Present Parameters

```cpp
D3DPRESENT_PARAMETERS d3dpp;
ZeroMemory(&d3dpp, sizeof(d3dpp));
d3dpp.BackBufferWidth        = 1280;
d3dpp.BackBufferHeight       = 720;
d3dpp.BackBufferFormat       = D3DFMT_X8R8G8B8;  // NOT A8R8G8B8
d3dpp.BackBufferCount        = 1;
d3dpp.MultiSampleType        = D3DMULTISAMPLE_NONE;
d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
d3dpp.hDeviceWindow          = NULL;
d3dpp.Windowed               = FALSE;
d3dpp.EnableAutoDepthStencil = TRUE;
d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
d3dpp.Flags                  = 0;
d3dpp.FullScreen_RefreshRateInHz = 60;
d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
```

---

## Rule 9: Fixed-Function Render States That DON'T Exist on Xbox 360

Remove all of these from converted code:
- `D3DRS_LIGHTING` — no FFP lighting
- `D3DRS_SHADEMODE` / `D3DSHADE_GOURAUD`
- `D3DRS_FOGENABLE`, `D3DRS_FOGCOLOR`, `D3DRS_FOGTABLEMODE`, `D3DRS_FOGDENSITY`
- `SetTextureStageState()` — no FFP texture stages
- `SetTransform()` — no FFP transform pipeline

States that DO work: `D3DRS_ZENABLE`, `D3DRS_ZWRITEENABLE`, `D3DRS_ZFUNC`, `D3DRS_CULLMODE`, `D3DRS_ALPHABLENDENABLE`, `D3DRS_SRCBLEND`, `D3DRS_DESTBLEND`, `SetSamplerState()`.

---

## Rule 10: OpenGL-to-D3D9 API Mapping (for script conversion)

| Java/OpenGL | Xbox 360 D3D9 | Notes |
|---|---|---|
| `GL11.glClear()` | `g_pd3dDevice->Clear()` | |
| `GL11.glBegin(GL_QUADS)` | `Tesselator::init()` | |
| `GL11.glEnd()` | `Tesselator::flush()` | Tesselator converts quads to triangles |
| `GL11.glVertex3f()` | `Tesselator::vertex()` | |
| `GL11.glTexCoord2f()` | `Tesselator::tex()` | |
| `GL11.glColor3f()` | `Tesselator::color()` | |
| `GL11.glBindTexture()` | `g_pd3dDevice->SetTexture(0, tex)` | |
| `GL11.glEnable(GL_TEXTURE_2D)` | (always enabled via shader) | Remove |
| `GL11.glDisable(GL_TEXTURE_2D)` | `g_pd3dDevice->SetTexture(0, NULL)` | Unbind |
| `GL11.glEnable(GL_FOG)` | `Shaders::setFogEnabled(true)` | Via shader constant |
| `GL11.glDisable(GL_FOG)` | `Shaders::setFogEnabled(false)` | Via shader constant |
| `GL11.glPushMatrix()` | (save current D3DX matrix) | |
| `GL11.glPopMatrix()` | (restore saved matrix) | |
| `GL11.glTranslatef()` | `D3DXMatrixTranslation` + multiply | |
| `GL11.glRotatef()` | `D3DXMatrixRotation*` + multiply | |
| `GL11.glScalef()` | `D3DXMatrixScaling` + multiply | |
| `GL11.glNewList()` | `Chunk::rebuild()` into VB | |
| `GL11.glCallList()` | `Chunk::render()` from VB | |
| `Display.update()` | `EndScene() + Present()` | |
| `Keyboard.isKeyDown()` | `XInputGetState()` | |

---

## Rule 11: Vertex Declaration (NOT FVF)

Xbox 360 requires a vertex declaration, not `SetFVF`.

```cpp
D3DVERTEXELEMENT9 declTextured[] = {
    { 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
    { 0, 16, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
    D3DDECL_END()
};
g_pd3dDevice->CreateVertexDeclaration(declTextured, &s_vertexDecl);
```

Bind BEFORE draw: `g_pd3dDevice->SetVertexDeclaration(Shaders::s_vertexDecl);`

---

## Summary: Conversion Checklist for Script

When `java2cpp360.py` outputs converted code, a post-processing pass must:

1. [ ] Remove all `GL11.glEnable(GL_LIGHTING)` / `GL_FOG` / fixed-function state calls
2. [ ] Replace `glBegin`/`glEnd` blocks with Tesselator init/vertex/flush calls  
3. [ ] Replace `glTranslatef`/`glRotatef`/`glScalef` with D3DX matrix math
4. [ ] Replace `glPushMatrix`/`glPopMatrix` with D3DX matrix save/restore
5. [ ] Reverse quad winding (0,2,1 / 0,3,2) in Tesselator
6. [ ] Replace `Textures.loadTexture()` calls with `D3DXCreateTextureFromFileExA`
7. [ ] Build camera view matrix with explicit D3DX calls, not MatrixStack
8. [ ] Add `Shaders::useTexturedShader()` + `updateShaderConstants()` before each render pass
9. [ ] Bind `g_terrainTexture` via `SetTexture(0, ...)` before chunk rendering
10. [ ] Cache chunk geometry in `IDirect3DVertexBuffer9`, draw with `DrawPrimitive`
11. [ ] Use `BeginScene`/`EndScene`/`Present` directly — no wrapper functions
