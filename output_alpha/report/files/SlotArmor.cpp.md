# SlotArmor.cpp

Risk score: **2**  ·  2 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L39` (marker, `PORT`): Java `var1.getItem() instanceof ItemArmor` -> dynamic_cast (Item is polymorphic)
  ```cpp
  // PORT: Java `var1.getItem() instanceof ItemArmor` -> dynamic_cast (Item is polymorphic)
  ```
