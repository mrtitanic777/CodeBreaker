# TileEntityMobSpawner.cpp

Risk score: **17**  ·  5 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L11` (marker, `PORT`): removed spurious "MobSpawner.h" (no such class)
  ```cpp
  // PORT: removed spurious "MobSpawner.h" (no such class)
  ```
- **🔴 HIGH** `L61` (marker, `PORT`): Java passed var9.getClass() to filter by type. No RTTI class
  ```cpp
  // PORT: Java passed var9.getClass() to filter by type. No RTTI class
  ```
- **🔴 HIGH** `L108` (marker, `PORT`): base TileEntity::writeToNBT now needs an id (no reflection on 360);
  ```cpp
  // PORT: base TileEntity::writeToNBT now needs an id (no reflection on 360);
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L14` (marker, `PORT`): worldObj-> calls (getClosestPlayer/rand/spawnParticle/...)
  ```cpp
  #include "World.h"  // PORT: worldObj-> calls (getClosestPlayer/rand/spawnParticle/...)
  ```
