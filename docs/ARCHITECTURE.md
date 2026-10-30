# CodeBreaker's next architecture: one program database, kept current by an engine, served

Design note, 2026-10-29 (adopted by Logan: all seven parts). It gathers what CORE_ROADMAP.md's
Track 2, IR_UNIFICATION.md and PLATFORM_DESCRIPTOR.md each began into one shape, and fixes the
order they are built in. What hurt on 2026-10-28 is the reason for each part: a cold rebuild of
USER took half an hour, the caches grew to 15 GB, nine servers sat idle, and every tool re-derived
what another had already worked out.

## The seven parts

| | Part | Today | The end state |
|---|---|---|---|
| 1 | **Program database** | a binary's knowledge is scattered: describe's text files, `/*@P`/`/*@S` notes inside the C, state/cache, the function store, loose JSON in state/ | one database per binary: bytes, segments, functions, L1 and L2, names, types, prototypes, signatures, verdicts -- every tool reads it and writes back to it |
| 2 | **Incremental engine** | scripts call scripts; each tool caches on its own | each step declares what it reads and makes; the engine recomputes only what changed, in parallel by itself; caching is what the engine *is*, not something added |
| 3 | **L2 the only form of code** | the decompiler prints C, c_text rewrites it with regexes, L2 rides beside it | the decompiler writes L2; every pass is a tree pass; C, C++, ... are back ends; nothing parses C text back |
| 4 | **One lane interface** | Platform descriptors exist (platforms/); lanes still wire loader, CPU, convention, emulator, compiler, packager each their own way | every platform fills the same six slots; the judge, the database and the UI work with any platform unchanged |
| 5 | **A service** | each command, the terminal UI and each MCP server start cold and recompute | one long-running process owns the databases and the engine; UI, MCP, CLI are thin clients |
| 6 | **A judge that explores paths** | fixed, directed, random and branch-guided inputs | concolic/symbolic execution finds the inputs that reach each path and proves paths equal, where it can; sampling only where it cannot |
| 7 | **One knowledge base** | SDK prototypes, library signatures, the name store, struct layouts each in their own place | one store keyed by platform that every lane and pass consults; anything learned once is known everywhere |

## How they fit

    clients:  terminal UI   MCP   command line          (5: thin, talk to the service)
                     \       |       /
    service:        the engine (2)  ----  knowledge base (7)
                         |
                program databases (1)   one per binary: facts, L1, L2 (3), verdicts
                         |
    lanes (4):  loader | CPU | convention | emulator (6) | compiler | packager

A **fact** is anything known about a binary: a function's start, its prototype, its L2 tree, its
compiled object, its verdict. Each fact is made by one **step** from other facts, and records the
version of the step's code and the hashes of what it was made from. When a fact is asked for, the
engine returns it if those still match, else recomputes it -- and, through the same rule, only what
depends on what changed. This replaces state/cache (its generations become step versions), the
describe text files, the notes inside C, and most of state/.

The database is SQLite per binary (state/db/<sha>.sqlite): tables for facts by kind, blobs for
large ones, kept to the facts of the current step versions (part of the size rule that
base/cache.py already enforces). The function store becomes the database's verdict table, so the
UI and the ledger read the same place.

## The order of work

Each step lands on master behind its switch, the old path kept until the new one gives the same
results (Logan, 2026-10-29: the full regression runs at milestones, not after every step).

A. **Database + engine core** (1, 2): the fact store and the step/engine API, and one pipeline on
   it end to end -- the 16-bit module build (describe -> decompile -> compile -> gate -> link) --
   giving the same image as module.build. Cold parallelism comes from the engine.
   See "Built: phase A" below.
B. **Service** (5): the engine in one process; the terminal UI and MCP as clients of it; the CLI
   starting it on demand. See "Built: phase B" below.
C. **L2 only** (3): the decompiler's output is the L2 fact; c_text's remaining passes become steps
   on it (IR_UNIFICATION step 3 continues); C is the back end's.
D. **Lanes on the interface** (4): win16 first (it is on the engine since A), then win32, drivers,
   DOS, PowerPC, the console lanes. See "Built: phase D" below.
E. **Knowledge base** (7): the name store, SDK prototypes and library signatures as facts of a
   platform database the per-binary ones refer to.
F. **Path-exploring judge** (6): concolic execution on the emulators, judging a function's paths
   as facts; sampling where it cannot.

## Built: phase A (2026-10-29)

The database and the engine are `codebreaker/engine/`, and the 16-bit module build runs on them
behind a switch: `python -m codebreaker build <module> --engine`, or `CB_ENGINE=1` (module.build
then hands a 16-bit module to the engine). The old path is still the default.

