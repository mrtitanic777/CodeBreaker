# ModelSkeleton.cpp

Risk score: **2**  ·  2 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L14` (rule, `rule-of-three`): ModelSkeleton owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for ModelSkeleton
