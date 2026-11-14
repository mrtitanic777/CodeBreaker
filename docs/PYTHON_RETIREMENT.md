# Retiring the Python

Plan, written 2026-11-13 against master `ede5a3636`. Nothing in it is built or changed yet: it is the survey and the
order of work. It makes concrete docs/CPP_DESIGN.md section 8 ("Retiring the Python") and section 9 decision 3
("Terminal UI stays Python").

The end state, in Logan's words: every command runs on `native/cb`; the Python that remains is the terminal UI
(`codebreaker/ui/terminal*` and what it needs to run); `native/ported`, `native/pyrt`, `native/typed`,
`native/pyengine` and the dynamic-translation build are gone.

Numbers below were counted in the tree (552 Python files / 169,110 lines under `codebreaker/`; 144 test files /
23,273 lines under `tests/`; 30 files / 4,688 lines under `tools/*.py`; 48 files / 5,682 lines of golden generators
inside `native/cb`; `native/cb` itself 590 `_test.cpp` files and 2,291 sources).

## 0. What the finding is, in five lines

1. 51 of the 72 commands in `codebreaker/__main__.py` run natively (`NATIVE`, `kNative`). 20 are still Python; the
   21st (`ui`) is the terminal and stays Python. For 18 of the 20 the native module and its `cli::Command` already
   exist in `native/cb`; the work is registering them, comparing them to the Python, and listing them. Only
   `build16` (no command declaration) and `invert` (needs an Environment wired to the compilers) need more.
2. The terminal UI is **not** a standalone Python program today. Its static imports are tiny (7 modules) but it
   imports `ui.mcp_server` (1,270 lines) to ask "is this name a tool?", and `ui.service` (which imports the corpus,
   the ledger, the module writer and the store), so the lazy closure is **419 of 552 modules**. It has to be cut
   into a thin client first (section 2).
3. `native/cb` **finds its checkout by `codebreaker/__main__.py`** (`base/config.cpp:83`) and **reads ~200 data
   files out of `codebreaker/`** (JSON tables, Java API maps, toolchain profiles, runtimes). Deleting the Python
   package without moving those breaks the native program (section 4.4).
4. Production code in `native/cb` includes nothing from `native/typed`, `native/ported`, `native/pyrt` or
   `native/pyengine`. Only the shim tests, `cb_shim_check` and the CMake lines that feed them do (section 4.1).
   `native/cb` still compiles sources owned by other native dirs (zlib, sqlite, xex reader, pyfront; 4.3).
5. The all-15 boot, the Setup chain and the slow suites have no committed native driver that runs them for real
   (the Python ones are what ran them). They must be ported or recorded before the Python goes (section 6, step 6).

## 1. Commands still on the Python

`NATIVE` lists 51; `kNative` in `native/cb/app/native_commands.cpp:58` is declared `std::array<..., 52>` with 51
initializers (the last element is an empty string_view: harmless, fix it when adding names). The 21 not native:

| Command | Python module | Native piece that exists | What is missing |
|---|---|---|---|
| `media` | formats/media | `formats/media.hpp` `command(Environment)`, `media_command.cpp` | register; compare on a floppy and a CD image |
| `fonts` | formats/fonts | `formats/fonts.hpp` `command(Environment)` | register |
| `gfx` | formats/gfx | `formats/gfx.hpp` `command(Environment)`, `gfx_ttf.cpp` | register; compare a font as TTF |
| `disc` | build/disc_image | `build/disc_image.hpp` `command(Environment)` | register |
| `boot-floppy` | build/floppy | `build/floppy.hpp` `command(Environment)` | register |
| `xex-imports` | formats/xex_imports | `formats/xex_imports.hpp` `command(Environment{tools, names})` | register; ordinal-name table wiring as `app/cmdrun_main.cpp:69-85` already does |
| `xex-edit` | formats/xex_edit | `build/xex_edit.hpp` `command(Environment)` | register |
| `ppc-calls` | analyze/ppc_calls | `analyze/ppc_calls_command.cpp` `command(Environment)` | register (same Environment as xex-imports) |
| `system-ui` | formats/xbox/system_ui | `formats/xbox/system_ui.hpp` `command(Environment)` | register |
| `names` | symbols/names | `symbols/names.hpp` `command(Environment)`, `names_command.cpp` | register with `symbols::Environment{corpus, config}` as `identify` is |
| `driver-services` | symbols/driver_services | `symbols/driver_services.hpp` `command(Environment)` | register |
| `origin` | analyze/origin | `analyze/origin.hpp` `command(Environment{scratch,...})` | register; scratch folder under `state/` |
| `coverage` | coverage/ledger | `coverage/ledger.hpp` `command(Services)`, `ledger_render.cpp`, `ledger_services.cpp` | register with the Services the corpus and function store give |
| `cache` | base/cache | `base/cache_command.hpp` `command(CommandEnvironment)` | register |
| `facts` | engine/show | `engine/show.hpp` `command(Environment)` | register (engine facts store) |
| `knowledge` | knowledge/show | `knowledge/show.hpp` `command(Environment)` | register |
| `tidy-state` | base/tidy_state | `base/tidy_state.hpp` (Layout, Rule, a run function returning the exit code) | no `cli::Command`: a raw runner over `Layout{state, converted}` and the cache's prune hook |
| `progress` | ui/progress | `ui/progress.hpp` `command(ReadFn, stop_token)`, `progress_store.hpp` `store_reader(services)` | register with the store reader |
| `invert` | invert/run | `invert/command.hpp` (raw runner; every tool native, `invert/module_pass`) | register with `invert::command::Environment{retail, compile_msc6, compile_msvc152}` from `build/msc6`, `build/msvc152` and the corpus |
| `build16` | build/period16 | `build/period16.hpp` (build, compare, normalize: the whole flow) | **the command is not declared anywhere** (the header says "declared with the app's commands"; only the catalog row exists): write the `cli::Command` (`<source folder> [--original M] [--work DIR] [--stub DONOR]`) and wire the DOS runner (`build/dos_run.hpp`) |
| `ui` | ui/terminal | none: it is the Python terminal | the exe launches it (section 5) |

