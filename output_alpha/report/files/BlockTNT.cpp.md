# BlockTNT.cpp

Risk score: **7**  ·  2 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L17` (rule, `ctor-stub`): constructor BlockTNT(int var1, int var2) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  BlockTNT(int var1, int var2) {...}
  ```

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L31` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
