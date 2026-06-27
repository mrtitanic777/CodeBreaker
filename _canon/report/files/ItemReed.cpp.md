# ItemReed.cpp

Risk score: **2**  ·  2 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L9` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for ItemReed
