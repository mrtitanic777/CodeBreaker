# CraftingManager.h

Risk score: **5**  ·  1 findings

## GUESSED — converter made a heuristic call that may be wrong -- INSPECT

- **🔴 HIGH** `L21` (marker, `PORT`): determine element type */ m_recipes;
  ```cpp
  std::vector<void*> /* PORT: determine element type */ m_recipes;
  ```
