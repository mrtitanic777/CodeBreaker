# TileEntityFurnace.h

Risk score: **2**  ·  2 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L37` (marker, `PORT`): satisfies IInventory pure-virtual
  ```cpp
  virtual void onInventoryChanged();  // PORT: satisfies IInventory pure-virtual
  ```
