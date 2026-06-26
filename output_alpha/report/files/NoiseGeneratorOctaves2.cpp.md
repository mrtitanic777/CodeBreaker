# NoiseGeneratorOctaves2.cpp

Risk score: **4**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L16` (rule, `rule-of-three`): NoiseGeneratorOctaves2 owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
- **🟡 MED** `L48` (rule, `uninit-array`): new double[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new double[N]() (the new bool[10] phantom-input bug).
  ```cpp
  var1 = new double[var6 * var7];
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L44` (marker, `PORT`): set this from allocation */; ++var16) {
  ```cpp
  for(int var16 = 0; var16 < var1_count /* PORT: set this from allocation */; ++var16) {
  ```
