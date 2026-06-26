# Teleporter.cpp

Risk score: **21**  ·  6 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L12` (marker, `PORT`): getBlockMaterial()->func_878_a() needs the full type
  ```cpp
  #include "Material.h"  // PORT: getBlockMaterial()->func_878_a() needs the full type
  ```

## RISK — matches a known console-crash / leak pattern

- **🔴 HIGH** `L149` (lint, `crash/label-goto`): goto label293 (Java labeled break/continue): verify it EXITs the loop / jumps to the do-while CONDITION -- else infinite loop (shore-water freeze).
- **🔴 HIGH** `L194` (lint, `crash/label-goto`): goto label231 (Java labeled break/continue): verify it EXITs the loop / jumps to the do-while CONDITION -- else infinite loop (shore-water freeze).

## WARNING — suspicious; verify by hand

- **🟡 MED** `L149` (marker, `PORT-WARNING`): (label goto): VERIFY semantics -- `break label` must EXIT (label AFTER the loop, else infinite loop); `continue label` on a do{}while must jump to the CONDITION.
  ```cpp
  goto label293;  // PORT-WARNING(label goto): VERIFY semantics -- `break label` must EXIT (label AFTER the loop, else infinite loop); `continue label` on a do{}w
  ```
- **🟡 MED** `L194` (marker, `PORT-WARNING`): (label goto): VERIFY semantics -- `break label` must EXIT (label AFTER the loop, else infinite loop); `continue label` on a do{}while must jump to the CONDITION.
  ```cpp
  goto label231;  // PORT-WARNING(label goto): VERIFY semantics -- `break label` must EXIT (label AFTER the loop, else infinite loop); `continue label` on a do{}w
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)
