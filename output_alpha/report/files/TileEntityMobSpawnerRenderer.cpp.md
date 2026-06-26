# TileEntityMobSpawnerRenderer.cpp

Risk score: **2**  ·  2 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L17` (marker, `PORT`): Java init = new HashMap (was left NULL -> crash on first render)
  ```cpp
  : m_field_1412_b(new std::map<std::string, void*>())  // PORT: Java init = new HashMap (was left NULL -> crash on first render)
  ```
