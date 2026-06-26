# BlockPressurePlate.cpp

Risk score: **9**  ·  4 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L98` (marker, `PORT`): Java filtered by EntityLiving.class / EntityPlayer.class. No RTTI class
  ```cpp
  // PORT: Java filtered by EntityLiving.class / EntityPlayer.class. No RTTI class
  ```

## WARNING — suspicious; verify by hand

- **🟡 MED** `L56` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L92` (marker, `PORT`): was '= NULL' (illegal vector init); default-construct empty
  ```cpp
  std::vector<void*> var8;  // PORT: was '= NULL' (illegal vector init); default-construct empty
  ```
