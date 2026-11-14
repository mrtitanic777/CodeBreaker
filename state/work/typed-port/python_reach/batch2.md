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
