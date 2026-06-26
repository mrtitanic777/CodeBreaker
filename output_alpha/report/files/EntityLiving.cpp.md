# EntityLiving.cpp

Risk score: **10**  ·  7 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L512` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L549` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L560` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.

## WARNING — suspicious; verify by hand

- **🟡 MED** `L780` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L450` (marker, `PORT`): header returns const std::string&; Java returned null here. Use a
  ```cpp
  // PORT: header returns const std::string&; Java returned null here. Use a
  ```
- **⚪ LOW** `L457` (marker, `PORT`): return a reference to a static so we don't dangle a temporary.
  ```cpp
  // PORT: return a reference to a static so we don't dangle a temporary.
  ```
