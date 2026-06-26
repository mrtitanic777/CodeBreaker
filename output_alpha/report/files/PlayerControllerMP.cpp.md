# PlayerControllerMP.cpp

Risk score: **6**  ·  13 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L86` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L89` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L5` (marker, `PORT`): SINGLEPLAYER 360 port. Every addToSendQueue(new Packet...) call is
  ```cpp
  // PORT: SINGLEPLAYER 360 port. Every addToSendQueue(new Packet...) call is
  ```
- **⚪ LOW** `L65` (marker, `PORT`): 360 MP API TODO -- Packet14BlockDig(3,...) (no-op).
  ```cpp
  // PORT: 360 MP API TODO -- Packet14BlockDig(3,...) (no-op).
  ```
- **⚪ LOW** `L83` (marker, `PORT`): 360 MP API TODO -- Packet14BlockDig(0,...) (no-op).
  ```cpp
  // PORT: 360 MP API TODO -- Packet14BlockDig(0,...) (no-op).
  ```
- **⚪ LOW** `L98` (marker, `PORT`): 360 MP API TODO -- Packet14BlockDig(2,...) (no-op).
  ```cpp
  // PORT: 360 MP API TODO -- Packet14BlockDig(2,...) (no-op).
  ```
- **⚪ LOW** `L107` (marker, `PORT`): 360 MP API TODO -- Packet14BlockDig(1,...) (no-op).
  ```cpp
  // PORT: 360 MP API TODO -- Packet14BlockDig(1,...) (no-op).
  ```
- **⚪ LOW** `L178` (marker, `PORT`): 360 MP API TODO -- Packet16BlockItemSwitch (no-op).
  ```cpp
  // PORT: 360 MP API TODO -- Packet16BlockItemSwitch (no-op).
  ```
- **⚪ LOW** `L185` (marker, `PORT`): 360 MP API TODO -- Packet15Place (no-op).
  ```cpp
  // PORT: 360 MP API TODO -- Packet15Place (no-op).
  ```
- **⚪ LOW** `L191` (marker, `PORT`): 360 MP API TODO -- Packet15Place (no-op).
  ```cpp
  // PORT: 360 MP API TODO -- Packet15Place (no-op).
  ```
- **⚪ LOW** `L201` (marker, `PORT`): 360 MP API TODO -- Packet7 use-entity (no-op).
  ```cpp
  // PORT: 360 MP API TODO -- Packet7 use-entity (no-op).
  ```
- **⚪ LOW** `L207` (marker, `PORT`): 360 MP API TODO -- Packet7 use-entity (no-op).
  ```cpp
  // PORT: 360 MP API TODO -- Packet7 use-entity (no-op).
  ```
