# Pathfinder.cpp

Risk score: **15**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🔴 HIGH** `L20` (rule, `uninit-array`): new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.
  ```cpp
  , m_pathOptions(new PathPoint*[32])
  ```
- **🔴 HIGH** `L28` (rule, `uninit-array`): new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.
  ```cpp
  , m_pathOptions(new PathPoint*[32])
  ```
- **🔴 HIGH** `L210` (rule, `uninit-array`): new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.
  ```cpp
  PathPoint** var5 = new PathPoint*[var3];
  ```
