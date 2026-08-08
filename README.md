# CodeBreaker

**A deterministic, evidence-only reverse-engineering workbench for early-PC binaries** —
disassemble → byte-exact reassembleable ASM → readable C, with an IR engine that lifts
proven-exact assembly into structured C, and a cross-version engine that tracks a function's
identity across releases like a version-control history.

CodeBreaker is a *tool*. It ships **no** binaries, source, or headers from any operating
system or third party — you point it at inputs you supply. See **[Disclaimer](#disclaimer)**.

---

## What it does

- **Byte-exact reconstruction.** "Reconstructed" means the emitted source re-assembles/
  re-links to the *exact* original bytes — verified by rebuilding, not by eyeballing.
  Nothing is claimed that isn't proven against the input bytes.
- **IR engine → readable C** (`engine/il.py`, `engine/decompile.py`). Lifts byte-exact
  assembly into a typed, explicit-flag intermediate representation, runs optimization
  passes (dead-flag elimination, constant/copy propagation, dead-store elimination),
  recovers `if/else`/loop structure and call arguments, and emits clean C. Because the
  assembly provably re-assembles to the original bytes, the lift can be *differentially
  validated against ground truth* — verified, not guessed.
- **Cross-version analysis** (`engine/diff/`, run as `wt diff …`). Fingerprints every
  function (CFG topology / normalized opcode stream / call vector), matches the same
  function across releases even across a 16-bit → 32-bit jump, and can carry recovered
  names from one release onto its unnamed twin in another. Includes a browser GUI
  (`wt diff ui`) that presents the corpus like a git history — releases as commits,
  functions as files, matches as a function's lineage, with a side-by-side diff.

## One entry point, many eras

`wt.py` is a single dispatcher. Era-specific toolchains live behind a `--target` switch;
the shared, target-agnostic core lives in `engine/`.

```
python wt.py --target <t> <command> [args...]   # run a tool of target <t>
python wt.py --target <t> list                  # every command available for <t>
python wt.py targets                            # list targets + where their tools live
python wt.py doctor                             # external-dependency check
python wt.py diff <command>                     # cross-version analysis (no --target)
```

## Quick start

Requirements: **Python 3.10+** and the packages in `requirements.txt`
(**[Capstone](https://www.capstone-engine.org)** for disassembly, **pycdlib** for ISO
images). Some era-specific tools additionally shell out to period toolchains (a DOS
emulator + vintage assemblers/compilers) that you install separately and point to in
`config.json` — those are only needed for the byte-exact rebuild loop, not for
disassembly or the IR/diff features.

```bash
pip install -r requirements.txt
python wt.py doctor            # shows which optional external tools are configured
python wt.py diff versions     # the cross-version engine's registered snapshots
```

`config.json` holds paths to the optional external tools (left blank by default; fill in
only what you use):

```json
{ "dosbox_x": "", "masm": "reference/masm/BIN", "msvc152c": "reference/msvc152c",
  "sevenzip": "7z", "ghidra": "" }
```

## Layout

```
wt.py            single dispatcher / CLI
engine/          the tool's core — a Python package of subsystem subpackages
  foundation/      shared base: layout, config, toolchain, parallel, progress
  foundation/cores.py   per-target override registry (dependency injection; see below)
  ir_engine/       IR engine: asm → typed IR → readable C (il, lift, cfg, decompile, …)
  disasm_engine/   disassembly & carving (trace, disasm, reachability)
  format_loaders/  NE / PE / LE / OMF parsers
  recon_engine/    byte-exact reconstruction + conductor (asmrecon, masmlift, verify, conduct)
  match_engine/    matching-decompilation → byte-exact C (permute, oracle, raiser, …)
  symbol_engine/   naming / ordinals (names, idord, genordinals)
  coverage/        ledger, goldens, link-coverage
  build_engine/    module/image build & packaging
  reference/       external reference-source tooling
  diff/            cross-version fingerprint / match / propagate + git-style UI  → wt diff
  recompile95/     make the lifted C compile/link                                → wt recompile
  targets/         per-era overrides (win1.0 … winnt4), injected via cores       → wt --target
```

**Imports are explicit** (`from engine.ir_engine import il`) — a module's location is its
identity. The modules that legitimately differ per era (the "quirk cores") are obtained
through `engine.foundation.cores.get(name)`, which returns the active target's override or
the engine default. That registry replaces `sys.path`-shadowing, so `--target` selection is
explicit and centralized rather than import-order luck.

## Disclaimer

CodeBreaker is intended for **interoperability, preservation, security research, and
education**, applied to software **you have the legal right to analyze**.

- This repository contains **no** operating-system code, binaries, headers, SDKs, DDKs,
  compilers, or other third-party/proprietary material. The tool operates only on inputs
  **you** provide.
- **Reconstructed output is a derivative of its input.** Whatever you produce by pointing
  CodeBreaker at a binary carries the copyright status of that binary. Producing, storing,
  or distributing such output is **your responsibility** and may be restricted by copyright
  law and by the software's license — obtain the rights you need first.
- The authors distribute the *tool* only, and make no representation that any particular
  use is lawful in your jurisdiction. Provided "AS IS", without warranty (see LICENSE).

Do not use CodeBreaker to infringe copyright, breach a license or contract, or circumvent
technical protection measures where doing so is unlawful.

## License

[Apache License 2.0](LICENSE). Copyright 2026 The CodeBreaker Authors. See [NOTICE](NOTICE).
