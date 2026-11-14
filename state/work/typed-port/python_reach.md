# Python reach inventory: who still reaches each codebreaker/ module

Made 2026-11-13, re-run after batch 2 on master `0f231b2e5` (after batch 1 and the nine commands moved to native). Every row is
computed (state/work/typed-port/python_reach/graph.py and report.py: AST import graph of every .py, lazy imports and dotted-name
strings included, plus tests/, tools/ and native/ generators); the batch-1 verdicts at the end were checked by hand; the batch-2 section corrects the batch-1 estimate of the deletable pool.

460 modules / 141557 lines under codebreaker/ (ui is listed too, it is the kept terminal).

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

### Unreached after batch 2 (computed: not in the closure of any live root, tools included)

- `analyze.ppc_calls` (268 lines)
- `base.commands` (33 lines)
- `base.tidy_state` (206 lines)
- `build.disc_image` (289 lines)
- `build.msvc152` (79 lines)
- `build.period` (250 lines)
- `build.period16` (212 lines)
- `build.ppc_asm` (246 lines)
- `decompile.vmx128` (374 lines)
- `decompile.xbox_effect` (217 lines)
- `decompile.xenos_hlsl` (918 lines)
- `formats.fonts` (115 lines)
- `formats.gfx` (223 lines)
- `formats.xbox.effect` (334 lines)
- `formats.xbox.shader` (152 lines)
- `formats.xbox.xma` (120 lines)
- `formats.xbox.xsb` (21 lines)
- `formats.xex_edit` (159 lines)
- `formats.xex_imports` (86 lines)
- `formats.xnb` (232 lines)
- `invert.compilers` (101 lines)
- `invert.early_c` (97 lines)
- `invert.early_c_check` (112 lines)
- `invert.early_c_forward` (172 lines)
- `invert.early_c_frame` (175 lines)
- `invert.early_c_frame_probe` (61 lines)
- `invert.early_c_order_probe` (38 lines)
- `invert.early_c_round_trip` (74 lines)
- `invert.grammars` (56 lines)
- `invert.known_differences` (78 lines)
- `invert.msc6` (77 lines)
- `invert.msc6_calls` (18 lines)
- `invert.msc6_function` (41 lines)
- `invert.msc6_idioms` (94 lines)
- `invert.msc6_registers` (42 lines)
- `invert.msc6_templates` (37 lines)
- `invert.msc6_walk` (29 lines)
- `invert.msvc2` (64 lines)
- `invert.msvc2_forward` (243 lines)
- `invert.msvc2_registers` (18 lines)
- `invert.quad_align` (76 lines)
- `invert.quad_dump` (67 lines)
- `invert.run` (258 lines)
- `invert.segment_thunk` (49 lines)
- `invert.whole_function` (28 lines)
- `recompile.builders` (0 lines)
- `recompile.builders.arith` (269 lines)
- `recompile.builders.floating` (172 lines)
- `recompile.builders.logical` (286 lines)
- `recompile.builders.memory` (492 lines)
- `recompile.builders.system` (384 lines)
- `recompile.builders.vector` (989 lines)
- `recompile.config` (260 lines)
- `recompile.disassemble` (158 lines)
- `recompile.discover` (181 lines)
- `recompile.gpu` (1044 lines)
- `recompile.jumptable` (364 lines)
- `recompile.powerpc` (1168 lines)
- `recompile.project` (876 lines)
- `recompile.switches` (553 lines)
- `recompile.title` (148 lines)
- `recompile.x360project` (198 lines)
- `source.dos_build` (21 lines)
- `source.le_writer` (518 lines)
- `source.ne_asm` (0 lines)
- `source.ne_asm.datacode` (126 lines)
- `source.ne_asm.fixups` (133 lines)
- `source.ne_asm.idiom` (530 lines)
- `source.ne_asm.reachability` (175 lines)
- `source.ne_asm.recon` (473 lines)
- `source.ne_asm.run` (44 lines)
- `source.ne_asm.segdiff` (232 lines)
- `source.ne_asm.segment` (703 lines)
- `source.ne_asm.sweep` (176 lines)
- `source.ne_writer` (458 lines)
- `source.pe_writer` (529 lines)
- `source.vxd` (0 lines)
- `source.vxd.boot` (338 lines)
- `source.vxd.coalesce` (234 lines)
- `source.vxd.corpus` (125 lines)
- `source.vxd.disassembly` (780 lines)
- `source.vxd.from_source` (355 lines)
- `source.vxd.idiom` (1118 lines)
- `source.vxd.le_writer` (286 lines)
- `source.vxd.link` (534 lines)
- `source.vxd.loadable_corpus` (114 lines)
- `source.vxd.masm` (667 lines)
- `source.vxd.owbuild` (186 lines)
- `source.vxd.run` (45 lines)
- `source.vxd.services` (115 lines)
- `source.vxd.symbols` (95 lines)
- `source.vxd.tree` (194 lines)
- `symbols.debug_build` (168 lines)
- `symbols.debug_build_estimate` (196 lines)
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
- `translate.javajudge` (307 lines)
- `verify.authored` (2 lines)
- `verify.authored.match` (674 lines)
- `verify.authored.match16` (281 lines)
- `verify.authored.repair` (587 lines)
- `verify.authored.search16` (212 lines)
- `verify.authored.shape` (210 lines)
- `verify.gate16.runtime16` (62 lines)
- `verify.gate16.scoreboard16` (203 lines)
- `xna` (3 lines)
- `xna.fixups` (170 lines)
- `xna.mgfx` (162 lines)
- `xna.port` (811 lines)
- `xna.run` (212 lines)

Kept alive by `tools/*.py` alone (39 modules, 22423 lines): `base.runs`, `decompile.ctree`, `exact.aim`, `exact.batch`, `exact.carve`, `exact.compare`, `exact.data`, `exact.draft`, `exact.draft.candidates`, `exact.draft.forms`, `exact.draft.prove`, `exact.draft.shapes`, `exact.draft.text`, `exact.library`, `exact.modules`, `exact.toolchain`, `exact.x86`, `formats.cab`, `invert`, `invert.flow16`, `invert.native_invert`, `recompile`, `recompile.xenon`, `translate.backends.java`, `translate.backends.java_libc`, `translate.backends.java_objects`, `translate.frontends.clang`, `translate.frontends.clang.ctypes`, `translate.frontends.clang.j2x`, `translate.frontends.clang.lower`, `translate.frontends.python`, `translate.frontends.python.infer`, `translate.frontends.python.lower`, `translate.frontends.python.methods`, `translate.frontends.python.program`, `translate.frontends.python.pytypes`, `translate.frontends.python.structfmt`, `translate.judge_native_jvm`, `verify.gate16.setup16`

## Totals by package

| package | modules | lines | reached by a live cmd | UI | tests only | tools too | nothing/native-text only | YYGYY |
|---|---|---|---|---|---|---|---|---|
| (root) | 1 | 23 | 1 | 1 | 0 | 0 | 0 | 0 |
| codebreaker.__main__ | 1 | 212 | 1 | 0 | 0 | 0 | 0 | 0 |
| codebreaker.analyze | 13 | 3877 | 12 | 12 | 1 | 0 | 0 | 0 |
| codebreaker.base | 10 | 2000 | 7 | 6 | 1 | 1 | 1 | 0 |
| codebreaker.build | 30 | 11042 | 25 | 16 | 5 | 0 | 0 | 0 |
| codebreaker.compare | 3 | 961 | 3 | 2 | 0 | 0 | 0 | 0 |
| codebreaker.coverage | 3 | 716 | 3 | 0 | 0 | 0 | 0 | 0 |
| codebreaker.decompile | 18 | 9034 | 14 | 11 | 3 | 1 | 0 | 1 |
| codebreaker.emulate | 19 | 7394 | 19 | 13 | 0 | 0 | 0 | 0 |
| codebreaker.engine | 7 | 2638 | 7 | 5 | 0 | 0 | 0 | 0 |
| codebreaker.exact | 27 | 12172 | 6 | 6 | 4 | 3 | 14 | 0 |
| codebreaker.formats | 50 | 10731 | 40 | 13 | 9 | 0 | 1 | 0 |
| codebreaker.invert | 28 | 2496 | 0 | 0 | 13 | 0 | 15 | 0 |
| codebreaker.java | 56 | 14659 | 34 | 34 | 10 | 0 | 12 | 4 |
| codebreaker.jobs | 2 | 287 | 2 | 2 | 0 | 0 | 0 | 0 |
| codebreaker.knowledge | 5 | 585 | 5 | 4 | 0 | 0 | 0 | 0 |
| codebreaker.platforms | 14 | 801 | 14 | 1 | 0 | 0 | 0 | 0 |
| codebreaker.recompile | 19 | 7802 | 0 | 0 | 5 | 2 | 12 | 13 |
| codebreaker.source | 47 | 13687 | 17 | 7 | 13 | 0 | 17 | 3 |
| codebreaker.store | 2 | 126 | 2 | 2 | 0 | 0 | 0 | 0 |
| codebreaker.symbols | 18 | 3717 | 16 | 16 | 1 | 0 | 1 | 0 |
| codebreaker.translate | 41 | 23734 | 14 | 14 | 15 | 2 | 10 | 4 |
| codebreaker.ui | 6 | 3193 | 6 | 3 | 0 | 0 | 0 | 0 |
| codebreaker.verify | 35 | 8312 | 18 | 14 | 14 | 1 | 2 | 0 |
| codebreaker.xna | 5 | 1358 | 0 | 0 | 5 | 0 | 0 | 0 |

