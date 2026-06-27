# ItemTool.cpp

Risk score: **2**  ·  2 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `—` (emit, `instance-init`): instance field initializers / init-blocks emitted into the constructor(s) (Java runs these per-ctor; dropping them caused null-deref/uninit fields)

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for ItemTool
