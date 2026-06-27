# MinecartTrackLogic.cpp

Risk score: **2**  ·  2 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L49` (rule, `virtual-call-in-ctor`): MinecartTrackLogic's constructor calls virtual setConnections() -- C++ dispatches to MinecartTrackLogic's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for MinecartTrackLogic
