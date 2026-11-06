# pyengine -- handoff (2026-11-05, phase 2): CodeBreaker as one C++ program

**State.** All of CodeBreaker -- codebreaker/ (554 modules), tests/, tools/, native/nasm/test/masm --
plus the standard modules CPython writes in Python (native/pyrt/stdlib.txt) and unicorn's and capstone's
bindings (native/pyrt/site.txt) translate into one program, 826 modules / 249k lines / 11,915 functions,
11 constructs refused (8 `async def`: ui/mcp_server, ui/terminal, 3 test files; 1 `match`: MemCrypto).
It **compiles and links** (MSVC, unity batches of 24 over a precompiled py_object.hpp; ~15 min cold /Od
with 2 jobs, a pyrt-only change relinks in ~2 min):

    tools\build_native.cmd pyengine pyrt
    tools\build_codebreaker_exe.cmd fast          -> state\cxx\codebreaker\build-fast\codebreaker.exe
    codebreaker.exe identify lz32.dll              (python -m codebreaker identify lz32.dll: the same bytes out)
    codebreaker.exe tests\run_all.py --quick      (the suite runner itself as C++; its jobs run codebreaker.exe)
    codebreaker.exe -m pytest tests\translate     (pytest in C++: lib_pytest.cc)

The program reads its command line as `python` does (module.cc run_main): `-m module` (a package: its
__main__; a C++ module like pytest: its _main), `script.py` (the module translated from that file),
interpreter flags skipped (-u -B -I -X opt ...); with no arguments it is `python -m codebreaker`. So
`sys.executable` children (`[sys.executable, "-m", "codebreaker", ...]`, run_all's `-X importtime path`,
`-m pytest`) run the same program.

### The general fixes (engine and runtime only; no generated C++ was edited)

Engine (src/dyn.cc, tools/pyengine_main.cpp): `--dyn-tree PKG`, `--dyn-files DIR` (files named as pytest
names them), `--dyn-exe`, `--dyn-stdlib LIB` / `--dyn-site DIR` (stdlib.txt / site.txt: `name`, `pkg.*`,
`alias = path`); unity + PCH CMake; MSVC's old keyword spellings (cdecl, _asm ...), errno / S_I* / O_*
macro names mangled; direct calls of names py:: also has kept out of ADL (`(is_int)(t)`); a lambda in a
default no longer drops the outer signature's cell parameters; a lambda in a nested comprehension's range
hoisted (MSVC miscompiled it); `//` comments never end in a backslash; private names mangled in classes
(`_Class__name`); `locals()`; a module name that is also a builtin falls back to the builtin until bound.

Runtime (native/pyrt): type.__new__ under a metaclass recursed (IntEnum); struct's alignment was read before
parse set it (`calcsize(parse_fmt(f, l, al), al)`); classes are never freed (instances do not own their
type: a class made and dropped left instances dangling -- pytest.mark was one); globals() is a live mapping
over the module's cells; `__slots__`; real ABCs (abc.ABCMeta register / hooks, collections.abc with mixins,
os.PathLike); eval of expressions (pyfront's tree evaluated); inspect.signature / getsource (by qualified
name in the source file: cache keys hash it); open(fd), os.utime on folders, os fd calls, errno names;
pow(b, -e, m); utf-32; sys.flags / implementation / version_info; memoryview; pickle.Pickler (fast).

### Libraries added

