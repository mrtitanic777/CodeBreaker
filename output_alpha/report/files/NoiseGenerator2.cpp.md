# NoiseGenerator2.cpp

Risk score: **4**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L22` (rule, `rule-of-three`): NoiseGenerator2 owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
- **🟡 MED** `L28` (rule, `uninit-array`): new int[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new int[N]() (the new bool[10] phantom-input bug).
  ```cpp
  m_field_4295_e = new int[512];
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L19` (marker, `PORT`): constructor delegation  // was: this(new Random());
  ```cpp
  // PORT: constructor delegation  // was: this(new Random());
  ```
