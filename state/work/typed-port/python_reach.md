# Python reach inventory: who still reaches each codebreaker/ module

Made 2026-11-13 on master `ede5a3636` merged with the thin-client UI branch (`worktree-agent-a82ead8c3a979e521`). Every row is
computed (state/work/typed-port/python_reach/graph.py and report.py: AST import graph of every .py, lazy imports and dotted-name
strings included, plus tests/, tools/ and native/ generators); the batch-1 verdicts at the end were checked by hand.

553 modules / 169527 lines under codebreaker/ (ui is listed too, it is the kept terminal).

## How to read it

**Reached by** (what keeps a module alive today):
- `live cmds`: reachable (static imports, lazy imports, dotted-name strings, the platforms registry) from the module of one of the 17
  commands that still run Python in the exe: names knowledge coverage progress facts origin driver-services disc media gfx fonts
  xex-edit xex-imports ppc-calls system-ui build16 boot-floppy. `all N` means more than three of them.
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

Note on the live-command list: it is the list this task named, on the base `ede5a3636`. Master has since moved nine of those 17 to native/cb
(`1b18fc591`: xex-imports, xex-edit, ppc-calls, system-ui, disc, media, gfx, fonts, build16), so for the rows that only those commands reach, the real
reach is smaller than shown; the next inventory run on master will show it.

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

## Totals by package

| package | modules | lines | reached by a live cmd | UI | tests only | tools too | nothing/native-text only | YYGYY |
|---|---|---|---|---|---|---|---|---|
| (root) | 1 | 23 | 1 | 1 | 0 | 0 | 0 | 0 |
| codebreaker.__main__ | 1 | 220 | 1 | 0 | 0 | 0 | 0 | 0 |
| codebreaker.analyze | 17 | 4642 | 13 | 12 | 4 | 0 | 0 | 2 |
| codebreaker.base | 10 | 2000 | 7 | 6 | 1 | 1 | 1 | 0 |
| codebreaker.build | 30 | 11035 | 28 | 16 | 2 | 0 | 0 | 0 |
| codebreaker.compare | 3 | 961 | 3 | 2 | 0 | 0 | 0 | 0 |
| codebreaker.coverage | 3 | 716 | 3 | 0 | 0 | 0 | 0 | 0 |
| codebreaker.decompile | 19 | 9165 | 15 | 11 | 3 | 1 | 0 | 0 |
| codebreaker.emulate | 19 | 7394 | 19 | 13 | 0 | 0 | 0 | 0 |
| codebreaker.engine | 7 | 2638 | 7 | 5 | 0 | 0 | 0 | 0 |
| codebreaker.exact | 50 | 16218 | 6 | 6 | 8 | 3 | 33 | 7 |
| codebreaker.formats | 91 | 18971 | 45 | 13 | 15 | 0 | 31 | 3 |
| codebreaker.invert | 28 | 2496 | 0 | 0 | 13 | 0 | 15 | 0 |
| codebreaker.java | 67 | 18271 | 34 | 34 | 19 | 0 | 14 | 13 |
| codebreaker.jobs | 2 | 287 | 2 | 2 | 0 | 0 | 0 | 0 |
| codebreaker.knowledge | 5 | 584 | 5 | 4 | 0 | 0 | 0 | 0 |
| codebreaker.platforms | 14 | 801 | 14 | 1 | 0 | 0 | 0 | 0 |
| codebreaker.port | 9 | 10494 | 0 | 0 | 4 | 0 | 5 | 3 |
| codebreaker.recompile | 19 | 7802 | 3 | 0 | 3 | 1 | 12 | 11 |
| codebreaker.source | 47 | 13687 | 17 | 7 | 13 | 0 | 17 | 3 |
| codebreaker.store | 2 | 126 | 2 | 2 | 0 | 0 | 0 | 0 |
| codebreaker.symbols | 19 | 3929 | 16 | 16 | 1 | 0 | 2 | 0 |
| codebreaker.translate | 41 | 23733 | 14 | 14 | 15 | 2 | 10 | 4 |
| codebreaker.ui | 7 | 3526 | 6 | 3 | 0 | 0 | 1 | 0 |
| codebreaker.verify | 37 | 8451 | 18 | 14 | 14 | 1 | 4 | 0 |
| codebreaker.xna | 5 | 1357 | 0 | 0 | 5 | 0 | 0 | 0 |

## Every module

### (root)

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| (package) | 23 | live cmds: all 17; UI; tests: n_subjects,ll,+136; tools: carve,causes,copy_walk; native gens: 38 | none found | NNGNN |

### codebreaker.__main__

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| __main__ | 220 | live cmds: facts; tools: carve,unreachable | none found | NN-NY |

### codebreaker.analyze

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| analyze | 23 | live cmds: all 17; UI; tests: analysis,c_backend,dos+2; native gens: 5 | cb/analyze/(analyze/ package) | YYGNN |
| analyze.call_graph | 110 | live cmds: all 17; UI; tests: analysis; native gens: 1 | cb/analyze/call_graph.cpp +1 | YYGNN |
| analyze.classes | 533 | live cmds: all 17; UI; tests: analysis | cb/analyze/classes.cpp +1 | YY-NN |
| analyze.conventions | 136 | live cmds: all 17; UI; tests: analysis; native gens: 1 | cb/analyze/conventions.cpp +1 | YYGNN |
| analyze.dispatch | 404 | live cmds: all 17; UI; tests: analysis | cb/analyze/dispatch.cpp +1 | YY-NN |
| analyze.dos_functions | 516 | live cmds: all 17; UI; tests: dos | cb/analyze/dos_functions.cpp +1 | YY-NN |
| analyze.facts | 91 | tests: analysis | cb/analyze/facts.cpp +1 | YY-YY |
| analyze.globals | 105 | live cmds: all 17; UI; tests: analysis; native gens: 1 | cb/analyze/globals.cpp +1 | YYGNN |
| analyze.image | 524 | live cmds: all 17; UI; tests: analysis,c_backend; native gens: 4 | cb/analyze/image.cpp +1 | YYGNN |
| analyze.native | 157 | live cmds: all 17; UI; native gens: 1 | cb/analyze/native.cpp +1 | YYGNN |
| analyze.observe | 360 | tests: analysis; native gens: 1 | cb/analyze/observe.cpp +2 | YYGYY |
| analyze.origin | 489 | live cmds: all 17; UI; tests: port_fixes | cb/analyze/origin.cpp +1 | YY-NN |
| analyze.ppc_calls | 268 | live cmds: facts,ppc-calls; tests: ppc_calls | cb/analyze/ppc_calls.cpp +2 | YY-NY |
| analyze.report | 214 | tests: analysis | cb/analyze/report.cpp +4 | YY-YY |
| analyze.resources | 100 | tests: analysis; native gens: 1 | cb/analyze/resources.cpp +1 | YYGYY |
| analyze.strings | 191 | live cmds: all 17; UI; tests: analysis; native gens: 3 | cb/analyze/strings.cpp +1 | YYGNN |
| analyze.structures | 421 | live cmds: all 17; UI; tests: analysis,c_backend | cb/analyze/structures.cpp +1 | YY-NN |

### codebreaker.base

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| base | 0 | live cmds: all 17; UI; tests: n_subjects,ll,analysis+24; tools: copy_walk,failures,freeze_tests; native gens: 11 | cb/base/(base/ package) | YYGNN |
| base.cache | 591 | live cmds: all 17; UI; tests: plumbing,verification | cb/base/cache.cpp +4 | YY-NN |
| base.commands | 33 | nothing | cb/base/commands.cpp +1 | YY-YY |
| base.converted | 188 | live cmds: all 17; UI; tests: converted | cb/base/converted.cpp +1 | YY-NN |
| base.corpus | 295 | live cmds: all 17; UI; tests: n_subjects,analysis,concolic+18; tools: failures,freeze_tests,l2c; native gens: 3 | cb/base/corpus.cpp +3 | YYGNN |
| base.ctext | 51 | live cmds: all 17; native gens: 1 | cb/base/ctext.cpp +1 | YYGNY |
| base.parallel | 268 | live cmds: all 17; UI; tests: ll,plumbing,verification | cb/base/parallel.cpp +1 | YY-NN |
| base.runs | 82 | tests: plumbing; tools: copy_walk,record | cb/base/runs.cpp +1 | YY-NN |
| base.settings | 286 | live cmds: all 17; UI; tests: assembler,csharp_kit,dos+6; tools: copy_walk,golden,judge_c_java; native gens: 9 | cb/base/settings.cpp +1 | YYGNN |
| base.tidy_state | 206 | tests: plumbing | cb/base/tidy_state.cpp +3 | YY-YY |

