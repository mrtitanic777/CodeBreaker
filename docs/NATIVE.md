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

## Step 2 (in progress): the 16-bit tracer and the driver runner -- the same DLL

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

**The 16-bit tracer (not yet done).** emulate/windows16.py `trace_function16` is the larger half
(GDT/selector layout, far calls, the NE import-thunk stand-ins, int 21h as DOS3Call, ports16's
I/O, the branch log, and the recorded-system context replay).  The design is the same: a native
`cbt_w16_*` recorder holding the per-instruction code hook, the shared memory hook wrapped by the
segment-limit check, the interrupt hook, the ports and the branch log, with Python still asked the
rare events (the import_stub model, device/service naming) -- the context-replay and shadow paths
staying Python at first.  Switch **`CB_NATIVE_TRACE16`** (default 0).  Build is shared with the
32-bit tracer (native/tracer/CMakeLists.txt; `cbtrace.dll` now carries the driver recorder too).

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
