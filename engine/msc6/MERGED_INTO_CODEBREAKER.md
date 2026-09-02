# w95recon has been merged into CodeBreaker

The byte-exact MSC-6.0a DLL-reconstruction capability of this tool now lives in the
CodeBreaker engine repo as **`engine/msc6/`**, driven by **`wt msc6 <cmd>`**.

- **Ported** (the unique byte-exact capability): `detect_recipe.py`, `ne_normalize.py`,
  `bruteforce.py`, `autoaxis.py`, the version-keyed toolchain resolver + manifest,
  `IDIOMS.md`, the reconstruction spec (→ `SPEC.md`), and the proven byte-exact
  LZEXPAND source (→ `engine/msc6/reconstructed/lzexpand/`).
- **Retired** (CodeBreaker already does these, better): `ne.py`, `disasm.py`, `omf.py`,
  `verify.py`/segdiff, `skeleton.py`/`pydiscover.py` (Ghidra skeleton → use `ir_engine`
  lift), `triage.py`/`scaffold.py`, `win95tree.py` (→ `data/src/generated/`).

Toolchain, corpus, and DOSBox paths are configured in `CodeBreaker/config.json`
(`msc6_toolchain`, `msc6_data`, `dosbox_x`).

The reorganized **toolchain/** (compilers/linkers/assemblers/…) stays here and is referenced
by CodeBreaker via `config.json:msc6_toolchain` — it is not duplicated into CodeBreaker
(which ships no third-party binaries). The `_work/treeA/` byte-exact build harness also
stays here as the working reconstruction; `engine/msc6/reconstructed/lzexpand/` holds the
clean deliverable source.

This directory is retained for its toolchain and working artifacts; new byte-exact
reconstruction work happens in CodeBreaker `engine/msc6/`.