### codebreaker.build

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| build | 0 | live cmds: all 17; UI; tests: ,c_backend,dos+21; tools: make_api_table,oss_bench,trace16 | cb/build/(build/ package) | YN-NN |
| build.asm16 | 689 | live cmds: all 17; UI; tests: verification | none found | NN-NN |
| build.data | 530 | live cmds: all 17; UI; tests: whole_module | none found | NN-NN |
| build.definitions | 208 | live cmds: all 17; UI; tests: whole_module | none found | NN-NN |
| build.disc_image | 289 | live cmds: facts,disc; tests: media | none found | NN-NY |
| build.dos_sessions | 185 | live cmds: all 17; UI | none found | NN-NN |
| build.dos_tools | 83 | live cmds: all 17; UI; tests: dos,drivers,invert | none found | NN-NN |
| build.driver | 332 | live cmds: all 17; UI; tests: drivers | none found | NN-NN |
| build.exepack | 107 | live cmds: all 17; UI; tests: exepack | none found | NN-NN |
| build.flat_thunk | 1085 | live cmds: all 17; UI; tests: flat_thunk,whole_module | none found | NN-NN |
| build.floppy | 219 | live cmds: facts,boot-floppy | none found | NN-NY |
| build.functions | 678 | live cmds: all 17; UI; tests: engine,match_powerpc,platforms+4 | none found | NN-NN |
| build.m6502_asm | 147 | live cmds: all 17; tests: nes | none found | NN-NY |
| build.m68k_asm | 123 | live cmds: all 17; tests: m68k | none found | NN-NY |
| build.masm | 442 | live cmds: all 17; UI; tests: dos | none found | NN-NN |
| build.module | 2162 | live cmds: all 17; UI; tests: c_backend,platforms,whole_module | none found | NN-NN |
| build.msc6 | 935 | live cmds: all 17; UI; tests: ,dos,verification+1 | none found | NN-NN |
| build.msvc152 | 79 | tests: port_fixes | none found | NN-YY |
| build.mzfile | 50 | live cmds: all 17 | none found | NN-NY |
| build.native_dos | 207 | live cmds: all 17; UI; tests: dos | none found | NN-NN |
| build.period | 250 | tests: verification | none found | NN-YY |
| build.period16 | 205 | live cmds: facts,build16; tests: whole_module | none found | NN-NY |
| build.pklite | 380 | live cmds: all 17; tests: pklite | none found | NN-NY |
| build.ppc_asm | 246 | live cmds: facts,xex-edit; tests: xex_edit | none found | NN-NY |
| build.resources | 658 | live cmds: all 17; UI; tests: whole_module | none found | NN-NN |
| build.sm83_asm | 117 | live cmds: all 17; tests: z80 | none found | NN-NY |
| build.stubjoin | 122 | live cmds: all 17 | none found | NN-NY |
| build.vxdfile | 63 | live cmds: all 17 | none found | NN-NY |
| build.watcom | 160 | live cmds: all 17; UI; tests: ,plumbing,readable+3; tools: make_api_table,oss_bench,trace16 | none found | NN-NN |
| build.z80_asm | 284 | live cmds: all 17; tests: port_fixes,z80 | none found | NN-NY |

### codebreaker.compare

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| compare | 0 | live cmds: all 17; UI; tests: symbols,versions | cb/compare/(compare/ package) | YY-NN |
| compare.mask | 90 | live cmds: all 17; UI; tests: symbols | cb/compare/mask.cpp +1 | YY-NN |
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
| decompile | 0 | live cmds: all 17; UI; tests: n_subjects,asm_blocks,c_backend+16; tools: freeze_tests,golden,goto_report; native gens: 1 | cb/decompile/(decompile/ package) | YYGNN |
| decompile.asm_blocks | 267 | live cmds: all 17; UI; tests: asm_blocks,c_backend | cb/decompile/asm_blocks.cpp +1 | YY-NN |
| decompile.c_text | 1428 | live cmds: all 17; UI; tests: n_subjects,c_backend,frozen+1; tools: freeze_tests | cb/decompile/c_text.hpp +5 | YY-NN |
| decompile.ctree | 241 | tools: goto_report,oss_bench | cb/decompile/ctree.cpp +1 | YY-NY |
| decompile.decompiler | 898 | live cmds: all 17; UI; tests: c_backend,dos,frozen+6; tools: freeze_tests,golden,goto_report | cb/decompile/decompiler.cpp +5 | YY-NN |
| decompile.describe | 2178 | live cmds: all 17; UI; tests: n_subjects,dos,platforms+2; tools: golden,trace16 | cb/decompile/describe.cpp +6 | YY-NN |
| decompile.frame16 | 236 | live cmds: all 17; UI; tests: frame16 | cb/decompile/frame16.cpp +1 | YY-NN |
| decompile.inline_asm | 191 | live cmds: all 17; UI | cb/decompile/inline_asm.cpp +1 | YY-NN |
| decompile.jar | 188 | live cmds: all 17; UI | cb/decompile/jar.cpp +1 | YY-NN |
| decompile.m6502 | 181 | live cmds: all 17; UI; tests: nes; tools: sleigh_verify_6502 | cb/decompile/m6502.cpp +1 | YY-NN |
| decompile.m68k | 751 | live cmds: all 17; tests: m68k | cb/decompile/m68k.cpp +1 | YY-NY |
| decompile.shape16 | 111 | live cmds: all 17; UI; tests: frame16 | cb/decompile/shape16.cpp +1 | YY-NN |
| decompile.sm83 | 270 | live cmds: all 17; tests: z80 | cb/decompile/sm83.cpp +1 | YY-NY |
| decompile.struct_text | 114 | live cmds: all 17; UI; tests: c_backend | cb/decompile/struct_text.cpp +1 | YY-NN |
| decompile.vmx128 | 374 | live cmds: facts,ppc-calls; tests: vmx128; native gens: 1 | cb/decompile/vmx128.cpp +1 | YYGNY |
| decompile.xbox_effect | 217 | tests: xbox_effect | cb/decompile/xbox_effect.cpp +1 | YY-YY |
| decompile.xenos | 131 | tests: xenos | cb/decompile/xenos.cpp +3 | YY-YY |
| decompile.xenos_hlsl | 918 | tests: xbox_effect | cb/decompile/xenos.cpp +3 | YY-YY |
| decompile.z80 | 471 | live cmds: all 17; tests: z80 | cb/decompile/z80.cpp +1 | YY-NY |

### codebreaker.emulate

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| emulate | 45 | live cmds: all 17; UI; tests: concolic,knowledge,m68k+8; tools: gate_teeth16,sleigh_verify_6502,sleigh_verify_ppc; native gens: 3 | cb/emulate/(emulate/ package) | YYGNN |
| emulate.driver | 217 | live cmds: all 17; tests: knowledge,native_trace,platforms; native gens: 1 | cb/emulate/driver.cpp +1 | YYGNY |
| emulate.emulator | 616 | live cmds: all 17; UI; tests: concolic,native_trace,platforms+1; native gens: 2 | cb/emulate/emulator.cpp +1 | YYGNN |
| emulate.fast_uc | 69 | live cmds: all 17; UI; tests: plumbing | none found | NN-NN |
| emulate.m6502 | 314 | live cmds: all 17; tests: nes,platforms,run_record; tools: sleigh_verify_6502 | cb/emulate/m6502.cpp +1 | YY-NY |
| emulate.m68k | 715 | live cmds: all 17; tests: m68k,platforms,run_record | cb/emulate/m68k.cpp +1 | YY-NY |
| emulate.native_driver | 204 | live cmds: all 17 | cb/emulate/native_driver.cpp +1 | YY-NY |
| emulate.native_shadow | 239 | live cmds: all 17; UI | cb/emulate/native_shadow.cpp +1 | YY-NN |
| emulate.native_trace | 390 | live cmds: all 17; UI; tests: native_trace | cb/emulate/native_trace.cpp +1 | YY-NN |
| emulate.native_win16 | 209 | live cmds: all 17; UI | cb/emulate/native_win16.cpp +1 | YY-NN |
| emulate.pcode | 147 | live cmds: all 17; UI; tests: concolic,native_trace | cb/emulate/pcode.cpp +1 | YY-NN |
| emulate.ports16 | 195 | live cmds: all 17; UI; tests: verification | cb/emulate/ports16.cpp +1 | YY-NN |
| emulate.powerpc | 916 | live cmds: all 17; UI; tests: concolic,platforms,powerpc; tools: sleigh_verify_ppc | cb/emulate/powerpc.cpp +1 | YY-NN |
| emulate.run | 76 | live cmds: all 17; UI; tests: run_record | cb/emulate/run.cpp +1 | YY-NN |
| emulate.shadow | 681 | live cmds: all 17; UI; tests: concolic,native_trace | cb/emulate/shadow.cpp +1 | YY-NN |
| emulate.sm83 | 355 | live cmds: all 17; tests: platforms,run_record,z80 | cb/emulate/sm83.cpp +1 | YY-NY |
| emulate.symbolic | 193 | live cmds: all 17; UI; tests: concolic | cb/emulate/symbolic.cpp +1 | YY-NN |
| emulate.windows16 | 1161 | live cmds: all 17; UI; tests: concolic,knowledge,native_trace+3; tools: gate_teeth16,trace16; native gens: 1 | cb/emulate/windows16.cpp +1 | YYGNN |
| emulate.z80 | 652 | live cmds: all 17; tests: platforms,run_record,z80 | cb/emulate/z80.cpp +1 | YY-NY |

