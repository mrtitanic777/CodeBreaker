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

## The porting engine: Python into C++ by translation (2026-10-31)

Every step above can be written by hand. The engine is the other road: CodeBreaker's own
core idea -- reconstruct readable source and prove it behaves as the original -- turned on
CodeBreaker. `codebreaker/translate/frontends/python` lowers Python into L2 with types it infers
over the whole package, `backends/cpp17.py` writes C++17 over `native/ported/include/py.hpp`
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

## The rest, in measured-cost order

Each is measured again at its start (a profile of the slowest real job it serves) before the
order is trusted; the order below is what is known today.

| # | Step | Today (Python) | What makes it cost | Native form |
|---|---|---|---|---|
| 2 | **The 16-bit tracer and the driver runner** | emulate/windows16.py `trace_function16` (the Win16 lane's judge), emulate/driver.py `run` (VxD and other flat ring-0 code) | the same per-instruction and per-memory callbacks as step 1; USER's gate made 18 M register reads and 22 M memory reads for 81 functions (fast_uc.py) | the same DLL: a 16-bit recorder (segments, far calls, the NE import thunks, ports) and the flat driver runner, beside step 1's |
| 3 | **The path explorer** | emulate/shadow.py (Shadow32 / the 16-bit shadow), emulate/symbolic.py, verify/guided.py | a symbolic step per instruction, kept in Python beside the run | the shadow inside the recorder, so a shadowed run stops calling Python per step; then step 1's last Python path goes |
| 4 | **Formats and loaders** | formats/ (PE, NE, LE, MZ, OMF, XEX ...), formats/pe_functions.py carving | reading and carving whole images in Python, per process, every run of a tool | a loader library (one per format, the decompiler's loaders reused) giving the same facts |
| 5 | **The L2 passes and the C back end** | the L2 tree passes and the C writer in Python, after the native decompiler | tree walks over every function, every build | into native/decompiler: L2 built and rewritten there, C written there; Python reads the result |
| 6 | **The engine and the store** | engine/, store/ on Python's sqlite3 | per-fact Python overhead around SQLite | SQLite's C API directly, the engine's scheduler in C++ |
| 7 | **Build orchestration** | build/ (OpenWatcom, MASM, period compilers under DOSBox-X) | process starts and file handling around the compilers | a C++ driver: one process starts the compilers and gathers their output |
| 8 | **The rest** | everything else that computes | -- | case by case; the glue (CLI, terminal UI, MCP) stays Python |

Not moved: the period compilers, OpenWatcom, MASM and DOSBox-X (other people's programs, run as
they are), and Unicorn and Capstone themselves (already native).
