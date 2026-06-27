# TileEntityRenderer.cpp

Risk score: **6**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L107` (rule, `static-init-order`): eager static init `TileEntityRenderer::s_instance = ...` runs in UNDEFINED cross-translation-unit order (it constructs a class or reads another class's statics) -- the static-init-order crash class. Defer it into initStatics() (run in dependency order by j2x_initAll).
  ```cpp
  TileEntityRenderer* TileEntityRenderer::s_instance = new TileEntityRenderer();
  ```
- **🟡 MED** `L112` (rule, `rule-of-three`): TileEntityRenderer owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (pipeline, `override`): handwritten source override used
