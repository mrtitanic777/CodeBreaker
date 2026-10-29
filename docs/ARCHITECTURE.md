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

## Decisions, and what is left open

- Python stays the language of the engine and the steps; the native decompiler stays a step. The
  Rust core of CORE_ROADMAP Track 2 can later replace steps one at a time, behind the same facts.
- SQLite, not a server database: one file per binary is easy to delete, copy and inspect.
- Open: how a fact made by a period compiler (DOSBox-X) is keyed (the kit's files' hashes); how the
  service and other sessions share a machine (the load setting); whether verdicts from before the
  database are carried over or the store starts fresh.
