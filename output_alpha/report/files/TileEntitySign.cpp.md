# TileEntitySign.cpp

Risk score: **14**  ·  5 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L32` (marker, `PORT`): base TileEntity::writeToNBT now needs an id (no reflection on 360);
  ```cpp
  // PORT: base TileEntity::writeToNBT now needs an id (no reflection on 360);
  ```
- **🔴 HIGH** `L48` (marker, `PORT`): Java was "Text" + (var2 + 1) (string concat). C++ needs explicit
  ```cpp
  // PORT: Java was "Text" + (var2 + 1) (string concat). C++ needs explicit
  ```

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L13` (rule, `rule-of-three`): TileEntitySign owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L17` (marker, `PORT(array-literal field init)`): C++03 has no init-list array fill; alloc + fill here.
  ```cpp
  // PORT(array-literal field init): C++03 has no init-list array fill; alloc + fill here.
  ```
