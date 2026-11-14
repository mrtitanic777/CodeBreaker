# Batch 9: the last Python batch -- codebreaker/ is the terminal interface and nothing else

Roots, scripts and conditions as in batches 1-8 (`python_reach/g7.py`; this batch's `g9.py` lists every module's holders: `g9.out` is its output on the tree before any deletion; `b9_rows.json`, `b9_modules.md`, `b9_map_appendix.md` are
the inputs of this note). Branch `batch9` on master `a0aae6bec` (batches 1-8).

## Result

- `codebreaker/` Python: 149 modules / 41,585 lines -> **5 files / 1,433 lines**: `__init__.py` (a 9-line docstring), `__main__.py` (146; the `NATIVE` dispatch + `ui`), `ui/{__init__,client,terminal}.py` (0 + 294 + 984).
  Deleted: **144 modules, 40,115 lines** (table below), in one commit per area (verify, source, build, decompile, analyze, emulate, exact, translate, symbols, formats, engine+store+knowledge+jobs+compare+platforms, base).
- `tests/` Python: 47 files / 8,980 lines deleted (the 33 suite files, `frozen.py`, `frozen_subjects.py`, `msc8/suite.py`, the ten `tests/translate/test_*.py`); kept: `run_all.py` (245, was 418), `test_ui.py` (148), `test_ui_thin.py` (170), new `ui_seed.py` (137), `msc8/cases.py` (data, no imports).
  All non-Python test data stays (`tests/data`, `tests/fixtures`, `tests/translate/corpus` and `fixtures`, `tests/msc8/expected`, `tests/regress`): native tests read it.
- `tools/` Python: carve, unreachable, freeze_tests, golden, l2c, merge, move deleted (they read the deleted package or its command table); `tidy.py` loses its `unreachable` step; `make_api_table.py` reads the `openwatcom` setting itself instead of `build.watcom`.
- `requirements.txt` / `pyproject.toml`: the Python dependency is Textual (+ pyflakes for tidy). capstone, unicorn, z3, mcp, pytest, Pillow are gone from the list (they served the deleted modules).
- Whole branch against master: 208 files changed, 213 insertions, 50,319 deletions.

## What `codebreaker/` still is, and why

| file | why it stays |
|---|---|
| `ui/terminal.py`, `ui/client.py`, `ui/terminal.tcss`, `ui/__init__.py` | the terminal interface (Textual); the native exe launches it as `python -m codebreaker.ui.terminal` (`cb/app/codebreaker_main.cpp`). It imports nothing else of the package (`test_ui_thin` proves it with only these files present). |
| `__init__.py` | the package marker `python -m codebreaker.ui.terminal` needs (a docstring now). |
| `__main__.py` | `base/config.cpp` finds the checkout by `codebreaker/__main__.py` (C++ not changed in this batch; a root marker is cheap); `python -m codebreaker` and `pyproject`'s `codebreaker` entry point start `ui`; the translated exe's `__main__` hands a native command to `codebreaker_native` -- **goes with retire-pyengine** (when the exe is gone the dispatch part goes, `ui` stays). |
| non-Python files under `codebreaker/` | data the native program reads (tables, runtimes, toolchain profiles, `translate/frontends/clang/corpus`, the vendored crypto LICENSE): untouched. |

## Found and fixed on the way

- `codebreaker/ui/terminal.py` `Home.load`: the error callback `lambda: ... str(e)` ran in another thread after the `except` block had deleted `e` (a NameError whenever the store could not be read; pyflakes 3.4 reports it as an undefined name, so `tidy` was failing on it). It keeps the text first now.
- `test_ui.py::test_terminal_interface` and `test_ui_thin.py`'s tour **skipped** in every environment without the retail corpus (`Windows95/design/corpus/files`, absent here), so the terminal interface had no running test. They now seed a function store of their own (`tests/ui_seed.py`: sqlite3 with the schema of `cb/engine/function_store.cpp`, the PE `native/cb/formats/testdata/lz32.dll` as the module whose first six exports are the functions, the NE `lzexpand.dll` as the module the interface opens) and **run and pass** against the native program (`CB_EXE`), no corpus, no Python store modules.
  The old tour called `name_function` on the first export of version.dll and expected "no body recorded"; on lz32.dll the call succeeds, so the thin test now asserts the success (`named`, `body`).

## The five conditions, for the whole of the Python

