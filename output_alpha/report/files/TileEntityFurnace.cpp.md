# TileEntityFurnace.cpp

Risk score: **9**  ·  7 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L100` (marker, `PORT`): base TileEntity::writeToNBT now needs an id; write it explicitly
  ```cpp
  // PORT: base TileEntity::writeToNBT now needs an id; write it explicitly
  ```

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L223` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L28` (marker, `PORT`): array-length companion or loop bounds are garbage
  ```cpp
  , m_furnaceItemStacks_count(3)   // PORT: array-length companion or loop bounds are garbage
  ```
- **⚪ LOW** `L74` (marker, `PORT`): return a reference to a stable static (Java returned a literal).
  ```cpp
  // PORT: return a reference to a stable static (Java returned a literal).
  ```
- **⚪ LOW** `L181` (marker, `PORT`): onInventoryChanged() is declared in BOTH TileEntity and IInventory
  ```cpp
  // PORT: onInventoryChanged() is declared in BOTH TileEntity and IInventory
  ```
- **⚪ LOW** `L227` (marker, `PORT`): declared in TileEntityFurnace.h to make the class concrete (IInventory's
  ```cpp
  // PORT: declared in TileEntityFurnace.h to make the class concrete (IInventory's
  ```
