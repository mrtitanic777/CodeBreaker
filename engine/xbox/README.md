# XboxTranspiler — an Xbox 360 reverse-engineering workbench

Forked from **WinTranspiler** (the Windows 1.0 / 3.1 / 95 teardown toolchain)
and extended to the **Xbox 360**. Same discipline as the lineage it comes from:
dependency-light tools that take a retail binary apart, plus *evidence only,
never guess* — every reconstructed byte traces to the binary, and anything the
tools cannot decode is flagged, never faked.

The Xbox 360 is a big-endian **PowerPC** console (the "Xenon" CPU). Its
executables ship as **XEX** containers wrapping an ordinary PE image whose code
is PowerPC. XboxTranspiler reads that container, decodes the payload, and
disassembles / byte-exact-reconstructs the PowerPC the same way the inherited PC
path handles x86. The Windows NE/PE/LE loaders are retained — this is a superset,
dispatched per file by format (`target.format_of`).

## The Xbox 360 pipeline

```
raw default.xex
  │  xex.py         parse XEX2/XEX1 header, optional-header directory, security info
  ▼
  ├─ AES-128-CBC decrypt   (public retail key; devkit key auto-tried) ── xex.py
  ├─ de-block + SHA-1      (per-block {size, hash} framing)          ── _lzx.py
  ├─ LZX / basic decompress                                          ── _lzx.py
  ▼
embedded PE  (machine 0x1F2 = PowerPC-BE)
  │  xex._pe_sections      .text + .embsec_* are Xenon code
  ▼
  ├─ disasm_ppc.py    capstone PPC, big-endian, recursive-descent code/data
  └─ asmrecon_ppc.py  byte-exact reconstruction (.long-backed; % live mnemonics)
```

## Project layout

The backend is a Python package, organized by domain. Modules keep the flat
sibling-import style they were written in; `xbt/__init__.py` injects every
subpackage dir onto `sys.path`, so the algorithm code is grouped without being
rewritten (byte-exact behavior is preserved by construction).

```
XboxTranspiler.py          # top-level launcher
xbt/
  __init__.py              # path injection -> one flat import namespace
  core/                    # shared infra + analysis (target, ledger, verify, ...)
  xbox/                    # XEX container + LZX
  ppc/                     # PowerPC pipeline (disasm / recon / recomp / boot / run)
  win/                     # NE / PE / LE / OMF x86 superset
  match/                   # matching-decompilation (permute, oracle, segdiff, conduct)
  assets/                  # (retool / pck / xzp live in cli/plugins)
  crypto/                  # Xbox-360-Crypto (GoobyCorp, BSD-3) — importable + crypto-* cmds
  cli/                     # entry point, build_exe, plugins/
  gui/                     # the graphical workbench
  vendor/                  # rex_kernel / rex_dxbc
samples/                   # local data (git-ignored): tu14, xbox-kernel, dashboard-17559
reference/                 # third-party tools kept for reference (git-ignored)
```

The entry point resolves each command to its module across the subpackages and
runs it as `__main__` — every tool runs unchanged, one entry point over all of
them. A graphical workbench fronts the whole set: `XboxTranspiler gui`.

## Commands

```bash
XboxTranspiler xex        default.xex --sections   # header + decoded PE section map
XboxTranspiler xex        default.xex --dump out.exe   # write the decoded basefile
XboxTranspiler ppcdisasm  default.xex --limit 40    # disassemble the PowerPC .text
XboxTranspiler ppcrecon   default.xex -o tetris.S   # byte-exact PowerPC reconstruction
XboxTranspiler ppcsym     default.xex --out sym/    # symbol map: imports+entry+RTTI+pdata -> json/map/Ghidra
XboxTranspiler ppccc      recon.c --flags "/c /O2"  # compile C with the XDK PowerPC compiler
XboxTranspiler permute --arch ppc --selftest        # matching-decompilation search (byte-exact)
XboxTranspiler doctor                               # check the toolchain is wired up
XboxTranspiler gui                                 # full graphical workbench
XboxTranspiler crypto-kv | crypto-cpukey | crypto-xdk   # Xbox 360 crypto (GoobyCorp, BSD-3)
```

Drop a `.xex` in `files/`, or pass a full path to any command.

## Build-diffing & asset extraction

Folded in from the standalone RETool + companion scripts, these operate on
*decoded* PE images (use `xex --dump` first) and on Minecraft LCE asset
containers. Same doctrine — every reported difference is a real byte in the
image; nothing is inferred.

```bash
XboxTranspiler retool info      a.exe [b.exe ...]   # metrics table (size/.text/funcs/classes/xdk/pdb)
XboxTranspiler retool classes   old.exe new.exe     # RTTI class-inventory diff (added/removed/kept)
XboxTranspiler retool scenes    old.exe new.exe     # XUI scene / .xur resource diff
XboxTranspiler retool imports   old.exe new.exe     # platform (kernel/XAM) symbol diff
XboxTranspiler retool timeline  a.exe b.exe c.exe   # first-seen / persistent-core matrix across builds
XboxTranspiler retool funcs     old.exe new.exe     # .pdata function diff (only meaningful within one build lineage)
XboxTranspiler retool patchdiff a.exe b.exe -o p.json   # capture a byte-patch as a portable, context-anchored diff
XboxTranspiler retool patchapply img.exe p.json         # re-apply a captured patch elsewhere

XboxTranspiler pck  TMSFiles.pck -x   # unpack an LCE .pck  container (PNG/loc/grf/col)
XboxTranspiler xzp  TexturePack.xzp -x   # unpack a XUIZ .xzp container (UI + texture packs)
```

