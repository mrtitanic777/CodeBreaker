# ChunkProviderClient.cpp

Risk score: **10**  ·  5 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L40` (rule, `rule-of-three`): ChunkProviderClient owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
- **🟡 MED** `L45` (rule, `uninit-array`): new unsigned char[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new unsigned char[N]() (the new bool[10] phantom-input bug).
  ```cpp
  m_blankChunk = new Chunk(var1, new unsigned char[32768], 0, 0);
  ```
- **🟡 MED** `L73` (rule, `uninit-array`): new unsigned char[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new unsigned char[N]() (the new bool[10] phantom-input bug).
  ```cpp
  unsigned char* var4 = new unsigned char[32768];
  ```

## WARNING — suspicious; verify by hand

- **🟡 MED** `L103` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)
