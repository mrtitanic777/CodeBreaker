# CodeBreaker in C++: the plan

Decided by Logan, 2026-10-30: CodeBreaker's own code is C++ wherever it can be. This reverses
ARCHITECTURE.md's "Python stays the language of the engine and the steps". Python keeps what
only glues -- the command line, the terminal UI, the MCP server, small scripts -- and anything
whose cost is not worth a rewrite; everything that computes moves.

## How each step is done

Every step follows the same four moves, so the tool keeps working the whole way:

1. **Behind a switch.** The C++ piece is added beside the Python it replaces, chosen by an
   environment variable (`CB_NATIVE_<STEP>`), off at first.
2. **Gated on identical output.** The same inputs through both, with fresh state and no caches;
   the step's output must be identical -- for the judge, every function's verdict, reason, limits
   and detail; for a pass, the same tree; for a loader, the same facts. A test traces/converts a
   small case both ways and compares the whole record, not just the verdict.
3. **Made the default** when identical everywhere tested (the switch set to 0 falls back).
4. **The Python deleted** at the next milestone after that (full regression: the Setup boots, the
   slow suites, a sweep), once nothing has needed the fallback.

The native pieces are plain C ABIs loaded with ctypes (no Python headers: not tied to one Python
version), or executables the Python starts, like the native decompiler. Each lives in
`native/<name>/` with a CMakeLists.txt (Visual Studio 2022+ toolset, Ninja), and its build
output's folder is a setting (`<name>_bin`), as `decompiler_bin` is.

## Step 1 (done): the 32-bit run recorder -- `native/tracer`

**Why, measured.** `reconstruct.run` on ping.exe (30 functions): decompile 4 s, build 1 s, judge
189 s. The judge made 2,459 emulator runs and Unicorn called back into Python 8.5 million times --
`hook_code` per instruction and `hook_block` per block in emulate/emulator.py `trace_function`
-- about 150 of the 183 s.

**What moved.** `cbtrace.dll` (native/tracer: api.cc, libs.cc, decode.cc, recorder.cc,
memory.cc, imports.cc, branches.cc; C interface cbtrace.h) holds trace_function's hooks:

- per instruction and per block (`fast`): the path, the instruction texts first seen first,
  calls and where they landed, the 00-padding stand-in, the step limit and the per-function
  governor; settling a fast run that stopped inside a straight run;
- memory: reads and writes (a write with what was there before), lean writes with each page as
  first written, pages mapped on demand and the two bails, the pages a kept machine must put back;
- interrupts (INT 20h VxD calls stepped over);
- the import stand-ins: arguments taken off by the signature's count, else by the pushes before
  the call (`_callee_pops`), what an API gives back through a pointer, the calls recorded;
- the branch log (each conditional jump of the function, and what the compare before it compared).

**What stayed in Python.** Laying the module out, the frame, starting each run (`uc.emu_start`),
putting the kept machine back, and turning the record into the TraceRecord dict. The DLL attaches
its hooks to Python's own Unicorn engine (`uc._uch`) in the order Python attached its own, so
Unicorn runs exactly the same. It calls the same unicorn.dll and capstone.dll Python's packages
loaded (found by path at start; nothing linked), so instruction text is byte-identical. During a
run Python is called the first time each import is entered: its argument count (`sig_resolver`)
and its out-parameters (`_outs`); both are lookups, so the answer is kept for the rest of the run
(ping.exe enters imports 1.3 million times in its judge -- asked each time, that alone was 23 of
the profiled 57 s). An exception raised in that call comes out of trace_function exactly as from
the Python hook. A run beside a shadow (the path explorer's symbolic re-run) stays on the Python
hooks for now (step 3).

**The text the record keeps** is made as the Python made it (`"%s %s" % (mnemonic, op_str)`
stripped), cached the same way (per code range as in the file for the life of the process; the
branch decode per module hash and address), so even a cache's effect on later runs is the same.

**The switch.** `CB_NATIVE_TRACE` (default 1; 0 = the Python hooks). Without the built DLL the
Python hooks run (native_trace.why_not() says why). Build:

    (vcvars64)  cmake -S native/tracer -B native/tracer/build -G Ninja -DCMAKE_BUILD_TYPE=Release
                cmake --build native/tracer/build

**The gate** (harness kept outside the repo, in state/work/native-trace2). Every function's (verdict,
reason, limits, detail) with the switch off and on, fresh CB_STATE, CB_NO_FP_CACHE=1, the same
decompiler binary, with two workers and again with one:

| module | judged | rows identical (2 workers) | rows identical (1 worker) | verdicts |
|---|---|---|---|---|
| ping.exe | 28 | 28 / 28 | 28 / 28 | equivalent 27, divergent 1 |
| lz32.dll | 20 | 20 / 20 | 20 / 20 | equivalent 17, divergent 3 |
| version.dll | 32 | 32 / 32 | 32 / 32 | equivalent 28, divergent 4 |

**Speed.** The judge's CPU seconds, one worker in-process (the machine was shared with other
sessions at 100% load, so wall seconds with workers below normal priority meant nothing):

| module | Python hooks | C++ hooks | |
|---|---|---|---|
| ping.exe | 112.0 s | 20.2 s | x5.5 (wall 134.8 s -> 21.5 s) |
| lz32.dll | 12.1 s | 11.7 s | -- |
| version.dll | 15.2 s | 15.5 s | -- |

lz32 and version spend their judge elsewhere: a profile of lz32 (native) puts it in Unicorn's own
emulation (emu_start's time inside the library, about 3 ms a run, not yet broken down -- the
next thing to look at in the 32-bit judge), the path explorer's shadowed runs (still the Python
hooks: step 3) and, in a fresh state, building the knowledge base's prototype table once.

