# SoundPool.cpp

Risk score: **2**  ·  3 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L2` (marker, `PORT`): override for SoundPool.java (Minecraft Alpha v1.2.6)
  ```cpp
  // Hand-written PORT override for SoundPool.java (Minecraft Alpha v1.2.6)
  ```
- **⚪ LOW** `L67` (marker, `PORT`): java.io.File / URL discovery is not used on Xbox 360.
  ```cpp
  (void)var2;  // PORT: java.io.File / URL discovery is not used on Xbox 360.
  ```