## Every module

### (root)

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| (package) | 23 | live cmds: all 8; UI; tests: n_subjects,ll,+125; tools: carve,causes,copy_walk; native gens: 27 | none found | NNGNN |

### codebreaker.__main__

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| __main__ | 212 | live cmds: facts; tools: carve,unreachable | none found | NN-NY |

### codebreaker.analyze

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| analyze | 23 | live cmds: all 8; UI; tests: analysis,c_backend,dos+2; native gens: 3 | cb/analyze/(analyze/ package) | YYGNN |
| analyze.call_graph | 110 | live cmds: all 8; UI; tests: analysis | cb/analyze/call_graph.cpp +1 | YY-NN |
| analyze.classes | 533 | live cmds: all 8; UI; tests: analysis | cb/analyze/classes.cpp +1 | YY-NN |
| analyze.conventions | 136 | live cmds: all 8; UI; tests: analysis; native gens: 1 | cb/analyze/conventions.cpp +1 | YYGNN |
| analyze.dispatch | 404 | live cmds: all 8; UI; tests: analysis | cb/analyze/dispatch.cpp +1 | YY-NN |
| analyze.dos_functions | 516 | live cmds: all 8; UI; tests: dos | cb/analyze/dos_functions.cpp +1 | YY-NN |
| analyze.globals | 105 | live cmds: all 8; UI; tests: analysis; native gens: 1 | cb/analyze/globals.cpp +1 | YYGNN |
| analyze.image | 524 | live cmds: all 8; UI; tests: analysis,c_backend; native gens: 2 | cb/analyze/image.cpp +1 | YYGNN |
| analyze.native | 157 | live cmds: all 8; UI | cb/analyze/native.cpp +1 | YY-NN |
| analyze.origin | 489 | live cmds: all 8; UI; tests: port_fixes | cb/analyze/origin.cpp +1 | YY-NN |
| analyze.ppc_calls | 268 | tests: ppc_calls | cb/analyze/ppc_calls.cpp +2 | YY-YY |
| analyze.strings | 191 | live cmds: all 8; UI; tests: analysis; native gens: 2 | cb/analyze/strings.cpp +1 | YYGNN |
| analyze.structures | 421 | live cmds: all 8; UI; tests: analysis,c_backend | cb/analyze/structures.cpp +1 | YY-NN |

### codebreaker.base

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| base | 0 | live cmds: all 8; UI; tests: n_subjects,ll,analysis+24; tools: copy_walk,failures,freeze_tests; native gens: 7 | cb/base/(base/ package) | YYGNN |
| base.cache | 591 | live cmds: all 8; UI; tests: plumbing,verification | cb/base/cache.cpp +4 | YY-NN |
| base.commands | 33 | nothing | cb/base/commands.cpp +1 | YY-YY |
| base.converted | 188 | live cmds: all 8; UI; tests: converted | cb/base/converted.cpp +1 | YY-NN |
| base.corpus | 295 | live cmds: all 8; UI; tests: n_subjects,analysis,concolic+18; tools: failures,freeze_tests,l2c; native gens: 2 | cb/base/corpus.cpp +3 | YYGNN |
| base.ctext | 51 | live cmds: all 8 | cb/base/ctext.cpp +1 | YY-NY |
| base.parallel | 268 | live cmds: all 8; UI; tests: ll,plumbing,verification | cb/base/parallel.cpp +1 | YY-NN |
| base.runs | 82 | tests: plumbing; tools: copy_walk,record | cb/base/runs.cpp +1 | YY-NN |
| base.settings | 286 | live cmds: all 8; UI; tests: assembler,csharp_kit,dos+6; tools: copy_walk,golden,judge_c_java; native gens: 6 | cb/base/settings.cpp +1 | YYGNN |
| base.tidy_state | 206 | tests: plumbing | cb/base/tidy_state.cpp +3 | YY-YY |

### codebreaker.build

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| build | 0 | live cmds: all 8; UI; tests: ,c_backend,dos+21; tools: make_api_table,oss_bench,trace16 | cb/build/(build/ package) | YN-NN |
| build.asm16 | 689 | live cmds: all 8; UI; tests: verification | none found | NN-NN |
| build.data | 530 | live cmds: all 8; UI; tests: whole_module | none found | NN-NN |
| build.definitions | 208 | live cmds: all 8; UI; tests: whole_module | none found | NN-NN |
| build.disc_image | 289 | tests: media | none found | NN-YY |
| build.dos_sessions | 185 | live cmds: all 8; UI | none found | NN-NN |
| build.dos_tools | 83 | live cmds: all 8; UI; tests: dos,drivers,invert | none found | NN-NN |
| build.driver | 332 | live cmds: all 8; UI; tests: drivers | none found | NN-NN |
| build.exepack | 107 | live cmds: all 8; UI; tests: exepack | none found | NN-NN |
| build.flat_thunk | 1085 | live cmds: all 8; UI; tests: flat_thunk,whole_module | none found | NN-NN |
| build.floppy | 219 | live cmds: facts,boot-floppy | none found | NN-NY |
| build.functions | 678 | live cmds: all 8; UI; tests: engine,match_powerpc,platforms+4 | none found | NN-NN |
| build.m6502_asm | 147 | live cmds: all 8; tests: nes | none found | NN-NY |
| build.m68k_asm | 123 | live cmds: all 8; tests: m68k | none found | NN-NY |
| build.masm | 442 | live cmds: all 8; UI; tests: dos | none found | NN-NN |
| build.module | 2162 | live cmds: all 8; UI; tests: c_backend,platforms,whole_module | none found | NN-NN |
| build.msc6 | 935 | live cmds: all 8; UI; tests: ,dos,verification+1 | none found | NN-NN |
| build.msvc152 | 79 | tests: port_fixes | none found | NN-YY |
| build.mzfile | 50 | live cmds: all 8 | none found | NN-NY |
| build.native_dos | 207 | live cmds: all 8; UI; tests: dos | none found | NN-NN |
| build.period | 250 | tests: verification | none found | NN-YY |
| build.period16 | 212 | tests: whole_module | none found | NN-YY |
| build.pklite | 380 | live cmds: all 8; tests: pklite | none found | NN-NY |
| build.ppc_asm | 246 | tests: xex_edit | none found | NN-YY |
| build.resources | 658 | live cmds: all 8; UI; tests: whole_module | none found | NN-NN |
| build.sm83_asm | 117 | live cmds: all 8; tests: z80 | none found | NN-NY |
| build.stubjoin | 122 | live cmds: all 8 | none found | NN-NY |
| build.vxdfile | 63 | live cmds: all 8 | none found | NN-NY |
| build.watcom | 160 | live cmds: all 8; UI; tests: ,plumbing,readable+3; tools: make_api_table,oss_bench,trace16 | none found | NN-NN |
| build.z80_asm | 284 | live cmds: all 8; tests: port_fixes,z80 | none found | NN-NY |

### codebreaker.compare

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| compare | 0 | live cmds: all 8; UI; tests: symbols,versions | cb/compare/(compare/ package) | YY-NN |
| compare.mask | 90 | live cmds: all 8; UI; tests: symbols | cb/compare/mask.cpp +1 | YY-NN |
| compare.versions | 871 | live cmds: facts; tests: versions | cb/compare/versions.cpp +3 | YY-NY |

