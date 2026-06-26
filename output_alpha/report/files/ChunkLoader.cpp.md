# ChunkLoader.cpp

Risk score: **4**  ·  9 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L269` (rule, `uninit-array`): new unsigned char[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new unsigned char[N]() (the new bool[10] phantom-input bug).
  ```cpp
  var4->m_heightMap = new unsigned char[256];
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L7` (marker, `PORT NOTES`): PORT NOTES:
  ```cpp
  // PORT NOTES:
  ```
- **⚪ LOW** `L35` (marker, `PORT`): Integer.toString(value, 36) — Java base-36 string (handles negatives).
  ```cpp
  // PORT: Integer.toString(value, 36) — Java base-36 string (handles negatives).
  ```
- **⚪ LOW** `L75` (marker, `PORT`): was java.io.File */* var1, bool var2)
  ```cpp
  ChunkLoader::ChunkLoader(std::string /* PORT: was java.io.File */* var1, bool var2)
  ```
- **⚪ LOW** `L90` (marker, `PORT`): was java.io.File */* ChunkLoader::chunkFileForXZ(int var1, int var2) {
  ```cpp
  std::string /* PORT: was java.io.File */* ChunkLoader::chunkFileForXZ(int var1, int var2) {
  ```
- **⚪ LOW** `L130` (marker, `PORT`): read the raw NBT bytes from the file, then parse via the
  ```cpp
  // PORT: read the raw NBT bytes from the file, then parse via the
  ```
- **⚪ LOW** `L181` (marker, `PORT`): createIfNecessary was false and the dirs don't exist -> nothing to do.
  ```cpp
  // PORT: createIfNecessary was false and the dirs don't exist -> nothing to do.
  ```
- **⚪ LOW** `L185` (marker, `PORT`): the JavaIO File bridge has no length()/renameTo(); Java's
  ```cpp
  // PORT: the JavaIO File bridge has no length()/renameTo(); Java's
  ```
