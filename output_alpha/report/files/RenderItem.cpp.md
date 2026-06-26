# RenderItem.cpp

Risk score: **10**  ·  3 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L58` (marker, `PORT`): D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);
  ```cpp
  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);
  ```
- **🔴 HIGH** `L135` (marker, `PORT`): D3D9 SetRenderState equivalent  // was: GL11.glDisable(GL12.GL_RESCALE_NORMAL);
  ```cpp
  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glDisable(GL12.GL_RESCALE_NORMAL);
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L206` (marker, `PORT`): bind active texture */);
  ```cpp
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  ```
