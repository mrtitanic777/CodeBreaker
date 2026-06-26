# ChunkProviderLoadOrGenerate.cpp

Risk score: **6**  ·  4 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L65` (rule, `rule-of-three`): ChunkProviderLoadOrGenerate owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
- **🟡 MED** `L93` (rule, `uninit-array`): new unsigned char[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new unsigned char[N]() (the new bool[10] phantom-input bug).
  ```cpp
  m_blankChunk = new Chunk(var1, new unsigned char[32768], 0, 0);
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L12` (marker, `PORT NOTE`): (m_chunks): the Java field is `Chunk[] m_chunks` -- an array of 1024
  ```cpp
  // PORT NOTE (m_chunks): the Java field is `Chunk[] m_chunks` -- an array of 1024
  ```
