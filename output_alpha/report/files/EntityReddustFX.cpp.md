# EntityReddustFX.cpp

Risk score: **5**  ·  2 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L17` (rule, `ctor-stub`): constructor EntityReddustFX(World* var1, double var2, double var4, double var6) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  EntityReddustFX(World* var1, double var2, double var4, double var6) {...}
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L20` (marker, `PORT`): constructor delegation  // was: this(var1, var2, var4, var6, 1.0F);
  ```cpp
  // PORT: constructor delegation  // was: this(var1, var2, var4, var6, 1.0F);
  ```
