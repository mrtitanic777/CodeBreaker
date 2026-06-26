# TileEntityChest.cpp

Risk score: **2**  ·  4 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L26` (marker, `PORT`): array-length companion (Java new ItemStack[36])
  ```cpp
  , m_field_827_a_count(36)   // PORT: array-length companion (Java new ItemStack[36])
  ```
- **⚪ LOW** `L72` (marker, `PORT`): return a reference to a stable static (Java returned a literal).
  ```cpp
  // PORT: return a reference to a stable static (Java returned a literal).
  ```
- **⚪ LOW** `L113` (marker, `PORT`): IInventory::onInventoryChanged() is pure-virtual; TileEntity has a
  ```cpp
  // PORT: IInventory::onInventoryChanged() is pure-virtual; TileEntity has a
  ```
