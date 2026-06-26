# TexturedQuad.cpp

Risk score: **2**  ·  2 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L44` (marker, `PORT`): was this(var1) — own a copy of the 4 verts, then set their UVs.
  ```cpp
  // PORT: was this(var1) — own a copy of the 4 verts, then set their UVs.
  ```
