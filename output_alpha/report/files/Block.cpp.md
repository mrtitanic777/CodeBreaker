# Block.cpp

Risk score: **73**  ·  27 findings

## GUESSED — converter made a heuristic call that may be wrong -- INSPECT

- **🔴 HIGH** `L344` (marker, `PORT`): value-vector, was Java List.add
  ```cpp
  var6.push_back(var7);  // PORT: value-vector, was Java List.add
  ```

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L238` (rule, `ctor-stub`): constructor Block(int var1, int var2, Material* var3) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  Block(int var1, int var2, Material* var3) {...}
  ```

## RISK — matches a known console-crash / leak pattern

- **🔴 HIGH** `L89` (rule, `uninit-array`): new bool[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new bool[N]() (the new bool[10] phantom-input bug).
  ```cpp
  bool* Block::s_tickOnLoad = new bool[256];
  ```
- **🔴 HIGH** `L90` (rule, `uninit-array`): new bool[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new bool[N]() (the new bool[10] phantom-input bug).
  ```cpp
  bool* Block::s_opaqueCubeLookup = new bool[256];
  ```
- **🔴 HIGH** `L91` (rule, `uninit-array`): new bool[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new bool[N]() (the new bool[10] phantom-input bug).
  ```cpp
  bool* Block::s_isBlockContainer = new bool[256];
  ```
- **🟡 MED** `L92` (rule, `uninit-array`): new int[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new int[N]() (the new bool[10] phantom-input bug).
  ```cpp
  int* Block::s_lightOpacity = new int[256];
  ```
- **🔴 HIGH** `L93` (rule, `uninit-array`): new bool[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new bool[N]() (the new bool[10] phantom-input bug).
  ```cpp
  bool* Block::s_field_340_s = new bool[256];
  ```
- **🟡 MED** `L94` (rule, `uninit-array`): new int[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new int[N]() (the new bool[10] phantom-input bug).
  ```cpp
  int* Block::s_lightValue = new int[256];
  ```
- **🔴 HIGH** `L522` (lint, `leak/new-leak`): `new` whose result is discarded/passed -> Java GC'd this; C++ leaks (TNT-stack OOM class). Use a stack value or delete it.
- **🟡 MED** `L548` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.

## WARNING — suspicious; verify by hand

- **🟡 MED** `L366` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L370` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L374` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L378` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L386` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L390` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L539` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L556` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L560` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L564` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L568` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L572` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L588` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L596` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L608` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L254` (marker, `PORT`): constructor delegation  // was: this(var1, var3);
  ```cpp
  // PORT: constructor delegation  // was: this(var1, var3);
  ```
