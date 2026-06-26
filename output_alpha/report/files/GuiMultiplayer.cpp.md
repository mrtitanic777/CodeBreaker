# GuiMultiplayer.cpp

Risk score: **2**  ·  6 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L2` (marker, `PORT`): override for GuiMultiplayer.java (Minecraft Alpha v1.2.6)
  ```cpp
  // Hand-written PORT override for GuiMultiplayer.java (Minecraft Alpha v1.2.6)
  ```
- **⚪ LOW** `L9` (marker, `PORT`): no-op.
  ```cpp
  // actual "Connect" action (which in Java spun up a GuiConnecting + socket) is a // PORT: no-op.
  ```
- **⚪ LOW** `L36` (marker, `PORT helpers`): small reimplementations of the Java String methods used here.
  ```cpp
  // PORT helpers: small reimplementations of the Java String methods used here.
  ```
- **⚪ LOW** `L114` (marker, `PORT`): no-op. Java built host[:port] and opened a GuiConnecting socket via
  ```cpp
  // PORT: no-op. Java built host[:port] and opened a GuiConnecting socket via
  ```
- **⚪ LOW** `L118` (marker, `PORT`): NetClientHandler / GuiConnecting networking dropped (single-player port).
  ```cpp
  // PORT: NetClientHandler / GuiConnecting networking dropped (single-player port).
  ```
