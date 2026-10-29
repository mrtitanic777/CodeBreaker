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
   starting it on demand.
C. **L2 only** (3): the decompiler's output is the L2 fact; c_text's remaining passes become steps
   on it (IR_UNIFICATION step 3 continues); C is the back end's.
D. **Lanes on the interface** (4): win16 first (it is on the engine since A), then win32, drivers,
   DOS, PowerPC, the console lanes.
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

## Decisions, and what is left open

- Python stays the language of the engine and the steps; the native decompiler stays a step. The
  Rust core of CORE_ROADMAP Track 2 can later replace steps one at a time, behind the same facts.
- SQLite, not a server database: one file per binary is easy to delete, copy and inspect.
- Open: how a fact made by a period compiler (DOSBox-X) is keyed (the kit's files' hashes); how the
  service and other sessions share a machine (the load setting); whether verdicts from before the
  database are carried over or the store starts fresh.
