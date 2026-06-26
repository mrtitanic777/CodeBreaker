# MCHashTable.h

Risk score: **0**  ·  4 findings

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L14` (marker, `PORT`): was Object */ lookup(int var1);
  ```cpp
  virtual void* /* PORT: was Object */ lookup(int var1);
  ```
- **⚪ LOW** `L15` (marker, `PORT`): was Object */ var2);
  ```cpp
  virtual void addKey(int var1, void* /* PORT: was Object */ var2);
  ```
- **⚪ LOW** `L16` (marker, `PORT`): was Object */ removeObject(int var1);
  ```cpp
  virtual void* /* PORT: was Object */ removeObject(int var1);
  ```
- **⚪ LOW** `L28` (marker, `PORT`): was Object */ var3, int var4);
  ```cpp
  void insert(int var1, int var2, void* /* PORT: was Object */ var3, int var4);
  ```
