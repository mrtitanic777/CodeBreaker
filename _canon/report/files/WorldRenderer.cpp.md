# WorldRenderer.cpp

Risk score: **6**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L107` (rule, `static-init-order`): eager static init `WorldRenderer::s_tessellator = ...` runs in UNDEFINED cross-translation-unit order (it constructs a class or reads another class's statics) -- the static-init-order crash class. Defer it into initStatics() (run in dependency order by j2x_initAll).
  ```cpp
  Tessellator* WorldRenderer::s_tessellator = Tessellator::s_instance;
  ```
- **🟡 MED** `L142` (rule, `virtual-call-in-ctor`): WorldRenderer's constructor calls virtual setPosition() -- C++ dispatches to WorldRenderer's own version, NOT a subclass override (Java would call the override). Wrong data may be recorded (the Block light-opacity bug). Set the field directly or finish init post-ctor.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (pipeline, `override`): handwritten source override used
