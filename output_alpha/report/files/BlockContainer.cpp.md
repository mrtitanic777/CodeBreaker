# BlockContainer.cpp

Risk score: **12**  ·  3 findings

## GUESSED — converter made a heuristic call that may be wrong -- INSPECT

- **🔴 HIGH** `L19` (marker, `PORT`): static bool[] (was bare isBlockContainer)
  ```cpp
  Block::s_isBlockContainer[var1] = true;  // PORT: static bool[] (was bare isBlockContainer)
  ```

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L22` (rule, `ctor-stub`): constructor BlockContainer(int var1, int var2, Material* var3) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  BlockContainer(int var1, int var2, Material* var3) {...}
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)
