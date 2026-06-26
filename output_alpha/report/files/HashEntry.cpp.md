# HashEntry.cpp

Risk score: **2**  ·  5 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L19` (marker, `PORT`): was Object */ var3, HashEntry* var4)
  ```cpp
  HashEntry::HashEntry(int var1, int var2, void* /* PORT: was Object */ var3, HashEntry* var4)
  ```
- **⚪ LOW** `L40` (marker, `PORT`): was Object */ HashEntry::getValue() {
  ```cpp
  void* /* PORT: was Object */ HashEntry::getValue() {
  ```
- **⚪ LOW** `L44` (marker, `PORT`): was Object */ var1) {
  ```cpp
  bool HashEntry::equals(void* /* PORT: was Object */ var1) {
  ```
- **⚪ LOW** `L64` (marker, `PORT`): MCHashTable::getHash is a non-static member here; replicate the
  ```cpp
  // PORT: MCHashTable::getHash is a non-static member here; replicate the
  ```
