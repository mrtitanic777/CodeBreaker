# CodeBreaker Diff — cross-version analysis & signature propagation

Maps function identity across the Windows lineage by abstracting each lifted
function into a **canonical fingerprint** that survives the 16-bit-real →
16-bit-protected → 32-bit-flat architecture jump, then matches and propagates
names across releases. Lives in the shared engine (it is target-agnostic) and
runs as `wt diff …`.

## Why fingerprints, not bytes

The same function across releases has different machine code (operand size,
register allocation, segment overrides, rebased offsets) but the same *shape*.
Three levels, weighted CFG 30% / opcode 40% / calls 30%:

- **L1 CFG topology** — basic blocks split on the lifter's own terminators
  (jcc/jmp/loop/jcxz/ret), edges from branch targets; node/edge counts,
  cyclomatic complexity (`E − N + 2P`), and a hash of the block-adjacency listing.
- **L2 normalized opcode stream** — every instruction with registers mapped to
  *roles* (`eax/ax/al → R_ACC`), immediates → `IMM`, memory → `[MEM]`, and the
  interchangeable mnemonics classed (`test/cmp → CANON_CMP`, the jcc family →
  `CANON_JCC`, `mov/lea/movzx → CANON_MOV`). This is the cross-era invariant.
- **L3 call vector** — external call targets in order, hashed.

Fingerprints are derived by **reusing `codebreaker/ir/il.parse_asm`** (so the db-escape,
macro headers, and mis-carved `ret` bytes are handled exactly as the real lifter
handles them). The engine's edge over Ghidra/IDA holds: the asm provably
re-assembles to the shipped bytes, so these features describe ground truth.

## Data

One row per `<ver>/asm/[group/]<module>/<seg>_<off>.asm` function file. Versions
that exist as reconstructed corpora — the real cross-version axis:

| version_id  | corpus            | architecture   |
|-------------|-------------------|----------------|
| `win101`    | `win1.0/asm`      | x86_16_real    |
| `win310`    | `win3.1/asm`      | x86_16_prot    |
| `win95_gold`| `win95/src/asm`   | x86_32_flat    |

(The blueprint's 2.x / 3.0 / 3.11 / OSR2 have no reconstructed corpus yet; add a
row to `corpus.VERSIONS` when they do.)

## Commands

```
wt diff versions                              # registered snapshots (OK = corpus present)
wt diff init                                  # create/upgrade corpus.db
wt diff ingest win95_gold [--limit N] [--modules a,b] [--force]   # resumable
wt diff enrich win310                         # real export names from NE tables -> corpus
wt diff stats                                 # per-version rows + coverage
wt diff match win101 win310 [--threshold T]   # score + store best target per source
wt diff census win101 win310                  # similarity report from stored matches
wt diff propagate win101 win310 [--min 0.85] [--apply]   # names A→B (dry-run default)
wt diff ui [--port 8790] [--no-open]          # git-style visual interface (browser)
```

Typical flow: `ingest` each version → `enrich` (names) → `ui` to browse, or
`match`/`census`/`propagate` on the command line.

## The UI (`wt diff ui`)

A git client for Windows' own history: releases are commits (a commit rail with
per-release function/named counts), modules are folders, functions are files.
Pick a function to see its **lineage** — its best counterpart in every other
release, with similarity score and match type — then click a lineage row for a
**git-style side-by-side diff** of the two normalized instruction streams
(red = only in the older, green = only in the newer). Stdlib server, self-contained
page. The diff view re-locates a function by module+filename if the corpus tree
was reorganised after ingest (so a renamed group dir doesn't break it).

`corpus.db` (codebreaker/engines/diff/, gitignored) is regenerable; ingest is idempotent.

## Thresholds — read before trusting a census

- A full **CFG + opcode** agreement scores **0.70** and is a strong cross-era
  match. Call vectors rarely agree across eras (ordinals get renamed), so demand
  0.70, not the naive 0.75, when comparing different architectures.
- **CFG-topology-alone (0.30)** collides for trivial/linear shapes — every
  one-block function hashes alike. Treat < 0.70 as a lead, not a match. The
  matcher skips functions under `min_insns` for the same reason.

## Status / next

- **Phase 1 (fingerprint + DB + ingest)**, **Phase 2 (matcher + census)**, and
  **Phase 3 (enrich + propagate)**: built and validated on the full 16-bit corpora
  (win101 3,633 fns / win310 18,004 fns). `enrich` named 3,747 win310 functions
  from their NE export tables; `propagate win101→win310` moved real recovered names
  (e.g. `PRINTPROC`) onto structurally-identical unnamed 3.1 functions.
- **The GUI** (`wt diff ui`) is built — commit rail, function browser, lineage,
  side-by-side normalized diff.
- **Key finding:** across 1.0→3.1 the *named exports* (window/dialog procs) were
  almost entirely **rewritten** — they do not match structurally — while the
  survivors are unnamed internal/compiler helpers. So cross-era name propagation
  is inherently sparse; that sparsity is itself the "structural evolution rate"
  the engine measures. The propagator guards against ambiguity (a generic
  skeleton matching several named sources is skipped, not guessed).
- **Next:** win95 PE-export enrichment (32-bit names), win1.0 bundle-system-module
  enrichment (via `idord`), and writing accepted propagations back into the golden
  C headers. Re-`ingest` after any corpus reorganisation (src paths are captured at
  ingest; the UI tolerates group renames, matching/stats want fresh rows).
