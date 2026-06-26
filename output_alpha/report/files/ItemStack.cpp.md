# ItemStack.cpp

Risk score: **39**  ·  17 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L23` (rule, `ctor-stub`): constructor ItemStack(Block* var1) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  ItemStack(Block* var1) {...}
  ```
- **🔴 HIGH** `L32` (rule, `ctor-stub`): constructor ItemStack(Block* var1, int var2) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  ItemStack(Block* var1, int var2) {...}
  ```
- **🔴 HIGH** `L41` (rule, `ctor-stub`): constructor ItemStack(Item* var1) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  ItemStack(Item* var1) {...}
  ```
- **🔴 HIGH** `L50` (rule, `ctor-stub`): constructor ItemStack(Item* var1, int var2) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  ItemStack(Item* var1, int var2) {...}
  ```
- **🔴 HIGH** `L59` (rule, `ctor-stub`): constructor ItemStack(int var1) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  ItemStack(int var1) {...}
  ```

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L148` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L165` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L169` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L173` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L177` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L185` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.

## WARNING — suspicious; verify by hand

- **🟡 MED** `L181` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L29` (marker, `PORT`): constructor delegation  // was: this((Block*)var1, 1);
  ```cpp
  // PORT: constructor delegation  // was: this((Block*)var1, 1);
  ```
- **⚪ LOW** `L38` (marker, `PORT`): constructor delegation  // was: this(var1->blockID, var2);
  ```cpp
  // PORT: constructor delegation  // was: this(var1->blockID, var2);
  ```
- **⚪ LOW** `L47` (marker, `PORT`): constructor delegation  // was: this((Item*)var1, 1);
  ```cpp
  // PORT: constructor delegation  // was: this((Item*)var1, 1);
  ```
- **⚪ LOW** `L56` (marker, `PORT`): constructor delegation  // was: this(var1->shiftedIndex, var2);
  ```cpp
  // PORT: constructor delegation  // was: this(var1->shiftedIndex, var2);
  ```
- **⚪ LOW** `L65` (marker, `PORT`): constructor delegation  // was: this(var1, 1);
  ```cpp
  // PORT: constructor delegation  // was: this(var1, 1);
  ```
