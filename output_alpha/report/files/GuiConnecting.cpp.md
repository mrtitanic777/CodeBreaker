# GuiConnecting.cpp

Risk score: **4**  ·  4 findings

## WARNING — suspicious; verify by hand

- **🟡 MED** `L48` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L5` (marker, `PORT`): SINGLEPLAYER 360 port. The connect screen normally spawns a
  ```cpp
  // PORT: SINGLEPLAYER 360 port. The connect screen normally spawns a
  ```
- **⚪ LOW** `L30` (marker, `PORT`): 360 MP API TODO -- (new ThreadConnectToServer(...))->start();
  ```cpp
  // PORT: 360 MP API TODO -- (new ThreadConnectToServer(...))->start();
  ```
