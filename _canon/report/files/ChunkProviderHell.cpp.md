# ChunkProviderHell.cpp

Risk score: **4**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `—` (emit, `instance-init`): instance field initializers / init-blocks emitted into the constructor(s) (Java runs these per-ctor; dropping them caused null-deref/uninit fields)
- **🟡 MED** `—` (emit, `rule-of-three-dtor`): Rule-of-Three: synthesized destructor frees 8 heap-owning field(s): noiseGen1, noiseGen2, noiseGen3, noiseGen5, noiseGen6, noiseGenA, noiseGenB, rand

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for ChunkProviderHell
