# ChunkProviderGenerate.cpp

Risk score: **20**  ·  12 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L53` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_905_r(new double[256])
  ```
- **🟡 MED** `L54` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_904_s(new double[256])
  ```
- **🟡 MED** `L55` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_903_t(new double[256])
  ```
- **🟡 MED** `L68` (rule, `rule-of-three`): ChunkProviderGenerate owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
- **🟡 MED** `L80` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_905_r(new double[256])
  ```
- **🟡 MED** `L81` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_904_s(new double[256])
  ```
- **🟡 MED** `L82` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_903_t(new double[256])
  ```
- **🟡 MED** `L287` (rule, `uninit-array`): new unsigned char[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new unsigned char[N]() (the new bool[10] phantom-input bug).
  ```cpp
  unsigned char* var3 = new unsigned char[-(-32768)];
  ```
- **🟡 MED** `L300` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  var1 = new double[var5 * var6 * var7];
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L63` (marker, `PORT`): allocate int[32][32] */)
  ```cpp
  , m_field_914_i(NULL /* PORT: allocate int[32][32] */)
  ```
- **⚪ LOW** `L90` (marker, `PORT`): allocate int[32][32] */)
  ```cpp
  , m_field_914_i(NULL /* PORT: allocate int[32][32] */)
  ```
