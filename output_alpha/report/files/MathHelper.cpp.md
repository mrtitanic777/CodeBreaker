# MathHelper.cpp

Risk score: **2**  ·  1 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L8` (rule, `uninit-array`): new float[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new float[N]() (the new bool[10] phantom-input bug).
  ```cpp
  float* MathHelper::s_SIN_TABLE = new float[65536];
  ```
