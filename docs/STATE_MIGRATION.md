# State migration: `codebreaker migrate-state`

codebreaker.exe is being switched command by command from the Python (translated) to the native program (`native/cb`).
The two keep some of their state in forms of their own, so a state folder the Python filled looks half empty to a
native command. `codebreaker migrate-state` is the one-time move that carries across what is worth carrying. The
native side is a rewrite, not an imitation: it does not learn the Python's hashes or reprs, the older state is
converted once instead.

    codebreaker migrate-state --dry          say what would be carried; write nothing
    codebreaker migrate-state                carry it
    codebreaker migrate-state --state DIR    another state folder than the configured one

It leaves every old file in place, never deletes anything, never replaces a result or a row the native side already
holds, and can be run again (a second run finds nothing to do). Code: `native/cb/store/migrate_state.{hpp,cpp}`; the
command is registered by the integrator (`cb/app/native_commands.cpp`) with the verifier settings in force
(`Reconstructor::profiles()`).

## When to run it

Once per state folder, when the first native command that reads judged results or the knowledge base is switched
over -- and again after any further Python run that filed new results under the Python's digests (it is cheap: the
profiles and modules that have nothing new are passed over). A state folder made by the native program from the start
has nothing to migrate and the command says so.

## What is migrated

### (a) Judged results (the function store)

A verdict is filed under the digest of the verifier settings it was reached under (the lane's *profile*). The
Python makes the digest from a JSON or repr of its settings, the native lanes from their own text, so the same
settings have two digests and a native reader finds no verdicts under its own (`coverage`, `versions`, `reach`,
`reconstruct` all ask `best(module, digest)`).

The store keeps the settings text beside every digest (`profile` table of `functions.sqlite`). The migration reads
each text back (JSON object: the 32-bit and driver lanes; tuple repr: the 16-bit, PowerPC, DOS, cartridge and Java
lanes -- `codebreaker/verify/lanes/*.py`, `profile` / `settings_profile`) down to what makes a verifier the same
verifier, and compares it with the settings in force now for that kind of module:

| recorded in both | taken from the Python | taken from the native text |
|---|---|---|
| kind | `lane` / `kind` / first tuple item | the lane's name |
| judge revision | `pipeline` / `judge` / second tuple item | `rev N` / `judge=N` |
| comparer revision | `oracle` / `comparer` / third item (`ne`, `powerpc`) | `compare N` / `comparer=N` |
| path exploration | `paths` (ready flag and limits) | `paths ...` / `paths=...` |
| inputs (cartridge lanes) | the states, as numbers | `states [...]`, as numbers |
| tools (compiler, DOS tools) | `name:size:mtime` stamps, as name and size | `compiler ...` / `tools ...` |

A profile whose settings agree on everything recorded on both sides is the same verifier: its verdicts (and its
build signatures, `built`) are filed under the native digest as well, with their own times and signatures, so
"judged under the signature the function has now" means exactly what it meant. The file time of a tool is not
compared (the two sides count time from different epochs and it says only when a tool was installed); a tool that is
named on one side only is not compared either.

A profile that disagrees is left alone and reported with the reason (`judge revision 23, now 24`, `the older judge
did not explore paths, this one does`, `no verifier for le files is set up here`, ...). Its verdicts mean something
the judge in force does not, so they are not re-filed: the native judge makes them again when it next measures the
module. Nothing is guessed: no digest is mapped by position, by date or by "the newest of the kind".

The index (`profile` and `summary` tables of `functions.sqlite`) is told the native digest's settings and the module's
summary lists it. Where a function already has a verdict under the native digest (this program judged it), the older
verdict is not copied over it.

### (b) The knowledge base

The Python keeps it in `state/db/knowledge.sqlite` (values tagged B/S/J/P: bytes, text, JSON, pickle), the native
program in `state/db/knowledge.v2.sqlite` (its own value encoding). Same tables, different file. Moved:

* **bodies**: each module's `body` rows (a function's body digest, the name its module gives it) and `module` rows,
  for the modules the native knowledge base does not have bodies for yet (a module this program indexed itself is
  left as it is). The body digest is the same on both sides (SHA-1 of the length and the masked bytes, 16 hex digits),
  which is what lets a name given in one module hold in every module with that body.
