# EntityFlying.cpp

Risk score: **11**  ·  4 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L15` (rule, `ctor-stub`): constructor EntityFlying(World* var1) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  EntityFlying(World* var1) {...}
  ```

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L47` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L58` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.

## WARNING — suspicious; verify by hand

- **🟡 MED** `L25` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
