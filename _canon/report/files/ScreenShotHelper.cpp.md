# ScreenShotHelper.cpp

Risk score: **4**  ·  2 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L45` (rule, `static-init-order`): eager static init `ScreenShotHelper::s_dateFormat = ...` runs in UNDEFINED cross-translation-unit order (it constructs a class or reads another class's statics) -- the static-init-order crash class. Defer it into initStatics() (run in dependency order by j2x_initAll).
  ```cpp
  DateFormat* ScreenShotHelper::s_dateFormat = new SimpleDateFormat("yyyy-MM-dd_HH.mm.ss");
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (pipeline, `override`): handwritten source override used
