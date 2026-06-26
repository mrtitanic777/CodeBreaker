# NBTTagCompound.cpp

Risk score: **4**  ·  2 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L135` (rule, `uninit-array`): new unsigned char[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new unsigned char[N]() (the new bool[10] phantom-input bug).
  ```cpp
  return !m_tagMap->count(var1) ? new unsigned char[0] : ((NBTTagByteArray*)(*m_tagMap)[var1])->m_byteArray;
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)
