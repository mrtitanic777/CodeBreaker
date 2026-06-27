# BlockFire.cpp

Risk score: **4**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `—` (emit, `instance-init`): instance field initializers / init-blocks emitted into the constructor(s) (Java runs these per-ctor; dropping them caused null-deref/uninit fields)
- **🟡 MED** `L47` (rule, `virtual-call-in-ctor`): BlockFire's constructor calls virtual setBurnRate() -- C++ dispatches to BlockFire's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for BlockFire
