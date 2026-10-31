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
