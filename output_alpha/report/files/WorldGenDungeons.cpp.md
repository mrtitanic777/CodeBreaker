# WorldGenDungeons.cpp

Risk score: **7**  ·  2 findings

## RISK — matches a known console-crash / leak pattern

- **🔴 HIGH** `L102` (lint, `crash/label-goto`): goto label110 (Java labeled break/continue): verify it EXITs the loop / jumps to the do-while CONDITION -- else infinite loop (shore-water freeze).

## WARNING — suspicious; verify by hand

- **🟡 MED** `L102` (marker, `PORT-WARNING`): (label goto): VERIFY semantics -- `break label` must EXIT (label AFTER the loop, else infinite loop); `continue label` on a do{}while must jump to the CONDITION.
  ```cpp
  goto label110;  // PORT-WARNING(label goto): VERIFY semantics -- `break label` must EXIT (label AFTER the loop, else infinite loop); `continue label` on a do{}w
  ```
