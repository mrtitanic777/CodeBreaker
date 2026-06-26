# EntityTNTPrimed.cpp

Risk score: **2**  ·  2 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L30` (marker, `PORT`): Java did this(var1); C++03 has no delegating ctors, so replicate
  ```cpp
  // PORT: Java did this(var1); C++03 has no delegating ctors, so replicate
  ```
