# BlockMobSpawner.cpp

Risk score: **12**  ·  3 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L10` (marker, `PORT`): removed spurious "MobSpawner.h" (no such class; nothing here uses it)
  ```cpp
  // PORT: removed spurious "MobSpawner.h" (no such class; nothing here uses it)
  ```
- **🔴 HIGH** `L17` (rule, `ctor-stub`): constructor BlockMobSpawner(int var1, int var2) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  BlockMobSpawner(int var1, int var2) {...}
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)
