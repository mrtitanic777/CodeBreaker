# ModelQuadraped.cpp

Risk score: **2**  ·  1 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L19` (rule, `rule-of-three`): ModelQuadraped owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