The record-level check (cmp.py in the same folder; tests/test_native_trace.py does the same on a small module and on lz32): every
function of ping.exe, lz32.dll and version.dll traced in nine configurations (plain, stubbed,
unique stand-in returns with reads, the judge's fast+lean+reuse, with a step cap, with the branch
log, one instruction at a time on a kept machine, no resolver): 1,062 runs, every record
identical in every key and in the order of its instruction texts.

## Step 2 (done): the 16-bit tracer and the driver runner -- the same DLL

The same four moves as step 1, beside it in `native/tracer`.  The memory and branch machinery is
the model step 1 already moved; what 16-bit/flat add is the segment/far model, the thunks, the
ports, and the flat driver's service calls.

**The driver runner (done, hand-gated).** `driver_rec.cc` (C interface `cbt_drv_*` in cbtrace.h;
ctypes in codebreaker/emulate/native_driver.py) holds emulate/driver.py `run`'s hooks: the per-
instruction code hook (the driver's own code, told from a call out through a pointer, with the
cover set and the outside-call stand-in), the shared memory hooks (reads and writes, pages mapped
on demand -- fill off, as driver.run's), the INT 20h service calls (recorded with the registers
and stack words they were given, answered as the input says, the service's answer register set),
and the I/O ports.  Python still lays the regions out, sets the registers, starts the run
(uc_emu_start to _RET) and reads the result; it attaches the DLL's hooks to uc._uch in driver.run's
order.  During a run Python is asked one thing, once per distinct service: which register the
service answers in (driver_services + the knowledge base), kept for the run.  A run kept beside a
shadow stays on the Python hooks (step 3).

The switch is **`CB_NATIVE_TRACE_DRV`** (default 1; 0 = the Python hooks, as also without the DLL
or on any setup failure).  Record identity is checked both ways on a hand-built flat image in
tests/test_native_trace.py (`test_driver_same_record`): the cover, the service and outside calls,
the ports, the writes, the page mapped on demand, the exit and the result registers, all identical
with the switch off and on, across four configurations.

The module gate (every entry point's full trace record AND its isolated verdict, the switch off vs
on, `CB_NO_FP_CACHE=1`, in a fresh process each) on two VxDs:

| module | entry points | records identical | verdicts | judge CPU off | on |
|---|---|---|---|---|---|
| vkd.vxd | 23 | 23 / 23 | equivalent 23 | 38.6 s | 27.3 s (x1.41) |
| splitter.vxd | 6 | 6 / 6 | equivalent 6 | 15.1 s | 10.5 s (x1.43) |

Identical everywhere tested, so the switch is on by default.

**The 16-bit tracer (done, gated).** `w16_rec.cc` (C interface `cbt_w16_*`; ctypes in
codebreaker/emulate/native_win16.py) holds emulate/windows16.py `trace_function16`'s per-
instruction code hook (the plain path: the callee-intercept, instruction-cap, near-and-far-return
and import-sentinel decisions, the instruction count and the path) and its memory hooks (the
segment-limit check wrapping the shared reads/writes, and the pages mapped on demand, filled with
emulator._pattern as trace_function16's).  trace_function16 still lays the NE module out (GDT,
segments, the frame), enters the function, starts the run (uc_emu_start to SENT_BASE) and reads the
result; it attaches the DLL's hooks to uc._uch in its own order.

What stays Python, called from the native hooks (rare relative to instructions): the import stand-
in (`import_stub`, the NE oracle), a call into another of the module's functions (`answer`), and
interrupts (`_intr`: int 21h as DOS3Call).  The branch log and the ports' device model (ports16)
stay Python hooks, which trace_function16 attaches itself; the ports' clock reads the live
instruction count the native recorder keeps.  A run beside a shadow, a recorded-system context, or
with a prelude stays entirely on the Python hooks.

The switch is **`CB_NATIVE_TRACE16`** (default 1; 0 = the Python hooks).  Record identity is
checked both ways on lzexpand.dll's functions in tests/test_native_trace.py
(`test_win16_same_record`; skipped without the corpus).  The module gate (`CB_NO_FP_CACHE=1`, a
fresh process each) on a spread of each module's exported functions:

| module | funcs (record) | records identical | funcs (verdict) | verdicts |
|---|---|---|---|---|
| lzexpand.dll | 12 | 12 / 12 | 6 | equivalent 6 |
| keyboard.drv | 27 | 27 / 27 | 6 | equivalent 6 |
| comm.drv | 19 | 19 / 19 | 6 | equivalent 6 |
| krnl386.exe | 30 | 30 / 30 | 6 | consistent 4, equivalent 2 |
| user.exe (USER) | 40 | 40 / 40 | 8 | equivalent 8 |

Every function's full trace record and its (self-judge) verdict is identical with the switch off
and on, so the switch is on by default.  The judge's CPU over these samples is about flat off vs
on -- a self-judge of these smaller functions is dominated by the comparer and the concolic
machinery, not the per-instruction loop; the per-instruction win (the 18 M register and 22 M
memory reads USER's full gate made) shows on the instruction-heavy full gate, not a capped sample.
`cbtrace.dll` now carries the 32-bit tracer, the driver recorder and the 16-bit recorder.
## Step 6 (gated): the engine and the store -- `native/engine`

**What moved.** `cbengine.dll` (native/engine; C interface cbengine.h; ctypes in
codebreaker/engine/native_engine.py), on SQLite's C API directly:

- the program database (facts.py's Database): the fact, blob and about tables in the same schema,
  values kept inline or once in the blob table past 4 KB, the SHA-1 of tag and bytes, remake,
  forget, other versions dropped once per process, the blobs no fact holds collected, the cap on all
  databases (`hold`, a database holding verdicts emptied of facts, not deleted), the binary's SHA-256
  kept by path, size and time;
- the engine's core (engine.py's Run and Context): the step registry, which facts are current (each
  decided once per run from the bottom up, the step's version and every input's hash), the inputs
  recorded as a step asks (`need`, `need_many`, `peek`, `knowledge`, undeclared reads refused),
  early cutoff (the same hash: the row's provenance remade, nothing past it), unkept values, batch
  steps, and the scheduler: stale facts made side by side on a C++ worker pool (`low_load`'s count,
  the first item's error raised once every item has ended, one log line for the lot);
- the function store (store/functions.py): the index (module, profile, summary, store_about) and
  every module's tables on the engine's own connection to its database -- record_many, best, tally,
  scores, latest, stored, variants, the summaries, the overview, and the carry-over of a store from
  before the databases;
- the knowledge base's database (knowledge/store.py: pieces, kinds made whole under a stamp, the
  name store) and the digests reads.py gives the engine (a piece's hash, a whole kind's, the name
  store's answers), with the per-process hash table reread when another process wrote the file;
- the verdict vocabulary (verify/verdicts.py: verdict, klass, match, classify and the tables).

**What stays Python, called back.** A step's code (each step is still a Python function: the C++
run calls it for the contexts it makes, in a copy of the asker's context on the pool's threads, so
knowledge reads and the service's captured output behave as before); a step's version (a digest of
its source through `inspect`); a knowledge piece's hash when its kind must first be brought up to
date by Python's table builders (reads._fresh); what a value IS (facts.encode/decode: bytes, text,
JSON, a pickle). An exception a step raises comes out of the C++ run as itself, with its kind
(a LookupError is still one to the staleness check).

**SQLite.** The SQLite 3.54.1 amalgamation is vendored in native/engine/third_party/sqlite (from
sqlite.org, its SHA3-256 checked against the one published; public domain; downloaded with Logan's
go-ahead, 2026-10-31) and compiled into cbengine.dll statically: the build needs nothing outside
the project and no SQLite DLL is shipped beside it.

**The switch.** `CB_NATIVE_ENGINE` (DEFAULT in native_engine.py; 0 = the Python engine and store).
It is read when a database, a store or a knowledge base is opened, and the object stays on the side
it was opened on, so one process can hold both (tests/test_native_engine.py does). Without the
built DLL the Python runs (native_engine.why_not() says why). The switch is not part of any step's
stamp (engine/lanes.py `_SKIP`), so both sides make facts of the same versions. Build:

    (vcvars64)  cmake -S native/engine -B native/engine/build -G Ninja -DCMAKE_BUILD_TYPE=Release
                cmake --build native/engine/build
                ctest --test-dir native/engine/build

**The gate** (harness outside the repository, state/work/native-engine: gate.py, drive.py). `measure`
of eight modules from an empty state folder (the same folder path both times, so a path a fact
keeps is the same), the switch off and then on, CB_NO_FP_CACHE=1, CB_LOW_LOAD=2, the same
decompiler binary; then the warm re-run of every module; then one knowledge piece changed by hand
(win32 argument_bytes.code KERNEL32.#20, 4 -> 8, which lz32's judge reads) and lz32 measured again;
then the piece put back and lz32 measured again. Every row compared: each database's facts (kind,
key, hash, tag, size, step, version, inputs, the value or its blob), blobs and notes; each module's
store tables (functions, candidates, verdicts with reason, limits, detail and carried, texts,
identified, built signatures, variants); the index (modules, profiles, summaries); the knowledge
base (pieces, loads, bodies) -- all but the times:

| module | lane | facts | store rows | verdicts (off == on) | every row identical |
|---|---|---|---|---|---|
| lzexpand.dll | ne | 313 | 420 | equivalent 49, divergent 50, consistent 1, no-compile 2 | yes |
| keyboard.drv | ne | 217 | 290 | equivalent 40, divergent 21, consistent 4, no-compile 5 | yes |
| comm.drv | ne | 130 | 165 | equivalent 20, divergent 3, consistent 18 | yes |
| lz32.dll | pe | 124 | 176 | equivalent 17, divergent 3, no-compile 19 | yes |
| version.dll | pe | 139 | 189 | equivalent 28, divergent 4, no-compile 12 | yes |
| imm32.dll | pe | 226 | 293 | equivalent 73 | yes |
| splitter.vxd | le | 25 | 19 | equivalent 6 | yes |
| choice.com | dos | 73 | 67 | equivalent 22 | yes |
| the index, the knowledge base | | | 20, 28,031 | | yes |

30,974 rows in ten databases, every one the same. The warm re-run: on both sides each module made
exactly one fact, its lane's always-run profile, and the same eight facts were touched. The
knowledge change: on both sides the same 87 facts were made again, lz32's judge 3 verdicts judged
and 36 kept; putting the piece back did the same, and the verdicts came back as they were.
tests/test_knowledge_reads.py's cases pass both ways.

Found on the way, the same on both sides (not this step's to fix): the 16-bit lane's
`lane.ne.module` value holds Python sets (Made.wide, .near, .prototyped, .exported_prologue), whose
pickle follows the process's string-hash seed, so its hash differs from one process to the next
with an equal value (the first gate run, without PYTHONHASHSEED, showed exactly that and nothing
else; early cutoff stops it at the functions, whose hashes are the same). And splitter.vxd's
`lane.le.profile` records a read of `vxd:device:*` on a warm run that it does not record on the cold
run (the kind was already read in that process before the step recorded it).

**Speed** (the machine shared with three other sessions; seconds):

| | Python | C++ |
|---|---|---|
| cold, eight modules, each its own process (wall) | 438.0 | 257.3 |
| ... without lzexpand.dll (whose 314 s off was the machine) | 123.9 | 118.5 |
| warm, eight processes (wall, Python starting included) | 9.7 | 11.2 |
| the engine's warm check alone, in process (lanes.judged, best of 5, CPU) | 0.72-0.86 | 0.73-0.88 |

The cold time is the steps' (the decompiler, the compilers, the judge), the same code on both
sides. On a warm check of these modules (about 1,250 facts) the C++ core is level with the Python:
each check is mostly the always-run profile step and decoding the values asked for, still Python.
What the C++ engine opens is the steps' turn: with the store and the scheduler native, a step
moved to C++ no longer crosses into Python at all.

Identical everywhere tested, so the switch is on by default (`CB_NATIVE_ENGINE=0` falls back). The
Python Database, Run/Context, FunctionStore and KnowledgeBase are deleted at the next milestone.

**Tests.** `ctest --test-dir native/engine/build` (cbengine_test: the C interface alone, 43 checks:
digests and blobs, a run with steps written in C++ -- made once then kept, early cutoff, a changed
source remade, side by side on two threads, a batch with an unkept value, a callback's failure and
an unknown kind with their kinds --, the vocabulary, the store, the knowledge base's digests);
tests/test_native_engine.py (4: the same pipeline, store, knowledge base and every vocabulary word
through both sides in one process, each whole record compared); the fast suites pass with the
switch unset (on) and at 0: test_engine 13, test_knowledge_reads 7, test_verdicts 6, test_store 3,
test_service 11, test_ui 10, test_verification 20 (1 skipped), test_whole_module 15, test_frozen 22,
test_plumbing 17, tools/check_imports 557.

## The porting engine: Python into C++ by translation (2026-10-31)

Every step above can be written by hand. The engine is the other road: CodeBreaker's own
core idea -- reconstruct readable source and prove it behaves as the original -- turned on
CodeBreaker. `codebreaker/translate/frontends/python` lowers Python into L2 with types it infers
over the whole package, the C++17 back end (C++: `native/l2/src/cpp17_python.cc`, through the shim
`backends/cpp17.py`) writes C++17 over `native/ported/include/py.hpp`
(docs/PYTHON_FRONTEND.md says how, and what is refused). A function either becomes C++ that
reads as written by hand -- int64_t, structs, py::list, C++'s own operators where the intervals
prove them -- or it is left in Python with its reason; never a variant type.

    python tools/py2cpp.py port codebreaker.formats.ne native/ported/ne/src
    python tools/py2cpp.py coverage --compile        (and --checked)

**Proved on one module: `native/ported/ne`** (codebreaker/formats/ne.py, the NE reader). 14 of its
24 functions translate under the strict int policy; 19 with `--checked`. `gate.py` runs every file
beginning "MZ" through the Python reader and the C++ (`ne_facts`) and compares every fact: the
header, both name tables, module references and imported names, the entry table and export
ordinals, embedded modules -- and, in the checked build, segments, relocations and resources
too; an exception compares by its Python name.

| corpus | files | NE modules | identical (strict) | identical (checked) | Python | C++ |
|---|---|---|---|---|---|---|
| Windows 95 | 1,342 | 388 | 1,342 | 1,342 | 0.69 s | 0.41 s |
| Windows 3.1 | 292 | 255 | 292 | 292 | 0.37 s | 0.11 s |
| Windows 1.0 | 27 | 27 | 27 | 27 | 0.03 s | 0.01 s |
| Windows 98 | 2,378 | 570 | 2,378 | 2,378 | 1.22 s | 0.74 s |

Speed x1.7-4 with the reading of the files counted (warm cache; the files that are not NE are
read whole and rejected by both). ne.py was already mostly struct.unpack_from and slicing done
in C, so a reader like it gains little; the other 10 functions stay Python: segments, resources
and relocations shift a file-read sector by a file-read alignment, an int Python lets grow past
64 bits -- strict refuses it, checked writes `py::shl` that raises where Python would not; from_buffer
is a classmethod, open_boot_image uses next(), standalone a nested function. Not wired behind a
`CB_NATIVE_NE` switch: the facts program is the gate, not a loader the tool calls.

**Measured over all of codebreaker/** (`coverage --compile`, 532 modules, 6,957 functions,
133,105 lines; types settle in 18 rounds, 1.5-3.5 minutes):

| package | functions | lines | translated | % lines | compiles | % lines |
|---|---|---|---|---|---|---|
| formats | 1288 | 18099 | 121 | 5.0% | 113 | 4.6% |
| exact | 501 | 12909 | 14 | 0.4% | 14 | 0.4% |
| translate | 831 | 12513 | 9 | 0.2% | 9 | 0.2% |
| java | 572 | 10906 | 34 | 1.5% | 29 | 1.3% |
| source | 483 | 10506 | 8 | 0.4% | 8 | 0.4% |
| port | 270 | 9483 | 2 | 0.2% | 2 | 0.2% |
| build | 379 | 8573 | 25 | 1.3% | 24 | 1.2% |
| invert | 285 | 7930 | 12 | 1.1% | 12 | 1.1% |
| decompile | 333 | 7363 | 15 | 1.2% | 15 | 1.2% |
| verify | 321 | 6534 | 2 | 0.1% | 1 | 0.1% |
| emulate | 345 | 6144 | 9 | 0.7% | 9 | 0.7% |
| recompile | 171 | 5604 | 6 | 2.6% | 6 | 2.6% |
| analyze | 208 | 4419 | 2 | 0.1% | 2 | 0.1% |
| symbols | 182 | 3176 | 6 | 1.2% | 5 | 1.0% |
| ui | 204 | 2341 | 3 | 0.4% | 3 | 0.4% |
| engine | 128 | 1412 | 2 | 0.4% | 2 | 0.4% |
| base, store, coverage, knowledge, jobs, __main__ | 296 | 3106 | 0 | 0% | 0 | 0% |
| xna, compare, platforms | 160 | 2087 | 4 | - | 4 | - |
| **all** | **6957** | **133105** | **274** | **1.3%** | **258** | **1.2%** |

With `--checked` ints: 363 functions, 1.8% of lines (formats 7.9%). The first reason each
refused function was refused for, by class (functions / lines):

| class | functions | lines | what it is |
|---|---|---|---|
| untyped | 1,963 | 17,542 | 1,218 parameters no annotation or typed call reaches; 307 whose only callers pass values of no type; attributes and containers also given untypable values |
| stdlib | 1,345 | 30,680 | `re` 484 functions (match, fullmatch, sub, search, findall, escape, compile ...), `open` in text or write mode 168, os.makedirs / os.path.abspath / os.environ / os.walk, json, subprocess, hashlib |
| conflict | 1,158 | 29,214 | one place, two kinds: dicts holding mixed values, variable-length tuples, a value that may be None where it is used, iterating something only ever None |
| dynamic | 1,037 | 26,027 | getattr 137, generators 90, id 72, first-class lambdas 64, next 64, global 50, *args 49, set iteration order 42 |
| syntax | 554 | 17,240 | nested functions 226, `with` other than open 71, `dict(...)` forms 62, unpacking into attributes |
| int-range | 268 | 4,375 | ints not shown to fit int64 (the checked policy takes all but 10) |
| unresolved, calls-refused, extern | 358 | 6,291 | `__doc__`-style names, refused callees, unicorn/capstone |

About 690 of the refusals are cascades (an untyped parameter whose callers are refused, a
refused callee, a shared attribute or container a refused function writes): fixing a root
cause frees them too.

**How far this engine can carry the migration -- honestly: not far, as CodeBreaker is written
today.** It carries the typed core of format readers (formats: 5% strict, 8% checked, and the
proven NE reader) and small pure helpers everywhere, and what it carries was right wherever it
was run (the NE reader on 4,039 files, the semantics test). It does not carry the bulk,
and the reasons are the code's, not the engine's alone: almost nothing is annotated, so types
must flow from callers, and one untypable caller (a `re.match` result, a `json.load`, a
`getattr`) cuts the flow for everything below it; data is passed as dicts and tuples of mixed
values that have no one C++ type; and the tool leans on `re`, generators, closures and dynamic
lookups. What would move the number, in order of yield:

1. A Python-compatible regular expression engine behind `re` (CPython's sre ported, or PCRE2
   with a translation of Python's syntax): `re` alone is the first refusal of 484 functions and
   the start of many cascades.
2. Nested functions as C++ lambdas, generators consumed whole as lists (and otherwise as callback
   iterators), `open` in text and write mode, the os/os.path rows: 600+ functions refused for these
   directly.
3. Annotations at the hubs (by hand, a few hundred lines): a typed parameter breaks a cascade where
   whole-program inference cannot reach.
4. Context-sensitive inference (a helper typed per caller, or written as a C++ template) instead of
   one join of every caller, so one sloppy caller does not refuse a shared helper.

What stays hand work regardless: the dynamic parts (getattr dispatch, registries, the terminal
UI and other glue -- which this plan keeps in Python anyway), dicts used as objects with values of
several kinds (they want hand-written structs or variants chosen by a person), and anything over
Unicorn or Capstone's Python bindings. The engine is worth running on every module before it is
moved by hand: what it translates is gated and done, and its refusal list is the hand-port's to-do.

### The engine in C++, piece 1: the Python parser -- `native/pyfront`

All of CodeBreaker is to be C++ (2026-10-31), so the engine above is being rewritten in C++,
and that rewrite can then port the rest of CodeBreaker. Its first piece is a complete
Python 3 parser: a hand-written tokenizer (INDENT/DEDENT, implicit joining, PEP 701 f-string
modes, every string prefix and escape, numbers with underscores, hex, octal, binary,
unbounded ints and complex) and a recursive-descent parser over CPython's
`Grammar/python.gram`. The parser covers all statements and expressions, f-strings and
t-strings, decorators, comprehensions, lambdas, async/await, match, walrus, star-args,
annotations, PEP 695 type parameters and `type` aliases. Its output is an AST with
CPython's shape: one plain C++ struct per `ast` node class, the same fields, the same
positions.

**Gate: identical trees.** For every `.py` under codebreaker/, tools/ and tests/, the dump
equals CPython 3.14's `ast.dump(ast.parse(src), include_attributes=True)` byte for byte.

| corpus | files | identical | pyfront parse | CPython ast.parse |
|---|---|---|---|---|
| codebreaker/ + tools/ + tests/ (10.7 MB, 1.25 M nodes) | 727 | **727** | 0.65-1.2 s | 2.5-5.1 s |
| CPython's own Lib/ (extra check) | 2,436 | 2,435 | -- | -- |

Both parsers are timed best of 3 under the same load: pyfront is about 4x faster. The one
file of CPython's Lib that differs uses characters new in Unicode 16 that Windows' ICU does
not know yet. Syntax errors are refused with CPython's line and offset: the 33 error cases
in the tests all match, as do Lib/test's `badsyntax_*` files, message text included. CTest
`pyfront_grammar` runs 301 grammar cases against dumps CPython made once. The gate script is
`state/work/pyfront/gate.py`, and Python is used only as its oracle.

**The API the next pieces consume** (native/pyfront/README.md has the full version):

- `pyfront::parse(bytes, filename)` returns a `pyfront::Module` (or a `SyntaxError`), and
  `pyfront::dump(module)` returns the canonical text.
- `Expr`, `Stmt`, `Pattern` and `TypeParam` each carry a `kind`. `as<T>()` and `cast<T>()`
  downcast to the per-class struct, such as `BinOp{left, op, right}` or
  `FunctionDef{name, args, body, decorator_list, returns, type_params}`.
- Operators and contexts are enums. Constants are a `ConstValue`: an Int keeps its decimal
  digits plus an `i64`, and a Str is UTF-8.
- Every node lives in the Module's arena and children are raw pointers, so the inference
  stage can key side tables on node addresses without copying the tree.
- The C ABI is in `pyfront/pyfront.h` (`pyfront.dll`).

**Left for the inference + lowering stage** (piece 2, on the native L2 core in native/l2):
1. A scope pass: names to their bindings (module, class, function, comprehension), with
   `global`/`nonlocal` and the attributes each class and its instances gain.
2. Whole-package type inference as `codebreaker/translate/frontends/python` does it today:
   types flow from annotations and from callers, ints are given ranges, and anything left
   untyped gets a reason. These are the same rules, rewritten over `pyfront::Module`.
3. Lowering to L2: from typed pyfront nodes to the L2 core's tree, with refusal reasons kept.
   That replaces the Python front end's lowering.
4. The C++17 back end (`backends/cpp17.py` and `py.hpp`), rewritten over L2 in C++.
pyfront deliberately does not yet provide a generic child walker or a parent map. The scope
pass is the first thing that needs them, so they should be built there.

## Step 5 (now first): the L2 core and the C back end -- `native/l2`

Logan, 2026-10-31: "All of it needs to be C++." The translator's core moves first: the L2 tree, and the
C back end on it, so decompile -> C is native end to end (the decompiler already builds L2 in C++,
PrintL2). `native/l2` (CMake + Ninja, VS 18 toolset) builds two static libraries and one DLL:

- **`l2core`** -- the L2 core (`src/value.hpp`, `src/json.hpp`, `src/l2.hpp`): what the translator back
  ends (C++03, C++17, Java, C#) and the porting engine's lowering sit on;
- **`l2c`** -- the C back end on it (codebreaker/translate/backends/c.py);
- **`cbl2.dll`** -- the C interface (`include/l2api.h`: `l2_call(fn, json_args) -> json_result`), loaded by
  codebreaker/translate/native_l2.py through ctypes.

    (vcvars64)  cmake -S native/l2 -B native/l2/build -G Ninja -DCMAKE_BUILD_TYPE=Release
                cmake --build native/l2/build && ctest --test-dir native/l2/build

### The core's C++ API (for the back ends and the porting engine)

`py::V` (value.hpp) is a value with Python's semantics, the material an L2 document and the C back end's
facts are made of: None, bool, int (to 2**64-1), float, str (UTF-8), list, tuple, dict (any key, insertion
order kept, a key set twice keeps its first place), set. Containers are shared -- copying a V copies a
reference -- so a dict is one object wherever it is held, and `v.id()` is Python's id(). Access as the
Python reads: `d.get("k")` (None when absent), `d.at("k")` (throws `py::PyError` "KeyError"), `d.has`,
`d.set`, `d.setdefault`, `v.seq()`, `py::truthy`, `py::eq` (Python's ==: True == 1), `py::lt` (sorted's
order), `py::repr`. Errors are `py::PyError{type, message}` with Python's exception name.

`py::json_parse(text)` / `py::json_dump(v)` (json.hpp) read and write JSON exactly as Python's json.loads
/ json.dumps with its defaults (", " and ": ", ensure_ascii, a float as its repr, NaN/Infinity, a lone
surrogate kept); `json_tagged` is the shim's form (tuples, sets and dicts of other keys tagged).

`l2.hpp` is ir.py + l2.py:

    l2::NodeP n = l2::from_json(doc);       // as l2.from_json: same slots kept, same KeyError/TypeError
    py::V d = l2::to_json(*n);              // as l2.to_json: keys in ir.py's slot order, empty lists left
                                            // out where ir.py leaves them out
    py::V same = l2::roundtrip(doc);        // to_json(from_json(doc))
    l2::Schema s(py::json_parse(schema_text));  s.valid(doc, &why);    // as l2.validate (draft 2020-12)

A `l2::Node` has its class (`n->kind()`: the ir class name; `l2::node_class(name)` gives every slot in
ir.py's order and the child fields walk() follows -- every node of ir.py, the C#, clang and Python kits'
optional slots included: Unit.lang/target, ClassDecl.tparams/layout, TypeParam, Property ...), its place
(`line`, `col`, `origin`) and its slots by name (`n->slot("cond")`, `n->find("x")`). A slot is an
`l2::Item`: another Node, a list of Nodes, a `l2::Type`, a list of Types, a `l2::Sym`, or a raw value (a
string, a number, a dict such as origin or Unit.target), decided as l2.from_json decides it. `l2::Type`
holds each kind's fields (prim/class/typevar/wildcard/array/ptr/selector/fn/fnptr/record, the
binary-facing ones of L2_BINARY_TYPES.md), `l2::Sym` a symbol's (kind, name, owner, static, type, params,
ret, varargs, refs, flags). From Python: `native_l2.roundtrip_text(doc)` (= json.dumps(l2.to_json(
l2.from_json(doc)))) and `native_l2.validate(doc)` -> (valid, why).

**The core's gate** (state/work/native-l2/l2gate.py): every L2 document recorded from the decompiler on
the gate modules (the 16-bit set, the 32-bit set, the PowerPC module) and every one the translator kits'
tests made or read (`pytest tests/translate`, recorded at l2.from_json / to_json / validate):

| documents | round trip byte-identical | schema verdict the same |
|---|---|---|
| 6,601 decompiler (unique) | 6,601 / 6,601 | 330 / 330 (every 20th; all 46 of lzexpand earlier) |
| 43 translator kits | 43 / 43 | 43 / 43 |

Validation: jsonschema took 155.6 s for the 332 sampled documents, the native check 5.4 s (x29). The
round trip is about even through the shim (its JSON both ways dominates); in C++, between C++ callers,
there is no JSON at all. tests/translate/test_native_l2_core.py holds both cores to each other on
documents of every kind; native/l2's CTest (`l2_tests`) holds the core to Python's json, str and repr.
## Step 4 (readers moved, gated, on): formats and loaders -- `native/formats`

Logan, 2026-10-31: "All of it needs to be C++." The file-format readers of codebreaker/formats/
are moving into one library, `formats.dll` (native/formats; setting `formats_bin`, default
native/formats/build/bin):

    (vcvars64)  cmake -S native/formats -B native/formats/build -G Ninja -DCMAKE_BUILD_TYPE=Release
                cmake --build native/formats/build
                ctest --test-dir native/formats/build          (the library alone, hand-made files)

**The design.** Each reader is plain C++ over the file's bytes -- `pe/pe.h` (pe::Image),
`ne/ne.h` (ne::Module), `le/le.h` (le::Image, le::W3, the writer in le/image.cc), `dos/dos.h`,
`omf/omf.h`, and for XEX native/xex's own parser compiled in (reader.cmake) -- with structs a C++
caller can use as they are. Every read goes through `core/bytes.h`, which keeps Python's rules
(a slice comes back short, `data[i]` raises IndexError, unpack_from raises struct.error with
Python's own message, a negative offset counts from the end), so a damaged file fails here
exactly where and how it failed in Python. Beside each reader a `questions.cc` writes its
answers in the Python reader's shapes (the same dict keys in the same order, tuples where it had
tuples). The C interface (`cbformats.h`) is one function, `cbf_call(question, bytes, args)`:
nothing is kept between calls, so there is no handle, and calls may run on any thread.
Arguments and answers are a pickle subset (`core/pyvalue.h`), which Python reads and writes at C
speed with no Python headers in the DLL. A case the C++ leaves to Python (an int past 64 bits,
a program whose unpacker runs in the emulator, an a.out object) is a Fallback: the Python reader
answers that call.

**The shim** (temporary, until the Python readers are deleted): codebreaker/formats/native.py
(loading, `call`, the switch, `asker` for methods, `native_first` for module functions -- each
keeps the Python function as `.python` and asks the C++ through `.cpp`) and one small module per
reader (native_pe, native_ne, native_le, native_dos, native_omf) holding the classes. The
readers' classes pick the C++ one in `__new__` (NEFile, BootImageModule, LEFile, W3File; PE
through open_pe), so callers change nothing. **Switch:** `CB_NATIVE_FORMATS` (all) or
`CB_NATIVE_FORMATS_<READER>` (PE, NE, LE, DOS, OMF, XEX); default on for the six gated readers
(native.READERS_ON); `0` falls back to Python; without the DLL Python runs (native.why_not()).

**The gate** (native/formats/gate.py): every file of every corpus through the reader with its
switch off and on; every attribute of every object and every method's answer (with probes:
RVAs, offsets, each segment's relocations, each object's bytes, every member of a container,
write_image of read_image, embedded and boot-image modules, both run_movers settings ...),
compared with their types and dict order, or the exception's name; the C++ side is checked to
have been asked. Seconds are the readers' own time (the comparison's work not counted).

| reader | corpus | files | accepted | identical | Python s | C++ s |
|---|---|---|---|---|---|---|
| PE (pe.py) | Win 1.0/3.1/95/98 | 10,042 | 2,050 | 10,042 | 7.95 | 8.87 |
| NE (ne.py) | Win 1.0/3.1/95/98 | 10,042 | 1,240 | 10,042 | 35.32 | 12.60 |
| LE (le.py) | Win 1.0/3.1/95/98 | 10,042 | 542 | 10,042 | 16.44 | 12.50 |
| MZ/DOS (dos.py) | Win 1.0/3.1/95/98 | 10,042 | 316 | 10,042 | 57.28 | 53.54 |
| OMF (omf.py) | the period toolkits ("kits") | 810 | 102 | 810 | 2.31 | 2.57 |
| XEX (xex.py) | the Xbox 360 library's .xex | 90 | 87 | 90 | 565.83 | 8.86 |

(Seconds from each reader's own gate run. A final run of PE, NE, LE and DOS together with the
finished code, on a machine shared with three other sessions, was again identical on every file:
PE 7.76 / 8.17 s, NE 66.49 / 26.09 s, LE 23.03 / 18.90 s, DOS 81.47 / 88.69 s.)

The speed column is honest about one thing: a ctypes call costs a few microseconds, so the
gate's many one-line probes (`_u`, `file_offset`, `directory`, ~150 per PE file) cost more in C++
than in Python, while the tables that walk a file are 3-6x faster (exports, resource_tree,
export_table on KERNEL32; NE overall x2.8). DOS time is dominated by the packed programs both
sides unpack in the emulator; XEX's by xex.exe's listing, which the C++ no longer runs.

Also with the readers on: test_frozen 22/22, test_plumbing 17/17, test_dos 20/20,
test_drivers 16/16, test_xex 6/6, test_whole_module 15/15, test_exepack, test_pklite,
tests/test_native_formats.py 14/14 (each reader both ways on corpus samples and a damaged file),
check_imports 562/562, and `tools/l2c.py --made lzexpand.dll keyboard.drv comm.drv lz32.dll`
223/223 the same as the reference.

**Left in Python, and why.** In the moved readers: omf.patched (its spec is a regular
expression), omf.rename_symbols (applies a Python function to every name), omf._idx (a two-line
helper; per-call ctypes would only slow its Python callers), dos.kernel_parts/exepack_stub
(assembly over build/exepack, reading nothing), xex.basefile/basic (they run the xex program),
the corpus walks (le.corpus_files, dos.programs). Not yet moved: pe_functions.py (PeCarve keeps
Capstone's Python instruction objects, which its callers read; function_spans and
disasm_whole32 can move once formats.dll loads Capstone as cbtrace.dll does), inspect.py,
detect.py, resources.py, fonts.py, mscompress/cab/lzx, media/gfx/xnb, powerpc_image.py,
xex_imports/xex_edit, the console ROM readers (gb, genesis, sms, nes, msx, zx) and formats/xbox/
(the container readers and the crypto tools). Each follows the same pattern.

### The C++17 back end -- `l2cpp17` (2026-10-31)

One back end per target language, in C++: the translator's C++17 back end is `native/l2/src/cpp17_*.cc`
(the static library `l2cpp17`, in cbl2.dll), and it replaced both Python C++17 back ends
(backends/cpp17.py's emitter and backends/cpp17_java.py, deleted). One emitter (`cpp17::Emitter`: the
block, the if/else-if chain, while, the range-for and its structured bindings, `auto`, lambdas and
immediately invoked ones, `std::make_shared`, `std::optional`), the source language's rules chosen by
`Unit.lang`:

- `cpp17_python.cc` -- 'python' (the Python front end; each unit now says `lang='python'`): a module's
  header and source over `native/ported/include/py.hpp`, Python's own operations from py.hpp or the API
  map (codebreaker/java/api_maps, read by the C++ as apimap.py resolves it, dialect c++17);
- `cpp17_java.cc` over `cpp17_jmodel.cc` (jmodel.py's object model in C++) -- 'java' or none: a whole
  program over `codebreaker/translate/runtime/cpp17/jrt.hpp`;
- 'csharp', 'c', 'cpp' decline until their rules are written.

    cpp17::write_unit(unit_doc, api_maps)   -> (header, source, spans)
    cpp17::program(unit_docs, main)         -> the program's text

The Python callers (tools/py2cpp.py, the Python front end, javajudge, the tests) go through the shim
`backends/cpp17.py` (`write_unit`, `program`: l2.to_json in, text out; a `Decline` comes back as
cpp17.Decline). There is no Python fallback and so no switch: without cbl2.dll the shim says why. L2
JSON now carries what the Python front end says of a class type (`record`, `ref`, `exception`;
l2.schema.json, l2.py and l2.cc), so its units cross whole. desugar.py still runs before `program` (a
pass, Python, shared with c_java and cpp03). Inside the Java transpiler (`convert(..., cpp_std='C++17')`)
the bodies are the C++03 back end's writer with the C++17 forms, now `cpp03.Bodies17` (the transpiler's
declarations are C++03's; those bodies move to C++ with cpp03).

**The gate -- identical generated text** (state/work/native-cpp17/gate.py against the capture of both
Python back ends, state/work/cpp-port-gate/translator-before, 3,566 files; inputs as captured: the
Python front end over codebreaker/ at 514599a20):

| group | what | files | identical | changed, judged equal | regressed |
|---|---|---|---|---|---|
| py | test_python_frontend's snippets, strict and checked | 4 | 4 | 0 | 0 |
| ne-strict | codebreaker.formats.ne (+ spans) | 3 | 3 | 0 | 0 |
| cov-strict | every module of codebreaker/ (+ spans) | 1,113 | 1,113 | 0 | 0 |
| cov-checked | the same, checked ints (+ spans) | 1,109 | 1,109 | 0 | 0 |
| java | java_modern, MathHelper, MCHashTable, Modern: C++17 and (unchanged) C | 16 | 16 | 0 | 0 |
| tree-Tally | the transpiler at C++17 | 208 | 208 | 0 | 0 |
| tree-alpha | Minecraft Alpha through the transpiler at C++17 | 1,113 | 1,113 | 0 | 0 |

Writing all 556 modules of codebreaker/ takes 1.9 s through the shim. CTest `cpp17_tests` holds the
rules' choice, a Python module and a Java program to the gate's text; tests/translate (the MSVC
semantics test, the java_modern programs beside the JVM) judge it compiled and run.

## The rest, in measured-cost order

Each is measured again at its start (a profile of the slowest real job it serves) before the
order is trusted; the order below is what is known today.

| # | Step | Today (Python) | What makes it cost | Native form |
|---|---|---|---|---|
| 2 | **The 16-bit tracer and the driver runner** | emulate/windows16.py `trace_function16` (the Win16 lane's judge), emulate/driver.py `run` (VxD and other flat ring-0 code) | the same per-instruction and per-memory callbacks as step 1; USER's gate made 18 M register reads and 22 M memory reads for 81 functions (fast_uc.py) | the same DLL: a 16-bit recorder (segments, far calls, the NE import thunks, ports) and the flat driver runner, beside step 1's |
| 3 | **The path explorer** | emulate/shadow.py (Shadow32 / the 16-bit shadow), emulate/symbolic.py, verify/guided.py | a symbolic step per instruction, kept in Python beside the run | the shadow inside the recorder, so a shadowed run stops calling Python per step; then step 1's last Python path goes |
| 4 | **Formats and loaders** | formats/ (PE, NE, LE, MZ, OMF, XEX ...), formats/pe_functions.py carving | reading and carving whole images in Python, per process, every run of a tool | a loader library (one per format, the decompiler's loaders reused) giving the same facts |
| 5 | **The L2 passes and the C back end** | the L2 tree passes and the C writer in Python, after the native decompiler | tree walks over every function, every build | into native/decompiler: L2 built and rewritten there, C written there; Python reads the result |
| 6 | **The engine and the store** (gated: "Step 6" above) | engine/, store/ on Python's sqlite3 | per-fact Python overhead around SQLite | native/engine: SQLite's C API directly, the engine's scheduler in C++; left: the steps themselves |
| 7 | **Build orchestration** | build/ (OpenWatcom, MASM, period compilers under DOSBox-X) | process starts and file handling around the compilers | a C++ driver: one process starts the compilers and gathers their output |
| 8 | **The rest** | everything else that computes | -- | case by case; the glue (CLI, terminal UI, MCP) stays Python |

Not moved: the period compilers, OpenWatcom, MASM and DOSBox-X (other people's programs, run as
they are), and Unicorn and Capstone themselves (already native).
