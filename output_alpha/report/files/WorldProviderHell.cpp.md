# WorldProviderHell.cpp

Risk score: **2**  ·  4 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L10` (marker, `PORT NOTES`): PORT NOTES:
  ```cpp
  // PORT NOTES:
  ```
- **⚪ LOW** `L43` (marker, `PORT`): MobSpawnerBase.hell singleton unavailable -> NULL biome (see header note).
  ```cpp
  // PORT: MobSpawnerBase.hell singleton unavailable -> NULL biome (see header note).
  ```
- **⚪ LOW** `L69` (marker, `PORT`): was java.io.File */* var1) {
  ```cpp
  IChunkLoader* WorldProviderHell::getChunkLoader(std::string /* PORT: was java.io.File */* var1) {
  ```