### codebreaker.coverage

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| coverage | 0 | live cmds: progress,facts; tests: platforms,ui | cb/coverage/(coverage/ package) | YY-NY |
| coverage.ledger | 341 | live cmds: coverage,progress,facts; tests: ui | cb/coverage/ledger.cpp +3 | YY-NY |
| coverage.reach | 375 | live cmds: facts; tests: platforms | cb/coverage/reach.cpp +3 | YY-NY |

### codebreaker.decompile

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| decompile | 0 | live cmds: all 8; UI; tests: n_subjects,asm_blocks,c_backend+15; tools: freeze_tests,golden,goto_report; native gens: 1 | cb/decompile/(decompile/ package) | YYGNN |
| decompile.asm_blocks | 267 | live cmds: all 8; UI; tests: asm_blocks,c_backend | cb/decompile/asm_blocks.cpp +1 | YY-NN |
| decompile.c_text | 1428 | live cmds: all 8; UI; tests: n_subjects,c_backend,frozen+1; tools: freeze_tests | cb/decompile/c_text.hpp +5 | YY-NN |
| decompile.ctree | 241 | tools: goto_report,oss_bench | cb/decompile/ctree.cpp +1 | YY-NY |
| decompile.decompiler | 898 | live cmds: all 8; UI; tests: c_backend,dos,frozen+6; tools: freeze_tests,golden,goto_report | cb/decompile/decompiler.cpp +5 | YY-NN |
| decompile.describe | 2178 | live cmds: all 8; UI; tests: n_subjects,dos,platforms+2; tools: golden,trace16 | cb/decompile/describe.cpp +6 | YY-NN |
| decompile.frame16 | 236 | live cmds: all 8; UI; tests: frame16 | cb/decompile/frame16.cpp +1 | YY-NN |
| decompile.inline_asm | 191 | live cmds: all 8; UI | cb/decompile/inline_asm.cpp +1 | YY-NN |
| decompile.jar | 188 | live cmds: all 8; UI | cb/decompile/jar.cpp +1 | YY-NN |
| decompile.m6502 | 181 | live cmds: all 8; UI; tests: nes; tools: sleigh_verify_6502 | cb/decompile/m6502.cpp +1 | YY-NN |
| decompile.m68k | 751 | live cmds: all 8; tests: m68k | cb/decompile/m68k.cpp +1 | YY-NY |
| decompile.shape16 | 111 | live cmds: all 8; UI; tests: frame16 | cb/decompile/shape16.cpp +1 | YY-NN |
| decompile.sm83 | 270 | live cmds: all 8; tests: z80 | cb/decompile/sm83.cpp +1 | YY-NY |
| decompile.struct_text | 114 | live cmds: all 8; UI; tests: c_backend | cb/decompile/struct_text.cpp +1 | YY-NN |
| decompile.vmx128 | 374 | tests: vmx128; native gens: 1 | cb/decompile/vmx128.cpp +1 | YYGYY |
| decompile.xbox_effect | 217 | tests: xbox_effect | cb/decompile/xbox_effect.cpp +1 | YY-YY |
| decompile.xenos_hlsl | 918 | tests: xbox_effect | cb/decompile/xenos.cpp +3 | YY-YY |
| decompile.z80 | 471 | live cmds: all 8; tests: z80 | cb/decompile/z80.cpp +1 | YY-NY |

### codebreaker.emulate

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| emulate | 45 | live cmds: all 8; UI; tests: concolic,knowledge,m68k+8; tools: gate_teeth16,sleigh_verify_6502,sleigh_verify_ppc; native gens: 2 | cb/emulate/(emulate/ package) | YYGNN |
| emulate.driver | 217 | live cmds: all 8; tests: knowledge,native_trace,platforms; native gens: 1 | cb/emulate/driver.cpp +1 | YYGNY |
| emulate.emulator | 616 | live cmds: all 8; UI; tests: concolic,native_trace,platforms+1; native gens: 1 | cb/emulate/emulator.cpp +1 | YYGNN |
| emulate.fast_uc | 69 | live cmds: all 8; UI; tests: plumbing | none found | NN-NN |
| emulate.m6502 | 314 | live cmds: all 8; tests: nes,platforms,run_record; tools: sleigh_verify_6502 | cb/emulate/m6502.cpp +1 | YY-NY |
| emulate.m68k | 715 | live cmds: all 8; tests: m68k,platforms,run_record | cb/emulate/m68k.cpp +1 | YY-NY |
| emulate.native_driver | 204 | live cmds: all 8 | cb/emulate/native_driver.cpp +1 | YY-NY |
| emulate.native_shadow | 239 | live cmds: all 8; UI | cb/emulate/native_shadow.cpp +1 | YY-NN |
| emulate.native_trace | 390 | live cmds: all 8; UI; tests: native_trace | cb/emulate/native_trace.cpp +1 | YY-NN |
| emulate.native_win16 | 209 | live cmds: all 8; UI | cb/emulate/native_win16.cpp +1 | YY-NN |
| emulate.pcode | 147 | live cmds: all 8; UI; tests: concolic,native_trace | cb/emulate/pcode.cpp +1 | YY-NN |
| emulate.ports16 | 195 | live cmds: all 8; UI; tests: verification | cb/emulate/ports16.cpp +1 | YY-NN |
| emulate.powerpc | 916 | live cmds: all 8; UI; tests: concolic,platforms,powerpc; tools: sleigh_verify_ppc | cb/emulate/powerpc.cpp +1 | YY-NN |
| emulate.run | 76 | live cmds: all 8; UI; tests: run_record | cb/emulate/run.cpp +1 | YY-NN |
| emulate.shadow | 681 | live cmds: all 8; UI; tests: concolic,native_trace | cb/emulate/shadow.cpp +1 | YY-NN |
| emulate.sm83 | 355 | live cmds: all 8; tests: platforms,run_record,z80 | cb/emulate/sm83.cpp +1 | YY-NY |
| emulate.symbolic | 193 | live cmds: all 8; UI; tests: concolic | cb/emulate/symbolic.cpp +1 | YY-NN |
| emulate.windows16 | 1161 | live cmds: all 8; UI; tests: concolic,knowledge,native_trace+3; tools: gate_teeth16,trace16; native gens: 1 | cb/emulate/windows16.cpp +1 | YYGNN |
| emulate.z80 | 652 | live cmds: all 8; tests: platforms,run_record,z80 | cb/emulate/z80.cpp +1 | YY-NY |

### codebreaker.engine

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| engine | 17 | live cmds: all 8; UI; tests: engine,knowledge,knowledge_reads+4; native gens: 2 | cb/engine/engine.cpp +1 | YYGNN |
| engine.engine | 268 | live cmds: all 8; UI; tests: engine,knowledge_reads,native_engine; native gens: 2 | cb/engine/engine.cpp +1 | YYGNN |
| engine.facts | 182 | live cmds: all 8; UI; tests: engine,knowledge,native_engine+1 | cb/engine/facts.cpp +1 | YY-NN |
| engine.lanes | 443 | live cmds: all 8; UI; tests: engine,platforms | cb/engine/lanes.cpp +3 | YY-NN |
| engine.native_engine | 1048 | live cmds: all 8; UI; tests: native_engine | none found | NN-NN |
| engine.show | 112 | live cmds: facts | cb/engine/show.cpp +1 | YY-NY |
| engine.win16 | 568 | live cmds: all 8; tests: platforms; native gens: 1 | cb/engine/win16.cpp +1 | YYGNY |

