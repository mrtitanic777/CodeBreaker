# EntityPainting.cpp

Risk score: **6**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L64` (rule, `virtual-call-in-ctor`): EntityPainting's constructor calls virtual setDirection() -- C++ dispatches to EntityPainting's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.
- **🟡 MED** `L64` (rule, `virtual-call-in-ctor`): EntityPainting's constructor calls virtual onValidSurface() -- C++ dispatches to EntityPainting's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (pipeline, `override`): handwritten source override used
