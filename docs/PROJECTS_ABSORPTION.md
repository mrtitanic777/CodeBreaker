# Absorbing Projects/ into CodeBreaker

29 project folders in `Projects/`. The goal is not to copy the applications in: it is to take
each project's **lessons** (what it proved can be done, and how) and its **genuinely-built
tools**, and fold those into CodeBreaker so CodeBreaker could reconstruct what each project
reconstructed. Third-party code that was only ever a reference stays out.

Almost everything here is one subject: the **Xbox 360** (PowerPC "Xenon", XEX containers, the
XDK). CodeBreaker already has the PowerPC decompile lane and, since this week, the XEX engine
(`native/xex`). These projects are the mature body of work that lane should grow into.

## A. XboxTranspiler — the sibling toolkit, the single biggest source

`XboxTranspiler/` is CodeBreaker's sibling: its own README says it was "forked from
WinTranspiler, the Windows 1.0/3.1/95 teardown toolchain" — the same lineage CodeBreaker came
from — and extended to the Xbox 360. It is ~90 % the same shape as CodeBreaker, one generation
behind the consolidation. It carries the mature PowerPC work CodeBreaker's lane is still thin on:

| XboxTranspiler module | What it is | CodeBreaker today |
|---|---|---|
| `xbox/xex.py`, `xbox/_lzx.py`, `xbox/lzx_encode.py` | XEX reader + LZX (a byte-exact port of XexTool-RE's encoder) | **already have** — this is exactly `native/xex` (same ancestor) |
| `ppc/disasm_ppc.py`, `ppc/vmx128.py` | PPC + VMX128 (the 128-vector ext stock disassemblers miss) | partial: our lane decodes PPC; **VMX128 is a real gap** |
| `ppc/ppcdecomp.py` | a Ghidra-independent PPC → pseudo-C decompiler | we use the Ghidra core; this is an independent check |
| `ppc/asmrecon_ppc.py`, `ppc/segdiff_ppc.py` | byte-exact PPC reconstruction + segment match | our `exact/` does this for x86, not yet PPC |
| `ppc/ppcsym.py`, `ppc/pdata.py`, `ppc/msvc_demangle.py`, `ppc/ppc_obj.py` | symbol/evidence map, .pdata unwind, MSVC demangler, COFF reader+reloc mask | **port**: our PowerPC `module_writer` is much thinner |
| `ppc/c2model.py`, `ppc/ppc_cc.py` | predict the XDK c2.dll's codegen; drive the XDK PPC compiler | feeds a PPC `exact`/`invert` lane |
| `match/{permute,oracle,segdiff,conduct}.py` | matching-decompilation: a deterministic byte-exact search that "replaces spawn-N-agents" | relates to `exact/` + `verify`; **a real engine we lack** |
| `core/` (target, ledger, verify, names, trace, parallel, progress) | the same infrastructure, one generation back | we have our own; mine for anything better |

**Recommendation:** treat XboxTranspiler as the source that brings CodeBreaker's PowerPC lane up
to the parity the x86 lane has. Port, in order: VMX128 decode; .pdata + ppcsym + MSVC demangle +
ppc_obj into the PowerPC `module_writer`/symbols; the PPC `exact` lane (asmrecon_ppc, segdiff_ppc,
ppc_cc, c2model); then the matching-decompilation engine as a general `verify`/`exact` facility.

## B. Static recompilation to runnable native — a new output lane

`MinecraftPC/`, `TetrisPC/`, `UP/` and `Shared/` are finished **static recompilations**: a whole
Xbox 360 title turned into C++ that builds and runs on PC (the ReXGlue / XenonRecomp method).
`XboxTranspiler/ppc/ppcrecomp.py`, `ppcproject.py`, `ppcrun.py`, `ppcboot.py` implement it; the
games are the proof it works end to end (sound, threads, saves — the same bar the Minecraft Java
port hit).

This is a **different deliverable** from CodeBreaker's "decompile each function and judge it":
whole-program lift to a runnable artifact. It is the native-code analogue of what
`codebreaker/java` already does for Minecraft (Java → C++03 → runs on PC/360). It belongs as a
recompile lane beside the decompile lane.

**Lesson, not code:** the method (lift every function to C++, a runtime that provides the
kernel/GPU imports, a generated VS/CMake project) is the valuable part. `Shared/` holds the clean
reference (XenonRecomp configs, switch-table TOML, RTTI/vtable recovery, import stubs,
`xdk_libmatch.py`). `rexglue-sdk/` is **not ours** (the rexglue org, 347 commits) — study it,
don't absorb it.

## C. New platforms to describe (feeds docs/PLATFORM_DESCRIPTOR.md)

Each of these is a platform CodeBreaker does not yet have a descriptor for; the project is the
proof and the format notes:

| Platform | Project | What it adds |
|---|---|---|
| Original **Xbox** (XBE, x86) | `xbox/xbe.py`, `Xbox-vs-360-Kernel-Diff` | XBE reader; x86 lane already exists, only the container + kernel imports are new |
| **XNA / C# managed** (Xbox 360) | `Arcadecraft` | managed C# recovery via IL decompile, XNB asset decode, decompiler-artifact repair — a bytecode lane like Java |
| **Scaleform GFx / GoodEngine** game data | `UP/IronKit`, `PixarUP/modkit` | engine-archive (.HO), GFx/SWF UI, tiled textures, layout recovery from a draw-trace |

## D. Container & asset format readers — a library to fold into formats/

A large, working set of readers, almost all in `XboxTranspiler/xbox/` (and mirrored in
`Shared/tools`). These are unambiguous additions to `codebreaker/formats/`:

- **Executables/containers:** XEX variants + patching (xex_deep, xex_patch, xex_sign, xexinject),
  XBE, STFS (CON/LIVE/PIRS), GoD/SVOD, FATX, GDF/XDVDFS, XGD ISO, CHD, CSO/CCI, ZArchive.
- **Assets:** textures (tiled DXT untile → DDS/PNG), XPR, XWB/XMA/XMA2 audio, Bink, XUR (compiled
  XUI), XDBF/GPD profiles, Scaleform GFx, XNB (XNA).
- **The lesson:** these extend the "Unpack" layer of the platform roadmap far past what the
  Windows corpus needed.

## E. The XDK toolchain studies — symbol & toolchain knowledge

`cl-re`, `c1-re`, `c2-re`, `link-re`, `imagexex-re`, `deltaxex-re`, `mspdb-re`, `xma2-re`,
`xui-re`, `xenon-shader-re`, `disc-re`, `xdk-build-demo` are reverse-engineering **studies** of
the XDK tools — mostly readable notes (the `-re` naming), a little recon code. Their value is
**knowledge**, to fold into CodeBreaker's docs and its symbol/toolchain models:
- how the XDK compiler chain works (cl → c1/c1xx → c2 → link → imagexex) — the Xbox 360 analogue
  of `codebreaker/invert`'s compiler models and the period-toolchain `exact` profiles.
- `imagexex-re` and `deltaxex-re` are the XEX packaging/patch format — already covered by
  `native/xex`, but the notes name fields our engine should carry.
- `xam/` is a 91.7 %-complete clean-room decompile of `xam.xex` with `ppc2c.py` (a 2,666-line
  PPC→C) and arity/clobber inference tools — a ready **corpus + the inference passes** a PPC lane
  wants.

## F. Directory / corpus-mapping tools — the ones you built to map it all

The tools that "map up a directory" and identify what everything is — port these into
CodeBreaker's `formats`/`analyze`/`coverage`:

- `xbox/identify.py` — drop any file, learn what it is (every Xbox/360 format). A peer to
  `codebreaker/formats/detect.py`, much wider.
- `xbox/report.py` — point at any file, get a one-page summary. Peer to `codebreaker info`.
- `core/ledger.py`, `coverage` — how much of a target is reconstructed (we have `coverage/`).
- `core/refscan.py` / `refpull.py` — tally an external reference tree and score its relevance;
  copy the relevant slice in with a manifest. A genuine "map the directory" tool.
- `xbox/selftest.py` — a self-contained regression suite for the content tools.

## G. Keep separate (not source-reconstruction)

- **Live-console & security:** `NexGen` (and `XboxTranspiler/xbox`): XBDM, JRPC2,
  memory scan, NAND/keyvault/forensics, a LIVE server. A large, self-contained Xbox 360
  security/homebrew toolkit — **not** part of taking software back to source. Keep it its own
  project (or an optional plugin); don't fold it into the core pipeline. (Its `crypto/` was
  absorbed after all, Logan 2026-10-23: `codebreaker/formats/xbox/crypto`, the `crypto` command.)
