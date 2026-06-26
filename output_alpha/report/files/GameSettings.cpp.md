# GameSettings.cpp

Risk score: **4**  ·  6 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L74` (rule, `rule-of-three`): GameSettings owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L54` (marker, `PORT`): rebuild the Java `keyBindings = new KeyBinding[]{ ... }` aggregate the converter
  ```cpp
  // PORT: rebuild the Java `keyBindings = new KeyBinding[]{ ... }` aggregate the converter
  ```
- **⚪ LOW** `L74` (marker, `PORT`): was java.io.File */* var2)
  ```cpp
  GameSettings::GameSettings(Minecraft* var1, std::string /* PORT: was java.io.File */* var2)
  ```
- **⚪ LOW** `L200` (marker, `PORT`): original reloads all textures here (anaglyph swaps the texture pack).
  ```cpp
  // PORT: original reloads all textures here (anaglyph swaps the texture pack).
  ```
- **⚪ LOW** `L312` (marker, `PORT`): Java did `Float.parseFloat`; "true"/"false" map to 1.0/0.0 (old options.txt
  ```cpp
  // PORT: Java did `Float.parseFloat`; "true"/"false" map to 1.0/0.0 (old options.txt
  ```
