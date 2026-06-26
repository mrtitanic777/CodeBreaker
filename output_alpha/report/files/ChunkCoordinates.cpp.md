# ChunkCoordinates.cpp

Risk score: **2**  ·  3 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L26` (marker, `PORT`): was Object */ var1) {
  ```cpp
  bool ChunkCoordinates::equals(void* /* PORT: was Object */ var1) {
  ```
- **⚪ LOW** `L27` (marker, `PORT`): Java did `var1 instanceof ChunkCoordinates`. The param is void*
  ```cpp
  // PORT: Java did `var1 instanceof ChunkCoordinates`. The param is void*
  ```
