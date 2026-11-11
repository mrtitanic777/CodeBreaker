# native/third_party

Everything here is somebody else's code, copied into the tree (nothing is downloaded by the build). The program
that needs each piece, its version and its licence:

| Piece | Where | Version | Licence | Used by |
|---|---|---|---|---|
| nlohmann/json | `nlohmann/json.hpp` | 3.10.5 | MIT | `cb/core/json` (the one JSON library; `cb::Json` = `ordered_json`). Moved here from `native/x360/system/live/third_party` on 2026-11-11; the x360 live client reads it from here. |
| Unicorn (prebuilt DLL) | `bin/unicorn.dll`, headers `include/unicorn/` | 2.1.4 (library reports 2.1) | LGPL-2.0 (headers say "released under LGPL2") | `cb/core/unicorn_api` -> `cb/emulate` |
| Capstone (prebuilt DLL) | `bin/capstone.dll`, headers `include/capstone/` | 5.0.7 | BSD-3-Clause (`bin/capstone-LICENSE.txt`) | `cb/core/capstone_api` -> `cb/decompile`, `cb/analyze` |
| Z3 (prebuilt DLL) | `bin/libz3.dll`, headers `include/z3/` (the C headers only, not `z3++.h`) | 5.3.2 | MIT | `cb/core/z3_api` -> `cb/verify` |

## The prebuilt DLLs

`unicorn.dll`, `capstone.dll` and `libz3.dll` were copied on 2026-11-11 from the folders of the Python packages
already installed on the build machine (`unicorn 2.1.4`, `capstone 5.0.7`, `z3-solver 5.3.2.0`; local copy, no
download). They are the official release builds the wheels carry. The decision (docs/CPP_DESIGN.md section 9,
items 1 and 2) is that the program ships them next to `codebreaker.exe` and loads them through `cb::DynLib`
(`cb/core/dynlib.hpp`); nothing is ever read from Python's `site-packages` again. The CMake build copies them
beside every executable that loads them (`cb_copy_prebuilt_dlls`).

| File | SHA-256 |
|---|---|
| `bin/capstone.dll` | `46b7c385efe50db4def8aa99aa351c44ea28b1f3c2ccbc4ca41dbb2a02340e39` |
| `bin/libz3.dll` | `52971ece5c1f5fb0ced6ff1d451f1f8e36320c29056d17dd76ef66cec5a8e8b2` |
| `bin/unicorn.dll` | `010d85dce84b8f6ffaec3d5420f1b5f7768f859eb831be574c7da23e656ee8f1` |

Notes:

* **Capstone's version changed.** The old native decompiler (WT_CAPSTONE) was written against Capstone 4.0.2; the
  DLL here is 5.0.7, and the headers in `include/capstone/` are the 5.0.7 ones, so `cs_insn` and the per-arch detail
  structs are the version-5 layout. Code ported from the 4.x era must be checked against these headers.
* **Unicorn is LGPL-2.** It is used only as a separate DLL that the user can replace (loaded with `LoadLibrary`),
  which is how the licence's relinking condition is met. Do not link it statically.
* **Z3 needs the Visual C++ runtime** (`msvcp140*.dll`, `vcomp140.dll`); the wheel carries its own copies beside
  `libz3.dll`, but any machine with the VC++ redistributable has them. If the shipped folder must stand alone, copy
  those DLLs beside the exe as well (not done: the build machine has them).
* To update a library: replace the DLL and the matching `include/<name>/` headers together, update the table and
  the hashes above, and run `cb_tests --filter core.unicorn` (and `core.capstone`, `core.z3`): each runs one real
  job through the API table, so a header/DLL mismatch fails there.

## How a library is loaded

`DynLib::load_beside_exe("unicorn.dll")` looks in, in order: the executable's directory, its `third_party/bin`
subfolder, the directory in the `CB_THIRD_PARTY_BIN` environment variable, and this folder's `bin/` (compiled in,
so a test run from the build tree works). A missing DLL is a `NotFoundError` that lists the places searched; a
DLL older than the API table in `cb/core/<name>_api.hpp` fails at load with the name of the missing function.
