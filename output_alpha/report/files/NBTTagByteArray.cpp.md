# NBTTagByteArray.cpp

Risk score: **2**  ·  1 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L34` (rule, `uninit-array`): new unsigned char[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new unsigned char[N]() (the new bool[10] phantom-input bug).
  ```cpp
  m_byteArray = new unsigned char[var2];
  ```
