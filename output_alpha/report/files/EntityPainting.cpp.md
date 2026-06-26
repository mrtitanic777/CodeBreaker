# EntityPainting.cpp

Risk score: **7**  ·  3 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L45` (rule, `ctor-stub`): constructor EntityPainting(World* var1, int var2, int var3, int var4, int var5) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  EntityPainting(World* var1, int var2, int var3, int var4, int var5) {...}
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L54` (marker, `PORT`): constructor delegation -- inlined body of EntityPainting(World*)
  ```cpp
  // PORT: constructor delegation -- inlined body of EntityPainting(World*)
  ```
