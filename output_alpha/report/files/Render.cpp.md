# Render.cpp

Risk score: **2**  ·  4 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L66` (marker, `PORT`): bind terrain.png (was Textures::load(...) with the result discarded -> nothing bound)
  ```cpp
  // PORT: bind terrain.png (was Textures::load(...) with the result discarded -> nothing bound)
  ```
- **⚪ LOW** `L177` (marker, `PORT`): Tesselator has no setTranslationD; fold the (var1,var3,var5) offset into every vertex coord.
  ```cpp
  // PORT: Tesselator has no setTranslationD; fold the (var1,var3,var5) offset into every vertex coord.
  ```
- **⚪ LOW** `L209` (marker, `PORT`): bind active texture */);
  ```cpp
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  ```
