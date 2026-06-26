# BlockFence.cpp

Risk score: **7**  ·  2 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L16` (rule, `ctor-stub`): constructor BlockFence(int var1, int var2) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  BlockFence(int var1, int var2) {...}
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)
