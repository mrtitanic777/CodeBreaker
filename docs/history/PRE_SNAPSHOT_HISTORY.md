# Pre-snapshot git history

On **2026-09-11** the CodeBreaker git history was re-initialized from a clean slate, taking the
then-current working tree as a fresh snapshot point (root commit `fa46252`). This file records
what the **previous** history held, so the scale of the work that led to this snapshot isn't lost
when the old commit graph is retired.

## Summary

| | |
|---|---|
| First commit | `409e4ef` — *"Initial commit: CodeBreaker — deterministic reverse-engineering workbench"* (2026-08-07) |
| Last commit | `1ca714e` — *"paths: relocate oracle/legacy ISOs + oracle tree out of binaries; fix win95 dangling refs"* (2026-09-11) |
| Span | ~5 weeks (2026-08-07 → 2026-09-11) |
| Commits on `master` | **404** (all branch work landed on master) |
| Branches | **13** |
| Tags | 1 (`v0.1.0`) |
| Authors | mrtitanic777 (402), Logan Greer (2) |
| Commit cadence | 51 commits in Aug 2026, 353 in Sep 2026 |
| Cumulative change (first → last on master) | **2,370 files changed, 333,528 insertions(+), 5,262 deletions(-)** |

## Branches (at the time of the snapshot)

Each was a development line that merged back into `master`; commit counts are the total reachable
from each branch tip (they overlap heavily with master's shared history).

| Branch | Commits | Last | Tip | Tip subject |
|---|---:|---|---|---|
| `master` | 404 | 2026-09-11 | `1ca714e` | paths: relocate oracle/legacy ISOs + oracle tree out of binaries; fix win95 dangling refs |
| `win95/ne-resxref` | 400 | 2026-09-11 | `02ec3bb` | docs: document resxref16 (16-bit resource↔code correlation, wave 13) |
| `win95/ne-callback-sig` | 398 | 2026-09-11 | `080acf0` | docs: document 16-bit callback signatures, datainit16, WM_ dispatch (wave 12) |
| `win95/ne-code-names` | 394 | 2026-09-11 | `f69f7c3` | docs: document 16-bit names/args/data/proto engines (wave 11) |
| `win95/ne-code-cascade` | 389 | 2026-09-11 | `5e7e7ef` | docs: document the 16-bit code cascade (interproc16 + whole-module coverage) |
| `win95/ne-resources` | 384 | 2026-09-11 | `aaf4f31` | docs: document NE resources, VxD DDB, thunk32, VBX, installer (wave 9) |
| `win95/win16-frontier` | 378 | 2026-09-11 | `c050abd` | docs: document the 16-bit / driver frontier engines (wave 8) |
| `win95/mfc-engines` | 372 | 2026-09-11 | `26082ef` | docs: document the MFC / C++ EH / navigation engines (wave 7) |
| `win95/vxd-le-bridge` | 356 | 2026-09-11 | `4199d3f` | docs: document the C++/advanced engines (demangle, rtti, structsynth, globalname, tailcall, strdispatch) |
| `l2/emit-cpp-v1` | 274 | 2026-09-10 | `bee07fc` | docs(win95): repoint WIN95_MAP reference to the migrated architecture doc |
| `wip/on-restructure` | 239 | 2026-09-10 | `6b10b05` | wip: rebase pre-restructure work onto the codebreaker/ layout |
| `restructure/v2` | 237 | 2026-09-10 | `c49adaa` | restructure stage 6: docs for the codebreaker/ layout |
| `behavior/flight-recorder-v1` | 225 | 2026-09-10 | `9238778` | behavior: stub-return from bogus indirect-call landings (user32 fall-throughs) |

## What those 404 commits built

In ~5 weeks the tool went from an initial deterministic RE workbench to a full reconstruction
engine: ~58 reconstruction engines across ~13 waves (32-bit PE + 16-bit NE + VxD lanes), the
behavioral reconstruction stack on the Unicorn oracle, the origin-triage cascade, the full
`engine/` → installable `codebreaker/` package restructure, and the PyInstaller-built
`CodeBreaker.exe`. The final stretch (this session) relocated all bulk data (corpus, oracle ISO,
reference SDK/DDK, VMs, period toolchains) out of the tool and into its proper homes, leaving the
repo as a clean code-only standard of measure — the state captured by root commit `fa46252`.

*The old commit graph still exists on the `origin` remote until it is overwritten; this file is
the durable in-repo record of its scale.*
