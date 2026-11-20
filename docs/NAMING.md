# Names, explained, and still proven

How CodeBreaker names the functions of a binary, where a name is kept, how it spreads from one module to
the next, what shows it, and how a name is *proposed* before it is accepted. This is track 7 of
[ROADMAP.md](ROADMAP.md).

Most functions of a stripped binary have no name (`sub_1_0116`). A name is worth having only if it is true
and does not move anything the judge decides, so a proposal is kept apart from the names until a person
accepts it, says where it came from and how sure it is, and is proven to leave the judge's input alone.

## 1. How names work today

### Where a function's name comes from

`cb::symbols::names::Namer::of_module` (`src/symbols/names.cpp`) gives every function the first of
these that has one, and says which (the second column of `codebreaker names <module>`):

| order | source | what it is |
|---|---|---|
| 1 | `export` | the module's own export table |
| 2 | `named body (...)` | a name given to this function's *body* by hand (`names --set`, MCP `name_function`), or by an accepted proposal |
| 3 | accepted proposal | an accepted proposal of a function too small to have a body (section 3) |
| 4 | `debug symbols (X.SYM)`, `ordinal table (...)`, `run-time type information`, `entry point` | what the module and its symbol files say themselves, used only where shown to be for this build |
| 5 | `runtime library: ...` | the library function its bytes are (`symbols/runtime_library`, `identify`) |
| 6 | `the same body in <module>` | the name another module gives the same body, when they agree |
| 7 | `(none)` | `sub_<address>`: a placeholder, not a name |

### The name store and how a name spreads

The same function is linked into many modules (the C runtime, helper libraries, the 16- and 32-bit halves
of a component). A function's **body** is recognised by a digest of its bytes with everything the linker
fills in left out (addresses inside the module, calls and jumps out of the function), so the digest is the
same wherever the code sits (`names::body_digest`, `bodies32`, `bodies16`). Bodies shorter than 24 bytes or
with fewer than six instructions of their own are everybody's and have no digest.

The knowledge base (`state/db/knowledge.v2.sqlite`, `src/knowledge/store.cpp`) keeps:

* `body(module, addr, hash, name, source)`: a function's body in one module, with the name the module itself
  gives it (an export, a symbol file);
* `body_name(hash, name, source, ts)`: a name given to a body. It is kept once and read back for every
  module that has the body (`Store::name_body`, `names_from_bodies`). Never dropped.

`store::BodyStore` (`src/store/bodies.hpp`) is the interface the readers use;
`store::KnowledgeBodies` implements it over the knowledge base. Naming a function
(`Namer::set_name(module, address, name, source)`) writes `body_name` for its digest, so *every module
holding that body has the name* the next time anyone asks.

### What reads names, and what does not

* `names` (command, MCP tool `names`), the terminal UI: list them.
* The source writer (`source/pipeline_module.cpp`, `Prepared::recovered`): after a module is reconstructed
  *and judged*, a `sub_...` placeholder in the written source is replaced by the recovered name, where that
  name is free (`module_writer::plan_names`).
* `compare/versions`: carries a name from one build to the next.
* **The judge does not read names.** `measure` and the verification inside `module` run on the module's
  bytes and the decompiler's output before any name is applied; the decompiler's own `Namer`
  (`decompile/environment.cpp`) is made without a body store, so a name given to a body cannot reach it. The
  verdict store is keyed by module hash and function address.

## 2. Proposed names

A **proposal** is a `store::Proposal` (`src/store/proposals.hpp`):

| field | |
|---|---|
| `module`, `key` | the module's content hash and the function store's key for the address (`1:0116`, `bfea1123`) |
| `name` | a valid identifier (letters, digits, `_`; `::` between names) no other function of the module has |
| `description` | one line: what the function does |
| `source` | `proposal` until accepted, then `accepted proposal (<rule>)` |
| `rule` | who made it: `import-wrapper`, `string-reference`, `assistant` |
| `confidence` | percent |
| `evidence` | one line of what it rests on (`only jumps to KERNEL32!CloseHandle`) |
| `digest` | the function's body, when it has one |
| `state` | `proposed`, `accepted`, `rejected` |

They live in the knowledge base as pieces `(names, proposal, "<module sha>/<key>")`
(`store::KnowledgeProposals`), **apart from `body_name`**: nothing that reads names sees a proposed name.
`names` shows an unnamed function as unnamed, with what is proposed beside it.

