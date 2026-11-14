## Batch 3: 73 modules, 16,964 lines; two tools retired (and what the roots are now that every command but `ui` is native)

Deleted: **73 modules, 16,964 lines** (`wc -l`), leaving 387 modules and 124,461 lines under `codebreaker/` (460 / 141,557 after batch 2); plus 16 dead definitions of surviving modules that `tools/tidy.py --apply` removed (their only callers were deleted: 111 lines), and
`tools/py2cpp.py` (300 lines; `tools/judge_c_java.py` was NOT retired, see (2)). Twenty-four `COMMANDS` rows are gone from `codebreaker/__main__.py`: `invert vxd ne-asm dos-build
boot-floppy disc versions fonts coverage reach mcp progress facts knowledge recompile-project xex-imports ppc-calls xex-edit tidy-state build16` (the
module of each is deleted) and the stale `crypto` row of batch 1. All 24 are in `NATIVE`, so `codebreaker.exe` never ran those modules.

### (1) The live roots, computed again (scripts: `python_reach/graph.py`, `live3.py`, `pool3.py`, `solve3.py`; the old `report.py` is updated to the same roots)

With `NATIVE` naming every command but `ui`, the Python that the translated exe (or `python -m codebreaker`) can reach from a command line is:

- `codebreaker/__init__.py` and `codebreaker/ui/{__init__,client,terminal}.py` (+ `terminal.tcss`): the terminal interface. `tests/test_ui_thin.py` runs it with only those
  files in front of it, so they import nothing else: `codebreaker/base/settings.py` at most.
- `codebreaker/__main__.py`: `main()` runs `corpus.take_option` and dispatches. Its only import of ours is `codebreaker.base.corpus` (module level: `base.settings`). The lazy
  imports inside corpus.py (`formats.detect`, `formats.ne`, `platforms`) belong to its other callers (tests, tools), not to what `__main__` runs.
- `native/typed/*.json`: only `codebreaker.compare.image` (the Python file was deleted earlier; the typed C++ answers for it in the exe, which is why plain `python tools/check_imports.py`
  lists the `compare.image` dangling imports of `source/*_writer.py`: they pass in `codebreaker.exe`, as they did before this batch). The compat shims under `native/cb/compat/` call native code only.
- So from commands, the UI and `__main__`: **8 modules, 2,093 lines.** Everything else under `codebreaker/` is alive only because a `tools/*.py` imports it, a Python test imports it, or
  a module that one of those reaches does (lazy imports and dotted-name strings included in the graph, so this over-counts rather than under-counts).
- `tools/*.py` still used (by a suite, a cmd file or another tool): `check_imports`, `tidy` (both are SUITES rows), `freeze_tests` / `golden` / `l2c` / `sleigh_verify_6502` (imported by
  `tests/test_frozen.py`, `test_native_trace.py`, `translate/test_c_backend.py`, `test_nes.py`), `carve`, `merge`, `move`, `prune`, `unreachable`, `make_api_table`, `failures`, `causes`
  (the `tidy` family and the dev reports). Not used by anything but themselves: `bench_re, cache_tidy_proof, cb_order, chart, gate_teeth16, ghidra_fork, goto_report, l2check, oss_bench, sample, sleigh_verify_ppc, struct_report,
  trace16, verdict_diff`, `tools/record16/*`, `tools/cb_shim/sample_*_cases.py`. Their closure reaches 200 modules / 67,612 lines (verify.gate16, verify.reconstruct, decompile.decompiler, exact.draft ...):
  **retiring those dev scripts is the next lever** (they are not part of any gate and none is a caller of a command), but it was not asked for here and some are Logan's
  analysis helpers, so they stay and are named.

### (2) The two tools

**`tools/py2cpp.py` is deleted** (300 lines). Nothing runs it: no suite, cmd file, tool, native source or other repository (Dashboard, Windows95, Win95VM, XboxMCP, LoganOS:
searched) names it except docs, a comment in `native/ported/ne/CMakeLists.txt`, a work-folder path in `tests/translate/test_python_frontend.py` and a comment in pyengine. The ports it made are
committed and `pyengine --dynamic` is the translator since 2026-11-05. docs/PYTHON_FRONTEND.md and NATIVE.md, which showed its command lines, now say it is retired and in git history. Its
modules (the Python front end, `cpp17`, ...) are held by `tests/translate/*` now, not by a tool.