### codebreaker.engine

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| engine | 17 | live cmds: all 17; UI; tests: engine,knowledge,knowledge_reads+4; native gens: 2 | cb/engine/engine.cpp +1 | YYGNN |
| engine.engine | 268 | live cmds: all 17; UI; tests: engine,knowledge_reads,native_engine; native gens: 2 | cb/engine/engine.cpp +1 | YYGNN |
| engine.facts | 182 | live cmds: all 17; UI; tests: engine,knowledge,native_engine+1 | cb/engine/facts.cpp +1 | YY-NN |
| engine.lanes | 443 | live cmds: all 17; UI; tests: engine,platforms | cb/engine/lanes.cpp +3 | YY-NN |
| engine.native_engine | 1048 | live cmds: all 17; UI; tests: native_engine | none found | NN-NN |
| engine.show | 112 | live cmds: facts | cb/engine/show.cpp +1 | YY-NY |
| engine.win16 | 568 | live cmds: all 17; tests: platforms; native gens: 1 | cb/engine/win16.cpp +1 | YYGNY |

### codebreaker.exact

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| exact | 0 | live cmds: all 17; UI; tests: cmpp,dos,dpmi+3; tools: sample_shapes_cases; native gens: 2 | cb/exact/(exact/ package) | YYGNN |
| exact.aim | 135 | tests: exact_fixes | cb/exact/aim.cpp +1 | YY-YY |
| exact.aout | 82 | live cmds: all 17; UI | cb/exact/aout.cpp +1 | YY-NN |
| exact.batch | 115 | nothing | cb/exact/batch.cpp +1 | YY-YY |
| exact.bench | 56 | nothing | cb/exact/bench.cpp +1 | YY-YY |
| exact.bss | 120 | native gens: 1 | cb/exact/bss.cpp +1 | YYGYY |
| exact.carve | 156 | native gens: 1 | cb/exact/carve.cpp +1 | YYGYY |
| exact.compare | 185 | nothing | cb/exact/compare.cpp +1 | YY-YY |
| exact.data | 517 | tests: port_fixes | cb/exact/data.cpp +1 | YY-YY |
| exact.draft | 299 | tests: port_fixes; tools: sample_shapes_cases | cb/exact/draft/(exact/draft/ package) | YY-NY |
| exact.draft.candidates | 1391 | tests: port_fixes | cb/exact/draft/candidates.cpp +5 | YY-YY |
| exact.draft.forms | 1814 | tests: port_fixes | cb/exact/draft/forms.cpp +3 | YY-YY |
| exact.draft.prove | 561 | nothing | cb/exact/draft/prove.cpp +1 | YY-YY |
| exact.draft.shapes | 2211 | tests: port_fixes; tools: sample_shapes_cases | cb/exact/draft/shapes.hpp +8 | YY-NY |
| exact.draft.text | 345 | tests: port_fixes; tools: sample_shapes_cases | cb/exact/draft/text.cpp +1 | YY-NY |
| exact.emu | 0 | live cmds: all 17; UI; tests: dos,dpmi,port_fixes | cb/exact/emu/(exact/emu/ package) | YY-NN |
| exact.emu.dos | 803 | live cmds: all 17; UI; tests: dos | cb/exact/emu/dos.cpp +1 | YY-NN |
| exact.emu.dpmi | 736 | live cmds: all 17; UI; tests: dpmi | cb/exact/emu/dpmi.cpp +1 | YY-NN |
| exact.emu.isis | 294 | nothing | cb/exact/emu/isis.cpp +1 | YY-YY |
| exact.emu.native_dosexec | 163 | live cmds: all 17; UI; tests: port_fixes | cb/exact/emu/native_dosexec.cpp +1 | YY-NN |
| exact.emu.xenix | 175 | nothing | cb/exact/emu/xenix.cpp +1 | YY-YY |
| exact.exepack | 44 | nothing | cb/exact/exepack.cpp +1 | YY-YY |
| exact.habits | 7 | tests: cmpp | cb/exact/habits/(exact/habits/ package) | YY-YY |
| exact.habits.boolx | 142 | nothing | cb/exact/habits/boolx.cpp +1 | YY-YY |
| exact.habits.cmpp | 97 | tests: cmpp | cb/exact/habits/cmpp.cpp +1 | YY-YY |
| exact.habits.newcomp | 425 | named in native text only | cb/exact/habits/newcomp.cpp +1 | YY-YY |
| exact.habits.oldcomp | 97 | named in native text only | cb/exact/habits/oldcomp.cpp +1 | YY-YY |
| exact.habits.symx | 160 | nothing | cb/exact/habits/symx.cpp +1 | YY-YY |
| exact.hooks_lattice | 14 | named in native text only | cb/exact/hooks_lattice.cpp +1 | YY-YY |
| exact.identify | 90 | nothing | cb/exact/identify.cpp +1 | YY-YY |
| exact.ledger | 175 | nothing | cb/exact/ledger.cpp +1 | YY-YY |
| exact.library | 722 | nothing | cb/exact/library.cpp +1 | YY-YY |
| exact.link | 526 | native gens: 1 | cb/exact/link.cpp +5 | YYGYY |
| exact.linkemu | 191 | nothing | cb/exact/linkemu.cpp +1 | YY-YY |
| exact.linkwhole | 298 | nothing | cb/exact/linkwhole.cpp +1 | YY-YY |
| exact.modules | 347 | nothing | cb/exact/modules.cpp +1 | YY-YY |
| exact.mspascal | 219 | named in native text only | cb/exact/mspascal.cpp +1 | YY-YY |
| exact.names | 209 | native gens: 1 | cb/exact/names.cpp +1 | YYGYY |
| exact.pascal_join | 260 | nothing | cb/exact/pascal_join.cpp +1 | YY-YY |
| exact.plm86 | 78 | named in native text only | cb/exact/plm86.cpp +1 | YY-YY |
| exact.project | 199 | native gens: 2 | cb/exact/project.cpp +2 | YYGYY |
| exact.readable | 99 | nothing | cb/exact/readable.cpp +1 | YY-YY |
| exact.recut | 119 | tests: recut | cb/exact/recut.cpp +1 | YY-YY |
| exact.repair | 174 | nothing | cb/exact/repair.cpp +1 | YY-YY |
| exact.run | 72 | native gens: 1 | cb/exact/run.cpp +1 | YYGYY |
| exact.seed | 129 | native gens: 1 | cb/exact/seed.cpp +1 | YYGYY |
| exact.standin | 358 | tests: exact_fixes,recut | cb/exact/standin.cpp +1 | YY-YY |
| exact.toolchain | 373 | nothing | cb/exact/toolchain.cpp +1 | YY-YY |
| exact.x86 | 88 | nothing | cb/exact/x86.cpp +1 | YY-YY |
| exact.xenix_c | 348 | named in native text only | cb/exact/xenix_c.cpp +1 | YY-YY |

