# MCHashTable.cpp

Risk score: **14**  ·  10 findings

## RISK — matches a known console-crash / leak pattern

- **🔴 HIGH** `L39` (rule, `uninit-array`): new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.
  ```cpp
  slots = new HashEntry*[16];
  ```
- **🟡 MED** `L79` (rule, `rule-of-three`): MCHashTable owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
- **🔴 HIGH** `L143` (rule, `uninit-array`): new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.
  ```cpp
  HashEntry** var4 = new HashEntry*[var1];
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L8` (marker, `PORT NOTE`): the auto-generated MCHashTable.h dropped the transient fields
  ```cpp
  // PORT NOTE: the auto-generated MCHashTable.h dropped the transient fields
  ```
- **⚪ LOW** `L109` (marker, `PORT`): was Object */ MCHashTable::lookup(int var1) {
  ```cpp
  void* /* PORT: was Object */ MCHashTable::lookup(int var1) {
  ```
- **⚪ LOW** `L122` (marker, `PORT`): was Object */ var2) {
  ```cpp
  void MCHashTable::addKey(int var1, void* /* PORT: was Object */ var2) {
  ```
- **⚪ LOW** `L157` (marker, `PORT`): the header declares copyTo(HashEntry*) but it semantically takes the
  ```cpp
  // PORT: the header declares copyTo(HashEntry*) but it semantically takes the
  ```
- **⚪ LOW** `L182` (marker, `PORT`): was Object */ MCHashTable::removeObject(int var1) {
  ```cpp
  void* /* PORT: was Object */ MCHashTable::removeObject(int var1) {
  ```
- **⚪ LOW** `L233` (marker, `PORT`): was Object */ var3, int var4) {
  ```cpp
  void MCHashTable::insert(int var1, int var2, void* /* PORT: was Object */ var3, int var4) {
  ```
