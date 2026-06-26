# Alpha→Xbox360 per-file port rules (READ FIRST)

You fix auto-transpiled Java→C++ files so they COMPILE for Xbox 360 (MSVC, C++03, D3D9).

PATHS: your cwd is `...\MinecraftAlpha\ConversionTool`. So use `output_alpha/<File>.cpp`,
`build_alpha28.log`, `handwritten_alpha/<File>.cpp` (all relative to cwd). The indev
porting reference (if present) is the ABSOLUTE path
`C:\Users\mrtit\Documents\Visual Studio 2010\Projects\MinecraftIndev\MinecraftIndev\<File>.cpp`.

## Per-file workflow (do this for EACH file you're assigned)
1. Read `output_alpha/<File>.cpp` (the converter output = your starting point) and `output_alpha/<File>.h` (the interface — match it EXACTLY).
2. Get THIS file's compiler errors: `grep -E "<File>\.cpp\(" build_alpha28.log` (line numbers + messages).
3. If the indev reference `...\MinecraftIndev\MinecraftIndev\<File>.cpp` exists, read it as a porting reference (how the same class was hand-ported for the sister project). Mine it for solutions; don't blindly copy (indev API differs from alpha).
4. Fix every error. Write the corrected file to `handwritten_alpha/<File>.cpp` (this is a durable override applied on every reconvert). Only write `handwritten_alpha/<File>.h` too if the HEADER itself must change (rare — prefer matching the existing .h). If a file you're assigned is ALREADY in `handwritten_alpha/`, edit THAT copy (it's the live one).
5. Keep going until the file would compile clean. Stub genuinely-unportable bits with a `// PORT:` comment rather than leaving a compile error.

## Hard rules (Xbox 360 / C++03 / MSVC / D3D9)
- C++03 ONLY. Use `NULL` not `nullptr`. No `auto`, no range-for, no `<cstdint>` fixed-width unless already used. No exceptions (`/EH` off) — `throw` is dead; guard with `if(false)` or remove.
- `Tesselator::getInstance()` returns a **REFERENCE**. Bind `Tesselator& t = Tesselator::getInstance();` then `t.method()`. NEVER copy it by value (the vertex buffer is ~MBs) and never `->` on the reference.
- **Value vs pointer is the #1 error class** (C2227 `left of ->X must point to class`, C2228 `left of .X must have class`). Java objects map to C++ pointers (`Type*`). Fix `.`↔`->` to match how the variable/field is declared in the .h. A `(Type)x` cast becomes `(Type*)x`. `new X()` returns `X*`.
- **Coords are BARE now**: `Vec3D` and `TileEntity` fields are `xCoord/yCoord/zCoord` (NOT `m_xCoord`). Use `vec->xCoord`.
- **Statics use `s_` prefix**: `Block::s_grass`, `Item::s_itemRand`, `EnumSkyBlock::s_Block/s_Sky`. An inherited static referenced bare in a subclass (e.g. `itemRand`) must become `s_itemRand`, and if it's a pointer (`Random*`) use `s_itemRand->nextFloat()`.
- Java collections → STL: `ArrayList<T>`/`List` → `std::vector<void*>` (elements are usually `(T*)` casts); `.get(i)`→`[i]`, `.size()`→`.size()`, `.add(x)`→`.push_back(x)`, `.isEmpty()`→`.empty()`. `HashMap`→`std::map<std::string,void*>*` (allocate with `new`, access `(*m_map)[key]`, use `.find`/`.count`). `java.util.Set` → if you hit it, use `std::set<void*>` or a `std::vector<void*>` with a contains-check.
- Undefined type (C2027) / missing header (C1083) → add the right `#include "X.h"` (a .cpp can include freely). C2065 undeclared → usually a missing `s_`/`m_` prefix or a missing include.
- NBT: `NBTTagCompound`/`NBTTagList` with `setTag/getTag/setInteger/...`. File I/O: `JavaIO`/`File` shims.
- Match obfuscated names from the .h EXACTLY: `func_XXX_a(...)`, `m_field_NNN_x`. Don't rename.
- D3D9 not GL. `MathHelper::sin/cos`, `sinf/cosf` for float.

## Output
- Write `ConversionTool/handwritten_alpha/<File>.cpp` for each assigned file.
- At the end, reply with one line per file: `<File>.cpp: <what you fixed>` — terse. Your reply is data for the orchestrator, not a human.
