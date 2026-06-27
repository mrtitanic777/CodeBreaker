# EntityFish.cpp

Risk score: **2**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L119` (rule, `virtual-call-in-ctor`): EntityFish's constructor calls virtual setThrowableHeading() -- C++ dispatches to EntityFish's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for EntityFish

## RULE — an automatic fix rule fired

- **⚪ LOW** `L45` (emit, `ctor-delegation-inlined`): ctor delegation this(var1) inlined from the 1-arg constructor (registration preserved)
