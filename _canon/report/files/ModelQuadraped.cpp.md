# ModelQuadraped.cpp

Risk score: **4**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `—` (emit, `instance-init`): instance field initializers / init-blocks emitted into the constructor(s) (Java runs these per-ctor; dropping them caused null-deref/uninit fields)
- **🟡 MED** `—` (emit, `rule-of-three-dtor`): Rule-of-Three: synthesized destructor frees 5 heap-owning field(s): body, leg1, leg2, leg3, leg4

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for ModelQuadraped
