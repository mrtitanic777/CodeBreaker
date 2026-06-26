# EntityArrow.cpp

Risk score: **5**  ·  1 findings

## RISK — matches a known console-crash / leak pattern

- **🔴 HIGH** `L190` (lint, `leak/new-leak`): `new` whose result is discarded/passed -> Java GC'd this; C++ leaks (TNT-stack OOM class). Use a stack value or delete it.
