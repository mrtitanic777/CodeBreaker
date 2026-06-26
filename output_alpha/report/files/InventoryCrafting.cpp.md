# InventoryCrafting.cpp

Risk score: **4**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L18` (rule, `rule-of-three`): InventoryCrafting owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.

## WARNING — suspicious; verify by hand

- **🟡 MED** `L89` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L34` (marker, `PORT`): quad vertex count */;
  ```cpp
  m_nbrSlots = 4 /* PORT: quad vertex count */;
  ```