### codebreaker.exact

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| exact | 0 | live cmds: all 8; UI; tests: dos,dpmi,exact_fixes+1; tools: sample_shapes_cases | cb/exact/(exact/ package) | YY-NN |
| exact.aim | 135 | tests: exact_fixes | cb/exact/aim.cpp +1 | YY-YY |
| exact.aout | 82 | live cmds: all 8; UI | cb/exact/aout.cpp +1 | YY-NN |
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
| exact.emu | 0 | live cmds: all 8; UI; tests: dos,dpmi,port_fixes | cb/exact/emu/(exact/emu/ package) | YY-NN |
| exact.emu.dos | 803 | live cmds: all 8; UI; tests: dos | cb/exact/emu/dos.cpp +1 | YY-NN |
| exact.emu.dpmi | 736 | live cmds: all 8; UI; tests: dpmi | cb/exact/emu/dpmi.cpp +1 | YY-NN |
| exact.emu.isis | 294 | closure (lazy imports of live modules) | cb/exact/emu/isis.cpp +1 | YY-YY |
| exact.emu.native_dosexec | 163 | live cmds: all 8; UI; tests: port_fixes | cb/exact/emu/native_dosexec.cpp +1 | YY-NN |
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
| formats | 0 | live cmds: all 8; UI; tests: n_subjects,concolic,dos+25; tools: gate_teeth16,golden,memmap16; native gens: 5 | cb/formats/(formats/ package) | YYGNN |
| formats.cab | 178 | tests: n_subjects,media | cb/formats/cab.cpp +1 | YY-YN |
| formats.detect | 88 | live cmds: all 8; UI; tests: n_subjects,plumbing,xbox_formats; native gens: 1 | cb/formats/detect.cpp +1 | YYGNN |
| formats.dos | 306 | live cmds: all 8; UI; tests: n_subjects,dos,exepack+2; native gens: 1 | cb/formats/dos.cpp +2 | YYGNN |
| formats.fonts | 115 | tests: n_subjects,versions | cb/formats/fonts.cpp +1 | YY-YY |
| formats.gb | 113 | live cmds: all 8 | cb/formats/gb.cpp +1 | YY-NY |
| formats.genesis | 128 | live cmds: all 8 | cb/formats/genesis.cpp +1 | YY-NY |
| formats.gfx | 223 | tests: xbox_formats | cb/formats/gfx.cpp +2 | YY-YY |
| formats.inspect | 807 | live cmds: coverage,progress,facts; tests: n_subjects,inspect_render,ui+1 | cb/formats/inspect.cpp +2 | YY-NY |
| formats.le | 411 | live cmds: all 8; UI; tests: n_subjects,drivers; tools: golden; native gens: 1 | cb/formats/le.cpp +4 | YYGNN |
| formats.media | 256 | live cmds: all 8; tests: dos,drivers,media+1 | cb/formats/media.cpp +2 | YY-NY |
| formats.msx | 66 | live cmds: all 8 | cb/formats/msx.cpp +1 | YY-NY |
| formats.native | 223 | live cmds: all 8; UI; tests: native_formats; native gens: 1 | none found | NNGNN |
| formats.ne | 163 | live cmds: all 8; UI; tests: n_subjects,concolic,invert+4; tools: gate_teeth16,memmap16,record; native gens: 1 | cb/formats/ne.cpp +3 | YYGNN |
| formats.nes | 138 | live cmds: all 8; UI; tests: nes | cb/formats/nes.cpp +1 | YY-NN |
| formats.omf | 248 | live cmds: all 8; UI; tests: invert,verification; native gens: 1 | cb/formats/omf.cpp +1 | YYGNN |
| formats.pe | 176 | live cmds: all 8; UI; tests: n_subjects,concolic,run_record+2; tools: sleigh_verify,sleigh_verify_ppc; native gens: 1 | cb/formats/pe.cpp +3 | YYGNN |
| formats.pe_functions | 366 | live cmds: all 8; UI; tests: n_subjects,versions | cb/formats/pe_functions.cpp +1 | YY-NN |
| formats.powerpc_image | 128 | live cmds: all 8; UI; tests: powerpc,vmx128; native gens: 2 | cb/formats/powerpc_image.cpp +1 | YYGNN |
| formats.resources | 196 | live cmds: all 8; UI; tests: n_subjects,whole_module | cb/formats/resources.cpp +1 | YY-NN |
| formats.sms | 129 | live cmds: all 8 | cb/formats/sms.cpp +1 | YY-NY |
| formats.xbox | 119 | live cmds: all 8; tests: xbe,xbox_effect,xbox_formats+1; native gens: 2 | cb/formats/xbox/(formats/xbox/ package) | YYGNY |
| formats.xbox.bink | 92 | live cmds: all 8; tests: xbox_formats; native gens: 2 | cb/formats/xbox/bink.cpp +1 | YYGNY |
| formats.xbox.chd | 124 | live cmds: all 8; native gens: 2 | cb/formats/xbox/chd.cpp +1 | YYGNY |
| formats.xbox.ciso | 123 | live cmds: all 8 | cb/formats/xbox/ciso.cpp +1 | YY-NY |
| formats.xbox.effect | 334 | tests: xbox_effect | cb/formats/xbox/effect.cpp +1 | YY-YY |
| formats.xbox.fatx | 760 | live cmds: all 8; tests: xbox_formats | cb/formats/xbox/fatx.cpp +1 | YY-NY |
| formats.xbox.gdf | 134 | live cmds: all 8; tests: xbox_formats | cb/formats/xbox/gdf.cpp +1 | YY-NY |
| formats.xbox.god | 207 | live cmds: all 8; tests: xbox_formats | cb/formats/xbox/god.cpp +1 | YY-NY |
| formats.xbox.heavyiron | 400 | live cmds: all 8; tests: xbox_formats | cb/formats/xbox/heavyiron.cpp +1 | YY-NY |
| formats.xbox.nand | 97 | live cmds: all 8; tests: xbox_formats | cb/formats/xbox/nand.cpp +1 | YY-NY |
| formats.xbox.pck | 126 | live cmds: all 8 | cb/formats/xbox/pck.cpp +1 | YY-NY |
| formats.xbox.shader | 152 | tests: xbox_effect | cb/formats/xbox/shader.cpp +1 | YY-YY |
| formats.xbox.stfs | 530 | live cmds: all 8; tests: xbox_formats | cb/formats/xbox/stfs.cpp +1 | YY-NY |
| formats.xbox.texture | 100 | live cmds: all 8; tests: xbox_formats,xna | cb/formats/xbox/texture.cpp +1 | YY-NY |
| formats.xbox.volumes | 199 | live cmds: all 8 | cb/formats/xbox/volumes.cpp +1 | YY-NY |
| formats.xbox.xbe | 129 | live cmds: all 8; tests: xbe,xbox_formats; native gens: 2 | cb/formats/xbox/xbe.cpp +1 | YYGNY |
| formats.xbox.xdbf | 188 | live cmds: all 8; tests: xbox_formats; native gens: 2 | cb/formats/xbox/xdbf.cpp +1 | YYGNY |
| formats.xbox.xma | 120 | tests: xbox_formats | none found | NN-YY |
| formats.xbox.xpr | 135 | live cmds: all 8; tests: xbox_formats; native gens: 2 | cb/formats/xbox/xpr.cpp +1 | YYGNY |
| formats.xbox.xsb | 21 | tests: xna | none found | NN-YY |
| formats.xbox.xur | 148 | live cmds: all 8; tests: xbox_formats; native gens: 2 | cb/formats/xbox/xur.cpp +1 | YYGNY |
| formats.xbox.xwb | 153 | live cmds: all 8; tests: xbox_formats | none found | NN-NY |
| formats.xbox.xzp | 145 | live cmds: all 8; tests: xbox_formats; native gens: 1 | cb/formats/xbox/xzp.cpp +1 | YYGNY |
| formats.xbox.zar | 1034 | live cmds: all 8 | cb/formats/xbox/zar.cpp +1 | YY-NY |
| formats.xex | 173 | live cmds: all 8; UI; tests: vmx128,x360,xex+1; native gens: 2 | cb/formats/xex.cpp +3 | YYGNN |
| formats.xex_edit | 159 | tests: xex_edit | none found | NN-YY |
| formats.xex_imports | 86 | nothing | cb/formats/xex_imports.cpp +1 | YY-YY |
| formats.xnb | 232 | tests: xbox_effect,xna | none found | NN-YY |
| formats.zx | 53 | live cmds: all 8; tests: z80 | cb/formats/zx.cpp +1 | YY-NY |

