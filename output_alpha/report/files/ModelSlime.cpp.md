# ModelSlime.cpp

Risk score: **4**  ·  2 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L17` (rule, `rule-of-three`): ModelSlime owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.

## WARNING — suspicious; verify by hand

- **🟡 MED** `L46` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
