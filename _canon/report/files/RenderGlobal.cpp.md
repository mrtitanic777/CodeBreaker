# RenderGlobal.cpp

Risk score: **4**  ·  2 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L151` (rule, `virtual-call-in-ctor`): RenderGlobal's constructor calls virtual renderStars() -- C++ dispatches to RenderGlobal's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (pipeline, `override`): handwritten source override used
