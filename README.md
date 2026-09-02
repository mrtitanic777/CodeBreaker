# CodeBreaker

## Project Status

**🟢 Working tool — actively developed.** A deterministic, evidence-only reverse-engineering
workbench: disassemble → byte-exact reassembleable ASM → readable C, with an IR engine that
lifts proven-exact assembly into structured C and a cross-version engine that tracks a
function's identity across releases like a version-control history. It spans early-PC binaries
(16-bit NE and PE across Windows 1.0 / 3.1 / 95 / 98) **and** the Xbox 360 (XEX2 / PowerPC),
behind one dispatcher.

CodeBreaker is a *tool*. It ships **no** binaries, source, or headers from any operating
system or third party — you point it at inputs you supply and attach your own compilers.
See **[Disclaimer](#disclaimer)**.

---

## What it does

- **Byte-exact reconstruction.** "Reconstructed" means the emitted source re-assembles/
  re-links to the *exact* original bytes — verified by rebuilding, not by eyeballing.
  Nothing is claimed that isn't proven against the input bytes; anything the tools cannot
  decode is flagged, never faked.
- **IR engine → readable C** (`engine/ir_engine/il.py`, `engine/ir_engine/decompile.py`). Lifts
  byte-exact assembly into a typed, explicit-flag intermediate representation, runs optimization
  passes (dead-flag elimination, constant/copy propagation, dead-store elimination), recovers
  `if/else`/loop structure and call arguments, and emits clean C. Because the assembly provably
  re-assembles to the original bytes, the lift can be *differentially validated against ground
  truth* — verified, not guessed.
- **Compiler-inverse engine** (`engine/inverse_engine/`, run as `wt inverse …`). Inverts a
  period compiler's codegen to recover byte-exact C — Lattice C 2.12 and MS C 2.03 (oracle-gated
  via `config.json`) — plus the **MSC-6.0a NE engine** (`engine/msc6/`, `wt msc6 …`) that
  reconstructs 16-bit Windows DLLs to source that re-links to the exact shipped bytes through
  the authentic MSC 6.0a / MASM 5.1 / LINK toolchain under DOSBox.
- **Cross-version analysis** (`engine/diff/`, run as `wt diff …`). Fingerprints every function
  (CFG topology / normalized opcode stream / call vector), matches the same function across
  releases even across a 16-bit → 32-bit jump, and can carry recovered names from one release
  onto its unnamed twin in another. Includes a browser GUI (`wt diff ui`) that presents the
  corpus like a git history — releases as commits, functions as files, matches as a function's
  lineage, with a side-by-side diff.
- **Toolchain backbone** (`engine/backbone/`, run as `wt backbone …`). One uniform interface
  over compilers / assemblers / linkers / disassemblers, swappable by name. Includes a native,
  cross-platform, DOSBox-free assembler — the vendored **nasm-masm** fork's `--masm` mode, which
  emits MASM-matching bytes (`xor eax,eax` → `33 c0`, MASM's encoding, not NASM's `31 c0`).
- **Xbox 360 subsystem** (`engine/xbox/`, run as `wt xbox …`). XEX2 container parsing and
  byte-exact PowerPC (Xenon) reconstruction, the same discipline as the PC path, plus
  console/format/crypto/forensics tooling (STFS/GoD/FATX, resigning, dump analysis).

## One entry point, many subsystems

`wt.py` is a single dispatcher. Target-agnostic subsystems are invoked by name; era-specific
toolchains live behind a `--target` switch; the shared core lives in `engine/`.

```
python wt.py doctor                    # external-dependency check (what's configured)
python wt.py targets                   # list eras + where their tools live

# target-agnostic subsystems (no --target):
python wt.py diff <command>            # cross-version fingerprint / match / propagate (+ ui)
python wt.py inverse <command>         # compiler-inverse: bytes -> byte-exact C (Lattice/MSC 2.03)
python wt.py msc6 <command>            # MSC-6.0a byte-exact reconstruction of 16-bit NE DLLs
python wt.py recompile <command>       # take lifted C the rest of the way (compile -> link)
python wt.py backbone <command>        # uniform compiler/assembler/linker/disassembler API
python wt.py xbox <command>            # Xbox 360: XEX2/PowerPC recon + console/crypto/forensics
python wt.py sdk <command>             # SDK/DDK ingestion: headers/defs -> typed signatures
python wt.py ui                        # CodeBreaker Studio (native Win32/GDI workbench)

# era-specific tools:
python wt.py --target <era> <command>  # run a tool for <era> (win1.0 | win3.1 | win95 | win98)
python wt.py --target <era> list       # every command available for <era>
```

