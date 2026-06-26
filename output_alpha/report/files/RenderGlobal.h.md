# RenderGlobal.h

Risk score: **15**  ·  5 findings

## GUESSED — converter made a heuristic call that may be wrong -- INSPECT

- **🔴 HIGH** `L32` (marker, `PORT`): determine element type */ m_field_1458_a;
  ```cpp
  std::vector<void*> /* PORT: determine element type */ m_field_1458_a;
  ```
- **🔴 HIGH** `L64` (marker, `PORT`): determine element type */ m_field_1446_m;
  ```cpp
  std::vector<void*> /* PORT: determine element type */ m_field_1446_m;
  ```
- **🔴 HIGH** `L97` (marker, `PORT`): determine element type */ m_field_1415_R;
  ```cpp
  std::vector<void*> /* PORT: determine element type */ m_field_1415_R;
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L73` (marker, `PORT`): was IntBuffer */ m_field_1437_v;
  ```cpp
  int* /* PORT: was IntBuffer */ m_field_1437_v;
  ```
- **⚪ LOW** `L91` (marker, `PORT`): was IntBuffer */ m_field_1456_c;
  ```cpp
  int* /* PORT: was IntBuffer */ m_field_1456_c;
  ```
