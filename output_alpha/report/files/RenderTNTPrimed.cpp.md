# RenderTNTPrimed.cpp

Risk score: **0**  ·  1 findings

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L63` (marker, `PORT`): bind active texture */);
  ```cpp
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  ```
