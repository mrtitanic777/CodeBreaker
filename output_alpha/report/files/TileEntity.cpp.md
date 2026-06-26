# TileEntity.cpp

Risk score: **24**  ·  7 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L13` (marker, `PORT`): subclass headers for the createAndLoadEntity dispatch (replaces Java reflection)
  ```cpp
  // PORT: subclass headers for the createAndLoadEntity dispatch (replaces Java reflection)
  ```
- **🔴 HIGH** `L38` (marker, `PORT`): Java kept Class<->name maps for reflection. Construction is now an
  ```cpp
  // PORT: Java kept Class<->name maps for reflection. Construction is now an
  ```
- **🔴 HIGH** `L67` (marker, `PORT`): Java did nameToClassMap.get(id).newInstance() via reflection.
  ```cpp
  // PORT: Java did nameToClassMap.get(id).newInstance() via reflection.
  ```
- **🔴 HIGH** `L109` (marker, `PORT`): Java registered Class<->name mappings here for reflection.
  ```cpp
  // PORT: Java registered Class<->name mappings here for reflection.
  ```

## WARNING — suspicious; verify by hand

- **🟡 MED** `L61` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L32` (marker, `PORT`): default = not persisted. Subclasses override to return their saved id.
  ```cpp
  // PORT: default = not persisted. Subclasses override to return their saved id.
  ```
