# ChunkProviderHell.cpp

Risk score: **22**  ·  12 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L49` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_4162_p(new double[256])
  ```
- **🟡 MED** `L50` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_4161_q(new double[256])
  ```
- **🟡 MED** `L51` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_4160_r(new double[256])
  ```
- **🟡 MED** `L70` (rule, `rule-of-three`): ChunkProviderHell owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
- **🟡 MED** `L81` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_4162_p(new double[256])
  ```
- **🟡 MED** `L82` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_4161_q(new double[256])
  ```
- **🟡 MED** `L83` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_4160_r(new double[256])
  ```
- **🟡 MED** `L283` (rule, `uninit-array`): new unsigned char[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new unsigned char[N]() (the new bool[10] phantom-input bug).
  ```cpp
  unsigned char* var3 = new unsigned char[-(-32768)];
  ```
- **🟡 MED** `L295` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  var1 = new double[var5 * var6 * var7];
  ```
- **🟡 MED** `L307` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  double* var14 = new double[var6];
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L10` (marker, `PORT NOTES`): PORT NOTES:
  ```cpp
  // PORT NOTES:
  ```