## Quick start

Requirements: **Python 3.10+** and the packages in `requirements.txt`
(**[Capstone](https://www.capstone-engine.org)** for disassembly, **pycdlib** for ISO images,
**pycryptodome** for the Xbox crypto tooling). Some subsystems additionally shell out to period
toolchains (a DOS emulator + vintage assemblers/compilers) that you install separately and point
to in `config.json` — those are only needed for the byte-exact rebuild loop, not for
disassembly or the IR/diff features.

```bash
pip install -r requirements.txt
python wt.py doctor            # shows which optional external tools are configured
python wt.py diff versions     # the cross-version engine's registered snapshots
```

`config.json` holds paths to the optional external tools (copy `config.json.example`; left blank
by default — fill in only what you use). Nothing here ships in the repo; you attach your own:

```json
{ "dosbox_x": "", "msc6_toolchain": "", "msc6_data": "",
  "msc203_oracle": "", "nasm": "tools/nasm-masm/nasm.exe",
  "masm": "reference/masm/BIN", "sevenzip": "7z", "ghidra": "" }
```

Bulk data (corpus, disassembly, Microsoft toolchains, VMs, build artifacts) lives under
`binaries/` — a git-ignored **data home** you attach locally. See
**[What is not in the repo](#what-is-not-in-the-repo)**.

## Layout

```
wt.py            single dispatcher / CLI
config.json      local external-tool paths (git-ignored; copy config.json.example)
env.sh           optional shell environment (paths for pipeline/build scripts)
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
  goldens/         golden fixtures for regression
  build_engine/    module/image build & packaging
  reference/       external reference-source tooling
  diff/            cross-version fingerprint / match / propagate + git-style UI  → wt diff
  inverse_engine/  compiler-inverse: bytes → byte-exact C (Lattice C 2.12 / MSC 2.03) → wt inverse
  msc6/            MSC-6.0a byte-exact 16-bit NE reconstruction + tree filing      → wt msc6
  recompile95/     make the lifted C compile/link                                 → wt recompile
  backbone/        uniform compiler/assembler/linker/disassembler API             → wt backbone
  sdk/             SDK/DDK ingestion → typed signatures                           → wt sdk
  studio/          CodeBreaker Studio — native Win32/GDI workbench                → wt ui
  xbox/            Xbox 360: XEX2/PowerPC recon + console/crypto/forensics         → wt xbox
  targets/         per-era overrides (win1.0 … win98), injected via cores         → wt --target
tools/           vendored build-time tooling (source only; binaries not committed)
  nasm-masm/       NASM fork with `--masm` byte-matching mode (native assembler backbone)
binaries/        git-ignored DATA HOME: corpus, toolchains, VMs, build output (attach your own)
```

**Imports are explicit** (`from engine.ir_engine import il`) — a module's location is its
identity. The modules that legitimately differ per era (the "quirk cores") are obtained
through `engine.foundation.cores.get(name)`, which returns the active target's override or
the engine default. That registry replaces `sys.path`-shadowing, so `--target` selection is
explicit and centralized rather than import-order luck.

## What is not in the repo

CodeBreaker tracks **tool code only**. Everything below is git-ignored and must be attached
locally — none of it is distributed. A full, categorized inventory lives in
[`docs/NOT_IN_REPO.md`](docs/NOT_IN_REPO.md).

- **`binaries/`** — the data home: input corpora, disassembly, period Microsoft toolchains
  (you supply), VM images, and build artifacts.
- **`config.json`** — machine-specific external-tool paths (copy `config.json.example`).
- **Third-party toolchain binaries** — DOSBox-X, DOS-era MS compilers/assemblers/linkers, the
  PowerPC toolchain for the Xbox path, and any built `nasm-masm`/`*.o`/`*.obj` output.
- **Console secrets, samples, and generated ledgers** — never committed (see the inventory).

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