* **names given to bodies** (`body_name`): moved unless this program has given a name to that body.
* **learned rows** whose keys mean the same on both sides: `win32 argument_bytes.code` (what an export of a 32-bit DLL
  removes from the stack, found from its code, keyed `DLL.name`).

Not moved, and why (printed by the command):

* *authored* rows (ordinal names, driver services, API models ...): the files are the knowledge; the native program
  loads them again when a file changes.
* *derived* rows (prototypes, structures, signatures from the headers): made again from the headers when their stamp
  changes; the Python's rows are tagged pickle in part and are not readable here.
* learned `win16 argument_bytes`: keyed by a digest of the Python source that reads the exports -- a table of another
  reading, not this one's.
* learned rows of a kind not listed above, and learned rows that are pickles or bytes.

### (c) Everything else was checked and needs nothing

| store | why |
|---|---|
| result cache `state/cache`, build cache | by design: native files are `<kind>/<generation>.cbc` in another format version and nothing older is read (`base/cache.hpp`). A cache the Python wrote is rebuilt; `codebreaker cache --prune` drops the old files. Not migrated: it is only time. |
| engine facts (`facts` tables of the program databases `state/db/<sha>.sqlite`) | step results memoised under the step's name and version; native steps have their own versions and make their results again. The verdict/candidate/built tables of the same files *are* the function store and are shared as they are (same schema, same engine). |
| function store index and module databases | the same schema and the same engine on both sides (`cbengine.dll` for the Python); only the profile digests differ, see (a). |
| `state/coverage_corpus.json` | same format on both sides (`path -> [size, mtime, code_bytes, kind]`). |
| reports (`state/teeth`, `state/reach`, `state/golden` ...), build outputs | outputs, not inputs; each command writes its own again. |

## Proof on a copy of the real state

`native/cb/store/migrate_state_proof_test.cpp` (run by hand, it does nothing without `CB_MIGRATE_PROOF_*` variables
naming a copy) reads a module the way the native commands do -- `store/versions_ledger` over the function store, the
name store over the knowledge base -- before and after the migration. On a copy of the main checkout's stores
(`functions.sqlite`, the program databases of `lz32.dll` and `lzexpand.dll`, `knowledge.sqlite`), with the native
32-bit profile built by the real `Win32Lane` (lifters and compiler of this machine) as the verifier in force:

| | Python, on the originals | native before | native after |
|---|---|---|---|
| lz32.dll judged | 17 verified, 3 unverified, 19 no-compile | nothing judged (39 no-source) | 17 verified, 3 unverified, 19 no-compile |
| lz32.dll bodies / named by itself / names from other modules | 15 / 8 / 0 | 0 / 0 / 0 | 15 / 8 / 0 |
| lzexpand.dll judged (its 16-bit results are of judge 20, the Python's own 16-bit digest holds none) | none judged (35 no-source) | none | none (35 no-source) |
| lzexpand.dll bodies / named by itself / names from other modules | 30 / 9 / 0 | 0 / 0 / 0 | 30 / 9 / 0 |

Of the 29 older profiles that hold results, one stood for the settings in force (the Python's current 32-bit digest,
`342fa094a98c` -> the native digest) and was carried (39 verdicts of 2 modules); 28 were left with their reasons (judge
revisions 19 to 47 against 48, and kinds with no verifier set up in that proof).
Knowledge base: 19,373 bodies of 25 modules and 2,915 learned rows (`win32 argument_bytes.code`) carried; 1,316 learned
win16 rows, 16,896 authored and 25,206 derived rows reported as left. A second run found nothing to carry.