**`tools/judge_c_java.py` is KEPT, on purpose.** I deleted it first; `tidy.py --apply` then found `judge_native_jvm.roundtrip` and `translate/frontends/clang/j2x.py` (302 lines) unreached, because
the tool's `roundtrip` mode (Java -> C++ -> Java, TRANSLATOR_DESIGN.md phase 5) was their only caller. `j2x.py` is the pass that maps the Java runtime written in C++ (`JavaString`, `JArray<T>`, `JCat()`, `System::out`)
back onto Java's library; **it has no native counterpart**: `native/cb/translate/judge_native_jvm.hpp` has the `roundtrip` function but takes the pass as a host hook (`lift_java_runtime`) that
no native host binds. Deleting the tool would delete the only working Java round-trip judge. So the tool, `roundtrip` and `j2x.py` are restored (and the docs/manifest lines about the tool put back).
The native gap to close first: port `j2x.py` as the native `lift_java_runtime`; then retire the tool (it keeps `translate.judge_native_jvm`, the clang front end and the Java back end reachable from a tool:
8 modules, 5,680 lines, which are also held by `test_clang_frontend` / `test_java_backend`).

### (3) What was deleted, and the five conditions

| package | modules | lines | native replacement | tests removed (and their native fate) |
|---|---|---|---|---|
| `source/vxd` (boot coalesce corpus disassembly from_source idiom le_writer link loadable_corpus masm owbuild run services symbols tree, `__init__`) | 16 | 5,186 | `native/cb/source/vxd/*` | `tests/test_drivers.py`: 5 of 17 cases cut (`kit_macro_source` -> `source.vxd.idiom.the_ddb_as_a_macro`, `a_service_call_as_a_macro`, `a_control_procedure_with_a_jump_table`; `kit_macro_source_builds_back` -> `source.vxd.from_source.splitter_comes_back_as_the_original`, `splitter_compiles_and_links_with_the_period_tools`; `boot_disk_injection` -> `source.vxd.boot.where_each_driver_goes_on_the_disk`; `boot_screens` -> `what_a_screen_shows`; `boot_bisect` -> `bisection_finds_the_units_that_fail_alone`). The other 12 stay |
| `invert` (compilers early_c early_c_check early_c_forward early_c_frame early_c_frame_probe early_c_order_probe early_c_round_trip grammars known_differences msc6 msc6_{calls,function,idioms,registers,templates,walk} msvc2 msvc2_forward msvc2_registers quad_align quad_dump run segment_thunk whole_function) | 25 | 2,105 | `native/cb/invert/*` (`early_c*`, `msc6*`, `msvc2*`, `command`, `module_pass`, `production`, `quad_*`, `grammars`, `known_differences`; the name heuristic of report.py finds no same-name file for 12 of them: checked by hand, they are inside `msc6_small`, `msc6_segment`, `module_pass`, `production`, `msvc2_battery`) | `tests/test_invert.py` deleted whole (15 cases, all `[C]` in test_port_map.md: `invert.early_c.golden_cases`, `early_c_frame.ground_truth_frames`, `msc6_small`/`production_msc6`, `msvc2_test`, `known_differences`, `command.the_table_has_every_tool_of_the_python`, `modules_test` (`module_pass`), `quad_align.functions_of_real_objects`, `grammars`, `source.ne_asm.*`, `symbols.debug_build.*`); `invert/flow16.py`, `native_invert.py` stay (a tool and `exact.draft` hold them) |
| `source/ne_asm` (datacode idiom reachability recon run sweep) | 6 | 1,524 | `native/cb/source/ne_asm/*` | same `test_invert.py` (`16bit_assembly_source` -> `source.ne_asm.idiom.the_source_of_a_range`, `...nasm_is_run_on_the_source_and_its_words_are_kept`); `ne_asm/{__init__,fixups,segment,segdiff}.py` stay (`test_port_fixes.py`, not mapped) |
| `symbols/{debug_build,debug_build_estimate}` | 2 | 364 | `native/cb/symbols/debug_build*.cpp` | `test_invert.py::debug_build_names` -> `symbols.debug_build.*` (`a_signature_masks_what_depends_on_the_offset`, `debug_build_estimate_test`) |
| `recompile/{gpu,project,title,x360project}` | 4 | 2,266 | `native/cb/recompile/{gpu,project,title,x360project}.*` | none imported them (the `recompile-project` command only); native `gpu_test project_test title_test x360project_test` |
| `ui/{mcp_server,service,progress}` | 3 | 1,916 | `native/cb/ui/{mcp_*,service*,progress*,serve*}` | `tests/test_mcp.py` deleted whole (13 cases, `[C]` in test_port_map.md: `ui.mcp.*`, `mcp_server_test`, `mcp_services_test`; it needed `pip install mcp` and an in-process client, so it skipped in the exe anyway); `tests/test_ui.py`: `ledger_agrees_with_the_store`, `ledger_reads_without_writing`, `progress_reading`, `service_reads_the_store` cut (-> `coverage.ledger.counts_come_from_the_store_by_the_best_candidates_class`, `the_json_and_the_csv`, `engine.function_store.read_only_stores_read_and_never_write`, `ui.progress.snapshot_rate_and_eta`, `line`, `ui.progress_store.results_in_the_store_are_the_progress`, `ui.service.a_functions_answer_is_the_page_document`, `a_call_by_name_is_answered_and_its_failures_are_said`); the inspector cases and `test_terminal_interface` stay |
| `coverage/{__init__,ledger,reach}` (the package, folder removed) | 3 | 716 | `native/cb/coverage/{ledger*,reach*}` | `tests/test_platforms.py`: `reach: platforms, gaps and data told apart` cut (-> `coverage.reach.describe_by_platform_gap_or_nothing`, `gap_of_names_platforms_without_a_lane`, `reach_command_test`); test_ui's ledger cases above |
| `compare/versions` | 1 | 871 | `native/cb/compare/versions*.cpp` | `tests/test_versions.py`: `a_module_against_itself`, `dos_program_and_shared`, `two_products_32bit`, `two_products_16bit` cut (-> `compare.versions.a_build_is_identical_to_itself`, `functions_shared_by_programs`, `windows_95_against_windows_98_is_what_the_original_wrote`, `names_and_reconstructions_carry_across_builds`, `a_whole_product`); the corpus, boot-image and carved-span cases stay |
| `formats/fonts` | 1 | 115 | `native/cb/formats/fonts.*` | `test_versions.py::fonts` cut (-> `formats.fonts.glyphs_are_drawn_from_the_bitmaps`, `a_fon_lists_its_fonts`); the frozen subject `fonts_vgafix` and `tests/fixtures/frozen/formats/fonts_vgafix.json` removed (-> `formats.fonts.a_fon_lists_its_fonts`, already `[C]` in the map) |
| `analyze/ppc_calls`, `formats/xex_imports` | 2 | 354 | `native/cb/analyze/ppc_calls*`, `native/cb/formats/xex_imports*` | `tests/test_ppc_calls.py` deleted whole (3 cases -> `analyze.ppc_calls.what_an_instruction_writes`, `consensus_of_call_sites`, `analyze.ppc_calls_command.a_module_is_analysed_like_the_python_did`; the third needs `Converted/Minecraft`, which was cleared, and skipped) |
| `build/{ppc_asm,period16,disc_image,floppy}`, `formats/xex_edit` | 5 | 1,125 | `native/cb/build/{ppc_asm,period16,disc_image,floppy,xex_edit}.*` | `tests/test_xex_edit.py` deleted whole (4 cases -> `build.ppc_asm.encodings`, `branches`, `errors`; `build.xex_edit.privileges_are_read_set_and_cleared_where_they_stand`, `code_edits_write_the_same_images_as_the_python`); `test_media.py`: the three disc-image cases cut (-> `build.disc_image.the_images_are_the_pythons`, `names_are_cut_to_level_one`, `an_image_is_listed_and_extracted_through_the_media_readers`); `test_whole_module.py`: `16bit_dll_with_the_period_tools` cut (-> `build.period16.build_compiles_each_module_and_links`, `segments_and_compare`, `period16_command_test`) |
| `base/{tidy_state,commands}` | 2 | 239 | `native/cb/base/{tidy_state,commands}.*` | `tests/test_plumbing.py::tidy_state_finds_the_stale` cut (-> `tidy_state_test`: stale old caches, run folders, strays) |
| `engine/show`, `knowledge/show`, `source/dos_build` | 3 | 183 | `native/cb/engine/show*`, `native/cb/knowledge/show*`, `native/cb/source/dos_build*` | none imported them (commands only) |

