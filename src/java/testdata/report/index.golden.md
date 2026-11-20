# Conversion Report

- **Profile:** test profile
- **Output Dir:** out/dir

## Totals

| Severity | Count |
|---|---|
| 🔴 HIGH | 3 |
| 🟡 MED  | 1 |
| ⚪ LOW  | 2 |
| **Total events** | **6** |

| Category | Count | | What it means |
|---|---:|---|---|
| `ERROR` | 1 | ███··············· | compiler rejected it (from the build log) |
| `GUESSED` | 1 | ███··············· | converter made a heuristic call that may be wrong -- INSPECT |
| `STUBBED` | 1 | ███··············· | placeholder; needs a hand implementation |
| `RISK` | 1 | ███··············· | matches a known console-crash / leak pattern |
| `ANNOTATED` | 1 | ███··············· | a note about a decision already taken |
| `PORTED` | 1 | ███··············· | translated literally and cleanly |

## Files by risk (inspect top-down)

3 files have findings; 3 carry HIGH-severity risk.

| Risk | File | 🔴 | 🟡 | guess | stub | risk | err |
|---:|---|---:|---:|---:|---:|---:|---:|
| 7 | [Block.cpp](files/Block.cpp.md) | 1 | 1 | 1 | 0 | 1 | 0 |
| 5 | [World.cpp](files/World.cpp.md) | 1 | 0 | 0 | 1 | 0 | 0 |
| 5 | [sub/Item.h](files/sub_Item.h.md) | 1 | 0 | 0 | 0 | 0 | 1 |

## Legend

Sources: `marker` (PORT/STUB/TODO comment), `rule` (structural crash-class check), `lint` (known crash/leak pattern), `build` (compiler error).
