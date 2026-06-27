# World.cpp

Risk score: **22**  ·  11 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L178` (rule, `virtual-call-in-ctor`): World's constructor calls virtual createChunkProvider() -- C++ dispatches to World's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.
- **🟡 MED** `L178` (rule, `virtual-call-in-ctor`): World's constructor calls virtual calculateInitialSkylight() -- C++ dispatches to World's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.
- **🟡 MED** `L304` (rule, `virtual-call-in-ctor`): World's constructor calls virtual createChunkProvider() -- C++ dispatches to World's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.
- **🟡 MED** `L304` (rule, `virtual-call-in-ctor`): World's constructor calls virtual calculateInitialSkylight() -- C++ dispatches to World's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.
- **🟡 MED** `L368` (rule, `virtual-call-in-ctor`): World's constructor calls virtual createChunkProvider() -- C++ dispatches to World's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.
- **🟡 MED** `L368` (rule, `virtual-call-in-ctor`): World's constructor calls virtual calculateInitialSkylight() -- C++ dispatches to World's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.
- **🟡 MED** `L440` (rule, `virtual-call-in-ctor`): World's constructor calls virtual createChunkProvider() -- C++ dispatches to World's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.
- **🟡 MED** `L440` (rule, `virtual-call-in-ctor`): World's constructor calls virtual calculateInitialSkylight() -- C++ dispatches to World's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.
- **🟡 MED** `L564` (rule, `virtual-call-in-ctor`): World's constructor calls virtual createChunkProvider() -- C++ dispatches to World's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.
- **🟡 MED** `L564` (rule, `virtual-call-in-ctor`): World's constructor calls virtual calculateInitialSkylight() -- C++ dispatches to World's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (pipeline, `override`): handwritten source override used
