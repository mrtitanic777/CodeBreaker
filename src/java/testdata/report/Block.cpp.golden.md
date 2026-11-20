# Block.cpp

Risk score: **7**  ·  3 findings

## GUESSED — converter made a heuristic call that may be wrong -- INSPECT

- **🟡 MED** `L3` (marker, `PORT`): value type assumed
  ```cpp
  // PORT: assumed by value
  ```

## RISK — matches a known console-crash / leak pattern

- **🔴 HIGH** `L12` (rule, `uninit-array`): new bool[] without ()
  ```cpp
  bool* a = new bool[16];
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `—` (marker, `PORT NOTE`): a note
