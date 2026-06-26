# TextureFlamesFX.cpp

Risk score: **13**  ·  5 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L17` (rule, `ctor-stub`): constructor TextureFlamesFX(int var1) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  TextureFlamesFX(int var1) {...}
  ```

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L12` (rule, `uninit-array`): new float[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new float[N]() (the new bool[10] phantom-input bug).
  ```cpp
  : m_field_1133_g(new float[320])
  ```
- **🟡 MED** `L13` (rule, `uninit-array`): new float[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new float[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_1132_h(new float[320])
  ```
- **🟡 MED** `L19` (rule, `uninit-array`): new float[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new float[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_1133_g(new float[320])
  ```
- **🟡 MED** `L20` (rule, `uninit-array`): new float[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new float[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_1132_h(new float[320])
  ```
