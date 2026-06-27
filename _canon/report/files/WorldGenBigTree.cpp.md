# WorldGenBigTree.cpp

Risk score: **6**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L54` (rule, `uninit-array`): new int[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new int[N]() (the new bool[10] phantom-input bug).
  ```cpp
  *      int[][] var2 = new int[var1 * this.heightLimit][4];
  ```
- **🟡 MED** `L56` (rule, `uninit-array`): new int[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new int[N]() (the new bool[10] phantom-input bug).
  ```cpp
  *      this.leafNodes = new int[var4][4];
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (pipeline, `override`): handwritten source override used
