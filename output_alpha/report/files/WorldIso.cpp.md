# WorldIso.cpp

Risk score: **2**  ·  3 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L21` (marker, `PORT`): was java.io.File */* var2, const std::string& var3)
  ```cpp
  WorldIso::WorldIso(CanvasIsomPreview* var1, std::string /* PORT: was java.io.File */* var2, const std::string& var3)
  ```
- **⚪ LOW** `L32` (marker, `PORT`): was java.io.File */* var1) {
  ```cpp
  IChunkProvider* WorldIso::func_4081_a(std::string /* PORT: was java.io.File */* var1) {
  ```
