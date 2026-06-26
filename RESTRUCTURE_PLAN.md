# Converter Restructure Plan

Restructuring the 10,100-line monolith `java2xbox360.py` into a modular package
(`j2x/`) that (1) is maintainable, (2) **reports everything it does**, and
(3) enforces the console-crash rule catalog below. The legacy converter stays
runnable and authoritative until each module that replaces it is proven to
produce byte-identical output. Backup: `java2xbox360.py.bak`.

## Why

- **168 post-hoc `fix_*` patch functions** operate on already-generated text,
  are order-dependent, overlap, and occasionally fight each other. The converter
  emits wrong text then string-patches symptoms — every new file grows the file.
- **No semantic model.** Value-vs-pointer (the #1 error class) is decided by
  heuristics and repaired afterward. It should be decided once, at emit time,
  from a resolved type.
- **No visibility.** What it ported / stubbed / guessed lived only in a 900 KB
  build log and scattered `// PORT:` comments. Fixed by the ledger (Phase 1).
- **166 hand-written overrides (72% of files)** win over generated output —
  a parallel codebase and the source of the "manually fix then back-port" grind.

## Target architecture

```
j2x/
  ledger.py     Event / Category / Severity / Ledger          [DONE]
  analyze.py    post-hoc analyzers -> ledger (Phase 1)         [DONE]
  rules.py      structural crash-class detectors               [DONE, growing]
  report.py     index.md / <File>.md / report.json             [DONE]
  cli.py        `python -m j2x report ...`                     [DONE]
  profiles/     VersionProfile + JSON loading                  [Phase 2]
  parser/       Java source -> IR                              [Phase 2]
  model/        symbol table: name -> type + ownership + static [Phase 3] *key*
  emit/         IR -> C++ (reads model; correct . vs -> first time) [Phase 3]
  pipeline.py   orchestration (replaces run_conversion/main)   [Phase 2]
```

## The reporting ledger (Phase 1 — DONE)

Every action becomes an `Event(file, line, category, severity, source, rule, message, snippet)`.

Categories (worst first): `ERROR` (compiler said no) · `GUESSED` (heuristic call,
likely latent bug) · `STUBBED` (placeholder) · `BLOCKED` · `RISK` (known
crash/leak) · `WARNING` · `OVERRIDDEN` · `ANNOTATED` · `PORTED` · `RULE`.

Outputs under `<output_dir>/report/`: `index.md` (totals + **risk-ranked file
list** + back-port-debt), `files/<File>.md` (per-file ledger), `report.json`.

Run: `python -m j2x report output_alpha --profile alpha`
Optional: `--pristine <dir>` (no-override regen) to classify each override as
**retire** (byte-identical to generated) vs **back-port** (real debt).

Phase-1 analyzers are NON-INVASIVE (read artifacts only): `marker` scan, `rule`
checks, `lint` (converter_lint), `build` log parse, `override` inventory.
Later phases move event production INTO the generator/rules so PORTED/GUESSED
decisions are captured at the moment they're made.

## Crash-class rule catalog (the bugs that cost hours on hardware)

The console hard-crashes (IP 0x0 / 0xC0000018) where Java tolerates the same code.
Each rule is named, self-describing, individually testable.

| Rule | Detects | Bug it maps to | Status |
|---|---|---|---|
| `uninit-array` | `new T[n]` without `()` | `new bool[10]` auto-jump; uninit `s_isBlockContainer` | DONE |
| `ctor-stub` | empty / delegation-only ctor body | Block blockID=0; BlockContainer flag unset | DONE |
| `rule-of-three` | owns heap, missing dtor / copy control | intermittent gen-phase heap corruption | DONE |
| `value-copy-owner` | heavy/owning type by value | Tesselator MB-buffer copy | DONE |
| `null-deref` | unguarded deref of possibly-NULL ptr | blocksList[id]->, tile-entity, NULL texture | converter_lint (broaden) |
| `label-goto` | labeled-break goto -> infinite loop | shore-water freeze | converter_lint |
| `render-state-stub` | every D3D9/texture stub surfaced loudly | ZENABLE, blend modes, color key, %blur% strip, alpha-ref | via marker scan (tag better) |

Not statically catchable (flag as `GUESSED`, never silent): wrong-list/merge
semantics (getCollidingBoundingBoxes terrain-collision bug).

## Phases

1. **Reporting overlay** — ledger + analyzers + rules + report, zero behavior
   change. **DONE.** Gives per-file visibility + risk ranking today.
2. **Modular split** — carve profiles/parser/pipeline out of the monolith behind
   a stable entry point; verify output byte-identical vs `java2xbox360.py.bak`.
3. **Type model** — real symbol table; emit decides value/pointer, `s_`/`m_`,
   includes correctly the first time. Retire the bulk of the 168 `fix_*` passes.
   Move event emission into emit/rules (live PORTED/GUESSED capture).
4. **Override coverage** — drive the 166 overrides down: retire the redundant
   ones, back-port the rest (the report's debt list is the work queue).

## Verification

- Golden tests per rule (input snippet -> expected events): millisecond regression
  check instead of the 27-min full rebuild.
- Output-identity check: regen vs `.bak`; diff the tree. No-op diff = safe refactor.
- Syntax harness `syntaxcheck.ps1` for real `cl.exe` compile status.
