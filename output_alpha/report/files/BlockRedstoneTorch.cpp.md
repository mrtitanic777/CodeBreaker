# BlockRedstoneTorch.cpp

Risk score: **7**  ·  3 findings

## GUESSED — converter made a heuristic call that may be wrong -- INSPECT

- **🔴 HIGH** `L15` (marker, `PORT`): determine element type */ BlockRedstoneTorch::s_torchUpdates;
  ```cpp
  std::vector<void*> /* PORT: determine element type */ BlockRedstoneTorch::s_torchUpdates;
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L108` (marker, `PORT`): Java did torchUpdates.remove(0) -- pop & free the expired front entry
  ```cpp
  // PORT: Java did torchUpdates.remove(0) -- pop & free the expired front entry
  ```