| library | how |
|---|---|
| argparse, difflib, random, datetime, csv, gzip, zipfile, pathlib, tomllib, shlex, pprint, queue, base64, hmac, secrets, calendar, filecmp, wave, glob, stat, posixpath, _pyio (io's classes), collections' User*/ChainMap (as _pycollections), ntpath's rest (as _pyntpath), reprlib, compression.* | **CPython's own Python, translated with the program** (stdlib.txt; read from CPython's Lib at translation): their behaviour is CPython's by construction |
| _random (Mersenne Twister), _csv, zlib (zlib 1.3.1 from native/decompiler + crc32), codecs, gettext, _operator, msvcrt, signal, weakref (strong), locale, faulthandler, runpy, _collections, sysconfig, importlib.resources | C++, lib_extra.cc |
| ctypes (+ util, wintypes) | C++, lib_ctypes.cc: simple types, POINTER, arrays, Structure / Union / BigEndian* from _fields_ (alignment, _pack_, bit fields), byref / cast / memmove / string_at, CDLL / WinDLL / windll, x64 calls (a caller per float pattern of the first four arguments), CFUNCTYPE callbacks (2,048 entry points; a DLL's own threads take the global lock) |
| unicorn, capstone | **their own ctypes bindings translated** (site.txt) over pyrt's ctypes: calls go straight to unicorn.dll / capstone.dll, loaded by path from the package folders (importlib.resources.files) |
| cryptography (hashes, HMAC, AES / 3DES / ARC4 ECB / CBC, RSA numbers), Crypto.Cipher.DES | C++ over bcrypt, lib_crypto.cc |
| pytest, unittest, jsonschema | C++, lib_pytest.cc / lib_extra.cc |
| z3 | **stubbed** (importing raises ImportError with the reason). Needed by emulate/symbolic.py and verify/concolic.py, which emulate/shadow.py and engine/win16.py import at module level -- so 6 suites. The installed z3-solver package is itself ctypes over libz3.dll, so it is one line in site.txt (`z3.*`) once Logan decides; nothing to download |
| asyncio | not added. The 3 `async def` of the product are ui/: the MCP server (mcp + uvicorn over asyncio), the terminal UI (textual + rich). A C++ front end needs: an MCP stdio JSON-RPC loop (small, hand-written), a terminal UI over the service's calls, and for `serve`/service: socket + socketserver + http.server (a winsock _socket, then CPython's Python for the rest) |
| PIL, mcp, textual / rich, http.*, socket, xml (pyexpat) | not added (counted below) |

### The suites as C++ (tests/run_all.py --quick, run by codebreaker.exe; Python: all 45 pass)

**31 of 45 pass as C++** (state/work/pyengine-dyn2/runall5.txt; /Od build, 1,054 s vs CPython's ~600 s):
tidy, assembler (176), dos, frozen, invert, m68k, z80, media, nes, pklite, exepack, converted, platforms,
port, powerpc, match_powerpc, ppc_calls, readable, recompile, store, symbols, vmx128, xbe, xbox_crypto,
xbox_effect, xbox_formats, xenos, xex, xex_edit, concolic, mcp (all 13 skipped: no mcp library).

| suite | as C++ | why not |
|---|---|---|
| check_imports | 546 / 603 | 8 modules do not import: 4 need z3 (emulate.shadow, emulate.symbolic, engine.win16, verify.concolic), 4 are the UI (ui.mcp_server, serve, service, terminal: http / socketserver / rich / mcp); the 49 "dangling" are imports of those 8 |
| java (pytest folder) | 400 / 416 directly (`-m pytest tests\java`: the same 416 Python collects); run_all's 8 shards report 28 / 32 | PIL x6, xml (pyexpat) x2, `-c` x1, 4 single bugs (test_finally, test_probe_libraries, test_kind's module named by a string, test_review_fixes DID NOT RAISE); the shards' low count: a shard ends early (to look at: likely the heap corruption below) |
| translate (pytest folder) | 143 / 148 directly (the same 148) | compile() of a source string x1, test_port_semantics x3 (a built program's file not found), test_clang_frontend x1 (`MethodDecl.implicit`) |
| run_record | crashes | **heap corruption** (0xC0000374) with no output: in the unicorn hook / ctypes path, not found yet |
| native_trace | 1 / 7 | z3 |
| native_engine, versions | 3 / 4, 8 / 9 | z3 |
| drivers | 13 / 16 | z3 x2, PIL x1 |
| whole_module | 12 / 15 | z3 x2, a dict subclass's attribute missing after a copy (`_Texts._made`) |
| analysis | 10 / 11 | test_conventions (an assertion; not looked at) |
| plumbing | 16 / 17 | `-c` source text |
| xna | 7 / 9 (6 skipped as in Python) | PIL |
| ui, service | do not start | http.server / socket (the UI front ends) |

Harness bugs found and fixed on the way (each made tests pass silently): a script outside the program
(native/nasm/test/masm/run.py) answered nothing and test_assembler counted 0 of 0 -- it is now in the
program (176 pass); pytest marks with arguments applied to a function did not decorate it, so skipif-marked
tests were never collected (translate collected 96 of 148, java 346 of 416) -- fixed, the counts now equal
Python's.

### Typed where proven (step 4): typed islands, `--dyn-typed` (2026-11-05)

    pyengine codebreaker --out T --cpp T/cpp                                     (the typed path, as before)
    pyengine --dynamic ... --dyn-typed T --dyn-islands native/pyengine/hints/islands.json

The typed path's C++ (l2cpp17 over py.hpp) is compiled in a library of its own with `py=pyt` (py.hpp and
pyrt both name their namespace py; they never share a translation unit), only the modules the islands reach.
src/islands.cc picks the islands (value parameters -- int within the range the proof assumed, bool, float,
str, bytes, None-or / tuples of those --, value results or fresh lists / dicts of values, no file writes /
environment / clock in the body or anything it calls, module names read only when typed constants); dyn.cc
writes each one's dynamic body as `<name>_dyn` and under its own name a wrapper that checks and unboxes the
arguments exactly, calls the typed body through the generated `_typed/islands.cpp`, boxes the result, and
runs the dynamic body for any other argument and whenever the typed body raises (so Python's exception and
message are the dynamic body's). The boundary is plain C++ (native/pyengine/islands). README "Typed islands".

| measure | result |
|---|---|
| translated by the typed path (strict, this tree) | 392 (HANDOFF's 472 was an older tree) |
| islands written (candidates) | **169**; 223 not islands: 116 methods, 46 container / record / None-or-int parameters or results, 38 effects (open 18, os.environ 11, shutil 6, os.makedirs 2, os.remove 1), 20 module names that are not typed constants, 2 translated as overloads, 1 int parameter with no recorded range |
| judged (PYRT_TRACE + `--judge-islands`, run_all --quick twice: islands on / PYISL_OFF=1, program built with PYENGINE_NO_DIRECT=1, traces per process via `%p`, only the islands via PYRT_TRACE_FUNCS) | **67 agree**, **1 differs**, 101 not run by the suites |
| dropped | `codebreaker.port.__main__.sound(path)`: the same path gave a different result in the two runs (3 of 3 argument lists). It reads an object file the test builds (`os.path.getsize(path) > 64` and its first bytes), and the two runs build into folders of their own: its answer is the file system's at the time, not a function of its argument, so the judge cannot hold it to one answer -- dropped as differing, as the rule says |
| kept (hints/islands.json, `--dyn-islands`) | **67** (the 101 not run are held back until a suite runs them) |
| suites, final program (direct calls, 67 islands, /Od) | the same as the program without islands, suite for suite and count for count (run_all --quick in this worktree; the baseline program built from master's emission): 35 ok, the same 10 failing for the reasons in the table above. test_native_trace crashes (0xC0000374 / 0xC0000005) in most runs of *both* programs -- the heap corruption HANDOFF already names, not the islands |
| typed calls answered | 86,222 over one --quick run (63 islands reached), 1 raised (it ran the dynamic body) |

**Speed** (bench: the islands called through their function objects as dynamic callers call them, /O2,
best of 3 on a loaded machine, ns per call, islands on / PYISL_OFF=1): `_match_brace(src, i)` (a loop over a
1.7k-character str) 73k / 517k (**7x**); `m6502_asm._split_args` (a loop over characters) 4.3k / 15.6k
(**3.6x**); `m68k._reg` (a %-format) 0.9k / 1.4k (1.5x); `m68k._s8` (one conditional on an int) 0.41k / 0.28k
and `rules._strip_block_comments` (one re.sub: both sides use pyre) 37k / 26k -- **slower**: boxing a str
in and out (UTF-8 to code points and back) costs more than a one-expression body saves. On the suites the
effect is inside the noise (the islands are a few thousand calls of small helpers in runs of minutes).
Next: a cost rule for islands -- a body that is one expression or one library call (re.sub) is left
dynamic; loops over str / bytes are where the typed body wins.

Gate 1 holds: `pyengine codebreaker --out` gives byte-identical coverage.json, coverage.md and 554 l2/*.json
with this branch's engine and master's (built from `git archive master`), and without --dyn-typed the
dynamic output of the whole tree (826 modules) is byte-identical to master's engine's.

### The cut-over plan (written, not executed; no Python deleted)

| caller | what it does | how it keeps working without the package |
|---|---|---|
| Dashboard tools/scripts/codebreaker_xenon.py (both trees) | `subprocess.run([sys.executable, "-m", "codebreaker", *args], cwd=CB)` (xenon-recompile ...) | point it at `CB\bin\codebreaker.exe` (one line), or keep the line and put a `codebreaker.exe` there: the program accepts `-m codebreaker ...` itself |
| Dashboard prepare_game.py, prepare_module*.py, prepare_unified.py, prepare_dash_versions.py, decompile_17559_modules.py | in-process: codebreaker.recompile.config / xenon, codebreaker.formats.xbox.gdf (`gdf._open`), codebreaker.formats.xex (`basic`) | (a, recommended) translate the scripts with CodeBreaker into their own program: `pyengine --dynamic --dyn-files <Dashboard>\tools\scripts --dyn-tree codebreaker --dyn-exe dashboard_tools`, then `dashboard_tools.exe prepare_game.py ...` -- the scripts unchanged, no Python; (b) commands for the four calls (`codebreaker.exe xex-basic`, `gdf-extract`, `recompile-config`, `xenon build/analyse`) and the scripts call them |
| Windows95/tools/place_map.py | codebreaker.base.corpus (executables), codebreaker.formats.detect (format_of) | the same two ways: translated with CodeBreaker (`--dyn-files Windows95\tools`), or `codebreaker.exe corpus --executables --json` and `codebreaker.exe detect --json <paths>` |
| Win95VM, XboxMCP | (no reference to codebreaker found) | nothing |
| .mcp.json (`python -m codebreaker mcp`), the terminal UI, `serve` | asyncio + mcp / textual / http.server | the front-end rewrite above; until then these stay Python clients |
| CodeBreaker's own `-c` children | build/module.py's parallel builds and test_plumbing's child pass source text to `sys.executable -c` | the two sites run a module instead (`-m codebreaker.build.child`): a two-line Python change, then C++ |

A minimal import surface for scripts that must stay Python (if any): none is needed if (a) is taken; the
alternative is a py::var <-> PyObject* bridge module, which the plan avoids.

### The honest distance to deleting the Python

1. **Correctness**: the suites that fail as C++ (above) -- z3 (a decision, 6 suites), the heap corruption in
   test_run_record, the UI front ends, PIL (images in 3 suites),
   xml.dom (pyexpat), `-c` (two sites), and a handful of single-test semantics bugs.
2. **The UI and MCP front ends** (a C++ rewrite, the only hand work left).
3. **Speed**: the /O2 build has not been timed; the /Od program is ~3x slower than CPython on short commands
   (identify lz32.dll: 1.9 s vs 0.56 s, start-up included).
4. **Build inputs**: the standard modules are translated from CPython's Lib at build time; to drop CPython
   the generated C++ of stdlib.txt/site.txt (or the .py files, PSF-licensed) is vendored once.
5. **The oracle**: CPython stays as the judge's reference (trace_py.py) until the C++ is the reference.

---

# pyengine -- handoff (2026-11-05): the dynamic mode

The new direction (Logan, 2026-11-05): the engine translates the rest of CodeBreaker's Python, not hand
ports. The typed path (below the line) refuses ~98% of lines; the **dynamic mode** writes C++ over
`py::var` (native/pyrt) wherever a type is not proven, and refuses only what needs an interpreter.

## What exists

| piece | where | state |
|---|---|---|
| census | `pyengine --census codebreaker tests` (src/census.cc) | the libraries and attributes, builtins, methods, features used; full output and the refusal-class table in state/work/pyengine-dyn/census.md (not in git: summary below) |
| runtime | native/pyrt (include/py_object.hpp, src/*.cc, ~9k lines) | `py::var`: None/bool/unbounded int (int64 inline, pyengine's `Big` past it)/float/str (UTF-8, code points counted)/bytes/bytearray/list/tuple/dict (CPython's ordered layout)/set+frozenset (CPython's table: CPython's iteration order)/range/slice; classes (C3 MRO, metaclasses, descriptors, properties, classmethod/staticmethod, `__init_subclass__`, `__set_name__`, `__getattr__`, `super`), exceptions (the hierarchy, CPython's messages, context/cause), generators on pooled fibers (send/throw/close, yield from), closures with cells, modules with cells and import; Python's operators, comparisons, hashing, repr/str, %-formatting, format specs, str.format, f-strings, argument binding with CPython's TypeErrors. Library modules: os, os.path, shutil, glob, fnmatch, tempfile, subprocess, time, stat, errno, platform, sys, io, re (over l2's pyre), json, struct, hashlib (bcrypt), collections, functools, itertools, math, string, textwrap, copy, bisect, heapq, operator, enum, dataclasses, contextlib, threading (a GIL), concurrent.futures (tasks run at submit), contextvars, importlib, types, typing, abc, uuid, traceback, warnings, atexit, binascii, zlib (crc32/adler32), keyword, sqlite3 (over native/engine's SQLite), pickle (CPython's protocol, both ways), ast (over pyfront), inspect (cleandoc/getdoc). 37 runtime checks (pyrt_tests) |
| emitter | `pyengine --dynamic` (src/dyn.cc, ~1.8k lines) | every statement and expression; shapes in README "The dynamic mode" |
| judge | `PYRT_TRACE` in pyrt + state/work/pyengine-dyn/trace_py.py + `pyengine --judge-traces` | per function, the calls (argument and result repr hashes, addresses masked, in order) of both runs compared |

## The prototype: codebreaker/port (chosen over exact: self-contained tests, no corpus, no unicorn)

    pyengine --dynamic --root . --dyn-out state/work/pyengine-dyn/port1 --dyn-main test_port \
             --dyn-closure --dyn-top --dyn-full codebreaker.port test_port=tests/test_port.py
    state/work/pyengine-dyn/dynbuild.cmd state/work/pyengine-dyn/port1 [fast]

| measure | result |
|---|---|
| translated | 29 modules (port's 9 = 10,411 lines, and everything they import at module level: base, jobs, translate.l2/ir, exact.emu.dos, verify.verdicts ...) = 13,939 lines, 544 functions; **0 constructs refused** (100% of lines) |
| compiles | **100%**: the 29 modules' 17k lines of port C++ (+ the rest) build with MSVC /O2 and /Od, no hand edits |
| tests | tests/test_port.py as C++: **5/5 pass**, the cache on (reading pickles CPython wrote) and off (CB_NO_CACHE=1); the same 5/5 as the Python. test_builds_and_runs runs the whole pipeline: pre_ansi's rewriting, sqlite3+pickle cache, gcc/clang builds of the ported program and its output |
| agrees (judge) | the test run's calls of every codebreaker.port function (first 200 calls of each, 4,820 compared; built with `PYENGINE_NO_DIRECT=1` so module-internal calls are traced too): **137 functions agree, 8 differ, 0 C++-only**; the 19 Python-only are module and class bodies. The 8 are all in port/__main__ and environment-bound: the object folder is named by `os.getpid()` (args, build, build.one, build.text, run), the process environment differs by the trace's own PYRT_TRACE variables (env_for), files checked in a set's order (Python's own order changes per run: str hashes are salted), and `memory_used` (ctypes is not in pyrt: None instead of the process's private bytes -- the one real gap) |
| speed | the 5 tests, cache off: C++ /O2 4.5 s, CPython 5.4 s (most of it is gcc/clang); the traced runs 99 s / 63 s (repr hashing) |

Bugs the judge found and fixed on the way (all general): Match's repr is `%.50R` (truncated to 50);
`=C:`-style keys are not in os.environ; interned attribute names were cached by a buffer's address
(a reused `std::string` buffer gave a wrong name: ast nodes' fields); the trace's set order and closure
counts. Also kept as checks: state/work/pyengine-dyn/samples/features.py (32 lines of output over the
constructs: identical to CPython's) and samples/seg.py (ast segments of arith16.py: identical).

Readability: each function keeps its Python name and signature, its docstring as a comment, its first
source line above it; locals are hoisted `py::var`s; see state/work/pyengine-dyn/port1/codebreaker/base/ctext.cpp
for a small module. Not yet typed anywhere (see next steps).

## All of codebreaker/, emitted (not yet compiled as a whole)

556 modules, 172,930 lines, 8,312 functions written in 2.8 s; **4 constructs refused**: three `async def`
(ui/) and one `match`. What will fail at run time is the library surface pyrt does not have yet: those
modules import and raise NotImplementedError when a function of theirs is called (argparse, difflib,
random, csv, zipfile, gzip, socket, ctypes, asyncio, datetime, hmac, secrets, multiprocessing,
faulthandler, msvcrt, locale, shlex, queue, signal, weakref, pprint, tomllib, unittest), and the
outside libraries (unicorn 434 refs, capstone 301, textual+rich 222, cryptography 62, z3 37, PIL 19,
pytest 140 in tests, mcp 15). Approach per library: census.md section 4 (C++ over each one's C API:
unicorn.dll, capstone, bcrypt for the crypto, libz3; ctypes callers call CodeBreaker's own DLLs' C ABI
directly; textual/mcp are front ends: a C++ terminal UI / a hand-written MCP over stdio).

## The census, in short

codebreaker/ is 172k lines (194k with tests/). Typed path today: 416 functions, 2.0% of function
lines. Refusal classes the dynamic mode absorbs (functions / lines): conflict 1,714 / 44,634;
dynamic 1,482 / 31,458 (getattr 138, next 85, call 84, lambdas as values 145, *args/**kwargs 155,
global 50, id 48, set order 44, computed regex 53); untyped 1,995 / 19,455; stdlib 533 / 10,648;
int-range 365 / 6,896; syntax 381 / 6,853; unresolved+extern 289 / 6,613; calls-refused 271 / 1,963.
Stdlib by files importing: os 370, re 203, sys 174, struct 170, json 90, tempfile 88, subprocess 82,
shutil 79, hashlib 60, time 38, collections 35, glob 27, ctypes 25, threading 24, argparse 22,
importlib 20, io 20, bisect 16, pathlib 15, unittest 13, pickle 12 ...

## Why the Python of port is not deleted yet

The dynamic mode builds whole programs. port's Python has an in-process caller outside the package
(tests/translate/test_module_model.py imports codebreaker.port.asm16), and the rest of CodeBreaker is
still Python. Deleting a package's Python needs either (a) its callers translated too (whole programs:
`codebreaker` itself as one C++ program with `-m codebreaker` as main), or (b) a bridge exposing a
translated package to CPython (py::var <-> PyObject* at the module boundary). (a) is the direction:
the package switches stay, and the deletion happens when the program its callers are in is C++.

## Next steps, in order

1. **Compile the whole tree** (`--dynamic` on all 556 modules, state/work/pyengine-dyn/all_cpp; fast
   build ~30-60 min with 2 jobs) and fix what fails to compile in the emitter (each error class is an
   emitter or runtime fix, never a per-module edit).
2. **Run the whole test suite as C++**: translate tests/* with their closures (one program per test
   file, or one program over all with a runner) and drive tests/run_all.py's list (do not touch
   run_all.py itself: another agent owns it). unittest and pytest's marks/fixtures/raises need pyrt
   harnesses (census: unittest 13 files, pytest 140 refs).
3. **The missing library surface**, by the census's counts: argparse (22 files), random (11; CPython's
   Mersenne Twister for the same sequences), difflib (8; CPython's SequenceMatcher), pathlib (15),
   datetime, csv, zipfile, gzip/zlib deflate (vendored zlib: ask), ctypes callers rewritten as direct
   calls, then unicorn/capstone over their C APIs.
4. **Typed islands**: where the typed path proves a function (its 2%), emit l2cpp17's typed C++ for it
   and box/unbox at the boundary (scalars and str first; containers only by copy where no aliasing).
   Also direct calls with defaults (now only all-positional calls of single-def module functions are
   direct), and generator expressions in other consumers without fibers.
5. `match`, async (asyncio.run of simple coroutines can run on the same fibers).
6. Then package by package behind switches, Python deleted per docs/NATIVE.md once every caller is C++.

**Projection, honest**: emission is done for all 172k lines (4 refusals). Compiling the port closure took
~12 emitter/runtime fixes, all general (evaluation order, proxies, macro names, precedence); expect a
few dozen more for the whole tree, not per-module work. Behaviour is bounded by the library surface:
the computational packages (formats, decompile, translate, java, build, source, verify) mostly use
what pyrt has; exact/emulate need unicorn, ui needs a rewrite, invert/verify need z3. The real cost now
is runtime breadth and the test-suite harness, not translation.

---

# The typed path (2026-11-04, unchanged)

Counts on all of codebreaker/ (strict): 472 translated (3,109 lines, 2.1%), 472 compile, 287 run beside
the Python, all agree. Gate 1: `--old` strict 274/274 L2 byte-identical, 7,756/7,756 refusal reasons the
same. The dynamic mode does not touch this path: `pyengine codebreaker --out` gives byte-identical
coverage.json, coverage.md and l2/* before and after this work. The check loop is state/work/pyengine2
(cycle.sh, judge.py, cachegate.sh); its next steps (batched judge compiles, one-package-to-100% with
hints) are superseded by the dynamic mode for coverage, and stay for the typed islands (step 4 above).
