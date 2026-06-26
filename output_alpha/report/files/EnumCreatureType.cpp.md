# EnumCreatureType.cpp

Risk score: **10**  ·  2 findings

## BLOCKED — explicitly could not be ported

- **🔴 HIGH** `L19` (marker, `PORT-BLOCKED`): s_monster = new EnumCreatureType(NULL /* PORT: IMobs.class */, 100);
  ```cpp
  // PORT-BLOCKED: s_monster = new EnumCreatureType(NULL /* PORT: IMobs.class */, 100);
  ```
- **🔴 HIGH** `L21` (marker, `PORT-BLOCKED`): s_creature = new EnumCreatureType(NULL /* PORT: EntityAnimals.class */, 20);
  ```cpp
  // PORT-BLOCKED: s_creature = new EnumCreatureType(NULL /* PORT: EntityAnimals.class */, 20);
  ```