### codebreaker.formats

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| formats | 0 | live cmds: all 17; UI; tests: n_subjects,concolic,dos+27; tools: gate_teeth16,golden,memmap16; native gens: 7 | cb/formats/(formats/ package) | YYGNN |
| formats.cab | 178 | tests: n_subjects,media | cb/formats/cab.cpp +1 | YY-YN |
| formats.detect | 88 | live cmds: all 17; UI; tests: n_subjects,plumbing,xbox_formats; native gens: 1 | cb/formats/detect.cpp +1 | YYGNN |
| formats.dos | 306 | live cmds: all 17; UI; tests: n_subjects,dos,exepack+2; native gens: 1 | cb/formats/dos.cpp +2 | YYGNN |
| formats.fonts | 115 | live cmds: facts,fonts; tests: n_subjects,versions | cb/formats/fonts.cpp +1 | YY-NY |
| formats.gb | 113 | live cmds: all 17 | cb/formats/gb.cpp +1 | YY-NY |
| formats.genesis | 128 | live cmds: all 17 | cb/formats/genesis.cpp +1 | YY-NY |
| formats.gfx | 223 | live cmds: facts,gfx; tests: xbox_formats | cb/formats/gfx.cpp +2 | YY-NY |
| formats.inspect | 807 | live cmds: coverage,progress,facts; tests: n_subjects,inspect_render,ui+1 | cb/formats/inspect.cpp +2 | YY-NY |
| formats.le | 411 | live cmds: all 17; UI; tests: n_subjects,drivers; tools: golden; native gens: 1 | cb/formats/le.cpp +4 | YYGNN |
| formats.media | 247 | live cmds: all 17; tests: dos,drivers,media+1 | cb/formats/media.cpp +2 | YY-NY |
| formats.msx | 66 | live cmds: all 17 | cb/formats/msx.cpp +1 | YY-NY |
| formats.native | 223 | live cmds: all 17; UI; tests: native_formats; native gens: 1 | none found | NNGNN |
| formats.ne | 163 | live cmds: all 17; UI; tests: n_subjects,concolic,invert+4; tools: gate_teeth16,memmap16,record; native gens: 1 | cb/formats/ne.cpp +3 | YYGNN |
| formats.nes | 138 | live cmds: all 17; UI; tests: nes | cb/formats/nes.cpp +1 | YY-NN |
| formats.omf | 248 | live cmds: all 17; UI; tests: invert,verification; native gens: 1 | cb/formats/omf.cpp +1 | YYGNN |
| formats.pe | 176 | live cmds: all 17; UI; tests: n_subjects,concolic,run_record+2; tools: sleigh_verify,sleigh_verify_ppc; native gens: 1 | cb/formats/pe.cpp +3 | YYGNN |
| formats.pe_functions | 366 | live cmds: all 17; UI; tests: n_subjects,versions | cb/formats/pe_functions.cpp +1 | YY-NN |
| formats.powerpc_image | 128 | live cmds: all 17; UI; tests: powerpc,vmx128; native gens: 2 | cb/formats/powerpc_image.cpp +1 | YYGNN |
| formats.resources | 196 | live cmds: all 17; UI; tests: n_subjects,whole_module | cb/formats/resources.cpp +1 | YY-NN |
| formats.sms | 129 | live cmds: all 17 | cb/formats/sms.cpp +1 | YY-NY |
| formats.xbox | 119 | live cmds: all 17; tests: port_fixes,xbe,xbox_crypto+3; native gens: 4 | cb/formats/xbox/(formats/xbox/ package) | YYGNY |
| formats.xbox.bink | 92 | live cmds: all 17; tests: xbox_formats; native gens: 2 | cb/formats/xbox/bink.cpp +1 | YYGNY |
| formats.xbox.chd | 124 | live cmds: all 17; native gens: 2 | cb/formats/xbox/chd.cpp +1 | YYGNY |
| formats.xbox.ciso | 123 | live cmds: all 17 | cb/formats/xbox/ciso.cpp +1 | YY-NY |
| formats.xbox.crypto | 46 | DELETED batch 1 was: tests: port_fixes,xbox_crypto; native gens: 2 | cb/formats/xbox/crypto/(formats/xbox/crypto/ package) | YYGYY |
| formats.xbox.crypto.MemCrypto | 467 | DELETED batch 1 was: native gens: 1 | cb/formats/xbox/crypto/mem_crypto.cpp +3 | YYGYY |
| formats.xbox.crypto.StreamIO | 173 | DELETED batch 1 was: tests: port_fixes,xbox_crypto | cb/formats/xbox/crypto/stream_io.cpp +1 | YY-YY |
| formats.xbox.crypto.XeCrypt | 1278 | DELETED batch 1 was: tests: port_fixes,xbox_crypto; native gens: 1 | cb/formats/xbox/crypto/xecrypt.cpp +4 | YYGYY |
| formats.xbox.crypto.bin2lang | 140 | DELETED batch 1 was: nothing | cb/formats/xbox/crypto/bin2lang.cpp +1 | YN-YY |
| formats.xbox.crypto.bootchain | 255 | DELETED batch 1 was: nothing | cb/formats/xbox/crypto/bootchain.cpp +1 | YN-YY |
| formats.xbox.crypto.build_lib | 466 | DELETED batch 1 was: tests: port_fixes | cb/formats/xbox/crypto/build_lib.cpp +1 | YY-YY |
| formats.xbox.crypto.cpu_key | 14 | DELETED batch 1 was: nothing | cb/formats/xbox/crypto/cpu_key.cpp +3 | YN-YY |
| formats.xbox.crypto.cpukey_ecd | 160 | DELETED batch 1 was: tests: port_fixes; native gens: 1 | cb/formats/xbox/crypto/cpu_key.cpp +3 | YNGYY |
| formats.xbox.crypto.create_key | 15 | DELETED batch 1 was: nothing | none found | NN-YY |
| formats.xbox.crypto.exp_build_task | 66 | DELETED batch 1 was: nothing | none found | NN-YY |
| formats.xbox.crypto.exp_signer | 120 | DELETED batch 1 was: tests: xbox_crypto; native gens: 1 | cb/formats/xbox/crypto/exp_signer.cpp +1 | YNGYY |
| formats.xbox.crypto.export_keys | 23 | DELETED batch 1 was: nothing | none found | NN-YY |
| formats.xbox.crypto.flashplan | 142 | DELETED batch 1 was: nothing | cb/formats/xbox/crypto/flashplan.cpp +1 | YN-YY |
| formats.xbox.crypto.format_key | 8 | DELETED batch 1 was: nothing | cb/formats/xbox/crypto/format_key.cpp +1 | YY-YY |
| formats.xbox.crypto.fuses | 77 | DELETED batch 1 was: nothing | cb/formats/xbox/crypto/fuses.cpp +1 | YN-YY |
| formats.xbox.crypto.harness | 392 | DELETED batch 1 was: nothing | cb/formats/xbox/crypto/harness.cpp +1 | YY-YY |
| formats.xbox.crypto.hypervisor | 196 | DELETED batch 1 was: nothing | cb/formats/xbox/crypto/hypervisor.cpp +1 | YN-YY |
| formats.xbox.crypto.keystore | 49 | DELETED batch 1 was: nothing | cb/formats/xbox/crypto/keystore.cpp +1 | YY-YY |
| formats.xbox.crypto.keyvault | 127 | DELETED batch 1 was: nothing | cb/formats/xbox/crypto/keyvault.cpp +1 | YN-YY |
| formats.xbox.crypto.kv_tool | 93 | DELETED batch 1 was: native gens: 1 | cb/formats/xbox/crypto/kv_tool.cpp +1 | YNGYY |
| formats.xbox.crypto.lzx | 653 | DELETED batch 1 was: nothing | cb/formats/xbox/crypto/lzx_dll.cpp +1 | YN-YY |
| formats.xbox.crypto.nand | 110 | DELETED batch 1 was: nothing | cb/formats/xbox/crypto/nand_dump.cpp +3 | YY-YY |
| formats.xbox.crypto.nand_tool | 555 | DELETED batch 1 was: nothing | cb/formats/xbox/crypto/nand_tool.cpp +1 | YY-YY |
| formats.xbox.crypto.patch_build_task | 16 | DELETED batch 1 was: nothing | none found | NN-YY |
| formats.xbox.crypto.patch_checker | 49 | DELETED batch 1 was: nothing | none found | NN-YY |
| formats.xbox.crypto.patch_compile | 22 | DELETED batch 1 was: nothing | none found | NN-YY |
| formats.xbox.crypto.patch_dumper | 42 | DELETED batch 1 was: nothing | none found | NN-YY |
| formats.xbox.crypto.sd_signer | 51 | DELETED batch 1 was: tests: port_fixes | cb/formats/xbox/crypto/sd_signer.cpp +1 | YY-YY |
| formats.xbox.crypto.se_patcher | 91 | DELETED batch 1 was: nothing | cb/formats/xbox/crypto/se_patcher.cpp +1 | YY-YY |
| formats.xbox.crypto.shadowboot | 858 | DELETED batch 1 was: nothing | cb/formats/xbox/crypto/shadowboot.cpp +1 | YY-YY |
| formats.xbox.crypto.smc | 146 | DELETED batch 1 was: nothing | cb/formats/xbox/crypto/smc.cpp +1 | YN-YY |
| formats.xbox.crypto.xcp_dumper | 425 | DELETED batch 1 was: tests: xbox_crypto | cb/formats/xbox/crypto/xcp_dumper.cpp +1 | YY-YY |
| formats.xbox.crypto.xdk_extract | 164 | DELETED batch 1 was: nothing | cb/formats/xbox/crypto/xdk_extract.cpp +1 | YN-YY |
| formats.xbox.crypto.xebuild_config | 28 | DELETED batch 1 was: nothing | none found | NN-YY |
| formats.xbox.crypto.xekeys_signer | 59 | DELETED batch 1 was: native gens: 1 | cb/formats/xbox/crypto/xekeys_signer.cpp +1 | YNGYY |
| formats.xbox.crypto.xell_unpack | 53 | DELETED batch 1 was: nothing | cb/formats/xbox/crypto/xell_unpack.cpp +1 | YN-YY |
| formats.xbox.crypto.xkelib_exports | 75 | DELETED batch 1 was: native gens: 1 | cb/formats/xbox/crypto/xkelib_exports.cpp +1 | YNGYY |
| formats.xbox.crypto.xsm3 | 337 | DELETED batch 1 was: tests: port_fixes; native gens: 1 | cb/formats/xbox/crypto/xsm3.cpp +1 | YNGYY |
| formats.xbox.crypto.xval | 104 | DELETED batch 1 was: nothing | cb/formats/xbox/crypto/xval.cpp +1 | YN-YY |
| formats.xbox.effect | 334 | tests: xbox_effect | cb/formats/xbox/effect.cpp +1 | YY-YY |
| formats.xbox.fatx | 760 | live cmds: all 17; tests: xbox_formats | cb/formats/xbox/fatx.cpp +1 | YY-NY |
| formats.xbox.gdf | 134 | live cmds: all 17; tests: xbox_formats | cb/formats/xbox/gdf.cpp +1 | YY-NY |
| formats.xbox.god | 207 | live cmds: all 17; tests: xbox_formats | cb/formats/xbox/god.cpp +1 | YY-NY |
| formats.xbox.heavyiron | 400 | live cmds: all 17; tests: xbox_formats | cb/formats/xbox/heavyiron.cpp +1 | YY-NY |
| formats.xbox.nand | 97 | live cmds: all 17; tests: xbox_formats | cb/formats/xbox/nand.cpp +1 | YY-NY |
| formats.xbox.pck | 126 | live cmds: all 17 | cb/formats/xbox/pck.cpp +1 | YY-NY |
| formats.xbox.shader | 152 | tests: xbox_effect | cb/formats/xbox/shader.cpp +1 | YY-YY |
| formats.xbox.stfs | 530 | live cmds: all 17; tests: xbox_formats | cb/formats/xbox/stfs.cpp +1 | YY-NY |
| formats.xbox.system_ui | 109 | live cmds: facts,system-ui | cb/formats/xbox/system_ui.cpp +1 | YY-NY |
| formats.xbox.texture | 100 | live cmds: all 17; tests: xbox_formats,xna | cb/formats/xbox/texture.cpp +1 | YY-NY |
| formats.xbox.volumes | 199 | live cmds: all 17 | cb/formats/xbox/volumes.cpp +1 | YY-NY |
| formats.xbox.xbe | 129 | live cmds: all 17; tests: xbe,xbox_formats; native gens: 2 | cb/formats/xbox/xbe.cpp +1 | YYGNY |
| formats.xbox.xdbf | 188 | live cmds: all 17; tests: xbox_formats; native gens: 2 | cb/formats/xbox/xdbf.cpp +1 | YYGNY |
| formats.xbox.xma | 120 | tests: xbox_formats | none found | NN-YY |
| formats.xbox.xpr | 135 | live cmds: all 17; tests: xbox_formats; native gens: 2 | cb/formats/xbox/xpr.cpp +1 | YYGNY |
| formats.xbox.xsb | 21 | tests: xna | none found | NN-YY |
| formats.xbox.xur | 148 | live cmds: all 17; tests: xbox_formats; native gens: 2 | cb/formats/xbox/xur.cpp +1 | YYGNY |
| formats.xbox.xwb | 153 | live cmds: all 17; tests: xbox_formats | none found | NN-NY |
| formats.xbox.xzp | 145 | live cmds: all 17; tests: xbox_formats; native gens: 1 | cb/formats/xbox/xzp.cpp +1 | YYGNY |
| formats.xbox.zar | 1034 | live cmds: all 17 | cb/formats/xbox/zar.cpp +1 | YY-NY |
| formats.xex | 173 | live cmds: all 17; UI; tests: vmx128,x360,xex+1; native gens: 2 | cb/formats/xex.cpp +3 | YYGNN |
| formats.xex_edit | 159 | live cmds: facts,xex-edit; tests: xex_edit | none found | NN-NY |
| formats.xex_imports | 81 | live cmds: facts,xex-imports,ppc-calls | cb/formats/xex_imports.cpp +1 | YY-NY |
| formats.xnb | 232 | tests: xbox_effect,xna | none found | NN-YY |
| formats.zx | 53 | live cmds: all 17; tests: z80 | cb/formats/zx.cpp +1 | YY-NY |

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
| invert.native_invert | 281 | nothing | none found | NN-YY |
| invert.quad_align | 76 | nothing | cb/invert/quad_align.cpp +1 | YY-YY |
| invert.quad_dump | 67 | nothing | cb/invert/quad_dump.cpp +1 | YY-YY |
| invert.run | 258 | tests: invert | none found | NN-YY |
| invert.segment_thunk | 49 | tests: invert | none found | NN-YY |
| invert.whole_function | 28 | nothing | none found | NN-YY |