Run-all rows removed: `test_mcp.py`, `test_ppc_calls.py`, `test_invert.py`, `test_xex_edit.py`. The golden generators under native testdata did not import any of these modules
(searched; nothing to delete there).

**The five conditions**

1. **C++ committed**: the files above, checked by hand one by one (the table), and `native/typed/cpp/codebreaker/**` (untouched, `cb_shim_check`).
2. **Own tests and every suite that exercised it pass as C++**: the native tests named above pass in this tree's checks (area builds and the gate below); every Python test that imported a
   deleted module was deleted or cut, and every other suite is in the gate run below.
3. **Judge / agreement**: these packages hold no per-body verdicts; the agreement evidence is the goldens the Python made, compared by the native tests above (`*_the_pythons*`, `golden_cases`,
   `the_images_are_the_pythons`, `code_edits_write_the_same_images_as_the_python`, `a_module_is_analysed_like_the_python_did` ...), and the 114 java-less test_port_map rows.
4. **Every caller through the exe**: none of the 73 is in the closure of the UI or `__main__`, of any tool still used, or of a surviving module (the solver keeps a module when anything
   that stays imports it); Dashboard, Windows95, Win95VM, XboxMCP and LoganOS have no hit for the module names or `python -m codebreaker <command>` of these; the 24 `COMMANDS` rows
   went with their modules. One dynamic edge is left dangling on purpose and is unreachable: `exact/draft/__init__.py::run` does `importlib.import_module("codebreaker.invert." + model)`; it
   runs only for `codebreaker exact ... draft`, a native command (`native/cb/exact/draft/*`), and the Python module is itself held only by `tools/cb_shim/sample_*_cases.py`.
   `native/pyengine/hints/stubs/*.pyi` and `islands.json` still name some deleted modules (hints for the translator; harmless; the same was true after batches 1 and 2).
