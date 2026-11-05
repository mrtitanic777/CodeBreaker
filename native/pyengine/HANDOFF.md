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
