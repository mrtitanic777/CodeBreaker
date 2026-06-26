# AxisAlignedBB.cpp

Risk score: **17**  ·  4 findings

## GUESSED — converter made a heuristic call that may be wrong -- INSPECT

- **🔴 HIGH** `L10` (marker, `PORT`): AxisAlignedBB is a VALUE type but Java pooled instances. The pool stores
  ```cpp
  // PORT: AxisAlignedBB is a VALUE type but Java pooled instances. The pool stores
  ```

## RISK — matches a known console-crash / leak pattern

- **🔴 HIGH** `L55` (lint, `leak/new-leak`): `new` whose result is discarded/passed -> Java GC'd this; C++ leaks (TNT-stack OOM class). Use a stack value or delete it.
- **🔴 HIGH** `L313` (lint, `leak/new-leak`): `new` whose result is discarded/passed -> Java GC'd this; C++ leaks (TNT-stack OOM class). Use a stack value or delete it.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)
