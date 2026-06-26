# ChunkCache.cpp

Risk score: **11**  ·  4 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L28` (rule, `rule-of-three`): ChunkCache owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
- **🔴 HIGH** `L48` (rule, `uninit-array`): new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.
  ```cpp
  m_field_1062_c[_r] = new Chunk*[cols];
  ```
- **🟡 MED** `L164` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)
