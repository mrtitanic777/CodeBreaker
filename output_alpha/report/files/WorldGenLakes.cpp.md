# WorldGenLakes.cpp

Risk score: **7**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🔴 HIGH** `L37` (rule, `uninit-array`): new bool[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new bool[N]() (the new bool[10] phantom-input bug).
  ```cpp
  bool* var6 = new bool[2048];
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L38` (marker, `PORT`): zero-init the lake voxel mask (Java new boolean[] is zeroed; C++ new bool[] is not).
  ```cpp
  // PORT: zero-init the lake voxel mask (Java new boolean[] is zeroed; C++ new bool[] is not).
  ```
