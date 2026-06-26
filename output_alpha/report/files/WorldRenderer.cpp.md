# WorldRenderer.cpp

Risk score: **17**  ·  14 findings

## GUESSED — converter made a heuristic call that may be wrong -- INSPECT

- **🔴 HIGH** `L176` (marker, `PORT`): determine element type */ var2, int var3, int var4, int var5, int var6, int var7)
  ```cpp
  WorldRenderer::WorldRenderer(World* var1, std::vector<void*> /* PORT: determine element type */ var2, int var3, int var4, int var5, int var6, int var7)
  ```

## RISK — matches a known console-crash / leak pattern

- **🔴 HIGH** `L158` (rule, `uninit-array`): new bool[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new bool[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_1748_p(new bool[2])
  ```
- **🔴 HIGH** `L192` (rule, `uninit-array`): new bool[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new bool[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_1748_p(new bool[2])
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L2` (marker, `PORT`): override for WorldRenderer.java (Minecraft Alpha v1.2.6)
  ```cpp
  // Hand-written PORT override for WorldRenderer.java (Minecraft Alpha v1.2.6)
  ```
- **⚪ LOW** `L10` (marker, `PORT`): no-ops), but it is shaped to the ALPHA WorldRenderer.h
  ```cpp
  // display-list calls stubbed as PORT no-ops), but it is shaped to the ALPHA WorldRenderer.h
  ```
- **⚪ LOW** `L43` (marker, `PORT`): per-chunk baked GPU vertex buffers. The Alpha header keeps the old GL display-list field
  ```cpp
  // PORT: per-chunk baked GPU vertex buffers. The Alpha header keeps the old GL display-list field
  ```
- **⚪ LOW** `L71` (marker, `PORT(rapid-fire crash guard from reference)`): never Release a VB still bound as stream 0.
  ```cpp
  // PORT(rapid-fire crash guard from reference): never Release a VB still bound as stream 0.
  ```
- **⚪ LOW** `L245` (marker, `PORT`): Java recorded the chunk's offset bounding box into GL display list var7 (glNewList /
  ```cpp
  // PORT: Java recorded the chunk's offset bounding box into GL display list var7 (glNewList /
  ```
- **⚪ LOW** `L254` (marker, `PORT`): translate the world matrix to this chunk's local origin. Kept for callers that still
  ```cpp
  // PORT: translate the world matrix to this chunk's local origin. Kept for callers that still
  ```
- **⚪ LOW** `L278` (marker, `PORT(rapid-fire crash guard from reference)`): unbind stream 0 before any VB Release in the bake.
  ```cpp
  // PORT(rapid-fire crash guard from reference): unbind stream 0 before any VB Release in the bake.
  ```
- **⚪ LOW** `L306` (marker, `PORT`): guard unregistered id -> NULL virtual call
  ```cpp
  if(var18 > 0 && Block::blocksList[var18] != NULL) {   // PORT: guard unregistered id -> NULL virtual call
  ```
- **⚪ LOW** `L314` (marker, `PORT`): TileEntityRenderer special-renderer registry isn't ported; collect
  ```cpp
  // PORT: TileEntityRenderer special-renderer registry isn't ported; collect
  ```
- **⚪ LOW** `L334` (marker, `PORT`): bake the quads collected for this pass into the chunk's GPU VB (was glEndList /
  ```cpp
  // PORT: bake the quads collected for this pass into the chunk's GPU VB (was glEndList /
  ```
