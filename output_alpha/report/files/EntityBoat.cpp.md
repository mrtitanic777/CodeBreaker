# EntityBoat.cpp

Risk score: **6**  ·  4 findings

## WARNING — suspicious; verify by hand

- **🟡 MED** `L351` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L355` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L71` (marker, `PORT`): Java did this(var1); replicate the World* ctor setup (no C++03 delegation).
  ```cpp
  // PORT: Java did this(var1); replicate the World* ctor setup (no C++03 delegation).
  ```
