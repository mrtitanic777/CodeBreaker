# MobSpawnerHell.cpp

Risk score: **9**  ·  3 findings

## BLOCKED — explicitly could not be ported

- **🔴 HIGH** `L13` (marker, `PORT-BLOCKED`): m_biomeMonsters was {EntityGhast.class, EntityPigZombie.class};
  ```cpp
  // PORT-BLOCKED: m_biomeMonsters was {EntityGhast.class, EntityPigZombie.class};
  ```

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L11` (rule, `rule-of-three`): MobSpawnerHell owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)
