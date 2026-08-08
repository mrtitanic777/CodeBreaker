# Win9x-inert tools on NT 4.0 — status

The CodeBreaker toolset was ported wholesale from win95/win98. Several tools
encode Win9x-only concepts (VxD/LE/VMM executables, spanning-CAB installs, the
real-mode boot chain, 16-bit-only paths) that **do not exist on NT 4.0** — a
pure-Win32/PE OS installed from an `expand`-compressed `I386\` tree. This file
is the honest record of what was pruned and what was left, and why.

## Atticked — physically moved to `_attic_win9x/`
Definitively inert and **import-closed** (nothing in the live toolset imports
them). Removed from the `winnt4re` command surface (`BUILTINS`).

`cab.py`, `cabwrite.py`, `vmmsvc.py`, `disc_build.py`, `bootchain.py`,
`isowrite.py`, `w3.py`

## Marked inert, left in place
LE/VxD tools that are conceptually inert on NT 4.0 but are **lazy-imported by
load-bearing survivors** (`ledger.py`, `buildmod.py`, `linkfile.py`,
`explore_model.py`) on code paths that never fire on an all-PE corpus. Physically
removing them would risk breaking those tools' imports, so they carry an
`INERT ON NT 4.0` banner instead and are not offered as commands.

`le.py`, `leloader.py`, `le_seg.py`

## Kept as-is — may serve NT 4.0's 16-bit (NE) tail
NT 4.0 media carries **195 NE (16-bit) modules** — the WOW (Win16-on-Win32)
support and 16-bit setup/tools. These 16-bit utilities could legitimately serve
that tail, so they were **not** pruned:

`cc16.py` (16-bit compile), `disasm16.py` (16-bit disasm), `lift.py` (NE→C),
`asmrecon.py` (shared reconstruction primitives; also the base for
`asmrecon32.py`).

## Deferred
Fully decoupling `le`/`leloader`/`le_seg` from the survivors (making the lazy
imports optional so the files can be removed too) is left for later — and may be
mooted by the in-progress unification of CodeBreaker into a single
multi-target (`--target`) workbench, which would supersede this per-project
snapshot entirely. See [`../PLAN.md`](../PLAN.md) Phase 1.
