# GuiTexturePacks.cpp

Risk score: **12**  ·  13 findings

## BLOCKED — explicitly could not be ported

- **🔴 HIGH** `L21` (marker, `PORT-BLOCKED`): in the converter
  ```cpp
  //   * Java mouse drag/scroll (Mouse.isButtonDown) was already // PORT-BLOCKED in the converter
  ```
- **🔴 HIGH** `L115` (marker, `PORT-BLOCKED`): .
  ```cpp
  // list is empty, so there is nothing to drag-select; XInput scrolling is N/A. // PORT-BLOCKED.
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L2` (marker, `PORT`): override for GuiTexturePacks.java (Minecraft Alpha v1.2.6)
  ```cpp
  // Hand-written PORT override for GuiTexturePacks.java (Minecraft Alpha v1.2.6)
  ```
- **⚪ LOW** `L13` (marker, `PORT`): stubbed. The "Open texture pack folder" button is a no-op.
  ```cpp
  //     texture-pack folder) -> // PORT: stubbed. The "Open texture pack folder" button is a no-op.
  ```
- **⚪ LOW** `L75` (marker, `PORT`): Java did `new File(mc.minecraftDir, "texturepacks").getAbsolutePath()`. No filesystem
  ```cpp
  // PORT: Java did `new File(mc.minecraftDir, "texturepacks").getAbsolutePath()`. No filesystem
  ```
- **⚪ LOW** `L87` (marker, `PORT`): Sys.openURL("file://" + folder) -> no OS file browser / texturepacks folder on 360.
  ```cpp
  // PORT: Sys.openURL("file://" + folder) -> no OS file browser / texturepacks folder on 360.
  ```
- **⚪ LOW** `L91` (marker, `PORT`): Java reloaded textures here; the built-in pack is always active on the 360.
  ```cpp
  // PORT: Java reloaded textures here; the built-in pack is always active on the 360.
  ```
- **⚪ LOW** `L114` (marker, `PORT`): Java handled left-mouse drag/scroll of the pack list here. With no external packs the
  ```cpp
  // PORT: Java handled left-mouse drag/scroll of the pack list here. With no external packs the
  ```
- **⚪ LOW** `L167` (marker, `PORT`): bind active texture */);
  ```cpp
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  ```
- **⚪ LOW** `L194` (marker, `PORT`): Tesselator has no setColorRGBA_I(rgb, alpha) -> color4b(r,g,b,a). rgb 0 = black.
  ```cpp
  // PORT: Tesselator has no setColorRGBA_I(rgb, alpha) -> color4b(r,g,b,a). rgb 0 = black.
  ```
- **⚪ LOW** `L210` (marker, `PORT`): bind active texture */);
  ```cpp
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  ```
- **⚪ LOW** `L230` (marker, `PORT`): setColorRGBA_I(4210752, alpha) -> color4b. 4210752 = 0x404040 (gray 64,64,64).
  ```cpp
  // PORT: setColorRGBA_I(4210752, alpha) -> color4b. 4210752 = 0x404040 (gray 64,64,64).
  ```
