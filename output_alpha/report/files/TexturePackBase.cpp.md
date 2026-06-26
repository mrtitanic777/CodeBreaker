# TexturePackBase.cpp

Risk score: **17**  ·  7 findings

## BLOCKED — explicitly could not be ported

- **🔴 HIGH** `L44` (marker, `PORT-BLOCKED`): return TexturePackBase.class.getResourceAsStream(var1);
  ```cpp
  // PORT-BLOCKED: return TexturePackBase.class.getResourceAsStream(var1);
  ```

## WARNING — suspicious; verify by hand

- **🟡 MED** `L24` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L28` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L32` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L36` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L40` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)
