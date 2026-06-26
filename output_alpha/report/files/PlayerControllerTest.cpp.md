# PlayerControllerTest.cpp

Risk score: **4**  ·  4 findings

## WARNING — suspicious; verify by hand

- **🟡 MED** `L65` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L16` (marker, `PORT`): below.
  ```cpp
  //     slot the block whose id == var2 + 1 (stone, grass, dirt, ...). PORT below.
  ```
- **⚪ LOW** `L46` (marker, `PORT`): creative palette was Session.registeredBlocksList[var2] (stripped on
  ```cpp
  // PORT: creative palette was Session.registeredBlocksList[var2] (stripped on
  ```