- **Forward compiler / OS:** `NexiaCompiler` (from-scratch C++ → PowerPC → XEX) and `NexiaOS`
  (a research kernel that runs XEX via its own PPC interpreter). These are *builders and
  runtimes*, the opposite direction from reconstruction. Their **lessons** (a complete PPC
  codegen + register allocator, XEX emission, a PPC interpreter) are worth noting against our
  `build`/`emulate` PPC pieces, but the projects themselves stay separate.

## H. Third-party — never absorb

`rexglue-sdk/` (rexglue org),
`Xbox 360 IDA/` (xorloser's IDA plugins + an IDA installer), `XuiWorkshop/` (2014, third-party).
Keep as local reference; keep out of git.

## Proposed order

1. **Format readers (D)** — the clearest win: fold the container/asset readers into
   `codebreaker/formats/`, with `identify`/`report` (F) extending `detect`/`info`.
2. **PowerPC lane to parity (A)** — VMX128, then .pdata/ppcsym/demangle/ppc_obj into the PowerPC
   `module_writer`, then the PPC `exact` lane.
3. **Platform descriptors (C)** — original Xbox (XBE) and XNA/C# as new platforms, alongside the
   6502 work already planned.
4. **Recompile lane (B)** — a whole-program-to-runnable-native lane, the native analogue of the
   Java transpiler, with `Shared/` as the reference.
5. **Matching-decompilation (E/A)** — the permute/oracle/segdiff/conduct engine as a general
   `exact`/`verify` facility.

Lessons recorded as they are folded; each tool rebranded and integrated natively (not copied
wholesale), the same standard as the XEX, NASM and RetroMCP folds.

## Per-folder audit (2026-10-22)

What each remaining folder still holds that CodeBreaker lacks, and what happens to it. Nothing is
deleted without Logan's yes; "trim" means build output, logs and caches inside a folder.

| Folder | Size | Holds | Status in CodeBreaker | Proposed |
|---|---|---|---|---|
| `MinecraftPC` | 1.8 G | Minecraft: Xbox 360 Edition **TU0** disc files; earlier port attempts (`Source/`) | recompiled and playing: `Converted/Minecraft` (its XEX in `original/`, its files in `assets/`) | delete once Logan has played it |
| `UP` | 8.3 G | Up disc files (`UP_Native_PC_Port/GameData`); four earlier ports; **IronKit** (mod kit); HiHoTool (third-party) | recompiled: `Converted/Up` (its files in `assets/`). IronKit absorbed (2026-10-22): `heavyiron` (formats/xbox/heavyiron.py: .HO archives read, replaced in place or grown, textures; a `media` volume), DXT decoding in formats/xbox/texture.py, `gfx` (formats/gfx.py: Scaleform GFx movies, images, fonts, placements; a font as TrueType). Superseded: its trace replay (the old port's own log; the renderer replays GPU traces). The test samples moved out (2026-10-23): `../xbox-samples` | delete once Logan agrees |
| `Shared` | 248 M | the ports' runtime (absorbed: `native/x360`), XenonRecomp (absorbed), docs and LIVE server (absorbed), `tools/scripts` | absorbed: switch scanner, XEX basic, import stubs, instruction tests; and (2026-10-22) `nand_fs_extract` -> `formats/xbox/nand.py` (a `media` volume), `xdk_libmatch` -> `xdk-match` (`symbols/xdk_libraries.py`, signatures cached: 11 s a title), `rtti_vtables` -> `classes` (`symbols/cpp_classes.py`; `demangle.type_name`), `extract_system_ui` -> `system-ui`, `xex_imports` -> `xex-imports`, `gen_fma_tests` -> `native/xenonrecomp/ppc_tests/own/gen_fused_madd.py`. Superseded (Ghidra only): `annotate_decomp`, `xex_symbols`, `ghidra_scripts`. Still elsewhere: `nand_keyvault`, `make_game_package`, `import_profile`, `prepare_module` (the runtime's comments name them; they are in the Dashboard repository, not here) | delete once Logan agrees (the Dashboard keeps its own `Shared`) |
| `XboxTranspiler` | 1.2 G | the sibling toolkit | VMX128, XBE, formats folded (waves A, C, D). `crypto/` absorbed and deleted from Projects/ (2026-10-23): `codebreaker/formats/xbox/crypto` (GoobyCorp's Xbox-360-Crypto, BSD-3, kept as written with its LICENSE and ATTRIBUTION.md; the `crypto` command; tests/test_xbox_crypto.py; not carried: its `bin/` (Microsoft LZX libraries, binutils, captured LIVE requests), `kv_checker.py`, `assembler.py`). The matching-decompilation engine (`match/`) dropped (Logan, 2026-10-23). Its test samples moved to `../xbox-samples`. Not yet: `xex_patch/sign/inject`, `identify`/`report`/`refscan` | absorb the rest, then delete |
| `xam` | 43 M | clean-room decompile of xam.xex (91.7 %), `ppc2c.py`, arity/clobber inference | not yet | absorb the inference passes and keep the corpus as a reference target; then delete |
| `Arcadecraft` | 2.2 G | XNA / C# (managed) decompilation | the XNA/C# platform lane is not built yet | build the lane, then delete |
| `cl-re`, `c1-re`, `c2-re`, `link-re`, `imagexex-re`, `deltaxex-re`, `mspdb-re`, `xma2-re`, `xui-re`, `xenon-shader-re`, `disc-re` | ~180 M | XDK toolchain studies (notes, reconstructed source) | each is a Converted project now (2026-10-23): `Converted/cl`, `c1`, `c2`, `link`, `imagexex`, `deltaxex`, `mspdb`, `xma2`, `xui`, `xenon-shader`, `disc` -- the tool in `original/`, the reconstructed source in `src/`, what was built from it in `build/`, notes, the raw decompile and the study's scripts in `reports/`, its git history as `reports/history.bundle` | deleted from Projects/ (2026-10-23) |
| `xdk-build-demo` | 2.5 M | a hello-world built with the XDK (not a study of a binary) | -- | keep or delete: Logan's call |
| `Xbox-vs-360-Kernel-Diff` | 1.5 M | kernel export comparison | used for the XBE lane's kernel names (wave C) | check, then delete |
| `powerpc-samples` | 19 M | the 12 Xbox 360 images the function store judged | copied to `../powerpc-samples` (2026-10-23), where NOT_IN_REPO points | deleted from Projects/ (2026-10-23) |
| `NexGen`, `NexiaCompiler`, `NexiaOS` | 2.3 G | security/homebrew toolkit; a forward compiler; a research OS | not reconstruction tools (section G) | move out to their own repos beside CodeBreaker |
| `rexglue-sdk` | 1.5 G | the rexglue SDK (third-party, public) | the parts used are vendored in `native/rexglue` | delete (it is on GitHub) |
| `ghidra`, `ghidra_12.1.3_PUBLIC` | 1.1 G | Ghidra source and install (third-party) | CodeBreaker no longer needs them (its own decompiler) | removed |
| `Xbox 360 IDA`, `XuiWorkshop` | 390 M | third-party IDA plugins and installer; a 2014 XUI tool | not used | delete |

## Removed

- **TetrisPC** (2026-10-22, with Logan's yes): Tetris Splash runs on CodeBreaker's own recompile
  lane and Xbox 360 runtime (docs/RECOMPILE.md), with sound, which the earlier port never had. Kept: the
  `bin.tort` archive reader (`codebreaker/formats/xbox/tort.py`); the game's original LIVE package
  and XEX and its files moved into `Converted/TetrisSplash/` (not in the repo). Its earlier ports,
  extracted copies, Ghidra project, raw audio dumps and audio diagnosis were superseded.
- **ghidra, ghidra_12.1.3_PUBLIC** (2026-10-22, Logan: no Ghidra at all): CodeBreaker decompiles with its
  own engine (`native/decompiler`, which carries the decompiler core it forked: docs/GHIDRA_FORK.md). Also
  gone: the Ghidra user settings (`%APPDATA%\ghidra`), a desktop shortcut to an install that no longer
  existed, and `native/ghidra_core` (an old, unused build) with its `ghidra_core` setting.
