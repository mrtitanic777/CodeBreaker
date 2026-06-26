# EntityClientPlayerMP.cpp

Risk score: **6**  ·  11 findings

## WARNING — suspicious; verify by hand

- **🟡 MED** `L66` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L77` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L5` (marker, `PORT`): SINGLEPLAYER 360 port. All packet-sending bodies here are stubbed
  ```cpp
  // PORT: SINGLEPLAYER 360 port. All packet-sending bodies here are stubbed
  ```
- **⚪ LOW** `L88` (marker, `PORT`): 360 MP API TODO -- arm-animation packet (no-op).
  ```cpp
  // PORT: 360 MP API TODO -- arm-animation packet (no-op).
  ```
- **⚪ LOW** `L100` (marker, `PORT`): 360 MP API TODO -- player position/look move packets (no-op).
  ```cpp
  // PORT: 360 MP API TODO -- player position/look move packets (no-op).
  ```
- **⚪ LOW** `L134` (marker, `PORT`): 360 MP API TODO -- player inventory packets (no-op).
  ```cpp
  // PORT: 360 MP API TODO -- player inventory packets (no-op).
  ```
- **⚪ LOW** `L141` (marker, `PORT`): 360 MP API TODO -- pickup-spawn packet (no-op). In SP the item is
  ```cpp
  // PORT: 360 MP API TODO -- pickup-spawn packet (no-op). In SP the item is
  ```
- **⚪ LOW** `L147` (marker, `PORT`): 360 MP API TODO -- chat packet (no-op).
  ```cpp
  // PORT: 360 MP API TODO -- chat packet (no-op).
  ```
- **⚪ LOW** `L153` (marker, `PORT`): 360 MP API TODO -- arm-animation packet (no-op).
  ```cpp
  // PORT: 360 MP API TODO -- arm-animation packet (no-op).
  ```
- **⚪ LOW** `L158` (marker, `PORT`): 360 MP API TODO -- respawn packet (no-op).
  ```cpp
  // PORT: 360 MP API TODO -- respawn packet (no-op).
  ```
