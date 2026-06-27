# ModelMinecart.cpp

Risk score: **30**  ·  16 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `—` (emit, `instance-init`): instance field initializers / init-blocks emitted into the constructor(s) (Java runs these per-ctor; dropping them caused null-deref/uninit fields)
- **🟡 MED** `L4` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)
- **🟡 MED** `L4` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)
- **🟡 MED** `L4` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)
- **🟡 MED** `L4` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)
- **🟡 MED** `L4` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)
- **🟡 MED** `L4` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)
- **🟡 MED** `L4` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)
- **🟡 MED** `L4` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)
- **🟡 MED** `L4` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)
- **🟡 MED** `L4` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)
- **🟡 MED** `L4` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)
- **🟡 MED** `L4` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)
- **🟡 MED** `L14` (rule, `rule-of-three`): ModelMinecart owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
- **🟡 MED** `L34` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for ModelMinecart