* **Accepting** a proposal promotes it: when the function has a body the name goes on the body
  (`Namer::set_name`, source `accepted proposal (<rule>)`), so it holds in every module with that body; a
  function too small to have a body keeps the name in its proposal record, which `of_module` reads (order 3
  above). The record stays as the provenance and the description. A name another function already has is
  refused.
* **Rejecting** keeps the record in the `rejected` state; the rules do not propose it again.
* Rules replace their own earlier proposals when they run again; they never overwrite an assistant's, an
  accepted one or a rejected one.
* Confidence below 80% (`proposals::kReviewBelow`) is shown for a person to accept; `--accept all` takes
  only 80% and over (or `--min-confidence`).

### Commands (`codebreaker names MODULE ...`)

| | |
|---|---|
| `--propose` | run the rules over the unnamed functions, store what they find, list it and count it |
| `--packets [--c] [--limit N]` | for each unnamed function, one JSON object a line: its address, body digest, size, calls (imports by `MODULE!name`, functions by their names), callers, strings, first 48 instructions, with `--c` its decompiled C, and its proposal if it has one |
| `--suggest ADDR NAME --describe TEXT --confidence N` | store an assistant's proposal (rule `assistant`) |
| `--proposals` | list proposed, accepted and rejected |
| `--accept ADDR` / `--accept all [--min-confidence N]` | promote to names |
| `--reject ADDR` | turn down |

MCP: `proposals(module, state?)` lists them, `propose_name(module, addr, name, description, confidence)`
stores one (not offered over `--public`), and `names` rows carry `proposed` for an unnamed function that has
one. The packet itself is what the reading tools already give (`disasm`, `xrefs`, `strings`,
`decompile_function`) in one call: `run_command names MODULE --packets --c`.

### The rules (no AI)

Rules propose only where the code says the name outright.

* **import-wrapper** (`analyze/function_evidence.cpp` finds the shape): the function's instructions are the
  frame's, moves of its arguments, one call or jump through an import, and the return. `<Import>_thunk`,
  90% when it does not touch its arguments (`jmp [import]`), 80% when it passes them on. The `_thunk`
  suffix keeps the name from being the import's own, which the written source would declare twice. Imports
  by number that no table names give no name.
* **string-reference**: a message the function uses opens with its own name (`LZInit: bad header`,
  `[ReadBlock] ...`, `WriteBlock() failed`), the name reads as a function's (a capital inside it, or an
  underscore) and not as a word, all such messages agree on one name, and the function does not call an
  import of that name. 50%: a message may name the function that failed, not the one that reports it.

Results are in section 4.

## 3. Proven to change nothing the judge sees

Three layers, each checked:

1. **By construction.** The judge's inputs are a module's bytes and the decompiler's output. The decompiler's
   `Namer` has no body store and no proposal store (`decompile/environment.cpp`); the verification runs
   before the writer applies names (`source/pipeline_module.cpp`); proposals never reach `body_name` until
   accepted.
2. **Unit test** `symbols.names.accepting_a_name_changes_what_names_shows_and_nothing_the_judge_reads`
   (`src/symbols/names_test.cpp`), on the real LZEXPAND.DLL and LZ32.DLL: propose, accept every
   proposal, and check that the body digests, the facts the analyses read of every function, and the
   decompiler's namer are identical before and after, while `names` changes for exactly the proposed
   functions and says `accepted proposal`.
3. **Measured.** `codebreaker measure` on a module before and after accepting its proposals: the same
   verdicts, function for function (section 4).

## 4. Results (2026-11-14)

### What the rules name