### codebreaker.java

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| java | 21 | live cmds: all 17; UI; tests: achievements,apimap,applet_ping+58; native gens: 3 | cb/java/(java/ package) | YYGNN |
| java._paths | 15 | live cmds: all 17; UI | cb/java/paths.cpp +1 | YY-NN |
| java.analyze | 237 | nothing | cb/java/analyze.cpp +1 | YY-YY |
| java.apimap | 399 | live cmds: all 17; UI; tests: apimap | cb/java/apimap.cpp +1 | YY-NN |
| java.bugfix_ledger | 163 | live cmds: all 17; UI | cb/java/bugfix_ledger.cpp +1 | YY-NN |
| java.cmake | 53 | live cmds: all 17; UI | cb/java/cmake.cpp +1 | YY-NN |
| java.console | 2 | live cmds: all 17; UI; tests: assets,platform_config,platform_refs+5; native gens: 2 | cb/java/console/(java/console/ package) | YYGNN |
| java.console.assets | 280 | tests: assets; native gens: 1 | cb/java/console/assets.cpp +1 | YYGYY |
| java.console.platform_config | 579 | tests: platform_config; native gens: 1 | cb/java/console/platform_config.cpp +1 | YYGYY |
| java.console.platform_equivalents | 213 | live cmds: all 17; UI | cb/java/console/platform_equivalents.cpp +1 | YY-NN |
| java.console.platform_refs | 501 | live cmds: all 17; UI; tests: platform_refs,runnable_entry | cb/java/console/platform_refs.cpp +1 | YY-NN |
| java.console.project_layout | 155 | tests: project_layout; native gens: 1 | cb/java/console/project_layout.cpp +1 | YYGYY |
| java.console.project_scan | 134 | tests: project_scan; native gens: 1 | cb/java/console/project_scan.cpp +1 | YYGYY |
| java.console.sdk | 192 | live cmds: all 17; UI; tests: platform_config | cb/java/console/sdk.cpp +1 | YY-NN |
| java.console.vcxproj | 283 | tests: profiles_selection,vcxproj_filters; native gens: 1 | cb/java/console/vcxproj.cpp +1 | YYGYY |
| java.console.worklist | 61 | native gens: 1 | cb/java/console/worklist.cpp +1 | YYGYY |
| java.console.xact | 613 | tests: assets; native gens: 1 | cb/java/console/xact.cpp +1 | YYGYY |
| java.console.xdk_scan | 118 | native gens: 1 | cb/java/console/xdk_scan.cpp +1 | YYGYY |
| java.debug | 321 | live cmds: all 17; UI; tests: platform_config,ui_mark | cb/java/debug.cpp +1 | YY-NN |
| java.emit | 21 | live cmds: all 17; UI; tests: beta_fixes,collection_out_params,emit+2 | cb/java/emit/(java/emit/ package) | YY-NN |
| java.emit.header | 335 | live cmds: all 17; UI; tests: beta_fixes,emit,emit_source+1 | cb/java/emit/header.cpp +1 | YY-NN |
| java.emit.naming | 47 | live cmds: all 17; UI | cb/java/emit/naming.cpp +1 | YY-NN |
| java.emit.ownership | 353 | live cmds: all 17; UI; tests: beta_fixes,collection_out_params | cb/java/emit/ownership.cpp +1 | YY-NN |
| java.emit.source | 556 | live cmds: all 17; UI; tests: beta_fixes,emit_source | cb/java/emit/source.cpp +2 | YY-NN |
| java.emit.std_transforms | 95 | live cmds: all 17; UI | cb/java/emit/std_transforms.cpp +1 | YY-NN |
| java.ir | 225 | live cmds: all 17; UI; tests: beta_fixes,collection_out_params,emit+7 | cb/java/ir.cpp +1 | YY-NN |
| java.ledger | 156 | live cmds: all 17; UI; tests: beta_fixes,emit,emit_source | cb/java/ledger.cpp +1 | YY-NN |
| java.overlays | 79 | live cmds: all 17; UI | cb/java/overlays.cpp +1 | YY-NN |
| java.parse | 444 | live cmds: all 17; UI; tests: beta_fixes,build3_fixes,bytes+19 | cb/java/parse.cpp +1 | YY-NN |
| java.platforms | 405 | live cmds: all 17; UI | cb/java/platforms.cpp +1 | YY-NN |
| java.probe | 859 | live cmds: all 17; UI; tests: bytes,cpp03_bodies,finally+12 | cb/java/probe.cpp +1 | YY-NN |
| java.profiles | 105 | live cmds: all 17; UI; tests: beta_fixes,build3_fixes,call_arg_cast+11; native gens: 1 | cb/java/profiles/(java/profiles/ package) | YYGNN |
| java.projects | 112 | live cmds: all 17; UI; tests: achievements,applet_ping,console_patches+10; native gens: 2 | cb/java/projects/(java/projects/ package) | YYGNN |
| java.projects.minecraft | 120 | tests: achievements,applet_ping,console_patches+9; native gens: 2 | cb/java/projects/minecraft/(java/projects/minecraft/ package) | YYGYY |
| java.projects.minecraft.achievements | 548 | tests: achievements | cb/java/projects/minecraft/achievements.cpp +1 | YY-YY |
| java.projects.minecraft.applet_ping | 130 | tests: applet_ping,console_patches; native gens: 1 | cb/java/projects/minecraft/applet_ping.cpp +1 | YYGYY |
| java.projects.minecraft.content | 131 | native gens: 1 | cb/java/projects/minecraft/content.cpp +1 | YYGYY |
| java.projects.minecraft.controller_nav | 284 | tests: controller_nav,review_fixes | cb/java/projects/minecraft/controller_nav.cpp +1 | YY-YY |
| java.projects.minecraft.edition | 158 | tests: edition_autodetect,edition_resolve,review_fixes | cb/java/projects/minecraft/edition.cpp +1 | YY-YY |
| java.projects.minecraft.keybind_mark | 126 | nothing | cb/java/projects/minecraft/keybind_mark.cpp +1 | YY-YY |
| java.projects.minecraft.platform_coverage | 419 | tests: platform_coverage_symbols | cb/java/projects/minecraft/platform_coverage.cpp +1 | YY-YY |
| java.projects.minecraft.profiles | 2 | nothing | cb/java/projects/minecraft/profiles/(java/projects/minecraft/profiles/ package) | YY-YY |
| java.projects.minecraft.profiles.minecraft_alpha | 26 | nothing | cb/java/projects/minecraft/profiles/minecraft_alpha.cpp +1 | YN-YY |
| java.projects.minecraft.profiles.minecraft_base | 46 | nothing | cb/java/projects/minecraft/profiles/minecraft_base.cpp +1 | YN-YY |
| java.projects.minecraft.profiles.minecraft_beta | 26 | nothing | cb/java/projects/minecraft/profiles/minecraft_beta.cpp +1 | YN-YY |
| java.projects.minecraft.profiles.minecraft_indev | 110 | nothing | cb/java/projects/minecraft/profiles/minecraft_indev.cpp +1 | YN-YY |
| java.projects.minecraft.profiles.minecraft_infdev | 52 | nothing | cb/java/projects/minecraft/profiles/minecraft_infdev.cpp +1 | YN-YY |
| java.projects.minecraft.profiles.minecraft_release | 37 | nothing | cb/java/projects/minecraft/profiles/minecraft_release.cpp +1 | YN-YY |
| java.projects.minecraft.profiles.minecraft_rubydung | 53 | nothing | cb/java/projects/minecraft/profiles/minecraft_rubydung.cpp +1 | YN-YY |
| java.projects.minecraft.profiles.minecraft_xbox360 | 213 | nothing | cb/java/projects/minecraft/profiles/minecraft_xbox360.cpp +1 | YY-YY |
| java.projects.minecraft.raycast | 114 | tests: pipeline | cb/java/projects/minecraft/raycast.cpp +1 | YY-YY |
| java.projects.minecraft.render_fix | 261 | tests: console_patches,render_fix; native gens: 1 | cb/java/projects/minecraft/render_fix.cpp +1 | YYGYY |
| java.projects.minecraft.sound_fix | 217 | tests: console_patches; native gens: 1 | cb/java/projects/minecraft/sound_fix.cpp +1 | YYGYY |
| java.projects.minecraft.ui_mark | 153 | tests: ui_mark | cb/java/projects/minecraft/ui_mark.cpp +1 | YY-YY |
| java.reflection_factory | 290 | live cmds: all 17; UI; tests: reflection_factory | cb/java/reflection_factory.cpp +1 | YY-NN |
| java.report | 149 | live cmds: all 17; UI | cb/java/report.cpp +1 | YY-NN |
| java.rules | 1046 | live cmds: all 17; UI; tests: beta_fixes,port_fixes,profiles_selection+1 | cb/java/rules.cpp +1 | YY-NN |
| java.run | 885 | tests: review_fixes | cb/java/run.cpp +7 | YY-YY |
| java.runtime | 1789 | live cmds: all 17; UI; tests: awt_stubs,beta_fixes,runnable_entry+1 | cb/java/runtime.cpp +5 | YY-NN |
| java.scaffold | 182 | tests: scaffold | cb/java/scaffold.cpp +1 | YY-YY |
| java.signatures | 207 | tests: profiles_selection | cb/java/signatures.cpp +1 | YY-YY |
| java.stubs | 168 | live cmds: all 17; UI | cb/java/stubs.cpp +1 | YY-NN |
| java.textutil | 39 | live cmds: all 17; UI | cb/java/textutil.cpp +1 | YY-NN |
| java.transpile | 1513 | live cmds: all 17; UI; tests: beta_fixes,build3_fixes,bytes+27 | cb/java/transpile.cpp +1 | YY-NN |
| java.twophase | 179 | live cmds: all 17; UI; tests: two_phase | cb/java/twophase.cpp +1 | YY-NN |
| java.typemap | 256 | live cmds: all 17; UI; tests: awt_stubs,beta_fixes,emit+2 | cb/java/typemap.cpp +1 | YY-NN |
| java.types | 210 | live cmds: all 17; UI; tests: emit,emit_source,pipeline+1 | cb/java/types.cpp +1 | YY-NN |