### codebreaker.invert

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| invert | 0 | tests: invert | cb/invert/(invert/ package) | YY-YY |
| invert.compilers | 101 | tests: invert | none found | NN-YY |
| invert.early_c | 97 | tests: invert | cb/invert/early_c.cpp +10 | YY-YY |
| invert.early_c_check | 112 | tests: invert | cb/invert/early_c.cpp +1 | YN-YY |
| invert.early_c_forward | 172 | nothing | cb/invert/early_c.cpp +3 | YY-YY |
| invert.early_c_frame | 175 | tests: invert | cb/invert/early_c.cpp +3 | YY-YY |
| invert.early_c_frame_probe | 61 | nothing | cb/invert/early_c.cpp +3 | YN-YY |
| invert.early_c_order_probe | 38 | nothing | cb/invert/early_c.cpp +1 | YN-YY |
| invert.early_c_round_trip | 74 | nothing | cb/invert/early_c.cpp +1 | YN-YY |
| invert.flow16 | 110 | tests: invert | cb/invert/flow16.cpp +1 | YN-YY |
| invert.grammars | 56 | tests: invert | cb/invert/grammars.cpp +1 | YY-YY |
| invert.known_differences | 78 | tests: invert | cb/invert/known_differences.cpp +1 | YY-YY |
| invert.msc6 | 77 | tests: invert | cb/invert/msc6.cpp +5 | YY-YY |
| invert.msc6_calls | 18 | nothing | none found | NN-YY |
| invert.msc6_function | 41 | nothing | none found | NN-YY |
| invert.msc6_idioms | 94 | nothing | none found | NN-YY |
| invert.msc6_registers | 42 | tests: invert | none found | NN-YY |
| invert.msc6_templates | 37 | nothing | none found | NN-YY |
| invert.msc6_walk | 29 | nothing | none found | NN-YY |
| invert.msvc2 | 64 | tests: invert | cb/invert/msvc2.cpp +3 | YY-YY |
| invert.msvc2_forward | 243 | nothing | cb/invert/msvc2.cpp +1 | YN-YY |
| invert.msvc2_registers | 18 | nothing | cb/invert/msvc2.cpp +1 | YN-YY |
| invert.native_invert | 281 | closure (lazy imports of live modules) | none found | NN-YY |
| invert.quad_align | 76 | nothing | cb/invert/quad_align.cpp +1 | YY-YY |
| invert.quad_dump | 67 | nothing | cb/invert/quad_dump.cpp +1 | YY-YY |
| invert.run | 258 | tests: invert | none found | NN-YY |
| invert.segment_thunk | 49 | tests: invert | none found | NN-YY |
| invert.whole_function | 28 | nothing | none found | NN-YY |

### codebreaker.java

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| java | 21 | live cmds: all 8; UI; tests: achievements,apimap,applet_ping+52; native gens: 1 | cb/java/(java/ package) | YYGNN |
| java._paths | 15 | live cmds: all 8; UI | cb/java/paths.cpp +1 | YY-NN |
| java.apimap | 399 | live cmds: all 8; UI; tests: apimap | cb/java/apimap.cpp +1 | YY-NN |
| java.bugfix_ledger | 166 | live cmds: all 8; UI | cb/java/bugfix_ledger.cpp +1 | YY-NN |
| java.cmake | 53 | live cmds: all 8; UI | cb/java/cmake.cpp +1 | YY-NN |
| java.console | 2 | live cmds: all 8; UI; tests: platform_refs,runnable_entry | cb/java/console/(java/console/ package) | YY-NN |
| java.console.platform_equivalents | 213 | live cmds: all 8; UI | cb/java/console/platform_equivalents.cpp +1 | YY-NN |
| java.console.platform_refs | 501 | live cmds: all 8; UI; tests: platform_refs,runnable_entry | cb/java/console/platform_refs.cpp +1 | YY-NN |
| java.console.sdk | 192 | live cmds: all 8; UI | cb/java/console/sdk.cpp +1 | YY-NN |
| java.console.xdk_scan | 118 | closure (lazy imports of live modules) | cb/java/console/xdk_scan.cpp +1 | YY-YY |
| java.debug | 321 | live cmds: all 8; UI; tests: ui_mark | cb/java/debug.cpp +1 | YY-NN |
| java.emit | 21 | live cmds: all 8; UI; tests: beta_fixes,collection_out_params,emit+2 | cb/java/emit/(java/emit/ package) | YY-NN |
| java.emit.header | 335 | live cmds: all 8; UI; tests: beta_fixes,emit,emit_source+1 | cb/java/emit/header.cpp +1 | YY-NN |
| java.emit.naming | 47 | live cmds: all 8; UI | cb/java/emit/naming.cpp +1 | YY-NN |
| java.emit.ownership | 353 | live cmds: all 8; UI; tests: beta_fixes,collection_out_params | cb/java/emit/ownership.cpp +1 | YY-NN |
| java.emit.source | 556 | live cmds: all 8; UI; tests: beta_fixes,emit_source | cb/java/emit/source.cpp +2 | YY-NN |
| java.emit.std_transforms | 95 | live cmds: all 8; UI | cb/java/emit/std_transforms.cpp +1 | YY-NN |
| java.ir | 225 | live cmds: all 8; UI; tests: beta_fixes,collection_out_params,emit+7 | cb/java/ir.cpp +1 | YY-NN |
| java.ledger | 156 | live cmds: all 8; UI; tests: beta_fixes,emit,emit_source | cb/java/ledger.cpp +1 | YY-NN |
| java.overlays | 79 | live cmds: all 8; UI | cb/java/overlays.cpp +1 | YY-NN |
| java.parse | 444 | live cmds: all 8; UI; tests: beta_fixes,build3_fixes,bytes+19 | cb/java/parse.cpp +1 | YY-NN |
| java.platforms | 405 | live cmds: all 8; UI | cb/java/platforms.cpp +1 | YY-NN |
| java.probe | 859 | live cmds: all 8; UI; tests: bytes,cpp03_bodies,finally+12 | cb/java/probe.cpp +1 | YY-NN |
| java.profiles | 105 | live cmds: all 8; UI; tests: beta_fixes,build3_fixes,call_arg_cast+10 | cb/java/profiles/(java/profiles/ package) | YY-NN |
| java.projects | 113 | live cmds: all 8; UI; tests: achievements,applet_ping,console_patches+10; native gens: 1 | cb/java/projects/(java/projects/ package) | YYGNN |
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
| java.reflection_factory | 290 | live cmds: all 8; UI; tests: reflection_factory | cb/java/reflection_factory.cpp +1 | YY-NN |
| java.report | 149 | live cmds: all 8; UI | cb/java/report.cpp +1 | YY-NN |
| java.rules | 1046 | live cmds: all 8; UI; tests: beta_fixes,port_fixes,profiles_selection+1 | cb/java/rules.cpp +1 | YY-NN |
| java.runtime | 1789 | live cmds: all 8; UI; tests: awt_stubs,beta_fixes,runnable_entry+1 | cb/java/runtime.cpp +5 | YY-NN |
| java.stubs | 168 | live cmds: all 8; UI | cb/java/stubs.cpp +1 | YY-NN |
| java.textutil | 39 | live cmds: all 8; UI | cb/java/textutil.cpp +1 | YY-NN |
| java.transpile | 1513 | live cmds: all 8; UI; tests: beta_fixes,build3_fixes,bytes+27 | cb/java/transpile.cpp +1 | YY-NN |
| java.twophase | 179 | live cmds: all 8; UI; tests: two_phase | cb/java/twophase.cpp +1 | YY-NN |
| java.typemap | 256 | live cmds: all 8; UI; tests: awt_stubs,beta_fixes,emit+2 | cb/java/typemap.cpp +1 | YY-NN |
| java.types | 210 | live cmds: all 8; UI; tests: emit,emit_source,pipeline+1 | cb/java/types.cpp +1 | YY-NN |

### codebreaker.jobs

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| jobs | 0 | live cmds: all 8; UI; tests: knowledge_reads,plumbing | cb/jobs/(jobs/ package) | YY-NN |
| jobs.pool | 287 | live cmds: all 8; UI; tests: knowledge_reads,plumbing | cb/jobs/pool.cpp +1 | YY-NN |

### codebreaker.knowledge

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| knowledge | 173 | live cmds: all 8; UI; tests: knowledge,knowledge_reads,native_engine | cb/knowledge/(knowledge/ package) | YY-NN |
| knowledge.reads | 174 | live cmds: all 8; UI; tests: knowledge_reads | cb/knowledge/reads.cpp +1 | YY-NN |
| knowledge.show | 50 | live cmds: knowledge,facts | cb/knowledge/show.cpp +1 | YY-NY |
| knowledge.sources | 116 | live cmds: all 8; UI; tests: knowledge | cb/knowledge/sources.cpp +1 | YY-NN |
| knowledge.store | 72 | live cmds: all 8; UI; tests: knowledge,knowledge_reads,native_engine | cb/knowledge/store.cpp +1 | YY-NN |

