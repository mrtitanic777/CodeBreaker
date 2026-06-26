# TexturePackList.cpp

Risk score: **7**  ·  6 findings

## GUESSED — converter made a heuristic call that may be wrong -- INSPECT

- **🔴 HIGH** `L90` (marker, `PORT`): determine element type */ TexturePackList::availableTexturePacks() {
  ```cpp
  std::vector<void*> /* PORT: determine element type */ TexturePackList::availableTexturePacks() {
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L5` (marker, `PORT`): On a single-player Xbox 360 port there are NO external (.zip) texture
  ```cpp
  // PORT: On a single-player Xbox 360 port there are NO external (.zip) texture
  ```
- **⚪ LOW** `L31` (marker, `PORT`): was java.io.File */* var2)
  ```cpp
  TexturePackList::TexturePackList(Minecraft* var1, std::string /* PORT: was java.io.File */* var2)
  ```
- **⚪ LOW** `L41` (marker, `PORT`): no texturepacks directory on 360 (java.io.File unsupported)
  ```cpp
  // PORT: no texturepacks directory on 360 (java.io.File unsupported)
  ```
- **⚪ LOW** `L78` (marker, `PORT`): only the built-in default pack exists on 360 (no .zip scanning).
  ```cpp
  // PORT: only the built-in default pack exists on 360 (no .zip scanning).
  ```
