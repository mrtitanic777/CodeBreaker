# CraftingInventoryPlayerCB.cpp

Risk score: **6**  ·  4 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `—` (emit, `instance-init`): instance field initializers / init-blocks emitted into the constructor(s) (Java runs these per-ctor; dropping them caused null-deref/uninit fields)
- **🟡 MED** `—` (emit, `rule-of-three-dtor`): Rule-of-Three: synthesized destructor frees 1 heap-owning field(s): craftMatrix
- **🟡 MED** `L52` (rule, `virtual-call-in-ctor`): CraftingInventoryPlayerCB's constructor calls virtual onCraftMatrixChanged() -- C++ dispatches to CraftingInventoryPlayerCB's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for CraftingInventoryPlayerCB
