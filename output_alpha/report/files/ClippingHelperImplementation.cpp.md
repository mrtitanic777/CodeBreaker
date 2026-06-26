# ClippingHelperImplementation.cpp

Risk score: **11**  ·  6 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L13` (rule, `rule-of-three`): ClippingHelperImplementation owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
- **🔴 HIGH** `L20` (rule, `uninit-array`): new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.
  ```cpp
  m_field_1688_a = new float*[6];
  ```
- **🟡 MED** `L22` (rule, `uninit-array`): new float[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new float[N]() (the new bool[10] phantom-input bug).
  ```cpp
  m_field_1688_a[i] = new float[4];
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L18` (marker, `PORT`): base ClippingHelper leaves m_field_1688_a (the 6x4 frustum-plane matrix)
  ```cpp
  // PORT: base ClippingHelper leaves m_field_1688_a (the 6x4 frustum-plane matrix)
  ```
- **⚪ LOW** `L52` (marker, `PORT`): Java read GL_PROJECTION_MATRIX and GL_MODELVIEW_MATRIX into NIO
  ```cpp
  // PORT: Java read GL_PROJECTION_MATRIX and GL_MODELVIEW_MATRIX into NIO
  ```