**The database** (engine/facts.py). One SQLite file per binary, state/db/<sha256>.sqlite. A fact
is (kind, key, value): `function.object` at `1:7cd4` (a function's address in the original), or
`ne.image` at "" (the whole binary). Each row keeps the step that made it, that step's version,
and the (kind, key, hash) of every fact it was made from. A value is kept as bytes, text, JSON
or a pickle, and its hash is the SHA-1 of that; past 4 KB it goes once into a blob table by its
hash. Size: the first time a step is used on a database in a process, its facts of other
versions are dropped (and the blobs nothing holds); all databases together are held under the
setting `db_max_gb` (default 4), the one opened longest ago deleted whole first. Files a step
writes as it works (a build folder) go beside it in <sha>.work/ -- not facts, deletable.

**The engine** (engine/engine.py). A step is a function declared with `@engine.step(kind,
reads=..., platform=..., uses=..., stamp=...)`. Its version is a digest of its own source, of
the modules and helper functions in `uses`, and of what `stamp()` gives (tool stamps, settings,
environment switches). What a fact depends on is recorded as the step asks for it
(`ctx.need(kind, key)`), so the dependencies are what was actually read; `reads` is checked
against it.

    db = engine.open(path)                       # the database; the binary is the fact `binary`
    engine.get(db, kind, key)                    # current: kept, or made again with what it needs
    engine.get_many(db, kind, keys)              # the stale ones made side by side
    engine.put(db, kind, key, value)             # a fact given by hand (an edited function's C)
    engine.forget(db, kind, key)                 # ... and back to its step
    engine.why(db, kind, key)                    # how it was made, and which inputs changed

A fact is current when its step's version is now's and every input is current with the hash it
had; that is decided once per run, from the bottom up. A fact made again with the same hash
leaves what was made from it current (a comment added to a function's C makes its object again,
the same bytes, and nothing past that). Stale facts asked for together are made on threads
(jobs/pool.threads), as many as the processors unless the setting `low_load` caps it (a number,
or true for base/parallel.workers()); a `batch` step is given its stale keys in one call (the
gate shares them out over its own four processes). `always` steps run once per run (a file's
hash: `ctx.file(path)`), and `Unkept(value)` is used and not kept (a verdict not reached).

    python -m codebreaker facts <binary>                 the kinds, counts, sizes, who made them
    python -m codebreaker facts <binary> <kind>          each fact: key, hash, size, made, version state
    python -m codebreaker facts <binary> <kind> <key>    one fact's value and what it was made from
    python -m codebreaker facts <binary> <kind> [key] --make      made current first
    python -m codebreaker facts --databases              every database and the cap

**The 16-bit module build on it** (engine/win16.py). The stages are module.build's own, called
as they are; the steps only say what each is made from:

    ne.segments, ne.module (lanes/win16.prepare: describe16 + decompiler + c_text), ne.functions,
    ne.header, ne.lines
    function.c -> function.source -> function.object, function.never_set   (per function)
    function.c + function.contract (the description's lines for it and its callees) -> function.gate
    ne.image   (module._build16 and module.build's gate loop, made of all the above)

The link is module._build16 unchanged: while it runs, a compile it asks for with exactly the
options and text a `function.object` fact was made from is handed that object (and the flow
check its `function.never_set`), and the compiler is not started; anything else it compiles
itself, so options that drift make the link slower, never wrong. The gate loop is module.build's
(verdicts already known first, the C functions of the build then run beside the original's in
it, a second link when some differ, not again after a resident-overflow rebuild); known verdicts
are taken per function, so a touched function costs one link when it still passes.

Evidence (this machine, low_load 6, other sessions' work running beside; seconds; each run a
process of its own, from an empty state folder -- no cache, no database):

    module          image == module.build's   module.build cold / warm   engine cold / warm / one function touched
    LZEXPAND.DLL    yes (b2113a90...)          23.1 / 13.4                26.9 / 0.1 / 4.1 (back: 4.2)
    KEYBOARD.DRV    yes (79749e52...)          8.5 / 3.6                  15.8 / 0.1 / 4.1 (back: 3.8)
    USER.EXE        yes (afd2d27f..., 40 gated) 1718 / 136                2628 / 4.5 / 201 (back: 115)

"Touched": a comment added to one function's C by hand (engine.put of function.c), then taken
away again (engine.forget): each time exactly that function's source, object, flow check,
contract and gate verdict were made again, and the link -- 1612 of 1613 objects kept, 645 of
646 verdicts -- and the image came out the same. Warm (nothing changed) checks ~10,000 facts of
USER and makes none. Of USER's 1625 compiles in the link, 1622 were handed the engine's objects.
The cold runs are slower than module.build's: they compile every function's flow check up front
(module.build skips those already forced to assembly) and the gate's 646 functions took 1909 s
against about 1215 s in the old run, the machine shared; cold speed is the other agent's work in
module.build and the decompiler, which the engine calls unchanged. Of the touched USER time, most
is module._build16 itself (the link, origin analysis, assembling) and contract16's two
descriptions -- the parts of the link that are not yet facts.

Tests: tests/test_engine.py (12, seconds: made once then kept, provenance recorded, a given fact
remakes only what depends on it, the same value stops the change, a new step version remakes
and drops the old, a changed binary is a new database, stale facts made side by side, undeclared
reads refused, batch steps and unkept values, files as facts, values and blobs, old versions and
the cap); test_plumbing and test_frozen pass.

**What phase A leaves for B and after.** ne.module is one step because describe, the decompiler
and c_text are one call today (compilable16); splitting them into facts of their own is phase C's
(L2 as the fact). The inner caches of the stages (compilable16, gate16_original, masm, the
decompiler's) are still state/cache: the engine is the outer layer, and each goes as its stage
becomes a step. The function store is not yet the verdict table. The engine runs in the process
that asks; phase B makes it the service, with its versions and memos kept across asks.

## Built: phase C, first part (2026-10-29)

**The notes are facts.** The native decompiler's notes before each function's C (`/*@G` a global
it touches, `/*@P` a pointer's size, `/*@S` a structure it recovered, `/*@L2` its L2 document) are
taken apart from the C once, as its output is read in (decompile/decompiler.split_notes):
run() hands on C alone and `Decompiled.notes` {function: facts} -- globals, pointers, structs,
l2, from_l2. decompile16 keeps them across its rounds (Description.notes; the L2 documents'
calls renamed with the texts), and c_text, compilable32, compilable_powerpc and decompile16's own
argument check read them there: nothing reads a note back out of C text (`_FACT`,
`_STRUCT_NOTE`, `moved_to_l2`, the `/*@C from L2 */` mark and decompiler.L2_DOCUMENTS are gone).

**The description is facts.** describe's Description holds its lines (`lines`; `records(kind)`);
the file the native decompiler reads is written from them (`write`), decompile16 adds and states
lines there rather than appending to the file, and c_text reads the records. On the engine they
are facts of the 16-bit steps: `ne.description` (the lines), `function.notes` and `function.l2`
per function, and ne.lines is made from ne.description; ne.image writes the decompiler's file from
the fact. (build/module.py and the 16-bit gate, other areas, still read the file written from
them.)

**L2 the only form of the 16-bit and 32-bit x86 C.** The printed C goes no further than
decompile16's rounds: every function's C is the C back end's, from its L2 document, and the passes
it does are gone from c_text -- ten on the pointers and the names (since 2026-10-28, now deleted),
and seven that need what the module says of the function, written with it once the module is
whole (c_text._write16, module_facts16; backends/c.IN_MODULE): the model word, the joined and far
arguments, the result made a word, `__export`, `__loadds`. The module's tables of near, wide,
no-value and export-prologue functions come from the documents and those facts (backends/c.flags).
A function the back end declines is one the decompiler could not make (GDI's sub_22_0B96, whose
statements PrintL2 cannot all write yet). docs/IR_UNIFICATION.md step 3 has the passes left in
c_text and why; c_text.py is 3,284 lines (3,621 before).

Since 2026-10-30 the back end also does, on the tree, the selector structures and structure gaps,
the calls through the function's own code segment, the interrupts, the definition in the older
style, the entry SP and the caller's stack words (what they find for the module -- structures,
interrupts -- comes back as facts that c_text gathers), and decompile16's rounds read calls and
arguments off the L2 documents, and the far-pointer chain has begun (`_far_addresses`,
`_far_casts`). What is left in c_text for 16-bit code is the rest of that chain (`_far_locals`
... `_near_to_far`), `_stack_segment_stores`, `_callee_pops` and the module-wide assembly;
c_text.py is 2,863 lines. `l2c --made` now also holds each module's header
and tables to the reference (recorded again from e93d9f5d7).

Gate: `tools/l2c.py --made` holds each function's final C to the printed path's texts, recorded
before the deletions: LZEXPAND, KEYBOARD, COMM, LZ32, KRNL386 the same, GDI but sub_22_0B96; the
module's tables (near, wide, novalue, exported_prologue, interrupts, selector imports,
prototypes, the header) the same as the printed path's. One fix in both paths before that, GDI's
reads behind `(int4)*(T *)(...)` (seven functions: IR_UNIFICATION.md). USER, gated for the first
time: 2,288 of 2,317 functions the same. Of the 29 others, 2 the back end declines (sub_3_160A,
sub_4_3FBE: now stand-ins), and the rest come from the decompiler's own runs, not a pass: with the
L2 notes on, the native program dies on other functions (PrintL2: MSGTHKCONNECTIONDATALS and
others), so it leaves out other ones (8 the printed run could not make now have C) and the
functions after a crash are typed in another process. That run also looped for hours before
decompiler._resuming took the last function written, in the description's order, as the one it
died on. Making PrintL2 survive those functions is the next step for USER.

## Built: phase F (2026-10-29)

The 16-bit judge (verify/lanes/win16.py) and the 16-bit gate (verify/gate16/isolated16.py) now
find their branch-guided inputs path by path instead of guessing them. Switch: on wherever the
lifter is there; `CB_NO_CONCOLIC=1` goes back to the guessing explorer (verify/guided.py) to
compare the two. The 32-bit and PowerPC lanes still guess (below; since then they explore too:
"Built: one run record, the path judge everywhere").

**The pieces.**

    native/decompiler/pcodecheck.cc  x86-16-real-lift (and x86-32-flat-lift): an instruction's p-code from
                                     the decompiler's own SLEIGH specification -- the one SLEIGH_VERIFY.md
                                     checks against the processor -- one long-running process per Python
    emulate/pcode.py                 the lifter's client: every instruction lifted once, by its bytes and offset
    emulate/symbolic.py              expressions over a run's inputs as plain tuples (folded where plain),
                                     made into z3 terms only for the conditions a question uses
    emulate/shadow.py                Shadow16: a run's symbolic shadow, stepped by trace_function16 before each
                                     instruction -- the instruction's p-code evaluated concretely (from
                                     Unicorn's registers and memory at that moment) and symbolically at once;
                                     only which register and memory bytes hold an expression over the inputs is
                                     kept. Inputs: the argument words, any byte of the data segment and of the
                                     scratch segment not yet written, what an unmodelled call returns (`ret`).
                                     A jump's condition over them is a condition of the path (its direction the
                                     one Unicorn then takes); an address or selector made from them is pinned
                                     at its value; a value of them into something not followed (a callee's
                                     made-up answer, a call modelled from its arguments, an op without meaning
                                     here, a run cut off) is said in `lost`. It never changes the run: anything
                                     that goes wrong in it ends the shadow, not the run (tests: the same run)
    verify/concolic.py               the explorer (generational search): each condition of each path turned and
                                     asked of z3 with those before it kept (one incremental solver per run);
                                     uncovered jump sides of the function's own code first, then its callees',
                                     then other paths through the same code, then string-instruction counts; of
                                     the inputs that turn a comparison, one at its bound; one input kept per new
                                     path; solver limits in z3's own steps (rlimit), so exploration is the same
                                     on any machine however busy (with a time limit, two runs of KEYBOARD's
                                     judge disagreed)
    lanes/win16.py explore_vectors   the lane's glue: inputs <-> the shadow's names, the seeds' paths known
                                     from their runs as fixed inputs (only one seed per path is run again with a
                                     shadow), every run handed back so the judge and the gate do not run the
                                     original again for the inputs it kept; where the paths found are not all
                                     and an own jump was seen one way only, the guessing explorer gets the rest
                                     (lane_common.GUIDED_REST runs) seeded with those runs -- a condition over a
                                     callee's made-up answer is no question for a solver

**Path equivalence, and what a verdict now says.** One input per path the original took is run on
both sides and compared as before. The judge's limits gain `paths=N` (distinct paths of the
function's own jumps) and `paths-all` when the exploration was complete: every condition of
every path found turned or proved impossible, no solver give-up, nothing lost. That is "all N
paths of the original reached and matched" -- for the inputs the explorer may set, with pointers
held where the inputs put them; it says nothing of values a path does not branch on (teeth below:
a constant changed in a DOS call's AH on a one-path function survives `paths-all`). The gate
says "paths N (all)" in a passing verdict's note.

**The judge itself, fixed where deeper inputs showed it wrong** (spurious differences, each seen
on a function the old inputs never took that far): `int 21h` is answered as DOS3Call (a module's
own code picks one by a flag its start-up sets; LZEXPAND's seven DOS helpers "faulted" against
C that calls DOS3Call); a code segment's selector is named by the original segment it holds and
a far pointer into a code segment by what it points at (a build orders segments and places
strings its own way: KEYBOARD's AllocCStoDSAlias and profile-string calls); the data places the
rebuild keeps for itself are left out on both sides, not only the rebuild's (an input that put
NEWTABLEEX's table at DS:0); a rebuild still going where the original returned gets 8x the
steps (C over a 448-entry table). win16 REV 23.

**Evidence** (this machine, other sessions' work beside; "before" is the tree as merged, run from
a copy of it; "after" this one; the same counting script for both).

The gate, cold (its caches emptied), serial, on Setup's modules' all-C builds:

    module        seconds       original runs     of them exploring   own jump sides compared   paths     all paths   verdicts
    LZEXPAND.DLL  38.1 -> 24.2  3852 -> 1516      2713 -> 393         169 -> 194  (of 506)      102->140  28 of 46    identical
    KEYBOARD.DRV  35.9 -> 29.9  2746 -> 1433      1801 -> 506         231 -> 276  (of 504)      129->237  25 of 34    identical
    VER.DLL       30.1 -> 24.0  2891 -> 1038      2188 -> 357         140 -> 168  (of 424)       72->119  17 of 31    identical
    all three     104  -> 78    9489 -> 3987      6702 -> 1256        540 -> 638                303->496  70 of 111

What is left of a cold gate is the fixed inputs on both sides and the emulator's own cost per run
(a fresh Unicorn per run, ~10 ms of setup): the next saving is there, not in the explorer.

The judge (`measure`, its own function store, CB_NO_FP_CACHE=1) and `teeth`:

    module        VERIFIED   weak      UNVERIFIED   judge time   teeth: caught
    lzexpand.dll  21 -> 28   3 -> 3    18 -> 12     18 -> 15 s   10 of 16 -> 10 of 22
    keyboard.drv  24 -> 30   17 -> 14  21 -> 18     48 -> 45 s   14 of 29 -> 21 of 31

Losing VERIFIED: lzexpand's sub_7_00F4 -- real: on an unmodelled call answering 2 it takes the
branch into sub_9_0000/sub_6_00A4 (both already UNVERIFIED), whose C leaves DS:262h written.
Losing VERIFIED (weak): keyboard's VKKEYSCANEX and MAPVIRTUALKEYEX -- real: with a key table
count set (DS:3Bh) they reach code whose C reads BX as sub_9_048E left it (`in_BX`), which the C
call does not carry; sub_2_031F -- real: once past its profile calls it leaves DS:33h unwritten.
Every other loss on the way here was the judge's own and is fixed above (and the shadow's: one
early version turned a selector no descriptor names into an exception inside Unicorn's hook,
which stopped the run -- now impossible, and tested).

Tests: tests/test_concolic.py (7, seconds: folded expressions mean what they were made from;
every path of a made-up function found and said to be all; seeds of one path run once; a loop an
input counts is sampled, not all; the lifter's p-code; a jump's condition through the p-code of
real 16-bit code in Unicorn, solved to its other side; a run with its shadow is the same run, on
LZEXPAND's functions). test_verification, test_drivers, test_frozen, test_engine pass.

**Left for after.** The pcodecheck in the setting decompiler_bin needs rebuilding for the lift
mode (`cmake --build native/decompiler/build --config Release --target pcodecheck`; until then
the lanes guess as before, or CB_PCODECHECK names a built one). z3-solver is a dependency
(requirements.txt). The 32-bit lane (win32.py) and PowerPC still guess: x86-32-flat-lift is there
and the shadow's evaluator is processor-blind past the register table and `segment`, so a
Shadow32 is the next step; a modelled call's result could follow the argument it passes through
(_llseek, _lread) instead of being lost; and a second input per path with every free byte
different would give stores the value diversity paths alone do not (teeth: removed stores
survive).

## Built: one run record, the path judge everywhere (2026-10-29)

**One run record.** Every platform's emulator slot (`p.emulator.run`) now returns the same
record, emulate/run.py `Run`, whichever emulator made it -- the 32-bit and 16-bit tracers, the
driver runner, the PowerPC interpreter, the 6502, Z80, SM83 and 68000:

    exit / ended    how it ended: the emulator's words / as the comparer says it (ret, fault, truncated ...)
    results         {register: value}: EAX (ST0); AX, DX:AX; eax, the flags and the result registers; r3, f1; A X Y ...
    writes          [Write(address, size, value, old, harness)] in order; `capped` when not all were recorded
    calls           [Call(name, args, detail)] the calls out, in order (args None: their number not known)
    io              [IO(step, kind, port, value, size)] device accesses in order (a machine: its bus log)
    stack           how far the stack pointer moved
    preserved       {register: (on entry, at the end)} the registers a function must hand back
    path, steps     the instructions that ran; how many
    truncated       cut off at a cap
    endian, raw     how a value's bytes lie; the emulator's own record (other tools read it, unchanged)

Each emulator module has `record(raw)` (its record as a Run) and `recorded(...)` (the slot: run,
then record); the module's own runner and record are what they were. The comparing moved to
verify/lane_common.py: `fingerprint(run, deps, ret, writes, saved, cov, reach, **own)` makes
what compare.py compares (the stack's move and the saved registers only for a run that
returned), `changed(run, place, word, most, everything)` / `left(...)` the memory left changed
(what a byte held before is what the first write found there; a harness write only says that;
`place` names a write's bytes or leaves it out, `word` gives a 4-byte value a token such as "S"),
`calls(run.calls, ...)` the calls as text. What each lane keeps is its own: win32 -- which places
are its frame and the rebuild's own image, "S" for a stored stack address, wild-call; win16 --
the data segment, the heap, a caller-frame helper's places, the calls and UART accesses merged by
step, LEFTOVER; the driver -- services named by the kit, ports after calls, its own memory digest
with the flags and result registers; PowerPC -- what an argument into an image points at, the
rebuilt image left out; the machines -- which registers, RAM and bus log make a tuple.
compare._exit_norm went (run.ended is it).

**The path judge everywhere.** The shadow (emulate/shadow.py) is one `Shadow` over a View of the
machine it follows, chosen by the platform's cpu slot (`shadow.mode(p.cpu)`: the lift mode from
the SLEIGH language, its bits and byte order): UnicornX86 (16 or 32 bits: register names as
Unicorn's, flags as EFLAGS bits) and PowerPCView (CodeBreaker's interpreter: r0-r31, LR, CTR,
cr0-cr7, XER's bits, f0-f31; memory read without making pages). Register and memory bytes are
kept least significant first on either byte order; addresses are cut to the address width (a
PowerPC register's upper word is not part of an address). Shadow16 (unchanged in effect), Shadow32,
ShadowLE (a driver: a service answers EAX and carry, both inputs) and ShadowPPC (64-bit `ret` in
r3) say only where an answered call's result lands and what it clobbers.

    native/decompiler/pcodecheck.cc  ppc-be-64-lift (ppc_64_xenon_be.sla): the PowerPC registers named, XER
    emulate/pcode.py                 lifter(mode) with the specification each mode reads (SLA)
    emulate/emulator.py              trace_function(shadow=): stepped per instruction (not fast), told of
                                     each import or callback answered and of what the answer wrote
    emulate/powerpc.py, driver.py    run(shadow=): the same
    lanes/win32.py explore_vectors   inputs: the argument dwords (stack, or ECX/EDX for a member function),
                                     the scratch memory and the module's writable sections, stub_ret;
                                     the seeds' paths known from a branch-logged run each (one per path
                                     run again with a shadow); the guessing explorer's GUIDED_REST where
                                     the paths are not all. PIPE_REV 48
    lanes/powerpc.py explore_vectors inputs: r3-r10, the memory behind the pointers (a page an input puts
                                     bytes in is given whole, the fill under it) and the image's writable
                                     sections, the calls' result; solver limit a quarter of the 16-bit
                                     lane's (64-bit questions cost four times as long to give up on). REV 5
    lanes/driver.py explore_vectors  new: the driver lane had only its fixed inputs. Inputs: EAX-EDI, the
                                     carry and zero flags, the stack words, the scratch memory, a service's
                                     EAX and carry; each seed run with a shadow (no branch log to know its
                                     path by). REV 4

A verdict's limits gain `paths=N` / `paths-all` as in the 16-bit lane; each lane's profile names
the lifter and the specification. CB_NO_CONCOLIC=1 (or no lifting pcodecheck) is the old search,
input for input.

**Evidence** (this machine, other sessions' work beside; "before" is the tree as merged, exported
and run from a copy; "after" this one; each module reconstructed and judged into a fresh function
store, CB_NO_FP_CACHE=1, serial (CB_WORKERS=1), the same counting script for both: a count of the
original's runs wraps the emulators' own runners).

The record, verdicts row for row (verdict, reason, limits, detail) -- the 16-bit lane with its
path judge on as before, the others with CB_NO_CONCOLIC=1 so their inputs are the old ones:

    module         lane     verdicts (before = after)                               original runs
    lzexpand.dll   ne       30 equivalent, 14 divergent, 2 no-compile                1278 = 1278
    ver.dll        ne       17 equivalent, 11 divergent, 3 no-compile                 587 = 587
    lz32.dll       pe       17 equivalent, 3 divergent, 19 no-compile                1625 = 1625
    splitter.vxd   le       6 equivalent
    PowerPC image  powerpc  45 equivalent, 21 divergent, 43 consistent, 2 no-compile 5334 = 5334
    choice.com     dos      22 equivalent

(The PowerPC image is tests/test_powerpc.py's cases linked with the SDK's libcmt: 111 functions.
keyboard.drv was in the list: its C does not compile at this tree, before or after -- E1009 at
sub_9_04B4 in 68 of 70 functions, the decompiler's -- so VER.DLL stands in for it.) The console
lanes' own judge tests (an edited cartridge: the edited function divergent, the rest equivalent)
pass on the record: test_nes, test_z80, test_m68k. tests/test_run_record.py checks the 32-bit and
16-bit memory digests made from the record against the lanes' old code, kept in the test, on
LZ32 (lean and plain) and LZEXPAND runs.

The path judge, before (guessing) -> after (path by path):

    module         seconds       original runs  of them exploring  paths  all paths  VERIFIED  weak     UNVERIFIED
    lz32.dll       9.1 -> 9.6    1625 -> 1383   670 -> 402         57     16 of 17   17 -> 17  0 -> 0   3 -> 3
    PowerPC image  60.1 -> 80.0  5334 -> 4096   3866 -> 2349       473    68 of 86   30 -> 30  58 -> 56 21 -> 23
    splitter.vxd   9.7 -> 11.4   (fixed inputs only before)        26     4 of 6     4 -> 4    2 -> 2   0 -> 0

    teeth          VERIFIED caught       VERIFIED (weak) caught
    lz32.dll       6 of 6 -> 6 of 6      --
    PowerPC image  9 of 14 -> 11 of 16   2 of 14 -> 7 of 16

PowerPC verdicts that moved: sub_93010BA0 (a number formatter) VERIFIED (weak) -> UNVERIFIED --
real: on an input the explorer found both return, the original having written 9 bytes and its C
4; sub_930125F0 (memset, its C with an unrecovered jump table) weak -> UNVERIFIED -- real: the C
faults where the original returns; sub_93016FE8 weak (fault-only) -> VERIFIED (an input it
returns from); sub_93010FB8 VERIFIED -> weak (an input found runs it to the step limit:
`truncated`). On the way, one loss was the judge's own and is fixed: sub_93016EA8's loop over a table
of globals (DIVERGENT before, through the guesser setting the table) came out equivalent with
paths-all -- the image's globals were not inputs, and a PowerPC address is a 64-bit register's low
word (`lis` sign-extends: the shadow looked for the inputs at 0xFFFFFFFF9302xxxx). Both fixed;
it is divergent again. The PowerPC's seconds are its solver: a C library's number parsing asks
questions z3 cannot settle (122-197 unknown of 256 in each of five functions), hence the lighter
limit; lz32's paths came at no cost.

**Not covered, and why.** The 6502, Z80, SM83 and 68000 lanes judge a program from fixed register
states and compare whole images; there is no input to solve for, and SLEIGH specifications for
the Z80, SM83 and 68000 are not in languages/ (the 6502's has no lift mode yet). DOS programs are
not run. The 16-bit gate keeps calling windows16.probe directly (it reads that record's own
fields). The driver lane runs each seed again with its shadow (its runs keep no branch log to
know their paths by): more runs than the Windows lanes' one-per-path. The explorer's runs are not
handed back to the 32-bit and PowerPC judges as fingerprints (the 16-bit judge's are): a shadow
run is one instruction at a time, and the judge's fingerprint run stays the fast one.

Tests: tests/test_run_record.py (5: the record's rules on made-up writes; a fingerprint from a
record; every platform's runner gives the record; the 32-bit and 16-bit memory digests as the
lanes' own code made them), tests/test_concolic.py (11, four new: a 32-bit run with its shadow is
the same run, on LZ32's exports; LZOpenFileA's paths all found; a PowerPC branch over an argument
and over a call's result solved, the same run with the shadow or without; the PowerPC lift mode's
registers and p-code). check_imports, test_verification (23), test_drivers (16), test_powerpc (17),
test_platforms (9), test_frozen (47), test_z80, test_m68k, test_nes pass; tidy reports only what
it reported before.

Left for after: the pcodecheck in the setting decompiler_bin needs rebuilding for ppc-be-64-lift
(`cmake --build native/decompiler/build --config Release --target pcodecheck`; until then the
PowerPC lane guesses as before, the x86 lanes explore). Handing the 32-bit explorer's runs back
as fingerprints would save about a quarter of lz32's original runs again.

## Built: phase B (2026-10-29)

The service is `python -m codebreaker serve` (codebreaker/ui/serve.py): one process per checkout
(per state folder) that the terminal interface, every MCP server and the command line's queries
ask. ui/service.py is its client and the query layer under all of them.

**What it is.** An HTTP+JSON server on 127.0.0.1, threaded, at a port fixed for the state folder
(the setting `service_port`, else 20000 + a CRC of the folder's path mod 20000; the next one up
when another program holds it), bound exclusively (SO_EXCLUSIVEADDRUSE on Windows, where two
sockets may otherwise share a port). Its record is state/service/service.json (pid, port, root,
state folder, started); its token state/service/token, a new secret each start, which every
request carries as X-CodeBreaker-Token (and the Host must be 127.0.0.1 or localhost); its log
state/service/serve.log. On exit it stops its jobs and takes the record and the token away.

    GET  /ping   -> {pid, port, root, state, started, idle_minutes, idle_for, calls, busy, jobs, stale}
    POST /call   {"name": N, "args": {...}, "owner": CLIENT}
                 -> {"value": ...} | {"error": text, "kind": ValueError | OSError | LookupError | ...}
    POST /stop   -> {"stopping": true}

A call's name is one of service.CALLS -- `functions`, `store.overview`, `store.latest`,
`jobs.all`, `jobs.running`, `jobs.stop_all`, `cli` (a query command run in the service, what it
prints sent back) -- or `mcp.<tool>`, any tool of the MCP server (mcp_server.TOOLS: the 29
reading tools, run_command, stop_job, name_function). The client re-raises the failure as its
kind, so a wrong module name is still a ValueError the MCP server says as a tool error.

**What it keeps between calls.** The databases open (facts._OPEN); each step's code digest
worked out once per process (Step.code -- in a long-lived process this is also the honest
version: the code that runs is the code loaded); binaries' SHA-256 by path, size and time
(facts.sha); the MCP server's slow answers (mcp_server.SLOW: the ledger, decompiles,
version comparisons, an hour); the jobs (service.JOBS: the ui/service JOBS role, moved into it --
each job still a `python -m codebreaker ...` process below normal priority, now the service's
child, with an owner: the client that started it).

**Lifetime.** A watchdog thread, every second: no call answered for `service_idle_minutes`
(default 20; `--idle MINUTES`) and no job running -> it exits (idle counts from the last job's
end). Every five seconds it compares the package's .py files (count, newest time, total size)
and settings.json's time with those at start; when they differ it is stale -- its answers would
be the old code's -- and it exits as soon as no job runs; a client that finds a stale one with no
job tells it to stop and starts a fresh one. A second `serve` cannot bind beside it: it finds the
one running (ping with the token, the same state folder) and says "already running"; two
starting at once, the one that loses the bind waits for the other's record. `serve --status`,
`serve --stop`.

**The clients.** `service.call(name, *args, **kw)` binds the arguments to the local function's
signature, then asks the service when there is one, else calls the function here -- the same
function either way, so the in-process path is the old one. Which:

    CB_SERVICE=0     always in process (the old path)
    CB_SERVICE=1     the service, started when not running
    unset            an interface (ui/terminal.py, ui/mcp_server.py: service.START) starts it on
                     demand -- in the background as it opens; anything else (`facts`, tests) uses
                     it when it runs and answers in process when not
    in the service   (service.LOCAL) everything answered there

A service not found is looked for again at most every five seconds; one that goes away mid-call
(connection refused or reset) is dropped, looked for (started, for an interface) once more, and
the call answered in process if that fails. It is started out of the client's console, process
group and (where allowed) job object, so an MCP server killed by its client does not take it
down; its stdout and stderr go to serve.log, never into an MCP server's protocol pipe.

- The terminal interface: Store forwards every mcp_server tool it uses as `mcp.<tool>`, the home
  screen's overview and a module's earlier verdicts as `store.overview`/`store.latest`; quitting
  stops only its own jobs (`service.stop_mine()`: jobs.stop_all for its owner).
- The MCP server stays an MCP stdio (or HTTP) server; each tool is registered as `_forward(fn)`,
  which sends it to the service (a job this process started itself, with no service then, stays
  here). Its job tools now see every interface's jobs; on exit it stops only its own.
- The command line: `facts` (service.QUERIES) asks the running service (`service.cli`) and prints
  its answer; with none it runs in process. It never starts one.
- What a call prints: the service's stdout is a Router -- inside `service.captured()` (the MCP
  server's _quiet, the `cli` call) what a thread prints goes to that thread's buffer, so
  concurrent calls never see each other's output.

Evidence (this machine, other sessions' work running beside; seconds, each client a fresh process,
a fresh state folder; "inside" is from the client's first import to its answer):

    decompile_function on version.dll   in process 3.95 (3.47 inside), again 3.14 (2.73)
                                        via the service: first client 3.12 (2.90), second 0.89 (0.68)
    xrefs on version.dll                in process 1.68 / 2.52; via the service 1.38 / 1.10
    facts --databases                   in process 1.45; via the service 1.30
    the service, cold start to its first answer: 1.40

The second client's decompile is the first's, kept in the service; in process every client
decompiles again. Most of what is left of a client's time is Python starting and importing.

Tests: tests/test_service.py (11, about a minute: in process with no service and with
CB_SERVICE=0; started on demand, its record and token; requests without the token refused;
queries answered by it equal to the in-process answer, failures carried over as their kind; a
job run in it with its owner, followed to its end; a second `serve` and `serve --status` finding
the first; `facts` through it, the same output as in process; the MCP server's tools forwarded;
the service gone -> answered in process; idle exit; stale exit when the package changes -- every
service started is stopped at the end); test_ui (10), test_mcp (13) and test_engine (12) pass.

**What phase B leaves.** The 16-bit build (`build --engine`) still runs the engine in the
process that asks (or as a job); running it inside the service, with progress streamed back, is
next once phase C has made more of the stages facts -- `facts --make` already runs the engine in
the service, its output sent when it is done. The function store is still opened per call (it
is the verdict table phase A left for later). The load setting between the service and other
sessions is still the open question below.

## Built: phase D (2026-10-29)

The lane interface is the Platform descriptor of codebreaker/platforms/, extended -- not a second
registry. Every platform has the same six slots, each a small object whose fields say what the
platform is and name (as a `Ref`: module and function, imported when first used) what does the
work:

    p.loader       container, open, readable, parts, entries, exports, imports
    p.cpu          the SLEIGH language id (p.cpu IS that string, as before), bits, endian,
                   decoder, registers, stack
    p.convention   names, arguments, word, order, results, pops, preserved
    p.emulator     run (the judge's runner), call (one export called: module.run), engine
    p.compiler     language, toolchain, tool, tools, flags, env, compile, library
    p.packager     link (the whole module), functions (the judge's build), writer (an
                   assembly lane's source writer + rebuild), tools

plus `p.steps`, the module of its steps on the engine, and `p.id`, the descriptor's file name
(win16), which `platforms.named` also answers to. A slot a platform has no use for is empty (a
DOS program is not run: its emulator's run is None). `platforms.machine(...)` describes an
assembly-language machine in one call (the six console and home-computer platforms).

**Which lanes are on it.** All thirteen platforms fill their slots: win16, win32, the drivers
(vxd), DOS, the Xbox 360, the NES, Master System, Game Boy, Genesis, ZX Spectrum, MSX, Java and the
original Xbox (loader and CPU only: every other stage gets its PE image). What asks them:

| Shared machinery | Asks the platform for | Was |
|---|---|---|
| engine/engine.py `load`, `steps` | `p.steps` | its own STEP_MODULES table |
| engine/win16.py | loader.parts (ne.segments), compiler.tool/options/env/compile/library, packager.link and tools (ne.image) | NEFile, watcom.ow_tool("wcc"), functions.CFLAGS16, ow_env("ne"), module._build16 |
| build/module.build | `p.steps_module().build` for a platform on the engine; packager.link; emulator.call | `engine16.handles`, `_build16 if kind == "ne" else _build32`, `_call`'s `if kind == "ne"` |
| verify/lanes/win16 (the judge) | emulator.run (every run, the branch tracer's too), convention.arguments and .preserved, packager.functions, loader.open, compiler.options (profile) | windows16.probe, ARG_WORDS = 8, SI/DI/DS written out, functions.build16, NEFile |
| verify/lanes/win32 | emulator.run, convention.arguments (ARITY), packager.functions, compiler.options | tracer.trace_function, the literal 4s, functions.build32, c_text.CFLAGS |
| verify/lanes/driver | emulator.run, loader.entries, packager.writer/functions/tools | emulate.driver.run, le.entry_points, driver_writer, build/driver |
| verify/lanes/dos | packager.writer and tools | dos_writer, build/driver.tool |
| verify/lanes/powerpc | emulator.run, convention.arguments, compiler.options, packager.functions | powerpc.run, ARGS = 8, CFLAGS_POWERPC, build_powerpc |
| verify/lanes/image_lane, z80_flat (six machines) | packager.writer, loader.open | each lane's writer name, the reader handed in |
| emulate.RUNNERS, reconstruct.KINDS, describe's languages, corpus._readable | emulator.run, lane, cpu, loader.readable | (since PLATFORM_DESCRIPTOR step 1) |

The per-lane branches that went: engine's STEP_MODULES, module.build's `engine16.handles` and
`kind == "ne"` choice of linker, module._call's `kind == "ne"`, the image lanes' writer argument,
z80_flat's reader argument. A new platform with a lane of the existing shapes now fills a
descriptor; its lane module gives only what is particular to judging it (its fingerprint).

**Left as they are, on purpose.** The 16-bit gate's branches in module.build (`b.kind == "ne"`:
the known verdicts, the gate, the whole-file image) are the gate's, not one of the six slots --
they become the win16 steps' once module.build is only the engine. decompile/decompiler.as_c,
compare/versions.load, coverage/ledger.extents and build/definitions still choose by kind inside
themselves (the decompiler and the comparer are other agents' work now; each is a loader- or
decompiler-shaped table to move next). The emulators' memory layouts (windows16.STACK_BASE ...)
are read from the emulator modules: they are the runner's, which the slot names.

Evidence (each module reconstructed and judged into a fresh function store, CB_NO_FP_CACHE=1 so
every original is run again, before and after the change, same machine):

    module         lane     profile digest        verdicts (before = after, row for row incl. reason and detail)
    lzexpand.dll   ne       775d76c5eed9 (same)   24 equivalent, 20 divergent, 2 no-compile
    lz32.dll       pe       5890b88354da (same)   17 equivalent, 3 divergent, 19 no-compile
    splitter.vxd   le       a66e119a9a64 (same)   6 equivalent
    choice.com     dos      3b24b406d543 (same)   22 equivalent

and the images: lzexpand.dll by module.build and by the engine b2113a90... before and after (the
12 export calls the same); lz32.dll's image differs between any two builds only in its link
time stamps (the PE header's and the directories'), before as after -- its 12 export calls the same.

Tests: tests/test_platforms.py (9: two new -- every platform fills the six slots and every Ref
resolves; the slots give what the lanes' own tables said), test_engine (12), test_verification
(21), test_drivers (16), test_dos (13), test_frozen (47), test_nes, test_z80, test_m68k pass.

## Built: phase E (2026-10-29)

The knowledge base is `codebreaker/knowledge/`: one SQLite file, state/db/knowledge.sqlite
(CB_KNOWLEDGE_DB), keyed by platform -- win16, win32, windows (both halves), vxd, dos, xbox360
(the lanes' names ne, pe, le, mz, powerpc, 16, 32 are taken too). It sits beside the program
databases but is not one of them: facts.hold never deletes it, since what was learned and given
is not made again.

**The store** (knowledge/store.py). A piece is (platform, kind, key, value), its value kept as
the program database keeps a fact's (engine/facts.py encode: bytes, text, JSON or a pickle, with
its SHA-1), with its source and its origin:

    authored   loaded from a file people write, kept where it is: the rows are its copy, loaded
               again when the file's hash changes (knowledge/sources.py lists them)
    derived    worked out from inputs outside the repository (the compiler's headers), made again
               when their stamp changes
    learned    found once by a step and kept: written once, seen by every later ask, any process
    given      given by hand (a body named)

A whole authored or derived kind is made in one go (`ensure(platform, kind, stamp, make)`):
the stamp it was made under is kept, and when it differs only its authored/derived rows are made
again -- learned and given rows stay. The name store's two tables, `body` (a body's digest per
module, with the name the module gives it) and `body_name` (a name given to a body), are here.

**The API** (knowledge/__init__.py):

    kb.prototype("win16", "KERNEL.GLOBALALLOC")   kb.declaration(...)   kb.spelling("GETDLGITEM")
    kb.argument_bytes("win16", "USER.@171")       kb.argument_bytes("win32", "FindWindowA", "USER32")
    kb.declared_argument_bytes("FindWindowA")     kb.import_name("USER", 91)
    kb.name(body_id)    kb.name_body(body_id, "Name")
    kb.signatures("win32")    kb.signature_matches("win32", code)
    kb.struct("win32", "RECT")    kb.api_model("win16", "GLOBALLOCK")    kb.api_model("vxd", ...)
    kb.table(platform, kind)    kb.known(...)    kb.learn(platform, kind, key, value, source)

    python -m codebreaker knowledge [platform [kind [key]]]     what it holds and where from (--load)

**What moved onto it.** Kept as the authored inputs, now read through it: ordinal_names.json
(windows ordinal_names), win32_arguments.json (win32 argument_bytes.header),
runtime_signatures.json (win32/win16 signature), driver_services.json (vxd device,
service_table, driver, message), dos_interrupts.json (dos interrupt), console_ordinals.json
(xbox360 ordinal_names: formats/xex_imports, module_writer, recompile/project). New authored
files, out of code: knowledge/models/win16.json (the 16-bit stand-ins of emulate/windows16.py
-- what each returns and removes, as data -- and KERNEL's numbers for them), models/win32.json
(emulate/emulator.py's documented argument counts), models/vxd.json (emulate/driver.py's
services that answer in another register). Derived: the headers' prototypes (win32/win16
prototype; symbols/prototypes.py) and structure layouts (win32 struct, struct.pointer;
symbols/sdk_structs.py), the XDK's library signatures (xbox360 library_signatures;
symbols/xdk_libraries.py). Learned: the bytes of arguments an export removes, read from its
code (win32 argument_bytes.code, win16 argument_bytes keyed by product and reading;
symbols/api_arguments.py). The bodies and body names (symbols/names.py through the function
store's body calls, which now go to the knowledge base -- the knowledge base beside a store
given a path of its own; an older store's bodies and hand-given names are carried over the
first time it is opened). describe.py fetches its import names and argument sizes through
the API; the emulator's 32-bit stand-in reads its out-parameters' types through kb.prototype.

Generated caches gone: state/prototypes.json, state/api16*.json, state/xdk/*.pickle (tidy-state
no longer knows them, so old copies go), and the corpus's apiarity/*.json is no longer written
(a file still there is read into the base once, per module). The `runtime_library.learn`,
`ordinals.collect` and `driver_services.generate` writers still write their JSON files, and the
base loads them again.

Evidence (each run a process of its own from an empty state folder, before = this branch
before the change): `names`, `identify`, the MCP triage and describe's description for
lz32.dll, lzexpand.dll and user.exe, and a table of direct asks (prototypes 16/32, spellings,
declarations, argument bytes 16/32, ordinal names and sources, four structure layouts, driver
services, a DOS interrupt) -- 13 files, all identical before and after (USER's description 3229
lines, its names 2689). `measure lzexpand.dll`, before and after: 46 functions -- VERIFIED 21, weak 3, IDENTIFIED 3,
UNVERIFIED 18 (exit-differs 8, deps-differ 7, ret-differs 3, writes-differ 2), DOES NOT COMPILE 1
-- the same counts and causes. The after state holds
knowledge.sqlite (prototypes 8292 + 1874, structures 8514, signatures 5233 + 3988, 1423 + 383
learned argument sizes, 1942 bodies) and none of prototypes.json, api16.*.json.

Tests: tests/test_knowledge.py (8, seconds: a kind made again only when its stamp changes and
learned pieces kept through it, values keep their shape, a name follows the body, an older
store's bodies carried over, the authored files read through it, the stand-ins as data, the size
rule leaves the base alone, platform names); test_symbols, test_engine, test_frozen,
test_verification, test_match_powerpc and test_invert pass.

**Left.** The per-binary databases do not yet refer to the base's pieces by (platform, kind, key)
as inputs of their facts (an engine step asks symbols/ as before, so a changed prototype does not
yet make the facts made from it stale by itself); that is the next step, with the service of
phase B owning the one open base. verify/lanes/win32's argc and c_text's argument sizes go
through api_arguments and so through the base, unchanged in their own code (other agents' areas).

## Built: every lane on the engine, one function store (2026-10-29)

**The lanes on the engine** (engine/lanes.py, behind phase A's switch: `CB_ENGINE=1`, or
`measure <module> --engine`, `build <module> --engine`). A lane is four calls (verify/reconstruct.py:
profile, prepare, build, judge); one set of steps, declared once per lane, makes reconstruct.run's
decompile, build and judge facts of the binary's database, for every lane at once -- 32-bit Windows
(pe), 16-bit Windows' judge (ne), the drivers (le), DOS, the Xbox 360 (powerpc), Java, and the six
console and home-computer machines:

    lane.K.profile          the lane's profile (digest, settings), asked every run   <- binary
    lane.K.module           the lane's prepare (the decompiled module)               <- binary
    lane.K.keys, lane.K.function <key>   each function's name and C (given by hand: engine.put)
    lane.K.texts            each function's name, signature and C as they now are, the header
    lane.K.build            the lane's build: context, items, why the rest are not in it, each
                            function's built signature (its code and what it reaches)
    lane.K.built <key>      what a verdict is reached for: the built signature (or, for a lane
                            that builds the module as one, the signature); or why it is not built
    lane.K.verdict <key>    the lane's judge in worker processes (batch; the build handed to it)
    lane.K.rows             the rows reconstruct.run records

and, for module.build of a 32-bit module, `pe.image` (the packager's link and module.check, made of
lane.pe.module and lane.pe.function). A verdict is made again exactly when the store would judge
the function again -- its built code (or, where a lane builds the whole module, its text) or the
lane's profile changed: a comment added to a function's C rebuilds the module and judges nothing.
reconstruct.run on the engine gives the store only the rows that differ from what it holds. Each
platform's descriptor names engine/lanes.py as its steps unless it has its own (win16: engine/win16.py,
whose module build and gate are phase A's). A Java program is a folder, not a binary: its database
is named by the lane's sha of its sources.

The versions: lane.K.module and lane.K.build are stamped with a digest of every .py of the
decompiling and building code (decompile, translate, symbols, analyze, formats, knowledge, source,
build, java, base, platforms ...), the lane's file, the native decompiler and its languages and the
CB_ switches; the judge's is the lane's own profile, as the store has always filed verdicts (a change
to the judge's code that leaves PIPE_REV/REV as it was does not judge again, on either path).

Evidence (each run a process of its own, CB_NO_FP_CACHE=1, CB_LOW_LOAD=4, the old path and the engine
each into a store of its own, the engine first cold -- its database removed --, then warm, then with
one function's C touched by a comment by hand, then forgotten; rows compared row for row including
reason, limits, detail and signature):

    module          lane     profile        old path: verdicts                                       engine = old   warm          touched (a comment) / back
    lz32.dll        pe       5890b88354da   17 equivalent, 3 divergent, 19 no-compile   (9.4s)        yes (8.9s)     0 judged 0.7s  0 judged, 2 rows new / the same
    lzexpand.dll    ne       4a267afaeca1   31 equivalent, 13 divergent, 2 no-compile   (21.8s)       yes (29.8s)    0 judged 1.1s  0 judged, 2 rows new / the same
    splitter.vxd    le       a66e119a9a64   6 equivalent                                (8.4s)        yes (9.6s)     0 judged 0.7s  6 judged (one module) / the same
    choice.com      dos      3b24b406d543   22 equivalent                               (0.9s)        yes (1.1s)     0 judged 0.8s  22 judged / the same
    bootanim .exe   powerpc  24af7b540c5b   237 equivalent, 54 consistent, 234 divergent, 29 no-compile (275s)  yes (204s)  0 judged 0.7s  46 judged, 508 kept (7.1s) / the same
    HelloWorld.sms  sms      c4cd1270b7a4   3 equivalent                                (0.9s)        yes (1.0s)     0 judged 0.8s  3 judged / the same
    tests' java     java     dd3bf79fff2d   3 equivalent, 6 inconclusive                (17.6s)       yes (17.3s)    0 judged 0.9s  1 judged, 8 kept (6.2s) / the same

"engine = old": every row of the store, verdict, reason, limits, detail and signature, the same.
The first lzexpand.dll run differed in one function (LZCOPY: equivalent / writes-differ) while the
engine judged in key order; the 16-bit judge's answer depends on the order the functions are shared
out over the workers, so the verdicts are now judged in the module's order, as reconstruct.run
judges them, and the rows came out the same. "Touched" is a comment added by hand to one function's
C (engine.put of lane.K.function): where a lane builds per function (pe, ne, powerpc, java) the
module is built again and only the functions whose built code changed are judged; where the module
is built as one (le, dos, the consoles) every function is, as reconstruct.run would.

The 32-bit module build (module.build lz32.dll / version.dll, the old path and CB_ENGINE=1, the
engine cold, warm, touched and back): every image the same as the old path's but for the five
fields that differ between any two builds by the old path too (the PE header's time stamp, the
resource directories' three, the version resource's file date); the report the same; the 12
export calls the same (lz32: 11 alike with the original's, version.dll: 10). Warm: pe.image kept,
0.3-0.4s; a comment by hand: only pe.image made again (the link, 0.5s).

**One function store.** store/functions.py keeps its API (the UI, the MCP server, the ledger and the
module writer read it), and a module's tables -- function, candidate, verdict, text, identified,
built, variant -- are now in that binary's program database (state/db/<sha>.sqlite, the engine's
file): the verdict table is the database's. The store's own file (state/functions.sqlite) is their
index: the modules (sha, name, path, kind), the profiles and a summary per module (functions,
verdicts, the latest class of each function, the profiles judged under) kept as each module is
written, so what is asked across the corpus (overview, the ledger's stamps, progress) reads one
file. The callers that read the store's tables by SQL across modules (the ledger, progress, the MCP
server's function and name_function, the service's stored_path, module_writer, tools/causes) ask
the store instead (module_rows, module_path, stamps, latest_profile, recent, verdicts_since, stored,
modules_with_body). A store given a path of its own keeps its databases in `db` beside it.

Carried over: the first time a store from before is opened, every module's rows are copied into
its database, each verdict marked `carried` = 1 (a verdict reached since is 0); rows an older
checkout writes into the old tables afterwards are carried on the next open (by their time). The
old tables are left where they were, unread. On a copy of this machine's store (522 MB):

    rows in the old tables      1262 modules, 219,177 functions, 212,960 candidates, 219,325 verdicts,
                                15,025 identified, 169,598 built signatures, 649 variants, 1262 texts
    carried                     every one of them (219,325 verdicts marked carried), 35s, once
    the next open               0.5s, nothing carried again
    overview()                  0.02s from the index (the same 1262 rows as the old store's overview)
    old readers = new readers   overview of all 1262 modules; scores, candidates and built of 169
                                (module, profile) pairs over 120 modules chosen at random; latest,
                                identified, addresses, the header text; modules(profile) for each of
                                the 51 profiles -- all the same

The databases' cap (facts.hold) no longer deletes a database that holds verdicts: its facts and
blobs go, the store's rows stay.

Tests: tests/test_store.py (new, 3: a module's rows in its database and the index across modules,
read-only, an older store carried over once and then only what was written since, with its
variant parents and verdicts marked carried; the cap leaves the verdicts); test_engine (13: one new
-- a made-up lane on the lanes' steps: made once then kept, a comment by hand judges nothing, a
change to one function's code judges that one, one that does not compile is not run, a new profile
with the build's files gone builds again there and judges all); test_platforms (every lane
platform's steps named); test_ui, test_mcp, test_service, test_plumbing, test_frozen, test_verification, test_whole_module, test_drivers, test_dos, test_powerpc, test_knowledge, test_symbols, test_analysis and tests/java/test_kind pass; tools/check_imports passes, tools/tidy reports nothing in these files (its findings are in c_text, describe, windows16 and translate/backends/c.py, other work).

**Left.** The inner caches of the stages (state/cache: the compiler's objects in build/functions,
masm, msc6, the period compiler's in verify/authored/match, origin16, and the 16-bit gate's and the
decompiler's, which are other work now) are still base/cache.py: the lanes' steps are the outer
layer, as phase A's were, and each goes as its stage becomes steps of its own (the compiler's
objects are per text, not per binary: they need a compile step the build asks, as engine/win16.py's
function.object). The engine path is behind its switch; the old reconstruct.run and module.build
paths stay the default until the switch is turned on for all.

## Decisions, and what is left open

- Python stays the language of the engine and the steps; the native decompiler stays a step. The
  Rust core of CORE_ROADMAP Track 2 can later replace steps one at a time, behind the same facts.
- SQLite, not a server database: one file per binary is easy to delete, copy and inspect.
- Open: how a fact made by a period compiler (DOSBox-X) is keyed (the kit's files' hashes); how the
  service and other sessions share a machine (the load setting). (Decided: verdicts from before the
  database are carried over, marked carried -- "Built: every lane on the engine" above.)
