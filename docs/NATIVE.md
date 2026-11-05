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

## Step 3 (done): the path explorer's shadow -- the same DLL; the tracers' Python hooks deleted

**What moved.** `shadow.cc` (C interface `cbt_sh_*` in cbtrace.h; ctypes in
codebreaker/emulate/native_shadow.py) is emulate/shadow.py's x86 shadow (Shadow16, Shadow32,
ShadowLE) and the per-step half of emulate/symbolic.py (its expression constructors, with their
folds): before each instruction the decompiler's p-code of it is evaluated concretely and
symbolically at once, the register and memory bytes made from an input kept, and a conditional
jump over them is a condition of the path. Every step is shadow.py's, in the same order, making
the same expressions; Python reads them back as symbolic.py's tuples (path, trail, vars, lost) after
the run. The recorders step it themselves -- recorder.cc / imports.cc (32-bit: each instruction,
the zero-landing and import stand-ins' `after_call`, an out-parameter's `written`), driver_rec.cc
(the driver's own code and the landing pads; a service's `after_call` with its carry; the answering
register's `forget`), w16_rec.cc (each ordinary instruction, its CS base read from the GDT) -- so a
shadowed run calls no Python per instruction. Python is asked one thing: the p-code of an
instruction not lifted before in the process (pcode.Lifter.line, pcodecheck's line; parsed and kept
in C++ by mode, bytes and offset). Values are at most 128 bits (an XMM register): an instruction
with a wider one ends the shadow ("shadow: wide") where Python's integers went on -- none met.

What stayed Python: symbolic.z3_of and inputs (the solver's side, once per path, not per step),
verify/concolic.py, and verify/guided.py (it has no per-step part: it runs whole inputs). The
PowerPC shadow (ShadowPPC) stays Python: the PowerPC interpreter that steps it is Python itself.
The `inputs` test a shadow is made with is now a list of (low, high) ranges (the three lanes and
test_concolic changed to say so); a 16-bit shadow's selector bases are the GDT's
(windows16.selector_base) or real mode's (shadow.real_mode).

**The 16-bit recorder's last Python-only runs.** A recorded-system context (the module's own code
reached through a recorded far pointer, OWN_BASE: `on_own`, a callback once per such landing) and a
prelude (`cbt_w16_reset` after the prelude's calls: the path, the count, the reads and writes start
again, the other counters carry on as trace_function16's did) now run on the C++ hooks too.

**The switch** was `CB_NATIVE_SHADOW` (off, gated, then on); with the Python hooks deleted it is
gone, as are `CB_NATIVE_TRACE`, `CB_NATIVE_TRACE16` and `CB_NATIVE_TRACE_DRV`: without cbtrace.dll a
run raises (native_trace / native_win16 / native_driver / native_shadow `require()`, saying why).

**The gate** (state/work/shadow-native: gate.py, runall.py; a fresh CB_STATE each, CB_NO_FP_CACHE=1,
one worker in-process, the switch off vs on): every verdict row (verdict, reason, limits, detail),
every exploration's kept inputs and summary, and every shadowed run's (trail, path, vars, lost, cut)
through measure's own reconstruct.run:

| module | lane | verdict rows identical | explorations | shadowed runs identical | verdicts |
|---|---|---|---|---|---|
| ping.exe | 32-bit | 30 / 30 | 27 | 74 / 74 | equivalent 27, divergent 1, no-compile 2 |
| lz32.dll | 32-bit | 39 / 39 | 18 | 61 / 61 | equivalent 17, divergent 3, no-compile 19 |
| version.dll | 32-bit | 44 / 44 | 28 | 45 / 45 | equivalent 28, divergent 4, no-compile 12 |
| tapi32.dll | 32-bit | 186 / 186 | 38 | 59 / 59 | equivalent 37, divergent 24, no-compile 125 |
| imm32.dll | 32-bit | 73 / 73 | 73 | 73 / 73 | equivalent 73 |
| lzexpand.dll | Win16 | 46 / 46 | 33 | 450 / 450 | equivalent 30, divergent 14, no-compile 2 |
| comm.drv | Win16 | 41 / 41 | 38 | 323 / 323 | equivalent 20, consistent 18, divergent 3 |
| ver.dll | Win16 | 31 / 31 | 21 | 254 / 254 | equivalent 17, divergent 12, no-compile 2 |
| vkd.vxd | driver | 23 / 23 | 23 | 564 / 564 | equivalent 23 |
| splitter.vxd | driver | 6 / 6 | 6 | 155 / 155 | equivalent 6 |
| vpd.vxd | driver | 2 / 2 | 2 | 89 / 89 | equivalent 2 |

Then tests/test_native_trace.py (129 cases, shadowed 32-bit, 16-bit and driver runs and prelude
runs among them: every record identical both ways -- their agreed digests are now
tests/data/native_trace_golden.txt, which the C++ is held to), test_concolic, test_run_record and
test_verification all pass.

**Speed** (CPU seconds of the shadowed runs alone, the same runs; the judge's total in brackets):

| module | Python shadow + hooks | C++ | |
|---|---|---|---|
| vkd.vxd | 17.2 s | 4.4 s | x3.9 (judge 310 -> 288 s) |
| splitter.vxd | 6.6 s | 0.8 s | x7.8 (judge 9.3 -> 3.5 s) |
| ping.exe | 2.3 s | 0.5 s | x4.2 (judge 29.4 -> 27.3 s) |
| lzexpand.dll | 16.6 s | 10.0 s | x1.7 (judge 47.3 -> 39.7 s) |
| vpd.vxd | 0.45 s | 0.30 s | -- |

The 16-bit runs keep Python per call (the NE import oracle, the callees' stand-in) and the branch
log and ports hooks, so lzexpand gains less. The judge's totals are now spent in the comparer, the
solver and the build, not the shadow.

**Deleted** (with the Python hooks): emulator.trace_function's hook_code, hook_block, settle,
hook_write, hook_intr and their state, _memory_hooks, _branch_hook, _decode_branchy, _address,
_operand_value, _straight_run, _callee_pops, _pattern and five constants only they used;
trace_function16's hook_code, hook_limited and hook_intr; driver.run's on_code, on_interrupt, on_in,
on_out and on_memory; shadow.py's UnicornX86 and its Python x86 shadows; the four switches and
their off branches (WT_DBG's Python-hook path with them). About 1,000 lines. Nothing in the other
repositories under GitHub/ referred to any of it (Dashboard, Windows95, Win95VM and the rest searched
for every deleted name and for any use of codebreaker.emulate).

After the deletions: run_all --quick passes (all suites; again with l2, engine, tracer, formats and
dosexec all built in the worktree), tools/check_imports 559/559, tools/tidy
clean.

**The GDI link fault of 2026-11-04 (fixed 2026-11-05).** In a fresh state folder `gdi.exe` did
not link (`E2052: asm/frame38_00.obj: relocation at 0026:0def not in the same segment`). The
reproduction builds the corpus's `gdi.exe` -- Windows' full GDI (WINDOWS/SYSTEM, 45 segments), not
MINI.CAB's 36-segment GDI.EXE that the all-15 boot uses (state/mini/original), which has no 32-bit
segment and builds from a fresh state (24 gated). The fault is the full GDI's own, not a cache's. The full GDI's segments 36 and 38 are 32-bit
(NE flag 2000h, its flat thunks), and the frame writer (build/asm16.py) decoded them as 16-bit
code: `66 B8 iw` took the `66 8E` of the next `mov gs, ax` for the rest of an imm32, and the
`E8 58 73` left over became a near call to 38:7358, past the segment's 13ECh bytes. That place
became an entry no piece defines, absent.c gave it a stand-in in XCODE, and the near call crossed
segments. The fix: a 32-bit segment is read with the 32-bit decoder and its functions are all
rebuilt from their instructions. A near call or jump past a segment's end is bytes, and so is a far
place past it. The full GDI then links from a fresh state, segments 36 and 38 at their numbers and
sizes. The all-15 MINI.CAB boot, built from a fresh state: PASS, the License page.

**The linker's backlog.** A 32-bit segment is written into the 16-bit MCODE class, each
instruction as its bytes with its 32-bit spelling beside them, not as use32 mnemonics. wlink keys a
class on its name and its bitness (objpass1.c, FindALeader), so `order clname MCODE ...` cannot
place a use32 segment among the 16-bit ones. Written use32, CODE36 and CODE38 went after every
16-bit segment and renumbered them. wlink's Windows NE writer also sets no 2000h flag for a use32
segment, so the built GDI's 36 and 38 carry 1D10h where the original has 3D10h. To write them as
use32 source that keeps both: wlink's ORDER taking use32 segments into a 16-bit class's sequence,
and the NE writer setting 2000h.

The 16-bit recorded-system context path has no record-level case of its own in test_native_trace
(prelude runs do), only the judge's.

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

**The switch** (gone: deleted with the Python at the 2026-11-04 milestone, below). It was
`CB_NATIVE_ENGINE`, read when a database, a store or a knowledge base was opened; without the built
DLL opening one now raises (native_engine.why_not() says why). Build:

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

Identical everywhere tested, so the switch went on by default; the Python Database, Run/Context,
FunctionStore and KnowledgeBase were deleted at the 2026-11-04 milestone (below), and
tests/test_native_engine.py now holds the C++ to the Python's answers as recorded before they went
(tests/data/native_engine_golden.txt).

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

### The engine in C++, piece 2: scopes, inference and lowering -- `native/pyengine`

Items 1-3 above, done (2026-10-31): `pyengine.exe <package dir> --coverage --out DIR` writes L2 per
module and the coverage ledger; a C ABI (`pyengine_api.dll`) gives the same to any caller.
native/pyengine/README.md has the whole design. In short:

- **Scopes** by CPython's symtable rules (module, class, function, lambda, comprehension;
  global/nonlocal; free names and cells), on a child walker, `ast.walk` and a parent map built here.
- **Inference**: infer.py and methods.py rule for rule over pyfront's tree (intervals on unbounded
  ints, union-find containers, records, branch narrowing, loop fixpoints, contracts at calls, the
  rounds to a fixpoint). `--old` runs exactly these rules.
- **Contracts, then the extensions**: the classic facts are frozen once they settle; code typed
  afterwards that would change one is refused at that statement. That keeps every function the old
  engine translated byte-identical. The extensions: *demand typing* (a parameter no caller types
  takes the type its uses demand), *per-caller specialization* (copies per argument signature,
  written as overloads), *closures* (capturing lambdas in `py.fn<R, P...>` locals), *generators*
  (a class deriving `py.generator<T>` whose `next()` resumes after each yield), *tagged variants*
  (`py.variant<...>` where scalar kinds meet, narrowed by `isinstance`), *Optional* as before and
  wider, *`re`* (patterns read for which groups may be unset; every call carries its pattern for the
  Python-compatible regex engine), *files and os/os.path/shutil/time/hashlib* rows.
- **Lowering** to the same L2 shapes the Python front end produced, written exactly as l2.to_json
  writes them.

**Gate 1 -- equals or beats the old engine** (all of codebreaker/: 564 modules, 8,158 functions):

| run | old engine translated | L2 byte-identical | refusal reasons the same | units identical |
|---|---|---|---|---|
| `--old` strict | 277 | 277 | 7,881 / 7,881 | 564 / 564 |
| `--old --checked` | 359 | 359 | 7,799 / 7,799 | -- |
| extended strict / checked | 277 / 359 | 277 / 359 (none refused) | -- | -- |

**Gate 2 -- coverage**: translated functions 277 -> **517** strict (1.1% -> **2.4%** of lines),
359 -> **656** checked (1.5% -> **3.2%**); formats 3.7% -> 6.0% (9.3% checked), java 1.5% -> 7.0%,
decompile 1.2% -> 3.7%, build 1.4% -> 2.9% (the per-package table is in the README). stdlib refusals
1,435 -> 512, syntax 620 -> 476; the remaining roots are untyped parameters no use pins down
(1,205: many are visitors over Python's own `ast` nodes), callers passing values of no type, getattr,
mixed-value dicts, `id`, `next`, `*args`. A run takes ~50 s (the Python engine: 115-130 s).
CTest: 8 suites, 70 checks (scopes, inference, specialization, closures, generators, variants,
regex, files). Left: the C++17 back end over this L2; `next()`/`zip()` over generators, escaping
closures, variants beyond scalars.

**Translated => compiles => agrees (2026-11-04).** The numbers above counted functions nothing had
compiled. Now `l2cpp17` is linked into the engine: a function counts as translated only when the
back end writes it (declines refuse that one function and its callers), `--cpp DIR` writes every
module, and the judge (state/work/pyengine2/judge.py, oracle side) compiles all of it with MSVC,
links one driver and runs every module-level function on generated inputs beside the Python,
comparing results and exception names; what does not compile or agree is refused through
`native/pyengine/hints/refuse.json` with the judge's reason. Getting there was mostly back-end and
runtime work the judge found: no C++ had existed for the extensions' library rows (now
`native/ported/include/py_os.hpp`: open/files, os/os.path/shutil/time, hashlib, re over pyre,
generators, variants; `py_unicode.hpp`: Unicode's isalpha/isdigit/isnumeric), the engine's L2 had
dropped class types' record/ref flags, `1 << 32` on a C++ int, `d.get(k)` not None when absent, NUL in
string literals, CRT macros as names. Also new: getattr with a constant name (the attribute when it
is always there, the default when it never is), hand-written parameter hints
(`hints/types.json`), module names whose value calls a refused function refused with their readers.

| policy | translated | compile | run | agree | old engine's, byte-identical |
|---|---|---|---|---|---|
| strict | 472 (2.1% of lines) | 472 | 287 | 287 | 264 / 274 (10 refused: do not compile or agree) |
| checked | 582 (2.8%) | 581 | 332 | 332 | 339 / 356 (17) |

The leaf packages (coverage, jobs, store, knowledge) are glue over sqlite3, threads, contextvars,
process pools, csv and json: parameter hints do not move them (8 hints on knowledge's facade: 2
functions). The engine carries computation -- format readers, decoders, text helpers; glue is
cheaper to write by hand. CTest: 10 suites, 87 checks (+ `dynamic`, `backend`).

## Step 5 (now first): the L2 core and the C back end -- `native/l2`

Logan, 2026-10-31: "All of it needs to be C++." The translator's core moves first: the L2 tree, and the
C back end on it, so decompile -> C is native end to end (the decompiler already builds L2 in C++,
PrintL2). `native/l2` (CMake + Ninja, VS 18 toolset) builds two static libraries and one DLL:

- **`l2core`** -- the L2 core (`src/value.hpp`, `src/json.hpp`, `src/l2.hpp`): what the translator back
  ends (C++03, C++17, Java, C#) and the porting engine's lowering sit on;
- **`l2c`** -- the C back end on it (codebreaker/translate/backends/c.py; below: gated, on by default);
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

### The C back end -- `l2c` (gated, on by default)

**What moved.** All of codebreaker/translate/backends/c.py: the layout (`cemit.cc`: PrintC's pretty
printer, its operator tokens and reverse-Polish stack), the writer (`cback.cc`: types, expressions,
conditions, statements, the heading and the function), every c_text pass done on the tree
(`cpasses.cc`, `cpasses2.cc`: the pointer passes, the module-facts passes, `_Locals`, `_buffer_fields`,
the stack-segment stores, the 32-bit path's and the PowerPC path's own) and what the module decides
(`cmodule.cc`: function, flags, signature, module_names32, type_names, parameter_names,
struct_accesses, takes32, prototypes, segment_plan, caller_frame_plan, seg_layout, fragment, module).
A port line by line: the same names, the same order of work, the same text; c.py's regular expressions
are carried over as written on a port of Python's `re` (`pyre.cc`, now the core's: backtracking, Unicode
\w\d\s, lookbehind, lazy quantifiers, MULTILINE/DOTALL, finditer/sub with Python 3.7's empty-match rule).

**What stayed Python.** c_text's module assembly (compilable16, compilable32, compilable_powerpc: the
glue that calls the writer); struct_text.plan (its analysis is analyze.structures over analyze.image,
the loaders step's -- native/formats is there to build on); the passes before the writer that read a
16-bit function's instructions, asm_blocks, frame16 and shape16 (CB_ASM_BLOCKS / CB_FRAME16, off by
default; an uncommitted capstone wrapper for them waits in the worktree).

**The switch** (gone: the Python writer and `CB_NATIVE_L2` were deleted at the 2026-11-04 milestone,
below). backends/c.py's entry points are native_l2's; without cbl2.dll a call raises
(native_l2.why_not()). The shim hands values over as JSON: an L2 document once and then by number
(the decompiler's note handed over as the text it was read from), a function document's four small
answers in one call (`describe`), a module's large flat sets once and then by number; the deep entries
run on a fiber with a 512 MB stack on the caller's own thread.

**The gate** (state/work/native-l2/gate.py): each module made compilable twice, `CB_NATIVE_L2=0` and
`=1`, each in a fresh process (compilable16's cache bypassed), one worker; every function's C, the
header and the module's tables (near, wide, novalue, exported prologue, interrupts, prototyped,
selector imports, failed, code data and reads, declarations, imports, order; 32-bit: imports, in_ecx,
in_edx; PowerPC: novalue, externs, argc):

| set | modules | entries | identical |
|---|---|---|---|
| 16-bit (l2c --made's) | lzexpand keyboard comm lz32 krnl386 gdi user | 5,425 | 5,425 |
| 32-bit | version imm32 tapi32 winspool mpr advapi32 user32 gdi32 comctl32 kernel32 shell32 | 7,073 | 7,073 |
| 32-bit, structures on (`CB_STRUCT_RECOVERY=32`) | mpr version tapi32 winspool comdlg32 | 797 | 797 |
| PowerPC | hud-b0ab65e560b6.exe | 670 | 670 |

And every recorded call (capture_site + compare.py: text, exception, the facts written) of the 16-bit
set (6,510 function calls, 42,434 arguments_at, 21,044 words ...), the small 32-bit set, shell32 and the
PowerPC module identical; tests/translate/test_native_l2.py makes every call test_c_backend.py makes
through both writers and compares the whole outcome.

**Speed.** Seconds inside backends/c's entry points (the C writing), off vs on, in the final gate run
(the machine shared with other sessions; DLL load and first use outside the clock, as the Python's
module import and pattern compiling are). On is faster than off on every module of the gate:

| module | off | on | | module | off | on |
|---|---|---|---|---|---|---|
| krnl386.exe | 17.2 | 6.2 | | kernel32.dll | 12.3 | 8.5 |
| gdi.exe | 61.7 | 15.5 | | comctl32.dll | 11.2 | 6.4 |
| user.exe | 49.4 | 14.7 | | shell32.dll | 10.9 | 7.0 |
| keyboard.drv | 1.02 | 0.34 | | gdi32.dll | 3.13 | 1.46 |
| comdlg32 (structs) | 1.99 | 0.92 | | user32.dll | 0.58 | 0.35 |
| hud (PowerPC) | 1.23 | 0.75 | | mpr.dll (structs) | 1.36 | 0.36 |

On the pure writing step (recorded calls, JSON both ways) the native writer is about 3x the Python on a
loaded machine (7x measured once on a quieter one); end to end the Python glue around it (c_text's
module assembly, the shim's JSON) is the rest. The profile build (`-DCBL2_PROFILE`) writes per-pass
seconds to cbl2_profile.txt: what is left is spread over the passes (far_locals, buffer_fields,
caller_frame_slots, names_used, heading_facts each about a tenth), the layouts about a tenth.

**Left.** The module assembly (c_text) and struct_text.plan into C++, after which nothing crosses the
shim per function; asm_blocks / frame16 / shape16 (off by default). (The Python writer: deleted, 2026-11-04.)
## Step 4 (six readers moved and their Python deleted; the rest of formats/ to go): formats and loaders -- `native/formats`

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

### 2026-11-04 (evening): the six readers' Python deleted

**The Fallbacks closed.** Counted over the whole corpus with the gate (native/formats/gate.py
now lists every Fallback reason it meets and fails on any), the only one ever reached was
`dos.read: a program whose unpacker runs in the emulator` (10 files). That is now C++:
dos/unpack.cc is emulate/unpack.py on Unicorn's C API (core/uc.h, the very unicorn.dll Python's
`unicorn` package carries, handed over once by native.py through `cbf_unicorn`; nothing linked).
The rest, never reached by any file, now answer instead of falling back: a PE entry past 64 bits
is written as the 65-bit int Python made (`Out::uinteger65`); `out[base:base+HEADER_SIZE] = hdr`
past an LE image's end appends as Python's slice does; unpacked_file's relocation outside the
image reads with unpack_from's rules (negative counts from the end, else struct.error); an int
past 64 bits raises OverflowError; an allocation past memory (an LE object/page copy, an NE
alignment past 2**40, an OMF segment or repeat, a PE export table of 2**26+ slots) raises
MemoryError; an argument that is not the expected dict, or a malformed argument stream, raises
TypeError; and any other C++ exception is raised as RuntimeError("formats.dll: ...") instead of
handing the call to Python. The one Fallback left is omf.read's a.out object, which omf.read
itself hands to exact/aout.py before asking the C++.

**The gate** (switch off = the Python readers, on = the C++, every fact compared), after the change:

| reader | corpus | files | identical | fallbacks | Python s | C++ s |
|---|---|---|---|---|---|---|
| MZ/DOS | Win 1.0/3.1/95/98 | 10,042 | 10,042 | 0 (was 10) | 50.59 | 6.99 |
| PE | Win 1.0/3.1/95/98 | 10,042 | 10,042 | 0 | 9.68 | 11.50 |
| NE | Win 1.0/3.1/95/98 | 10,042 | 10,042 | 0 | 47.95 | 19.43 |
| LE | Win 1.0/3.1/95/98 | 10,042 | 10,042 | 0 | 25.84 | 20.13 |
| OMF | the kits | 810 | 810 | 0 | 6.76 | 8.34 |

(DOS is 7x faster: the 10 packed programs' unpackers now run in C++.) XEX's questions did not
change (90/90 identical at its own gate); its re-run over the Xbox 360 library was stopped at the
library walk. A new question, `dos.read_mz` (kernel_parts' SYSINIT: the MZ program an image's
header describes), matched dos._read_mz on 582 of 582 MZ files sampled from the corpora.

**Deleted:** the Python readers of pe.py, ne.py, le.py, dos.py, omf.py and xex.py (their
`header` listing parser too), native_pe/ne/le/dos/omf.py, native.py's switch, `asker` and
`native_first`, and emulate/unpack.py (its one other caller, test_dos, now asks `dos.unpack_emulated`). Each module is now its API: a def with its signature and docstring under
`@native.ask` (a method) or `@native.answered` (a function), formats.dll answering; without the
DLL a reader raises native.required()'s OSError. `CB_NATIVE_FORMATS(_<READER>)` is gone. Kept as
Python, and why: omf.patched and rename_symbols (a regular expression; a Python function on every
name), dos.kernel_parts / exepack_stub / programs and le.corpus_files (assembly over
build/exepack and corpus walks, reading no format), ne.open_boot_image (reads two files and asks
embedded/from_buffer), xex.basefile / basic / import_libraries (the xex program). native/ported/ne's
input, ne.py as it was, is kept at native/ported/ne/src_python/ne.py (py2cpp ports it with that
file in codebreaker/formats/ne.py's place). tests/test_native_formats.py now holds the C++ to
digests of the Python readers' facts recorded before the deletion (14 cases).

Checks after the deletion: test_native_formats 14/14, test_dos 20/20, test_xex 5/5, test_frozen
22/22, test_drivers 16/16, test_plumbing 17/17, test_whole_module 15/15, test_exepack and
test_pklite pass, check_imports 553/553, tidy clean; run_all --quick 45/45 suites, again with every
native library (l2, engine, tracer, formats, dosexec) built from this branch's own sources.

### 2026-11-05: group 2 -- detect, zx, lzx, cab, media -- in C++, their Python deleted

New in formats.dll: `detect/detect.cc` (signatures, lanes, the DOS header, identify), `zx/zx.cc`
(.sna, .z80 v1/v2/128K, .tap), `lzx/lzx.cc` (LZXD and the XEX block chain, SHA-1 by native/xex's
crypto; `lzx.h` for the C++ readers that hold LZX, xnb next), `cab/cab.cc` (headers, stored /
MSZIP / Quantum folders across a set's cabinets, extract with the files' times, write) and
`media/media.cc` (FAT12/16/32 with long names, the partition table and logical drives, ISO-9660
with Joliet and raw sectors, El Torito's floppy, put()); core/codecs.cc (cp437, utf-16-be,
utf-8 with "replace" by CPython's rule) and file helpers in core/files (sizes, folders, writes,
times, ntpath's abspath/dirname/join). MSZIP uses zlib 1.3.1, the copy native/decompiler carries
(its static library `cbf_zlib`, crc32 in core/zcrc32.c). native.py gained `RAISES` (a reader's
own exception: cab.CabError) and zlib.error, FileNotFoundError, PermissionError, RecursionError.

Each module is its API over formats.dll. Kept as Python, and why: detect.kind (lane, then
dos.kind -- both C++), flat and wide_resources (one comparison on a name); cab.Cabinet (its
folder_of), cab.main and _gather (the command); media.FatVolume / IsoVolume (their attributes
are the C++'s dicts; files/read/_next/_chain/boot_image ask it with the volume's place, the FAT
passed as the call's bytes), media.BundleVolume (over formats.ne), media.volumes' first step
(xbox.volumes, group 4) and media.extract / main / _safe (a walk over every kind of volume,
Xbox's and the bundle's included, writing files).

**One difference, by design: cab.write's MSZIP bytes.** Python's zlib module is zlib-ng 1.3.1;
formats.dll's deflate is zlib 1.3.1, so a written cabinet's compressed blocks differ (both are
valid deflate; inflate is the same in both). The gate checks that every cabinet written either
way reads back to the same files, that each side decodes the other's, and that a stored
(`compress="none"`) cabinet is byte-identical (file dates masked). A block's short/stored choice
follows the compressed size, so it can fall differently on data that barely compresses.

**The gate** (state/work/formats-native2/gate2.py: the Python modules as they were, kept in
state/work/formats-native2/orig/, against the shims; every answer compared with its type, or the
exception's name; fallbacks counted and failing):

| module | inputs | files | identical | fallbacks | Python s | C++ s |
|---|---|---|---|---|---|---|
| detect | the four corpora, Win95 disc folder, tests/data, state/win95 | 11,835 | 11,835 | 0 | 141.10 | 170.53 |
| zx | test_z80's demo (.sna, .tap), .z80 v1 / v1 packed / v2 / 128K made from it, and cut copies | 36 | 36 | 0 | 0.02 | 0.04 |
| lzx | 400 compressed XNBs (Arcadecraft), each whole, cut twice and damaged | 400 | 400 | 0 | 303.71 | 8.36 |
| media | every disk/CD image under those roots, both name modes, attrs, files, reads; put() on copies | 136 | 136 | 0 | 5.07 | 4.50 |
| cab | 171 cabinets (105 MSZIP, 73 Quantum, 13 stored, 10 LZX folders): read, every folder, extract (names, bytes, times); checksum; write x16 | 192 | 192 | 0 | 2,669.96 | 108.66 |

detect is slower in C++ only by the ctypes calls (seven questions a file, each opening it).
Also with the shims: test_media 12/12 (put() into FAT16 partitions and FAT32), test_z80 11/11,
test_xbox_formats 18/18 (4 skipped), test_plumbing 17/17, test_xna 9/9 (6 skipped), test_drivers
16/16, test_frozen 22/22, test_native_formats 14/14, test_dos 20/20, check_imports 555/555, tidy
clean; run_all --quick all but test_native_trace's test_same_record_lz32, which fails the same
with group 2's Python put back (state/work/formats-native2/with_orig.py): not this change.
Outside callers checked: Windows95/tools/place_map.py (detect.format_of); Dashboard, Win95VM and
XboxMCP use none of the five.

### 2026-11-05: group 3 -- fonts, resources, inspect, pe_functions, powerpc_image, xex_edit -- in C++

New in formats.dll (`native/formats/win/`): `fonts.cc` (the .FNT header, the glyph table, glyph and
render), `resources.cc` (strings, dialog, menu, accelerators, version, icon/cursor groups and the
.ico/.cur and .bmp makers, type libraries, the 16-bit resource table), `powerpc_image.cc` (the
function table both ways and the layout whose entries point at code, import stubs, register
save/restore runs, `bl` targets; the virtual methods run-time type information names),
`pe_functions.cc` (the sweep, the starts, the extents, the whole-section listing), `inspect.cc`
(strings, the hex listing, categorize), `xex_edit.cc` (privilege bits read and changed in place).
Core additions: `core/build.h` (an answer built as a tree and written at the end: put() is a dict
assignment, emit() writes it), `core/cs32.cc` (Capstone x86-32 by the capstone.dll Python's package
carries, found by path -- `native.capstone_dll()` -- the very decoder `platforms.decoder(32)` gives).

Each module is its API over formats.dll. Kept as Python, and why: `Resource` and the PE walk over
`pe.resource_tree` and `decode()`'s choice of decoder (resources); `Font`'s attributes (the C++
answers them as a dict; glyph/render/describe ask or format); `Image.offset/word/bytes_at` and the
`.json` base beside a basefile (powerpc_image); PeCarve's queries that read Capstone's own
instruction objects (`resolve_call`, `calls_in`, `disasm_func`, `data_refs`, `xrefs_to`,
`string_refs`) and `PeSeg` (base-relocation mask); `PeCarve.insn` is now a lazy mapping: the decoded
addresses come from the C++ and an instruction (Capstone's Python object, which `symbols/names.py`
reads) is decoded when asked for; inspect's `describe`, `markdown`, `render`, `folder_map`,
`resource_body` and the command (they assemble the readers' answers: the readers they call are C++);
xex_edit's `Image`, `hook` and the command (the XEX writer and the PowerPC assembler).

Differences, by design: `PeCarve(path, progress_cb)` and `disasm_whole32(..., progress_cb)` call
`progress_cb` once, when the work is done (formats.dll sweeps whole); `inspect.categorize` is gone
from the module (nothing used it; `strings` does it in C++, ASCII-exact -- every string it is
given is ASCII).

**The gate** (state/work/formats-native3/gate3*.py: the Python modules as they were, kept in
state/work/formats-native3/orig/, against the shims; every answer compared with its type and its
order; fallbacks counted and failing):

| module | inputs | files | identical | fallbacks | Python s | C++ s |
|---|---|---|---|---|---|---|
| fonts | every .FON/.FNT/NE under the corpora (fonts_in, describe, render, 9 glyphs, damaged .FNT) | 3,928 | 3,928 | 0 | 8.74 | 6.14 |
| resources | every PE and NE module under the corpora: read, decode of each resource, each decoder both ways on every resource and 5 damaged copies of every 7th, groups + icon_file, bitmap_file, type libraries | 3,929 | 3,929 | 0 | 24.98 | 29.35 |
| powerpc_image | the basefiles of the Xbox 360 library (11 XEX; 3 are not XEX2 files and skipped): read, virtual_methods, regions, word | 11 | 11 | 0 | 63.46 | 25.69 |
| xex_edit | the 15 XEX of the library: privileges, six change sets, damaged copies | 15 | 15 | 0 | 1.01 | 1.51 |
| pe_functions | function_spans on 643 PE32 modules; PeCarve (funcs, starts, call targets, every insn address, 400 sampled instructions, calls_in, disasm_func) on 60; disasm_whole32 on 60 (two inputs each) | 643 / 60 / 60 | all | 0 | | |
| inspect | strings (5 option sets), hexdump (5 ranges) on 301 files; 31 categorize strings (non-ASCII ones too); resource_body for 40 modules' resources | 301 / 40 | all | 0 | 59.08 | 72.51 (the harness; see below) |

The resources gate found one difference at first (bitmap_file of a damaged header: `struct.pack("<I")`
raises `struct.error` past 32 bits, where the C++ wrapped) -- fixed with a checked pack.

Speed (one call, kernel32.dll / shell32.dll): `PeCarve` 3.6 s -> 0.74 s (4.9x; the sweep is native, the
instructions are no longer 2 KB Python objects each); `function_spans` 0.55 s -> 0.41 s (Python's was
already Capstone's C iterator); `inspect.strings` 0.09 s -> 0.035 s (2.7x). The gate's wall times are the
harness's (it builds and compares the answers); powerpc_image's 63 s -> 26 s is `virtual_methods`' regex scan.
Python lines: 791 removed, 117 added (codebreaker/formats/: fonts, resources, inspect, pe_functions,
powerpc_image, xex_edit, native). Also with the shims: check_imports 555/555, tidy clean, test_powerpc,
test_versions, test_xex_edit, test_xbox_formats, test_x360, test_vmx128, test_frozen, test_native_formats.
Outside callers checked: Dashboard, Windows95/tools, Win95VM and XboxMCP import none of the six
(searched their .py for the module names and their functions); in CodeBreaker: analyze/image.py
(PeCarve.funcs/exports), emulate/emulator.py (PeCarve, PeSeg), symbols/names.py (carve.insn,
_sec_of), symbols/runtime_library.py, verify/lanes/win32.py, compare/versions.py,
source/pe_writer.py, verify/authored/match.py (function_spans), recompile/discover.py (Image).

## HANDOFF: the rest of formats/ (left for the next session)

Done: groups 1, 2 and 3 (above). Not started, in this order, each by the same process (C++ in
native/formats, the module made its API with `@native.answered` / `native.call`, gated against the
Python kept aside in state/work/formats-native3/orig/ by a gate in the style of gate2.py / gate3*.py,
then deleted). The gate harness, `with_orig`-style swap and settings script are in
state/work/formats-native3/ (copy them; settings.json needs absolute paths: mksettings.py).

4. Xbox formats CodeBreaker wrote: xbox/ (stfs, fatx, zar, god, ciso, xdbf, xsb, xbe, xzp, gdf,
   volumes, texture, xpr, effect, heavyiron), gfx.py, xnb.py (lzx.h is there for it). The
   Dashboard imports xbox.gdf (`gdf._open`, `GDF`) and xex.basic and runs `python -m codebreaker
   xenon-recompile`: their APIs must stay exactly. Notes from reading gdf.py: it reads a "device"
   (`read_at(offset, length)`: a file, or god.SvodReader over a .data folder), so port god (the SVOD
   reader) with it, and let the C++ open the file or folder itself; `build_xdvdfs` is a pure builder.
5. formats/xbox/crypto/ -- ALL of it (Logan, 2026-11-05): CodeBreaker's own files (bootchain,
   cpukey_ecd, flashplan, fuses, hypervisor, keyvault, nand, smc, StreamIO) AND the vendored
   upstream tools ATTRIBUTION.md lists, each C++ port keeping its upstream attribution and
   license notice, and ATTRIBUTION.md updated to say what each became. Same gate: identical
   outputs on real inputs and the test samples (tests/test_xbox_crypto.py and its other users).

Also open: omf.patched / rename_symbols could move (the C++ walks the records; the shim applies
the regex/function to the names it returns); the XEX gate re-run over the Xbox 360 library; the
Python left in group 3 (inspect.describe and the rest of the presentation, PeCarve's instruction
queries) moves when the Capstone detail (operands) is wanted in C++.

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

## Step 7, first piece (gated, on): the DOS executive -- `native/dosexec`

exact/emu/dos.py `run()` (the real-mode DOS executive: PSP, the MCB arena in guest RAM, MZ load and
relocation, INT 20h/21h -- console, files by handle and by FCB over a MemFS or the disk, memory,
EXEC and 4B03h overlays, the PSP calls, version and clock -- installed real-mode vectors, INT 15h)
and exact/emu/dpmi.py (the DPMI host TNT ML runs under: the mode switch, INT 31h, raw switches,
protected-mode dispatch, real-mode interrupt reflection) are ported by hand, line for line, into
`cbdos.dll` (exec.cc, dpmi.cc, mcb.cc, fs.cc; C interface cbdos.h).  It calls the very unicorn.dll
Python loaded, maps the low megabyte from its own buffer, and keeps Python's error texts (the
block and time limits, cpu faults, "not implemented") because callers keep them (port/verify.py).
Done by hand, not through the porting engine.

The shim is codebreaker/exact/emu/native_dosexec.py: dos.run hands it a run when the switch is
on and the run has no Python hooks of its own (`instrument`, `instrument_when`, `driver` -- the
replay and hybrid tools), no DPMI host object made by the caller, and a plain `dos.MemFS` or none.
A MemFS's files, hidden folders and removals go over; what the run set or removed comes back into
the same MemFS.  Every caller (build/native_dos.py, exact/toolchain.py, link.py, linkwhole.py,
mspascal.py, port/asm16.py, port/verify.py) is unchanged.

One deliberate difference: disk files are unbuffered CRT descriptors, where Python's were buffered
file objects; what a program writes is on the disk at once, so an EXEC child sees all its parent
wrote (under Python a parent's last unflushed bytes were invisible to it).  No gated run differs.

The switch is **`CB_NATIVE_DOSEXEC`** (default 1; 0 = the Python executive, as also without the
DLL).  The gate (state/work/dos-native/gate*.py, Python vs C++, each in a fresh process, every
output kept and compared whole):

| set | entries | identical | Python | C++ |
|---|---|---|---|---|
| MASM 5.10A on 60 self-contained DOS 4.5 sources (build/masm.py's native path: .OBJ, .LST, log) | 60 | 60 | 38.1 / 36.4 s | 17.2 / 4.9 s |
| the same, 15 in one work folder (`assemble_many`) | 1 | 1 | 5.8 / 8.3 s | 2.7 / 0.8 s |
| TNT ML (DPMI) on 40 Win95 VxD sources, OMF and COFF (objects, console) | 80 | 80 | 70.2 / 76.6 s | 39.1 / 22.9 s |
| dos.run not fast (exit, console, the executed-block set, steps, trace_calls counts, files left) | 6 | 6 | 13.0 / 10.4 s | 1.0 / 0.7 s |
| dos.run on a MemFS (the files and removals it leaves) | 10 | 10 | 1.0 / 0.8 s | 0.8 / 0.7 s |
| exact/toolchain.py: MSC 3.00, 4.00, 5.00 (CL EXECs C1/C2/C3), MASM 5.10, MASM 1.00 on 12 recovered C / asm sources each, MemFS | 60 | 60 | 35.9 s | 32.6 s |
| the same on the disk (CB_MEMFS=0) | 60 | 60 | 26.5 s | 16.4 s |
| MSC 5.00's LINK (response file, PSP 1000h, not fast) + EXEMOD; DOS 2.0 LINK and MASM (FCB I/O, block limit); SORT/FIND/FC with stdin | 34 | 34 | 22.9 s | 14.8 s |

311 / 311 identical.  tests/test_dos.py with the switch on: 19 passed, 1 skipped (the native
decompiler not built), including test_native_masm_matches_dosbox, test_native_tnt_ml_matches_dosbox
and the DPMI and MCB-walker programs.  The time is now Unicorn's own (translation and execution of
the tools); a run that called Python per block (not fast) is 12-15x faster; MASM 2.2-7.5x and TNT ML 1.8-3.3x
(the first five rows were run twice, "first / second": the machine was shared with another session's
builds, and the first C++ run overlapped the Python one).

Left: exact/emu/isis.py (an 8080 interpreter in Python for the ISIS-II tools, PL/M-86) and
exact/emu/xenix.py (a XENIX a.out runner on Unicorn with its own syscall trap) share nothing of
the executive and are their own pieces.  The runs with Python hooks stay Python.

## The milestone of 2026-11-04: the replaced Python deleted

The regression, every native switch at its default (on), in a worktree with every native piece
built from its own sources and a fresh state folder:

- **run_all, the slow suites included:** 47 of 48 suites pass. The one failure is tools/tidy.py (19
  unused imports and definitions, static, older than this milestone and untouched by any switch).
- **The all-15 MINI.CAB Setup boot:** all 15 modules rebuilt from source (module.build: GDI 118
  functions as C, USER 514, KRNL386 18 ...), booted together: PASS, Setup's License page.
- **A sample sweep** (43 modules: about ten each of PE, NE, LE and DOS spread by size, plus the
  10-30 milestone's flagged system.drv, mouse.drv, rasapi16.dll), against the main store's last
  results: 38 the same or better, 5 worse (netcpl.cpl, awdevl16.dll, system.drv, mouse.drv,
  rasapi16.dll -- all 16-bit). Each of the 5 measured again with every native switch at 0 in a
  fresh state folder gave exactly the same counts: older regressions, none caused by the C++.

So the Python behind two switches is deleted:

| switch | deleted | lines removed |
|---|---|---|
| `CB_NATIVE_L2` | backends/c.py's Python writer (all but MOVED and the pass lists, Decline, SIZEOF, _param_name; the 21 entry points bound to native_l2's), native_l2's switch and dispatcher, tests/translate/test_native_l2.py (test_c_backend.py runs every case through the C++). The compilable16 cache key and the 16-bit lane's module stamp now carry cbl2.dll's identity. | 4,625 |
| `CB_NATIVE_ENGINE` | facts.py's Python Database and its helpers, engine.py's Run and Context, store/functions.py's FunctionStore, knowledge/store.py's KnowledgeBase, reads.digest's fallback, the switch in native_engine.py and verdicts.py | 1,653 |

After the deletions: run_all --quick passes but tools/tidy.py (17 now, the same older ones),
tools/check_imports 563/563, and the all-15 MINI.CAB modules rebuilt with the deleted code gone
boot together to Setup's License page again.

Kept, and why:

- **`CB_NATIVE_TRACE`, `CB_NATIVE_TRACE16`, `CB_NATIVE_TRACE_DRV`:** kept at this milestone for the runs
  beside a shadow; deleted with step 3 (above).
- **`CB_NATIVE_FORMATS(_<READER>)`:** kept at this milestone; deleted later the same day with the six
  readers' Python, once the Fallbacks were closed (step 4, "the six readers' Python deleted").
- **`CB_NATIVE_NE`** never existed: native/ported/ne is the porting engine's gate, not a loader.
- **`CB_ENGINE`** (the old direct reconstruct.run / module.build path, ~250 lines) is Python replaced
  by Python, not by C++; the module.build half also still reports conventions disputes the engine
  path does not.
- **Dead by import** (nothing imports them; each a `python -m` tool or a library, none a fallback):
  exact/fleet.py, formats/mscompress.py, verify/gate16/diverge16.py, formats/xbox/tort.py,
  formats/xbox/crypto/forensics.py and kdnet.py (1,296 lines), and 11 tools/ scripts run by path
  (958). Deleted with Logan's yes (2026-11-04) after the merge; git history keeps them. `score16` and `exact` are commands in
  __main__'s table missing from its listing.

## Step 8, first pieces (on, Python deleted): symbols/ -- `native/symbols`

codebreaker/symbols/ and codebreaker/analyze/ (about 9,900 lines) are being ported by hand, module
by module, into `cbsymbols.dll` (native/symbols; C interface cbsymbols.h: one call, `cbs_ask(question,
text as UTF-32)`, the answer as JSON in ASCII with `\u` escapes, so Python's str values come back
exactly). The shim is codebreaker/symbols/native.py (`native.ask`); the folder is the setting
`symbols_bin` (default native/symbols/build/bin). Build:

    (vcvars64)  cmake -S native/symbols -B native/symbols/build -G Ninja -DCMAKE_BUILD_TYPE=Release
                cmake --build native/symbols/build

Each module was gated before its Python went, so there is no fallback and no switch: without the
DLL the questions raise (native.why_not() says why). Gate harnesses: state/work/analyze-native/.

| module | what moved | gate | Python lines removed |
|---|---|---|---|
| symbols/demangle | the reader (`parse`, `type_name`): demangle.cc. Kept in the shim: declaration, short, plain, readable, stack_bytes, main (one-line wrappers over parse, and plain's regex) | every `?`-name and `.?A`-name found as text in the 2,562 corpus files (17,385 names) plus 4 random truncations of each and non-ASCII cases: 43,433 names, `parse` and `type_name` identical on all (4,033 readable); tests/test_symbols.py | 342 |
| symbols/prototypes | read_header, _split, _parameter and _build (every header of every folder, the merge of parameter names): prototypes.cc. Kept: _folders, _stamp, table, of, spelling, declaration | the whole table from the 5 header folders (OpenWatcom h/nt, h/win; the Win32 SDK; the Win95 DDK INC32, INC16): 10,166 prototypes, the JSON identical byte for byte, order included (Python 2.4 s, C++ 0.5 s); 360 mutated headers (truncated anywhere, lone CR, Latin-1 bytes, odd STDAPI_ forms): 1,984 prototypes identical; tests/test_symbols.py | 97 |

The knowledge-base stamp of the prototype table now carries a hash of cbsymbols.dll (the reader is
an input of the table), so the table is read again once.

Known, deliberate limits of the C++ (none met in the corpus): demangle treats only ASCII digits (and
Latin-1's superscripts, as unreadable) as digits where Python's isdigit() took any Unicode digit;
its nesting limit (400) stands in for Python's recursion limit; prototypes lower-cases file names
past Latin-1 as `?`.

**HANDOFF (2026-11-04).** Left, by importers across codebreaker/ (most used first): analyze/image
(15), symbols/api_arguments (12), analyze/call_graph (10), symbols/ordinals (8), runtime_library
(7), driver_services (7), analyze/conventions (7), symbols/names (6), analyze/dos_functions (6),
dos_interrupts, debug_build, analyze/strings (5), sdk_structs, structures, globals (4), and the
rest. What was learned:

- analyze/image is the keystone: call_graph, conventions, classes, structures, dispatch, globals
  and strings all take its `Image` (functions and instructions from the native decompiler, the
  module's spans, pointers and imports). Porting any of them alone means marshalling the whole
  image across ctypes per call; port image first (into native/symbols or a native/analyze beside
  it, reading the decompiler's output and the formats DLL directly), then the passes over it.
- The symbols/ modules left are mostly glue around JSON tables, the corpus and the formats readers
  (ordinals, driver_services, runtime_library, api_arguments); their computing cores (an import
  library's members, the VxD include-file reader, the signature matcher, api_arguments' stack
  scan) are what moves; each needs the table files' reads kept identical, and every knowledge-base
  or cache key that their output feeds must take the DLL's hash, as prototypes' does.
- The method that worked: a corpus-wide harvest of real inputs plus random truncations of each,
  compared whole (JSON dumps, order included), before the Python is deleted.

## Step 8, analyze/: the image and its passes -- `native/analyze` (2026-11-05)

`cbanalyze.dll` (native/analyze; C interface cbanalyze.h; shim codebreaker/analyze/native.py; folder
setting `analyze_bin`, default native/analyze/build/bin; built by `tools\build_native.cmd analyze`).
Like native/tracer it is compiled against the headers Python's `capstone` package carries and links
no Capstone: at run time it calls the very capstone.dll Python loaded (5.0.x), so every
instruction's mnemonic, operand text, operands and register sets are the ones Python's had.

The image is handed over once (`native.Handle(img)`: spans, function starts and limits, the
import addresses, the pointers in the file's order, the data segment, the roots; then the names)
and stays in the DLL; each pass is one question on the handle and answers JSON whose order is the
Python's. analyze/image.py keeps the loaders (they are glue over formats/ and decompile/describe,
other lanes' code) and the cheap address helpers (span, read, owner, label, covering); the
decoding, the jump tables, `discover` (call targets, then held code addresses, pass by pass) are
image.cc. A Python set's iteration order reaches the output in places (Tarjan's walk over each
function's callees, the far addresses a 16-bit instruction holds, `max(set(pops), key=count)`),
so pyset.h is CPython's own set table (open addressing, 9 linear probes, perturb 5, resize at 3/5),
which iterates in the very order Python's does for the same adds.

| module | what moved | gate | Python lines removed |
|---|---|---|---|
| analyze/image | _convert, function decoding, _jump_table, discover, _held_code_addresses, _looks_like_code: image.cc. Kept: the loaders, Ins/Function/Image (made from the answer), definition, arguments, destination; new: Image.linear (n instructions straight on, which ui/mcp_server read through the removed `_md`/`_convert`) | every pe/ne/le file of the 5 corpora in settings.json, deduplicated by hash (3,680): the reference image.py (from git) and the DLL, whole images as JSON (names in order, functions, limits, discovered, every instruction, tables): 3,680 identical, 0 different; plus comctl32, shell32, user.exe, vmm32.vxd by hand | 256 (46 added: the shim's side, `linear` for ui/mcp_server's straight-on reads) |
| analyze/call_graph | _build (calls, callers, users, refs, taken, Tarjan order and cycles, reach), _references, _relocated, _strongly_connected: call_graph.cc. Kept: Graph, READS/PURE_WRITES, analyse (the report) | the same 3,680 files: the reference module and the new one on the same image, the Graph whole and analyse() as JSON: 3,680 identical, 0 different; tests/test_analysis.py, test_mcp.py | 152 |
| analyze/conventions | _functions and every helper it used (_frame, _argument_slots, _register_inputs, _returns, _result_use, _cleanup, _loaded_before, _pushed_inputs, _entry16, _callback): conventions.cc. Kept: stack_delta (structures uses it), duplicates, thunk_layer, analyse | the same 3,680 files: the reference module and the new one on the same image, the Graph whole and analyse() as JSON: 3,680 identical, 0 different; tests/test_analysis.py, test_mcp.py | 413 |

No cache or knowledge-base key takes these modules' code: what they feed (describe32's ECX/EDX
functions, the struct and global text) reaches the caches as content (the description text, the
bodies), so a different answer is a different key by itself; and the answers are identical.

Known, deliberate limits of the C++ (none met in the corpus): the callback-name patterns read `\w`
as ASCII and fold case in ASCII only (Python's `re.I` folds Unicode); `upper()` of an import name
is ASCII.

**HANDOFF (2026-11-05).** Left in analyze/, by importers: strings (5: table, uses; feeds globals,
classes, module_writer), structures and globals (4), dispatch, classes, resources, dos_functions
(its own 16-bit decoder, no Image), origin, observe, ppc_calls, report, facts. Then symbols/
(api_arguments, ordinals, runtime_library, driver_services, names, dos_interrupts, debug_build,
sdk_structs and the rest). What was learned:

- Port a pass as one question on the handle: the image is already in the DLL, decoded, with its
  call graph (Image::calls, built once by call_graph.cc) and its names (name_text, import_text,
  label()). A pass's answer is JSON built in Python's key order; Python makes its objects from it
  (a reference's instruction is found again by `fn.index[addr]`).
- strings.table iterates `referenced`, a set comprehension unioned with another (`|=`): the
  answer's dict order is that set's order. PySet has add only; the union needs set_merge's
  behaviour (CPython copies the table outright when the target is empty and the masks match, else
  inserts the other's entries in table order).
- The gates: state/work/analyze-native2/ (gate_image.py: the reference image.py from git against
  the DLL, whole images as JSON; gate_pass.py: a pass's reference module against the new one on
  the same image, several passes per load). An NE file's load is mostly describe16 (Python), so
  the gate's time goes there, not to the analyses.

## Step 8, first piece: the compiler models -- `native/invert` (2026-11-04)

codebreaker/invert/ (the models that read 16-bit functions back into the source their compiler
was given: early_c, msc6, msc6_registers, msvc2, msvc2_forward, msvc2_registers, flow16, run;
about 9,300 lines with their helpers) is being ported by hand into `cbinvert.dll` (C interface
cbinvert.h; text in, JSON out, a Python exception's kind and message back).  The shim is
codebreaker/invert/native_invert.py.  Build it like the others (vcvars64 shell):
`cmake -S native/invert -B native/invert/build -G Ninja -DCMAKE_BUILD_TYPE=Release`, then
`cmake --build native/invert/build -j 1`; its folder is the `invert_bin` setting.

**Piece 1, flow16 (done, Python deleted).** flow16.py's analysis -- the leaders and blocks, the
edges, unreachable blocks, switches by compares and by a bounds-tested jump table (CMPSWITCH),
dominators, post-dominators, natural loops and the whole dominator-guided structurer -- is
flow16.cc, decision for decision.  flow16.py keeps the Python API its callers use (Block, SWITCH,
CMPSWITCH, build_cfg, structure; callers: early_c.recover_cfg, exact/draft, tests/test_invert.py):
the SWITCH hook is asked in Python for every indirect jmp and its answers sent with the
instructions.  The one place the Python's answer hung on a set's order (ipdom's first candidate,
only ambiguous for blocks that never reach an exit, whose candidate sets are dense small ints)
is CPython's order there: ascending.

The gate (state/work/invert-native/gate_flow16.py, both in one process, the switch toggled per
call): every `push bp; mov bp,sp` chunk (to the next, at most 4 KB) of every NE code segment, DOS
MZ load image and .COM file under the two corpora, decoded by Capstone, through build_cfg and
structure with CMPSWITCH off and on, and with no SWITCH hook and with one reading a word table out
of the chunk (raising for some).  Compared whole: each block's every field and its instructions'
addresses, idx2blk, the tree's repr, or the exception.

| corpus | calls | identical | Python | C++ (with the shim) |
|---|---|---|---|---|
| Windows95/design/corpus/files | 133,324 | 133,324 | 383 s | 239 s |
| Microsoft-Assemblers-Compilers (every compiler, assembler and tool kit there) | 1,780,844 | 1,780,844 | 2,465 s | 1,480 s |

1,914,168 / 1,914,168 identical.  tests/test_invert.py with the C++: 14 passed, 1 skipped
(Microsoft C 2.03 is not set up in settings.json: kits.msc203), including test_against_the_real_compiler,
test_a_module, test_dos_tools and test_16bit_assembly_source.  The speed-up is 1.6x end to end;
the shim's text and JSON (and the Block objects over the caller's instructions) are most of what
is left: the analysis itself is no longer the cost.

**Piece 2, msc6 (done, Python deleted, 2026-11-05).**  The `invert <module>` path's models:
msc6.invert (msc6.cc: the register walk, if/else, do-while, short-circuit and materialised
conditions, far/near/cdecl/vtable calls, far pointers, longs, globals, locals, the parameter
inference from the body and retf N), msc6_calls.infer_segment_sigs, segment_thunk (parse, verify,
emit_asm), whole_function.candidates and msc6_function._mask (msc6_helpers.cc), and the three
byte-level readers msc6_function.invert_cleanup, msc6_walk.invert and msc6_templates.invert
(msc6_small.cc).  **Capstone:** cs.cc loads the capstone.dll of Python's `capstone` 5.0.7 package
by path (cbinv_init, called by the shim with `<package>/lib/capstone.dll`; CMake reads the package's
include/ through `python -c "import capstone"`), so every mnemonic and operand text is the one the
Python saw; native/decompiler's Capstone 4.0.2 is not used.  pyutil.h makes the Python behaviours
the text depends on exact: int(x, 0) / int(x, 16) with their ValueError (a `[bp + si]` operand made
the Python raise, and so does the C++), "%x", chr() as UTF-8, split/partition, and small cursors
standing for each regular expression; the exception kinds the Python raised (IndexError,
ValueError, KeyError for a near jcc's 0x0f opcode, AssertionError) come back as the same types.
The Python modules are shims keeping their API and their self-tests (the compiler orchestration);
msc6_idioms.py (compiles snippets, nothing else) and grammars.py (text enumeration, no decoder)
stay Python.  Python 1,948 -> 282 lines over the seven modules (1,666 removed; the shim +66).

The gate (state/work/invert-native2/gate_msc6.py and gate_small.py, both in one process, 2
workers): every CODE segment of the 388 NE modules of Windows95/design/corpus/files through
infer_segment_sigs, and every chunk from a prologue (far loadds in both encodings, near loadds) to
the next or 4 KB through segment_thunk verify and parse (+ emit_asm), whole_function.candidates,
_mask and msc6.invert five ways (no parameters; the segment's inferred ones; void; long; a typed
far/int/long list with imports and a near map), and the three byte readers on each chunk and on it
cut at its first epilogue.  Compared whole: text and meta, or the reason, or the exception's type.

| models | calls | identical | Python | C++ (with the shim) |
|---|---|---|---|---|
| msc6 + msc6_calls + segment_thunk + whole_function + _mask | 144,845 | 144,845 | 103.7 s | 27.4 s |
| invert_cleanup + msc6_walk + msc6_templates | 55,215 | 55,215 | 0.2 s | 3.1 s |

(The byte readers were already cheap in Python; through the shim they cost more, which matters to no
caller: they are self-test tools.)  tests/test_invert.py with the C++: 14 passed, 1 skipped (kits.msc203
not set up), including test_a_module, test_against_the_real_compiler and test_routing_thunks_and_grammars;
check_imports and tidy clean.

**Piece 3, the MSVC 2.0 models (done, Python deleted, 2026-11-05).**  msvc2_forward (emit: every
construct -> its exact bytes, framed, frameless /Oy and __fastcall, the statement lists; to_c;
frame_layout), msvc2 (invert: the candidates from the bytes, each accepted only when emit gives the
bytes back under the relocation mask) and msvc2_registers (interp, the straight-line register walk)
are msvc2.cc.  The nodes are Python values in the Python's own shapes, so pyval.h/pyval.cc carry
them: None, bool, int, str, tuple, list and dict with Python's indexing errors, dict.get / d[k]
(KeyError), and == (1 == True; a tuple is never a list); on the wire a tuple is a JSON array, a list
{"L": [..]}, a dict {"D": [[k, v]..]} (native_invert._enc/_dec).  cs.cc opens a third handle,
32-bit x86 (disasm32, as platforms.decoder(32)).  Python generator laziness kept: the candidates are
collected until the Python's generator would have raised, tried in order, and the exception raised
only when none matched (a KeyError out of interp's `env["eax"]` reaches invert's caller, as before).
The self-tests (battery, the frame battery, the oracle round trips) stay Python.  Python 2,021 ->
325 lines over the three modules (1,696 removed; the shim +61).

The gate (state/work/invert-native3/gate_msvc2.py, both sides in one process, 2 workers): every
executable section of the 635 PE modules of Windows95/design/corpus/files; every chunk from
`push ebp; mov ebp,esp` to the next or 1 KB, that chunk cut after its first ret, and every frameless
chunk (after a ret and its padding, to the next ret); 204,359 distinct chunks (2,137 recovered).
Calls: invert with mask None and an empty mask, interp with nargs 0 and the chunk's own, emit + to_c
on every candidate the Python generated and its fpo form, and the batteries.

| model | calls | identical | Python | C++ (with the shim) |
|---|---|---|---|---|
| msvc2 + msvc2_forward + msvc2_registers | 822,993 | 822,993 | 750.1 s | 284.9 s |

**Piece 4, msc6_registers (done, Python deleted, 2026-11-05).**  invert_reg and invert_loop are
msc6_registers.cc (cbinv_msc6reg).  msc6_function's PROLOGUE and EPI went with it (their last
user).  Python 201 -> 42 lines (159 removed, 3 more from msc6_function).  The gate
(gate_msc6reg.py): every chunk from a prologue of every CODE segment of the 388 NE modules (the
other two prologue encodings given the model's own), cut after its first epilogue too, and the
model's prologue before every `mov di,[bp+N]`, `push si` and `push di` (to 512 bytes), each with no
imports and with three; 237,247 inputs, 130 recovered.

| model | calls | identical | Python | C++ (with the shim) |
|---|---|---|---|---|
| msc6_registers.invert_reg + invert_loop | 948,988 | 948,988 | 9.3 s | 59.4 s |

(Like the byte readers, msc6_registers declines at the first instruction it does not know, so the
Python was already cheap; no hot path calls it, only its self-test and tests/test_invert.py.)

**HANDOFF: what is left.**

- **run.py** stays Python: it is orchestration (win16.prepare, the compilers through build/msc6,
  verify/authored/match, the SDK prototypes), all of it other packages' Python; there is nothing
  in it that computes on its own.  It goes native when the pieces it calls do.
- **early_c** (+ early_c_frame), the last model: 3,129 lines, the 1983-84 compiler.  Most of it is
  `class R` (lines 124-2336, the MC 2.03 register tracker) and the text passes after it (_tidy,
  _fold_longs, _dead_regs with the `_DR_TOK` lookbehind regex, _fold_ladders, _fold_goto_or,
  _strip_tail_returns, _fold_shortcircuit, detect_copies).  Entry points: recover, recover_cfg
  (onto flow16.cc directly: build_cfg and structure without the shim's text), detect_copies,
  local_decls, to_c, framemodel (early_c_frame); callers: early_c_check, early_c_round_trip,
  early_c_forward, run.TOOLS, tests/test_invert.py (no other importer in codebreaker/, Dashboard, Windows95/tools, Win95VM or XboxMCP on 2026-11-05).  Use cs.h's
  disasm16, pyutil.h's int0/cursors, pyval.h if a value must cross as a Python object.  The gate: the
  gate_flow16.py chunker (every `push bp; mov bp,sp` chunk of every NE segment, MZ image and .COM
  under both corpora, Windows95 and Microsoft-Assemblers-Compilers), recover and recover_cfg
  compared whole, plus tests/test_invert.py.
- Pattern pitfall: pyutil's Cur::lit and std::string::compare throw std::out_of_range ("invalid
  string position") when the start is past the end; test the cheap prefix first (msc6_registers'
  gate caught one: it came back as RuntimeError where the Python declined).

## The rest, in measured-cost order

Each is measured again at its start (a profile of the slowest real job it serves) before the
order is trusted; the order below is what is known today.

| # | Step | Today (Python) | What makes it cost | Native form |
|---|---|---|---|---|
| 2 | **The 16-bit tracer and the driver runner** | emulate/windows16.py `trace_function16` (the Win16 lane's judge), emulate/driver.py `run` (VxD and other flat ring-0 code) | the same per-instruction and per-memory callbacks as step 1; USER's gate made 18 M register reads and 22 M memory reads for 81 functions (fast_uc.py) | the same DLL: a 16-bit recorder (segments, far calls, the NE import thunks, ports) and the flat driver runner, beside step 1's |
| 3 | **The path explorer** (done, above) | -- | -- | the shadow inside the recorder (shadow.cc); the tracers' Python hooks deleted |
| 4 | **Formats and loaders** | formats/ (PE, NE, LE, MZ, OMF, XEX ...), formats/pe_functions.py carving | reading and carving whole images in Python, per process, every run of a tool | a loader library (one per format, the decompiler's loaders reused) giving the same facts |
| 5 | **The L2 passes and the C back end** | the L2 tree passes and the C writer in Python, after the native decompiler | tree walks over every function, every build | into native/decompiler: L2 built and rewritten there, C written there; Python reads the result |
| 6 | **The engine and the store** (gated: "Step 6" above) | engine/, store/ on Python's sqlite3 | per-fact Python overhead around SQLite | native/engine: SQLite's C API directly, the engine's scheduler in C++; left: the steps themselves |
| 7 | **Build orchestration** | build/ (OpenWatcom, MASM, period compilers under DOSBox-X) | process starts and file handling around the compilers | a C++ driver: one process starts the compilers and gathers their output; the in-process DOS executive is done ("Step 7, first piece" above) |
| 8 | **The rest** | everything else that computes | -- | case by case; the glue (CLI, terminal UI, MCP) stays Python |

Not moved: the period compilers, OpenWatcom, MASM and DOSBox-X (other people's programs, run as
they are), and Unicorn and Capstone themselves (already native).
