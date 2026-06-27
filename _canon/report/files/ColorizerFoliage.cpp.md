# ColorizerFoliage.cpp

Risk score: **4**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `—` (emit, `static-init-ordered`): static field inits + static{} blocks deferred into initStatics(), run in dependency order by j2x_initAll() (kills the static-init-order crash class)
- **🟡 MED** `L6` (emit, `uninit-array-fixed`): uninit-array fixed: JArray<int >(65536) value-initialized + length-tracked (Java zero-fills)

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for ColorizerFoliage
