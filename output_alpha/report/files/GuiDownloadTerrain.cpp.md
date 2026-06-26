# GuiDownloadTerrain.cpp

Risk score: **6**  ·  5 findings

## WARNING — suspicious; verify by hand

- **🟡 MED** `L32` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L52` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L5` (marker, `PORT`): SINGLEPLAYER 360 port. Keep-alive packet sending is stubbed; this
  ```cpp
  // PORT: SINGLEPLAYER 360 port. Keep-alive packet sending is stubbed; this
  ```
- **⚪ LOW** `L42` (marker, `PORT`): 360 MP API TODO -- keep-alive packet (no-op).
  ```cpp
  // PORT: 360 MP API TODO -- keep-alive packet (no-op).
  ```
