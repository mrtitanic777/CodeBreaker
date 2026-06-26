# MovementInputFromOptions.cpp

Risk score: **10**  ·  2 findings

## RISK — matches a known console-crash / leak pattern

- **🔴 HIGH** `L11` (rule, `uninit-array`): new bool[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new bool[N]() (the new bool[10] phantom-input bug).
  ```cpp
  : m_field_1179_f(new bool[10])
  ```
- **🔴 HIGH** `L17` (rule, `uninit-array`): new bool[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new bool[N]() (the new bool[10] phantom-input bug).
  ```cpp
  : m_field_1179_f(new bool[10])
  ```
