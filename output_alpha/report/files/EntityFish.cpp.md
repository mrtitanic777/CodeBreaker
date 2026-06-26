# EntityFish.cpp

Risk score: **7**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🔴 HIGH** `L310` (lint, `leak/new-leak`): `new` whose result is discarded/passed -> Java GC'd this; C++ leaks (TNT-stack OOM class). Use a stack value or delete it.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L101` (marker, `PORT`): Java did this(var1); replicate the World* ctor setup (no C++03 delegation).
  ```cpp
  // PORT: Java did this(var1); replicate the World* ctor setup (no C++03 delegation).
  ```