5. **All-15 boot**: none of the 73 is in the Python closure of `verify/gate16/*` or `source/module_writer` (the `boot` flag of report.py is false for every one; `verify.authored.*`, `verify.lanes.powerpc`
   and `verify.gate16.{runtime16,scoreboard16}` are the ones that are, and they stay), and `regress` is native. `codebreaker regress all15 --runs 2` was NOT run in this worktree (as instructed):
   the integrator runs it on master after the merge.

### What was verified here

- `tools\build_codebreaker_exe.cmd fast` (after `build_native.cmd pyengine pyrt` and the other libraries; first build in this worktree needed `_CL_=/wd4702`, as before) builds; rebuilt after the last edit.
- `codebreaker.exe tools/check_imports.py`: **387 modules import, 0 dangling imports** (plain `python tools/check_imports.py` lists the 43 `compare.image` failures it listed before: that module is the typed manifest, see (1)).
- `codebreaker.exe tools/tidy.py`: clean. It found 16 definitions of surviving modules whose only callers were deleted here (`build.dos_tools.masm_root`, `formats.le.HEADER_SIZE`, 7 functions of
  `invert.native_invert`, `recompile.config.write_switch_tables`, 3 of `source.ne_asm.segdiff`, `translate.backends.cpp17.header_path`) and `--apply` removed them. It also wanted to remove `translate/frontends/clang/j2x.py` and
  `judge_native_jvm.roundtrip` when I had retired `judge_c_java.py`: that is what stopped me retiring it, see (2).
- **`codebreaker.exe tests/run_all.py --quick --jobs 2 --no-build`: ALL SUITES PASSED, 42 suites in 1,479 s** (test_java 374 passed, test_translate 190, test_recompile 7 in 1,025 s -- `native/xex` is built here, which is what makes
  `test_whole_image` run -- test_ui 5 passed 1 skipped, test_drivers 12, test_versions 4, test_media 9, test_whole_module 14, test_plumbing 16, ...). The suites that were removed or cut are listed in (3).
- `tools\gate.cmd`: **`cb_tests`: 5,595 passed, 0 failed (8 shards)**, built from this tree. UI tests (`python tests\test_ui.py --only test_terminal_interface`, `python tests\test_ui_thin.py`): **pass** (the first run found a bug of
  mine: test_ui's `_fill_store` called `ledger.profiles()`; now `reconstruct.digests()["pe"]`). The 155 `cb_exe_gate` command cases: 144 pass here. **The 11 that fail are the worktree's environment, not the batch** -- none of those commands runs
  Python at all (they are all in `NATIVE`), and the same harness with the main checkout's pre-change exe fails 5 of 155 in this worktree: `dos-build`, `driver-build`, `facts-binary`, `facts-make`, `settings` (their goldens are Logan's machine: relative `settings.json`
  paths, `state` sizes). With the corpus found (`CB_CORPUS_ROOT`; the native corpus looks for `Windows95` beside the checkout root, which is three levels up from a worktree) and `decompiler.exe` copied in from the main checkout, this tree's exe
  fails those same five plus `tidy-dry`, `tidy-stray` (golden "Converted/*/build/ (5): 270.8 MB"), `scoreboard`, `module-pe` (a hash of text the local decompiler made), `vxd-masm` (symbolic=500 in the golden, 0 here: the driver kit includes) and
  `xenon-recompile-missing` (builds XenonRecomp first): state and tools a worktree does not have. They need to be run on master after the merge together with condition 5.
