# Tessellator.cpp

Risk score: **4**  ·  2 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L38` (rule, `static-init-order`): eager static init `Tessellator::s_instance = ...` runs in UNDEFINED cross-translation-unit order (it constructs a class or reads another class's statics) -- the static-init-order crash class. Defer it into initStatics() (run in dependency order by j2x_initAll).
  ```cpp
  Tessellator* Tessellator::s_instance = new Tessellator(2097152);
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (pipeline, `override`): handwritten source override used