Why class/scene/import diffing and not raw function bytes: RTTI class names,
scene names and platform imports are source-derived and survive recompilation,
so they diff meaningfully across builds; PPC function bodies almost never do
(measured ~6% survival between adjacent same-toolchain builds), which is why
`retool funcs` self-diagnoses and only reports within one lineage.

> Supersedes the earlier loose `xexclassdiff.py` (→ `retool classes`) and
> `xexpatch.py` (→ `retool patchdiff` / `retool patchapply`).


## Matching decompilation (byte-exact source recovery)

The permuter from the Win95 lineage, retargeted to PowerPC. Same doctrine — a
reconstruction is *proven* only when C compiled with the **original toolchain**
(here the **Xbox 360 XDK `cl.exe … for PowerPC`**, v16.00) reassembles to the
**exact shipped bytes**:

```
recon C  --ppc_cc (XDK cl.exe)-->  PowerPC COFF object
         --ppc_obj------------->  per-function bytes + BIT-level reloc mask
         --segdiff_ppc--------->  mask-aware diff vs the shipped XEX function
         --permute (search)---->  byte-exact form found, no LLM
```

Because PowerPC instructions are fixed 4-byte words, the relocation mask is a
**bit mask** (ignore just the `bl` displacement or the `lis/addi` address
immediate, still compare opcode + registers) — stricter than the NE byte-mask.
Point it at a real function:

```bash
XboxTranspiler permute --arch ppc --xex default.xex --vaddr 0x820FBC58 \
    recon.c --choices idioms.json --func my_func
```

## Validation — proven against ground truth

Validated against a real title (Xbox 360 Tetris) using a decompressed copy of
the same image as an oracle:

| stage | result |
|---|---|
| XEX2 header / optional headers / security info | exact (entry `0x820FBC58`, base `0x82000000`, original name `tetrisXENON.exe`) |
| AES-128 decrypt (retail key, session key at security-info `+0x150`) | confirmed — SHA-1 block hashes verify |
| LZX decompression (`_lzx.py`, ported from the XEX LZXD codec) | **byte-exact** — all 18 PE sections of the retail image match the decompressed oracle; output is the exact `PPC_IMAGE_SIZE` (`0x520000`) |
| PowerPC disassembly | validated on real game code (function prologues, `bl`/`bc`, address-building `lis`/`addi` pairs); ~99% decode coverage on Tetris `.text` |
| byte-exact reconstruction | `.long`-backed emission proven to re-parse to the exact section bytes; reports true "% live" decode coverage |
| matching-decompilation permuter | proven byte-exact with the real XDK PowerPC compiler — the search recovers source that compiles to the exact target bytes |

## Honest limitations (flagged, per doctrine)

- **VMX128 vector instructions** (`vperm128`, `vpkd3d128`, …) are Xbox-360
  extensions stock Capstone does not decode. These are now **fully rendered**:
  `vmx128.py` ports the 77-row VMX128 opcode table (operands included), wired
  into `ppcdisasm` / `ppcrecon` / `ppcdecomp`. On the retail Minecraft `.text`
  (`vmx128 --coverage default.xex`): 6,693 primary-op 4/5/6 words →
  **6,109 decoded by the VMX128 table + 584 plain AltiVec by Capstone = 100%
  operand coverage, 0 undecoded**. `vmx128 --selftest` verifies all 77 rows.
  (A residual: functions containing VMX128 still disassemble byte-exact, but a
  handful of *rare* VMX128 forms outside the 77-row table would fall back to
  `.word` — none were hit in this title.)
- **`ppcrecon` mnemonic promotion** (turning `.long` lines into assembler-
  *verified* instructions) needs a PowerPC assembler — the XDK ships none, and
  there's no `llvm-mc` on the box. Without one, output is still byte-exact by
  construction, every line carries its decoded mnemonic, and the tool reports the
  honest **decode-coverage "% live"** (~99% on Tetris). Install `llvm-mc` and set
  `config ppc_as llvm-mc` to enable the stricter reassemble-verified metric.
- **XEX1 / delta (title-update) images**: XEX1's key sits at security-info
  `+0x134` (handled); delta-compressed images must be applied against their base
  title first (reported, not silently mis-decoded).

## Build

```bash
python tools/build_exe.py        # -> tools/dist/XboxTranspiler.exe (onefile)
```

`capstone` (x86 + PowerPC) and a crypto lib (pycryptodome / cryptography, if
present) are bundled; Ghidra and an optional PowerPC assembler are *driven by
path* (`config`), not embedded.

## Doctrine (unchanged from the lineage)

Retail bytes are truth. A reconstruction is "done" only when it is **byte-exact**,
which is machine-checkable — the reason no LLM is in the loop. Whatever resists
automation is flagged (`.long`, "unverified"), never faked.
