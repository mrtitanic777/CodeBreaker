# TextureLavaFlowFX.cpp

Risk score: **8**  ·  4 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L12` (rule, `uninit-array`): new float[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new float[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_1143_g(new float[256])
  ```
- **🟡 MED** `L13` (rule, `uninit-array`): new float[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new float[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_1142_h(new float[256])
  ```
- **🟡 MED** `L14` (rule, `uninit-array`): new float[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new float[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_1141_i(new float[256])
  ```
- **🟡 MED** `L15` (rule, `uninit-array`): new float[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new float[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_1140_j(new float[256])
  ```
