# TexureWaterFlowFX.cpp

Risk score: **8**  ·  4 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L12` (rule, `uninit-array`): new float[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new float[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_1138_g(new float[256])
  ```
- **🟡 MED** `L13` (rule, `uninit-array`): new float[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new float[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_1137_h(new float[256])
  ```
- **🟡 MED** `L14` (rule, `uninit-array`): new float[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new float[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_1136_i(new float[256])
  ```
- **🟡 MED** `L15` (rule, `uninit-array`): new float[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new float[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_1135_j(new float[256])
  ```
