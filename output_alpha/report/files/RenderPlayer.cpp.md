# RenderPlayer.cpp

Risk score: **2**  ·  4 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L2` (marker, `PORT`): of RenderPlayer.java (Minecraft Alpha v1.2.6)
  ```cpp
  // Hand-written PORT of RenderPlayer.java (Minecraft Alpha v1.2.6)
  ```
- **⚪ LOW** `L147` (marker, `PORT`): re-bind font texture handled by drawString */);
  ```cpp
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: re-bind font texture handled by drawString */);
  ```
- **⚪ LOW** `L173` (marker, `PORT`): re-bind font texture handled by drawString */);
  ```cpp
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: re-bind font texture handled by drawString */);
  ```
