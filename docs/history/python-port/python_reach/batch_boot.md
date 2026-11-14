# Batch boot: the boot closure of the Python (verify/authored, verify/gate16, verify/lanes/powerpc, the module_writer closure)

Roots, scripts and conditions as in batches 1-5, plus (all in `python_reach/`): `boot.py` (the static closure of the four roots), `fixpoint.py` (the deletion set as a fixed point: the
modules that are dead once the tests that import a member are cut, with the keepers being the terminal UI, `base.settings`, batch 6's areas and every tool),
`holders.py` / `direct.py` (who still imports a module), `native_closure.py` (the native side, below), `cutfuncs.py` (cuts named test functions).
Made on the tree after batch 5 (`worktree-agent-ae5d84404904262f2` merged first): 286 modules / 98,610 lines before; 276 / 95,736 after.

## 1. The native boot uses none of this Python (and none of any other)

What `codebreaker regress all15` runs (native/cb/verify/regress.cpp, `gate16/setup16`, `build/module` through `engine/lane_kit` `module_services`), traced three ways:

1. **Source closure** (`native_closure.py`): from `verify/regress.cpp`, `app/codebreaker_main.cpp` and `engine/lane_kit.cpp`, every `#include "cb/..."` followed to its header and its same-stem
   `.cpp` (and unity siblings): **1,231 files in 26 areas**. Not one of them includes `py*.hpp` (native/ported), anything under `native/typed`, `native/pyrt`, `native/pyengine`, `compat/`
   (the old-path shims, `py_bridge.hpp`, `old_*.hpp`) or `codebreaker/...` headers, and none uses `py::`. The only `python` in the closure is `app/codebreaker_main.cpp`'s `find_python()`, used by
   the `ui` command alone (the terminal interface). No source in it spawns `python`, runs a `.py` or imports a module (grep of `\.py\b` in non-test native/cb: only comments, the UI launcher, the
   `settings` path rule, `translate/frontends/python` which reads Python as data, and `inspect`'s file-type table).
2. **Link closure**: `cb_regress` / `cb_codebreaker` link `cb_app` and the `cb_*` area libraries only. The shims under `native/cb/compat` and `native/typed` build only into `cb_tests` /
   `cb_shim_check` (CMakeLists lines 150-300), never into a program. `native/typed/*.json` is one manifest, `codebreaker.compare.image`; nothing on the boot path asks for it (it is
   reached only by Python that imports `codebreaker.compare.image`, and no Python on the path does).
3. **What the shipped exe runs before the native code**: `codebreaker.exe` is the translated Python plus native/cb. For a command in `NATIVE` (`regress` is) it executes exactly
   `cb_host_main_hook` (native), then the translated `codebreaker/__init__.py` (imports `base.settings` and calls `export_for_carried_over_code()`: sets `CB_CORPUS_ROOT`, `CB_OPENWATCOM`,
   `CB_DECOMPILER_BIN`, `CB_RESULTS_DB`), then `codebreaker/__main__.py` `main` -> `_native_command` -> `codebreaker_native.main(["cb", ...])` -> `cb_native_command`. **Three translated
   modules, 0 of them in this batch's set.** Production native/cb reads none of the four environment variables (grep: only `_test.cpp` files set or read them), it reads `settings.json`
   through `cb::config`. The plain program (`cb_codebreaker`, `cb_regress`) runs none of the three.
   `base/config.cpp` finds the checkout by the file `codebreaker/__main__.py` (it stays) and reads ~200 data files in `codebreaker/` (not Python: they stay).

Verdict: **the all-15 boot does not use the Python of this batch**, and the Python it does pass through is `__init__` + `base/settings` + `__main__`, which this batch keeps.

## 2. What was deleted

**10 modules, 2,874 lines**: `verify/authored/{__init__,match,match16,repair,search16,shape}` (1,966), `verify/gate16/{runtime16,scoreboard16}` (265), `build/period` (250),
`verify/lanes/powerpc` (393); the tests `tests/test_match_powerpc.py` (114) and `tests/translate/test_authored_rewrites.py` (29); eight cases of `tests/test_verification.py` (142 lines);
the `match`, `score16` and `repair` rows of `__main__.COMMANDS` (all three are in `NATIVE`); the `test_match_powerpc.py` row of `tests/run_all.py`.
Wiring changed: `platforms/xbox360.py` loses `lane` (now `None`: the lane is `native/cb/verify/lanes/powerpc`, as batch 5 did for dos/vxd), so `engine.lanes.KINDS` loses `powerpc`
and `tests/test_platforms.py` expects the shorter table; `tests/test_verdicts.py` loses four lines on `match._ORDER`/`match.compare`; `tests/translate/test_c_backend.py` loses the
`needed_header` assertion (a batch 6 file: a two-line cut, nothing else); one comment in `symbols/libraries/__init__.py`. `tools/check_imports.py`: 276 modules, 0 failing, 0 dangling.
Callers outside the repo (Dashboard, Windows95, Win95VM, XboxMCP): no reference to any deleted module or to `match`/`repair`/`score16` through python (the one hit is a copied rescue folder in
Dashboard/_scratchpad-rescue).

### The tests, case by case

| Python case | native test |
|---|---|
| test_verification `test_repair_knobs_preserve_meaning` (pad / order / swap `b + a` / `codebreaker_unused[12]`; the distance measure: identical, same size, masked call displacement ignored) | `verify.repair.knobs`, `knobs_frame`, `test_swaps`, `compare.mask.distance_*` (identical / same size / masked bytes ignored) |
| `test_repair_search_with_any_compiler` (a stand-in compiler: only the authored declaration order is at distance 0; the kinds `order register regparam`) | `verify.repair.repair_finds_the_authored_order`, `knobs` |
| `test_period_compiler_gives_the_original_back` (XENIX C compiling a source file of the 1984 linker; SKIPPED without `settings: xenix_c`, a tool outside this repo) | not ported as such: the native `repair --truth` says it does not run xcc.py (`source.pipeline.repair_truth_of_a_compiler_with_no_units`); the identical / different verdict is `compare.mask.distance_*`, the XENIX compiler itself `exact.xenix_c.*` |
| `test_period_source_gives_the_declarations_an_author_had` | **new** `source.pipeline_period.a_period_source_has_the_declarations_an_author_had` (+ `a_function_whose_result_is_used_keeps_its_type`); the `repair` knobs half: `verify.repair.test_swaps` (not a call, not `&`), `knobs_frame` (`frame4`) |
| `test_other_shapes_of_the_same_function` | `verify.repair.knobs_switch_return`, `verify.shape.switches` (default, `break` in a branch), `returns`, `returned_type` (narrower variable) |
| `test_the_last_mile_search_rewrites_a_comparison_the_way_a_source_had_it` | `verify.search16.each_rewrite_alone` (`0x19 < x` -> `x >= 0x1a`, `<= 0x19` -> `< 0x1a`), `rewrites_are_repeatable_and_varied` |
| `test_source_files_are_told_by_push_cs_calls_and_an_if_else_can_be_turned_around` | `verify.match16.source_files`, `verify.search16.if_else_blocks_and_swappable_statements` |
| `test_a_run_time_library_routine_is_named_outside_the_linker_s_bytes` | **new** `source.pipeline_period.a_library_routine_is_named_outside_the_linkers_bytes` (the same bytes and masks; plus the longest-routine rule) |
| test_authored_rewrites (octal constants in search16 / shape) | `verify.search16.each_rewrite_alone` (`x <= 010` left alone), `verify.shape.switches` (`x == 012`) |
| test_match_powerpc (4 cases) | `source.pipeline_ppc.*` and `source.pipeline.match_of_a_powerpc_image_as_the_python_did` (as the port map recorded) |
| test_verdicts `match._ORDER`, `match.compare` | `verify.verdicts.byte_match`, `class_words_and_order`, `compare.mask.distance_bytes` |
| test_c_backend `needed_header` assertion | **new** `source.pipeline_period.the_needed_header_keeps_a_structure_only_when_the_function_names_it` |
| test_platforms `every lane` (powerpc row) | `verify.lanes.powerpc.*` (the lane's own table), `platforms.*` |

Native code made testable for the three new tests (no behaviour change): `source/pipeline_period.hpp` declares `repair_period_source` and `repair_needed_header` (they were file-local in
`pipeline_match.cpp`); `source/pipeline_match16.hpp` declares `identify_run_time_library(routines, functions)` (the loading of the library folders is split from the matching).

## 3. What was NOT deleted, and what holds it

The static closure of `source/module_writer` is 191 modules, but it is mostly the same shared base the rest of the package stands on; the part that exists only for the boot is held by files
outside this batch's scope. Nothing below can go until those do. Holders (direct importers, `direct.py`):

| module | held by |
|---|---|
| `source/module_writer` | `symbols/names`, `source/java_writer`; `tests/java/test_kind.py` |
| `build/module` | `engine/lanes`, `engine/win16`, `platforms/win16`, `platforms/win32`, `source/module_writer`; `tests/test_whole_module.py`, `tests/test_platforms.py`, `tests/translate/test_c_backend.py` |
| `symbols/names` | `decompile/describe`, `verify/reconstruct`; `tests/test_symbols.py` |
| `verify/reconstruct` | `build/module`, `engine/lanes`, `verify/teeth`; **`tools/{causes,failures,oss_bench,sample}.py`**, `native/cb/engine/testdata/lane_kit/run_py.py` (a golden generator), `tests/{test_ui,test_platforms,test_verification,test_verdicts}.py`, `tests/java/test_kind.py` |
| `engine/lanes` | `platforms`, `verify/reconstruct`; `tests/test_engine.py`, `tests/test_platforms.py` |
| `verify/gate16/setup16`, `base/runs` | **`tools/record16/{record,copy_walk}.py`**, `tests/test_verification.py` (`test_a_screen_shorter...`), `tests/test_plumbing.py` |
| `verify/gate16/isolated16`, `contract16` | `build/module`, `engine/win16`; **`tools/record16/{record,replay}.py`, `tools/gate_teeth16.py`**, `native/cb/verify/gate16/testdata/gen_isolated16.py`, `tests/test_verification.py` |
| `verify/teeth` | **`tools/gate_teeth16.py`**, `tests/test_verification.py` |

The bold rows are `tools/` (batch 6's, not touched here): the recorder of Setup's 16-bit calls over QEMU's gdb stub (`record16/*`, no native twin: it writes pickles of memory for
`isolated16`), the teeth gate (`gate_teeth16.py`, native twin: `teeth` command, `source.pipeline_teeth`, `verify.teeth.*`), and four sweep/report scripts on `verify.reconstruct`.
Once those scripts are retired (or ported) and `test_engine` / `test_ui` / `test_whole_module` / `test_symbols` / the rest of `test_verification` are cut, the second deletion is
`source/module_writer`, `build/module`, `symbols/names`, `verify/reconstruct`, `engine/{lanes,win16}`, `verify/gate16/{setup16,isolated16,contract16}`, `base/runs`, `verify/teeth`,
the six `platforms/*` descriptors (their registry is the only Python reader), about 6,700 lines. The by-name map of the tests that reach them, so that cut is ready:

| Python test | native twin (by name; not run here) |
|---|---|
| test_whole_module: resources 32/16, round trips | `build.resources.*` (`a_16_bit_module_survives_the_round_trip_through_wrc`, ...) |
| `test_definitions_32_bit/16_bit` | `build.definitions.win32_definition_file`, `win16_definition_file` |
| `test_data_objects` | `build.data.*` |
| `test_build_32_bit_module`, `test_build_16_bit_module`, `test_fixed_code_segment_keeps_its_size` | `build.module.document32_*`, `document16_*`, `pack_holes_*`, `check_*`, `run_*`, `the_gate_*` |
| `test_flat_thunk_dll` | `build.flat_thunk.*` |
| test_symbols (demangle, definition file and library, ordinals, prototypes, signatures, COFF long names, wine16, identify, store/bodies, names of a module, source files from symbol files) | `symbols.demangle*`, `symbols.libraries.*`, `symbols.ordinals*`, `symbols.prototypes*`, `symbols.api_arguments*`, `symbols.wine16*`, `symbols.identify*`, `symbols.runtime_library*`, `symbols.names*`, `symbols.debug_build*` |
| test_engine (13) | `engine.engine.*`, `engine.facts.*`, `engine.function_store.*`, `engine.lanes.*` (test_port_map, half 1) |
| test_drivers (16) | `verify.driver.*`, `source.driver_writer.*`, `emulate.driver.*`, `build.driver.*` (batch 5 table; this file does not reach the closure) |
| test_readable, test_port_fixes (`msvc152`, `segdiff`) | `decompile.*` / `build.msvc152.*` / `source.ne_asm.segdiff.*` (not touched: they do not import this batch's modules) |
| test_ui | the terminal interface: stays |

## 4. The five conditions, for the ten modules

1. **Their C++ is committed as source.** Yes: `native/cb/verify/authored/{match16,repair,search16,shape}`, `verify/gate16/scoreboard16`, `source/pipeline_{match,match16,ppc,score16,teeth}`, `verify/lanes/powerpc`,
   `build/period16`, `compare/mask`; the older translation in `native/typed/cpp/codebreaker/...` (checked by `cb_shim_check`, untouched).
2. **Their tests pass as C++.** `tools\build_cb.cmd area source`: see section 5. The cases above are the Python tests' native twins.
3. **The judge traces agree.** These are authoring tools (a match score, a repair search), not judged code. The agreement evidence is the Python-made goldens the existing native tests compare
   against (`verify.match16.*`, `verify.repair.*`, `verify.search16.*`, `verify.shape.*`, `source.pipeline.{match_lzexpand,repair_lz32,score16_...}_as_the_python_did`).
4. **Every caller runs through codebreaker.exe.** `match`, `repair`, `score16` are in `NATIVE` (the exe runs them natively); the `COMMANDS` rows (the plain-python fallback) are gone; no tool, test or
   outside repo imports a deleted module (`check_imports`: 0 dangling).
5. **The all-15 boot passes with the exe.** Not claimed here: the worktree's boots fail from the copied environment. Section 1 shows the boot path reaches none of the deleted files, so the
   integrator's `regress all15 --runs 2` on master after the merge is the confirmation, not a new risk.

## 5. What was run in this worktree

- `tools\check_imports.py` (plain python): 276 modules, 0 fail to import, 0 dangling imports.
- `tools\build_cb.cmd area source` (CB_BUILD_JOBS=3): `cb_tests_source` 433 passed, 0 failed (32,491 checks), including the 4 new `source.pipeline_period.*` tests (the native code gives what the Python cases asserted).
- `tools\build_cb.cmd shims`: compiles (exit 0; only the old layer's existing warnings).
- Not run here: `codebreaker.exe tests/run_all.py --quick --jobs 2` and `tools\gate.cmd cb_tests` (no pyengine/pyrt/exe is built in the worktree; the translated exe is about an hour cold, full cb_tests about 40 minutes).
  Plain-python runs of test_platforms / test_verdicts / test_verification fail only on "formats.dll / cbengine.dll not built"; the edited cases themselves pass. The integrator's merge check covers these,
  and `regress all15 --runs 2` is condition 5.
