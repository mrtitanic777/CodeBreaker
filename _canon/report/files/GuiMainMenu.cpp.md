# GuiMainMenu.cpp

Risk score: **6**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L141` (rule, `static-init-order`): eager static init `GuiMainMenu::s_rand = ...` runs in UNDEFINED cross-translation-unit order (it constructs a class or reads another class's statics) -- the static-init-order crash class. Defer it into initStatics() (run in dependency order by j2x_initAll).
  ```cpp
  Random* GuiMainMenu::s_rand = new Random();
  ```
- **🟡 MED** `L143` (rule, `rule-of-three`): GuiMainMenu owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (pipeline, `override`): handwritten source override used
