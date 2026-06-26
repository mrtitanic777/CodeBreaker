# AxisAlignedBB.h

Risk score: **5**  ·  1 findings

## GUESSED — converter made a heuristic call that may be wrong -- INSPECT

- **🔴 HIGH** `L44` (marker, `PORT`): determine element type */ s_boundingBoxes;
  ```cpp
  static std::vector<void*> /* PORT: determine element type */ s_boundingBoxes;
  ```
