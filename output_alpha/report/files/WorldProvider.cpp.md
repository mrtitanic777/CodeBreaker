# WorldProvider.cpp

Risk score: **4**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L24` (rule, `uninit-array`): new float[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new float[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_lightBrightnessTable(new float[16])
  ```
- **🟡 MED** `L26` (rule, `uninit-array`): new float[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new float[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_4217_f(new float[4])
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L63` (marker, `PORT`): was java.io.File */* var1) {
  ```cpp
  IChunkLoader* WorldProvider::getChunkLoader(std::string /* PORT: was java.io.File */* var1) {
  ```
