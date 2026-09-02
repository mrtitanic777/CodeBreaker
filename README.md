# CodeBreaker

**Take a program apart, see how it works, and rebuild it — byte for byte.**

CodeBreaker is a reverse-engineering workbench. You give it a program — a Windows `.exe` or
`.dll`, or an Xbox 360 title — and it disassembles it, turns it into readable C, and for
supported cases can recover source code that recompiles to the *exact same bytes* as the
original. Everything it claims, it proves by rebuilding and comparing; it never guesses.

It ships as a **tool only**. It contains no Microsoft code, no game code, nothing copyrighted —
you point it at files you have the right to analyze. See **[Disclaimer](#disclaimer)**.

![The CodeBreaker workbench](docs/workbench.png)

## Start here

The easiest way in is the **workbench** — an ordinary desktop app. Drop a program onto it and it
tells you, in plain terms, what the file is and what you can do with it.

- **Have the packaged app?** Double-click **`CodeBreaker.exe`** and the workbench opens.
- **Running from source?**
  ```bash
  pip install -r requirements.txt
  python wt.py            # opens the workbench
  ```
- **Prefer the command line?** It's the same program — run it *with a command* instead of
  double-clicking: `python wt.py doctor`, `python wt.py diff versions`, and so on. In the
  packaged app, `CodeBreaker.exe doctor` behaves the same way.

That's all you need to look around, disassemble, and decompile. The one thing that needs extra
setup is the **byte-exact rebuild** loop, which drives real period compilers — see
**[Get the dependencies](#get-the-dependencies)**.

## What you can do with it

- **Take a program apart** — disassemble any Windows (16/32-bit) or Xbox 360 binary into clean,
  reassembleable assembly.
- **Turn it into C** — lift that assembly into readable, structured C you can actually follow.
- **Rebuild it byte-for-byte** — for supported classes, recover source that recompiles to the
  *exact* original bytes (proven by rebuilding, not by eye).
- **Compare across versions** — line the same function up across releases (even 16-bit → 32-bit)
  and carry a recovered name forward onto its twin.
- **Xbox 360** — the same discipline for XEX2 / PowerPC, plus package, crypto, and forensics tools.

## Get the dependencies

CodeBreaker itself is just the tool, and disassembly, decompiling, and browsing work out of the
box. The **byte-exact rebuild** loop drives the *original period toolchains*, which you attach
yourself. Open **Settings & tools** in the workbench (or run `python wt.py doctor`) to see what's
configured — a green dot means ready.

| Tool | What it's for | Where to get it |
|---|---|---|
| **DOSBox-X** | runs the DOS-era compilers | https://dosbox-x.com/ |
| **7-Zip** | unpacking archives / disc images | https://www.7-zip.org/ |
| **Ghidra** *(optional)* | extra cross-checking | https://ghidra-sre.org/ |
| **NASM** — the bundled `nasm-masm` fork | native assembler | build it (below); upstream: https://www.nasm.us/ |
| **Period Microsoft compilers** — MS C 6.0a, MASM 5.1, MS C 2.03, MSVC 1.52c | the exact tools the originals were built with | see the note below |

**Building the bundled NASM.** The MASM-matching assembler lives in `tools/nasm-masm/` as source.
Build it (`sh autogen.sh && ./configure && make`, or `nmake -f Mkfiles/msvc.mak`) and point
`config.json`'s `nasm` at the resulting `nasm` executable.

**The vintage Microsoft compilers.** These are decades-old Microsoft products and are **not**
distributed with CodeBreaker — use a copy you are licensed to use. For preservation they are
commonly archived at **WinWorld** (https://winworldpc.com/) and the **Internet Archive**
(https://archive.org/); obtaining and using them is your responsibility and remains subject to
Microsoft's rights. You only need these for the byte-exact rebuild loop, not for disassembly,
decompiling, or the cross-version tools.

Copy `config.json.example` to `config.json` and fill in the paths you use (the workbench's
**Settings** panel writes the same file). A full inventory of everything that lives outside the
repo is in [`docs/NOT_IN_REPO.md`](docs/NOT_IN_REPO.md).

## Under the hood

One program — `wt.py`, packaged as `CodeBreaker.exe` — drives everything. The workbench (`wt ui`)
is a friendly face over the same commands:

```
wt ui                    the workbench (also what a double-click opens)
wt diff <cmd>            compare a function across releases (+ a git-style browser)
wt inverse <cmd>         recover byte-exact C (Lattice C 2.12 / MS C 2.03)
wt msc6 <cmd>            byte-exact 16-bit Windows (NE) DLL reconstruction
wt recompile <cmd>       take recovered C the rest of the way (compile → link)
wt backbone <cmd>        the uniform compiler / assembler / linker / disassembler interface
wt xbox <cmd>            Xbox 360 (XEX2 / PowerPC) reconstruction + console tools
wt sdk <cmd>             read an SDK/DDK into typed signatures
wt --target <era> <cmd>  per-era tools (win1.0 / win3.1 / win95 / win98)
wt doctor                what's installed and configured
```

Requirements: **Python 3.10+** and the packages in `requirements.txt` — **Capstone** for
disassembly, **pycdlib** for ISO images, **pycryptodome** for the Xbox crypto, and **pythonnet**
for the native UI on Windows (the UI falls back to a built-in one if pythonnet isn't present).

Reconstruction means the emitted source re-assembles/re-links to the *exact* original bytes,
verified by rebuilding — nothing is claimed that isn't proven against the input, and anything the
tools cannot decode is flagged, never faked.

### Layout

```
wt.py            single dispatcher / CLI (and the double-click entry point)
config.json      local external-tool paths (git-ignored; copy config.json.example)
engine/          the tool's core — a Python package of subsystem subpackages
  foundation/      shared base: layout, config, toolchain, parallel, progress
  ir_engine/       IR engine: asm → typed IR → readable C
  disasm_engine/   disassembly & carving
  format_loaders/  NE / PE / LE / OMF parsers
  recon_engine/    byte-exact reconstruction + conductor
  inverse_engine/  compiler-inverse: bytes → byte-exact C (Lattice C / MS C 2.03) → wt inverse
  msc6/            MSC-6.0a byte-exact 16-bit NE reconstruction                    → wt msc6
  recompile95/     make the lifted C compile/link                                 → wt recompile
  diff/            cross-version fingerprint / match / propagate + git-style UI    → wt diff
  backbone/        uniform compiler/assembler/linker/disassembler interface        → wt backbone
  sdk/             SDK/DDK ingestion → typed signatures                            → wt sdk
  studio/          the workbench UI (native WinForms, GDI fallback)                → wt ui
  xbox/            Xbox 360: XEX2/PowerPC recon + console/crypto/forensics         → wt xbox
  targets/         per-era overrides (win1.0 … win98)                             → wt --target
tools/           vendored build-time tooling (source only; binaries not committed)
  nasm-masm/       NASM fork with MASM-matching output (the native assembler)
binaries/        git-ignored DATA HOME: corpus, toolchains, VMs, build output (attach your own)
```

## What is not in the repo

CodeBreaker tracks **tool code only.** Input corpora, the period Microsoft toolchains, VM images,
build artifacts, machine-specific config, and any secrets are never committed — they're attached
locally. The authoritative inventory, with sizes and re-attach steps, is in
[`docs/NOT_IN_REPO.md`](docs/NOT_IN_REPO.md).

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
