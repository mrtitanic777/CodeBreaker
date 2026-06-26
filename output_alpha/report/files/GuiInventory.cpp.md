# GuiInventory.cpp

Risk score: **9**  ·  5 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L91` (marker, `PORT`): the inventory 3D player preview needs DEPTH so its parts self-occlude (GUI base is Z off).
  ```cpp
  // PORT: the inventory 3D player preview needs DEPTH so its parts self-occlude (GUI base is Z off).
  ```

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L34` (rule, `rule-of-three`): GuiInventory owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L108` (marker, `PORT`): compensating Ry(180) (paired with NEGATED yaw below) so Steve faces forward in the
  ```cpp
  // PORT: compensating Ry(180) (paired with NEGATED yaw below) so Steve faces forward in the
  ```
- **⚪ LOW** `L128` (marker, `PORT`): renderYawOffset(m_field_735_n)/yaw NEGATED to pair with the compensating Ry(180) above.
  ```cpp
  // PORT: renderYawOffset(m_field_735_n)/yaw NEGATED to pair with the compensating Ry(180) above.
  ```
