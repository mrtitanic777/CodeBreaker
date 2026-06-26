# GuiButton.cpp

Risk score: **9**  ·  4 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L26` (rule, `ctor-stub`): constructor GuiButton(int var1, int var2, int var3, const std::string& var4) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  GuiButton(int var1, int var2, int var3, const std::string& var4) {...}
  ```

## WARNING — suspicious; verify by hand

- **🟡 MED** `L98` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L102` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L36` (marker, `PORT`): constructor delegation  // was: this(var1, var2, var3, 200, 20, var4);
  ```cpp
  // PORT: constructor delegation  // was: this(var1, var2, var3, 200, 20, var4);
  ```
