# Chunk.h

Risk score: **15**  ·  3 findings

## GUESSED — converter made a heuristic call that may be wrong -- INSPECT

- **🔴 HIGH** `L38` (marker, `PORT`): determine element type */ m_entities;
  ```cpp
  std::vector<void*>* /* PORT: determine element type */ m_entities;
  ```
- **🔴 HIGH** `L71` (marker, `PORT`): determine element type */ var3);
  ```cpp
  virtual void getEntitiesWithinAABBForEntity(Entity* var1, AxisAlignedBB var2, std::vector<void*> /* PORT: determine element type */ var3);
  ```
- **🔴 HIGH** `L72` (marker, `PORT`): determine element type */ var3);
  ```cpp
  virtual void getEntitiesOfTypeWithinAAAB(void* var1, AxisAlignedBB var2, std::vector<void*> /* PORT: determine element type */ var3);
  ```