- Environment notes for the integrator: a gate in a worktree needs `settings.json` with absolute paths, `CB_CORPUS_ROOT`, `native/decompiler/build/bin` and the other `native/*/build` libraries (`tools\build_native.cmd`), `native/xex` built, and
  `state\cxx\cb\bin\cbtrace.dll` (`build_native.cmd cb`); a first run without the libraries fails ~30 suites with "formats.dll is required: not built".

### What is left, and what blocks it (the pool after batch 3: 179 modules / 54,888 lines unreached by the UI, `__main__` and the tools; every one of them is held by a Python test)

| group | modules | lines | blocked by |
|---|---|---|---|
| one connected tangle: `recompile/{powerpc,builders/*,config,discover,disassemble,jumptable,switches,xenon}`, `source/*_writer`, `build/{*_asm,msvc152,period,mzfile,pklite,stubjoin,vxdfile}`, `decompile/{m68k,sm83,z80,vmx128}`, `emulate/{m68k,sm83,z80,driver,native_driver}`, `formats/{gb,genesis,msx,sms,zx}`, the `platforms/*` registry and `verify/lanes/*` (loaded by name), `verify/authored/*` and `verify/gate16/*` (the boot closure) | 93 | 27,498 | 21 test files with no native mapping yet (`test_port_fixes, test_recompile, test_x360, test_verification, test_verdicts, test_run_record, test_vmx128, test_z80, test_module_model, test_c_backend, test_authored_rewrites, test_pytypes, test_python_frontend ...`); the registry and lanes are `importlib`-named, so they can go only together; `verify.authored*` and `verify.gate16.*` are in the boot closure (condition 5 needs the real boot on a tree without them) |
| `formats/xbox/*` (24), `formats/{xnb,gfx,inspect,media}`, `decompile/{xbox_effect,xenos_hlsl}`, `xna/*`, `platforms.xbox` | 36 | 9,603 | **native gaps**: `formats.xnb`, `formats.xbox.{xma,xsb,xwb}` have no native counterpart; tests `test_xna, test_xbox_effect, test_xbox_formats, test_xbe` unmapped (the other formats have natives under `native/cb/formats/xbox/`) |
| `java/projects/minecraft/*` (+ profiles), `java.console.xdk_scan`, `translate.rewrite` | 23 | 3,458 | loaded by name (`java.projects.load`, `java.profiles.get`) from `java.transpile`, which 23 tests/java files use through a profile name; the natives exist (`native/cb/java/projects/minecraft/*`) and tests/java is mapped, but cutting those files one by one has to go with `java.transpile` and `java.profiles` (the whole Python java front end) |
| C# kit: `translate/{backends/csharp,backends/csharp_java,dotnet/*,frontends/csharp/*}` | 8 | 4,828 | **native gap**: the Roslyn driver of `CsTreeDump.cs` (`frontends/csharp/__init__.py`) and the judged pairs (`dotnet/pairs.py`) have no native counterpart (native has only `lower.cpp` over dumps); `test_csharp_kit, test_csharp_lower_trees, test_dotnet_toolchain` |
| clang front end + Java back end: `translate/{backends/java*,frontends/clang/*,judge_native_jvm}` | 8 | 5,680 | `test_clang_frontend`, `test_java_backend` unmapped; `clang.j2x` and `clang.ctypes` have no same-name native; also held by `tools/judge_c_java.py` (kept, see (2)) |
| `translate/{backends/c_java,backends/jmodel,javajudge}` | 3 | 2,610 | `test_cpp17_backend` tests the live cpp17 back end beside jmodel (cases must be cut one by one); `test_java_as_c` |
| `exact/{emu/isis,emu/xenix,plm86,xenix_c,mspascal,hooks_lattice}` | 6 | 1,150 | named as `"plugin"` in `data/exact/toolchains/*.json` and imported by `exact.toolchain`, which `tools/cb_shim/sample_shapes_cases.py` keeps alive through `exact.draft`; no test of their own; native twins `native/cb/exact/*` exist. Free once `tools/cb_shim/sample_*_cases.py` are retired |

The tools that still hold modules alive are listed in section (1); retiring `tools/cb_shim/sample_*_cases.py` and the other un-called dev scripts would free `exact.*`, the
`verify.gate16`/`verify.reconstruct` Python and the decompile reports from the tool side, so they would then only be held by tests.
