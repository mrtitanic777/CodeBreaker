# NoiseGeneratorOctaves.cpp

Risk score: **4**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L12` (rule, `rule-of-three`): NoiseGeneratorOctaves owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
- **🟡 MED** `L29` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for NoiseGeneratorOctaves
