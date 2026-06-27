# ItemStack.cpp

Risk score: **8**  ·  10 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L104` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)
- **🟡 MED** `L108` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)
- **🟡 MED** `L115` (rule, `virtual-call-in-ctor`): ItemStack's constructor calls virtual readFromNBT() -- C++ dispatches to ItemStack's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.
- **🟡 MED** `L123` (emit, `null-deref-guard`): null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -> IP 0x0 crash)

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for ItemStack

## RULE — an automatic fix rule fired

- **⚪ LOW** `L7` (emit, `ctor-delegation-inlined`): ctor delegation this((Block*)var1, 1) inlined from the 2-arg constructor (registration preserved)
- **⚪ LOW** `L11` (emit, `ctor-delegation-inlined`): ctor delegation this(var1->blockID, var2) inlined from the 2-arg constructor (registration preserved)
- **⚪ LOW** `L15` (emit, `ctor-delegation-inlined`): ctor delegation this((Item*)var1, 1) inlined from the 2-arg constructor (registration preserved)
- **⚪ LOW** `L19` (emit, `ctor-delegation-inlined`): ctor delegation this(var1->shiftedIndex, var2) inlined from the 2-arg constructor (registration preserved)
- **⚪ LOW** `L23` (emit, `ctor-delegation-inlined`): ctor delegation this(var1, 1) inlined from the 2-arg constructor (registration preserved)
