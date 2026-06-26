# BlockFlowing.cpp

Risk score: **23**  ·  7 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L22` (rule, `ctor-stub`): constructor BlockFlowing(int var1, Material* var2) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  BlockFlowing(int var1, Material* var2) {...}
  ```

## RISK — matches a known console-crash / leak pattern

- **🔴 HIGH** `L17` (rule, `uninit-array`): new bool[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new bool[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_459_b(new bool[4])
  ```
- **🟡 MED** `L18` (rule, `uninit-array`): new int[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new int[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_461_c(new int[4])
  ```
- **🔴 HIGH** `L25` (rule, `uninit-array`): new bool[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new bool[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_459_b(new bool[4])
  ```
- **🟡 MED** `L26` (rule, `uninit-array`): new int[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new int[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_461_c(new int[4])
  ```
- **🟡 MED** `L146` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L249` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
