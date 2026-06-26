# Entity.cpp

Risk score: **24**  ·  15 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L501` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L505` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L509` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L525` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L594` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.

## WARNING — suspicious; verify by hand

- **🟡 MED** `L581` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L698` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L753` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L992` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L1002` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L1006` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L177` (marker, `PORT`): was Object */ var1) {
  ```cpp
  bool Entity::equals(void* /* PORT: was Object */ var1) {
  ```
- **⚪ LOW** `L705` (marker, `PORT`): converter emitted abs_max(); Java is var2*var2 + var4*var4.
  ```cpp
  // PORT: converter emitted abs_max(); Java is var2*var2 + var4*var4.
  ```
- **⚪ LOW** `L771` (marker, `PORT`): header returns const std::string&; Java returned null. Use a static empty string.
  ```cpp
  // PORT: header returns const std::string&; Java returned null. Use a static empty string.
  ```
