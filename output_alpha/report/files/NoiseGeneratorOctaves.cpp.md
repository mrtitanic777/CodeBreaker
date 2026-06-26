# NoiseGeneratorOctaves.cpp

Risk score: **4**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L16` (rule, `rule-of-three`): NoiseGeneratorOctaves owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
- **🟡 MED** `L49` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  var1 = new double[var8 * var9 * var10];
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L52` (marker, `PORT`): set this from allocation */; ++var17) {
  ```cpp
  for(int var17 = 0; var17 < var1_count /* PORT: set this from allocation */; ++var17) {
  ```
