# Python reach inventory: who still reaches each codebreaker/ module

Made 2026-11-13, re-run after batch 3 (every command but `ui` native; batches 1, 2 and 3 deleted; the totals and rows below are the tree after batch 3). Every row is
computed (docs/history/python-port/python_reach/graph.py and report.py: AST import graph of every .py, lazy imports and dotted-name
strings included, plus tests/, tools/ and native/ generators); the batch-1 verdicts at the end were checked by hand; the batch-2 section corrects the batch-1 estimate of the deletable pool.

320 modules / 109750 lines under codebreaker/ (ui is listed too, it is the kept terminal).

## How to read it

**Reached by** (what keeps a module alive today):
- `live cmds`: reachable (static imports, lazy imports, dotted-name strings, the platforms registry) from the module of one of the 8
  commands that still run Python in the exe: names knowledge coverage progress facts origin driver-services boot-floppy. `all N` means more than three of them.
- `UI`: reachable from `ui/terminal.py` or `ui/client.py` (the thin client: stdlib + settings + verdicts only).
- `platforms registry`: loaded by name from `codebreaker/platforms/*` (ORDER tuple, `Ref("module", "name")`, `machine(...)` tuples).
- `tests: x,y`: imported by `tests/test_x.py` (gate suites or pytest folders). `tools: x`: imported by `tools/x.py`.
  `native gens: n`: imported by n golden generators under native/cb/**/testdata (the goldens they made are checked in).
- `named in native text only`: a comment or test title in native/ or a data file mentions the dotted name; no Python caller.
- The translated exe (pyengine --dynamic over codebreaker/, tests/, tools/) reaches exactly what those reach; the only
  `native/typed/*.json` manifest is `codebreaker.compare.image` (live: `compare` is imported by 10 packages). The shims under
  `native/cb/compat/` convert types and call the new native functions; none calls translated Python.

