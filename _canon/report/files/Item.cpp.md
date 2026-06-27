# Item.cpp

Risk score: **6**  ·  4 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `—` (emit, `instance-init`): instance field initializers / init-blocks emitted into the constructor(s) (Java runs these per-ctor; dropping them caused null-deref/uninit fields)
- **🟡 MED** `—` (emit, `static-init-ordered`): static field inits + static{} blocks deferred into initStatics(), run in dependency order by j2x_initAll() (kills the static-init-order crash class)
- **🟡 MED** `L6` (emit, `uninit-array-fixed`): uninit-array fixed: JArray<Item* >(32000) value-initialized + length-tracked (Java zero-fills)

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for Item
