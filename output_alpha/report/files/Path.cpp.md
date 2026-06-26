# Path.cpp

Risk score: **10**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🔴 HIGH** `L10` (rule, `uninit-array`): new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.
  ```cpp
  : m_pathPoints(new PathPoint*[1024])
  ```
- **🔴 HIGH** `L25` (rule, `uninit-array`): new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.
  ```cpp
  PathPoint** var2 = new PathPoint*[m_count << 1];
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L24` (marker, `PORT`): quad vertex count */) {
  ```cpp
  if(m_count == 4 /* PORT: quad vertex count */) {
  ```
