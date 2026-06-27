# Block.cpp

Risk score: **14**  ·  7 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L214` (rule, `virtual-call-in-ctor`): Block's constructor calls virtual unusedMethod() -- C++ dispatches to Block's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.
- **🟡 MED** `L214` (rule, `virtual-call-in-ctor`): Block's constructor calls virtual setBlockBounds() -- C++ dispatches to Block's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.
- **🟡 MED** `L214` (rule, `virtual-call-in-ctor`): Block's constructor calls virtual isOpaqueCube() -- C++ dispatches to Block's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.
- **🟡 MED** `L247` (rule, `virtual-call-in-ctor`): Block's constructor calls virtual unusedMethod() -- C++ dispatches to Block's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.
- **🟡 MED** `L247` (rule, `virtual-call-in-ctor`): Block's constructor calls virtual setBlockBounds() -- C++ dispatches to Block's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.
- **🟡 MED** `L247` (rule, `virtual-call-in-ctor`): Block's constructor calls virtual isOpaqueCube() -- C++ dispatches to Block's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (pipeline, `override`): handwritten source override used
