# Converter Rebuild Plan

Replacing the 10,100-line monolith `java2xbox360.py` (a generator + 168 post-hoc
`fix_*` patches) with a clean, modular converter in `j2x/` that emits **correct
C++03 by construction** — no postfix passes. The bugs that crashed the console
(uninit arrays, dropped ctor delegation, Rule-of-Three, value-copy of owners)
are all emit-time decisions; the new pipeline makes them once, correctly.

## Method (agreed)

Clean rewrite, **one module at a time**, never in a single pass:
1. Build the next module cleanly against its contract.
2. Write its tests; they must pass.
3. **All prior modules' tests must still pass** (no regressions).
4. `git commit` the module. Git is the record of what changed.
5. Iterate to the next module until the converter is fully functioning.

The legacy `java2xbox360.py` stays in the tree (and in git history) as a
reference oracle for translation behavior until the new pipeline matches/beats it.

## Pipeline & module build order

```
Java source ──▶ parse ──▶ IR ──▶ types ──▶ emit ──▶ C++03 ──▶ verify ──▶ report
                (front-end)   (resolve      (correct-by-          (crash-class
                              ownership)    construction)          rules = 0)
```

| # | Module | Contract | Status |
|---|---|---|---|
| 1 | `ir.py` | data shapes: JClass/Field/Method/JType (parse↔emit contract) | **building** |
| 2 | `parse/` | Java text → IR (no translation, just structure) | next |
| 3 | `types.py` | resolve each name → C++ type + ownership (value/ptr) + static | |
| 4 | `emit/` | IR + types → correct .h/.cpp (new T[n](), real delegation, R-of-3, refs) | |
| 5 | `pipeline.py` | discover → parse → resolve → emit → verify → report | |
| 6 | `verify` | run crash-class `rules.py` on new output; gate on zero HIGH | |

Reusable, already built (version-agnostic reporting layer):
`ledger.py` (Event/Category/Severity), `rules.py` (crash-class detectors),
`report.py` (index/per-file/json), `analyze.py` (post-hoc legacy-output analysis),
`cli.py`.

Profiles (version config) are built when `types`/`emit` reveal exactly what
config they need — not speculatively.

## Verification protocol

- **Per module:** `python -m unittest discover -s tests -t .` is green, including
  all earlier modules' tests.
- **End-to-end (once pipeline connects):** run the converter on the Java source,
  then `python -m j2x report <out>` — the crash-class rule counts (uninit-array,
  ctor-stub, rule-of-three, value-copy-owner) must trend to **zero**. They are the
  acceptance test: a correct generator produces no findings.
- **Compile truth:** `syntaxcheck.ps1` (real Xbox 360 `cl.exe`) on the new output.
- The legacy converter's output is the behavioral reference for diffing translations.

## Crash-class catalog the new emit must satisfy by construction

| Case | Correct emit | Legacy bug it prevents |
|---|---|---|
| primitive/pointer array | always `new T[n]()` | `new bool[10]` auto-jump; uninit `s_isBlockContainer` |
| constructor delegation | emit real `init()` call / inline target body | Block blockID=0; container flag unset |
| class owns heap | emit dtor + copy ctor + copy assign (or disable copy) | gen-phase heap corruption |
| heavy/owning type arg | pass `const T&` / `T&`, never by value | Tesselator MB-buffer copy |
| possibly-NULL deref | guard before `->` on list-indexed / TE / texture | blocksList[id]-> IP 0x0 |
| labeled break | `goto` that provably exits the loop | shore-water freeze |
| render-state stub | emit + loudly report every D3D9/texture stub | silent no-op render bugs |

(Not statically decidable, e.g. wrong-list/merge semantics → emit must flag as
`GUESSED`, never silent.)
