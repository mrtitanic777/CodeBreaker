# MinecartTrackLogic.h

Risk score: **7**  ·  3 findings

## GUESSED — converter made a heuristic call that may be wrong -- INSPECT

- **🔴 HIGH** `L27` (marker, `PORT`): determine element type */ m_field_1161_g;
  ```cpp
  std::vector<void*> /* PORT: determine element type */ m_field_1161_g;
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L40` (marker, `PORT`): Java static helper (called as MinecartTrackLogic::func_791_a)
  ```cpp
  static int func_791_a(MinecartTrackLogic* var0);  // PORT: Java static helper (called as MinecartTrackLogic::func_791_a)
  ```
