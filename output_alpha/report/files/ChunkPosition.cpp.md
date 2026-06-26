# ChunkPosition.cpp

Risk score: **2**  ·  3 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L29` (marker, `PORT`): was Object */ var1) {
  ```cpp
  bool ChunkPosition::equals(void* /* PORT: was Object */ var1) {
  ```
- **⚪ LOW** `L30` (marker, `PORT`): Java did `var1 instanceof ChunkPosition`. The param is void* (RTTI
  ```cpp
  // PORT: Java did `var1 instanceof ChunkPosition`. The param is void* (RTTI
  ```
