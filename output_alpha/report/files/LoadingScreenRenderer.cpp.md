# LoadingScreenRenderer.cpp

Risk score: **4**  ·  6 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L110` (lint, `warn/clear-target`): Clear(TARGET|ZBUFFER): if this came from glClear(DEPTH) it should be ZBUFFER-only -- TARGET wipes the rendered world (the 'eraser' bug).

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L52` (marker, `PORT`): was 'throw new MinecraftError()' (EH off) -> abort loading
  ```cpp
  // PORT: was 'throw new MinecraftError()' (EH off) -> abort loading
  ```
- **⚪ LOW** `L76` (marker, `PORT`): was 'throw new MinecraftError()' (EH off) -> abort loading
  ```cpp
  // PORT: was 'throw new MinecraftError()' (EH off) -> abort loading
  ```
- **⚪ LOW** `L90` (marker, `PORT`): was 'throw new MinecraftError()' (EH off) -> abort loading
  ```cpp
  // PORT: was 'throw new MinecraftError()' (EH off) -> abort loading
  ```
- **⚪ LOW** `L140` (marker, `PORT`): bind active texture */);
  ```cpp
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  ```
