# WorldGenLiquids.cpp

Risk score: **2**  ·  1 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L71` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