`codebreaker names MODULE --propose` over modules of the corpus (functions the module had no name for, and
how many got a proposal; every proposal here is `import-wrapper`; `string-reference` found none in these
modules, whose release builds keep no messages that open with a function's name):

| module | functions | no name | proposed |
|---|---:|---:|---:|
| lz32.dll (32-bit) | 39 | 10 | 5 (50%) |
| lzexpand.dll (16-bit) | 46 | 30 | 0 |
| comctl32.dll | 1102 | 967 | 11 |
| user32.dll | 602 | 71 | 40 (56%) |
| gdi32.dll | 865 | 493 | 37 |
| shell32.dll | 1897 | 1785 | 25 |
| ole32.dll | 2209 | 2046 | 88 |
| winmm.dll | 415 | 140 | 24 |
| version.dll | 44 | 24 | 19 (79%) |
| kernel32.dll, user.exe, krnl386.exe, gdi.exe (16-bit) | | | 0 |

The rules are safe and small: they name the thin wrappers around an import (`DisableThreadLibraryCalls_thunk`,
`SMapLS_IP_EBP_12_thunk`) and leave the 95% of the rest for an assistant, which reads `--packets` and answers
with `--suggest` (or MCP `propose_name`) for a person to accept. A 16-bit module's functions are not
wrappers (each starts with the Windows prolog and keeps its data segment), so none is named by the import
rule there.

### The judge's verdicts are the same

`measure` on a fresh state folder (`CB_STATE`, nothing cached: every function is judged) twice, the second
after `names --propose` and `names --accept all` on the same module (knowledge base `CB_KNOWLEDGE_DB`):

| module | functions judged | verdicts (equivalent / divergent / no-compile) | accepted | per-function rows of the program database |
|---|---:|---|---:|---|
| lz32.dll | 39, both runs | 17 / 3 / 19, both runs | 5 | 447 rows (function, candidate source and signature, verdict, reason, limits, detail): byte-identical |

The candidates' C is part of those rows: no accepted name appears in it, because the judge reconstructs and
judges before any name is applied (the writer renames `sub_...` afterwards, in the written source).

## 5. Phase 2: measured rules (2026-11-18)

### A ground truth, and `names MODULE --measure`

The functions a module itself names (its export table, ordinal table or debug symbols) are the ground truth:
`names MODULE --measure` hides those names from the rules (the evidence goes in, the name stays out), runs them,
and compares what they say with the truth. A proposal is *exact* when its name, without the rule's suffix
(`_thunk`), a leading underscore and a stdcall decoration, is the truth (case ignored), and *related* when one
contains the other. It prints recall (functions proposed / functions the module names), precision (related /
proposed), the rows per rule and each miss; `--rules basic` runs the phase 1 rules only, the baseline.
`Proposals::measure` (`symbols/proposals.cpp`) stores nothing.

### A new rule, and one that was measured and dropped

* **function-forward** (70%, shown for review): the function only passes its arguments on to one other
  function of the module (`FunctionEvidence::forwards_to_function`, read by `analyze/function_evidence`) and
  that function has a name (exported, debug symbol, accepted): `<Target>_thunk`. Because it needs the
  target's name, accepting names lets it reach the next link of a chain on the next `--propose`.
* **api-wrapper** ("a small function whose only call is one import is `<Import>_wrapper`") was built and
  measured first: on the exports of shell32, winmm, comctl32 and user32 it was wrong in 62 of 64 cases
  (stubs that only call `SetLastError`, functions that happen to call one API), so it is **not** in the rule
  set. The measurement is why: a rule that cannot be shown to be right is not shipped.

### Measured (the truth = the module's own names)

| module | names the module gives | phase 1 proposed | phase 2 proposed | related | precision |
|---|---:|---:|---:|---:|---|
| kernel32.dll | 506 | 0 | 13 | 10 | 77% (`GlobalCompact` is forwarded to `LocalCompact`, not named for it) |
| ole32.dll | 160 | 3 | 3 | 3 | 100% |
| user32.dll | 427 | 1 | 5 | 1 | 20% (import-wrapper 1 of 1; function-forward 0 of 4) |
| gdi32.dll | 274 | 0 | 2 | 1 | 50% |
| comctl32, winmm, shell32, lz32 | 125, 180, 76, 12 | 0 | 0 | | |

The numbers are small because the ground truth is the exported API: an exported function is rarely a
wrapper, and the functions the rules do name (internal thunks) have no truth to compare with. On the
modules' *unnamed* functions `--propose` is unchanged from section 4 (import-wrapper only), since the
targets of forwarding functions there are themselves unnamed: function-forward grows as names are accepted.

### Proven unchanged

`names --propose` and `--measure` write no name; the invariance test of section 3 and the 3 command cases of
`names` pass with the rule set extended (the judge's inputs are untouched by construction: the new evidence
field is read only by the rules).

### Left

* Parameter, local and struct-field names (from the SDK prototype a parameter is passed to) need the
  decompiler to carry a name per variable through the verdict-neutral rename; not started.
* Names across lanes: names are stored per body for the NE and PE lanes (Win16/32 and Xbox 360 PE);
  DOS, the 8- and 16-bit consoles and the other platforms have no body digest yet.
* Ground truth with private names (a module with its debug symbols, homebrew with published source) to
  measure the rules on internal functions, not only exports.
* Round trip of accepted names into Converted/ projects and a before-and-after build and verdict check on
  a whole project.
