# ChunkProviderIso.cpp

Risk score: **18**  ·  6 findings

## RISK — matches a known console-crash / leak pattern

- **🔴 HIGH** `L16` (rule, `uninit-array`): new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.
  ```cpp
  : m_chunks(new Chunk*[256])
  ```
- **🟡 MED** `L19` (rule, `uninit-array`): new unsigned char[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new unsigned char[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_899_a(new unsigned char[-(-32768)])
  ```
- **🔴 HIGH** `L25` (rule, `uninit-array`): new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.
  ```cpp
  : m_chunks(new Chunk*[256])
  ```
- **🟡 MED** `L28` (rule, `uninit-array`): new unsigned char[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new unsigned char[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_899_a(new unsigned char[-(-32768)])
  ```

## WARNING — suspicious; verify by hand

- **🟡 MED** `L81` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)
