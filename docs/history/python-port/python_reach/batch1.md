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