**Conditions** (Logan's five; `feedback-delete-python-only-when-proven`), shown as five letters per module:
1. native C++ for it is committed: Y when a file of the same name exists in native/cb at the same path (heuristic; N = none found).
2. its own tests pass as C++: Y when a `<name>*_test.cpp` sits beside the native file (existence only; batch 1 was run).
3. judge traces agree: G when native tests compare against goldens the Python made (a generator under testdata imports the
   module); `-` when no such evidence is recorded. Per-body judge traces exist only for judged modules; for the rest the
   Python-made golden IS the agreement evidence.
4. every caller runs through codebreaker.exe: N when a live command, the UI, the platforms registry or a tool still imports it.
5. the all-15 boot passes with the exe: N when the module is in the Python closure of the boot drivers (verify/gate16/*,
   source/module_writer); Y means the boot never reaches it, so it cannot change the boot. There is no committed native driver for
   the real boot run, so for N rows this condition cannot be shown until step 6 of docs/PYTHON_RETIREMENT.md.

A module is deletable only with `YYGYY`, plus the by-hand checks in the batch section. The letters are heuristics: rows were computed on the
tree before the batch-1 deletion; 9 rows read YYGYY (the port and crypto ones, and some translate ones) and the translate ones fail the by-hand
check (their pytest files in tests/java and tests/translate have no confirmed native fate; `G` there comes only from a golden generator importing them).


## Batch 1: what was deleted and why those two packages

Deleted in the deletion commit: **49 modules, 18,639 lines** (and 9 test/generator files, see below), leaving 504 modules and 150,888 lines (169,527 before).

| package | modules | lines | native replacement |
|---|---|---|---|
| `codebreaker/port` | 9 (`__init__`, `__main__`, arith16, asm16, asmdata, oldlayout, pre_ansi, sanitize, verify) | 10,494 | `native/cb/port/` (arith16, asm16*, asmdata, oldlayout, old_c_*, sanitize*, tool_*, mend*, csource ...; tests: `cb_tests_port`) |
| `codebreaker/formats/xbox/crypto` | 40 (XeCrypt, MemCrypto, StreamIO, 37 tool modules and `__init__`) | 8,145 | `native/cb/formats/xbox/crypto/` (xecrypt*, mem_crypto, stream_io, tools.cpp dispatch, one .cpp per tool); the `crypto` command is in `NATIVE`, so `codebreaker.exe crypto` never ran these |

Kept on purpose: `codebreaker/port/{compilers.json,platform/,runtimes/}` (data native/cb/port reads at run time: docs/PYTHON_RETIREMENT.md 4.4)
and `codebreaker/formats/xbox/crypto/{LICENSE,ATTRIBUTION.md}` (the BSD licence of the vendored upstream code).

### The five conditions, shown for both packages

1. **Its C++ is committed as source.** Yes: the idiomatic native/cb code above, and the older translation in `native/typed/cpp/codebreaker/{port,formats/xbox/crypto}`
   (checked by `cb_shim_check`, which is untouched).
2. **Its own tests and every suite that exercises it pass as C++.** Run in this worktree on the merged tree:
   `tools\build_cb.cmd area port` -> `cb_tests_port` **278 passed, 0 failed (19,413 checks)**;
   `tools\build_cb.cmd area formats` -> `cb_tests_formats --filter formats.xbox.crypto` **123 passed, 0 failed (3,479 checks)**.
   The Python tests that exercised them and what covers each (they were deleted or cut):
   - `tests/test_port.py` (whole file, 11 cases): covered by `port.*` tests: widening passes, octal constants, size tables (`old_c_passes_test`,
     `old_c_data_test`), frames and formats (`layout_test`, `program_test`), builds-and-runs (`old_c_pipeline_test`, `real_program_test`).
   - `tests/test_xbox_crypto.py` (whole file, 7 cases): hashes/ciphers/keyvault vectors -> `xecrypt_test` (hashes_and_macs, rc4, aes_and_des,
     keyvaults_as_the_python_sealed_them); stream I/O -> `stream_io_test`; the `crypto` command -> `tools_test`; xcp_dumper wrong key ->
     `xcp_dumper_test`; exp_signer HXPC -> `tool_logic_test`.
   - `tests/test_port_fixes.py`: five cases cut (cpukey validity, sd_signer key kept, StreamIO offset/labels, apply_jump_sd_4bl, xsm3 challenge verify):
     -> `tool_goldens_test`/`tool_logic_test`, `sd_signer_test`, `stream_io_test.labels_and_functions_at_an_offset`,
     `build_lib_test.the_4bl_jump_branches_to_the_end_of_the_image`, `tool_logic_test`. The other 13 cases of that file stay.
   - `tests/translate/test_module_model.py`: `test_the_port_reads_its_module_tree` cut (-> `port.asm16_test`); the file's other cases stay.
   - `tests/run_all.py` SUITES: the `test_port.py` and `test_xbox_crypto.py` rows removed.
   - Seven golden generators under native/cb testdata imported the deleted modules (`gen_golden.py`, `gen_listings.py`, `gen_oldlayout_golden.py`,
     `make_cases.py`, `make_views_cases.py`, `make_tools_goldens.py`, `make_tools_fn_goldens.py`) and were deleted; the goldens they made are
     checked in and are what the native tests above compare against (git history keeps the generators). `make_xcp_samples.py` stays: it drives
     `codebreaker.exe crypto`, which is native.
3. **The judge traces agree, no unexplained differences.** These two packages are not judged code (no per-body verdicts); the equivalent
   agreement evidence is the Python-made goldens above: the port's asm16 oracle, old-C golden cases, mend golden, oldlayout golden, and the crypto
   tool goldens (`tools_goldens.txt`, `tools_fn_goldens.txt`, `xcp_goldens.txt`) compared in `cb_tests_port` / `cb_tests_formats`. The
   differences the ports fixed on purpose are listed in state/work/typed-port/_bugs/LIST.md.
4. **Every caller runs through codebreaker.exe.** No live command, the UI, the platforms registry, `tools/*.py` or the exe's shims reach them
   (import graph, row `reached by`). `codebreaker/__main__.py` still names `codebreaker.formats.xbox.crypto` in `COMMANDS["crypto"]`, but `crypto` is in
   `NATIVE`, so the exe never imports it (the line belongs to the plain-python fallback that does not exist for native commands; it goes with
   `__main__`, which this batch does not touch). Outside callers: grep of Dashboard, Windows95, Win95VM and XboxMCP finds no `codebreaker.port` or
   `xbox.crypto` use (only a scratch copy of CodeBreaker inside Dashboard's `_scratchpad-rescue`, not a caller).
5. **The all-15 boot passes with the exe.** Not re-run: there is no committed native driver for the real boot run (docs/PYTHON_RETIREMENT.md step 6) and
   the rule is milestone-only regressions. It holds by non-reach: neither package is in the Python closure of `verify/gate16/*` or `source/module_writer`
   (report column 5 = Y for every deleted row), and nothing the boot imports changed.

### What was verified after the deletion

- `tools\build_codebreaker_exe.cmd fast` rebuilds: pyengine translated 994 modules (372,016 lines) and the exe linked. (This worktree's compiler needed
  `_CL_=/wd4702`: `/W4 /WX` turns C4702 "unreachable code" in `native/cb/translate/frontends/python/lower_fn.cpp:91` and `native/cb/source/vxd/owbuild.cpp:224`
  into errors under /O2; unrelated to this batch, left alone, worth fixing in native/cb.)
- The old gate `codebreaker.exe tests/run_all.py --quick --jobs 2 --no-build` on the rebuilt exe: 49 suites in 857 s. `check_imports`: 504 modules
  import, 0 dangling imports; `tidy` (pyflakes) ok; 45 suites ok (test_translate 186 passed, test_port_fixes 12 passed, test_xenos, test_z80, test_m68k,
  test_nes, test_platforms, test_xex, test_mcp, ... all ok). Four suites FAILED, none of them touching a deleted module (their test files import
  neither package; no `dangling` or ImportError anywhere): `test_recompile` and `test_ppc_calls` (the worktree had no native/xex build: "the xex program is
  not built"); `test_java` (2 cases in tests/java/test_pc_platform.py: a translated Java program aborts with an UnsupportedOperationException); `test_service`
  (`test_unavailable_says_why`, `test_started_on_demand`: the thin client from the UI branch finds a codebreaker service that is not this one, other agents'
  services run on this machine). No pre-deletion baseline was run (a second 15-minute exe build and a 14-minute gate for a comparison nobody asked
  for); the integrator's merge check is the baseline.

### What the inventory says about the rest (after batch 1: 504 modules, 150,888 lines)

`codebreaker/__main__.py` names every command's module as a string, and `ui/service.py` and `ui/mcp_server.py` import `__main__`, so a plain
import graph makes everything "live through facts/progress/coverage". The tables below cut that edge for the NATIVE commands (the exe never imports their
Python); the cut is in report.py, `__main__` itself is untouched.

- **288 modules** are reached by a live command, the thin UI, the platforms registry or a `tools/*.py`: they stay.
- **216 modules / 64,874 lines** are reached by none of those. Of these, **107 (36,064 lines) are reached by tests only** (deletable only with
  those tests, each needing a fate row: translate 15 tests, java 19, source 13, verify 14, invert 13, exact 8, ...) and **109 (28,810 lines) by nothing
  but each other** (translate 25 modules / 18,726 lines in all, exact 41 / 11,579, source 30 / 9,304, recompile 15 / 7,203, java 33 / 6,960, verify 18 /
  2,700, invert 28 / 2,496, xna 5 / 1,357, decompile 3, formats 6, analyze 4, symbols 3, build 2, base 2, ui 1: the Python `ui/serve.py` server half).
  These are the candidate pool for batch 2; each still needs its native counterpart and tests confirmed by hand (the letters in the tables are
  filename heuristics), and 7 of them sit in the boot closure, where condition 5 cannot be shown yet.
- Why batch 1 stopped at two packages: they were the ones where every condition could be shown with evidence in hand (a native test tree that passed and
  goldens made from the Python, no importer anywhere, tests with named native coverage). The translator packages need their pytest folders' fate rows
  (docs/PYTHON_RETIREMENT.md 5.2) and `tools/py2cpp.py`/`judge_c_java.py` still import part of them; `exact`/`recompile`/`java` are loaded by name through
  importlib tables and toolchain JSON (`exact/toolchain.py`, `exact/bench.py`, `engine/engine.py`), which the import graph can only see as strings.

## Batch 2: 44 modules, 9,360 lines (and what the first estimate of the pool got wrong)

Deleted in the two batch-2 deletion commits: **44 modules, 9,360 lines** (`wc -l`), leaving 460 modules and 141,557 lines (504 / 150,888 after
batch 1). Eight commands' Python are gone from `COMMANDS` (`exact analyze serve measure sweep classes system-ui java`; all eight are in `NATIVE`, so
`codebreaker.exe` never ran their Python; the rows belonged to the plain-python fallback that does not exist for native commands).

| package | modules | lines | native replacement | tests removed (and their native fate) |
|---|---|---|---|---|
| `decompile/xenos` | 1 | 131 | `native/cb/decompile/xenos.*` | `tests/test_xenos.py` (4 checks) -> `decompile.xenos.sample_shader` (the same fxc listing lines) + 6 more `decompile.xenos.*` |
| `exact` (bench bss exepack habits/{__init__,boolx,cmpp,newcomp,oldcomp,symx} identify ledger link linkemu linkwhole names pascal_join project readable recut repair run seed standin) | 23 | 4,047 | `native/cb/exact/*` (one file each) | `tests/test_cmpp.py` -> `exact.habits.cmpp.*` (`walk_by_hand`, the `tricky` entry-data case in `what_is_not_compared`); `tests/test_recut.py` (4 cases) -> `exact.recut.cuts_where_the_relocations_run_up_again`, `left_alone_when_the_relocations_are_not_the_programs`, `relocation_order`, `exact.standin.a_fixup_location_is_four_bits`; `tests/test_exact_fixes.py`: the standin case cut (-> `exact.standin.a_fixup_location_is_four_bits`), the aim case stays |
| `java` (analyze run scaffold signatures, console/{assets platform_config project_layout project_scan vcxproj worklist xact}) | 11 | 3,616 | `native/cb/java/*`, `native/cb/java/console/*` | tests/java: `test_assets.py` (11) -> `java.console.assets.*` + `java.console.xact.*`; `test_platform_config.py` (12) -> `platform_config_test` (15); `test_project_layout.py` (8) -> `project_layout_test` + `java.console.ports.*`; `test_project_scan.py` (4) -> `project_scan_test` (5); `test_scaffold.py` (5) -> `scaffold_test` (7); `test_vcxproj_filters.py` (4) -> `vcxproj_test` (10); `test_review_fixes.py::test_applet_ping_cli_is_registered` cut (-> `java.minecraft.applet_ping.*`, `run_test`); `test_profiles_selection.py`: the vcxproj/signatures assertions cut (-> `vcxproj_test`, `signatures_test`), the typemap/rules/pipeline reseed cases stay |
| `analyze` (facts observe report resources) | 4 | 765 | `native/cb/analyze/{facts,observe,report,resources}.*` | `tests/test_analysis.py`: `test_observation_arithmetic`, `test_report_and_fact_table` and the resources lines of the dispatch case cut (-> 46 `analyze.*` results in `cb_tests_analyze`, goldens made by the Python), the other 10 cases stay |
| singles: `formats/xbox/system_ui`, `ui/serve`, `symbols/cpp_classes`, `verify/measure`, `verify/sweep` | 5 | 801 | `native/cb/formats/xbox/system_ui`, `native/cb/ui/serve`, `symbols/cpp_classes`, `verify/measure`, `verify/sweep` | none imported them (commands only) |

Also removed: seven golden generators under native/cb testdata that imported the deleted modules (`exact/testdata/gen_w5b3.py`, `gen_seed.py`,
`java/console/testdata/gen_golden.py`, `gen_config_golden.py`, `analyze/testdata/gen_observe.py`, `gen_goldens.py`; git history keeps them; the goldens they
made are checked in and are what the native tests compare against), `tests/run_all.py` SUITES rows `test_xenos.py`, `test_recut.py`, `test_cmpp.py`,
and two README lines. `native/typed/cpp/codebreaker/**` (the older shims, `cb_shim_check`) is untouched, as in batch 1.

### The five conditions

1. **C++ committed**: the files above (`native/cb/**`), checked by hand module by module, not by the name heuristic.
2. **Tests pass as C++**, run in this worktree on the final tree (`tools\build_cb.cmd area <a>`): exact 446, java 590, analyze 218, symbols 225,
   verify 302, ui 49, formats 604, decompile 189 passed, 0 failed (the translate area, 478 passed, was run for the C# kit below).
3. **Agreement with the Python**: the native tests for these modules compare against goldens the Python made (`matches_the_original`,
   `the_pythons_listings`, `real_objects_as_the_python_recut_them`, `sound_banks_match_the_original` ...); no per-body judge verdicts exist for them.
4. **Every caller through the exe**: the module is in no live closure (below); no tool, platform, test or COMMANDS row reaches it any more; Dashboard,
   Windows95, Win95VM and XboxMCP have no hit for `codebreaker.exact|analyze|java.console` and the like.
5. **All-15 boot**: none of the 44 modules is in the Python closure of `verify/gate16/*` / `source/module_writer`, and `regress` is a native command (it
   runs `native/cb`, never these Python modules), so the deletion cannot change it. **Run, and it did not pass here: `codebreaker regress all15 --runs 2`
   -> FAIL in 14m40s** (also FAIL in 23m33s on the first, loaded attempt): the 15 modules build and link (KRNL386 679 functions / 18 C, GDI 883 / 114,
   USER 1613 / 409, ...), but both boots end `SLOW 5m30s` with the screen at "Copying files needed for Windows Setup...", never reaching the License page, and
   the note says USER C 409 / gated 176 differs from the milestone's 509 / 61. I did not separate environment from cause: this worktree has no state, so
   `state/mini`, `state/bootfloppy`, `state/build`, `state/cache`, `native/decompiler/build/bin/decompiler.exe` and a settings.json (made absolute: the
   relative `../Windows95` paths resolve three levels too deep from a worktree) were copied in from the main checkout, and other agents' builds and QEMU VMs
   were running. The same result on the unchanged native code would show it is not the batch; I could not run main's regress without writing into main's state.
   Condition 5 is therefore "holds by non-reach", not "shown by a passing boot".

### What was verified after the deletion

- `toolsuild_codebreaker_exe.cmd fast` rebuilds (incremental from batch 1; the first build in this worktree needed `_CL_=/wd4702`, as in batch 1).
- `codebreaker.exe tools/check_imports.py`: 460 modules import, 0 dangling imports; `tidy` ok.
- The old gate on the rebuilt exe, `tests/run_all.py --quick --jobs 2 --no-build` (CB_EXE = this worktree's build-fast exe, settings.json absolute,
  `native/xex/build` absent as in an integrator checkout): **ALL SUITES PASSED, 46 suites in 1759 s** (test_java 374 passed, test_translate, test_readable 4 passed,
  test_analysis, test_exact_fixes, test_ppc_calls, test_recompile, test_platforms, test_service, test_mcp, ...). Two environment notes for the next person:
  with `native/xex` built, `test_recompile.py` runs `test_whole_image` for 15 minutes without finishing (it is what ran in an earlier gate of this
  session, killed at 902 s); without a settings.json the OpenWatcom suites (test_readable, ...) report 4 skipped, with one made in a worktree
  they fail until its relative paths are made absolute.

### The pool estimate of the batch-1 section was wrong (twice)

The batch-1 tables called a module "reached by nothing live" when no live command, the UI, the platforms registry or a `tools/*.py` imported it
DIRECTLY, and then counted 109 modules / 28,810 lines "reached only by each other". Two errors, both fixed in report.py:

1. The `tools` column was the direct importers only. `report.py` now closes over the imports (a module a live module imports is live): roots are the
   modules of the 8 commands that still run Python (names knowledge coverage progress facts origin driver-services boot-floppy), the thin UI, `platforms/*`,
   and every module a `tools/*.py` imports (`tools/py2cpp.py` alone keeps 7 modules / 6,076 lines alive and `tools/judge_c_java.py` 8 / 5,680; both are
   older than pyengine and are the natural next deletions of the tools themselves).
2. Edges the AST graph cannot see were treated as dead. Three were found by reading the loaders and are now roots (`_dyn` in report.py):
   `platforms/__init__.py:310` names a lane as `"codebreaker.verify.lanes." + name` (the gb/genesis/msx/nes/sms/zx/z80_flat lanes and `image_lane` are live);
   `exact/toolchain.py` loads `"plugin": "module:Class"` and `"call": "module:function"` from `data/exact/toolchains/*.json` (`exact.{plm86,mspascal,xenix_c,hooks_lattice}`
   and what they import: `exact.emu.{isis,xenix}`); `java/projects/__init__.load()` and `java/profiles.get()` import `codebreaker.java.projects.<name>` and its
   `profiles.*` by manifest name (a dozen kept tests/java files use them through a profile name).

After both: **118 modules / 34,876 lines are unreached** (the list at the end of this file), not 109 / 28,810. Of those, this batch took the 43 whose
every condition holds and whose tests could be given a native fate.

### What is left unreached and why it was not taken (118 modules)

- **The big component (86 modules, 22,597 lines: source/ne_asm, source/vxd, invert, recompile, verify/authored, build/period ...)** is one connected
  tangle through `tests/test_verification.py`, `test_drivers.py`, `test_invert.py`, `test_recompile.py`, `test_x360.py`, `test_port_fixes.py`. Six of
  its modules (`verify.authored*`, `verify.gate16.{runtime16,scoreboard16}`) are in the boot closure (condition 5 needs the real boot run on a tree without
  them), 19 have no native counterpart or no native test (`invert.msc6_*`, `invert.run`, `invert.compilers`, `build.msvc152`, `build.period`, ...), and
  `test_recompile.py` needs `native/xex` built (it is also the suite that fails in this environment, see the gate result).
- **The C# kit (8 modules, 4,828 lines)** fails condition 1: `translate/frontends/csharp/__init__.py` (the Roslyn driver that compiles `CsTreeDump.cs`
  with the SDK's csc.dll and runs it) and `translate/dotnet/pairs.py` (the judged language pairs) have no native counterpart; native/cb has only the
  lowering of the tool's JSON (`frontends/csharp/lower.cpp`, tested on checked-in dumps). Deleting the set would remove the only code that produces a
  tree dump from C# source. The native gap to close first: port the CsTreeDump driver and the pairs, with a real-Roslyn test.
- **c_java / jmodel / javajudge (3 modules, 2,610 lines)**: `test_cpp17_backend.py` tests the live cpp17 back end beside jmodel, so its cases need cutting one by one.
- **xna / xbox formats (13 modules, 3,575 lines)**: `formats.xnb`, `formats.xbox.{xma,xsb}` have no native counterpart found.
- **java/projects/minecraft/profiles/*, 7 modules** have no native test of their own (they are data tables, covered through `java.minecraft.package`).
- Single modules with no native test beside them: `source.dos_build`, `invert.msvc2_registers`, `build.{disc_image,period16,ppc_asm}`, `formats.xex_edit` (these
  have native code but their tests are by name only), left until a hand check shows the native test.

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

### Unreached after batch 3 (computed: not in the closure of the UI, __main__ or a tool still used; tests are NOT roots here)

- `build.m6502_asm` (147 lines)
- `build.msvc152` (79 lines)
- `build.mzfile` (50 lines)
- `build.period` (250 lines)
- `build.pklite` (380 lines)
- `build.stubjoin` (122 lines)
- `build.vxdfile` (63 lines)
- `decompile.vmx128` (374 lines)
- `engine.win16` (568 lines)
- `platforms.dos` (33 lines)
- `platforms.java` (25 lines)
- `platforms.nes` (19 lines)
- `platforms.vxd` (43 lines)
- `platforms.win16` (72 lines)
- `platforms.win32` (58 lines)
- `platforms.xbox360` (45 lines)
- `recompile` (17 lines)
- `recompile.builders` (0 lines)
- `recompile.builders.arith` (269 lines)
- `recompile.builders.floating` (172 lines)
- `recompile.builders.logical` (286 lines)
- `recompile.builders.memory` (492 lines)
- `recompile.builders.system` (384 lines)
- `recompile.builders.vector` (989 lines)
- `recompile.config` (234 lines)
- `recompile.disassemble` (158 lines)
- `recompile.discover` (181 lines)
- `recompile.jumptable` (364 lines)
- `recompile.powerpc` (1168 lines)
- `recompile.switches` (553 lines)
- `recompile.xenon` (243 lines)
- `source.dos_writer` (1005 lines)
- `source.driver_writer` (625 lines)
- `source.image_writer` (345 lines)
- `source.le_writer` (518 lines)
- `source.ne_asm` (0 lines)
- `source.ne_asm.fixups` (133 lines)
- `source.ne_asm.segdiff` (217 lines)
- `source.ne_asm.segment` (703 lines)
- `source.ne_writer` (458 lines)
- `source.nes_writer` (160 lines)
- `source.pe_writer` (529 lines)
- `translate.backends.c_java` (1792 lines)
- `translate.backends.csharp` (1566 lines)
- `translate.backends.csharp_java` (631 lines)
- `translate.backends.jmodel` (511 lines)
- `translate.dotnet` (149 lines)
- `translate.dotnet.cs2cpp` (844 lines)
- `translate.dotnet.judge` (149 lines)
- `translate.dotnet.pairs` (114 lines)
- `translate.frontends.csharp` (142 lines)
- `translate.frontends.csharp.lower` (1233 lines)
- `translate.frontends.python` (319 lines)
- `translate.frontends.python.infer` (1955 lines)
- `translate.frontends.python.lower` (1816 lines)
- `translate.frontends.python.methods` (749 lines)
- `translate.frontends.python.program` (874 lines)
- `translate.frontends.python.pytypes` (308 lines)
- `translate.frontends.python.structfmt` (55 lines)
- `translate.javajudge` (307 lines)
- `verify.authored` (2 lines)
- `verify.authored.match` (674 lines)
- `verify.authored.match16` (281 lines)
- `verify.authored.repair` (587 lines)
- `verify.authored.search16` (212 lines)
- `verify.authored.shape` (210 lines)
- `verify.gate16.runtime16` (62 lines)
- `verify.gate16.scoreboard16` (203 lines)

Kept alive by `tools/*.py` alone (208 modules, 73222 lines): `analyze`, `analyze.call_graph`, `analyze.classes`, `analyze.conventions`, `analyze.dispatch`, `analyze.dos_functions`, `analyze.globals`, `analyze.image`, `analyze.native`, `analyze.origin`, `analyze.strings`, `analyze.structures`, `base.cache`, `base.converted`, `base.ctext`, `base.parallel`, `base.runs`, `build`, `build.asm16`, `build.data`, `build.definitions`, `build.dos_sessions`, `build.dos_tools`, `build.driver`, `build.exepack`, `build.flat_thunk`, `build.functions`, `build.masm`, `build.module`, `build.msc6`, `build.native_dos`, `build.resources`, `build.watcom`, `compare`, `compare.mask`, `decompile`, `decompile.asm_blocks`, `decompile.c_text`, `decompile.ctree`, `decompile.decompiler`, `decompile.describe`, `decompile.frame16`, `decompile.inline_asm`, `decompile.jar`, `decompile.m6502`, `decompile.shape16`, `decompile.struct_text`, `emulate`, `emulate.emulator`, `emulate.fast_uc`, `emulate.m6502`, `emulate.native_shadow`, `emulate.native_trace`, `emulate.native_win16`, `emulate.pcode`, `emulate.ports16`, `emulate.powerpc`, `emulate.run`, `emulate.shadow`, `emulate.symbolic`, `emulate.windows16`, `engine`, `engine.engine`, `engine.facts`, `engine.lanes`, `engine.native_engine`, `exact`, `exact.aim`, `exact.aout`, `exact.batch`, `exact.carve`, `exact.compare`, `exact.data`, `exact.draft`, `exact.draft.candidates`, `exact.draft.forms`, `exact.draft.prove`, `exact.draft.shapes`, `exact.draft.text`, `exact.emu`, `exact.emu.dos`, `exact.emu.dpmi`, `exact.emu.native_dosexec`, `exact.library`, `exact.modules`, `exact.toolchain`, `exact.x86`, `formats`, `formats.cab`, `formats.detect`, `formats.dos`, `formats.le`, `formats.native`, `formats.ne`, `formats.nes`, `formats.omf`, `formats.pe`, `formats.pe_functions`, `formats.powerpc_image`, `formats.resources`, `formats.xex`, `invert`, `invert.flow16`, `invert.native_invert`, `java`, `java._paths`, `java.apimap`, `java.bugfix_ledger`, `java.cmake`, `java.console`, `java.console.platform_equivalents`, `java.console.platform_refs`, `java.console.sdk`, `java.debug`, `java.emit`, `java.emit.header`, `java.emit.naming`, `java.emit.ownership`, `java.emit.source`, `java.emit.std_transforms`, `java.ir`, `java.ledger`, `java.overlays`, `java.parse`, `java.platforms`, `java.probe`, `java.profiles`, `java.projects`, `java.reflection_factory`, `java.report`, `java.rules`, `java.runtime`, `java.stubs`, `java.textutil`, `java.transpile`, `java.twophase`, `java.typemap`, `java.types`, `jobs`, `jobs.pool`, `knowledge`, `knowledge.reads`, `knowledge.sources`, `knowledge.store`, `platforms`, `source`, `source.assembly`, `source.classes_writer`, `source.java_writer`, `source.module_writer`, `source.placement`, `source.readable`, `store`, `store.functions`, `symbols`, `symbols.api_arguments`, `symbols.demangle`, `symbols.dos_interrupts`, `symbols.driver_services`, `symbols.libraries`, `symbols.libraries.coff`, `symbols.libraries.omf`, `symbols.names`, `symbols.native`, `symbols.ordinals`, `symbols.prototypes`, `symbols.runtime_library`, `symbols.sdk_structs`, `symbols.wine16`, `symbols.xdk_libraries`, `translate`, `translate.backends`, `translate.backends.asm`, `translate.backends.c`, `translate.backends.cpp03`, `translate.backends.cpp17`, `translate.backends.java`, `translate.backends.java_libc`, `translate.backends.java_objects`, `translate.backends.module_file`, `translate.desugar`, `translate.frontends`, `translate.frontends.clang`, `translate.frontends.clang.ctypes`, `translate.frontends.clang.j2x`, `translate.frontends.clang.lower`, `translate.frontends.java`, `translate.frontends.java.lower`, `translate.ir`, `translate.judge_native_jvm`, `translate.l2`, `translate.native_l2`, `verify`, `verify.compare`, `verify.concolic`, `verify.gate16`, `verify.gate16.contract16`, `verify.gate16.isolated16`, `verify.gate16.setup16`, `verify.guided`, `verify.lane_common`, `verify.lanes`, `verify.lanes.java`, `verify.lanes.win16`, `verify.lanes.win32`, `verify.reconstruct`, `verify.teeth`, `verify.verdicts`

## Totals by package

| package | modules | lines | reached by a live cmd | UI | tests only | tools too | nothing/native-text only | YYGYY |
|---|---|---|---|---|---|---|---|---|
| (root) | 1 | 23 | 1 | 1 | 0 | 0 | 0 | 0 |
| codebreaker.__main__ | 1 | 186 | 0 | 0 | 0 | 1 | 0 | 0 |
| codebreaker.analyze | 12 | 3609 | 12 | 0 | 0 | 0 | 0 | 0 |
| codebreaker.base | 8 | 1758 | 7 | 3 | 0 | 1 | 0 | 0 |
| codebreaker.build | 23 | 9546 | 21 | 0 | 2 | 0 | 0 | 0 |
| codebreaker.compare | 2 | 90 | 2 | 0 | 0 | 0 | 0 | 0 |
| codebreaker.decompile | 13 | 6407 | 11 | 0 | 1 | 1 | 0 | 1 |
| codebreaker.emulate | 16 | 5672 | 16 | 0 | 0 | 0 | 0 | 0 |
| codebreaker.engine | 6 | 2526 | 6 | 0 | 0 | 0 | 0 | 0 |
| codebreaker.exact | 27 | 12172 | 6 | 0 | 4 | 3 | 14 | 0 |
| codebreaker.formats | 14 | 2780 | 13 | 0 | 1 | 0 | 0 | 0 |
| codebreaker.invert | 3 | 345 | 0 | 0 | 0 | 0 | 3 | 0 |
| codebreaker.java | 56 | 14659 | 34 | 0 | 10 | 0 | 12 | 4 |
| codebreaker.jobs | 2 | 287 | 2 | 0 | 0 | 0 | 0 | 0 |
| codebreaker.knowledge | 4 | 535 | 4 | 0 | 0 | 0 | 0 | 0 |
| codebreaker.platforms | 8 | 684 | 0 | 0 | 1 | 0 | 7 | 0 |
| codebreaker.recompile | 15 | 5510 | 0 | 0 | 7 | 0 | 8 | 14 |
| codebreaker.source | 18 | 6347 | 11 | 0 | 5 | 0 | 2 | 3 |
| codebreaker.store | 2 | 126 | 2 | 0 | 0 | 0 | 0 | 0 |
| codebreaker.symbols | 16 | 3353 | 16 | 0 | 0 | 0 | 0 | 0 |
| codebreaker.translate | 41 | 23730 | 14 | 0 | 16 | 1 | 10 | 4 |
| codebreaker.ui | 3 | 1277 | 0 | 3 | 0 | 0 | 0 | 0 |
| codebreaker.verify | 29 | 8128 | 18 | 0 | 9 | 1 | 1 | 0 |

## Every module

### (root)

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| (package) | 23 | UI; platforms registry; tests: n_subjects,ll,+114; tools: carve,causes,copy_walk; native gens: 25 | none found | NNGNN |

### codebreaker.__main__

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| __main__ | 186 | tools: carve,unreachable | none found | NN-NY |

### codebreaker.analyze

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| analyze | 23 | platforms registry; tests: analysis,c_backend,dos+1; native gens: 3 | cb/analyze/(analyze/ package) | YYGNN |
| analyze.call_graph | 110 | platforms registry; tests: analysis | cb/analyze/call_graph.cpp +1 | YY-NN |
| analyze.classes | 533 | platforms registry; tests: analysis | cb/analyze/classes.cpp +1 | YY-NN |
| analyze.conventions | 136 | platforms registry; tests: analysis; native gens: 1 | cb/analyze/conventions.cpp +1 | YYGNN |
| analyze.dispatch | 404 | platforms registry; tests: analysis | cb/analyze/dispatch.cpp +1 | YY-NN |
| analyze.dos_functions | 516 | platforms registry; tests: dos | cb/analyze/dos_functions.cpp +1 | YY-NN |
| analyze.globals | 105 | platforms registry; tests: analysis; native gens: 1 | cb/analyze/globals.cpp +1 | YYGNN |
| analyze.image | 524 | platforms registry; tests: analysis,c_backend; native gens: 2 | cb/analyze/image.cpp +1 | YYGNN |
| analyze.native | 157 | platforms registry | cb/analyze/native.cpp +1 | YY-NN |
| analyze.origin | 489 | platforms registry; tests: port_fixes | cb/analyze/origin.cpp +1 | YY-NN |
| analyze.strings | 191 | platforms registry; tests: analysis; native gens: 2 | cb/analyze/strings.cpp +1 | YYGNN |
| analyze.structures | 421 | platforms registry; tests: analysis,c_backend | cb/analyze/structures.cpp +1 | YY-NN |

### codebreaker.base

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| base | 0 | UI; platforms registry; tests: n_subjects,ll,analysis+21; tools: copy_walk,failures,freeze_tests; native gens: 7 | cb/base/(base/ package) | YYGNN |
| base.cache | 591 | platforms registry; tests: plumbing,verification | cb/base/cache.cpp +4 | YY-NN |
| base.converted | 188 | platforms registry; tests: converted | cb/base/converted.cpp +1 | YY-NN |
| base.corpus | 292 | UI; platforms registry; tests: n_subjects,analysis,concolic+16; tools: failures,freeze_tests,l2c; native gens: 2 | cb/base/corpus.cpp +3 | YYGNN |
| base.ctext | 51 | platforms registry | cb/base/ctext.cpp +1 | YY-NY |
| base.parallel | 268 | platforms registry; tests: ll,plumbing,verification | cb/base/parallel.cpp +1 | YY-NN |
| base.runs | 82 | tests: plumbing; tools: copy_walk,record | cb/base/runs.cpp +1 | YY-NN |
| base.settings | 286 | UI; platforms registry; tests: assembler,csharp_kit,dos+3; tools: copy_walk,golden,judge_c_java; native gens: 6 | cb/base/settings.cpp +1 | YYGNN |

### codebreaker.build

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| build | 0 | platforms registry; tests: ,c_backend,dos+16; tools: make_api_table,oss_bench,trace16 | cb/build/(build/ package) | YN-NN |
| build.asm16 | 689 | platforms registry; tests: verification | none found | NN-NN |
| build.data | 530 | platforms registry; tests: whole_module | none found | NN-NN |
| build.definitions | 208 | platforms registry; tests: whole_module | none found | NN-NN |
| build.dos_sessions | 185 | platforms registry | none found | NN-NN |
| build.dos_tools | 77 | platforms registry; tests: dos | none found | NN-NN |
| build.driver | 332 | platforms registry; tests: drivers | none found | NN-NN |
| build.exepack | 107 | platforms registry; tests: exepack | none found | NN-NN |
| build.flat_thunk | 1085 | platforms registry; tests: flat_thunk,whole_module | none found | NN-NN |
| build.functions | 678 | platforms registry; tests: engine,match_powerpc,platforms+4 | none found | NN-NN |
| build.m6502_asm | 147 | platforms registry; tests: nes | none found | NN-NY |
| build.masm | 442 | platforms registry; tests: dos | none found | NN-NN |
| build.module | 2162 | platforms registry; tests: c_backend,platforms,whole_module | none found | NN-NN |
| build.msc6 | 935 | platforms registry; tests: ,dos,verification | none found | NN-NN |
| build.msvc152 | 79 | tests: port_fixes | none found | NN-YY |
| build.mzfile | 50 | platforms registry | none found | NN-NY |
| build.native_dos | 207 | platforms registry; tests: dos | none found | NN-NN |
| build.period | 250 | tests: verification | none found | NN-YY |
| build.pklite | 380 | platforms registry; tests: pklite | none found | NN-NY |
| build.resources | 658 | platforms registry; tests: whole_module | none found | NN-NN |
| build.stubjoin | 122 | platforms registry | none found | NN-NY |
| build.vxdfile | 63 | platforms registry | none found | NN-NY |
| build.watcom | 160 | platforms registry; tests: ,plumbing,readable+3; tools: make_api_table,oss_bench,trace16 | none found | NN-NN |

### codebreaker.compare

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| compare | 0 | platforms registry; tests: symbols | cb/compare/(compare/ package) | YY-NN |
| compare.mask | 90 | platforms registry; tests: symbols | cb/compare/mask.cpp +1 | YY-NN |

### codebreaker.decompile

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| decompile | 0 | platforms registry; tests: n_subjects,asm_blocks,c_backend+11; tools: freeze_tests,golden,goto_report; native gens: 1 | cb/decompile/(decompile/ package) | YYGNN |
| decompile.asm_blocks | 267 | platforms registry; tests: asm_blocks,c_backend | cb/decompile/asm_blocks.cpp +1 | YY-NN |
| decompile.c_text | 1428 | platforms registry; tests: n_subjects,c_backend,frozen+1; tools: freeze_tests | cb/decompile/c_text.hpp +5 | YY-NN |
| decompile.ctree | 241 | tools: goto_report,oss_bench | cb/decompile/ctree.cpp +1 | YY-NY |
| decompile.decompiler | 898 | platforms registry; tests: c_backend,dos,frozen+5; tools: freeze_tests,golden,goto_report | cb/decompile/decompiler.cpp +5 | YY-NN |
| decompile.describe | 2178 | platforms registry; tests: n_subjects,dos,platforms+2; tools: golden,trace16 | cb/decompile/describe.cpp +6 | YY-NN |
| decompile.frame16 | 236 | platforms registry; tests: frame16 | cb/decompile/frame16.cpp +1 | YY-NN |
| decompile.inline_asm | 191 | platforms registry | cb/decompile/inline_asm.cpp +1 | YY-NN |
| decompile.jar | 188 | platforms registry | cb/decompile/jar.cpp +1 | YY-NN |
| decompile.m6502 | 181 | platforms registry; tests: nes; tools: sleigh_verify_6502 | cb/decompile/m6502.cpp +1 | YY-NN |
| decompile.shape16 | 111 | platforms registry; tests: frame16 | cb/decompile/shape16.cpp +1 | YY-NN |
| decompile.struct_text | 114 | platforms registry; tests: c_backend | cb/decompile/struct_text.cpp +1 | YY-NN |
| decompile.vmx128 | 374 | tests: vmx128; native gens: 1 | cb/decompile/vmx128.cpp +1 | YYGYY |

### codebreaker.emulate

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| emulate | 45 | platforms registry; tests: concolic,knowledge,native_trace+6; tools: gate_teeth16,sleigh_verify_6502,sleigh_verify_ppc; native gens: 2 | cb/emulate/(emulate/ package) | YYGNN |
| emulate.driver | 217 | platforms registry; tests: knowledge,native_trace,platforms; native gens: 1 | cb/emulate/driver.cpp +1 | YYGNY |
| emulate.emulator | 616 | platforms registry; tests: concolic,native_trace,platforms+1; native gens: 1 | cb/emulate/emulator.cpp +1 | YYGNN |
| emulate.fast_uc | 69 | platforms registry; tests: plumbing | none found | NN-NN |
| emulate.m6502 | 314 | platforms registry; tests: nes,platforms,run_record; tools: sleigh_verify_6502 | cb/emulate/m6502.cpp +1 | YY-NY |
| emulate.native_driver | 204 | platforms registry | cb/emulate/native_driver.cpp +1 | YY-NY |
| emulate.native_shadow | 239 | platforms registry | cb/emulate/native_shadow.cpp +1 | YY-NN |
| emulate.native_trace | 390 | platforms registry; tests: native_trace | cb/emulate/native_trace.cpp +1 | YY-NN |
| emulate.native_win16 | 209 | platforms registry | cb/emulate/native_win16.cpp +1 | YY-NN |
| emulate.pcode | 147 | platforms registry; tests: concolic,native_trace | cb/emulate/pcode.cpp +1 | YY-NN |
| emulate.ports16 | 195 | platforms registry; tests: verification | cb/emulate/ports16.cpp +1 | YY-NN |
| emulate.powerpc | 916 | platforms registry; tests: concolic,platforms,powerpc; tools: sleigh_verify_ppc | cb/emulate/powerpc.cpp +1 | YY-NN |
| emulate.run | 76 | platforms registry; tests: run_record | cb/emulate/run.cpp +1 | YY-NN |
| emulate.shadow | 681 | platforms registry; tests: concolic,native_trace | cb/emulate/shadow.cpp +1 | YY-NN |
| emulate.symbolic | 193 | platforms registry; tests: concolic | cb/emulate/symbolic.cpp +1 | YY-NN |
| emulate.windows16 | 1161 | platforms registry; tests: concolic,knowledge,native_trace+3; tools: gate_teeth16,trace16; native gens: 1 | cb/emulate/windows16.cpp +1 | YYGNN |

### codebreaker.engine

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| engine | 17 | platforms registry; tests: engine,knowledge,knowledge_reads+4; native gens: 2 | cb/engine/engine.cpp +1 | YYGNN |
| engine.engine | 268 | platforms registry; tests: engine,knowledge_reads,native_engine; native gens: 2 | cb/engine/engine.cpp +1 | YYGNN |
| engine.facts | 182 | platforms registry; tests: engine,knowledge,native_engine+1 | cb/engine/facts.cpp +1 | YY-NN |
| engine.lanes | 443 | platforms registry; tests: engine,platforms | cb/engine/lanes.cpp +3 | YY-NN |
| engine.native_engine | 1048 | platforms registry; tests: native_engine | none found | NN-NN |
| engine.win16 | 568 | platforms registry; tests: platforms; native gens: 1 | cb/engine/win16.cpp +1 | YYGNY |

### codebreaker.exact

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| exact | 0 | platforms registry; tests: dos,dpmi,exact_fixes+1; tools: sample_shapes_cases | cb/exact/(exact/ package) | YY-NN |
| exact.aim | 135 | tests: exact_fixes | cb/exact/aim.cpp +1 | YY-YY |
| exact.aout | 82 | platforms registry | cb/exact/aout.cpp +1 | YY-NN |
| exact.batch | 115 | closure (lazy imports of live modules) | cb/exact/batch.cpp +1 | YY-YY |
| exact.carve | 156 | closure (lazy imports of live modules) | cb/exact/carve.cpp +1 | YY-YY |
| exact.compare | 185 | closure (lazy imports of live modules) | cb/exact/compare.cpp +1 | YY-YY |
| exact.data | 517 | tests: port_fixes | cb/exact/data.cpp +1 | YY-YY |
| exact.draft | 299 | tests: port_fixes; tools: sample_shapes_cases | cb/exact/draft/(exact/draft/ package) | YY-NY |
| exact.draft.candidates | 1391 | tests: port_fixes | cb/exact/draft/candidates.cpp +5 | YY-YY |
| exact.draft.forms | 1814 | tests: port_fixes | cb/exact/draft/forms.cpp +3 | YY-YY |
| exact.draft.prove | 561 | closure (lazy imports of live modules) | cb/exact/draft/prove.cpp +1 | YY-YY |
| exact.draft.shapes | 2211 | tests: port_fixes; tools: sample_shapes_cases | cb/exact/draft/shapes.hpp +8 | YY-NY |
| exact.draft.text | 345 | tests: port_fixes; tools: sample_shapes_cases | cb/exact/draft/text.cpp +1 | YY-NY |
| exact.emu | 0 | platforms registry; tests: dos,dpmi,port_fixes | cb/exact/emu/(exact/emu/ package) | YY-NN |
| exact.emu.dos | 803 | platforms registry; tests: dos | cb/exact/emu/dos.cpp +1 | YY-NN |
| exact.emu.dpmi | 736 | platforms registry; tests: dpmi | cb/exact/emu/dpmi.cpp +1 | YY-NN |
| exact.emu.isis | 294 | closure (lazy imports of live modules) | cb/exact/emu/isis.cpp +1 | YY-YY |
| exact.emu.native_dosexec | 163 | platforms registry; tests: port_fixes | cb/exact/emu/native_dosexec.cpp +1 | YY-NN |
| exact.emu.xenix | 175 | closure (lazy imports of live modules) | cb/exact/emu/xenix.cpp +1 | YY-YY |
| exact.hooks_lattice | 14 | named in native text only | cb/exact/hooks_lattice.cpp +1 | YY-YY |
| exact.library | 722 | closure (lazy imports of live modules) | cb/exact/library.cpp +1 | YY-YY |
| exact.modules | 347 | closure (lazy imports of live modules) | cb/exact/modules.cpp +1 | YY-YY |
| exact.mspascal | 219 | named in native text only | cb/exact/mspascal.cpp +1 | YY-YY |
| exact.plm86 | 78 | named in native text only | cb/exact/plm86.cpp +1 | YY-YY |
| exact.toolchain | 374 | closure (lazy imports of live modules) | cb/exact/toolchain.cpp +1 | YY-YY |
| exact.x86 | 88 | closure (lazy imports of live modules) | cb/exact/x86.cpp +1 | YY-YY |
| exact.xenix_c | 348 | named in native text only | cb/exact/xenix_c.cpp +1 | YY-YY |

### codebreaker.formats

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| formats | 0 | platforms registry; tests: n_subjects,concolic,dos+17; tools: gate_teeth16,golden,memmap16; native gens: 3 | cb/formats/(formats/ package) | YYGNN |
| formats.cab | 178 | tests: n_subjects,media | cb/formats/cab.cpp +1 | YY-YN |
| formats.detect | 88 | platforms registry; tests: n_subjects,plumbing | cb/formats/detect.cpp +1 | YY-NN |
| formats.dos | 306 | platforms registry; tests: n_subjects,dos,exepack+2; native gens: 1 | cb/formats/dos.cpp +2 | YYGNN |
| formats.le | 397 | platforms registry; tests: n_subjects,drivers; tools: golden; native gens: 1 | cb/formats/le.cpp +4 | YYGNN |
| formats.native | 223 | platforms registry; tests: native_formats; native gens: 1 | none found | NNGNN |
| formats.ne | 163 | platforms registry; tests: n_subjects,concolic,native_trace+3; tools: gate_teeth16,memmap16,record; native gens: 1 | cb/formats/ne.cpp +3 | YYGNN |
| formats.nes | 138 | platforms registry; tests: nes | cb/formats/nes.cpp +1 | YY-NN |
| formats.omf | 248 | platforms registry; tests: verification; native gens: 1 | cb/formats/omf.cpp +1 | YYGNN |
| formats.pe | 176 | platforms registry; tests: n_subjects,concolic,run_record+1; tools: sleigh_verify,sleigh_verify_ppc; native gens: 1 | cb/formats/pe.cpp +3 | YYGNN |
| formats.pe_functions | 366 | platforms registry; tests: n_subjects,versions | cb/formats/pe_functions.cpp +1 | YY-NN |
| formats.powerpc_image | 128 | platforms registry; tests: powerpc,vmx128; native gens: 2 | cb/formats/powerpc_image.cpp +1 | YYGNN |
| formats.resources | 196 | platforms registry; tests: n_subjects,whole_module | cb/formats/resources.cpp +1 | YY-NN |
| formats.xex | 173 | platforms registry; tests: vmx128,x360,xex; native gens: 2 | cb/formats/xex.cpp +3 | YYGNN |

### codebreaker.invert

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| invert | 0 | closure (lazy imports of live modules) | cb/invert/(invert/ package) | YY-YY |
| invert.flow16 | 110 | closure (lazy imports of live modules) | cb/invert/flow16.cpp +1 | YN-YY |
| invert.native_invert | 235 | closure (lazy imports of live modules) | none found | NN-YY |

### codebreaker.java

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| java | 21 | platforms registry; tests: achievements,apimap,applet_ping+52; native gens: 1 | cb/java/(java/ package) | YYGNN |
| java._paths | 15 | platforms registry | cb/java/paths.cpp +1 | YY-NN |
| java.apimap | 399 | platforms registry; tests: apimap | cb/java/apimap.cpp +1 | YY-NN |
| java.bugfix_ledger | 166 | platforms registry | cb/java/bugfix_ledger.cpp +1 | YY-NN |
| java.cmake | 53 | platforms registry | cb/java/cmake.cpp +1 | YY-NN |
| java.console | 2 | platforms registry; tests: platform_refs,runnable_entry | cb/java/console/(java/console/ package) | YY-NN |
| java.console.platform_equivalents | 213 | platforms registry | cb/java/console/platform_equivalents.cpp +1 | YY-NN |
| java.console.platform_refs | 501 | platforms registry; tests: platform_refs,runnable_entry | cb/java/console/platform_refs.cpp +1 | YY-NN |
| java.console.sdk | 192 | platforms registry | cb/java/console/sdk.cpp +1 | YY-NN |
| java.console.xdk_scan | 118 | closure (lazy imports of live modules) | cb/java/console/xdk_scan.cpp +1 | YY-YY |
| java.debug | 321 | platforms registry; tests: ui_mark | cb/java/debug.cpp +1 | YY-NN |
| java.emit | 21 | platforms registry; tests: beta_fixes,collection_out_params,emit+2 | cb/java/emit/(java/emit/ package) | YY-NN |
| java.emit.header | 335 | platforms registry; tests: beta_fixes,emit,emit_source+1 | cb/java/emit/header.cpp +1 | YY-NN |
| java.emit.naming | 47 | platforms registry | cb/java/emit/naming.cpp +1 | YY-NN |
| java.emit.ownership | 353 | platforms registry; tests: beta_fixes,collection_out_params | cb/java/emit/ownership.cpp +1 | YY-NN |
| java.emit.source | 556 | platforms registry; tests: beta_fixes,emit_source | cb/java/emit/source.cpp +2 | YY-NN |
| java.emit.std_transforms | 95 | platforms registry | cb/java/emit/std_transforms.cpp +1 | YY-NN |
| java.ir | 225 | platforms registry; tests: beta_fixes,collection_out_params,emit+7 | cb/java/ir.cpp +1 | YY-NN |
| java.ledger | 156 | platforms registry; tests: beta_fixes,emit,emit_source | cb/java/ledger.cpp +1 | YY-NN |
| java.overlays | 79 | platforms registry | cb/java/overlays.cpp +1 | YY-NN |
| java.parse | 444 | platforms registry; tests: beta_fixes,build3_fixes,bytes+19 | cb/java/parse.cpp +1 | YY-NN |
| java.platforms | 405 | platforms registry | cb/java/platforms.cpp +1 | YY-NN |
| java.probe | 859 | platforms registry; tests: bytes,cpp03_bodies,finally+12 | cb/java/probe.cpp +1 | YY-NN |
| java.profiles | 105 | platforms registry; tests: beta_fixes,build3_fixes,call_arg_cast+10 | cb/java/profiles/(java/profiles/ package) | YY-NN |
| java.projects | 113 | platforms registry; tests: achievements,applet_ping,console_patches+10; native gens: 1 | cb/java/projects/(java/projects/ package) | YYGNN |
| java.projects.minecraft | 120 | tests: achievements,applet_ping,console_patches+9; native gens: 1 | cb/java/projects/minecraft/(java/projects/minecraft/ package) | YYGYY |
| java.projects.minecraft.achievements | 548 | tests: achievements | cb/java/projects/minecraft/achievements.cpp +1 | YY-YY |
| java.projects.minecraft.applet_ping | 130 | tests: applet_ping,console_patches; native gens: 1 | cb/java/projects/minecraft/applet_ping.cpp +1 | YYGYY |
| java.projects.minecraft.content | 131 | closure (lazy imports of live modules) | cb/java/projects/minecraft/content.cpp +1 | YY-YY |
| java.projects.minecraft.controller_nav | 284 | tests: controller_nav,review_fixes | cb/java/projects/minecraft/controller_nav.cpp +1 | YY-YY |
| java.projects.minecraft.edition | 158 | tests: edition_autodetect,edition_resolve,review_fixes | cb/java/projects/minecraft/edition.cpp +1 | YY-YY |
| java.projects.minecraft.keybind_mark | 126 | closure (lazy imports of live modules) | cb/java/projects/minecraft/keybind_mark.cpp +1 | YY-YY |
| java.projects.minecraft.platform_coverage | 419 | tests: platform_coverage_symbols | cb/java/projects/minecraft/platform_coverage.cpp +1 | YY-YY |
| java.projects.minecraft.profiles | 2 | closure (lazy imports of live modules) | cb/java/projects/minecraft/profiles/(java/projects/minecraft/profiles/ package) | YY-YY |
| java.projects.minecraft.profiles.minecraft_alpha | 26 | closure (lazy imports of live modules) | cb/java/projects/minecraft/profiles/minecraft_alpha.cpp +1 | YN-YY |
| java.projects.minecraft.profiles.minecraft_base | 46 | closure (lazy imports of live modules) | cb/java/projects/minecraft/profiles/minecraft_base.cpp +1 | YN-YY |
| java.projects.minecraft.profiles.minecraft_beta | 26 | closure (lazy imports of live modules) | cb/java/projects/minecraft/profiles/minecraft_beta.cpp +1 | YN-YY |
| java.projects.minecraft.profiles.minecraft_indev | 110 | closure (lazy imports of live modules) | cb/java/projects/minecraft/profiles/minecraft_indev.cpp +1 | YN-YY |
| java.projects.minecraft.profiles.minecraft_infdev | 52 | closure (lazy imports of live modules) | cb/java/projects/minecraft/profiles/minecraft_infdev.cpp +1 | YN-YY |
| java.projects.minecraft.profiles.minecraft_release | 37 | closure (lazy imports of live modules) | cb/java/projects/minecraft/profiles/minecraft_release.cpp +1 | YN-YY |
| java.projects.minecraft.profiles.minecraft_rubydung | 53 | closure (lazy imports of live modules) | cb/java/projects/minecraft/profiles/minecraft_rubydung.cpp +1 | YN-YY |
| java.projects.minecraft.profiles.minecraft_xbox360 | 213 | closure (lazy imports of live modules) | cb/java/projects/minecraft/profiles/minecraft_xbox360.cpp +1 | YY-YY |
| java.projects.minecraft.raycast | 114 | tests: pipeline | cb/java/projects/minecraft/raycast.cpp +1 | YY-YY |
| java.projects.minecraft.render_fix | 261 | tests: console_patches,render_fix; native gens: 1 | cb/java/projects/minecraft/render_fix.cpp +1 | YYGYY |
| java.projects.minecraft.sound_fix | 217 | tests: console_patches; native gens: 1 | cb/java/projects/minecraft/sound_fix.cpp +1 | YYGYY |
| java.projects.minecraft.ui_mark | 153 | tests: ui_mark | cb/java/projects/minecraft/ui_mark.cpp +1 | YY-YY |
| java.reflection_factory | 290 | platforms registry; tests: reflection_factory | cb/java/reflection_factory.cpp +1 | YY-NN |
| java.report | 149 | platforms registry | cb/java/report.cpp +1 | YY-NN |
| java.rules | 1046 | platforms registry; tests: beta_fixes,port_fixes,profiles_selection+1 | cb/java/rules.cpp +1 | YY-NN |
| java.runtime | 1789 | platforms registry; tests: awt_stubs,beta_fixes,runnable_entry+1 | cb/java/runtime.cpp +5 | YY-NN |
| java.stubs | 168 | platforms registry | cb/java/stubs.cpp +1 | YY-NN |
| java.textutil | 39 | platforms registry | cb/java/textutil.cpp +1 | YY-NN |
| java.transpile | 1513 | platforms registry; tests: beta_fixes,build3_fixes,bytes+27 | cb/java/transpile.cpp +1 | YY-NN |
| java.twophase | 179 | platforms registry; tests: two_phase | cb/java/twophase.cpp +1 | YY-NN |
| java.typemap | 256 | platforms registry; tests: awt_stubs,beta_fixes,emit+2 | cb/java/typemap.cpp +1 | YY-NN |
| java.types | 210 | platforms registry; tests: emit,emit_source,pipeline+1 | cb/java/types.cpp +1 | YY-NN |

### codebreaker.jobs

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| jobs | 0 | platforms registry; tests: knowledge_reads,plumbing | cb/jobs/(jobs/ package) | YY-NN |
| jobs.pool | 287 | platforms registry; tests: knowledge_reads,plumbing | cb/jobs/pool.cpp +1 | YY-NN |

### codebreaker.knowledge

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| knowledge | 173 | platforms registry; tests: knowledge,knowledge_reads,native_engine | cb/knowledge/(knowledge/ package) | YY-NN |
| knowledge.reads | 174 | platforms registry; tests: knowledge_reads | cb/knowledge/reads.cpp +1 | YY-NN |
| knowledge.sources | 116 | platforms registry; tests: knowledge | cb/knowledge/sources.cpp +1 | YY-NN |
| knowledge.store | 72 | platforms registry; tests: knowledge,knowledge_reads,native_engine | cb/knowledge/store.cpp +1 | YY-NN |

### codebreaker.platforms

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| platforms | 389 | tests: nes,platforms,port_fixes+1 | cb/platforms/(platforms/ package) | YY-YN |
| platforms.dos | 33 | nothing | cb/platforms/dos.cpp +1 | YN-YY |
| platforms.java | 25 | nothing | cb/platforms/java.cpp +1 | YN-YY |
| platforms.nes | 19 | nothing | cb/platforms/nes.cpp +1 | YN-YY |
| platforms.vxd | 43 | named in native text only | cb/platforms/vxd.cpp +1 | YY-YY |
| platforms.win16 | 72 | named in native text only | cb/platforms/win16.cpp +1 | YY-YY |
| platforms.win32 | 58 | nothing | none found | NN-YY |
| platforms.xbox360 | 45 | nothing | none found | NN-YY |

### codebreaker.recompile

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| recompile | 17 | tests: python_frontend,recompile,x360; native gens: 7 | cb/recompile/(recompile/ package) | YYGYY |
| recompile.builders | 0 | native gens: 2 | cb/recompile/builders/(recompile/builders/ package) | YYGYY |
| recompile.builders.arith | 269 | native gens: 1 | cb/recompile/builders/arith.cpp +1 | YYGYY |
| recompile.builders.floating | 172 | native gens: 2 | cb/recompile/builders/floating.cpp +1 | YYGYY |
| recompile.builders.logical | 286 | native gens: 1 | cb/recompile/builders/logical.cpp +1 | YYGYY |
| recompile.builders.memory | 492 | native gens: 1 | cb/recompile/builders/memory.cpp +1 | YYGYY |
| recompile.builders.system | 384 | native gens: 1 | cb/recompile/builders/system.cpp +1 | YYGYY |
| recompile.builders.vector | 989 | native gens: 1 | cb/recompile/builders/vector.cpp +1 | YYGYY |
| recompile.config | 234 | tests: x360; native gens: 1 | cb/recompile/config.cpp +1 | YYGYY |
| recompile.disassemble | 158 | tests: recompile; native gens: 2 | cb/recompile/disassemble.cpp +1 | YYGYY |
| recompile.discover | 181 | tests: recompile,x360; native gens: 3 | cb/recompile/discover.cpp +1 | YYGYY |
| recompile.jumptable | 364 | native gens: 1 | cb/recompile/jumptable.cpp +1 | YYGYY |
| recompile.powerpc | 1168 | tests: recompile; native gens: 2 | cb/recompile/powerpc.cpp +1 | YYGYY |
| recompile.switches | 553 | tests: x360; native gens: 2 | cb/recompile/switches.cpp +1 | YYGYY |
| recompile.xenon | 243 | tests: python_frontend,x360 | cb/recompile/xenon.cpp +3 | YY-YY |

### codebreaker.source

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| source | 0 | platforms registry; tests: drivers,kind,module_model+3; native gens: 1 | cb/source/(source/ package) | YYGNN |
| source.assembly | 232 | platforms registry | cb/source/assembly.cpp +1 | YY-NN |
| source.classes_writer | 162 | platforms registry | cb/source/classes_writer.cpp +1 | YY-NN |
| source.dos_writer | 1005 | platforms registry | cb/source/dos_writer.cpp +1 | YY-NY |
| source.driver_writer | 625 | platforms registry; tests: drivers | cb/source/driver_writer.cpp +1 | YY-NY |
| source.image_writer | 345 | platforms registry; tests: module_model | cb/source/image_writer.cpp +1 | YY-NY |
| source.java_writer | 60 | platforms registry | cb/source/java_writer.cpp +1 | YY-NN |
| source.le_writer | 518 | tests: module_model | cb/source/le_writer.cpp +1 | YY-YY |
| source.module_writer | 435 | platforms registry; tests: kind | cb/source/module_writer.cpp +1 | YY-NN |
| source.ne_asm | 0 | tests: port_fixes; native gens: 1 | cb/source/ne_asm/(source/ne_asm/ package) | YYGYY |
| source.ne_asm.fixups | 133 | native gens: 1 | cb/source/ne_asm/fixups.cpp +1 | YYGYY |
| source.ne_asm.segdiff | 217 | tests: port_fixes | cb/source/ne_asm/segdiff.cpp +1 | YY-YY |
| source.ne_asm.segment | 703 | native gens: 1 | cb/source/ne_asm/segment.cpp +1 | YYGYY |
| source.ne_writer | 458 | tests: module_model | cb/source/ne_writer.cpp +1 | YY-YY |
| source.nes_writer | 160 | platforms registry; tests: nes | cb/source/nes_writer.cpp +1 | YY-NY |
| source.pe_writer | 529 | tests: module_model,port_fixes | cb/source/pe_writer.cpp +1 | YY-YY |
| source.placement | 121 | platforms registry | cb/source/placement.cpp +1 | YY-NN |
| source.readable | 644 | platforms registry; tests: module_model,readable | cb/source/readable.cpp +1 | YY-NN |

### codebreaker.store

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| store | 0 | platforms registry; tests: kind,knowledge,native_engine+6; tools: causes,failures,oss_bench; native gens: 1 | cb/store/(store/ package) | YYGNN |
| store.functions | 126 | platforms registry; tests: kind,knowledge,native_engine+5; tools: causes,failures,oss_bench; native gens: 1 | cb/store/functions.cpp +1 | YYGNN |

### codebreaker.symbols

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| symbols | 0 | platforms registry; tests: dos,drivers,knowledge_reads+2; tools: struct_report; native gens: 1 | cb/symbols/(symbols/ package) | YYGNN |
| symbols.api_arguments | 434 | platforms registry; tests: knowledge_reads; native gens: 1 | cb/symbols/api_arguments.cpp +1 | YYGNN |
| symbols.demangle | 107 | platforms registry; tests: symbols | cb/symbols/demangle.cpp +1 | YY-NN |
| symbols.dos_interrupts | 101 | platforms registry; tests: dos | cb/symbols/dos_interrupts.cpp +1 | YY-NN |
| symbols.driver_services | 280 | platforms registry; tests: drivers | cb/symbols/driver_services.cpp +2 | YY-NN |
| symbols.libraries | 104 | platforms registry; tests: symbols,verification | cb/symbols/libraries/(symbols/libraries/ package) | YY-NN |
| symbols.libraries.coff | 166 | platforms registry; tests: symbols | cb/symbols/libraries/coff.cpp +1 | YY-NN |
| symbols.libraries.omf | 152 | platforms registry | cb/symbols/libraries/omf.cpp +1 | YY-NN |
| symbols.names | 547 | platforms registry; tests: symbols | cb/symbols/names.cpp +4 | YY-NN |
| symbols.native | 70 | platforms registry | none found | NN-NN |
| symbols.ordinals | 377 | platforms registry; tests: symbols | cb/symbols/ordinals.cpp +1 | YY-NN |
| symbols.prototypes | 124 | platforms registry; tests: knowledge_reads,symbols; tools: struct_report | cb/symbols/prototypes.cpp +1 | YY-NN |
| symbols.runtime_library | 386 | platforms registry; tests: symbols | cb/symbols/runtime_library.cpp +1 | YY-NN |
| symbols.sdk_structs | 265 | platforms registry; tools: struct_report | cb/symbols/sdk_structs.cpp +1 | YY-NN |
| symbols.wine16 | 89 | platforms registry; tests: symbols,verification | cb/symbols/wine16.cpp +1 | YY-NN |
| symbols.xdk_libraries | 151 | platforms registry | cb/symbols/xdk_libraries.cpp +1 | YY-NN |

### codebreaker.translate

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| translate | 6 | platforms registry; tests: c_backend,clang_frontend,console_patches+21; tools: judge_c_java,l2c,l2check; native gens: 3 | cb/translate/(translate/ package) | YYGNN |
| translate.backends | 1 | platforms registry; tests: c_backend,console_patches,cpp03_bodies+10; tools: l2c; native gens: 3 | cb/translate/backends/(translate/backends/ package) | YYGNN |
| translate.backends.asm | 71 | platforms registry; tests: drivers,module_model | cb/translate/backends/asm.cpp +1 | YY-NN |
| translate.backends.c | 96 | platforms registry; tests: c_backend,frame16,l2_survives; tools: l2c | cb/translate/backends/c.cpp +31 | YY-NN |
| translate.backends.c_java | 1792 | tests: java_as_c | cb/translate/backends/c_java.cpp +9 | YY-YY |
| translate.backends.cpp03 | 1857 | platforms registry; tests: console_patches,cpp03_bodies,projects | cb/translate/backends/cpp03.cpp +13 | YY-NN |
| translate.backends.cpp17 | 67 | platforms registry; tests: cpp17_backend,python_frontend | cb/translate/backends/cpp17.cpp +1 | YY-NN |
| translate.backends.csharp | 1566 | tests: csharp_kit; native gens: 1 | cb/translate/backends/csharp.cpp +3 | YYGYY |
| translate.backends.csharp_java | 631 | nothing | cb/translate/backends/csharp.cpp +3 | YY-YY |
| translate.backends.java | 1978 | tests: java_backend; native gens: 2 | cb/translate/backends/java.cpp +6 | YYGYY |
| translate.backends.java_libc | 80 | closure (lazy imports of live modules) | cb/translate/backends/java_libc.cpp +1 | YY-YY |
| translate.backends.java_objects | 1036 | closure (lazy imports of live modules) | cb/translate/backends/java_objects.cpp +1 | YY-YY |
| translate.backends.jmodel | 511 | tests: cpp17_backend | cb/translate/backends/jmodel.cpp +1 | YY-YY |
| translate.backends.module_file | 127 | platforms registry; tests: module_model | cb/translate/backends/module_file.cpp +1 | YY-NN |
| translate.desugar | 453 | platforms registry; tests: cpp17_backend,desugar | cb/translate/desugar.cpp +1 | YY-NN |
| translate.dotnet | 149 | tests: csharp_kit,dotnet_toolchain; native gens: 1 | cb/translate/dotnet/(translate/dotnet/ package) | YYGYY |
| translate.dotnet.cs2cpp | 844 | tests: csharp_kit | cb/translate/dotnet/cs2cpp.cpp +6 | YY-YY |
| translate.dotnet.judge | 149 | tests: csharp_kit | cb/translate/dotnet/judge.cpp +1 | YY-YY |
| translate.dotnet.pairs | 114 | tests: csharp_kit; native gens: 1 | none found | NNGYY |
| translate.frontends | 2 | platforms registry; tests: clang_frontend,csharp_kit,csharp_lower_trees+6; native gens: 2 | cb/translate/frontends/(translate/frontends/ package) | YNGNN |
| translate.frontends.clang | 129 | tests: clang_frontend,java_backend; native gens: 2 | cb/translate/frontends/clang/(translate/frontends/clang/ package) | YYGYY |
| translate.frontends.clang.ctypes | 387 | tests: clang_frontend | cb/translate/frontends/clang/ctypes.cpp +1 | YN-YY |
| translate.frontends.clang.j2x | 302 | closure (lazy imports of live modules) | none found | NN-YY |
| translate.frontends.clang.lower | 1505 | closure (lazy imports of live modules) | cb/translate/frontends/clang/lower.hpp +6 | YY-YY |
| translate.frontends.csharp | 142 | tests: csharp_kit,csharp_lower_trees | cb/translate/frontends/csharp/(translate/frontends/csharp/ package) | YY-YY |
| translate.frontends.csharp.lower | 1233 | tests: csharp_lower_trees | cb/translate/frontends/csharp/lower.cpp +1 | YY-YY |
| translate.frontends.java | 108 | platforms registry; tests: java_frontend,java_lower,l2 | none found | NN-NN |
| translate.frontends.java.lower | 543 | platforms registry; tests: java_lower,l2 | none found | NN-NN |
| translate.frontends.python | 319 | tests: python_frontend,pytypes | cb/translate/frontends/python/(translate/frontends/python/ package) | YY-YY |
| translate.frontends.python.infer | 1955 | nothing | cb/translate/frontends/python/infer.hpp +9 | YY-YY |
| translate.frontends.python.lower | 1816 | nothing | cb/translate/frontends/python/lower.cpp +10 | YY-YY |
| translate.frontends.python.methods | 749 | nothing | cb/translate/frontends/python/methods.cpp +1 | YY-YY |
| translate.frontends.python.program | 874 | nothing | cb/translate/frontends/python/program.cpp +1 | YY-YY |
| translate.frontends.python.pytypes | 308 | tests: pytypes | cb/translate/frontends/python/pytypes.cpp +1 | YY-YY |
| translate.frontends.python.structfmt | 55 | tests: pytypes | cb/translate/frontends/python/structfmt.cpp +1 | YY-YY |
| translate.ir | 320 | platforms registry; tests: clang_frontend,console_patches,cpp17_backend+5 | cb/translate/ir.cpp +1 | YY-NN |
| translate.javajudge | 307 | tests: cpp17_backend,java_as_c | cb/translate/javajudge.cpp +3 | YY-YY |
| translate.judge_native_jvm | 263 | tests: java_backend; tools: judge_c_java | cb/translate/judge_native_jvm.cpp +1 | YY-NY |
| translate.l2 | 431 | platforms registry; tests: c_backend,clang_frontend,csharp_kit+10; tools: l2check; native gens: 3 | cb/translate/l2.cpp +1 | YYGNN |
| translate.native_l2 | 340 | platforms registry; tests: native_l2_core | cb/translate/native_l2.cpp +1 | YY-NN |
| translate.rewrite | 114 | closure (lazy imports of live modules) | cb/translate/rewrite.cpp +1 | YY-YY |

### codebreaker.ui

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| ui | 0 | UI; tests: service,ui,ui_thin+1 | cb/ui/(ui/ package) | YY-NY |
| ui.client | 294 | UI; tests: service,ui,ui_thin | none found | NN-NY |
| ui.terminal | 983 | UI; tests: ui,ui_thin,verdicts | none found | NN-NY |

### codebreaker.verify

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| verify | 12 | platforms registry; tests: n_subjects,authored_rewrites,bytes+39; tools: causes,copy_walk,failures; native gens: 4 | cb/verify/(verify/ package) | YYGNN |
| verify.authored | 2 | tests: authored_rewrites,c_backend,match_powerpc+3 | cb/verify/authored/(verify/authored/ package) | YY-YN |
| verify.authored.match | 674 | tests: c_backend,match_powerpc,recompile+2 | cb/verify/authored/match16.cpp +1 | YY-YN |
| verify.authored.match16 | 281 | tests: verification | cb/verify/authored/match16.cpp +1 | YY-YN |
| verify.authored.repair | 587 | tests: verification | cb/verify/authored/repair.cpp +1 | YY-YY |
| verify.authored.search16 | 212 | tests: authored_rewrites,verification | cb/verify/authored/search16.cpp +1 | YY-YN |
| verify.authored.shape | 210 | tests: authored_rewrites | cb/verify/authored/shape.cpp +1 | YY-YY |
| verify.compare | 228 | platforms registry; tests: plumbing,verdicts,verification | cb/verify/compare.cpp +1 | YY-NN |
| verify.concolic | 229 | platforms registry; tests: concolic | cb/verify/concolic.cpp +1 | YY-NN |
| verify.gate16 | 2 | platforms registry; tests: verification; tools: copy_walk,gate_teeth16,record; native gens: 1 | cb/verify/gate16/(verify/gate16/ package) | YYGNN |
| verify.gate16.contract16 | 385 | platforms registry; tests: verification | cb/verify/gate16/contract16.cpp +1 | YY-NN |
| verify.gate16.isolated16 | 470 | platforms registry; tools: gate_teeth16,record,replay; native gens: 1 | cb/verify/gate16/isolated16.cpp +1 | YYGNN |
| verify.gate16.runtime16 | 62 | tests: verification | none found | NN-YN |
| verify.gate16.scoreboard16 | 203 | nothing | cb/verify/gate16/scoreboard16.cpp +1 | YY-YN |
| verify.gate16.setup16 | 255 | tests: verification; tools: copy_walk,record | cb/verify/gate16/setup16.cpp +1 | YY-NN |
| verify.guided | 222 | platforms registry | cb/verify/guided.cpp +1 | YY-NN |
| verify.lane_common | 264 | platforms registry; tests: run_record,verdicts,verification | cb/verify/lane_common.cpp +1 | YY-NN |
| verify.lanes | 4 | platforms registry; tests: n_subjects,bytes,concolic+31; tools: gate_teeth16,trace16; native gens: 3 | cb/verify/lanes/(verify/lanes/ package) | YYGNN |
| verify.lanes.dos | 75 | platforms registry; tests: platforms,verification | cb/verify/lanes/dos.cpp +1 | YY-NY |
| verify.lanes.driver | 567 | platforms registry; tests: drivers,platforms,run_record; native gens: 1 | cb/verify/lanes/driver.cpp +1 | YYGNY |
| verify.lanes.image_lane | 108 | tests: verification | cb/verify/lanes/image_lane.cpp +1 | YY-YY |
| verify.lanes.java | 225 | platforms registry; tests: bytes,console_patches,cpp03_bodies+23 | cb/verify/lanes/java.cpp +1 | YY-NN |
| verify.lanes.nes | 38 | tests: nes,platforms | none found | NN-YY |
| verify.lanes.powerpc | 393 | platforms registry; tests: platforms | cb/verify/lanes/powerpc.cpp +1 | YY-NN |
| verify.lanes.win16 | 727 | platforms registry; tests: platforms,run_record,verdicts+1; tools: gate_teeth16,trace16; native gens: 2 | cb/verify/lanes/win16.cpp +1 | YYGNN |
| verify.lanes.win32 | 917 | platforms registry; tests: n_subjects,concolic,native_trace+4 | cb/verify/lanes/win32.cpp +5 | YY-NN |
| verify.reconstruct | 305 | platforms registry; tests: kind,platforms,ui+2; tools: causes,failures,oss_bench; native gens: 1 | cb/verify/reconstruct.cpp +1 | YYGNN |
| verify.teeth | 256 | platforms registry; tests: verification; tools: gate_teeth16 | cb/verify/teeth.cpp +1 | YY-NY |
| verify.verdicts | 215 | platforms registry; tests: native_engine,verdicts,verification; tools: gate_teeth16,replay,verdict_diff | cb/verify/verdicts.cpp +1 | YY-NN |

