# WorldChunkManager.cpp

Risk score: **11**  ·  4 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L32` (rule, `rule-of-three`): WorldChunkManager owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
- **🟡 MED** `L85` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  var1 = new double[var4 * var5];
  ```
- **🔴 HIGH** `L118` (rule, `uninit-array`): new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.
  ```cpp
  var1 = new MobSpawnerBase*[var4 * var5];
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)
