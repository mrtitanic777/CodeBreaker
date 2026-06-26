# TextureFX.cpp

Risk score: **6**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L11` (rule, `uninit-array`): new unsigned char[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new unsigned char[N]() (the new bool[10] phantom-input bug).
  ```cpp
  : m_field_1127_a(new unsigned char[1024])
  ```
- **🟡 MED** `L21` (rule, `uninit-array`): new unsigned char[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new unsigned char[N]() (the new bool[10] phantom-input bug).
  ```cpp
  : m_field_1127_a(new unsigned char[1024])
  ```

## WARNING — suspicious; verify by hand

- **🟡 MED** `L37` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