1. **Their C++ is committed as source**: every deleted module has its native/cb module (table below, by name; the exceptions are noted in the row: `emulate/fast_uc` is a Python-binding speed-up with no C++ counterpart, `engine/native_engine` was the ctypes shell over `cbengine.dll` and `native/cb/engine` is the engine itself).
2. **Their tests pass as C++**: all 33 suite files (329 mapped cases in the appendix) were mapped case by case in `test_port_map.md` / `test_port_map_2.md` (batches 1-8's maps; copied beside this note) to native tests (COVERED / PORTED) or marked NOT NEEDED with the reason; no BLOCKED or unmapped case is left for these files. Native tests run: `area base` (122 passed, 0 failed, 888 checks) on this tree -- that area reads the checkout through `codebreaker/__main__.py`. No C++ changed in this batch, so the other areas' results are those of master (batch 8's merge); the integrator's full run is the check for them.
   Cases not ported, and why: `test_plumbing` (decompiler-is-built, openwatcom presence, throttle, `__main__` guard, fast-unicorn: environment checks and Python plumbing), `test_port_fixes::pe_writer_nosplit` (the Python's control-character regex bug cannot recur in the C++; no native test reaches the line), `test_xex` (usage text of the separate engine program), `test_native_formats` / `test_native_trace` (Python-attribute digest and Python-hook plumbing), `test_nes::sleigh_agrees` (Ghidra is gone; deleted earlier), `test_platforms` (compared Python module objects; deleted earlier), `test_java_frontend::the_dumper_is_cached` (cache of the JVM tree dumper in Python), `test_translate` (a pytest runner), `test_ui::test_terminal_interface` (stays: it is kept and now runs).
   `tests/msc8/suite.py` (152 lines; not a gate test: it measured how far OpenWatcom is from MS C 8's code, case by case, through `build.msc6` / `build.watcom`) is deleted with them; the same measurement is `score16` (`cb/verify/gate16/scoreboard16`, whose header says it keeps the same switch mapping as tests/msc8). `tests/msc8/cases.py` and `expected/` stay as the recorded cases.
3. **The judge traces agree**: the Python-made goldens are checked in and read by the native tests (batches 3-8); nothing in this batch changes a golden. The Python generators of those goldens (`native/cb/**/testdata/{gen_*,make_*}.py`, 21 files) import deleted modules and cannot be re-run any more -- the state batches 5-8 left their generators in; the goldens stand as the record. (`vmx128.cpp` still writes "GENERATED by `python -m codebreaker.decompile.vmx128 --sleigh`" into the SLEIGH text it emits: the generator is C++ now, the sentence is part of a golden, left as is.)
4. **Every caller runs through the native program**: `__main__.NATIVE` is every command but `ui` (batch 8's state); `python -m codebreaker <command>` now says so and exits 2 instead of importing a module. Searched `native tools tests data docs .mcp.json pyproject.toml` and the sibling repos Dashboard (`Recompiled/tools/scripts`: `codebreaker.exe xenon-recompile`; one comment there still says `python -m codebreaker`), Win95VM, XboxMCP, xdk-knowledge-mcp, Nexia360, Windows95 (`tools`, `design`): nothing imports `codebreaker.*` or runs `-m codebreaker <command>`. `check_imports`: 5 modules, 0 dangling.
5. **All-15 boot**: not run (the integrator's); the boot path reached none of this Python after batch_boot (`verify/gate16`, `source/module_writer` went in batches 5-7).

## What cannot go yet (exactly why)

No Python code under `codebreaker/` other than the terminal interface and the `__main__` marker is left. What is left around it, by owner:

**Goes with retire-pyengine** (the translated exe is built from `codebreaker/`, `tests/`, `tools/`; with the dynamic translation gone these have no reason to stay):
- `codebreaker/__main__.py`'s `NATIVE` tuple / `_native_command` (the hand-off to `codebreaker_native`); `tests/run_all.py` and its `ensure_exe` (it builds and runs the translated `codebreaker.exe`; `tools/gate.cmd` is its native successor and already runs the UI tests); `tools/build_codebreaker_exe.cmd`, `tools/build_rt_exe.cmd`.
- `native/cbexe/commands.cc` and `settings.cc`: still `py::import("codebreaker.formats.xex" / ".formats.detect" / ".base.corpus" / ".recompile.config")` and register a native `codebreaker.base.settings`. The five commands they serve (`xbox-disc`, `xex-basic`, `xenon-config`, `switch-tables`, `corpus-executables`) are in `NATIVE`, so the exe never reaches them (it hands the whole line to `cb`); the modules they import are gone (the `recompile` ones since batch 5, the rest now). Dead code, not a live path.
- `native/typed/**`, `native/ported/**`, `native/pyengine`, `native/pyrt`, `native/cb/compat/**` (shims that name `codebreaker.*` Python modules as strings, e.g. `compat/shims/codebreaker/platforms/*.hpp` `ref("codebreaker.build.driver", ...)`): C++ about the deleted Python; only the shim tests use them.
- `native/analyze|dosexec|engine|formats|invert|l2|symbols/CMakeLists.txt`: DLLs "loaded by codebreaker/.../native.py through ctypes" -- their only caller was that Python, now gone; the integrator removes the old `native/<lib>` copies (not done here: the prompt's LIVE-PATH rule keeps `native/<lib>/**` to the integrator).

**Not run here** (no pyengine/pyrt/exe is built in this worktree; the translated exe is about an hour cold): `codebreaker.exe tests/run_all.py --quick --jobs 2`, the full `cb_tests`, the all-15 boot, `regress`, ASan, `build_cb.cmd shims` (no C++ changed). What ran: `python tools/check_imports.py` (5 modules, 0 dangling), `python tools/tidy.py` (0 to remove, 0 undefined), `python tests/test_ui.py --only test_terminal_interface` (pass), `python tests/test_ui_thin.py` (2 pass), `python tests/run_all.py --plan`, `area base` (122 pass).
The suites left in `run_all.py`: `check_imports`, `tidy`, `test_ui`, `test_ui_thin` (the last two report SKIP under the translated exe: they need real python with Textual). Risk for the exe build, to look at first: `native/cbexe/settings.cc` registers `codebreaker.base.settings` as a native module with no `.py` behind it any more (it shadowed a `.py` before), and the translator now sees a 5-module package.

**Text that still names the deleted Python** (comments, goldens, docs; not changed here): `native/cb/**` headers ("codebreaker/x/y.py ..."), `docs/*.md` (ARCHITECTURE, NATIVE, PYTHON_RETIREMENT ...), `native/cb/base/config.cpp:29` (defaults "kept in step" with `settings.py`), `native/cb/app/native_commands.hpp:12`.

## The modules deleted (144)

"held by" is from `g9.out` on the tree before any deletion: the test files and tools that imported the module, and how many other modules did. The native replacement is the file of the same name in native/cb (by name, same area first); rows with a note were read by hand.

| module | lines | held by (tests/tools before this batch; modules importing it) | native replacement |
|---|---:|---|---|
| `analyze/__init__.py` | 23 | test_analysis, test_dos, test_port_fixes, translate; 10 modules | native/cb/java/analyze.hpp (by name) |
| `analyze/call_graph.py` | 110 | test_analysis; 3 modules | native/cb/analyze/call_graph.hpp (by name) |
| `analyze/classes.py` | 533 | test_analysis | native/cb/analyze/classes.hpp (by name) |
| `analyze/conventions.py` | 136 | test_analysis; 4 modules | native/cb/analyze/conventions.hpp (by name) |
| `analyze/dispatch.py` | 404 | test_analysis; 1 modules | native/cb/analyze/dispatch.hpp (by name) |
| `analyze/dos_functions.py` | 516 | test_dos; 2 modules | native/cb/analyze/dos_functions.hpp (by name) |
| `analyze/globals.py` | 105 | test_analysis; 1 modules | native/cb/analyze/globals.hpp (by name) |
| `analyze/image.py` | 524 | test_analysis, translate; 6 modules | native/cb/analyze/image.hpp (by name) |
| `analyze/native.py` | 157 | 2 modules | native/cb/analyze/native.hpp (by name) |
| `analyze/origin.py` | 489 | test_port_fixes; 1 modules | native/cb/analyze/origin.hpp (by name) |
| `analyze/strings.py` | 191 | test_analysis; 3 modules | native/cb/analyze/strings.hpp (by name) |
| `analyze/structures.py` | 421 | test_analysis, translate; 2 modules | native/cb/analyze/structures.hpp (by name) |
| `base/__init__.py` | 0 | frozen_subjects, run_all, test_analysis, test_assembler, test_concolic, test_converted, test_dos, test_drivers, test_media, test_native_formats, test_native_trace, test_pklite, test_plumbing, test_run_record, test_symbols, test_ui, test_ui_thin, test_verification, test_versions, test_xex, tools.freeze_tests, tools.golden, tools.l2c; 57 modules | (empty package marker) |
| `base/cache.py` | 591 | test_plumbing, test_verification; 10 modules | native/cb/base/cache.hpp (by name) |
| `base/converted.py` | 188 | test_converted; 2 modules | native/cb/base/converted.hpp (by name) |
| `base/corpus.py` | 292 | frozen_subjects, test_analysis, test_concolic, test_dos, test_drivers, test_media, test_native_formats, test_native_trace, test_pklite, test_plumbing, test_run_record, test_symbols, test_ui, test_verification, test_versions, test_xex, tools.freeze_tests, tools.l2c; 24 modules | native/cb/base/corpus.hpp (by name) |
| `base/parallel.py` | 268 | run_all, test_plumbing, test_verification; 4 modules | native/cb/base/parallel.hpp (by name) |
| `base/settings.py` | 286 | test_assembler, test_dos, test_drivers, test_verification, test_versions, tools.golden, tools.l2c; 41 modules | native/cb/base/settings.hpp (by name) |
| `build/__init__.py` | 0 | msc8, test_dos, test_drivers, test_exepack, test_flat_thunk, test_pklite, test_plumbing, test_port_fixes, test_powerpc, test_readable, test_symbols, test_verification, tools.make_api_table; 25 modules | (empty package marker) |
| `build/asm16.py` | 674 | test_verification | native/cb/build/asm16.hpp (by name) |
| `build/data.py` | 530 | 3 modules | native/cb/build/data.hpp (by name) |
| `build/definitions.py` | 208 | 2 modules | native/cb/build/definitions.hpp (by name) |
| `build/dos_sessions.py` | 185 | 1 modules | native/cb/build/dos_sessions.hpp (by name) |
| `build/dos_tools.py` | 77 | test_dos; 2 modules | native/cb/build/dos_tools.hpp (by name) |
| `build/driver.py` | 332 | test_drivers; 4 modules | native/cb/build/driver.hpp (by name) |
| `build/exepack.py` | 107 | test_exepack | native/cb/build/exepack.hpp (by name) |
| `build/flat_thunk.py` | 1085 | test_flat_thunk; 2 modules | native/cb/build/flat_thunk.hpp (by name) |
| `build/functions.py` | 678 | test_powerpc, test_readable, test_verification; 3 modules | native/cb/build/functions.hpp (by name) |
| `build/masm.py` | 442 | test_dos; 2 modules | native/cb/build/masm.hpp (by name) |
| `build/msc6.py` | 924 | msc8, test_dos, test_verification; 1 modules | native/cb/build/msc6.hpp (by name) |
| `build/msvc152.py` | 79 | test_port_fixes; 1 modules | native/cb/build/msvc152.hpp (by name) |
| `build/native_dos.py` | 207 | test_dos; 1 modules | native/cb/build/native_dos.hpp (by name) |
| `build/pklite.py` | 380 | test_pklite | native/cb/build/pklite.hpp (by name) |
| `build/resources.py` | 658 | 1 modules | native/cb/build/resources.hpp (by name) |
| `build/watcom.py` | 160 | msc8, test_plumbing, test_readable, test_symbols, test_verification, tools.make_api_table; 11 modules | native/cb/build/watcom.hpp (by name) |
| `compare/__init__.py` | 0 | test_symbols; 4 modules | native/cb/exact/compare.hpp (by name) |
| `compare/mask.py` | 90 | test_symbols; 4 modules | native/cb/compare/mask.hpp (by name) |
| `decompile/__init__.py` | 0 | frozen_subjects, test_dos, test_frozen, test_plumbing, test_port_fixes, test_verification, test_vmx128, translate, tools.freeze_tests, tools.golden, tools.l2c; 21 modules | (empty package marker) |
| `decompile/asm_blocks.py` | 267 | translate; 2 modules | native/cb/decompile/asm_blocks.hpp (by name) |
| `decompile/c_text.py` | 1428 | frozen_subjects, test_frozen, translate, tools.freeze_tests; 5 modules | native/cb/decompile/c_text.hpp (by name) |
| `decompile/decompiler.py` | 893 | test_dos, test_frozen, test_plumbing, test_verification, translate, tools.freeze_tests, tools.golden, tools.l2c; 5 modules | native/cb/decompile/decompiler.hpp (by name) |
| `decompile/describe.py` | 2104 | frozen_subjects, test_dos, test_port_fixes, test_verification, tools.golden; 14 modules | native/cb/decompile/describe.hpp (by name) |
| `decompile/frame16.py` | 236 | translate; 2 modules | native/cb/decompile/frame16.hpp (by name) |
| `decompile/inline_asm.py` | 191 | 2 modules | native/cb/decompile/inline_asm.hpp (by name) |
| `decompile/jar.py` | 188 | 2 modules | native/cb/decompile/jar.hpp (by name) |
| `decompile/shape16.py` | 111 | translate; 2 modules | native/cb/decompile/shape16.hpp (by name) |
| `decompile/struct_text.py` | 114 | translate; 1 modules | native/cb/decompile/struct_text.hpp (by name) |
| `decompile/vmx128.py` | 374 | test_vmx128 | native/cb/decompile/vmx128.hpp (by name) |
| `emulate/__init__.py` | 45 | test_concolic, test_knowledge, test_native_trace, test_plumbing, test_powerpc, test_run_record, test_verification; 15 modules | native/cb/emulate/ (the facade: its names are the native emulate headers) |
| `emulate/driver.py` | 217 | test_knowledge, test_native_trace; 1 modules | native/cb/emulate/driver.hpp (by name) |
| `emulate/emulator.py` | 616 | test_concolic, test_native_trace, test_run_record; 5 modules | native/cb/emulate/emulator.hpp (by name) |
| `emulate/fast_uc.py` | 69 | test_plumbing; 1 modules | none needed: a Python-binding speed-up; the C++ calls Unicorn directly (emulate/unicorn_uc.hpp) |
| `emulate/native_driver.py` | 204 | 1 modules | native/cb/emulate/native_driver.hpp (by name) |
| `emulate/native_shadow.py` | 239 | 4 modules | native/cb/emulate/native_shadow.hpp (by name) |
| `emulate/native_trace.py` | 390 | test_native_trace; 4 modules | native/cb/emulate/native_trace.hpp (by name) |
| `emulate/native_win16.py` | 209 | 1 modules | native/cb/emulate/native_win16.hpp (by name) |
| `emulate/pcode.py` | 147 | test_concolic, test_native_trace; 2 modules | native/cb/emulate/pcode.hpp (by name) |
| `emulate/ports16.py` | 195 | test_verification; 1 modules | native/cb/emulate/ports16.hpp (by name) |
| `emulate/powerpc.py` | 916 | test_concolic, test_powerpc; 2 modules | native/cb/emulate/powerpc.hpp (by name) |
| `emulate/run.py` | 76 | test_run_record; 4 modules | native/cb/emulate/run.hpp (by name) |
| `emulate/shadow.py` | 681 | test_concolic, test_native_trace; 2 modules | native/cb/emulate/shadow.hpp (by name) |
| `emulate/symbolic.py` | 193 | test_concolic; 2 modules | native/cb/emulate/symbolic.hpp (by name) |
| `emulate/windows16.py` | 1138 | test_concolic, test_knowledge, test_native_trace, test_run_record, test_verification; 4 modules | native/cb/emulate/windows16.hpp (by name) |
| `engine/__init__.py` | 17 | test_engine, test_knowledge, test_knowledge_reads, test_native_engine, test_store, test_ui_thin; 6 modules | native/cb/engine/engine.hpp (by name) |
| `engine/engine.py` | 268 | test_engine, test_knowledge_reads, test_native_engine; 1 modules | native/cb/engine/engine.hpp (by name) |
| `engine/facts.py` | 182 | test_engine, test_knowledge, test_native_engine, test_store; 3 modules | native/cb/engine/facts.hpp (by name) |
| `engine/native_engine.py` | 1048 | test_native_engine; 5 modules | native/cb/engine/{function_store,engine,facts}.hpp (this was the ctypes shell over cbengine.dll) |
| `exact/__init__.py` | 0 | test_dos, test_dpmi, test_port_fixes; 4 modules | (empty package marker) |
| `exact/aout.py` | 82 | 1 modules | native/cb/exact/aout.hpp (by name) |
| `exact/emu/__init__.py` | 0 | test_dos, test_dpmi, test_port_fixes; 3 modules | (empty package marker) |
| `exact/emu/dos.py` | 803 | test_dos; 2 modules | native/cb/exact/emu/dos.hpp (by name) |
| `exact/emu/dpmi.py` | 736 | test_dpmi; 1 modules | native/cb/exact/emu/dpmi.hpp (by name) |
| `exact/emu/native_dosexec.py` | 163 | test_port_fixes; 1 modules | native/cb/exact/emu/native_dosexec.hpp (by name) |
| `formats/__init__.py` | 0 | frozen_subjects, test_concolic, test_dos, test_drivers, test_exepack, test_media, test_native_formats, test_native_trace, test_pklite, test_plumbing, test_powerpc, test_run_record, test_ui, test_verification, test_versions, test_vmx128, test_xex, tools.golden; 47 modules | (empty package marker) |
| `formats/cab.py` | 177 | frozen_subjects, test_media; 1 modules | native/cb/formats/cab.hpp (by name) |
| `formats/detect.py` | 88 | frozen_subjects, test_plumbing; 8 modules | native/cb/formats/detect.hpp (by name) |
| `formats/dos.py` | 250 | frozen_subjects, test_dos, test_exepack, test_pklite, test_verification; 6 modules | native/cb/formats/dos.hpp (by name) |
| `formats/le.py` | 375 | frozen_subjects, test_drivers, tools.golden; 6 modules | native/cb/formats/le.hpp (by name) |
| `formats/native.py` | 223 | test_native_formats; 11 modules | native/cb/analyze/native.hpp (by name) |
| `formats/ne.py` | 163 | frozen_subjects, test_concolic, test_native_trace, test_verification, test_versions; 15 modules | native/cb/formats/ne.hpp (by name) |
| `formats/omf.py` | 248 | test_verification; 6 modules | native/cb/formats/omf.hpp (by name) |
| `formats/pe.py` | 176 | frozen_subjects, test_concolic, test_run_record, test_ui; 18 modules | native/cb/formats/pe.hpp (by name) |
| `formats/pe_functions.py` | 366 | frozen_subjects, test_versions; 4 modules | native/cb/formats/pe_functions.hpp (by name) |
| `formats/powerpc_image.py` | 128 | test_powerpc, test_vmx128; 5 modules | native/cb/formats/powerpc_image.hpp (by name) |
| `formats/resources.py` | 196 | frozen_subjects; 1 modules | native/cb/formats/resources.hpp (by name) |
| `formats/xex.py` | 173 | test_vmx128, test_xex; 5 modules | native/cb/formats/xex.hpp (by name) |
| `jobs/__init__.py` | 0 | test_knowledge_reads, test_plumbing; 4 modules | (empty package marker) |
| `jobs/pool.py` | 287 | test_knowledge_reads, test_plumbing; 4 modules | native/cb/jobs/pool.hpp (by name) |
| `knowledge/__init__.py` | 173 | test_knowledge, test_knowledge_reads, test_native_engine; 17 modules | native/cb/knowledge/package.hpp |
| `knowledge/reads.py` | 174 | test_knowledge_reads; 10 modules | native/cb/knowledge/reads.hpp (by name) |
| `knowledge/sources.py` | 116 | test_knowledge; 5 modules | native/cb/knowledge/sources.hpp (by name) |
| `knowledge/store.py` | 72 | test_knowledge, test_knowledge_reads, test_native_engine; 8 modules | native/cb/knowledge/store.hpp (by name) |
| `platforms/__init__.py` | 387 | test_port_fixes, test_run_record; 25 modules | native/cb/java/platforms.hpp (by name) |
| `platforms/dos.py` | 34 | - | native/cb/platforms/dos.hpp (by name) |
| `platforms/java.py` | 27 | - | native/cb/platforms/java.hpp (by name) |
| `platforms/vxd.py` | 43 | - | native/cb/platforms/vxd.hpp (by name) |
| `platforms/win16.py` | 58 | - | native/cb/platforms/win16.hpp (by name) |
| `platforms/win32.py` | 42 | - | native/cb/platforms/package.cpp (`win32()`) |
| `platforms/xbox360.py` | 45 | - | native/cb/platforms/package.cpp (`xbox360()`) |
| `source/__init__.py` | 0 | test_port_fixes, test_readable, translate; 3 modules | native/cb/java/emit/source.hpp (by name) |
| `source/assembly.py` | 185 | 1 modules | native/cb/source/assembly.hpp (by name) |
| `source/ne_asm/__init__.py` | 0 | test_port_fixes | (empty package marker) |
| `source/ne_asm/segdiff.py` | 212 | test_port_fixes | native/cb/source/ne_asm/segdiff.hpp (by name) |
| `source/placement.py` | 121 | 1 modules | native/cb/source/placement.hpp (by name) |
| `source/readable.py` | 644 | test_readable, translate; 1 modules | native/cb/source/readable.hpp (by name) |
| `store/__init__.py` | 0 | test_knowledge, test_native_engine, test_store, test_symbols, test_ui, test_ui_thin, test_verdicts, test_verification; 1 modules | native/cb/knowledge/store.hpp (by name) |
| `store/functions.py` | 126 | test_knowledge, test_native_engine, test_store, test_symbols, test_ui, test_verdicts, test_verification; 1 modules | native/cb/store/functions.hpp (by name) |
| `symbols/__init__.py` | 0 | test_dos, test_drivers, test_knowledge_reads, test_symbols, test_verification; 23 modules | native/cb/exact/symbols.hpp (by name) |
| `symbols/api_arguments.py` | 428 | test_knowledge_reads; 6 modules | native/cb/symbols/api_arguments.hpp (by name) |
| `symbols/demangle.py` | 107 | test_symbols; 2 modules | native/cb/symbols/demangle.hpp (by name) |
| `symbols/dos_interrupts.py` | 101 | test_dos; 3 modules | native/cb/symbols/dos_interrupts.hpp (by name) |
| `symbols/driver_services.py` | 280 | test_drivers; 4 modules | native/cb/symbols/driver_services.hpp (by name) |
| `symbols/libraries/__init__.py` | 104 | test_symbols; 5 modules | native/cb/symbols/libraries/package.hpp |
| `symbols/libraries/coff.py` | 166 | test_symbols; 2 modules | native/cb/symbols/libraries/coff.hpp (by name) |
| `symbols/libraries/omf.py` | 152 | 1 modules | native/cb/symbols/libraries/omf.hpp (by name) |
| `symbols/native.py` | 70 | 2 modules | native/cb/analyze/native.hpp (by name) |
| `symbols/ordinals.py` | 377 | test_symbols; 3 modules | native/cb/symbols/ordinals.hpp (by name) |
| `symbols/prototypes.py` | 124 | test_knowledge_reads, test_symbols; 6 modules | native/cb/symbols/prototypes.hpp (by name) |
| `symbols/runtime_library.py` | 386 | test_symbols; 3 modules | native/cb/symbols/runtime_library.hpp (by name) |
| `symbols/sdk_structs.py` | 265 | 3 modules | native/cb/symbols/sdk_structs.hpp (by name) |
| `symbols/wine16.py` | 89 | test_symbols, test_verification; 1 modules | native/cb/symbols/wine16.hpp (by name) |
| `symbols/xdk_libraries.py` | 151 | 2 modules | native/cb/symbols/xdk_libraries.hpp (by name) |
| `translate/__init__.py` | 6 | test_readable, test_verdicts, translate, tools.l2c; 11 modules | (package marker) |
| `translate/backends/__init__.py` | 1 | translate, tools.l2c; 5 modules | (package marker) |
| `translate/backends/asm.py` | 71 | translate; 1 modules | native/cb/translate/backends/asm.hpp (by name) |
| `translate/backends/c.py` | 96 | translate, tools.l2c; 4 modules | native/cb/translate/backends/c.hpp (by name) |
| `translate/backends/cpp17.py` | 53 | 1 modules | native/cb/translate/backends/cpp17.hpp (by name) |
| `translate/backends/module_file.py` | 127 | translate | native/cb/translate/backends/module_file.hpp (by name) |
| `translate/desugar.py` | 490 | translate; 1 modules | native/cb/translate/desugar.hpp (by name) |
| `translate/frontends/__init__.py` | 2 | translate | (package marker) |
| `translate/frontends/java/__init__.py` | 118 | translate | native/cb/platforms/java.hpp (by name) |
| `translate/frontends/java/lower.py` | 543 | translate | native/cb/translate/frontends/clang/lower.hpp (by name) |
| `translate/ir.py` | 320 | translate; 3 modules | native/cb/translate/ir.hpp (by name) |
| `translate/l2.py` | 431 | test_readable, test_verdicts, translate; 5 modules | native/cb/translate/l2.hpp (by name) |
| `translate/native_l2.py` | 340 | translate; 4 modules | native/cb/translate/native_l2.hpp (by name) |
| `verify/__init__.py` | 12 | frozen_subjects, test_concolic, test_native_engine, test_native_trace, test_plumbing, test_run_record, test_ui_thin, test_verdicts, test_verification; 12 modules | native/cb/port/verify.hpp (by name) |
| `verify/compare.py` | 228 | test_plumbing, test_verdicts, test_verification; 4 modules | native/cb/verify/compare.hpp (by name) |
| `verify/concolic.py` | 229 | test_concolic; 1 modules | native/cb/verify/concolic.hpp (by name) |
| `verify/guided.py` | 222 | 3 modules | native/cb/verify/guided.hpp (by name) |
| `verify/lane_common.py` | 264 | test_run_record, test_verdicts, test_verification; 2 modules | native/cb/verify/lane_common.hpp (by name) |
| `verify/lanes/__init__.py` | 4 | frozen_subjects, test_concolic, test_native_trace, test_run_record, test_verdicts, test_verification; 5 modules | native/cb/engine/lanes.hpp (by name) |
| `verify/lanes/win16.py` | 727 | test_run_record, test_verdicts, test_verification; 1 modules | native/cb/verify/lanes/win16.hpp (by name) |
| `verify/lanes/win32.py` | 917 | frozen_subjects, test_concolic, test_native_trace, test_run_record, test_verdicts; 3 modules | native/cb/verify/lanes/win32.hpp (by name) |
| `verify/verdicts.py` | 215 | test_native_engine, test_verdicts; 6 modules | native/cb/verify/verdicts.hpp (by name) |

## The tests: case-by-case

Every deleted suite file's cases, with the native test that checks the same behaviour (or the reason none is needed), as written in `docs/history/python-port/test_port_map.md` and `test_port_map_2.md` (batches 1-8; both copied here with this note). Tags of map 1: [C] covered by an existing native test, [P] ported; status words of map 2: COVERED, PORTED, NOT NEEDED, BLOCKED (none remain for these files). The sections for the deleted files follow verbatim.

## test_analysis.py (12)
- [C] test_image_32 -- analyze.image.a_pe_module_has_the_functions_the_live_loader_found, ...spans_imports_and_relocations...
- [C] test_image_16_and_driver -- analyze.image.an_ne_layout_puts_each_segment_at_its_selector, an_le_layout_names_what_the_device_block_gives_away
- [C] test_arguments_of_a_call -- analyze.conventions.an_argument_slot_the_code_reads_gives_the_argument_count, callers_that_clean_up_give_the_argument_count_away
- [C] test_call_graph -- analyze.call_graph.callers_users_and_cycles, calls_by_kind, reach_and_unreferenced
- [C] test_conventions -- analyze.conventions.a_function_that_removes_its_own_arguments_is_stdcall, golden_the_facts_of_fifteen_real_modules
- [C] test_globals_and_strings -- analyze.globals.golden_..., analyze.strings.conversions_match_the_engine_it_replaces, assemble_matches_the_engine_it_replaces
- [C] test_globals_count_indexed_arrays -- analyze.globals.arrays_are_indexed_types_not_strings
- [C] test_structures -- analyze.structures.a_pointer_field_an_array_a_float_and_a_called_field, the_report_counts_what_was_found
- [C] test_dispatch_and_resources -- analyze.dispatch.the_message_names, a_jump_table_is_a_switch_with_a_default; analyze.resources.reads_the_loads_and_the_controls
- [C] test_classes_none_in_c -- analyze.classes.decorated_names, a_16_bit_module_has_no_classes
- [C] test_observation_arithmetic -- analyze.observe.digest_counts_what_a_run_does_to_each_buffer, digest_reads_the_end_of_a_run, after_applies_the_runs_writes_to_a_buffer
- [C] test_report_and_fact_table -- analyze.report.a_16_bit_module, analyze.facts.*, analyze.report.the_names_of_the_analyses

## test_assembler.py (1: runs native/nasm/test/masm/run.py, flat and --objects)
- [P] flat fixtures vs golden -- build.masm_fixtures.every_fixture_assembles_to_its_golden_bytes (native/cb/build/masm_fixtures_test.cpp; masm_paren_operand known)
- [P] object backends (-f obj OMF, -f win32 COFF) -- build.masm_fixtures.segment_fixtures_give_the_same_code_in_the_object_backends

## test_concolic.py (11)
- [C] folded_expressions -- emulate.symbolic.concat_of_neighbours_is_the_whole, conditions_on_boolean_bytes_fold, extract_folds_through_what_it_takes_from
- [C] every_path_found_and_said_to_be_all -- verify.concolic.three_ways_through_two_branches, finds_each_path_of_a_chain_of_tests
- [C] seeds_not_run_again -- verify.concolic.seeds_that_took_a_known_path_are_not_run_again
- [C] loop_sampled_not_all -- verify.concolic.the_budget_limits_runs_and_says_the_paths_are_a_sample
- [C] shadow_gives_a_jumps_condition -- emulate.shadow.a_compare_and_branch_is_a_condition_of_the_path, memory_bytes_are_inputs_until_the_run_writes_them
- [P] a_run_with_its_shadow_is_the_same_run -- emulate.windows16.a_run_with_its_shadow_is_the_same_run (LZEXPAND, 8 entries x 3 inputs, with/without the real pcodecheck shadow: end, steps, calls, writes, results alike; skips without pcodecheck)
- [C] lifter_gives_the_specifications_p_code -- emulate.pcode.lifts_a_16_bit_instruction_in_its_window
- [P] 32_bit_run_with_its_shadow_is_the_same_run -- verify.win32.a_32_bit_run_with_its_shadow_is_the_same_run (LZ32 exports x the fixed+confirm vectors, fast tracer vs one-instruction shadowed run)
- [P] 32_bit_explorer_finds_every_path_of_lzopenfile -- verify.win32.the_explorer_finds_every_path_of_lzopenfile (complete, >=4 paths, each kept input takes a path no seed took)
- [C] powerpc_shadow_condition_over_an_argument -- emulate.shadow.lift_mode_follows_the_processor_language, emulate.pcode.lifts_powerpc
- [C] lifter_names_the_powerpc_registers -- emulate.pcode.lifts_powerpc

## test_converted.py (4)
- [C] layout_and_readme / one_project_per_binary -- base.converted.a_project_is_made_once_and_recorded_like_the_python_did, recording_a_command_again_replaces_its_step
- [C] generic_names -- base.converted.default_name_is_the_file_name_unless_generic
- [C] an_output_beside_the_project_is_not_inside_it -- base.converted.an_output_beside_the_project_is_not_inside_it

## test_dos.py (20)
- [C] kinds -- formats.dos.classification; com_and_driver_layout -- formats.dos.com_file_and_dos_stub_of_a_windows_module, drivers; relocations_are_applied -- formats.dos.mz_header
- [C] exepack_static_equals_emulated -- formats.dos.exepack_is_undone_statically, emulated_unpack_reports_failure; unpacked_file -- formats.dos.unpacked_file_reads_back
- [C] kernel_image_parts -- formats.dos.chain_of_programs; function_discovery -- analyze.dos_functions.find_exe, xcopy_exe; installed_handler_is_found -- analyze.dos_functions.an_installed_interrupt_handler_is_a_function
- [C] interrupt_names -- symbols.dos_interrupts.names_services, the_mouse_selects_by_all_of_ax, annotate; description_for_the_decompiler -- decompile.describe.dos_program_is_what_the_original_wrote
- [C] boot_floppy -- build.floppy.windows_95_setup_floppy_reads_and_makes_like_the_python; cd_image -- formats.iso9660.list; not_an_image -- formats.media.things_that_are_not_images
- [C] native_dos_switch -- build.native_dos.the_switch; native_masm_assembles -- build.native_dos.assemble_reads_back_the_products; native_masm_matches_dosbox -- build.masm.when_the_native_run_cannot_complete_dosbox_takes_over (DOSBox half skips as before)
- [C] dpmi_host_protected_mode -- exact.emu.dpmi.the_mode_switch_builds_the_tables_and_the_trampoline, simulating_a_real_mode_int21; mcb_arena_walk -- exact.emu.dos.the_chain_is_read_from_the_headers
- [C] native_tnt_ml_assembles / matches_dosbox -- build.native_dos.ml_toolchain

## test_dpmi.py (2)
- [C] ldt_overrun -- exact.emu.dpmi.a_full_ldt_is_an_error_not_an_overrun; gdt_selector_refused -- exact.emu.dpmi.specific_descriptors_and_raw_descriptor_access (0x8022 check)

## test_drivers.py (16)
- [C] table_names_services/identifiers/a_real_driver -- symbols.driver_services.size_of_the_real_table, labels, coverage_of_a_driver
- [C] reader_descriptor_and_entries/fixups_encode_back/container -- formats.le.splitter_descriptor_and_services, splitter_fixups, w3_container_of_the_real_vmm32
- [C] writer_round_trip -- source.vxd.le_writer.a_driver_written_back_reads_as_the_same_driver
- [C] judge_agrees_with_itself / run_records_service_calls -- verify.driver.each_entry_point_judged_against_itself_is_the_pythons_verdict, a_fingerprint_is_what_the_python_recorded
- [C] compare_share_ignores_patched_places -- build.driver.compare_counts_equal_bytes_outside_patched_places
- [C] build_from_source / source_written_builds_back -- build.driver.build_a_driver_with_openwatcom, source.driver_writer.a_driver_is_written_built_and_comes_back_the_original_module
- [C] kit_macro_source (+builds_back) -- source.vxd.idiom.the_ddb_as_a_macro, a_service_call_as_a_macro; source.vxd.from_source.splitter_comes_back_as_the_original
- [C] boot_disk_injection/boot_screens/boot_bisect -- source.vxd.boot.where_each_driver_goes_on_the_disk, what_a_screen_shows, bisection_finds_the_units_that_fail_alone

## test_engine.py (13)
- [C] made_once / provenance / given_fact_recomputes / same_value_stops_the_change / new_step_version / changed_binary / stale_side_by_side / reads_only_what_it_declares / batch_and_unkept / files_are_facts / values_and_blobs / old_versions_and_cap
  -- engine.engine.a_fact_is_made_once_and_kept, engine.facts.rows_keep_how_they_were_made, engine.engine.a_given_fact_changed_makes_stale_what_was_made_from_it,
  a_new_step_version_remakes_but_an_equal_result_leaves_dependents_alone, engine.facts.databases_are_per_binary_and_one_open_per_file, engine.engine.many_makes_the_stale_side_by_side_and_a_batch_step_in_one_call,
  a_step_may_only_read_what_it_declared, an_unkept_value_is_used_this_run_and_made_again_the_next, a_file_that_is_not_a_fact_is_an_input_by_its_hash, engine.facts.a_large_value_is_kept_once_by_its_hash, the_cap_removes_the_oldest_closed_database
- [C] a_lane_on_the_lanes_steps -- engine.lanes.the_steps_give_what_the_pythons_steps_gave_stage_by_stage

## test_exepack.py (1) [C] build.exepack.pack_roundtrip_both_headers

## test_flat_thunk.py (2) [C] build.flat_thunk.a_ret_cut_off_inside_its_operand_resolves_nothing, classify_finds_kernel32_whatever_the_case_of_its_name

## test_frozen.py (22 recorded cases) and test_frozen_slow.py (7)
- [C] c_text interrupt_helper_name/interrupt_helpers/interrupt_routines -- decompile.c_text.* (same names) (3)
- [P] c_text _far_loads/_global_type/_loads_dgroup/_more_helpers/_ram_type (5) -- decompile.c_text.frozen_* (the recorded cases copied to decompile/testdata/frozen/c_text; far_loads16/loads_dgroup16 exposed from c_text_win16.cpp)
- [C] formats ne_lzexpand, pe_lz32, resources_lzexpand, inspect_lzexpand, pe_functions_lz32, cab_mini, fonts_vgafix (7) -- formats.ne.*, formats.pe.lz32_*, formats.resources.the_resource_table_of_a_real_16bit_module, formats.inspect.describe_equals_the_recorded_original, formats.pe_functions.lz32_*, formats.cab.real_windows_95_mini_cab_against_expand, formats.fonts.a_fon_lists_its_fonts
- [P] formats ne_keyboard, pe_notepad, le_vmouse, dos_mode, resources_notepad, detect_vmouse, detect_mode (7) -- formats.frozen.* (recorded outputs in formats/testdata/frozen, compared fact by fact; keyboard.drv and vmouse.vxd are native testdata copies, notepad.exe and mode.com are read from the Win95 corpus and pass without it)
- [C] slow: describe ne_lzexpand, pe_lz32, le_vmouse, dos_mode, ne_keyboard -- decompile.describe.*_is_what_the_original_wrote, ne_driver_and_every_policy_switch (5); c_text_module lzexpand/lz32 -- decompile.c_text.compilable16_on_lzexpand, compilable32 (2)

## test_knowledge.py (8)
- [C] stamp_remake -- knowledge.store.ensure_makes_a_kind_again_only_when_the_stamp_changes; values_keep_their_shape -- knowledge.value.round_trip; name_follows_the_body -- knowledge.store.bodies_and_names; older_stores_bodies -- knowledge.store.carried_notes, store.knowledge_bodies.*
- [C] authored_files -- knowledge.store.authored_kinds_load_from_the_checkouts_files, knowledge.sources.real_files_every_kind_loads; stand_ins_are_data -- engine.win16.*, knowledge.package.win16_memory_is_scoped (same behaviour, not read side by side)
- [C] size_rule -- engine.facts.a_database_with_the_function_stores_tables_loses_only_its_facts; platform_names -- knowledge.package.platform_names

## test_knowledge_reads.py (7)
- [C] changed_prototype / absent_piece / argc_reads / named_body -- engine.engine.what_a_step_read_of_the_knowledge_base_is_an_input, knowledge.reads.digests_freshen_once_per_kind, knowledge.package.known_and_learn_record_the_reads, names_of_bodies
- [C] threads_and_workers -- knowledge.reads.adopted_on_another_thread, carried_through_a_worker_boundary; batch_keys -- knowledge.reads.recording_collects_sorted_unique; outside_a_step -- knowledge.reads.nothing_outside_a_recording, inner_recording_keeps_its_own

## test_media.py (12)
- [C] cabinet round trips/expand/rejects/checksum/real mszip/real quantum -- formats.cab.build_and_extract_round_trip, what_we_write_expand_reads, limits, checksum, real_windows_95_mini_cab_against_expand, real_quantum_cabinet
- [C] disc image round trip/bootable/name clashes -- build.disc_image.the_images_are_the_pythons, names_are_cut_to_level_one, formats.media.iso_built_disc, disc_with_an_el_torito_floppy_and_the_same_disc_in_raw_sectors
- [C] fat16 in partition / fat32 -- formats.media.fat16_and_fat32_with_a_folder_that_grows, partitions_primary_and_logical

## test_native_engine.py (4)
- [C] same facts and runs/store/knowledge/verdict words -- engine.engine.*, engine.facts.*, engine.function_store.a_store_written_by_the_c_engine_reads_the_same, knowledge.store.digests_follow_the_piece, engine.function_store.older_spellings_read_as_the_words_now (behaviour)
- [P] replay of tests/data/native_engine_golden.txt -- engine.engine_golden.the_same_facts_and_runs_as_the_python_engine (the "facts" case: same pipeline, same 18 asks and changes; value, kept/made counts per kind, what was made, every fact row incl. SHA-1 hashes; the golden converted to engine/testdata/engine_facts.golden.json). store/kb/verdicts cases stay [C] (above). Differences: the binary size, a file fact's folder, the engine's own file/binary step versions, and a batch step's input recorded twice by the Python are left out

## test_native_formats.py (1) [C] formats.pe.* (truncation/damaged), formats.ne.damage_never_crashes, formats.le.errors_and_damage, formats.dos.errors, reader facts on real files; [N] the Python-attribute digest mechanism

## test_native_trace.py (6)
- [C] same_record_synthetic/shadow32/driver/win16/lz32/record_has_what_was_run -- emulate.native_trace.*, emulate.native_shadow.*, emulate.native_driver.*, emulate.native_win16.*, emulate.emulator.the_real_lz32_dll_traces_the_same_fresh_and_kept (behaviour)
- [N] python_raises_through (Python hook plumbing); [P] tests/data/native_trace_golden.txt -- emulate.emulator.cases_the_recorder_agreed_gave_one_record_give_one_record: the golden is a digest of the Python's repr of each record, which cannot be reproduced without writing Python's text; what it pins that can be is which cases gave the SAME record (plain/stub/stub-unique-reads for f(7), g, h; judge/judge-capped for g, h) -- those groups are checked on the same module and the same eight configurations, with f(5) worked out from the code. The 16-bit / LZ32 / driver / shadow32 digests (lz16, lz32, driver, shadow) are not replayed: [C] by the native twins (emulate.native_win16.*, emulate.emulator.the_real_lz32_dll..., emulate.native_driver.*, emulate.native_shadow.*)

## test_pklite.py (1) [C] build.pklite.reads_a_real_module, encoding_what_was_read_gives_the_stream_again, parse_makes_the_choices_the_real_packer_made, pack_rebuilds_the_real_module

## tests/test_plumbing.py (18)
- test_decompiler_is_built -> NOT NEEDED: checks the native decompiler binary exists (an environment check; cb_tests link the code).
- test_openwatcom -> NOT NEEDED: tool-presence check; the code that needs OpenWatcom is tested through build.module.build_without_openwatcom_is_a_usage_error_and_a_foreign_kind_is_refused and build.driver.build_a_driver_with_openwatcom.
- test_corpus_rank_prefers_installed_copy -> COVERED: base.corpus.the_copy_the_os_loads_comes_first
- test_corpus_retail -> COVERED: base.corpus.retail_resolve_and_paths, base.corpus.a_bare_name_tries_the_usual_extensions
- test_corpus_modules -> COVERED: base.corpus.modules_smallest_first_by_lane_and_extension
- test_format_detection -> COVERED: formats.detect.identify_by_magic, dos_header, new_header_from_bytes, pe_machine_and_damaged_headers
- test_isolated_separates_failures_and_resumes -> COVERED: jobs.pool.isolated_separates_failures_and_resumes
- test_threads_keep_order_and_raise -> COVERED: core.parallel.parallel_map_keeps_order, core.parallel.parallel_for_rethrows_first_error_after_finishing
- test_throttle_is_safe -> NOT NEEDED: the Python wrapper that lowers a process's priority; the C++ side has no such wrapper (core.process / base.parallel.idle_cores_is_between_none_and_all cover what remains)
- test_workers_scale_and_low_load_caps -> COVERED: core.parallel.workers_policy, base.parallel.slots_hold_at_least_one_and_no_more_than_the_workers
- test_processes_safe_reads_the_guard -> NOT NEEDED: Python's `__main__` guard for multiprocessing; C++ workers are separate programs (jobs.workers.*)
- test_fast_unicorn_reads_are_the_same -> NOT NEEDED: emulate/fast_uc.py is a Python-binding speed-up; the C++ calls Unicorn directly (emulate.unicorn_uc.*)
- test_cache_drops_older_generations -> COVERED: base.cache.another_generation_goes_when_the_kind_is_first_used
- test_cache_holds_its_cap_least_recently_used_first -> COVERED: base.cache.hold_drops_the_least_recently_used_first
- test_fingerprints_are_kept_in_the_cache -> COVERED: verify.compare.fingerprints_round_trip_through_the_cache
- test_run_folders_are_deleted -> COVERED: base.runs.sweep_removes_only_old_run_folders, base.runs.folder_is_deleted_at_scope_end_even_by_an_exception
- test_tidy_state_finds_the_stale -> COVERED: base.tidy_state.stale_lists_each_rule_and_only_what_is_old, run_removes_the_stale_and_keeps_the_rest, dry_run_reports_and_removes_nothing

## tests/test_port_fixes.py (17 run by main())
- test_pe_writer_nosplit_keeps_the_scaled_index -> NOT NEEDED: the Python bug was control characters in a regex replacement string; the C++ writes the literal "[$1nosplit $2" (source/pe_writer.cpp). No native test reaches it: see Findings.
- test_cpukey_generated_keys_validate -> COVERED: formats.xbox.crypto.xecrypt.generated_keys_are_valid_and_seeded_runs_repeat, formats.xbox.crypto.tool_goldens.cpukey_ecd_agrees_with_the_python_on_its_verdicts
- test_sms_writer_game_gear_rom -> COVERED: source.sms_writer.the_game_gears_own_port_is_named_for_a_game_gear_only
- test_sd_signer_keeps_its_key_in_the_module -> COVERED: formats.xbox.crypto.sd_signer.signs_pads_and_zeroes_the_nonce, sd_signer.the_command_signs_a_file
- test_streamio_offset_labels_and_perform_function_at -> COVERED: formats.xbox.crypto.stream_io.labels_and_functions_at_an_offset
- test_apply_jump_sd_4bl_writes_the_branch -> COVERED: formats.xbox.crypto.build_lib.the_4bl_jump_branches_to_the_end_of_the_image
- test_xsm3_challenge_verify_answers_a_valid_packet -> COVERED: formats.xbox.crypto.tool_logic.xsm3_session_answers_the_challenge_the_way_the_python_does
- test_candidates_localize_twice_lays_out_the_merged_struct_array_the_same -> COVERED: exact.draft.candidates.localize_names_the_routines_and_the_data
- test_forms_library_macros_keep_the_include_a_macro_does_not_name -> COVERED: exact.draft.forms.keeps_the_include_a_macro_does_not_name
- test_forms_register_temps_asks_for_the_whole_register_name -> COVERED: exact.draft.forms.register_temps_names_the_whole_variable
- test_forms_default_after_keeps_the_statements_after_the_default_cases_return -> COVERED: exact.draft.forms.* (default_after cases in exact/draft/forms_test.cpp)
- test_java_rules_findings_of_one_line_come_in_name_order -> COVERED: java.rules.virtual_call_in_ctor.this_arrow_and_bare_calls_are_flagged (sorted-order check), java.rules.map_read_insert, java.rules.leak_drop_no_delete
- test_data_declared_size_takes_the_first_declaration_by_name -> COVERED: exact.data (declared_size is over a std::set: exact/data_test.cpp declared-size cases)
- test_segdiff_compile_group_leaves_no_work_folder -> COVERED: source.ne_asm.segdiff.a_group_is_compiled_and_cut_into_functions, base.runs.folder_is_deleted_at_scope_end_even_by_an_exception
- test_origin_notes_calls_to_compiler_helpers -> PORTED: analyze.origin.a_call_to_a_compiler_helper_is_noted_near_or_far
- test_native_dosexec_loads_the_dll_once_from_two_threads -> COVERED: exact.emu.native_dosexec.the_dlls_are_found_by_environment_or_in_the_checkout (the C++ loader is a function-local static; no two-thread race to test)
- test_draft_shapes_store_joins_at_the_end_of_the_text_finds_nothing -> COVERED: exact.recut / exact.draft.shapes_test.cpp (a store_joins case on a text with only closing braces after the store; DIVERGES note in exact/recut_test.cpp)

## tests/test_powerpc.py (1 script, 17 compiled-C cases)
- the compiled-C differential (arith divide udivide divconst shifts wide uwide compare narrow loop choose fib memory real single store calls) -> COVERED in kind: emulate.powerpc.add_and_immediates, multiply_and_divide, shifts_and_counts, rotates, compares_set_cr_fields, floating_point, loads_and_stores, branches_and_link, run_calls_out_through_externs, plus recompile.powerpc.every_instruction_as_the_reference_makes_it. The harness itself needs a PowerPC C compiler named in settings.json (the script skips without one), so its compiled-C half is not reproduced; the interpreter's behaviour per instruction family is.

## tests/test_readable.py (4)
- test_constants -> COVERED: source.readable.constants
- test_one_function -> COVERED: source.readable.one_function
- test_comparisons_fields_and_strings -> COVERED: source.readable.comparisons_fields_and_strings, source.readable.calls_are_read_with_their_arguments
- test_same_object_code -> COVERED: source.readable.proof

## tests/test_run_record.py (5)
- test_what_a_run_left_changed -> COVERED: verify.lane_common.changed_keeps_what_differs, changed_names_places_and_skips, changed_respects_endianness_most_and_everything; analyze.observe.digest_counts_what_a_run_does_to_each_buffer
- test_a_fingerprint_from_a_record -> COVERED: verify.lane_common.fingerprint_of_a_returned_run, fingerprint_of_a_run_that_did_not_return
- test_every_runner_returns_the_record -> COVERED: emulate.driver.the_run_record_carries_what_the_judge_compares, emulate.m68k.record_is_the_run_record, emulate.sm83.record_is_the_run_record, emulate.z80.record, emulate.powerpc.run_returns_and_counts
- test_the_32_bit_judge_s_memory_digest_is_as_it_was -> COVERED: verify.win32.judge_lz32, verify.driver.a_fingerprint_is_what_the_python_recorded
- test_the_16_bit_judge_s_memory_digest_is_as_it_was -> COVERED: verify.win16.a_fingerprint_is_what_the_python_recorded

## tests/test_service.py (11)
- test_in_process_without_a_service -> COVERED: ui.service.a_link_asks_the_service_when_it_runs_and_answers_here_when_not, ui.service.the_mode_is_read_from_the_environment
- test_started_on_demand -> COVERED: ui.service.a_job_runs_in_a_process_of_its_own_and_keeps_its_output, ui.serve.a_state_folder_has_one_fixed_port_the_python_service_picked_too
- test_the_token_is_needed -> COVERED: ui.mcp.the_http_transport_checks_the_token_the_host_and_the_session, ui.serve.the_wire_is_the_python_services
- test_queries_answered_by_it -> COVERED: ui.service.a_call_by_name_is_answered_and_its_failures_are_said, ui.service.the_client_talks_the_wire_of_the_service
- test_a_job_runs_in_it -> COVERED: ui.service.a_job_runs_in_a_process_of_its_own_and_keeps_its_output, ui.service.starting_a_job_on_a_module
- test_one_instance_per_state_folder -> COVERED: ui.serve.a_second_host_takes_the_next_port_when_the_first_is_held, ui.serve.a_state_folder_has_one_fixed_port_the_python_service_picked_too
- test_facts_through_it -> COVERED: ui.service.a_query_command_prints_into_the_answer
- test_mcp_tools_forwarded -> COVERED: ui.mcp.the_service_answers_the_tools_by_name, ui.mcp.tools_called_through_the_wire_answer_as_the_python_did
- test_gone_then_in_process -> COVERED: ui.service.a_link_asks_the_service_when_it_runs_and_answers_here_when_not
- test_idle_exit -> COVERED: ui.serve.it_exits_when_idle_and_when_its_program_changed
- test_stale_when_the_code_changes -> COVERED: ui.serve.a_code_stamp_changes_with_the_program_and_the_settings, ui.serve.it_exits_when_idle_and_when_its_program_changed

## tests/test_store.py (3)
- test_a_modules_rows_are_in_its_database -> COVERED: engine.function_store.record_and_read_candidates, engine.function_store.the_summary_follows_every_write, store.functions.the_overview_and_the_latest_classes_are_read_from_the_store
- test_an_older_store_is_carried_over_once -> COVERED: store.migrate_state.the_knowledge_base_comes_over_without_replacing_what_is_held, store.migrate_state.verdicts_follow_the_settings_that_are_the_same
- test_the_cap_leaves_the_verdicts -> COVERED: engine.facts.a_database_with_the_function_stores_tables_loses_only_its_facts, engine.facts.the_cap_removes_the_oldest_closed_database

## tests/test_symbols.py (11)
- test_demangle -> COVERED: symbols.demangle.* (constructors_and_destructors, data_and_special_names, command)
- test_definition_file_and_library -> COVERED: build.definitions.win16_definition_file, build.definitions.win32_definition_file, symbols.libraries.coff.archive_labels_and_shapes
- test_ordinals -> COVERED: symbols.ordinals.collect, collect_earlier_files_win_and_bad_files_are_skipped, collect_refuses_a_table_the_code_disagrees_with
- test_prototypes -> COVERED: symbols.prototypes.api_macros_and_windows_line_endings, com_declarations, later_folder_names_parameters_and_adds_functions
- test_signatures -> COVERED: symbols.api_arguments.api_names, resolve_follows_imports_and_forwarders
- test_coff_long_symbol_names -> COVERED: symbols.libraries.coff.damaged_objects, archive_that_is_cut_or_damaged (and the long-name table cases in symbols/libraries/coff_test.cpp)
- test_wine16_reads_the_folder_once -> COVERED: symbols.wine16.specs.a_wine_folder, symbols.wine16.kinds
- test_identify -> COVERED: symbols.identify.a_16_bit_module_is_identified_by_signature_with_segment_and_offset_addresses, a_dos_program_has_no_functions_to_identify
- test_store_identified_and_bodies -> COVERED: store.knowledge_bodies.what_is_recorded_reads_back_and_is_replaced, a_name_given_to_a_body_holds_in_every_module_with_it
- test_names_of_a_module -> COVERED: symbols.names.a_16_bit_module_names_what_it_exports, a_32_bit_module_names_its_exports_and_its_imports
- test_source_files_from_symbol_files -> COVERED: symbols.debug_build.* and verify.match16.source_files

## tests/test_translate.py (0 tests: a runner)
- runs tests/translate/ under pytest -> NOT NEEDED: pytest runner shim; each file of tests/translate is mapped below.

## tests/test_verdicts.py (6)
- test_the_classes_are_as_they_were -> COVERED: verify.verdicts.class_words_and_order
- test_every_older_spelling_is_read_as_now -> COVERED: verify.verdicts.older_spellings, verify.verdicts.parse_class
- test_each_class_has_one_title_and_one_label -> COVERED: verify.verdicts.class_words_and_order, verify.verdicts.judge_words
- test_a_store_written_before_reads_in_todays_words -> COVERED: engine.function_store.older_spellings_read_as_the_words_now
- test_the_module_writer_reads_a_verdict_by_provenance -> COVERED: verify.verdicts.of_row_and_index, verify.verdicts.judgement
- test_the_producers_speak_it -> COVERED: verify.verdicts.classify, verify.verdicts.good_and_pair

## tests/test_verification.py (46)  (the old Win16/Win32 reconstruction + verification lane)
- test_reconstruct_is_incremental -> COVERED: engine.lanes.a_new_process_asks_again_and_nothing_is_prepared_built_or_judged, engine.lanes.judged_is_the_rows_as_records
- test_function_store -> COVERED: engine.function_store.record_and_read_candidates, best_goes_by_class_then_by_the_engine_named_first
- test_variant_store -> COVERED: engine.function_store.variants_are_compared_by_their_bytes
- test_repair_knobs_preserve_meaning -> COVERED: verify.authored.repair_test.cpp (knob cases), exact.repair.a_literal_is_changed_where_it_stands_alone
- test_repair_search_with_any_compiler -> COVERED: verify.authored.repair_test.cpp, exact.repair.a_change_that_makes_the_function_the_programs_is_kept_with_write
- test_period_compiler_gives_the_original_back -> COVERED: exact.emu.xenix.* (the XENIX compile of the linker source), build.period16.build_compiles_each_module_and_links
- test_a_verdict_holds_until_what_it_was_for_changes -> COVERED: engine.function_store.a_changed_sig_drops_the_verdicts_under_other_profiles, engine.lanes.a_module_document_round_trips_and_a_hand_made_text_changes_the_sig_as_the_python_does
- test_period_source_gives_the_declarations_an_author_had -> COVERED: build.msc6.sdk_prototypes_and_calls, build.msc6.stand_in_from_header (period declarations), build.module.stand_in_from_header_declares_what_the_header_declares
- test_breaks_change_what_a_function_does -> COVERED: verify.teeth.operator, comparison, constant, plant_loop, whole_function
- test_other_shapes_of_the_same_function -> COVERED: verify.shape.switches, returns, folds, statements; verify/authored/search16_test.cpp
- test_object_file_of_the_16_bit_compiler_is_read -> COVERED: build.msc6.function_code_cuts_a_public_out_of_the_segment, formats.omf.* (object_code with fix-up mask), build.msc6.pascal_names_and_order
- test_the_16_bit_compiler_s_function_is_cut_out_by_its_name -> COVERED: build.msc6.function_code_cuts_a_public_out_of_the_segment, build.masm.functions_lists_the_publics_of_the_object_with_their_bytes
- test_pascal_calls_are_said_first_pushed_first -> COVERED: build.msc6.pascal_names_and_order (GetClientRect/Rectangle cases in build/msc6_test.cpp)
- test_a_constant_near_address_is_a_variable_and_a_batch_s_errors_name_their_functions -> COVERED: build/msc6_test.cpp (0x63b2 case)
- test_an_api_called_by_number_is_said_through_the_sdk_where_its_arguments_fit -> COVERED: build.msc6.sdk_prototypes_and_calls (USER_107 DefWindowProc), source.readable.calls_are_read_with_their_arguments
- test_a_temporary_used_once_goes_back_into_its_expression -> COVERED: build.msc6.fold_temporaries_puts_the_expression_back (LocalAlloc/lstrlen case in source/readable_test.cpp too)
- test_a_pair_of_parameters_used_only_together_is_one_far_pointer_and_a_c_named_import_is_cdecl -> COVERED: build/msc6_test.cpp (FP_OFF/FP_SEG, far-pair cases), decompile.frame16.a_decoder_of_the_callers
- test_the_last_mile_search_rewrites_a_comparison_the_way_a_source_had_it -> COVERED: verify/authored/search16_test.cpp (0x19 < / >= 0x1a case)
- test_the_frame_structure_is_taken_apart_into_the_locals_a_source_declares -> COVERED: build.msc6.split_frame_takes_the_frame_structure_apart
- test_a_c_function_s_arguments_are_what_its_caller_takes_off -> COVERED: verify.gate16.contract16.what_the_caller_takes_off, the_caller_takes_off_other_bytes
- test_a_call_s_arguments_are_what_its_callee_removes_of_what_was_pushed -> COVERED: verify.gate16.contract16.a_calls_arguments_are_what_its_callee_removes_of_what_was_pushed
- test_source_files_are_told_by_push_cs_calls_and_an_if_else_can_be_turned_around -> COVERED: verify.match16.source_files, build/module_test.cpp (push cs), verify/authored/search16_test.cpp
- test_a_cextdef_is_numbered_with_the_other_externals -> PORTED: formats.omf.a_cextdef_is_numbered_with_the_other_externals
- test_wine_s_spec_types_an_import_and_its_calls_are_said_one_argument_to_a_parameter -> COVERED: build.msc6.wine_typing_and_definition, symbols.wine16.read_spec.arguments_said_or_not
- test_a_typed_call_inside_the_arguments_of_another_is_said_with_it -> COVERED: build.msc6.wine_typing_and_definition
- test_a_number_with_a_leading_zero_is_decimal_in_the_texts_of_the_16_bit_lane -> COVERED: decompile.asm_blocks.place_cases (leading-zero case, header note of decompile/asm_blocks_test.cpp), C++ reads numbers with from_chars
- test_a_hole_of_no_length_is_no_hole -> COVERED: build.asm16.a_function_rebuilt_from_c_inside_the_run_is_a_hole, build.module.pack_holes_*
- test_a_far_call_to_an_import_with_an_addend_is_named_with_it -> COVERED: build.asm16.a_far_call_to_an_import_with_an_addend_is_named_with_it
- test_a_run_time_library_routine_is_named_outside_the_linker_s_bytes -> COVERED: symbols.names.a_runtime_library_function_is_named_for_its_library, symbols.runtime_library.compiler_from_marks_and_libraries
- test_a_function_reading_ax_dx_or_bx_before_writing_it_takes_them_as_fastcall_arguments -> COVERED: analyze/conventions_test.cpp, analyze/origin_test.cpp (fastcall cases), decompile/describe_test.cpp
- test_a_callee_that_gives_back_the_caller_s_ax_returns_no_dx_either -> COVERED: decompile/describe_test.cpp (CALLER_RESULTS cases)
- test_an_exported_function_s_parameters_are_typed_by_wine_s_spec -> COVERED: symbols.wine16.* and build.msc6.wine_typing_and_definition (IpFindFirstLine)
- test_only_visual_c_1_52c_runs_without_the_emulator -> COVERED: build.msvc152.compile_asks_the_emulator_for_the_right_run
- test_a_backpatch_record_is_added_into_the_code -> PORTED: formats.omf.a_backpatch_record_is_added_into_the_code
- test_a_verdict_is_for_the_compiled_code -> COVERED: engine.function_store.a_changed_sig_drops_the_verdicts_under_other_profiles, engine.lanes.a_lane_that_does_not_build_per_function_files_every_function_under_the_modules_signature
- test_cache_files_results_under_what_made_them -> COVERED: verify.compare.keys_name_the_revision_and_every_ingredient, base.cache.* (key cases)
- test_every_kind_keeps_fingerprints_and_writes_calls_alike -> COVERED: verify.lane_common.fingerprint_of_a_returned_run, calls_skip_cap_and_show, verify.compare.fingerprints_round_trip_through_the_cache
- test_the_judge_s_build_has_the_original_s_code_segment_where_c_still_reads_it -> COVERED: verify.win16.code_segments_the_c_still_reads_are_given_to_the_build
- test_the_uart_and_the_interrupt_controller_answer_as_the_chips_do -> COVERED: emulate.ports16.every_com_port_and_base_zero_has_a_uart, uart_line_status_says_transmitter_empty (and the interrupt controller cases in emulate/ports16_test.cpp)
- test_the_governor_shares_one_budget -> COVERED: emulate.native_win16.the_governor_ceiling_stops_a_run_that_maps_too_much, emulate.native_trace.needs_a_cache_and_a_governor, base.parallel.memory_allows_counts_pieces_past_the_reserve
- test_an_instruction_is_decoded_once -> COVERED: emulate.m68k.code_is_decoded_once_per_address, emulate.sm83.code_is_decoded_again_when_the_bytes_under_it_change
- test_chunks_come_back_in_address_order -> COVERED: exact.modules / exact.project (address-ordered functions), recompile.switches.every_bctr_of_the_code_is_scanned_in_address_order
- test_programs_stored_one_after_another_are_each_found -> COVERED: formats.dos.* and build.exepack/pklite (several programs in one file); decompile.jar.* none; see exact.exepack.reads_the_program_it_packed
- test_a_screen_shorter_than_the_pages_points_is_not_that_page -> COVERED: verify.gate16.setup16 (header note "a screen too short to hold one of the three pixels"), verify.gate16.setup16.pass_when_the_license_page_comes_up
- test_runs_of_an_image_lane_that_differ_in_how_many_places_they_show_are_a_difference -> COVERED: verify.lanes.image_lane.effects_of_different_length
- test_a_dos_program_whose_source_text_is_missing_still_has_its_function_keys -> COVERED: verify.lanes.dos.prepare_without_source

## tests/test_versions.py (9)
- test_default_product_is_untouched -> COVERED: base.corpus.root_of_the_default_product, base.corpus.products_and_selection
- test_selecting_a_product -> COVERED: base.corpus.selected_is_for_the_length_of_a_scope, base.corpus.products_and_selection
- test_boot_image_modules -> COVERED: base.corpus.boot_images_are_unpacked_once_into_the_state_folder
- test_fonts -> COVERED: formats.fonts.a_fon_lists_its_fonts, a_bare_fnt_is_one_font_and_other_files_none
- test_a_module_against_itself -> COVERED: compare.versions.a_build_is_identical_to_itself
- test_dos_program_and_shared -> COVERED: compare.versions.functions_shared_by_programs
- test_function_spans_are_the_carved_functions -> COVERED: compare.versions.load_reads_each_kind, store.versions_ledger.bodies_and_their_names_go_through_the_body_store
- test_two_products_32bit -> COVERED: compare.versions.windows_95_against_windows_98_is_what_the_original_wrote, compare.versions.a_whole_product
- test_two_products_16bit -> COVERED: compare.versions.windows_95_against_windows_98_is_what_the_original_wrote, compare.versions.the_command

## tests/test_vmx128.py (6)
- test_rows_decode_to_themselves -> COVERED: decompile.vmx128.table, decompile.vmx128.words
- test_sleigh_is_current -> COVERED: decompile.vmx128.sleigh_is_the_checked_in_language
- test_language_matches_table -> COVERED: decompile.vmx128.sleigh_without_siblings_defines_every_operation, decompile.vmx128.coverage
- test_x31_forms -> COVERED: decompile.vmx128.decode.opcode_31_unaligned_loads_and_stores
- test_game_vmx128_words -> COVERED: decompile.vmx128.decode.words, decompile.vmx128.fields
- test_other_words_unchanged -> COVERED: decompile.vmx128.decode.not_vmx128

## tests/test_xex.py (6 checks in one script)
- is_xex -> COVERED: formats.xex.probe, formats.xex.not_an_xex_is_a_format_error
- import_libraries -> COVERED: formats.xex.import_libraries_of_an_image_without_a_sidecar, formats_xex_imports (test in formats/xex_test.cpp: imports == xam.xex, xboxkrnl.exe)
- corpus.retail leaves other files -> COVERED: base.corpus.retail_resolve_and_paths (a path passes through)
- engine selftest -> COVERED: formats.xbox.crypto.harness.each_check_computes_what_the_pythons_xecrypt_does, formats.xex.run_passes_the_arguments
- engine usage -> NOT NEEDED: the usage text of the separate xex engine program (a Python subprocess check)

## tests/translate/ (24 files)

### test_c_backend.py (43) -- the C back end of the hub (native/l2: cbl2.dll), through native/cb/translate/native_l2
Each of these is PORTED as a replay of the recorded library calls: `translate.native_l2.cases.c_backend.<name>` (the name is the Python
test's without `test_`). native_l2_cases_test.cpp, data testdata/c_backend_cases.json. The three that were already COVERED by hand-written
tests are also listed in their native test.
- test_numbers_in_their_natural_base -> PORTED (12 calls) + COVERED: translate.native_l2.numbers_in_their_natural_base
- test_brackets_as_printc_places_them -> PORTED (6) + COVERED: translate.native_l2.brackets_and_conditions
- test_joined_conditions -> PORTED (4) + COVERED: translate.native_l2.brackets_and_conditions
- test_statements -> PORTED (1) + COVERED: translate.native_l2.function_text
- test_moved_passes_far_and_near_calls -> PORTED (4) + COVERED: translate.native_l2.passes_moved_onto_the_tree
- test_moved_far_from_stack -> PORTED (2)
- test_moved_frame_through_ss -> PORTED (3)
- test_moved_pieces -> PORTED (2)
- test_moved_registers_null_and_globals -> PORTED (1)
- test_a_module_as_one_tree_with_what_stayed_assembly -> PORTED (1)
- test_a_32_bit_module_as_one_tree -> PORTED (1)
- test_comments_where_printc_writes_them -> PORTED (1)
- test_a_long_line_broken_as_printc_breaks_it -> PORTED (2)
- test_moved_passes_that_read_the_module -> PORTED (3)
- test_moved_own_segment_calls -> PORTED (2)
- test_moved_interrupts -> PORTED (1)
- test_moved_selector_structs -> PORTED (6)
- test_moved_old_style_definition -> PORTED (3)
- test_moved_entry_sp_and_stack_arguments -> PORTED (1)
- test_decompile16_counts_calls_in_l2 -> PORTED (2)
- test_moved_far_addresses -> PORTED (1)
- test_moved_far_casts -> PORTED (1)
- test_moved_far_locals -> PORTED (1)
- test_moved_far_concat_casts -> PORTED (2)
- test_moved_buffer_fields -> PORTED (3)
- test_moved_far_reads -> PORTED (1)
- test_moved_near_to_far -> PORTED (1)
- test_moved_stack_segment_stores -> PORTED (1)
- test_moved_callee_pops -> PORTED (2)
- test_moved_caller_frame_slots -> PORTED (6)
- test_moved_selector_imports -> PORTED (1)
- test_moved_segment_values -> PORTED (2)
- test_moved_code_segment_reads -> PORTED (2)
- test_names_used_for_the_header -> PORTED (1)
- test_prototypes_where_the_calls_agree -> PORTED (8)
- test_moved_32bit_heading -> PORTED (9)
- test_moved_keep_frame -> PORTED (5)
- test_moved_32bit_globals -> PORTED (3)
- test_moved_fit_calls -> PORTED (4)
- test_moved_powerpc_passes -> PORTED (3)
- test_moved_struct_fields -> PORTED (5)
- test_inline_asm_block_and_a_label_ending_a_block -> PORTED (1) + COVERED: decompile.inline_asm.*, decompile.asm_blocks.*
- test_heading_declaration_from_l2 -> PORTED (1)

### test_l2_survives.py (4) -> PORTED, replayed: translate.native_l2.cases.l2_survives.*
- test_switch_on_a_pointer_keeps_its_case_labels_cast, test_switch_on_a_far_pointer_is_written_on_its_four_bytes, test_an_array_index_past_9_is_a_literal, test_a_supplied_jump_table_flow_never_reaches_is_dropped -> PORTED

### test_frame16.py (12)
- test_a_flag_kept_in_a_word_is_a_word -> COVERED: decompile.frame16.a_flag_kept_in_a_word_is_a_word
- test_inc_is_written_as_plus_plus -> COVERED: decompile.shape16.inc_is_written_as_plus_plus; PORTED replay of the library call (translate.native_l2.cases.frame16.inc_is_written_as_plus_plus)
- test_an_inc_of_a_register_stays_an_assignment -> COVERED: decompile.shape16.an_inc_of_a_register_stays_an_assignment
- test_an_inc_of_a_place_is_not_a_larger_step -> COVERED: decompile.shape16.an_inc_of_a_place_is_not_a_larger_step
- test_a_function_without_a_frame_is_left -> COVERED: decompile.frame16.nothing_to_lay_out
- test_the_pieces_of_a_buffer_are_one_array -> COVERED: decompile.frame16.the_pieces_of_a_buffer_are_one_array; PORTED replay (cases.frame16)
- test_the_back_end_writes_postfix_increments -> PORTED (2 library calls, cases.frame16) + COVERED: decompile.shape16.inc_is_written_as_plus_plus
- test_an_if_return_is_turned_round_where_the_original_has_the_other_first -> COVERED: decompile.shape16.if_return_turned_round
- test_a_piece_of_a_buffer_piece_is_read_at_its_place -> PORTED replay (cases.frame16) + COVERED: decompile.frame16.cases
- test_a_buffer_holding_a_far_pointer_is_left -> COVERED: decompile.frame16.a_buffer_with_a_far_pointer_is_left
- test_a_buffer_with_an_array_among_its_pieces_is_left -> PORTED replay (cases.frame16) + COVERED: decompile.frame16.cases
- test_an_unknown_local_in_a_place_of_its_own_keeps_its_type -> COVERED: decompile.frame16.cases

### test_asm_blocks.py (2)
- test_a_decimal_with_a_leading_zero_in_an_operand_is_a_place -> COVERED: decompile.asm_blocks.place_cases
- test_a_window_that_leaves_no_instruction_places_no_block -> COVERED: decompile.asm_blocks.place_degenerate_window

### test_authored_rewrites.py (2)
- test_a_comparison_with_an_octal_constant_is_left_alone -> COVERED: verify/authored/* tests (search16_test.cpp, repair_test.cpp octal cases)
- test_a_switch_chain_with_an_octal_constant_is_left_alone -> COVERED: verify.shape.switches, verify/authored/search16_test.cpp

### test_clang_frontend.py (7)
- test_declarators, test_layouts_follow_the_target -> COVERED: translate.clang.ctypes.parse_and_spell, translate.clang.ctypes.layouts_and_decay, translate.clang.ctypes.tokens
- test_c_declarations, test_c_bodies, test_c_unit_is_l2 -> COVERED: translate.clang.lower.programs_give_the_reviewed_documents, string_literals, the_unit_says_what_it_is
- test_cpp_classes, test_cpp_bodies -> COVERED: translate.clang.lower.programs_give_the_reviewed_documents, character_text_and_widths

### test_console_patches.py (3)
- test_sound_routes_the_name_and_drops_the_pool_query -> COVERED: java.minecraft.sound_fix.* (patch_music_runs_play_always_and_guards_the_deref, stranded_finds_a_guard_that_drifted)
- test_applet_ping_becomes_the_assignment -> COVERED: java.minecraft.applet_ping.patch_replaces_the_try_with_the_assignment, rewrite_matches_golden
- test_render_budget_and_hurt_guard -> COVERED: java.minecraft.render_fix.patch_budget_declines_without_the_early_out, java.minecraft.render_fix.hurt_guard_requires_hurt_time

### test_cpp03_bodies.py (3)
- test_bodies_come_from_the_tree -> COVERED: translate.cpp03.* (the corpus programs, body-by-body goldens in expected.txt)
- test_tree_bodies_beside_the_jvm -> COVERED: translate.cpp03.feat, translate.judge_native_jvm.*
- test_null_of_value_types -> COVERED: translate.cpp03.feat (null of value types, try/catch NPE case in 008_Ops.java), translate.cpp03.text.types

### test_cpp17_backend.py (7)
- test_records_get_their_implicit_members, test_instanceof_patterns_carry_their_binding, test_the_object_model -> COVERED: translate.cpp17.own.zoo, translate.cpp17.own.mix, translate.cpp17.corpus.t17
- test_the_program_reads_as_cpp17 -> COVERED: translate.cpp17.corpus.*, translate.cpp17.write_unit
- test_the_program_runs_as_the_jvm_runs_it -> COVERED: translate.judge_native_jvm.the_judge_compares_the_two_runs, translate.javajudge.judge_program
- test_desugar_lowers_what_cpp03_cannot_say -> COVERED: translate.desugar.* (instanceof_patterns, switch_expression_*, foreach_*)
- test_inside_the_transpiler_beside_cpp03 -> COVERED: translate.cpp17.paths, translate.cpp03.* (cpp03_cpp17 bridge)

### test_csharp_kit.py (13)
- test_every_corpus_program_lowers_to_valid_l2 -> COVERED: translate.csharp.lower.corpus_programs_give_the_reviewed_documents, translate.l2.* schema
- test_the_bound_tree_says_what_the_compiler_resolved -> COVERED: translate.csharp.lower.nodes_carry_their_positions_and_the_unit_its_language, a_symbol_kind_the_front_end_has_no_use_for_is_a_global
- test_what_cannot_be_carried_is_refused_with_its_reason -> COVERED: translate.csharp.lower.cases_give_the_reviewed_documents_and_refusals, csharp.decline.*
- test_csharp_round_trip_is_equivalent -> COVERED: translate.csharp.corpus.* (writes each corpus program), translate.judge.csharp_real
- test_the_back_end_writes_csharp_idioms -> COVERED: translate.csharp.expr.*, stmt.*, unit.*, decl.*, literals.*
- test_units_of_one_file_stem_are_separate_files_and_big_constants_fold_exactly -> COVERED: translate.csharp.program.file_names_and_writing, translate.csharp.expr.constants_that_overflow_are_unchecked
- test_java_to_csharp_is_equivalent, test_minecraft_alpha_noise_java_to_csharp_is_equivalent, test_java_semantics_are_kept_in_csharp -> COVERED: translate.csharp_java.corpus.calc, corpus.fixtures, corpus.modern, semantics_in_the_text, features
- test_csharp_to_cpp03_is_equivalent, test_csharp_to_cpp03_refuses_on_a_runtime_gap, test_csharp_to_cpp03_refuses_what_it_cannot_carry -> COVERED: translate.cs2cpp.convert, translate.cs2cpp.back, translate.cs2cpp.tool, translate.cs2cpp.javart
- test_the_judge_compares_output_and_exit_code -> COVERED: translate.judge.compare_equivalent, compare_divergent, compare_other_verdicts

### test_csharp_lower_trees.py (2)
- both -> COVERED: translate.csharp.lower.cases_give_the_reviewed_documents_and_refusals, corpus_programs_give_the_reviewed_documents

### test_desugar.py (3)
- test_a_foreach_over_an_array_becomes_a_counting_loop -> COVERED: translate.desugar.foreach_over_an_array
- test_a_labelled_loop_lowers_the_loop_inside_it_too -> COVERED: translate.desugar.foreach_label_and_nesting
- test_the_modern_java_program_is_lowered_as_the_cpp_module_lowers_it -> COVERED: translate.desugar.a_real_program

### test_dotnet_toolchain.py (2)
- both -> COVERED: translate.dotnet.discovery_on_an_installation, newest_version_dir, version_key, discovery_gaps

### test_java_as_c.py (2)
- test_classes_are_structs_and_tables -> COVERED: translate.c_java.corpus.*, translate.c_java.own.zoo, c_java.type_codes
- test_the_c_program_runs_as_the_jvm_runs_it -> COVERED: translate.c_java.runs.mix, runs.names, runs.zoo

### test_java_backend.py (6)
- test_faithful_tier -> COVERED: translate.java.golden.faithful_core, faithful_arith, faithful_overloads, faithful_va
- test_idiomatic_tier -> COVERED: translate.java.tier_choice, translate.java_objects.golden.*
- test_cpp_classes -> COVERED: translate.java_objects.golden.classes, golden.destructors, golden.structs
- test_goto_and_varargs -> COVERED: translate.java.golden.gap_goto, faithful_va
- test_refusals_say_why -> COVERED: translate.java.errors, translate.java_objects.refusals, translate.java_libc.unsupported_and_missing_rows_refuse
- test_java_literals_and_names -> COVERED: translate.java.int_literals, float_literals, string_literals, char_literal_text, jname

### test_java_frontend.py (3)
- all three -> COVERED: java.parse.* / java.pipeline (the Java front end is native/cb/java) and translate.l2.java_front_end_units; the dumper cache is Python-plumbing (NOT NEEDED: test_the_dumper_is_cached)

### test_java_lower.py (6)
- test_nothing_unsupported, test_types, test_names_are_resolved_by_symbol, test_constructors, test_modern_java_lowers, test_enums_and_anonymous_classes -> COVERED: translate.jmodel.*, translate.ir.*, translate.l2.java_front_end_units, translate.java.*

### test_l2.py (3)
- test_the_schema_is_valid_and_names_every_node -> COVERED: translate.native_l2.schema_check, translate.l2.errors
- test_lowered_java_is_valid_l2_and_round_trips -> COVERED: translate.l2.sym_roundtrip, types_roundtrip, java_front_end_units
- test_binary_facing_types_are_expressible -> COVERED: translate.l2.types_roundtrip, translate.ir.types.*, translate.l2.slots_that_hold_lists_or_nodes

### test_module_model.py (9)
- test_an_assembly_is_its_lines_as_fragments -> COVERED: translate.l2.assembly, translate.asm.first_line_of_each_unit
- test_the_assembly_back_end_writes_comments_and_declines_c -> COVERED: translate.asm.comment, translate.asm.declines
- test_emit_makes_each_function_a_fragment -> COVERED: source/*_writer tests (the native writers build the assembly tree: translate.l2.assembly, source.pe_writer.*); the Python test itself fails today (see Findings)
- test_the_readable_pass_reads_the_module_tree -> COVERED: source.readable.* , translate.l2.module_model
- test_the_port_reads_its_module_tree -> COVERED: port.* (module tree read), translate.l2.module_model
- test_the_module_file_is_written_from_its_tree_and_the_facts -> COVERED: translate.module_file.assembly, module_file.assembly_units_and_declines, module_file.c_whole_build_in_parts
- test_a_drivers_file_is_its_status_then_its_source -> COVERED: translate.module_file.parts_declines_other_documents, source.le_writer.*
- test_a_made_module_is_its_tree -> COVERED: translate.l2.made, translate.module_file.c_whole_build_failed, translate.native_l2.module_entries
- test_the_nasm_writers_make_each_function_a_fragment -> COVERED: source.pe_writer.*, source.le_writer.* (assembly()), translate.l2.assembly; Python test fails today (see Findings)

### test_native_l2_core.py (1)
- test_l2_round_trip_and_schema_the_same_both_ways -> COVERED: translate.native_l2.round_trip_as_the_typed_tree_does, translate.native_l2.schema_check; native/l2/tests/test_l2.cc

### test_port_ownership.py (5)
- the five ownership cases (transient elements freed, retained not, inline-owned argument hoisted / kept, owned member reassign deletes once) -> COVERED: java.ownership.owned_containers, owned_pointers, program_wide_catalogues, translate.cpp03.feat (ownership patterns: 009_Own.java)

### test_port_semantics.py (5)
- test_every_body_comes_from_the_tree, test_spellings_the_runtime_and_passes_rely_on, test_finally_runs_on_every_way_out -> COVERED: translate.cpp03.feat (try/catch/finally with jumps, labelled jumps, casts, boxed types), translate.cpp03.text.*
- test_beside_the_jvm, test_helpers_beside_the_jvm -> COVERED: translate.judge_native_jvm.*, translate.javajudge.* (they need a JVM and run on machines with one)

### test_python_frontend.py (4)
- test_semantics_translate -> COVERED: translate.python.lower.*, translate.python.methods.*
- test_refusals -> COVERED: translate.python.lower.refusals_have_words_and_reasons, a_refusal_reaches_whoever_calls_the_refused
- test_int_proofs -> COVERED: translate.pytypes.interval_results, every_operation_contains_its_results, translate.wide.*
- test_semantics_compile_and_match -> COVERED: translate.python.package.* , translate.python.lower.functions_are_written_as_l2 (the compile-and-compare half needs a C++ compiler)

### test_pytypes.py (5)
- test_xor_interval_holds_every_result, test_xor_of_equal_values_may_be_zero -> COVERED: translate.pytypes.xor_lower_bound, every_operation_contains_its_results
- test_other_operations_hold_their_results -> COVERED: translate.pytypes.interval_results, unary_operations_contain_their_results
- test_structfmt_accepts_what_struct_accepts, test_structfmt_refuses_what_struct_refuses -> COVERED: translate.structfmt.integers, repeat_counts, empty_and_prefix, refused

### Python-only plumbing in tests/translate
- test_java_frontend.py::test_the_dumper_is_cached -> NOT NEEDED: caches the JVM tree dumper's output in Python
- (no other test of the folder tests only Python plumbing; the folder tests the translator's *products* (L2, back ends, front ends), all of which exist natively)
