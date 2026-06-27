# Task B: paste the ORIGINAL JAVA next to each stubbed section (read fully first)

The Minecraft Alpha → C++03 port compiles 100%. Some override `.cpp` files contain **stubs**:
spots where the C++ does NOT faithfully do what the original Java did (a behaviour was replaced
with a no-op / placeholder / local shim / approximation to make it compile or because an external
library isn't available). For every such stub, paste the **original alpha Java** for that spot as a
**comment** right above it, so a reader sees what the code is supposed to do. COMMENT-ONLY edits —
never change code. The file must still compile clean afterward.

## Paths
- Override files you edit (IN PLACE): `C:/Users/mrtit/Documents/Visual Studio 2010/Projects/MinecraftAlpha/ConversionTool/overrides/<Class>.cpp`
- Original alpha Java (your source to copy from): `C:/Users/mrtit/Desktop/MinecraftAlpha/minecraft/src/net/minecraft/src/<Class>.java`
- Reference headers for compiling (read-only): `/c/Users/mrtit/AppData/Local/Temp/j2xref`
- Compiler: `C:/msys64/mingw64/bin/g++.exe`

## What IS a stub (paste the original Java above it)
- A **local shim** block added near the top of the file faking an external API (e.g. a `namespace`
  / `struct` / function standing in for `HttpURLConnection`, `InetAddress`, `FileReader`,
  `Collections.sort`, XML/DOM, a `Frame`, etc.).
- A method body (or block) **replaced with a no-op / default return / placeholder** instead of the
  real logic (e.g. file IO that does nothing, network/XML parsing reduced to nothing, a
  class-dispatch that returns NULL, audio that no-ops).
- **Dropped behaviour**: an empty `catch{}`/`finally{}` that lost work, a loop body that was gutted,
  recipe `Object[]` data reduced to a tagged-box workaround, restored-but-uncertain tables.
- Any spot already marked with `// PORT:`, `inert`, `no real effect`, `needs review`, `stubbed`,
  `shim`, `no-op`, `not provided`, `tagged box`.

## What is NOT a stub (do NOT paste anything — the logic is faithful)
- Pure compile fixes: `.`→`->`, casts `(T*)x`, type spellings (`JavaString`/`JArray<T>`/`JList<T>`),
  added braces, `s_name` statics, `name_` collision renames, `JCat(...)` string concat.
- A `try/finally` rewritten so the finally body still runs on every path (behaviour preserved).
- `instanceof`→`dynamic_cast`, `Foo.values().length`→the constant count, `.ordinal()` dropped on an
  enum int — these are faithful equivalents.
If a file has NONE of the "IS a stub" cases, leave it untouched and report it as pure-compile-fix.

## How to do it, per assigned class C
1. Read `overrides/C.cpp`. Identify the stub spots (per the rules above).
2. Open `.../minecraft/src/net/minecraft/src/C.java`. Find the original Java for each stub spot
   (the method/block the stub stands in for).
3. Insert, IMMEDIATELY ABOVE the stubbed C++ line/block, a block comment with the verbatim Java:
   ```
   /* ---- ORIGINAL JAVA (stubbed below) ----
    *  <paste the relevant original Java lines here, verbatim>
    *  ---- end original Java ---- */
   ```
   - If the original Java itself contains `*/`, change that inner `*/` to `* /` in the COMMENT copy
     only (so the C++ comment doesn't terminate early), and note it.
   - Keep the snippet tight: the method/block the stub replaces, not the whole class.
4. After editing C.cpp, compile to confirm comments didn't break it (0 errors required):
   `"C:/msys64/mingw64/bin/g++.exe" -std=c++03 -fsyntax-only -I"<overrides>" -I/c/Users/mrtit/AppData/Local/Temp/j2xref "<overrides>/C.cpp"`
   If it errors (usually a stray `*/` in the pasted Java), fix the COMMENT (never the code) and recompile.
5. Next class.

## Report (return this)
A table: each assigned class → number of stub-sections annotated, or `pure compile-fix (none)`.
Note any file where you had to neutralize an inner `*/`. Confirm every edited file still compiles (0 errors).