### codebreaker.platforms

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| platforms | 389 | live cmds: all 8; UI; tests: m68k,nes,platforms+4 | cb/platforms/(platforms/ package) | YY-NN |
| platforms.dos | 33 | live cmds: all 8 | cb/platforms/dos.cpp +1 | YN-NY |
| platforms.gb | 19 | live cmds: all 8 | cb/platforms/gb.cpp +1 | YN-NY |
| platforms.genesis | 19 | live cmds: all 8 | cb/platforms/genesis.cpp +1 | YN-NY |
| platforms.java | 25 | live cmds: all 8 | cb/platforms/java.cpp +1 | YN-NY |
| platforms.msx | 19 | live cmds: all 8 | cb/platforms/msx.cpp +1 | YN-NY |
| platforms.nes | 19 | live cmds: all 8 | cb/platforms/nes.cpp +1 | YN-NY |
| platforms.sms | 19 | live cmds: all 8 | cb/platforms/sms.cpp +1 | YN-NY |
| platforms.vxd | 43 | live cmds: all 8 | cb/platforms/vxd.cpp +1 | YY-NY |
| platforms.win16 | 72 | live cmds: all 8 | cb/platforms/win16.cpp +1 | YY-NY |
| platforms.win32 | 58 | live cmds: all 8 | none found | NN-NY |
| platforms.xbox | 22 | live cmds: all 8 | none found | NN-NY |
| platforms.xbox360 | 45 | live cmds: all 8 | none found | NN-NY |
| platforms.zx | 19 | live cmds: all 8 | none found | NN-NY |

### codebreaker.recompile

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| recompile | 17 | tests: python_frontend,recompile,x360; tools: py2cpp; native gens: 7 | cb/recompile/(recompile/ package) | YYGNY |
| recompile.builders | 0 | native gens: 2 | cb/recompile/builders/(recompile/builders/ package) | YYGYY |
| recompile.builders.arith | 269 | native gens: 1 | cb/recompile/builders/arith.cpp +1 | YYGYY |
| recompile.builders.floating | 172 | native gens: 2 | cb/recompile/builders/floating.cpp +1 | YYGYY |
| recompile.builders.logical | 286 | native gens: 1 | cb/recompile/builders/logical.cpp +1 | YYGYY |
| recompile.builders.memory | 492 | native gens: 1 | cb/recompile/builders/memory.cpp +1 | YYGYY |
| recompile.builders.system | 384 | native gens: 1 | cb/recompile/builders/system.cpp +1 | YYGYY |
| recompile.builders.vector | 989 | native gens: 1 | cb/recompile/builders/vector.cpp +1 | YYGYY |
| recompile.config | 260 | tests: x360; native gens: 1 | cb/recompile/config.cpp +1 | YYGYY |
| recompile.disassemble | 158 | tests: recompile; native gens: 2 | cb/recompile/disassemble.cpp +1 | YYGYY |
| recompile.discover | 181 | tests: recompile,x360; native gens: 3 | cb/recompile/discover.cpp +1 | YYGYY |
| recompile.gpu | 1044 | named in native text only | cb/recompile/gpu.cpp +1 | YY-YY |
| recompile.jumptable | 364 | native gens: 1 | cb/recompile/jumptable.cpp +1 | YYGYY |
| recompile.powerpc | 1168 | tests: recompile; native gens: 2 | cb/recompile/powerpc.cpp +1 | YYGYY |
| recompile.project | 876 | nothing | cb/recompile/project.cpp +1 | YY-YY |
| recompile.switches | 553 | tests: x360; native gens: 2 | cb/recompile/switches.cpp +1 | YYGYY |
| recompile.title | 148 | nothing | cb/recompile/title.cpp +1 | YY-YY |
| recompile.x360project | 198 | nothing | cb/recompile/x360project.cpp +1 | YY-YY |
| recompile.xenon | 243 | tests: python_frontend,x360; tools: py2cpp | cb/recompile/xenon.cpp +3 | YY-NY |

### codebreaker.source

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| source | 0 | live cmds: all 8; UI; tests: drivers,invert,kind+6; native gens: 1 | cb/source/(source/ package) | YYGNN |
| source.assembly | 232 | live cmds: all 8; UI | cb/source/assembly.cpp +1 | YY-NN |
| source.classes_writer | 162 | live cmds: all 8; UI | cb/source/classes_writer.cpp +1 | YY-NN |
| source.dos_build | 21 | nothing | cb/source/dos_build.cpp +1 | YN-YY |
| source.dos_writer | 1005 | live cmds: all 8 | cb/source/dos_writer.cpp +1 | YY-NY |
| source.driver_writer | 625 | live cmds: all 8; tests: drivers | cb/source/driver_writer.cpp +1 | YY-NY |
| source.gb_writer | 135 | live cmds: all 8; tests: z80 | cb/source/gb_writer.cpp +1 | YY-NY |
| source.genesis_writer | 148 | live cmds: all 8; tests: m68k | cb/source/genesis_writer.cpp +1 | YY-NY |
| source.image_writer | 345 | live cmds: all 8; tests: module_model | cb/source/image_writer.cpp +1 | YY-NY |
| source.java_writer | 60 | live cmds: all 8; UI | cb/source/java_writer.cpp +1 | YY-NN |
| source.le_writer | 518 | tests: module_model | cb/source/le_writer.cpp +1 | YY-YY |
| source.module_writer | 435 | live cmds: all 8; UI; tests: kind | cb/source/module_writer.cpp +1 | YY-NN |
| source.msx_writer | 43 | live cmds: all 8; tests: z80 | cb/source/msx_writer.cpp +1 | YY-NY |
| source.ne_asm | 0 | tests: invert,port_fixes; native gens: 1 | cb/source/ne_asm/(source/ne_asm/ package) | YYGYY |
| source.ne_asm.datacode | 126 | nothing | cb/source/ne_asm/datacode.cpp +1 | YY-YY |
| source.ne_asm.fixups | 133 | native gens: 1 | cb/source/ne_asm/fixups.cpp +1 | YYGYY |
| source.ne_asm.idiom | 530 | tests: invert | cb/source/ne_asm/idiom.cpp +1 | YY-YY |
| source.ne_asm.reachability | 175 | nothing | cb/source/ne_asm/reachability.cpp +1 | YY-YY |
| source.ne_asm.recon | 473 | nothing | cb/source/ne_asm/recon.cpp +1 | YY-YY |
| source.ne_asm.run | 44 | nothing | cb/source/ne_asm/run.cpp +1 | YY-YY |
| source.ne_asm.segdiff | 232 | tests: port_fixes | cb/source/ne_asm/segdiff.cpp +1 | YY-YY |
| source.ne_asm.segment | 703 | tests: invert; native gens: 1 | cb/source/ne_asm/segment.cpp +1 | YYGYY |
| source.ne_asm.sweep | 176 | nothing | cb/source/ne_asm/sweep.cpp +1 | YY-YY |
| source.ne_writer | 458 | tests: module_model | cb/source/ne_writer.cpp +1 | YY-YY |
| source.nes_writer | 160 | live cmds: all 8; tests: nes | cb/source/nes_writer.cpp +1 | YY-NY |
| source.pe_writer | 529 | tests: module_model,port_fixes | cb/source/pe_writer.cpp +1 | YY-YY |
| source.placement | 121 | live cmds: all 8; UI | cb/source/placement.cpp +1 | YY-NN |
| source.readable | 644 | live cmds: all 8; UI; tests: module_model,readable | cb/source/readable.cpp +1 | YY-NN |
| source.sms_writer | 133 | live cmds: all 8; tests: port_fixes,z80 | cb/source/sms_writer.cpp +1 | YY-NY |
| source.vxd | 0 | tests: drivers | cb/source/vxd/(source/vxd/ package) | YY-YY |
| source.vxd.boot | 338 | tests: drivers | cb/source/vxd/boot.cpp +1 | YY-YY |
| source.vxd.coalesce | 234 | nothing | cb/source/vxd/coalesce.cpp +1 | YY-YY |
| source.vxd.corpus | 125 | nothing | cb/source/vxd/corpus.cpp +1 | YY-YY |
| source.vxd.disassembly | 780 | tests: drivers | cb/source/vxd/disassembly.cpp +2 | YY-YY |
| source.vxd.from_source | 355 | tests: drivers | cb/source/vxd/from_source.cpp +1 | YY-YY |
| source.vxd.idiom | 1118 | tests: drivers | cb/source/vxd/idiom.cpp +1 | YY-YY |
| source.vxd.le_writer | 286 | nothing | cb/source/vxd/le_writer.cpp +1 | YY-YY |
| source.vxd.link | 534 | nothing | cb/source/vxd/link.cpp +1 | YY-YY |
| source.vxd.loadable_corpus | 114 | nothing | cb/source/vxd/loadable_corpus.cpp +1 | YN-YY |
| source.vxd.masm | 667 | nothing | cb/source/vxd/masm.cpp +1 | YY-YY |
| source.vxd.owbuild | 186 | nothing | cb/source/vxd/owbuild.cpp +1 | YY-YY |
| source.vxd.run | 45 | tests: drivers | cb/source/vxd/run.cpp +1 | YY-YY |
| source.vxd.services | 115 | nothing | cb/source/vxd/services.cpp +1 | YY-YY |
| source.vxd.symbols | 95 | nothing | cb/source/vxd/symbols.cpp +1 | YY-YY |
| source.vxd.tree | 194 | nothing | cb/source/vxd/tree.cpp +1 | YY-YY |
| source.z80_writer | 94 | live cmds: all 8 | cb/source/z80_writer.cpp +1 | YY-NY |
| source.zx_writer | 41 | live cmds: all 8; tests: z80 | cb/source/zx_writer.cpp +1 | YN-NY |

