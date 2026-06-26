# WorldGenBigTree.cpp

Risk score: **18**  ·  6 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L26` (rule, `rule-of-three`): WorldGenBigTree owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
- **🟡 MED** `L45` (rule, `uninit-array`): new int[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new int[N]() (the new bool[10] phantom-input bug).
  ```cpp
  m_field_879_d = new int[3];
  ```
- **🔴 HIGH** `L85` (rule, `uninit-array`): new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.
  ```cpp
  int** var2 = new int*[var2Cap];
  ```
- **🟡 MED** `L87` (rule, `uninit-array`): new int[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new int[N]() (the new bool[10] phantom-input bug).
  ```cpp
  var2[vi] = new int[4];
  ```
- **🔴 HIGH** `L165` (rule, `uninit-array`): new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.
  ```cpp
  m_field_868_o = new int*[var4 > 0 ? var4 : 1];
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)
