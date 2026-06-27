# CraftingManager.cpp

Risk score: **6**  ·  4 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `—` (emit, `instance-init`): instance field initializers / init-blocks emitted into the constructor(s) (Java runs these per-ctor; dropping them caused null-deref/uninit fields)
- **🟡 MED** `—` (emit, `static-init-ordered`): static field inits + static{} blocks deferred into initStatics(), run in dependency order by j2x_initAll() (kills the static-init-order crash class)
- **🟡 MED** `L58` (rule, `virtual-call-in-ctor`): CraftingManager's constructor calls virtual addRecipe() -- C++ dispatches to CraftingManager's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for CraftingManager
