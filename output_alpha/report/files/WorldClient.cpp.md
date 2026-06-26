# WorldClient.cpp

Risk score: **6**  ·  7 findings

## WARNING — suspicious; verify by hand

- **🟡 MED** `L126` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L130` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L5` (marker, `PORT`): SINGLEPLAYER 360 port. WorldClient is the MP client world; SP never
  ```cpp
  // PORT: SINGLEPLAYER 360 port. WorldClient is the MP client world; SP never
  ```
- **⚪ LOW** `L114` (marker, `PORT`): was java.io.File */* var1) {
  ```cpp
  IChunkProvider* WorldClient::func_4081_a(std::string /* PORT: was java.io.File */* var1) {
  ```
- **⚪ LOW** `L255` (marker, `PORT`): 360 MP API TODO -- Packet59ComplexEntity (no-op).
  ```cpp
  // PORT: 360 MP API TODO -- Packet59ComplexEntity (no-op).
  ```
- **⚪ LOW** `L260` (marker, `PORT`): 360 MP API TODO -- Packet255KickDisconnect("Quitting") (no-op).
  ```cpp
  // PORT: 360 MP API TODO -- Packet255KickDisconnect("Quitting") (no-op).
  ```
