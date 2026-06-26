# GuiChat.cpp

Risk score: **12**  ·  4 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L39` (marker, `PORT`): Keyboard.enableRepeatEvents(true) -> XInput text entry replaces LWJGL Keyboard.
  ```cpp
  // PORT: Keyboard.enableRepeatEvents(true) -> XInput text entry replaces LWJGL Keyboard.
  ```
- **🔴 HIGH** `L43` (marker, `PORT`): Keyboard.enableRepeatEvents(false) -> XInput text entry replaces LWJGL Keyboard.
  ```cpp
  // PORT: Keyboard.enableRepeatEvents(false) -> XInput text entry replaces LWJGL Keyboard.
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L2` (marker, `PORT`): override for GuiChat.java (Minecraft Alpha v1.2.6)
  ```cpp
  // Hand-written PORT override for GuiChat.java (Minecraft Alpha v1.2.6)
  ```