Everything but `build16`/`invert`/`ui` is registration plus parity: one `table.set(...)` in
`register_native_commands`, the name in `kNative` and `NATIVE`, and a comparison case against a golden recorded from
the Python (section 5.3). The harness programs `cb_cmdrun` (media, fonts, gfx, disc, boot-floppy, xex-imports,
xex-edit, ppc-calls, system-ui, crypto) and `cb_toolrun`, `cb_pipeline`, `cb_analyze_run`, `cb_cmds` already build
the Environments for the ones they cover: move those blocks into `native_commands.cpp` rather than rewriting them.

Python-only jobs that are not commands but are used by them (check, then drop or port): `codebreaker/port/`
(9 files, 10,494 lines, imported by nothing in `codebreaker/`, not a command; native `cb/port` exists),
`tools/py2cpp.py`, and the generator behind `decompile/vmx128.cpp`'s table (`python -m codebreaker.decompile.vmx128
--sleigh`, noted in the generated file).

## 2. The terminal UI: what it keeps and how it talks to the native side

### 2.1 Today

`codebreaker/ui/terminal.py` (848 lines, Textual/Rich) imports at top level only `verify.verdicts` (215 lines) and,
and uses `base/settings.py` (286 lines, imports only `json/os/sys`). The rest is lazy:

- `Store.m` imports **`ui.mcp_server`** (1,270 lines) only to read `TOOLS` and the tool functions' names; every
  screen call then goes `service.call("mcp.<tool>", ...)`.
- `ui.service` (508 lines) holds both the *client* (Client, find, launch, call, OWNER, START, stop_mine) and the
  Python *server side* (Jobs, ACTIONS, CALLS, Router, `_functions`), and imports `base.corpus`, `coverage.ledger`,
  `source.module_writer`, `store.functions` at top level. It falls back to answering **in process** (via
  `mcp_server`) when no service is reachable.
- `verify/verdicts.py` calls `engine/native_engine.py` (ctypes onto `cbengine.dll`) for `verdict/klass/classify/match`.
- `service.launch` starts `[sys.executable, "-m", "codebreaker", "serve"]`; `Jobs.launch` starts
  `[sys.executable, "-u", "-m", "codebreaker", action, ...]` (below normal priority, output in `state/ui_jobs/`).

Closure computed by AST walk (top-level imports only / including lazy imports): **7 / 419** of 552 modules.

### 2.2 The wire the native side already speaks

`native/cb/ui/serve.hpp`: `codebreaker serve` (HTTP+JSON on 127.0.0.1, token in `<state>/service/token`, record in
`<state>/service/service.json`; `GET /ping`, `POST /call {name,args,owner}`, `POST /stop`). The call names are the
Service's (`functions`, `store.overview`, `store.latest`, `jobs.*`, `cli`) and `mcp.<tool>` for every MCP tool.
`serve.hpp` says explicitly that the Python terminal's client speaks the same wire. `native/cb/ui/` also has the
MCP server (`mcp_*.cpp`, stdio and HTTP), `service_jobs.cpp` (the jobs), `progress*.cpp`. So the UI needs no
Python server code at all.

The terminal's calls (the keep list of tool names): `triage`, `jobs`, `job`, `stop_job`, `functions`, `function`,
`xrefs`, `disasm`, `run_command` (measure), `decompile_function`, `name_function`; and `store.overview`,
`store.latest`.

### 2.3 The keep set (after the cut)

| File | Lines | Why |
|---|---|---|
| `codebreaker/__init__.py` | 18 | package marker |
| `codebreaker/ui/__init__.py` | 0 | package marker |
| `codebreaker/ui/terminal.py` | 848 | the UI |
| `codebreaker/ui/terminal.tcss` | - | its stylesheet (data file, kept beside it) |
| `codebreaker/base/__init__.py`, `codebreaker/base/settings.py` | 286 | reads `settings.json`, state folder; pure stdlib. (Or a trimmed copy `ui/settings_lite.py`: the UI needs only `get("state")`, `state(name)`, `ROOT`.) |
| `codebreaker/verify/__init__.py`, `verify/verdicts.py` | 215 | the verdict vocabulary the UI colours rows with; **must lose its `native_engine` calls** (constants plus a dict; `classify` is a service call or a copy of the word table). The vocabulary is the same one `cb/verify` owns: generate the Python table from it, or have `store.latest` return the classes |
| `codebreaker/ui/client.py` (new, ~180 lines) | | the client half of `ui/service.py`: `Client`, `find`, `launch`, `call`, `client`, `OWNER`, `START`, `stop_mine`, `mode`, `folder`, `record`. Its `launch` runs `codebreaker.exe serve` (path from `settings.json` or `CB_EXE`, default `state/cxx/codebreaker/build/codebreaker.exe`) instead of `sys.executable -m codebreaker serve`; jobs are the service's (native `service_jobs.cpp`), so `Jobs` is not kept |
| `codebreaker/ui/progress.py` | 137 | **not** kept: `progress` is a native command; the UI polls `STORE.jobs()` |

`Store` becomes an explicit `__getattr__` over a fixed tuple of the tool names above (no `mcp_server`), and drops
the "answer in process" fallback: no service means start `codebreaker.exe serve`, and if that fails say so.
Outside Python packages: `textual`, `rich` (as `requirements.txt` already lists; drop `capstone`, `unicorn`,
`z3-solver`, `mcp`, `pyflakes`, `pytest`, `Pillow`). Result: 6 Python files plus a stylesheet, about 1,550 lines.

How the UI talks to native: (a) `serve` over HTTP (all queries and jobs; started on demand by the client);
(b) the MCP tools through the same `serve` (`mcp.*` names); (c) it never imports CodeBreaker modules, never runs
`python -m codebreaker`. `codebreaker.exe ui` (and no-argument-in-a-terminal) starts it: `main.cpp` spawns the
Python (setting `python`, else `py -3`/`python` on PATH) on `-m codebreaker.ui.terminal`, with `CB_EXE` set to
itself. If Textual is missing it prints the same "needs Textual" line the Python prints today.

Check: `python -I -c "import codebreaker.ui.terminal, sys; print(sorted(m for m in sys.modules if m.startswith('codebreaker')))"`
prints exactly the keep set; a Textual Pilot run (tests/test_ui.py adapted) against a native `serve` passes.

## 3. Every other Python module, who imports it

Counts are files / lines, then how many test files (`tests/`) and tool/experiment files import the package. "Imported
by" is by other `codebreaker` packages (lazy imports included). Once every command is native, the only importers of
the groups below are other doomed modules, `tests/`, `tools/` and `codebreaker/__main__.py` (which loads command
modules by string from `COMMANDS`), so all are deletable; none is needed by the keep set.

### 3.1 Server side of the UI (superseded by `native/cb/ui`)

`ui/mcp_server.py` (1,270; also imports analyze, compare, coverage, decompile, java, source.placement, symbols),
`ui/serve.py` (335), `ui/service.py` (508, minus the client half moved to `ui/client.py`), `ui/progress.py` (137).
Importers: `terminal.py` (the lazy fallback, removed in the cut), `__main__`, `tests/test_ui.py`, `test_mcp.py`,
`test_service.py` (4 test files).

### 3.2 Command implementations, native counterpart exists (51 native + the 18 to wire)

| Package | Files | Lines | tests | tools | imported by (packages) |
|---|---|---|---|---|---|
| translate | 41 | 23,733 | 24 | 4 | source 30, decompile 15, java 14, port 4 |
| formats | 91 | 18,997 | 30 | 7 | source 90, build 33, exact 32, verify 32, platforms 26, symbols 25 |
| java | 67 | 18,271 | 61 | 0 | translate 22, verify 4 |
| exact | 50 | 16,218 | 6 | 1 | build 5, port 5 |
| source | 47 | 13,687 | 9 | 0 | `__main__` 10, symbols 8, platforms 8 |
| build | 30 | 11,035 | 24 | 3 | source 51, platforms 22, verify 18 |
| port | 9 | 10,494 | 2 | 0 | none (no command names it) |
| decompile | 19 | 9,165 | 19 | 9 | verify 19, build 15, emulate 12 |
| verify | 37 | 8,451 | 41 | 10 | engine 15, build 13, translate 10 |
| recompile | 19 | 7,802 | 3 | 1 | `__main__` 3, analyze 3 |
| emulate | 19 | 7,394 | 11 | 4 | verify 40, platforms 10 |
| analyze | 17 | 4,642 | 5 | 0 | source 9, decompile 7 |
| symbols | 19 | 3,929 | 7 | 1 | source 19, verify 19, knowledge 10 |
| engine | 7 | 2,638 | 5 | 0 | verify 5, store 3 |
| invert | 28 | 2,492 | 1 | 0 | exact 2 |
| base | 10 | 1,994 | 26 | 12 | source 81, verify 50, build 47 (`settings` stays) |
| xna | 5 | 1,357 | 2 | 0 | `__main__` 1 |
| compare | 3 | 961 | 2 | 0 | symbols 6, source 4 |
| platforms | 14 | 801 | 7 | 0 | verify 19, source 7 |
| coverage | 3 | 716 | 2 | 0 | ui 7 |
| knowledge | 5 | 584 | 3 | 0 | symbols 21, engine 7 |
| jobs | 2 | 287 | 2 | 0 | verify 8, build 6 |
| store | 2 | 126 | 8 | 4 | verify 10, ui 6 |
| `__main__` | 1 | 220 | 0 | 2 | ui 2 |

(Sum 552 files, 169,110 lines. The keep set takes out `ui/terminal`, `base/settings`, `verify/verdicts` and the
`__init__`s.)

### 3.3 The ctypes/native bridges (retired with the DLLs, 4.2)

30 files: `formats/native.py`, `engine/native_engine.py`, `symbols/native.py`, `analyze/native.py`,
`invert/native_invert.py`, `translate/native_l2.py`, `emulate/{native_driver,native_shadow,native_trace,native_win16,
fast_uc}.py`, `exact/emu/{dos,native_dosexec}.py`, `coverage/reach.py` (ctypes), `base/parallel.py`,
`formats/xbox/crypto/{build_lib,lzx,nand_tool,shadowboot,StreamIO,xcp_dumper,xdk_extract,XeCrypt}.py`,
`port/{asm16,__main__}.py`, `translate/frontends/clang/{ctypes,lower,__init__}.py`,
`translate/frontends/python/program.py`. Importers: the modules in 3.2 and the tests that call
`native.call(...)` (20 modules call it). Gone with their packages.

### 3.4 Outside the package that imports it

- `tests/`: 144 files, 138 import `codebreaker` (and `tests/run_all.py` runs them inside `codebreaker.exe`).
- `tools/*.py`: 30 files, 26 import `codebreaker` (`check_imports.py`, `tidy.py`, `freeze_tests.py`, `golden.py`,
  `merge.py`, `l2check.py`, `verdict_diff.py`, ...). Each is dropped or rewritten as a `codebreaker.exe` command
  or a C++ tool; `tidy.py` and `check_imports.py` are gate members today and die with the gate.
- `native/cb/**.py`: 48 golden generators / compare scripts (`gen_*.py`, `make_*golden*.py`, `compare_mcp.py`),
  5,682 lines, import `codebreaker`. The goldens they made are checked in; the generators are deleted (git history
  keeps them), the same way `tests/data/native_engine_golden.txt` already outlived its Python.
- `experiments/`, `targets/`: no `.py` file there mentions `codebreaker` (grep, 2026-11-13).
- Sibling repos (Dashboard, Windows95, Win95VM, XboxMCP): `docs/NATIVE.md` records that outside callers use
  `codebreaker.exe` commands (`prepare_game.py` -> `xbox-disc`, `recompile-project`; `place_map.py` -> `detect`).
  **Re-check just before deleting** with `grep -rn "import codebreaker\|from codebreaker\|-m codebreaker"` run inside
  each named sibling repo folder (never over the drive).
- `.mcp.json` already runs `state/cxx/codebreaker/build/codebreaker.exe mcp`: keep that output path stable.

## 4. What in `native/` depends on what is to be retired

### 4.1 `native/typed`, `native/ported`, `native/pyengine`'s `big.cc`

Production code under `native/cb` includes none of them (grep of `#include "py*.hpp" / "codebreaker/..."` finds
hits only in `compat/` (113 files, 82 `.cpp`), the `*shims*_test.cpp` files (~25), `exact/**/shims*_test.cpp`,
`java/shims*_test.cpp`, `invert/shims_test.cpp`, `recompile/*shims_test.cpp` and one golden header). The CMake
wiring that keeps them alive, in `native/cb/CMakeLists.txt`:

- lines 150-153, 175, 184-187: `/W3 /WX-` options for the shim tests;
- lines 245-351: `cb_shim_check` (an OBJECT library of `compat/shim_check_*.cpp` plus ~60 `native/typed/cpp/codebreaker/**.cpp`
  sources, with `native/ported/include`, `native/typed/cpp`, `native/l2/src` on its include path);
- lines 382-392: `cb_tests` adds the same include directories and compiles `native/pyengine/src/big.cc`
  (the pytypes shim test needs `Big`).

Deleting `native/typed`, `native/ported`, `native/cb/compat`, the shim tests and those CMake blocks leaves
`native/cb` self-sufficient. Gate: `grep -rln "py\(_[a-z]*\)\?\.hpp\|native/typed\|native/ported" native/cb` is empty
(CPP_DESIGN 6.5 step 2). Sizes: `native/typed` 579 files (151 `.cpp`) plus `codebreaker.compare.image.json`,
`native/ported` 27, `native/pyrt` 46, `native/pyengine` 383, `native/cbexe` 2 (`commands.cc`, `settings.cc`: the old
`codebreaker_native` module, whose commands are native now): about 1,037 files, roughly 170k lines.

### 4.2 The per-library DLLs

| Library | Built by | Loaded by | After |
|---|---|---|---|
| `formats.dll` (`native/formats`) | `build_native.cmd formats` | Python `formats/native.py` | gone with the Python; **but** `cb_zlib` compiles `native/formats/core/zcrc32.c` (CMake line 95): move that file to `native/third_party/zlib/` first |
| `cbengine.dll` (`native/engine`) | `build_native.cmd engine` | Python `engine/native_engine.py` (and `verify/verdicts.py`) | gone; **but** `cb_sqlite3` compiles `native/engine/third_party/sqlite` (line 108, the CMake comment already says it moves): move to `native/third_party/sqlite/` |
| `cbinvert.dll` (`native/invert`), `cbanalyze.dll` (`native/analyze`), `cbsymbols.dll` (`native/symbols`) | `build_native.cmd` | Python `*/native*.py` | gone. Native cb uses none (the names in `engine/facts.hpp`, `analyze/native.hpp` are comments) |
| `cbtrace.dll` (cb's own `emulate/tracer` exported, CMake 365-374) | `build_native.cmd cb` | Python `emulate/native_trace.py`, the dynamic exe | drop the target with the Python callers; the tracer stays as `cb_emulate` |
| **`cbl2.dll`** (`native/l2`) | `build_native.cmd l2` | **`cb/translate/native_l2.cpp`** (`Library::default_dll`) | **keep** (the L2 C back end); consider linking it statically later |
| **`cbdos.dll`** (`native/dosexec`) | `build_native.cmd dosexec` | **`cb/exact/emu/native_dosexec.cpp`, `cb/port/verify.cpp`** | **keep** |
| **`decompiler.exe`** (`native/decompiler`) | Visual Studio generator | **`cb/decompile/decompiler.cpp`** (`decompiler_bin`) | **keep**; `native/decompiler/core/zlib` is also the zlib `cb_zlib` builds |
| `unicorn.dll`, `capstone.dll`, `libz3.dll` (`native/third_party/bin`) | prebuilt | `cb/core/*_api.cpp` | **keep** (decision 9.1/9.2) |
| `testkit.exe` (`native/testkit`) | `build_native.cmd testkit` | `tests/run_all.py` | keep for now (4.4, 5.3) |

`native/xex` (the XEX reader, `reader.cmake`) is shared with the x360 runtime: keep. `native/pyfront` (Python parser)
is `cb_pyfront`, the translator's Python front end: **keep** (it is not the Python runtime). `native/nasm`, `x360`,
`xenonrecomp`, `rexglue`, `retromcp` are unrelated to the retirement.

### 4.3 Sources `native/cb` borrows from other native dirs

`native/xex` (keep), `native/pyfront` (keep), `native/third_party` (keep), `native/l2/src` (headers; keep),
`native/formats/core/zcrc32.c`, `native/decompiler/core/zlib/*.c`, `native/engine/third_party/sqlite` (these
three move to `native/third_party/` before their parents are deleted), `native/pyengine/src/big.cc` (dies with
the shim tests).

### 4.4 The data native reads out of `codebreaker/` (the blocker; done in step 3, this is the survey that was)

`base/config.cpp:83` recognises a checkout by `codebreaker/__main__.py` + `native/`. These native sites read files
under `codebreaker/` (checked in as non-`.py` files, ~200 of them: 87 `.h`, 43 `.json`, 42 `.cpp`, 12 `.cs`,
6 `.java`, `.txt`, `.csproj`, `.c`):

- `app/native_commands.cpp:275`, `decompile/decompiler_lanes.cpp:202`: `symbols/dos_interrupts.json`
- `symbols/identify.cpp:169`: `symbols/runtime_signatures.json`; `symbols/workbench.cpp:127`: `ordinal_names.json`
- `recompile/command.cpp:48`: `symbols/console_ordinals.json`; `knowledge/sources.cpp:59`: `symbols/`, `knowledge/models/`
- `engine/lane_kit_build.cpp:196`: `build/kept_as_asm.txt`
- `source/pipeline.cpp:83`, `pipeline_module.cpp:363`: `source/placement.json`
- `exact/toolchain.cpp:151`: `exact/toolchains/` (13 files)
- `java/paths.cpp`, `java/production.cpp`, `java/tools.cpp`, `engine/lane_kit_java.cpp:53`, `source/pipeline_module.cpp:347`:
  `java/` (api maps, platform reference and canonical sets: about 140 files) and
  `translate/frontends/java/TreeDump.java`
- plus runtimes/templates the translators and `port`/`xna` ship (`translate/runtime/*`, `translate/dotnet/runtime`,
  `translate/backends/java_runtime/crt`, `xna/runtime`, `xna/apistubs`, `xna/shadercheck`, `port/runtimes`,
  `port/platform`)

Plan: `git mv` them to a top-level `data/` (same sub-paths: `data/symbols/...`, `data/java/...`), update the ~20
sites above through one `config::data_root()` accessor, and change the checkout marker to `native/CMakeLists.txt`
+ `settings.example.json`. Do this **before** any Python deletion (step 3).

## 5. The program, the build, the gate

### 5.1 `codebreaker.exe` without pyengine

Today `tools/build_codebreaker_exe.cmd` runs `pyengine.exe --dynamic` over `codebreaker/`, `tests/`, `tools/`
(`--dyn-tree`, `--dyn-files`), `native/cbexe` and `native/typed`, generates a CMake project, and builds it with
Ninja (about 15 minutes `fast`, longer at /O2); the `codebreaker.exe` it links contains `pydyn`, `pyrt` and the
`codebreaker_native` shim, and `codebreaker/__main__.py` hands native commands to `cb_native_command`
(`native_commands.cpp:434`) first. `-DCB_WITH_PYTHON=ON` is the CMake option that describes it (`native/CMakeLists.txt`).

Target state: a plain native `main`:

```
native/cb/app/main.cpp      int main(int, char**)
                              1. cb_host_main_hook-equivalent (app/host_hook.cpp): run_as_worker for the lane kit's and
                                 analyze's worker processes, else mark this process the host
                              2. args = argv; no command + terminal, or `ui`  -> launch the Python terminal (2.3)
                              3. return cb::app::run_native_command(args)    (CommandTable over native_commands.cpp)
native/cb/CMakeLists.txt    add_executable(codebreaker app/main.cpp app/host_hook.cpp cb.manifest)
                            target_link_libraries(codebreaker PRIVATE cb_app cb_core); cb_copy_prebuilt_dlls(codebreaker)
```

The `CB_WITH_PYTHON` option and the `codebreaker_native` shim are removed (`cb_pyfront` stays: it is the translator's parser).
`CommandTable::fallback_` (hand the line to the translated `__main__`) is deleted; an unknown command is a usage
error. The harness mains `cb_cmdrun`, `cb_toolrun`, `cb_pipeline`, `cb_analyze_run`, `cb_cmds`, `cb_serve`,
`cb_mcp_stdio`, `cb_source_commands` (all EXCLUDE_FROM_ALL, proof harnesses of command layers) are deleted once the
exe runs those commands: their job becomes the exe's comparison cases.

Output path stays `state/cxx/codebreaker/build/codebreaker.exe` (`.mcp.json`, `tests/run_all.py` `CB_EXE`, the
Dashboard and Windows95 tools expect it): `tools/build_codebreaker_exe.cmd` becomes ten lines, `cmake --build
state/cxx/cb --target codebreaker` and a copy to that path (keeping its rename-aside of a running exe, which the
current script does for the background service). `tools/build_native.cmd` default list becomes `l2 dosexec testkit`
(`cb` and the rest of the old list drop out); `tools/build_rt_exe.cmd` (pyrt) is deleted. Release and `asan`
configurations of `tools/build_cb.cmd` are unchanged and now build the real exe too.

### 5.2 What the old gate does and what replaces it

`tests/run_all.py` runs, inside `codebreaker.exe`, 54 suites (`SUITES`: 51 quick, 3 slow: `test_x360`,
`test_frozen_slow`, `test_verification`), with groups for DOSBox and service ports, `--quick`, `--changed` (via
`native/testkit plan`, which reads `tools/check_imports.py`'s Python import graph), and a g++/javac compile cache
(`testkit lookup/store`). It rebuilds the pyengine exe first when any Python/tests/tools file is newer.

Replacement, `tools/gate.cmd` (and `tools/build_cb.cmd gate`):

1. Build `codebreaker` and `cb_tests` (Release; `area <name>` builds stay for development, `CB_BUILD_JOBS=4`,
   `disk_guard.cmd`).
2. `cb_tests` (all 590 test files, the module tests) - the unit/regression layer, plus `shims` is gone.
3. **`cb_exe_gate`**, new: a small C++ program (`native/cb/app/exe_gate_main.cpp`, data in
   `tests/commands/cases.json` + `tests/commands/golden/`) that runs the *real* `codebreaker.exe` as a subprocess for
   every catalog command: `--help`, and one or more cases per command (`argv`, input files from `tests/fixtures`,
   `tests/data`, `native/cb/*/testdata`, or the corpus when present, normalised comparison per CPP_DESIGN 7: error
   text and unordered output ignored). Goldens are **recorded from the Python before it is deleted** (step 0) for the
   18 wired commands and kept from the existing compare scripts' outputs for the 51 already native. Also: an MCP stdio
   handshake plus one tool call, `serve` ping/call/stop, and the terminal smoke (2.3 check).
4. Slow tier (`--slow`): the old slow suites' native equivalents (see step 6).

`tools/check_imports.py` and `tools/tidy.py` (pyflakes) leave the gate with the Python. `testkit` keeps the
compile cache for the Java and C++ round-trip tests (`g++`/`javac` runs in the translator tests) and its `plan`
is re-based on the C++ include graph by area (`build_cb.cmd area` already gives the unit of change); until then
`--changed` is replaced by the area build.

The 144 Python test files each get one of three fates, recorded in a table `docs/PYTHON_TESTS_FATE.md` (made by
step 0): (a) already covered by a `cb_tests` module test (most of `test_native_*`, `test_exepack`, `test_pklite`,
`test_dpmi`, `test_z80`, `test_m68k`, ... - confirm by name), (b) turned into an `exe_gate` case (command-level
tests: `test_inspect_render`, `test_media`, `test_xex*`, `test_ppc_calls`, `test_recompile`, `test_xbox_*`), (c)
retired with the Python because it tests Python-only code (`test_plumbing`, `test_store`, `test_service`,
`test_mcp` Python side, `tests/translate` pytest of the Python front end). The two big ones to plan: `tests/java`
(61 files, the Java transpiler, pytest) and `tests/translate` (24 files); `test_frozen*` are frozen outputs of the
Python (they become golden data for `cb_tests` as they are).

## 6. Ordered deletion plan

Each step ends with `tools\build_cb.cmd` (full, release) and the gate as it is at that step; a step does not start
until the previous one is green. Branch `retire-python`, cut from master; tag master `python-last` first.

**Step 0 - Freeze and record (no deletions).**
Tag `python-last`. For every one of the 72 commands record golden output from the Python exe on real inputs into
`tests/commands/golden` (this is the last time the Python is the oracle for the 18 unwired). Write the 144-test fate
table (5.2). Record timings of `run_all --quick`, a sweep sample and the all-15 boot as the baseline (STATUS.md has
the 2026-10-30 baseline: boot PASS twice; sweep 10,688 verified of 363 compared). Check: goldens re-run clean
against the Python (determinism); table has no test without a fate.

**Step 1 - Wire the 18 commands (3 batches), then `build16`, `invert`.**
Batch A readers: `media fonts gfx disc boot-floppy xex-imports xex-edit ppc-calls system-ui` (blocks move from
`cb_cmdrun_main`). Batch B store/UI: `cache facts knowledge tidy-state progress coverage names driver-services
origin`. Batch C: `invert` (Environment from `build/msc6`, `build/msvc152`), `build16` (declare the command, DOS
runner). Per command: `kNative` + `NATIVE` + `table.set`, an `exe_gate` case compared with its golden, area test
(`tools\build_cb.cmd area app`). Fix the `std::array<..., 52>` size. Check: `NATIVE` equals every `COMMANDS` key but
`ui`; `python -m codebreaker <each>` and `codebreaker.exe <each>` agree on the goldens; the unit tests of the
touched areas pass.

**Step 2 - The plain native exe.**
Add `app/main.cpp` and the `codebreaker` target; build with no `pydyn`. Keep `build_codebreaker_exe.cmd` building
the pyengine exe in parallel for this one step only, as the comparison reference. Check: `cb_exe_gate` all green on
the plain exe; `codebreaker.exe` with no argument in a terminal reaches the `ui` launcher; `mcp` and `serve` work
from `.mcp.json`; exe size and start time recorded (no CPython-world start-up: should be faster).

**Step 3 - Move the data out of `codebreaker/`. DONE (2026-11-13).**
190 non-Python files moved with `git mv` semantics to `data/` (same layout under it: `data/symbols`, `data/java`,
`data/exact/toolchains`, `data/knowledge/models`, `data/translate`, `data/xna`, `data/port`, `data/source`,
`data/build`); `config::data_root()` added; every native read site and every Python reader points at `data/` (no
copies, no links); the checkout marker is `data/` + `native/`. Not moved because nothing native reads them:
`formats/xbox/{empty_live.bin,THIRD_PARTY.txt,crypto/*}`, `ui/terminal.tcss`, `translate/frontends/clang/corpus/*`,
`translate/frontends/csharp/CsTreeDump.cs` (move them with the code that starts reading them). Original check: `grep -rn '"codebreaker" /' native/cb --include=*.cpp` (outside tests) is empty;
`cb_tests` and `cb_exe_gate` green with the folder `codebreaker/` temporarily renamed (proves nothing reads it).

**Step 4 - Cut the terminal UI. DONE 2026-11-13** (`ui/client.py`: stdlib only, finds/starts `<exe> serve` with
CB_EXE, `exe` in settings.json, state/cxx/codebreaker/build, or PATH; `Store` has the eleven calls by name;
`ServiceDown`; `tests/test_ui_thin.py` runs it with only `codebreaker/__init__.py` and four `ui/` files present;
`codebreaker/__init__.py` tolerates a missing `codebreaker.base`). Left from the plan: `ui/settings_lite` is inside
`client.py`; `ui/progress.py` and the Python server half (`ui/service.py`, `serve.py`, `mcp_server.py`) stay until
the Python goes. The original text:
Create `ui/client.py` (client half of `service.py`, launching `codebreaker.exe serve`), explicit tool names in
`Store`, verdicts without `native_engine`, drop the in-process fallback. Check: the `python -I` module-set check
(2.3) prints the keep set; the adapted `tests/test_ui.py` passes against the native `serve`; the UI on a real
module (measure, decompile, jobs) by hand once.

**Step 5 - Replace the gate.**
`tools/gate.cmd` with `cb_tests` + `cb_exe_gate`; port the (a)/(b) tests of the fate table; check the gate is green
twice in a row and no slower than `run_all --quick`'s recorded time (speed is a requirement: Logan 2026-11-12).

**Step 6 - Milestone regression, on the plain exe, before anything is deleted (Logan's rule: milestones only).**
Everything the Python suites and scripts covered that the quick gate does not:

- **All-15 MINI.CAB boot**: rebuild all 15 modules from source (`module`/`build`), boot Setup to the License page
  with `verify/gate16/setup16.cpp` (`continue_test`), each twice, from a fresh state, against the STATUS.md numbers
  (USER C 509/61 gated, GDI C 118/24, USER fixed segment 1 BAC4h; PASS twice). **There is no committed native driver
  for the real run** (`setup16` is a library; its tests use `cb_fake_qemu`, and the real run was Python/state-script
  work): commit one (`cb_boot15`, or `codebreaker.exe module --boot-check` over the 15) in step 5, with the
  QEMU/ISO paths from settings.
- **The Setup chain**: oemsetup/suwin/setupx rebuilt and run to Setup's Welcome (`phase2`/`phase3` runs) and the
  full install on all-ours modules to the desktop (`phase3-full-install`): same driver.
- **Slow suites**: `test_x360` (Xbox 360 recompile: Tetris, Minecraft, Up via `recompile-project --check`),
  `test_frozen_slow`, `test_verification` (reconstruct/judge, teeth, the period-compiler round trip) as their
  native equivalents; VxD lane (VKD boot: `vxd`/`driver-judge`), DOS lane (`dos-source`/`dos-build`).
- **Sweeps and speed**: a `sweep` sample (the 95 smallest 32-bit, 90 each of 16-bit/drivers/DOS) compared with the
  stored verdict tallies; `tests` speed tally in `docs/TYPED_CPP_SPEED.md` updated.
- Run each long job with workers = 4 and at most 3 VMs (Logan's load rule); check free space first
  (`disk_guard.cmd`); clean per-run copies afterwards.

Result is recorded in `docs/STATUS.md`. Any failure stops the plan: fix in C++ (and note the Python was the
reference), never restore the Python.

**Step 7 - Delete, in one branch (CPP_DESIGN 8 step 4), in this order, building and running the gate after each.**

- 7a. `tools/*.py` importers (26) and the 48 `native/cb/**.py` generators; then `tests/*.py` and `tests/java`,
  `tests/translate` once their fate-table rows are done (keep `tests/fixtures`, `tests/data` as data of
  `cb_exe_gate`; keep `tests/test_ui.py` adapted). Check: gate green; fixtures still read.
- 7b. `codebreaker/**` Python except the keep set (about 545 files, ~167k lines); `requirements.txt` to `textual`
  and `rich`; `pyproject.toml`: drop `[project.scripts]` and the capstone/unicorn dependencies (or delete the file
  if the UI needs no install: it is started by the exe). Check: `python -I -c "import codebreaker.ui.terminal"` works;
  `grep -rn "python -m codebreaker" . ` finds only history/docs.
- 7c. `native/cb/compat`, every `*shims*_test.cpp`, `cb_shim_check` and the CMake blocks (4.1), `tools/cb_shim`,
  `tools\build_cb.cmd shims`. Check: `grep -rln "py\(_[a-z]*\)\?\.hpp\|native/typed\|native/ported" native/cb` empty;
  `cb_tests` count drops by the shim tests only; ASan build green.
- 7d. `native/typed`, `native/ported`, `native/cbexe`; then `native/pyrt`, `native/pyengine`; `tools/py2cpp.py`,
  `tools/build_rt_exe.cmd`; rewrite `tools/build_codebreaker_exe.cmd` (5.1). Keep `native/pyfront`. Check: a clean
  configure from scratch (`cmake -S native`) has no reference to the deleted dirs; the exe builds in one pass.
- 7e. Old library projects: move `sqlite`, `zcrc32.c` and `decompiler/core/zlib` to `native/third_party` (4.2/4.3)
  then delete `native/engine`, `native/formats`, `native/symbols`, `native/invert`, `native/analyze`; remove the
  `cbtrace` target; set `build_native.cmd` to `l2 dosexec testkit`. Check: `build_cb.cmd` full and `cb_exe_gate`
  green; `native/l2`, `dosexec`, `decompiler` still found by the three loaders.
- 7f. Docs: `docs/ARCHITECTURE.md`, `docs/NATIVE.md` (names Python throughout), `README.md`, `docs/CPP_DESIGN.md`
  sections 6.5 and 8 (mark done), `docs/STATUS.md`, `docs/PYTHON_FRONTEND.md` (the translator's front end is native
  `cb_pyfront`/`cb/translate`: say so), the memory notes. Check: README build steps run on a clean machine with MSVC,
  CMake, Ninja and (for the UI only) Python with Textual.

**Step 8 - After the merge.** Re-run the milestone regression once on the merged master (step 6 list); run
`tools\clean_builds.cmd`; remove the worktree and its `state\cxx` build output (Logan's disk rule). The Python stays
in git history under tag `python-last`.

## 7. Risks and open questions for Logan

- **The terminal cut changes behaviour** (no in-process fallback): when `codebreaker.exe serve` cannot start the UI
  shows a message instead of answering locally. DONE (2026-11-13): `ServiceDown` says why (no program found and
  where it looked, the program stopped again with the tail of `<state>/service/serve.log`, no answer in 60 s) and `r`
  retries, starting the service when it is not running; the screens ask again once it is back.
- **Where the UI's Python lives**: keeping `codebreaker/ui/` keeps the import path stable and the `ui` launcher
  trivial; a top-level `terminal/` folder would stop a stray `codebreaker/` Python package from existing at all.
  The plan keeps `codebreaker/ui/` as the request words it ("`codebreaker/ui/terminal*` and what it needs").
- **Verdict vocabulary** lives in `cb/verify` (C++) and in `verify/verdicts.py`; the UI needs it for colours. DONE:
  the service returns classes (the `function` tool's `older_verdicts` carry `class`), and the UI keeps only its own
  colour/mark/label tables, held to the vocabulary by `tests/test_verdicts.py`.
- **Python tests that test Python-only behaviour** (service, store, mcp Python side, the Python front end) are
  retired, not ported: their native equivalents (`cb/ui/*_test`, `cb/translate`) already exist; confirm with the
  fate table.
- **The Java suite** (`tests/java`, 61 pytest files, and `codebreaker/java` 67 files / 18k lines) is the largest
  Python-only test body; its native counterpart is `cb/java` (shim tests are not behaviour tests). Budget a
  session for its fate rows.
- **Outside callers** (Dashboard, Windows95, Win95VM, XboxMCP) re-checked as in 3.4 before step 7b.
- **The exe's `ui` launcher needs a Python**: found by setting `python` or PATH; document in README.

## 8. The plain native program beside the old one (2026-11-13)

`tools\build_codebreaker.cmd` builds target `cb_codebreaker` (`native/cb/app/codebreaker_main.cpp`, no pyengine) in the
same tree as `tools\build_cb.cmd` (`state\cxx\cb`, objects shared) and installs it with unicorn/capstone/libz3 as
`state\cxx\codebreaker\native\codebreaker.exe`. `state\cxx\codebreaker\build\codebreaker.exe` (the translated Python,
`tools\build_codebreaker_exe.cmd`) is untouched. 25 MB (plus 35 MB of the three prebuilt DLLs) against 130 MB; start-up
about 10 ms.

- **`ui`** (and no command in a terminal) runs `python -m codebreaker.ui.terminal` in the console, with `CB_EXE` set to
  the program itself and `PYTHONPATH` the checkout. Python is `CB_PYTHON`, the setting `python`, then `python`, then
  `py -3` on the PATH (the first that runs `-c pass`); none: exit 2 with a message saying what to install. Textual's own
  missing-package line comes from `terminal.py`. This is the only Python the program starts.
- **One setting points everything at it**: `CB_EXE=<path to the exe>`. `codebreaker/ui/client.py` (`exe` setting or
  `CB_EXE`), `tests/run_all.py`, `tools\gate.cmd` (default `--exe` is `CB_EXE`, else the old build) and `.mcp.json`
  (`${CB_EXE:-<old build>}`) all read it. Unset, the old program is still the default.
- Unknown words now get the table's usage and exit 2 (a listed command that is not native still says so); the goldens
  `cli-help` and `cli-unknown` were re-recorded for the native wording (no `python -m`, the native catalog, usage on stderr).
- The native program still loads the prebuilt `native/<lib>/build/bin` DLLs (`cbl2.dll` for decompile, the xex program,
  ...): they must exist beside the checkout until those libraries are folded in.

## 9. The plain program is codebreaker.exe; pyengine is gone (branch `retire-pyengine`)

Done on the branch `retire-pyengine` (to be merged when the Python batches are done; details and numbers in
`state/work/typed-port/retire_pyengine.md`):

- `tools\build_codebreaker_exe.cmd` builds target `codebreaker` of the one `native/cb` tree (`state\cxx\cb`) and installs it, with
  unicorn/capstone/libz3, as `state\cxx\codebreaker\build\codebreaker.exe`. That is the path `.mcp.json`, `tests/run_all.py`,
  `tools\gate.cmd` and `codebreaker/ui/client.py` already used, so none of them changed their default. `tools\build_codebreaker.cmd`
  (the side-by-side build of section 8) is folded into it. `tools\build_cb.cmd` builds the program too (it is part of the default target).
- Deleted: `native/pyengine`, `native/pyrt`, `native/typed` (with the `compare.image` manifest), `native/ported`, `native/cb/compat`
  (shims and `py_bridge`), `native/cbexe`, the 15 `*shims*_test.cpp`, `tools/cb_shim`, `tools/build_rt_exe.cmd`; from
  `native/cb/CMakeLists.txt` the `cb_shim_check` library, the shim/compat test blocks, the `big.cc` source and the include roots of
  the old layer; `CB_WITH_PYTHON`; `tools\build_cb.cmd shims`.
- `tools\gate.cmd` is the one gate: `cb_tests`, `cb_exe_gate` on the real exe, then the UI's Python tests under real python. It builds
  and installs the exe when `--exe`/`CB_EXE` do not name one.
- `tests/run_all.py` no longer runs suites inside the exe (there is no translated Python): it runs them under real python, with `CB_EXE`
  set, and does not build anything. Nothing in the gate needs it. Keep it only while Python suites other than the UI's remain; delete
  it with the last of them (its compile cache and `--changed` plan live in `native/testkit` and the Python import graph, both Python-world).
- Still there, on purpose: the `cbtrace` target (the Python `emulate/native_trace.py` loads it) and `native/{engine,formats,symbols,invert,analyze}`
  (step 7e, after the Python callers go).
