# CodeBreaker

**Recover a program's original, authored source — the readable C and C++ its developers actually
wrote — from the shipped binary.**

CodeBreaker is a reverse-engineering workbench. You give it a program — a 16- or 32-bit Windows
`.exe`/`.dll`/`.vxd`, or an Xbox 360 title — and it disassembles it, lifts it to structured C, and
runs a deep cascade of reconstruction engines that recover names, types, control flow, data
structures, C++ classes, resources, and whole function bodies as source a person can read. Where
the class of code allows, it goes all the way to source that **recompiles to the exact original
bytes** — and proves it by rebuilding and comparing.

Its guiding rule is **faithfulness over coverage**: it triages every function by origin first
(compiler/CRT/library code is *identified*, not "reconstructed"), it backs recovery with a live
execution oracle instead of guesswork, and anything it cannot establish it flags — it never
fabricates. Byte-exact reassembly is how it *verifies* itself, not the goal; the goal is the
authored source.

It ships as a **tool only** — no Microsoft code, no game code, nothing copyrighted. You point it at
files you have the right to analyze. See **[Disclaimer](#disclaimer)**.

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
- **Prefer the command line?** Same program, run *with a command*: `python wt.py doctor`,
  `python wt.py diff versions`, and so on. In the packaged app, `CodeBreaker.exe doctor` is the same.

Disassembly, lifting, decompiling, and browsing work out of the box. The one thing that needs extra
setup is the **byte-exact rebuild** loop, which drives real period compilers — see
**[Get the dependencies](#get-the-dependencies)**.

## By the numbers

A single installable Python package (`src/`), driven by one CLI (`wt.py`) and buildable to
`CodeBreaker.exe`. Current snapshot:

| Area (`src/…`) | Files | Lines | What lives here |
|---|---:|---:|---|
| `engines/` | 239 | 43,974 | the reconstruction engines (10 groups — see below) |
| `xbox/` | 206 | 57,638 | the Xbox 360 / XEX2 / PowerPC subsystem + console tools |
| `targets/` | 171 | 40,175 | per-era overrides (win1.0 · win3.1 · win95 · win98) |
| `ir/` | 10 | 6,045 | the lifter: asm → typed IR → readable C |
| `toolchain/` | 32 | 3,387 | uniform compiler/asm/linker interface, SDK reader, Ghidra bridge |
| `studio/` | 5 | 2,489 | the workbench UI (native WinForms, GDI fallback) |
| `formats/` | 12 | 2,095 | NE / PE / LE / OMF parsers |
| `core/` | 11 | 2,027 | path resolver, layout, config, toolchain, parallelism, progress |
| `disasm/` | 5 | 1,110 | disassembly & function carving |
| `cli/` | 2 | 15 | shared CLI helpers |
| **Total** | **694** | **~159,000** | (plus `wt.py`, tests, and vendored build tooling) |

**Reconstruction engines** (`src/engines/`, modules per group): **behavior 82** · **inverse 52** ·
**recompile 24** · **recon 19** · **symbol 15** · **msc6 10** · **match 9** · **diff 8** ·
**build 7** · **coverage 3** — ~230 engine modules in all, each routed by a cascade that sends every
function to whatever can best explain it.

## How it works

1. **Triage by origin first.** Before reconstructing anything, classify each function: is it the
   app's own code, an OS API, a statically-linked library (CRT, MFC, OLE), or compiler-emitted glue
   (`_chkstk`, `__ftol`, EH prologs)? Toolchain code is *identified and named* — not reconstructed,
   because it isn't the author's source. Only app/OS code is a reconstruction target.
2. **Recover from evidence, not guesswork.** The behavioral engine runs the real machine code on an
   emulation oracle (Unicorn), records what each function actually does, and actively experiments to
   learn what its inputs mean — then synthesizes C and **differential-tests it back against the
   binary**.
3. **Honest tiers.** Every result carries a confidence tier. A function the evidence fully pins down
   is emitted with high confidence; a partial result is labelled as such; anything undecidable is
   flagged, never faked. A module is always reported at 100% coverage only because the floor is a
   verified byte-exact ASM transcription — so nothing is hidden behind a fabricated body.
4. **Prove it by rebuilding.** Where the code class allows, the recovered source is reassembled /
   recompiled and compared to the original **byte for byte**; that is the verification standard, and
   it's how the tool earns the right to claim a reconstruction is correct.

## What it recovers

### Disassembly & lifting
Disassemble any Windows 16/32-bit or Xbox 360 binary into clean, reassembleable assembly, then lift
it through a typed IR into readable, structured C with real control flow.

### Behavioral reconstruction — the oracle-driven cascade
`wt behavior trace | experiment | explore | hyp | graph | synth | struct | cfg | globals | loops | recon | sweep`
A cascade of specialist engines, each validated on the oracle, routes every function to whatever
explains it:
- **synth / multisynth / piecewise** — pure input→output expressions (single- and multi-statement).
- **struct / effect** — struct layouts and field-level side effects from a pointer argument.
- **globals / gptr** — writes to global state (byte/word/dword-typed; consecutive writes collapse into
  array loops) and global-pointer structs.
- **loops** — counted-loop idioms: `memset`/fill, `strlen` (scan-to-sentinel), `memcpy` (copy).
- **cfg / cfgstruct** — control flow: branch predicates and the *structured skeleton* (basic blocks +
  conditions from `cmp`/`jcc`), each call placed in its block.
- **interproc / interproc_v3** — "orchestrator" functions as their call sequence, call args mapped to
  the caller's parameters, **exact arg counts from the SDK protos**, value flow
  (`r0 = f(x); g(r0); return r0`), bottom-up so callees are named before callers.
- **envprov / handleprov** — provision plausible OS state so functions that fault on a null-global
  deref/call run to completion, then reconstruct them.
- **fuzz / fracture / perturb / exectree / gatebust / terminate** — the dynamic cluster: drive,
  mutate, and split execution to expose behavior the static view misses.

The floor engines together recover **~93%** of the functions the leaf engines alone leave behind — a
window procedure, for example, comes back as
`sub(HWND a0, UINT a1, WPARAM a2, LPARAM a3) { … LRESULT r = DefWindowProcA(a0,a1,a2,a3); return r; }`.

### C / C++ recovery
- **demangle** — MSVC-mangled names → readable C++ signatures (`?AddRef@CFoo@@UAGKXZ` →
  `public: virtual unsigned long __stdcall CFoo::AddRef(void)`), incl. templates (`?$CArray@H@@` →
  `CArray<int>`); names a huge share of MFC/OLE functions for free.
- **rtti / mfcclass** — MSVC RTTI *and* MFC `CRuntimeClass` → real **class names + inheritance**
  (`class CFoo : public CObject`), with or without RTTI present.
- **classlayout** — vtables → C++/COM class structs (IUnknown/IDispatch prefixes labelled).
- **structsynth** — synthesize a **novel struct** (incl. C bit-fields) from a pointer's field-access
  pattern when it isn't a known SDK type.
- **cxxeh** — `__CxxFrameHandler` tables → real `try { } catch (T) { }` with the caught types.
- **mfcmap** — MFC message/interface/dispatch maps → `BEGIN_MESSAGE_MAP` / `BEGIN_INTERFACE_MAP` /
  `BEGIN_DISPATCH_MAP` — an MFC app's entire event + OLE wiring, as source.

### Readability & typing
- **datarec** — resolve constant pointers to their data: offsets become `"Cannot open %s"`.
- **type propagation / typeunify** — infer real types (`HWND`, `LPCSTR`, `HANDLE`) from how values
  flow into SDK calls and across the call graph, instead of `unsigned`.
- **constants** — magic constants → SDK flag/enum names (`LMEM_ZEROINIT`, `MB_YESNO | MB_ICONQUESTION`).
- **structure** — lift the goto/label CFG into real `if` / `if-else` / `while` / `do-while` / `for` /
  short-circuit `&&`/`||` for every region it can *prove* reducible, keeping `goto` only where control
  flow is genuinely irreducible.
- **switch / jump-table**, **vtable / COM** (`call [this+N]` → `vcall_N`), **SEH**
  (`__try`/`__except`), **dataflow (SSA-lite)** (predicates read `if (result != 0)`, not
  `if (eax == ecx)`), **stackframe** (typed locals), **localname / globalname / rettype / callconv /
  arity** (names, return types, calling convention, parameter counts by call-site consensus).
- **fpsynth / fpblock / simdblock** — x87 float reconstruction (whole functions and embedded blocks)
  and MMX/SSE packed ops → intrinsics.
- **stackstring** — strings built inline on the stack (`mov [ebp-8],'H'`) → the literal `"Hello"`.

### Origin triage & libraries
- **crtsig** — identify CRT / compiler-helper functions so they're marked *toolchain*, not
  reconstructed.
- **libsig** — FLIRT-style identification of statically-linked library functions (CRT, MFC, OLE), with
  signatures built from the period `.OBJ`/`.LIB` files themselves.
- **compiler_id** — fingerprint the toolchain (linker version, Rich header, CRT strings) to guide
  which library signatures apply.
- **fmtargs** — type a printf-family call's varargs from its format string (`%s`→`LPCSTR`, `%ld`→`long`).
- **mfcord / win16ord** — resolve **ordinal-only imports** (`MFC250.dll!#282`, `USER.1`) to real names
  from the period `.def`/`.lib` ground truth; names thousands of calls at once.
- **xref / constprop** — whole-program cross-reference index and forward constant propagation.

### 16-bit & driver frontier
The other half of the corpus (NE, VxD, DOS):
- **win16 code cascade** (`tracer16` / `recon16` / `interproc16`) — reconstruct 16-bit NE function
  bodies, including **caller reconstruction** that recovers a Win16 function's named call sequence
  (`CreateWindow → AppendMenu → WinHelp …`) statically from the relocation table, for **every**
  function (exports + internal). Real authored names come from the NE name tables, args are typed by
  the Win16 proto DB (`win16proto`, 679 signatures from the SDK `WINDOWS.H`), and string args resolve
  against the data segment (`datarec16`): `MessageBox(hWnd, "Cannot open %s", "Error", MB_OK)`.
- **callbacksig16 / wmdispatch16** — callbacks get their real signature
  (`LRESULT CALLBACK MainWindowProc(HWND, UINT, WPARAM, LPARAM)`) and the `WM_` messages they handle.
- **resxref16** — cross-link code and resources: `WM_COMMAND` cases → menu items, `LoadString` ids →
  strings, `GetDlgItem` ids → dialog controls, `DialogBox` → its DlgProc.
- **vxdsvc / vxdddb / dosint** — VxD ring-0 service calls (`int 20h` + id → `VMM_Get_VMM_Version`; the
  DDK service tables), a VxD's Device Descriptor Block + control-message handlers, and DOS/BIOS
  interrupts (`int 21h`/`10h`/… → `DOS_WriteFile`).
- **neidiom / thunk32 / vbxmodel / installer** — Win16 NE idioms, 32↔16 thunks, VBX control
  properties, installer family + embedded INF.

### Non-code reconstruction
- **resources** — the PE `.rsrc` tree back to `.rc`: STRINGTABLE, VS_VERSION_INFO, ACCELERATORS,
  MESSAGETABLE, and RT_BITMAP/ICON/CURSOR extracted to real `.bmp`/`.ico`/`.cur` files.
- **dialogs** — RT_DIALOG / RT_MENU templates back to authored `.rc` `DIALOG`/`MENU` source.
- **neresources** — the whole UI surface of a **16-bit** NE app back to `.rc` (its own on-disk format).
- **typelib** — a COM type library back toward **IDL** (interfaces, coclasses, GUIDs).
- **datainit** — initialized global data (function-pointer tables, vtables, string globals) as C
  initializers, via the base relocations.

### Byte-exact verification & VxD-from-source
The proof floor. The reconstruction is reassembled/recompiled and compared to the original byte for
byte. On the Win95 VxD target the byte-exact loop verifies all **222/222 modules (903/903 objects)**,
a Python LE/W3 writer re-synthesizes them into loadable binaries, and the reconstructed **VMM kernel +
static VxD stack boot real Windows 95 to the logon** (see [`tools/vmm32boot`](tools/vmm32boot)). The
Win95 *Layer-2* pipeline classifies every function of a 32-bit PE, emits clean C (`emit_c`) and clean
C++ for `__thiscall` methods (`emit_cpp`) — each **confirmed byte-exact by the period MSVC compiler** —
and floors the rest at verified ASM, so a module is 100% covered and nothing is faked
(see [`src/targets/win95/L2_PIPELINE.md`](src/targets/win95/L2_PIPELINE.md)).

### Cross-version diffing
Line the same function up across releases — even **16-bit → 32-bit** — and carry a recovered name
forward onto its twin, with a git-style browser.

### Xbox 360
The same discipline for **XEX2 / PowerPC**, plus package, crypto, and forensics tools.

## Commands

One program — `wt.py`, packaged as `CodeBreaker.exe` — drives everything; the workbench (`wt ui`) is a
friendly face over the same commands.

```
wt ui                    the workbench (also what a double-click opens)
wt diff <cmd>            compare a function across releases (+ a git-style browser)
wt behavior <cmd>        run the binary as an oracle: trace → experiment → hypotheses → synthesize
wt inverse <cmd>         recover byte-exact C (Lattice C 2.12 / MS C 2.03)
wt msc6 <cmd>            byte-exact 16-bit Windows (NE) DLL reconstruction
wt recompile <cmd>       take recovered C the rest of the way (compile → link)
wt backbone <cmd>        the uniform compiler / assembler / linker / disassembler interface
wt sdk <cmd>             read an SDK/DDK into typed signatures
wt xbox <cmd>            Xbox 360 (XEX2 / PowerPC) reconstruction + console tools
wt --target <era> <cmd>  per-era tools (win1.0 / win3.1 / win95 / win98)
wt doctor                what's installed and configured
```

Requirements: **Python 3.10+** and the packages in `requirements.txt` — **Capstone** (disassembly),
**pycdlib** (ISO images), **pycryptodome** (Xbox crypto), and **pythonnet** (native UI on Windows; the
UI falls back to a built-in one if absent).

For the full engineering overview — every engine, its entry command, status, and measured byte-exact
results — see **[`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md)**.

## Get the dependencies

CodeBreaker is just the tool; disassembly, lifting, decompiling, and browsing work out of the box. The
**byte-exact rebuild** loop drives the *original period toolchains*, which you attach yourself — they
live under `toolchains/` at the repo root (git-ignored). Open **Settings & tools** in the workbench (or
run `python wt.py doctor`) to see what's configured.

| Tool | What it's for | Where to get it |
|---|---|---|
| **DOSBox-X** | runs the DOS-era compilers | https://dosbox-x.com/ |
| **7-Zip** | unpacking archives / disc images | https://www.7-zip.org/ |
| **Ghidra** *(optional)* | extra cross-checking + type archives | https://ghidra-sre.org/ |
| **NASM** — the bundled `nasm-masm` fork | native MASM-matching assembler | build it (below); upstream: https://www.nasm.us/ |
| **Period Microsoft compilers** — MS C 6.0a, MASM 5.1, MS C 2.03, MSVC 1.52c, and MSVC 7.1 SP1 for the recompile path | the exact tools the originals were built with | see the note below |

**Building the bundled NASM.** The MASM-matching assembler lives in `tools/nasm-masm/` as source.
Build it (`sh autogen.sh && ./configure && make`, or `nmake -f Mkfiles/msvc.mak`) and point
`config.json`'s `nasm` at the resulting `nasm` executable — the byte-exact rebuild then runs natively
(no DOSBox), ~10–20× faster. See [`docs/NATIVE_ASSEMBLER.md`](docs/NATIVE_ASSEMBLER.md).

**The vintage Microsoft compilers.** These are decades-old Microsoft products, **not** distributed with
CodeBreaker — use a copy you are licensed to use. For preservation they are commonly archived at
**WinWorld** (https://winworldpc.com/) and the **Internet Archive** (https://archive.org/); obtaining
and using them is your responsibility and remains subject to Microsoft's rights. You only need them for
the byte-exact rebuild loop, not for disassembly, lifting, decompiling, or the cross-version tools.

Copy `config.json.example` to `config.json` and fill in the paths you use (the workbench's **Settings**
panel writes the same file). A full inventory of everything outside the repo — the toolchains, and the
Win95 corpus/oracle/reference/VMs that live in the sibling `Windows95` project repo — is in
[`docs/NOT_IN_REPO.md`](docs/NOT_IN_REPO.md).

## Layout

CodeBreaker is an installable package (`pip install -e .`); `wt.py` is the single CLI entry.

```
pyproject.toml   packaging (makes the `src` package importable; deps; the `wt` console script)
wt.py            single dispatcher / CLI (and the double-click entry point)
config.json      local external-tool paths (git-ignored; copy config.json.example)
src/             the tool — one package, grouped by role
  core/            shared base: paths resolver, layout, config, toolchain, parallel, progress
  formats/         NE / PE / LE / OMF parsers
  disasm/          disassembly & carving
  ir/              IR engine: asm → typed IR → readable C
  cli/             shared CLI helpers
  engines/         the reconstruction engines
    inverse/         compiler-inverse: bytes → byte-exact C (Lattice C / MS C 2.03)    → wt inverse
    recon/           byte-exact reconstruction + conductor
    behavior/        dynamic reconstruction on the Unicorn oracle                      → wt behavior
    msc6/            MSC-6.0a byte-exact 16-bit NE reconstruction                       → wt msc6
    recompile/       make the lifted C compile/link                                    → wt recompile
    diff/            cross-version fingerprint / match / propagate + git-style UI       → wt diff
    match/ symbol/ build/ coverage/   supporting engines
  toolchain/       backbone (uniform compiler/asm/linker → wt backbone), sdk (→ wt sdk), reference, ghidra
  studio/          the workbench UI (native WinForms, GDI fallback)                    → wt ui
  xbox/            Xbox 360: XEX2/PowerPC recon + console/crypto/forensics             → wt xbox
  targets/         per-era overrides (win1.0 … win98), loaded by path as DI            → wt --target
tools/           vendored build-time tooling + smoke.py (source only; binaries not committed)
  nasm-masm/       NASM fork with MASM-matching output (the native assembler)
toolchains/      git-ignored: period MS compilers + DOSBox-X + MSVC 7.1 SP1 (attach your own)
binaries/        git-ignored: generated Ghidra type archives (ghidra_gdt) only
docs/            architecture, the not-in-repo inventory, and the pre-snapshot history record
```

The Win95 corpus, the golden oracle ISO, the period reference SDK/DDK, and the VM images are **not**
under CodeBreaker — they live in the sibling `Windows95` project repo and are resolved via
`WT_DATA_ROOT` / the `paths` resolver. See [`docs/NOT_IN_REPO.md`](docs/NOT_IN_REPO.md).

## What is not in the repo

CodeBreaker tracks **tool code only.** Input corpora, the period Microsoft toolchains, VM images, build
artifacts, machine-specific config, and any secrets are never committed — they're attached locally. The
authoritative inventory, with sizes and re-attach steps, is in
[`docs/NOT_IN_REPO.md`](docs/NOT_IN_REPO.md).

## Disclaimer

CodeBreaker is intended for **interoperability, preservation, security research, and education**,
applied to software **you have the legal right to analyze**.

- This repository contains **no** operating-system code, binaries, headers, SDKs, DDKs, compilers, or
  other third-party/proprietary material. The tool operates only on inputs **you** provide.
- **Reconstructed output is a derivative of its input.** Whatever you produce by pointing CodeBreaker at
  a binary carries the copyright status of that binary. Producing, storing, or distributing such output
  is **your responsibility** and may be restricted by copyright law and by the software's license —
  obtain the rights you need first.
- The authors distribute the *tool* only, and make no representation that any particular use is lawful
  in your jurisdiction. Provided "AS IS", without warranty (see LICENSE).

Do not use CodeBreaker to infringe copyright, breach a license or contract, or circumvent technical
protection measures where doing so is unlawful.

## License

[Apache License 2.0](LICENSE). Copyright 2026 The CodeBreaker Authors. See [NOTICE](NOTICE).
