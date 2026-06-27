# CanvasIsomPreview.cpp

Risk score: **4**  ·  2 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L143` (rule, `rule-of-three`): CanvasIsomPreview owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (pipeline, `override`): handwritten source override used