### codebreaker.jobs

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| jobs | 0 | live cmds: all 17; UI; tests: knowledge_reads,plumbing; native gens: 1 | cb/jobs/(jobs/ package) | YYGNN |
| jobs.pool | 287 | live cmds: all 17; UI; tests: knowledge_reads,plumbing; native gens: 1 | cb/jobs/pool.cpp +1 | YYGNN |

### codebreaker.knowledge

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| knowledge | 173 | live cmds: all 17; UI; tests: knowledge,knowledge_reads,native_engine | cb/knowledge/(knowledge/ package) | YY-NN |
| knowledge.reads | 174 | live cmds: all 17; UI; tests: knowledge_reads | cb/knowledge/reads.cpp +1 | YY-NN |
| knowledge.show | 50 | live cmds: knowledge,facts | cb/knowledge/show.cpp +1 | YY-NY |
| knowledge.sources | 115 | live cmds: all 17; UI; tests: knowledge | cb/knowledge/sources.cpp +1 | YY-NN |
| knowledge.store | 72 | live cmds: all 17; UI; tests: knowledge,knowledge_reads,native_engine | cb/knowledge/store.cpp +1 | YY-NN |

### codebreaker.platforms

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| platforms | 389 | live cmds: all 17; UI; tests: m68k,nes,platforms+4; native gens: 1 | cb/platforms/(platforms/ package) | YYGNN |
| platforms.dos | 33 | live cmds: all 17 | cb/platforms/dos.cpp +1 | YN-NY |
| platforms.gb | 19 | live cmds: all 17 | cb/platforms/gb.cpp +1 | YN-NY |
| platforms.genesis | 19 | live cmds: all 17 | cb/platforms/genesis.cpp +1 | YN-NY |
| platforms.java | 25 | live cmds: all 17 | cb/platforms/java.cpp +1 | YN-NY |
| platforms.msx | 19 | live cmds: all 17 | cb/platforms/msx.cpp +1 | YN-NY |
| platforms.nes | 19 | live cmds: all 17 | cb/platforms/nes.cpp +1 | YN-NY |
| platforms.sms | 19 | live cmds: all 17 | cb/platforms/sms.cpp +1 | YN-NY |
| platforms.vxd | 43 | live cmds: all 17 | cb/platforms/vxd.cpp +1 | YY-NY |
| platforms.win16 | 72 | live cmds: all 17 | cb/platforms/win16.cpp +1 | YY-NY |
| platforms.win32 | 58 | live cmds: all 17 | none found | NN-NY |
| platforms.xbox | 22 | live cmds: all 17 | none found | NN-NY |
| platforms.xbox360 | 45 | live cmds: all 17 | none found | NN-NY |
| platforms.zx | 19 | live cmds: all 17 | none found | NN-NY |

### codebreaker.port

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| port | 19 | DELETED batch 1 was: tests: module_model,port; native gens: 5 | cb/port/(port/ package) | YYGYY |
| port.__main__ | 1047 | DELETED batch 1 was: tests: port; native gens: 1 | none found | NNGYY |
| port.arith16 | 402 | DELETED batch 1 was: nothing | cb/port/arith16.cpp +1 | YY-YY |
| port.asm16 | 2200 | DELETED batch 1 was: tests: module_model; native gens: 2 | cb/port/asm16.hpp +4 | YYGYY |
| port.asmdata | 343 | DELETED batch 1 was: nothing | cb/port/asmdata.cpp +1 | YY-YY |
| port.oldlayout | 398 | DELETED batch 1 was: native gens: 1 | cb/port/oldlayout.cpp +1 | YYGYY |
| port.pre_ansi | 5471 | DELETED batch 1 was: tests: port; native gens: 2 | cb/port/pre_ansi_types.cpp +1 | YNGYY |
| port.sanitize | 340 | DELETED batch 1 was: nothing | cb/port/sanitize.cpp +3 | YY-YY |
| port.verify | 274 | DELETED batch 1 was: nothing | cb/port/verify.cpp +1 | YY-YY |

