# ModelGhast.cpp

Risk score: **2**  ·  4 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L10` (rule, `rule-of-three`): ModelGhast owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L20` (marker, `PORT`): set this from allocation */; ++var3) {
  ```cpp
  for(int var3 = 0; var3 < m_tentacles_count /* PORT: set this from allocation */; ++var3) {
  ```
- **⚪ LOW** `L40` (marker, `PORT`): set this from allocation */; ++var7) {
  ```cpp
  for(int var7 = 0; var7 < m_tentacles_count /* PORT: set this from allocation */; ++var7) {
  ```
- **⚪ LOW** `L50` (marker, `PORT`): set this from allocation */; ++var7) {
  ```cpp
  for(int var7 = 0; var7 < m_tentacles_count /* PORT: set this from allocation */; ++var7) {
  ```
