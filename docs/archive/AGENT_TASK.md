# Override-generation task (read fully before starting)

You are turning **generated C++ files that fail to compile** into **working handwritten
overrides** for an unofficial Xbox 360 (RGH) port of Minecraft Alpha. The transpiler
(`j2x`) emits C++03; a handful of files don't compile and need hand-fixing. Your job: make
your assigned files compile cleanly with **g++ -std=c++03**, preserving the game logic.

## Paths (absolute)
- Baseline (READ-ONLY reference, all 443 generated `.h`/`.cpp` + `stdafx.h` + runtime headers
  `JavaRT.h`/`JavaString.h`/`JArray.h`/`JList.h`/`GL11.h`/… ): `/c/Users/mrtit/AppData/Local/Temp/j2xbase`
- Overrides output dir (write your fixed files HERE): `C:/Users/mrtit/Documents/Visual Studio 2010/Projects/MinecraftAlpha/ConversionTool/overrides`
- Compiler: `C:/msys64/mingw64/bin/g++.exe`

## Per-file procedure (do this for EACH class C you are assigned)
You work in your OWN private scratch dir (given in your prompt as SCRATCH) so parallel agents
never clobber each other; you publish a file to the shared `overrides/` dir only once it's clean.
1. Copy the generated starting point into SCRATCH (use the Bash tool):
   `cp /c/Users/mrtit/AppData/Local/Temp/j2xbase/C.h   "$SCRATCH/C.h"`
   `cp /c/Users/mrtit/AppData/Local/Temp/j2xbase/C.cpp "$SCRATCH/C.cpp"`
   (Some classes are header-only — no `.cpp`. Only copy what exists.)
2. Compile (SCRATCH first so your edits win, baseline supplies everything else):
   `"C:/msys64/mingw64/bin/g++.exe" -std=c++03 -fsyntax-only -I"$SCRATCH" -I/c/Users/mrtit/AppData/Local/Temp/j2xbase "$SCRATCH/C.cpp" 2>&1 | head -40`
   (Header-only class: compile a tiny TU `echo '#include "C.h"' > $SCRATCH/_t.cpp` then compile
    `_t.cpp` the same way. Most header-only classes already compile; focus on `.cpp`.)
3. Read the errors, edit `$SCRATCH/C.h` / `$SCRATCH/C.cpp` with the Edit tool to fix them.
4. Repeat 2–3 until the compile prints **zero** `error:` lines.
5. PUBLISH: copy the clean file(s) to the shared overrides dir:
   `cp "$SCRATCH/C.h" "<overrides>/C.h"` and `cp "$SCRATCH/C.cpp" "<overrides>/C.cpp"`
   (Only publish files that compile clean. If you can't get one clean, leave it UNpublished and
    report it.)
6. Move to the next file.

## Hard rules
- **Preserve game logic.** Make the *minimal* correctness-preserving change that compiles.
  Do NOT delete or stub out real behavior. Never `#if 0` a method body or replace logic with
  `return 0;` just to silence an error — fix the actual C++ issue. If a single statement is
  genuinely untranslatable, port it faithfully (see patterns below), don't gut it.
- **Match the conventions** (the generated code already follows them):
  - Every Java reference type is a POINTER (`Block*`, `Entity*`). No game class is a value type.
  - `String` → `JavaString` (a `std::string` subclass; has `length()/charAt/substring/split/...`).
  - Arrays → `JArray<T>` (`.length`, bounds-checked `[]`, converts to `T*`). `new T[n]` → `JArray<T>(n)`.
  - Lists → `JList<T>` (a `std::vector<T>` subclass; `add/get/remove/contains/indexOf/size/isEmpty`).
    Maps → `std::map`, Sets → `std::set` (by value). `map.put(k,v)` → `map[k]=v`, `map.get(k)` → `map[k]`.
  - Static fields are `s_name`; a method that collides with a same-named field is `name_`.
  - Every `.cpp` begins `#include "stdafx.h"` then `#include "JavaRT.h"` (keep these).
  - String concat `"a"+n+obj` → `(JCat() + "a" + n + obj)`. Array literal `new T[]{a,b}` →
    `(JArrayInit<T>(), a, b)`.
- You may add small **local** helpers at the top of your `.cpp` (a static helper function, a
  `using`, a small struct) when that's the cleanest fix.
- **Do NOT edit** files you weren't assigned, the baseline dir, `j2x/`, or `runtime.py`.
- If a fix would require a method on a RUNTIME STUB that doesn't exist (e.g. `JavaString`,
  `GL11`, `File`, a class defined in `j2x/runtime.py`), DON'T try to edit the stub. Instead
  add a minimal local shim in your `.cpp` if possible, and **list it in your final report**
  under "runtime-stub gaps" so the converter owner can add it centrally.

## Common fix patterns you will hit
- **`.` should be `->`** on a pointer receiver → change `.` to `->`. (Conversely a value
  like a `JList`/`std::map`/`JavaString` uses `.`.)
- **`X.class` / `getClass()`** already become `Class()`. If used as an arg where a `Class*` is
  wanted it converts automatically.
- **`instanceof` on `void*` (Object)** can't RTTI → already emitted as `(x != NULL)`; if you
  see a raw `instanceof` left over, rewrite `x instanceof T` → `(dynamic_cast<T*>(x) != NULL)`
  for real class pointers, or `(x != NULL)` when `x` is `void*`.
- **`Object` is `void*`.** A cast `(SomeClass)voidPtr` → `(SomeClass*)voidPtr`; member access
  through it → `((SomeClass*)voidPtr)->field`.
- **try/finally**: C++ has no `finally`. Convert `try { A } finally { B }` to run B on all
  paths — simplest faithful port: move B after the try and also before any early return/throw,
  or use a small RAII guard struct whose destructor runs B. Keep A's logic intact.
- **enum `Foo.values()` / `.ordinal()`**: enums are emitted as a struct of `static const int`.
  `.ordinal()` on an enum value is just the int itself (drop `.ordinal()`); `Foo.values().length`
  has no equivalent — replace with the known constant count of that enum (count its members in
  the generated `Foo.h`).
- **Iterator pattern** `set.iterator()` / `it.hasNext()` / `it.next()`: rewrite to a C++
  iterator loop, e.g. `for (std::set<void*>::iterator it = s.begin(); it != s.end(); ++it) { void* x = *it; ... }`.
- **recipe `Object[]{...}` autoboxing** (CraftingManager + Recipes*): the array mixes
  `JavaString`/`char`/`Block*`/`Item*`. To compile as `JArray<void*>`, box each element to
  `void*`: pointers cast directly `(void*)ptr`; a `char` key → `(void*)(long long)c`; a string
  literal → make it a `JavaString*` or store its `(void*)`. Then in the consumer
  (`addRecipe`) the dispatch that was `instanceof Character/String` must read the boxed value
  back — preserve the STRUCTURE (pattern rows, key chars, ingredients) as faithfully as you
  can; if exact semantics can't be preserved without a larger redesign, make it COMPILE with
  the data intact and note "recipe semantics need review" in your report. Prefer a small local
  tagged representation over losing the data.

## Final report (return this as your message)
- A table: each assigned file → CLEAN (0 errors) or STILL-FAILING (with the remaining error
  count + the single hardest error).
- A "runtime-stub gaps" list (symbols missing from `j2x/runtime.py` stubs) if any.
- A "semantics-need-review" list (files you made compile but where behavior may differ).
Be precise and honest; do not claim CLEAN unless the g++ command prints zero `error:` lines.