### codebreaker.store

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| store | 0 | live cmds: all 8; UI; tests: kind,knowledge,native_engine+6; tools: causes,failures,oss_bench; native gens: 1 | cb/store/(store/ package) | YYGNN |
| store.functions | 126 | live cmds: all 8; UI; tests: kind,knowledge,native_engine+5; tools: causes,failures,oss_bench; native gens: 1 | cb/store/functions.cpp +1 | YYGNN |

### codebreaker.symbols

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| symbols | 0 | live cmds: all 8; UI; tests: dos,drivers,invert+4; tools: struct_report; native gens: 1 | cb/symbols/(symbols/ package) | YYGNN |
| symbols.api_arguments | 434 | live cmds: all 8; UI; tests: knowledge_reads; native gens: 1 | cb/symbols/api_arguments.cpp +1 | YYGNN |
| symbols.debug_build | 168 | tests: invert | cb/symbols/debug_build.cpp +3 | YY-YY |
| symbols.debug_build_estimate | 196 | nothing | cb/symbols/debug_build.cpp +3 | YY-YY |
| symbols.demangle | 107 | live cmds: all 8; UI; tests: symbols | cb/symbols/demangle.cpp +1 | YY-NN |
| symbols.dos_interrupts | 101 | live cmds: all 8; UI; tests: dos | cb/symbols/dos_interrupts.cpp +1 | YY-NN |
| symbols.driver_services | 280 | live cmds: all 8; UI; tests: drivers | cb/symbols/driver_services.cpp +2 | YY-NN |
| symbols.libraries | 104 | live cmds: all 8; UI; tests: symbols,verification | cb/symbols/libraries/(symbols/libraries/ package) | YY-NN |
| symbols.libraries.coff | 166 | live cmds: all 8; UI; tests: symbols | cb/symbols/libraries/coff.cpp +1 | YY-NN |
| symbols.libraries.omf | 152 | live cmds: all 8; UI | cb/symbols/libraries/omf.cpp +1 | YY-NN |
| symbols.names | 547 | live cmds: all 8; UI; tests: symbols | cb/symbols/names.cpp +4 | YY-NN |
| symbols.native | 70 | live cmds: all 8; UI | none found | NN-NN |
| symbols.ordinals | 377 | live cmds: all 8; UI; tests: symbols,xbe | cb/symbols/ordinals.cpp +1 | YY-NN |
| symbols.prototypes | 124 | live cmds: all 8; UI; tests: knowledge_reads,symbols; tools: struct_report | cb/symbols/prototypes.cpp +1 | YY-NN |
| symbols.runtime_library | 386 | live cmds: all 8; UI; tests: symbols | cb/symbols/runtime_library.cpp +1 | YY-NN |
| symbols.sdk_structs | 265 | live cmds: all 8; UI; tools: struct_report | cb/symbols/sdk_structs.cpp +1 | YY-NN |
| symbols.wine16 | 89 | live cmds: all 8; UI; tests: symbols,verification | cb/symbols/wine16.cpp +1 | YY-NN |
| symbols.xdk_libraries | 151 | live cmds: all 8; UI | cb/symbols/xdk_libraries.cpp +1 | YY-NN |

### codebreaker.translate

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| translate | 6 | live cmds: all 8; UI; tests: c_backend,clang_frontend,console_patches+21; tools: judge_c_java,l2c,l2check; native gens: 3 | cb/translate/(translate/ package) | YYGNN |
| translate.backends | 1 | live cmds: all 8; UI; tests: c_backend,console_patches,cpp03_bodies+10; tools: l2c,py2cpp; native gens: 3 | cb/translate/backends/(translate/backends/ package) | YYGNN |
| translate.backends.asm | 71 | live cmds: all 8; UI; tests: drivers,module_model | cb/translate/backends/asm.cpp +1 | YY-NN |
| translate.backends.c | 96 | live cmds: all 8; UI; tests: c_backend,frame16,l2_survives; tools: l2c | cb/translate/backends/c.cpp +31 | YY-NN |
| translate.backends.c_java | 1792 | tests: java_as_c | cb/translate/backends/c_java.cpp +9 | YY-YY |
| translate.backends.cpp03 | 1857 | live cmds: all 8; UI; tests: console_patches,cpp03_bodies,projects | cb/translate/backends/cpp03.cpp +13 | YY-NN |
| translate.backends.cpp17 | 71 | live cmds: all 8; UI; tests: cpp17_backend,python_frontend; tools: py2cpp | cb/translate/backends/cpp17.cpp +1 | YY-NN |
| translate.backends.csharp | 1566 | tests: csharp_kit; native gens: 1 | cb/translate/backends/csharp.cpp +3 | YYGYY |
| translate.backends.csharp_java | 631 | nothing | cb/translate/backends/csharp.cpp +3 | YY-YY |
| translate.backends.java | 1978 | tests: java_backend; native gens: 2 | cb/translate/backends/java.cpp +6 | YYGYY |
| translate.backends.java_libc | 80 | closure (lazy imports of live modules) | cb/translate/backends/java_libc.cpp +1 | YY-YY |
| translate.backends.java_objects | 1036 | closure (lazy imports of live modules) | cb/translate/backends/java_objects.cpp +1 | YY-YY |
| translate.backends.jmodel | 511 | tests: cpp17_backend | cb/translate/backends/jmodel.cpp +1 | YY-YY |
| translate.backends.module_file | 127 | live cmds: all 8; UI; tests: module_model | cb/translate/backends/module_file.cpp +1 | YY-NN |
| translate.desugar | 453 | live cmds: all 8; UI; tests: cpp17_backend,desugar | cb/translate/desugar.cpp +1 | YY-NN |
| translate.dotnet | 149 | tests: csharp_kit,dotnet_toolchain; native gens: 1 | cb/translate/dotnet/(translate/dotnet/ package) | YYGYY |
| translate.dotnet.cs2cpp | 844 | tests: csharp_kit | cb/translate/dotnet/cs2cpp.cpp +6 | YY-YY |
| translate.dotnet.judge | 149 | tests: csharp_kit | cb/translate/dotnet/judge.cpp +1 | YY-YY |
| translate.dotnet.pairs | 114 | tests: csharp_kit; native gens: 1 | none found | NNGYY |
| translate.frontends | 2 | live cmds: all 8; UI; tests: clang_frontend,csharp_kit,csharp_lower_trees+6; tools: py2cpp; native gens: 2 | cb/translate/frontends/(translate/frontends/ package) | YNGNN |
| translate.frontends.clang | 129 | tests: clang_frontend,java_backend; native gens: 2 | cb/translate/frontends/clang/(translate/frontends/clang/ package) | YYGYY |
| translate.frontends.clang.ctypes | 387 | tests: clang_frontend | cb/translate/frontends/clang/ctypes.cpp +1 | YN-YY |
| translate.frontends.clang.j2x | 302 | closure (lazy imports of live modules) | none found | NN-YY |
| translate.frontends.clang.lower | 1505 | closure (lazy imports of live modules) | cb/translate/frontends/clang/lower.hpp +6 | YY-YY |
| translate.frontends.csharp | 142 | tests: csharp_kit,csharp_lower_trees | cb/translate/frontends/csharp/(translate/frontends/csharp/ package) | YY-YY |
| translate.frontends.csharp.lower | 1233 | tests: csharp_lower_trees | cb/translate/frontends/csharp/lower.cpp +1 | YY-YY |
| translate.frontends.java | 108 | live cmds: all 8; UI; tests: java_frontend,java_lower,l2 | none found | NN-NN |
| translate.frontends.java.lower | 543 | live cmds: all 8; UI; tests: java_lower,l2 | none found | NN-NN |
| translate.frontends.python | 319 | tests: python_frontend,pytypes; tools: py2cpp | cb/translate/frontends/python/(translate/frontends/python/ package) | YY-NY |
| translate.frontends.python.infer | 1955 | closure (lazy imports of live modules) | cb/translate/frontends/python/infer.hpp +9 | YY-YY |
| translate.frontends.python.lower | 1816 | closure (lazy imports of live modules) | cb/translate/frontends/python/lower.cpp +10 | YY-YY |
| translate.frontends.python.methods | 749 | closure (lazy imports of live modules) | cb/translate/frontends/python/methods.cpp +1 | YY-YY |
| translate.frontends.python.program | 874 | closure (lazy imports of live modules) | cb/translate/frontends/python/program.cpp +1 | YY-YY |
| translate.frontends.python.pytypes | 308 | tests: pytypes | cb/translate/frontends/python/pytypes.cpp +1 | YY-YY |
| translate.frontends.python.structfmt | 55 | tests: pytypes | cb/translate/frontends/python/structfmt.cpp +1 | YY-YY |
| translate.ir | 320 | live cmds: all 8; UI; tests: clang_frontend,console_patches,cpp17_backend+5 | cb/translate/ir.cpp +1 | YY-NN |
| translate.javajudge | 307 | tests: cpp17_backend,java_as_c | cb/translate/javajudge.cpp +3 | YY-YY |
| translate.judge_native_jvm | 263 | tests: java_backend; tools: judge_c_java | cb/translate/judge_native_jvm.cpp +1 | YY-NY |
| translate.l2 | 431 | live cmds: all 8; UI; tests: c_backend,clang_frontend,csharp_kit+10; tools: l2check; native gens: 3 | cb/translate/l2.cpp +1 | YYGNN |
| translate.native_l2 | 340 | live cmds: all 8; UI; tests: native_l2_core | cb/translate/native_l2.cpp +1 | YY-NN |
| translate.rewrite | 114 | closure (lazy imports of live modules) | cb/translate/rewrite.cpp +1 | YY-YY |