### codebreaker.recompile

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| recompile | 17 | live cmds: facts,ppc-calls; tests: python_frontend,recompile,x360; tools: py2cpp; native gens: 7 | cb/recompile/(recompile/ package) | YYGNY |
| recompile.builders | 0 | native gens: 2 | cb/recompile/builders/(recompile/builders/ package) | YYGYY |
| recompile.builders.arith | 269 | native gens: 1 | cb/recompile/builders/arith.cpp +1 | YYGYY |
| recompile.builders.floating | 172 | native gens: 2 | cb/recompile/builders/floating.cpp +1 | YYGYY |
| recompile.builders.logical | 286 | native gens: 1 | cb/recompile/builders/logical.cpp +1 | YYGYY |
| recompile.builders.memory | 492 | native gens: 1 | cb/recompile/builders/memory.cpp +1 | YYGYY |
| recompile.builders.system | 384 | native gens: 1 | cb/recompile/builders/system.cpp +1 | YYGYY |
| recompile.builders.vector | 989 | native gens: 1 | cb/recompile/builders/vector.cpp +1 | YYGYY |
| recompile.config | 260 | tests: x360; native gens: 1 | cb/recompile/config.cpp +1 | YYGYY |
| recompile.disassemble | 158 | live cmds: facts,ppc-calls; tests: recompile; native gens: 2 | cb/recompile/disassemble.cpp +1 | YYGNY |
| recompile.discover | 181 | live cmds: facts,ppc-calls; tests: recompile,x360; native gens: 3 | cb/recompile/discover.cpp +1 | YYGNY |
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
| source | 0 | live cmds: all 17; UI; tests: drivers,invert,kind+6; native gens: 1 | cb/source/(source/ package) | YYGNN |
| source.assembly | 232 | live cmds: all 17; UI | cb/source/assembly.cpp +1 | YY-NN |
| source.classes_writer | 162 | live cmds: all 17; UI | cb/source/classes_writer.cpp +1 | YY-NN |
| source.dos_build | 21 | nothing | cb/source/dos_build.cpp +1 | YN-YY |
| source.dos_writer | 1005 | live cmds: all 17 | cb/source/dos_writer.cpp +1 | YY-NY |
| source.driver_writer | 625 | live cmds: all 17; tests: drivers | cb/source/driver_writer.cpp +1 | YY-NY |
| source.gb_writer | 135 | live cmds: all 17; tests: z80 | cb/source/gb_writer.cpp +1 | YY-NY |
| source.genesis_writer | 148 | live cmds: all 17; tests: m68k | cb/source/genesis_writer.cpp +1 | YY-NY |
| source.image_writer | 345 | live cmds: all 17; tests: module_model | cb/source/image_writer.cpp +1 | YY-NY |
| source.java_writer | 60 | live cmds: all 17; UI | cb/source/java_writer.cpp +1 | YY-NN |
| source.le_writer | 518 | tests: module_model | cb/source/le_writer.cpp +1 | YY-YY |
| source.module_writer | 435 | live cmds: all 17; UI; tests: kind | cb/source/module_writer.cpp +1 | YY-NN |
| source.msx_writer | 43 | live cmds: all 17; tests: z80 | cb/source/msx_writer.cpp +1 | YY-NY |
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
| source.nes_writer | 160 | live cmds: all 17; tests: nes | cb/source/nes_writer.cpp +1 | YY-NY |
| source.pe_writer | 529 | tests: module_model,port_fixes | cb/source/pe_writer.cpp +1 | YY-YY |
| source.placement | 121 | live cmds: all 17; UI | cb/source/placement.cpp +1 | YY-NN |
| source.readable | 644 | live cmds: all 17; UI; tests: module_model,readable | cb/source/readable.cpp +1 | YY-NN |
| source.sms_writer | 133 | live cmds: all 17; tests: port_fixes,z80 | cb/source/sms_writer.cpp +1 | YY-NY |
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
| source.z80_writer | 94 | live cmds: all 17 | cb/source/z80_writer.cpp +1 | YY-NY |
| source.zx_writer | 41 | live cmds: all 17; tests: z80 | cb/source/zx_writer.cpp +1 | YN-NY |

### codebreaker.store

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| store | 0 | live cmds: all 17; UI; tests: kind,knowledge,native_engine+6; tools: causes,failures,oss_bench; native gens: 1 | cb/store/(store/ package) | YYGNN |
| store.functions | 126 | live cmds: all 17; UI; tests: kind,knowledge,native_engine+5; tools: causes,failures,oss_bench; native gens: 1 | cb/store/functions.cpp +1 | YYGNN |

### codebreaker.symbols

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| symbols | 0 | live cmds: all 17; UI; tests: dos,drivers,invert+4; tools: struct_report; native gens: 1 | cb/symbols/(symbols/ package) | YYGNN |
| symbols.api_arguments | 434 | live cmds: all 17; UI; tests: knowledge_reads; native gens: 1 | cb/symbols/api_arguments.cpp +1 | YYGNN |
| symbols.cpp_classes | 214 | nothing | cb/symbols/cpp_classes.cpp +1 | YY-YY |
| symbols.debug_build | 168 | tests: invert | cb/symbols/debug_build.cpp +3 | YY-YY |
| symbols.debug_build_estimate | 196 | nothing | cb/symbols/debug_build.cpp +3 | YY-YY |
| symbols.demangle | 107 | live cmds: all 17; UI; tests: symbols | cb/symbols/demangle.cpp +1 | YY-NN |
| symbols.dos_interrupts | 101 | live cmds: all 17; UI; tests: dos | cb/symbols/dos_interrupts.cpp +1 | YY-NN |
| symbols.driver_services | 280 | live cmds: all 17; UI; tests: drivers | cb/symbols/driver_services.cpp +2 | YY-NN |
| symbols.libraries | 104 | live cmds: all 17; UI; tests: symbols,verification | cb/symbols/libraries/(symbols/libraries/ package) | YY-NN |
| symbols.libraries.coff | 166 | live cmds: all 17; UI; tests: symbols | cb/symbols/libraries/coff.cpp +1 | YY-NN |
| symbols.libraries.omf | 152 | live cmds: all 17; UI | cb/symbols/libraries/omf.cpp +1 | YY-NN |
| symbols.names | 545 | live cmds: all 17; UI; tests: symbols | cb/symbols/names.cpp +4 | YY-NN |
| symbols.native | 70 | live cmds: all 17; UI | none found | NN-NN |
| symbols.ordinals | 377 | live cmds: all 17; UI; tests: symbols,xbe | cb/symbols/ordinals.cpp +1 | YY-NN |
| symbols.prototypes | 124 | live cmds: all 17; UI; tests: knowledge_reads,symbols; tools: struct_report | cb/symbols/prototypes.cpp +1 | YY-NN |
| symbols.runtime_library | 386 | live cmds: all 17; UI; tests: symbols | cb/symbols/runtime_library.cpp +1 | YY-NN |
| symbols.sdk_structs | 265 | live cmds: all 17; UI; tools: struct_report | cb/symbols/sdk_structs.cpp +1 | YY-NN |
| symbols.wine16 | 89 | live cmds: all 17; UI; tests: symbols,verification | cb/symbols/wine16.cpp +1 | YY-NN |
| symbols.xdk_libraries | 151 | live cmds: all 17; UI | cb/symbols/xdk_libraries.cpp +1 | YY-NN |

### codebreaker.translate

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| translate | 6 | live cmds: all 17; UI; tests: c_backend,clang_frontend,console_patches+21; tools: judge_c_java,l2c,l2check; native gens: 3 | cb/translate/(translate/ package) | YYGNN |
| translate.backends | 1 | live cmds: all 17; UI; tests: c_backend,console_patches,cpp03_bodies+10; tools: l2c,py2cpp; native gens: 3 | cb/translate/backends/(translate/backends/ package) | YYGNN |
| translate.backends.asm | 71 | live cmds: all 17; UI; tests: drivers,module_model | cb/translate/backends/asm.cpp +1 | YY-NN |
| translate.backends.c | 96 | live cmds: all 17; UI; tests: c_backend,frame16,l2_survives; tools: l2c | cb/translate/backends/c.cpp +31 | YY-NN |
| translate.backends.c_java | 1792 | tests: java_as_c | cb/translate/backends/c_java.cpp +9 | YY-YY |
| translate.backends.cpp03 | 1857 | live cmds: all 17; UI; tests: console_patches,cpp03_bodies,projects | cb/translate/backends/cpp03.cpp +13 | YY-NN |
| translate.backends.cpp17 | 70 | live cmds: all 17; UI; tests: cpp17_backend,python_frontend; tools: py2cpp | cb/translate/backends/cpp17.cpp +1 | YY-NN |
| translate.backends.csharp | 1566 | tests: csharp_kit; native gens: 1 | cb/translate/backends/csharp.cpp +3 | YYGYY |
| translate.backends.csharp_java | 631 | nothing | cb/translate/backends/csharp.cpp +3 | YY-YY |
| translate.backends.java | 1978 | tests: java_backend; native gens: 2 | cb/translate/backends/java.cpp +6 | YYGYY |
| translate.backends.java_libc | 80 | nothing | cb/translate/backends/java_libc.cpp +1 | YY-YY |
| translate.backends.java_objects | 1036 | nothing | cb/translate/backends/java_objects.cpp +1 | YY-YY |
| translate.backends.jmodel | 511 | tests: cpp17_backend | cb/translate/backends/jmodel.cpp +1 | YY-YY |
| translate.backends.module_file | 127 | live cmds: all 17; UI; tests: module_model | cb/translate/backends/module_file.cpp +1 | YY-NN |
| translate.desugar | 453 | live cmds: all 17; UI; tests: cpp17_backend,desugar | cb/translate/desugar.cpp +1 | YY-NN |
| translate.dotnet | 149 | tests: csharp_kit,dotnet_toolchain; native gens: 1 | cb/translate/dotnet/(translate/dotnet/ package) | YYGYY |
| translate.dotnet.cs2cpp | 844 | tests: csharp_kit | cb/translate/dotnet/cs2cpp.cpp +6 | YY-YY |
| translate.dotnet.judge | 149 | tests: csharp_kit | cb/translate/dotnet/judge.cpp +1 | YY-YY |
| translate.dotnet.pairs | 114 | tests: csharp_kit; native gens: 1 | none found | NNGYY |
| translate.frontends | 2 | live cmds: all 17; UI; tests: clang_frontend,csharp_kit,csharp_lower_trees+6; tools: py2cpp; native gens: 2 | cb/translate/frontends/(translate/frontends/ package) | YNGNN |
| translate.frontends.clang | 129 | tests: clang_frontend,java_backend; native gens: 2 | cb/translate/frontends/clang/(translate/frontends/clang/ package) | YYGYY |
| translate.frontends.clang.ctypes | 387 | tests: clang_frontend | cb/translate/frontends/clang/ctypes.cpp +1 | YN-YY |
| translate.frontends.clang.j2x | 302 | nothing | none found | NN-YY |
| translate.frontends.clang.lower | 1505 | nothing | cb/translate/frontends/clang/lower.hpp +6 | YY-YY |
| translate.frontends.csharp | 142 | tests: csharp_kit,csharp_lower_trees | cb/translate/frontends/csharp/(translate/frontends/csharp/ package) | YY-YY |
| translate.frontends.csharp.lower | 1233 | tests: csharp_lower_trees | cb/translate/frontends/csharp/lower.cpp +1 | YY-YY |
| translate.frontends.java | 108 | live cmds: all 17; UI; tests: java_frontend,java_lower,l2 | none found | NN-NN |
| translate.frontends.java.lower | 543 | live cmds: all 17; UI; tests: java_lower,l2 | none found | NN-NN |
| translate.frontends.python | 319 | tests: python_frontend,pytypes; tools: py2cpp | cb/translate/frontends/python/(translate/frontends/python/ package) | YY-NY |
| translate.frontends.python.infer | 1955 | nothing | cb/translate/frontends/python/infer.hpp +9 | YY-YY |
| translate.frontends.python.lower | 1816 | nothing | cb/translate/frontends/python/lower.cpp +10 | YY-YY |
| translate.frontends.python.methods | 749 | nothing | cb/translate/frontends/python/methods.cpp +1 | YY-YY |
| translate.frontends.python.program | 874 | nothing | cb/translate/frontends/python/program.cpp +1 | YY-YY |
| translate.frontends.python.pytypes | 308 | tests: pytypes | cb/translate/frontends/python/pytypes.cpp +1 | YY-YY |
| translate.frontends.python.structfmt | 55 | tests: pytypes | cb/translate/frontends/python/structfmt.cpp +1 | YY-YY |
| translate.ir | 320 | live cmds: all 17; UI; tests: clang_frontend,console_patches,cpp17_backend+5 | cb/translate/ir.cpp +1 | YY-NN |
| translate.javajudge | 307 | tests: cpp17_backend,java_as_c | cb/translate/javajudge.cpp +3 | YY-YY |
| translate.judge_native_jvm | 263 | tests: java_backend; tools: judge_c_java | cb/translate/judge_native_jvm.cpp +1 | YY-NY |
| translate.l2 | 431 | live cmds: all 17; UI; tests: c_backend,clang_frontend,csharp_kit+10; tools: l2check; native gens: 3 | cb/translate/l2.cpp +1 | YYGNN |
| translate.native_l2 | 340 | live cmds: all 17; UI; tests: native_l2_core | cb/translate/native_l2.cpp +1 | YY-NN |
| translate.rewrite | 114 | nothing | cb/translate/rewrite.cpp +1 | YY-YY |

