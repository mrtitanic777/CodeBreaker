# ModelBiped.cpp

Risk score: **7**  ·  4 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L24` (rule, `ctor-stub`): constructor ModelBiped(float var1) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  ModelBiped(float var1) {...}
  ```

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L39` (rule, `rule-of-three`): ModelBiped owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L21` (marker, `PORT`): constructor delegation  // was: this(0.0F);
  ```cpp
  // PORT: constructor delegation  // was: this(0.0F);
  ```
- **⚪ LOW** `L36` (marker, `PORT`): constructor delegation  // was: this(var1, 0.0F);
  ```cpp
  // PORT: constructor delegation  // was: this(var1, 0.0F);
  ```
