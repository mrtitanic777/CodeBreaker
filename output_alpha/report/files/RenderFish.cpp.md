# RenderFish.cpp

Risk score: **2**  ·  4 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L2` (marker, `PORT`): of RenderFish.java (Minecraft Alpha v1.2.6)
  ```cpp
  // Hand-written PORT of RenderFish.java (Minecraft Alpha v1.2.6)
  ```
- **⚪ LOW** `L119` (marker, `PORT`): GL line-strip mode */ 3);
  ```cpp
  var12.init(/* PORT: GL line-strip mode */ 3);
  ```
- **⚪ LOW** `L133` (marker, `PORT`): restore active world texture */);
  ```cpp
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: restore active world texture */);
  ```
