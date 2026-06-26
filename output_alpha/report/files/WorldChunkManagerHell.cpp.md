# WorldChunkManagerHell.cpp

Risk score: **13**  ·  5 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L55` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  var1 = new double[var4 * var5];
  ```
- **🔴 HIGH** `L68` (rule, `uninit-array`): new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.
  ```cpp
  var1 = new MobSpawnerBase*[var4 * var5];
  ```
- **🟡 MED** `L71` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  m_temperature = new double[var4 * var5];
  ```
- **🟡 MED** `L74` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  m_humidity = new double[var4 * var5];
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)
