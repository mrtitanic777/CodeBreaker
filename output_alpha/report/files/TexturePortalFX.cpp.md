# TexturePortalFX.cpp

Risk score: **2**  ·  2 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L12` (rule, `rule-of-three`): TexturePortalFX owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L15` (marker, `PORT`): allocate byte[32][1024] */)
  ```cpp
  , m_field_4226_h(NULL /* PORT: allocate byte[32][1024] */)
  ```
