# GuiScreen.cpp

Risk score: **2**  ·  7 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L7` (marker, `PORT`): notes. Real input is driven through
  ```cpp
  // 360; those paths are stubbed with // PORT: notes. Real input is driven through
  ```
- **⚪ LOW** `L55` (marker, `PORT`): Java AWT system clipboard (Toolkit / Transferable / DataFlavor) does not
  ```cpp
  // PORT: Java AWT system clipboard (Toolkit / Transferable / DataFlavor) does not
  ```
- **⚪ LOW** `L101` (marker, `PORT`): Java polled LWJGL Mouse.next()/Keyboard.next() event queues here and
  ```cpp
  // PORT: Java polled LWJGL Mouse.next()/Keyboard.next() event queues here and
  ```
- **⚪ LOW** `L109` (marker, `PORT`): was Mouse.getEventButtonState()/getEventX()/getEventY()/getEventButton()
  ```cpp
  // PORT: was Mouse.getEventButtonState()/getEventX()/getEventY()/getEventButton()
  ```
- **⚪ LOW** `L115` (marker, `PORT`): was Keyboard.getEventKeyState()/getEventKey()/getEventCharacter() (LWJGL),
  ```cpp
  // PORT: was Keyboard.getEventKeyState()/getEventKey()/getEventCharacter() (LWJGL),
  ```
- **⚪ LOW** `L164` (marker, `PORT`): not invoked by the base screen; subclasses (e.g. GuiYesNo flows) override
  ```cpp
  // PORT: not invoked by the base screen; subclasses (e.g. GuiYesNo flows) override
  ```
