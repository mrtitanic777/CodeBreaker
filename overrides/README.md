# Fresh handwritten overrides (alpha)

Drop `<Class>.h` and/or `<Class>.cpp` here, written in the **current** converter
convention, and they're used verbatim instead of generated output. The converter
applies overrides automatically — no flag needed:

    python -m j2x convert <alpha_src> <out_dir>

These are for the irreducible cases the converter cannot do by construction:
recipe `Object[]{...}` autoboxing, `Object`->`void*` dispatch, `try/finally`,
and anything else flagged in `WORKLIST.md`. The goal is 100% compile of the
game code (the external runtime stubs in `j2x/runtime.py` excepted).

**Convention (must match generated code, or the boundary breaks):**
- Java reference types are POINTERS (`Block*`, `Entity*`); no class is a value type.
- `String` -> `JavaString` (include `JavaString.h`); arrays -> `JArray<T>` (`JArray.h`).
- Containers are by-value `std::vector`/`std::map`/`std::set`.
- Static fields are `s_name`; a method colliding with a field is `name_`.
- Every `.cpp` starts with `#include "stdafx.h"` then `#include "JavaRT.h"`.

`WORKLIST.md` (regenerate with `python -m j2x worklist <out_dir>`) lists the failing
files sorted by error count — author overrides from the bottom (hardest) up, or fix
the near-clean ones in the converter where it's a general improvement.
