# RenderManager.cpp

Risk score: **2**  ·  3 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L2` (marker, `PORT`): override for RenderManager.java (Minecraft Alpha v1.2.6)
  ```cpp
  // Hand-written PORT override for RenderManager.java (Minecraft Alpha v1.2.6)
  ```
- **⚪ LOW** `L239` (marker, `PORT`): guard against unregistered / null entities
  ```cpp
  return;  // PORT: guard against unregistered / null entities
  ```
