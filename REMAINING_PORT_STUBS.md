# Remaining PORT Stubs — Minecraft Indev Xbox 360 Port

**Converter Status:** 94.5% concrete across 230 Java files (1,152 D3D9/Xbox calls generated)

The 67 remaining `// PORT:` stubs require **design decisions**, not mechanical conversion. They break down into three subsystems:

---

## 1. RenderEngine — Runtime Texture Creation (16 stubs)

**What it is:**
Java's `BufferedImage` + `ImageIO` code that reads PNG files into pixel arrays, creates GL textures from raw pixels, and uploads them.

**What's already handled:**
`Textures::load()` handles loading PNGs from disk with `D3DXCreateTextureFromFileExA`, streaming from `game:\Content\`.

**What's left:**
The *animated texture* path — where Indev generates water/lava/fire/gear textures **procedurally every frame** by writing pixel data into a buffer and uploading it to the GPU.

**Affected classes:**
- `TextureLavaFX` — generates lava animation pixel data
- `TextureWaterFX` — generates water animation pixel data
- `TextureWaterFlowFX` — generates flowing water pixel data
- `TextureFlamesFX` — generates fire animation pixel data
- `TextureGearsFX` — generates gear rotation pixel data
- `RenderEngine` — orchestrates texture upload to GPU

**Xbox 360 approach:**
```cpp
// Create a dynamic texture at init
g_pd3dDevice->CreateTexture(16, 16, 1, D3DUSAGE_DYNAMIC,
    D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pAnimTexture, NULL);

// Each frame: lock, write pixels, unlock
D3DLOCKED_RECT lr;
pAnimTexture->LockRect(0, &lr, NULL, D3DLOCK_DISCARD);
memcpy(lr.pBits, pixelData, 16 * 16 * 4);
pAnimTexture->UnlockRect(0);
```

**Design decision:**
- **LockRect per frame** — faithful to Java, ~5 textures updated per frame (cheap on 360)
- **Pre-baked sprite sheets** — extract all animation frames offline, cycle through UV coordinates at runtime (faster, less faithful)

---

## 2. RenderGlobal — Display List Batching + Occlusion (15 stubs)

**What it is:**
OpenGL display list batch rendering and ARB occlusion query culling for chunk visibility.

### Display List Batching (5 stubs)

**What's left:**
`glCallLists()` draws multiple chunk display lists in one call. On Xbox 360, each "display list" is a `IDirect3DVertexBuffer9`.

**Xbox 360 approach:**
```cpp
// Instead of glCallLists(count, GL_INT, buffer):
for (int i = 0; i < chunkCount; ++i) {
    WorldRenderer* wr = sortedWorldRenderers[i];
    if (wr->isInFrustrum && wr->hasVertexBuffer) {
        g_pd3dDevice->SetStreamSource(0, wr->vertexBuffer, 0, sizeof(TesselatorVertex));
        g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, wr->triCount);
    }
}
```

### Occlusion Culling (3 stubs)

**What's left:**
ARB occlusion queries that test if chunks are visible behind other geometry before drawing them.

**Xbox 360 options:**
- **Skip entirely** — Indev worlds are small (256×256×64), frustum culling alone is probably sufficient
- **D3D9 occlusion queries** — Xbox 360 supports `IDirect3DQuery9` with `D3DQUERYTYPE_OCCLUSION`:
```cpp
IDirect3DQuery9* pQuery;
g_pd3dDevice->CreateQuery(D3DQUERYTYPE_OCCLUSION, &pQuery);
pQuery->Issue(D3DISSUE_BEGIN);
// Draw bounding box
pQuery->Issue(D3DISSUE_END);
DWORD pixelsDrawn;
while (pQuery->GetData(&pixelsDrawn, sizeof(DWORD), D3DGETDATA_FLUSH) == S_FALSE);
if (pixelsDrawn > 0) { /* chunk is visible */ }
```

**Design decision:**
- **Skip occlusion culling for now** — get the port running first, add it as an optimization later
- **Implement D3D9 queries** — more accurate culling, slight GPU overhead

### Remaining (7 stubs)
Display mode / present params — already handled by `Main.cpp` D3D9 init. These are dead code in the ported version.

---

## 3. Minecraft + EntityRenderer — Edge Cases (36 stubs)

**What it is:**
LWJGL window management, caps checking, and input polling remnants.

### Display Setup (~10 stubs)
`Display.setDisplayMode`, `Display.create`, caps checking — all handled by `Main.cpp` D3D9 device creation. These stubs are in `Minecraft.run()` init code that can be replaced by a single call to your D3D9 init function.

### XInput Input Loop (~15 stubs)
`Keyboard.getEventKey`, `Keyboard.next`, `XInputGetKeystroke` — these are in `Minecraft.run()`'s input polling loop. The entire keyboard/mouse event loop needs to be replaced with XInput gamepad polling.

**Suggested button mapping:**
| Xbox 360 | Action |
|----------|--------|
| Left Stick | Move (WASD) |
| Right Stick | Look (Mouse) |
| A | Jump |
| X | Place Block |
| Right Trigger | Break Block |
| Left Trigger | Use Item |
| B | Open Inventory |
| Y | Cycle Hotbar |
| D-Pad | Hotbar Slot Select |
| Start | Pause Menu |
| Back | Quit |
| Left Bumper | Sprint |
| Right Bumper | Sneak |

**Xbox 360 approach:**
```cpp
XINPUT_STATE state;
if (XInputGetState(0, &state) == ERROR_SUCCESS) {
    // Left stick -> movement
    float moveX = state.Gamepad.sThumbLX / 32768.0f;
    float moveZ = state.Gamepad.sThumbLY / 32768.0f;

    // Right stick -> look
    float lookX = state.Gamepad.sThumbRX / 32768.0f;
    float lookY = state.Gamepad.sThumbRY / 32768.0f;

    // Buttons
    bool jump = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
    bool inventory = (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
    bool breakBlock = state.Gamepad.bRightTrigger > 128;
    bool placeBlock = (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
}
```

### Misc (~11 stubs)
- Exception stack traces → already converted to `DbgPrintF`
- `Thread` creation → stub out (Indev is single-threaded gameplay)
- Java I/O streams → level save/load needs `CreateFile`/`ReadFile`/`WriteFile`

---

## Summary

| Category | Stubs | Priority | Effort |
|----------|-------|----------|--------|
| Animated Textures | 16 | Medium | 1-2 hours (LockRect approach) |
| Display List Batching | 5 | High | 2-3 hours (WorldRenderer VB loop) |
| Occlusion Culling | 3 | Low | Skip for now |
| Display Setup | 10 | None | Already handled by Main.cpp |
| XInput Input | 15 | High | 3-4 hours (full gamepad system) |
| Misc Edge Cases | 18 | Low | Fix as encountered |

**Total estimated manual work: ~8-10 hours** to get a running port with block rendering, player movement, and basic gameplay.

---

## The converter's job is done.

What's left requires design decisions:
- **Animated textures:** LockRect per frame, or pre-baked sprite sheets?
- **Occlusion culling:** Skip it for now, or implement D3D9 queries?
- **XInput mapping:** A=jump, B=inventory, sticks=move/look — or your own layout?

These are things to build by hand as you get the port running.
