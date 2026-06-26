# SpawnerAnimals.cpp

Risk score: **17**  ·  4 findings

## BLOCKED — explicitly could not be ported

- **🔴 HIGH** `L8` (marker, `PORT-BLOCKED`): the vanilla spawn loop builds an EntityLiving via Java reflection
  ```cpp
  //  - PORT-BLOCKED: the vanilla spawn loop builds an EntityLiving via Java reflection
  ```
- **🔴 HIGH** `L86` (marker, `PORT-BLOCKED`): no entity .class list / reflection on 360 -> cannot spawn here.
  ```cpp
  // PORT-BLOCKED: no entity .class list / reflection on 360 -> cannot spawn here.
  ```
- **🔴 HIGH** `L90` (marker, `PORT-BLOCKED`): var34 = (EntityLiving)var33[i].getConstructor(World.class)
  ```cpp
  // PORT-BLOCKED: var34 = (EntityLiving)var33[i].getConstructor(World.class)
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)
