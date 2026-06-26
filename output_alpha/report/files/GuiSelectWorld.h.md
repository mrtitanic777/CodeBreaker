# GuiSelectWorld.h

Risk score: **2**  ·  2 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L32` (marker, `PORT`): backing storage for getWorldName ref return
  ```cpp
  std::string m_worldNameBuf; // PORT: backing storage for getWorldName ref return
  ```
