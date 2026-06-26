# EntityOtherPlayerMP.cpp

Risk score: **2**  ·  3 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L5` (marker, `PORT`): SINGLEPLAYER 360 port. This is the remote-player avatar; in SP it is
  ```cpp
  // PORT: SINGLEPLAYER 360 port. This is the remote-player avatar; in SP it is
  ```
- **⚪ LOW** `L40` (marker, `PORT`): Java was 'var2 != null && var2.length() > 0'; a std::string ref
  ```cpp
  // PORT: Java was 'var2 != null && var2.length() > 0'; a std::string ref
  ```
