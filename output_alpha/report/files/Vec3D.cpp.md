# Vec3D.cpp

Risk score: **7**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🔴 HIGH** `L47` (lint, `leak/new-leak`): `new` whose result is discarded/passed -> Java GC'd this; C++ leaks (TNT-stack OOM class). Use a stack value or delete it.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L148` (marker, `PORT`): Java string concat. Return a reference into a static buffer so the
  ```cpp
  // PORT: Java string concat. Return a reference into a static buffer so the
  ```
