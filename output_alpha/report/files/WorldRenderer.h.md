# WorldRenderer.h

Risk score: **15**  ·  3 findings

## GUESSED — converter made a heuristic call that may be wrong -- INSPECT

- **🔴 HIGH** `L18` (marker, `PORT`): determine element type */ var2, int var3, int var4, int var5, int var6, int var7);
  ```cpp
  WorldRenderer(World* var1, std::vector<void*> /* PORT: determine element type */ var2, int var3, int var4, int var5, int var6, int var7);
  ```
- **🔴 HIGH** `L49` (marker, `PORT`): determine element type */ m_field_1745_B;
  ```cpp
  std::vector<void*> /* PORT: determine element type */ m_field_1745_B;
  ```
- **🔴 HIGH** `L66` (marker, `PORT`): determine element type */ m_field_1737_F;
  ```cpp
  std::vector<void*> /* PORT: determine element type */ m_field_1737_F;
  ```
