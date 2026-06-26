# MobSpawnerBase.cpp

Risk score: **17**  ·  5 findings

## BLOCKED — explicitly could not be ported

- **🔴 HIGH** `L72` (marker, `PORT-BLOCKED`): biomeMonsters was {EntitySpider,EntityZombie,EntitySkeleton,EntityCreeper}.class
  ```cpp
  // PORT-BLOCKED: biomeMonsters was {EntitySpider,EntityZombie,EntitySkeleton,EntityCreeper}.class
  ```
- **🔴 HIGH** `L78` (marker, `PORT-BLOCKED`): > nothing to free.
  ```cpp
  // m_biomeMonsters / m_biomeCreatures are never allocated (PORT-BLOCKED) -> nothing to free.
  ```

## RISK — matches a known console-crash / leak pattern

- **🔴 HIGH** `L59` (rule, `uninit-array`): new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.
  ```cpp
  MobSpawnerBase** MobSpawnerBase::s_biomeLookupTable = new MobSpawnerBase*[4096];
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L22` (marker, `PORT`): java.awt.Color.getHSBColor(h,s,b).getRGB() & 0xFFFFFF -> packed 0x00RRGGBB.
  ```cpp
  // PORT: java.awt.Color.getHSBColor(h,s,b).getRGB() & 0xFFFFFF -> packed 0x00RRGGBB.
  ```
