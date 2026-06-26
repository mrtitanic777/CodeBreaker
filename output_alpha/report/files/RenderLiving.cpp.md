# RenderLiving.cpp

Risk score: **14**  ·  5 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L70` (marker, `PORT`): D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);
  ```cpp
  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);
  ```
- **🔴 HIGH** `L140` (marker, `PORT`): D3D9 SetRenderState equivalent  // was: GL11.glDisable(GL12.GL_RESCALE_NORMAL);
  ```cpp
  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glDisable(GL12.GL_RESCALE_NORMAL);
  ```

## WARNING — suspicious; verify by hand

- **🟡 MED** `L158` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L174` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L137` (marker, `PORT`): bind active texture */);
  ```cpp
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  ```
