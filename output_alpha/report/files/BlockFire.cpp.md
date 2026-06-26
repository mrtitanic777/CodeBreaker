# BlockFire.cpp

Risk score: **8**  ·  4 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L17` (rule, `uninit-array`): new int[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new int[N]() (the new bool[10] phantom-input bug).
  ```cpp
  : m_chanceToEncourageFire(new int[256])
  ```
- **🟡 MED** `L18` (rule, `uninit-array`): new int[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new int[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_abilityToCatchFire(new int[256])
  ```
- **🟡 MED** `L24` (rule, `uninit-array`): new int[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new int[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_chanceToEncourageFire(new int[256])
  ```
- **🟡 MED** `L25` (rule, `uninit-array`): new int[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new int[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_abilityToCatchFire(new int[256])
  ```
