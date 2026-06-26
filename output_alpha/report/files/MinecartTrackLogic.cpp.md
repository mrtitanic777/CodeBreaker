# MinecartTrackLogic.cpp

Risk score: **2**  ·  2 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L84` (marker, `PORT`): free replaced entry (was Java List.set, GC'd)
  ```cpp
  delete (ChunkPosition*)m_field_1161_g[var1];  // PORT: free replaced entry (was Java List.set, GC'd)
  ```
