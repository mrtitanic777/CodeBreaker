# BlockSign.cpp

Risk score: **12**  ·  3 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L11` (marker, `PORT`): reflection (newInstance) replaced by explicit construction
  ```cpp
  #include "TileEntitySign.h"  // PORT: reflection (newInstance) replaced by explicit construction
  ```
- **🔴 HIGH** `L90` (marker, `PORT`): Java did m_signEntityClass.newInstance() via reflection. No RTTI
  ```cpp
  // PORT: Java did m_signEntityClass.newInstance() via reflection. No RTTI
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)
