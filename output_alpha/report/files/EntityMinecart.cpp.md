# EntityMinecart.cpp

Risk score: **12**  ·  3 findings

## RISK — matches a known console-crash / leak pattern

- **🔴 HIGH** `L74` (rule, `uninit-array`): new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.
  ```cpp
  self->m_cargoItems = new ItemStack*[36];
  ```
- **🔴 HIGH** `L667` (rule, `uninit-array`): new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.
  ```cpp
  m_cargoItems = new ItemStack*[36];
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)
