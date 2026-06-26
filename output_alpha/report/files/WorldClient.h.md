# WorldClient.h

Risk score: **2**  ·  6 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L7` (marker, `PORT`): SINGLEPLAYER 360 port. WorldClient is the client-side world used only in
  ```cpp
  // PORT: SINGLEPLAYER 360 port. WorldClient is the client-side world used only in
  ```
- **⚪ LOW** `L48` (marker, `PORT`): was java.io.File */* var1);
  ```cpp
  virtual IChunkProvider* func_4081_a(std::string /* PORT: was java.io.File */* var1);
  ```
- **⚪ LOW** `L54` (marker, `PORT`): WorldBlockPositionType* */ m_field_1057_z;
  ```cpp
  std::vector<void*> /* PORT: WorldBlockPositionType* */ m_field_1057_z;
  ```
- **⚪ LOW** `L59` (marker, `PORT`): was java.util.Set<Entity*>
  ```cpp
  std::vector<void*>* m_F;            // PORT: was java.util.Set<Entity*>
  ```
- **⚪ LOW** `L60` (marker, `PORT`): was java.util.Set<Entity*>
  ```cpp
  std::vector<void*>* m_field_1053_F; // PORT: was java.util.Set<Entity*>
  ```
