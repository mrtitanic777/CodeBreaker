# BlockGlass.cpp

Risk score: **5**  ·  1 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L14` (rule, `ctor-stub`): constructor BlockGlass(int var1, int var2, Material* var3, bool var4) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  BlockGlass(int var1, int var2, Material* var3, bool var4) {...}
  ```
