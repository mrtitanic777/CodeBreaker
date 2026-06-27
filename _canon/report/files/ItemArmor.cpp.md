# ItemArmor.cpp

Risk score: **0**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **⚪ LOW** `L3` (emit, `static-aggregate-array`): static array new T[]{...} -> JArray over a file-static data array (C++03 has no braced new[]; length tracked)
- **⚪ LOW** `L4` (emit, `static-aggregate-array`): static array new T[]{...} -> JArray over a file-static data array (C++03 has no braced new[]; length tracked)

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for ItemArmor
