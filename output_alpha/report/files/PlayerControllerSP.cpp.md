# PlayerControllerSP.cpp

Risk score: **13**  ·  6 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L45` (rule, `ctor-stub`): constructor PlayerControllerSP(Minecraft* var1) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  PlayerControllerSP(Minecraft* var1) {...}
  ```

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L80` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L89` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L92` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L114` (marker, `PORT`): guard unregistered block id before deref
  ```cpp
  if(var6 == NULL) {   // PORT: guard unregistered block id before deref
  ```
