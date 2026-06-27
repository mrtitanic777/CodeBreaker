# RenderPlayer.cpp

Risk score: **4**  ·  4 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `—` (emit, `instance-init`): instance field initializers / init-blocks emitted into the constructor(s) (Java runs these per-ctor; dropping them caused null-deref/uninit fields)
- **⚪ LOW** `L8` (emit, `static-aggregate-array`): static array new T[]{...} -> JArray over a file-static data array (C++03 has no braced new[]; length tracked)
- **🟡 MED** `L109` (rule, `rule-of-three`): RenderPlayer owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for RenderPlayer
