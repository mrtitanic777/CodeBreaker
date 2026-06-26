# GuiSmallButton.cpp

Risk score: **5**  ·  1 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L13` (rule, `ctor-stub`): constructor GuiSmallButton(int var1, int var2, int var3, const std::string& var4) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  GuiSmallButton(int var1, int var2, int var3, const std::string& var4) {...}
  ```
