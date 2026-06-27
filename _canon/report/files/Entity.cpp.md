# Entity.cpp

Risk score: **10**  ·  6 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `—` (emit, `instance-init`): instance field initializers / init-blocks emitted into the constructor(s) (Java runs these per-ctor; dropping them caused null-deref/uninit fields)
- **🟡 MED** `L53` (rule, `rule-of-three`): Entity owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
- **🟡 MED** `L53` (rule, `virtual-call-in-ctor`): Entity's constructor calls virtual setPosition() -- C++ dispatches to Entity's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.
- **🟡 MED** `L237` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)
- **🟡 MED** `L237` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for Entity