### codebreaker.ui

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| ui | 0 | live cmds: facts; UI; tests: mcp,service,ui+2 | cb/ui/(ui/ package) | YY-NY |
| ui.client | 294 | live cmds: facts; UI; tests: service,ui,ui_thin | none found | NN-NY |
| ui.mcp_server | 1271 | live cmds: facts; tests: mcp | cb/ui/mcp_server.hpp | YY-NY |
| ui.progress | 137 | live cmds: progress,facts; tests: ui | cb/ui/progress.cpp +3 | YY-NY |
| ui.serve | 335 | nothing | cb/ui/serve.cpp +3 | YY-YY |
| ui.service | 508 | live cmds: facts; tests: ui | cb/ui/service.cpp +3 | YY-NY |
| ui.terminal | 981 | live cmds: facts; UI; tests: ui,ui_thin,verdicts | none found | NN-NY |

### codebreaker.verify

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| verify | 12 | live cmds: all 17; UI; tests: n_subjects,authored_rewrites,bytes+39; tools: causes,copy_walk,failures; native gens: 4 | cb/verify/(verify/ package) | YYGNN |
| verify.authored | 2 | tests: authored_rewrites,c_backend,match_powerpc+3 | cb/verify/authored/(verify/authored/ package) | YY-YN |
| verify.authored.match | 674 | tests: c_backend,match_powerpc,recompile+2 | cb/verify/authored/match16.cpp +1 | YY-YN |
| verify.authored.match16 | 281 | tests: verification | cb/verify/authored/match16.cpp +1 | YY-YN |
| verify.authored.repair | 587 | tests: verification | cb/verify/authored/repair.cpp +1 | YY-YY |
| verify.authored.search16 | 212 | tests: authored_rewrites,verification | cb/verify/authored/search16.cpp +1 | YY-YN |
| verify.authored.shape | 210 | tests: authored_rewrites | cb/verify/authored/shape.cpp +1 | YY-YY |
| verify.compare | 228 | live cmds: all 17; UI; tests: plumbing,verdicts,verification | cb/verify/compare.cpp +1 | YY-NN |
| verify.concolic | 229 | live cmds: all 17; UI; tests: concolic | cb/verify/concolic.cpp +1 | YY-NN |
| verify.gate16 | 2 | live cmds: all 17; UI; tests: verification; tools: copy_walk,gate_teeth16,record; native gens: 1 | cb/verify/gate16/(verify/gate16/ package) | YYGNN |
| verify.gate16.contract16 | 385 | live cmds: all 17; UI; tests: verification | cb/verify/gate16/contract16.cpp +1 | YY-NN |
| verify.gate16.isolated16 | 470 | live cmds: all 17; UI; tools: gate_teeth16,record,replay; native gens: 1 | cb/verify/gate16/isolated16.cpp +1 | YYGNN |
| verify.gate16.runtime16 | 62 | tests: verification | none found | NN-YN |
| verify.gate16.scoreboard16 | 203 | nothing | cb/verify/gate16/scoreboard16.cpp +1 | YY-YN |
| verify.gate16.setup16 | 255 | tests: verification; tools: copy_walk,record | cb/verify/gate16/setup16.cpp +1 | YY-NN |
| verify.guided | 222 | live cmds: all 17; UI | cb/verify/guided.cpp +1 | YY-NN |
| verify.lane_common | 264 | live cmds: all 17; UI; tests: run_record,verdicts,verification | cb/verify/lane_common.cpp +1 | YY-NN |
| verify.lanes | 4 | live cmds: all 17; UI; tests: n_subjects,bytes,concolic+31; tools: gate_teeth16,trace16; native gens: 3 | cb/verify/lanes/(verify/lanes/ package) | YYGNN |
| verify.lanes.dos | 75 | live cmds: all 17; tests: platforms,verification | cb/verify/lanes/dos.cpp +1 | YY-NY |
| verify.lanes.driver | 567 | live cmds: all 17; tests: drivers,platforms,run_record; native gens: 1 | cb/verify/lanes/driver.cpp +1 | YYGNY |
| verify.lanes.gb | 41 | tests: platforms | none found | NN-YY |
| verify.lanes.genesis | 40 | tests: platforms | none found | NN-YY |
| verify.lanes.image_lane | 108 | tests: verification | cb/verify/lanes/image_lane.cpp +1 | YY-YY |
| verify.lanes.java | 225 | live cmds: all 17; UI; tests: bytes,console_patches,cpp03_bodies+23 | cb/verify/lanes/java.cpp +1 | YY-NN |
| verify.lanes.msx | 6 | tests: platforms | cb/verify/lanes/msx.hpp | YN-YY |
| verify.lanes.nes | 38 | tests: nes,platforms | none found | NN-YY |
| verify.lanes.powerpc | 393 | live cmds: all 17; tests: platforms | cb/verify/lanes/powerpc.cpp +1 | YY-NN |
| verify.lanes.sms | 39 | tests: platforms | none found | NN-YY |
| verify.lanes.win16 | 727 | live cmds: all 17; UI; tests: platforms,run_record,verdicts+1; tools: gate_teeth16,trace16; native gens: 2 | cb/verify/lanes/win16.cpp +1 | YYGNN |
| verify.lanes.win32 | 917 | live cmds: all 17; UI; tests: n_subjects,concolic,native_trace+4 | cb/verify/lanes/win32.cpp +5 | YY-NN |
| verify.lanes.z80_flat | 52 | nothing | none found | NN-YY |
| verify.lanes.zx | 6 | tests: platforms | cb/verify/lanes/zx.hpp | YN-YY |
| verify.measure | 40 | nothing | cb/verify/measure.cpp +1 | YY-YY |
| verify.reconstruct | 305 | live cmds: all 17; UI; tests: kind,platforms,verdicts+2; tools: causes,failures,oss_bench; native gens: 1 | cb/verify/reconstruct.cpp +1 | YYGNN |
| verify.sweep | 99 | nothing | cb/verify/sweep.cpp +1 | YY-YY |
| verify.teeth | 256 | live cmds: all 17; tests: verification; tools: gate_teeth16 | cb/verify/teeth.cpp +1 | YY-NY |
| verify.verdicts | 215 | live cmds: all 17; UI; tests: native_engine,verdicts,verification; tools: gate_teeth16,replay,verdict_diff | cb/verify/verdicts.cpp +1 | YY-NN |

### codebreaker.xna

| module | lines | reached by | native replacement | 1-5 |
|---|---|---|---|---|
| xna | 3 | tests: xbox_effect,xna | cb/xna/(xna/ package) | YY-YY |
| xna.fixups | 170 | tests: xna | cb/xna/fixups.cpp +1 | YY-YY |
| xna.mgfx | 162 | tests: xbox_effect | cb/xna/mgfx.cpp +1 | YY-YY |
| xna.port | 810 | tests: xna | cb/xna/port.cpp +1 | YY-YY |
| xna.run | 212 | tests: xna | cb/xna/run.cpp +1 | YY-YY |

