# GuiIngame.cpp

Risk score: **7**  ·  6 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L236` (marker, `PORT`): F3 debug overlay (XInput has no F3; Runtime memory APIs unportable) -> dropped.
  ```cpp
  // PORT: F3 debug overlay (XInput has no F3; Runtime memory APIs unportable) -> dropped.
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L2` (marker, `PORT`): override for GuiIngame.java (Minecraft Alpha v1.2.6)
  ```cpp
  // Hand-written PORT override for GuiIngame.java (Minecraft Alpha v1.2.6)
  ```
- **⚪ LOW** `L56` (marker, `PORT`): java.awt.Color.HSBtoRGB(h, s, b) -> packed 0x00RRGGBB. Used for the record-toast colour.
  ```cpp
  // PORT: java.awt.Color.HSBtoRGB(h, s, b) -> packed 0x00RRGGBB. Used for the record-toast colour.
  ```
- **⚪ LOW** `L219` (marker, `PORT`): GL_RESCALE_NORMAL -> normalize is shader-based on Xbox 360.
  ```cpp
  // PORT: GL_RESCALE_NORMAL -> normalize is shader-based on Xbox 360.
  ```
- **⚪ LOW** `L234` (marker, `PORT`): GL_RESCALE_NORMAL -> normalize is shader-based on Xbox 360.
  ```cpp
  // PORT: GL_RESCALE_NORMAL -> normalize is shader-based on Xbox 360.
  ```
