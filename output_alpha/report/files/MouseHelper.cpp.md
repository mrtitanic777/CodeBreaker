# MouseHelper.cpp

Risk score: **7**  ·  3 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L27` (rule, `ctor-stub`): constructor MouseHelper(void* var1) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  MouseHelper(void* var1) {...}
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L2` (marker, `PORT`): override for MouseHelper.java (Minecraft Alpha v1.2.6)
  ```cpp
  // Hand-written PORT override for MouseHelper.java (Minecraft Alpha v1.2.6)
  ```
