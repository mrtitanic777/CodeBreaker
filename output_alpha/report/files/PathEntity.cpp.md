# PathEntity.cpp

Risk score: **7**  ·  2 findings

## RISK — matches a known console-crash / leak pattern

- **🔴 HIGH** `L46` (lint, `leak/new-leak`): `new` whose result is discarded/passed -> Java GC'd this; C++ leaks (TNT-stack OOM class). Use a stack value or delete it.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)
