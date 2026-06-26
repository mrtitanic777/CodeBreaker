# Gui.cpp

Risk score: **0**  ·  2 findings

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L37` (marker, `PORT`): bind active texture */);
  ```cpp
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  ```
- **⚪ LOW** `L68` (marker, `PORT`): bind active texture */);
  ```cpp
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  ```