### codebreaker.ui

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| ui | 0 | live cmds: facts; UI; tests: mcp,service,ui+2 | cb/ui/(ui/ package) | YY-NY |
| ui.client | 294 | live cmds: facts; UI; tests: service,ui,ui_thin | none found | NN-NY |
| ui.mcp_server | 1271 | live cmds: facts; tests: mcp | cb/ui/mcp_server.hpp | YY-NY |
| ui.progress | 137 | live cmds: progress,facts; tests: ui | cb/ui/progress.cpp +3 | YY-NY |
| ui.service | 508 | live cmds: facts; tests: ui | cb/ui/service.cpp +3 | YY-NY |
| ui.terminal | 983 | live cmds: facts; UI; tests: ui,ui_thin,verdicts | none found | NN-NY |

### codebreaker.verify

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| verify | 12 | live cmds: all 8; UI; tests: n_subjects,authored_rewrites,bytes+39; tools: causes,copy_walk,failures; native gens: 4 | cb/verify/(verify/ package) | YYGNN |
| verify.authored | 2 | tests: authored_rewrites,c_backend,match_powerpc+3 | cb/verify/authored/(verify/authored/ package) | YY-YN |
| verify.authored.match | 674 | tests: c_backend,match_powerpc,recompile+2 | cb/verify/authored/match16.cpp +1 | YY-YN |
| verify.authored.match16 | 281 | tests: verification | cb/verify/authored/match16.cpp +1 | YY-YN |
| verify.authored.repair | 587 | tests: verification | cb/verify/authored/repair.cpp +1 | YY-YY |
| verify.authored.search16 | 212 | tests: authored_rewrites,verification | cb/verify/authored/search16.cpp +1 | YY-YN |
| verify.authored.shape | 210 | tests: authored_rewrites | cb/verify/authored/shape.cpp +1 | YY-YY |
| verify.compare | 228 | live cmds: all 8; UI; tests: plumbing,verdicts,verification | cb/verify/compare.cpp +1 | YY-NN |
| verify.concolic | 229 | live cmds: all 8; UI; tests: concolic | cb/verify/concolic.cpp +1 | YY-NN |
| verify.gate16 | 2 | live cmds: all 8; UI; tests: verification; tools: copy_walk,gate_teeth16,record; native gens: 1 | cb/verify/gate16/(verify/gate16/ package) | YYGNN |
| verify.gate16.contract16 | 385 | live cmds: all 8; UI; tests: verification | cb/verify/gate16/contract16.cpp +1 | YY-NN |
| verify.gate16.isolated16 | 470 | live cmds: all 8; UI; tools: gate_teeth16,record,replay; native gens: 1 | cb/verify/gate16/isolated16.cpp +1 | YYGNN |
| verify.gate16.runtime16 | 62 | tests: verification | none found | NN-YN |
| verify.gate16.scoreboard16 | 203 | nothing | cb/verify/gate16/scoreboard16.cpp +1 | YY-YN |
| verify.gate16.setup16 | 255 | tests: verification; tools: copy_walk,record | cb/verify/gate16/setup16.cpp +1 | YY-NN |
| verify.guided | 222 | live cmds: all 8; UI | cb/verify/guided.cpp +1 | YY-NN |
| verify.lane_common | 264 | live cmds: all 8; UI; tests: run_record,verdicts,verification | cb/verify/lane_common.cpp +1 | YY-NN |
| verify.lanes | 4 | live cmds: all 8; UI; tests: n_subjects,bytes,concolic+31; tools: gate_teeth16,trace16; native gens: 3 | cb/verify/lanes/(verify/lanes/ package) | YYGNN |
| verify.lanes.dos | 75 | live cmds: all 8; tests: platforms,verification | cb/verify/lanes/dos.cpp +1 | YY-NY |
| verify.lanes.driver | 567 | live cmds: all 8; tests: drivers,platforms,run_record; native gens: 1 | cb/verify/lanes/driver.cpp +1 | YYGNY |
| verify.lanes.gb | 41 | tests: platforms | none found | NN-YY |
| verify.lanes.genesis | 40 | tests: platforms | none found | NN-YY |
| verify.lanes.image_lane | 108 | tests: verification | cb/verify/lanes/image_lane.cpp +1 | YY-YY |
| verify.lanes.java | 225 | live cmds: all 8; UI; tests: bytes,console_patches,cpp03_bodies+23 | cb/verify/lanes/java.cpp +1 | YY-NN |
| verify.lanes.msx | 6 | tests: platforms | cb/verify/lanes/msx.hpp | YN-YY |
| verify.lanes.nes | 38 | tests: nes,platforms | none found | NN-YY |
| verify.lanes.powerpc | 393 | live cmds: all 8; tests: platforms | cb/verify/lanes/powerpc.cpp +1 | YY-NN |
| verify.lanes.sms | 39 | tests: platforms | none found | NN-YY |
| verify.lanes.win16 | 727 | live cmds: all 8; UI; tests: platforms,run_record,verdicts+1; tools: gate_teeth16,trace16; native gens: 2 | cb/verify/lanes/win16.cpp +1 | YYGNN |
| verify.lanes.win32 | 917 | live cmds: all 8; UI; tests: n_subjects,concolic,native_trace+4 | cb/verify/lanes/win32.cpp +5 | YY-NN |
| verify.lanes.z80_flat | 52 | closure (lazy imports of live modules) | none found | NN-YY |
| verify.lanes.zx | 6 | tests: platforms | cb/verify/lanes/zx.hpp | YN-YY |
| verify.reconstruct | 305 | live cmds: all 8; UI; tests: kind,platforms,verdicts+2; tools: causes,failures,oss_bench; native gens: 1 | cb/verify/reconstruct.cpp +1 | YYGNN |
| verify.teeth | 256 | live cmds: all 8; tests: verification; tools: gate_teeth16 | cb/verify/teeth.cpp +1 | YY-NY |
| verify.verdicts | 215 | live cmds: all 8; UI; tests: native_engine,verdicts,verification; tools: gate_teeth16,replay,verdict_diff | cb/verify/verdicts.cpp +1 | YY-NN |

### codebreaker.xna

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| xna | 3 | tests: xbox_effect,xna | cb/xna/(xna/ package) | YY-YY |
| xna.fixups | 170 | tests: xna | cb/xna/fixups.cpp +1 | YY-YY |
| xna.mgfx | 162 | tests: xbox_effect | cb/xna/mgfx.cpp +1 | YY-YY |
| xna.port | 811 | tests: xna | cb/xna/port.cpp +1 | YY-YY |
| xna.run | 212 | tests: xna | cb/xna/run.cpp +1 | YY-YY |

