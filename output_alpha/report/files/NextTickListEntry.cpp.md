# NextTickListEntry.cpp

Risk score: **2**  ·  3 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L38` (marker, `PORT`): was Object */ var1) {
  ```cpp
  bool NextTickListEntry::equals(void* /* PORT: was Object */ var1) {
  ```
- **⚪ LOW** `L60` (marker, `PORT`): was Object */ var1) {
  ```cpp
  int NextTickListEntry::compareTo(void* /* PORT: was Object */ var1) {
  ```
