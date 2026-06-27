# EnumOSMappingHelper.cpp

Risk score: **4**  ·  2 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L11` (rule, `uninit-array`): new int[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new int[N]() (the new bool[10] phantom-input bug).
  ```cpp
  *  public static final int[] osMappings = new int[EnumOS2.values().length];
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (pipeline, `override`): handwritten source override used
