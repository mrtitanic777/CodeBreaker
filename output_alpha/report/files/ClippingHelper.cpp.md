# ClippingHelper.cpp

Risk score: **6**  ·  4 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L10` (rule, `uninit-array`): new float[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new float[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_1687_b(new float[16])
  ```
- **🟡 MED** `L11` (rule, `uninit-array`): new float[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new float[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_1690_c(new float[16])
  ```
- **🟡 MED** `L12` (rule, `uninit-array`): new float[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new float[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_1689_d(new float[16])
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L9` (marker, `PORT`): allocate float[16][16] */)
  ```cpp
  : m_field_1688_a(NULL /* PORT: allocate float[16][16] */)
  ```
