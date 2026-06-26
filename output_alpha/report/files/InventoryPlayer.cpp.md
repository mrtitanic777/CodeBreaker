# InventoryPlayer.cpp

Risk score: **2**  ·  2 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L31` (marker, `PORT`): array-length companions must be set or loop bounds are garbage
  ```cpp
  , m_mainInventory_count(36)   // PORT: array-length companions must be set or loop bounds are garbage
  ```
