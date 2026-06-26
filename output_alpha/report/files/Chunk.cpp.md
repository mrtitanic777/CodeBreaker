# Chunk.cpp

Risk score: **26**  ·  11 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L493` (marker, `PORT`): Thread.dumpStack() -- no stack-dump on 360.
  ```cpp
  // PORT: Thread.dumpStack() -- no stack-dump on 360.
  ```
- **🔴 HIGH** `L669` (marker, `PORT`): Java did var1(Class).isAssignableFrom(var9.getClass()) -- no RTTI
  ```cpp
  // PORT: Java did var1(Class).isAssignableFrom(var9.getClass()) -- no RTTI
  ```

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L66` (rule, `rule-of-three`): Chunk owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
- **🟡 MED** `L100` (rule, `uninit-array`): new unsigned char[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new unsigned char[N]() (the new bool[10] phantom-input bug).
  ```cpp
  m_heightMap = new unsigned char[256];
  ```
- **🟡 MED** `L138` (rule, `uninit-array`): new unsigned char[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new unsigned char[N]() (the new bool[10] phantom-input bug).
  ```cpp
  m_heightMap = new unsigned char[256];
  ```
- **🟡 MED** `L382` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L401` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L421` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L437` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L26` (marker, `PORT`): was a HashMap<ChunkPosition,..>)
  ```cpp
  // ChunkPosition into a stable string key. (PORT: was a HashMap<ChunkPosition,..>)
  ```
