# HashEntry.h

Risk score: **0**  ·  4 findings

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L12` (marker, `PORT`): was Object */ var3, HashEntry* var4);
  ```cpp
  HashEntry(int var1, int var2, void* /* PORT: was Object */ var3, HashEntry* var4);
  ```
- **⚪ LOW** `L16` (marker, `PORT`): was Object */ getValue();
  ```cpp
  virtual void* /* PORT: was Object */ getValue();
  ```
- **⚪ LOW** `L17` (marker, `PORT`): was Object */ var1);
  ```cpp
  virtual bool equals(void* /* PORT: was Object */ var1);
  ```
- **⚪ LOW** `L23` (marker, `PORT`): was Object */ m_valueEntry;
  ```cpp
  void* /* PORT: was Object */ m_valueEntry;
  ```
