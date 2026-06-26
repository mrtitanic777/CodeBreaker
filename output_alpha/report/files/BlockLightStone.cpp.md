# BlockLightStone.cpp

Risk score: **5**  ·  1 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L16` (rule, `ctor-stub`): constructor BlockLightStone(int var1, int var2, Material* var3) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  BlockLightStone(int var1, int var2, Material* var3) {...}
  ```
