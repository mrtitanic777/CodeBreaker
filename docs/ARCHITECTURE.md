# CodeBreaker — Architecture & Capabilities

The single authoritative map of what CodeBreaker is, how it's built, and what it
can do today. Front-door narrative lives in [`README.md`](../README.md); this is
the engineering overview for planning and regrouping.

---

## 1. What it is

CodeBreaker is a **deterministic reverse-engineering workbench**. Point it at a
program — a 16/32-bit Windows `.exe`/`.dll`/`.vxd`, or an Xbox 360 title — and it
disassembles it, lifts it to readable C, and for supported classes recovers source
that recompiles to the **exact same bytes** as the original.

**The doctrine, everywhere:** evidence only; **byte-exact or flagged, never faked**;
every claim proven by rebuilding and comparing against the input, using the
*original period toolchains* as oracles; **no LLM in the reconstruction loop**.
Anything the tools can't decode is declined and marked, not guessed.

Two complementary reconstruction philosophies live under one roof:
- **Static, byte-exact** — lift the bytes, model the compiler's codegen, prove the
  rebuild is byte-identical (the `inverse` / `msc6` / `recon` / L2 pipelines).
- **Dynamic, behavioral** — run the real machine code as an oracle (Unicorn),
  observe what it does, and reconstruct C from evidence (the `behavior` engine).

---

## 2. One entry point

Everything is driven by **`wt.py`** (packaged as `CodeBreaker.exe`). It selects a
target era, becomes that era's data context (cwd + import path + DI overrides), and
dispatches. The **workbench** (`wt ui`) is a friendly face over the same commands.

```
wt ui                     the drag-and-drop workbench (also what a double-click opens)
wt doctor                 external-dependency check
wt targets                list eras and where their tools live
wt --target <era> <cmd>   per-era tools (win1.0 / win3.1 / win95 / win98)
wt inverse  <cmd>         bytes -> byte-exact C (compiler-inverse engine)
wt msc6     <cmd>         byte-exact 16-bit NE (MSC 6.0a) reconstruction
wt recompile <cmd>        take lifted C the rest of the way (compile -> link)
wt behavior <cmd>         run the binary as an oracle: trace -> experiment -> synth
wt diff     <cmd>         cross-version fingerprint / match / propagate + git-style UI
wt backbone <cmd>         uniform compiler/assembler/linker/disassembler interface
wt sdk      <cmd>         read an SDK/DDK into typed signatures
wt xbox     <cmd>         Xbox 360 (XEX2 / PowerPC) reconstruction + console tools
```

Dispatch styles: `inverse`/`msc6`/`recompile`/`behavior` are **script-directory
subsystems** (each non-underscore `.py` is a sub-command); `diff`/`backbone`/`sdk`/
`xbox`/`ui` have their own entry; `recon`/`match`/`symbol`/`coverage`/`build`/`ir`
tools are **shared engine commands** run as `wt --target <era> <cmd>`.

---

## 3. The layered package (`src/`)

Installable (`pip install -e .`); one package, grouped by role.

### Foundation — `core/`
The DI + path/config plumbing that lets one shared engine serve five eras.
- `paths.py` — the single canonical location resolver (repo/package/binaries/
  target/data roots; PyInstaller-aware).
- `layout.py` — a target's data-dir tree; encodes a gradual per-target migration
  from flat `reconstructed/` to `src/{generated,reconstructed}` + a component tree.
- `cores.py` — **the DI registry**: `cores.get("cc16")` lazily resolves to the
  active target's override or an engine default (replaced ~150 `sys.path` hacks).
- `config.py`, `toolchain.py` (resolves the real CL 9.0 / LINK 2.50–2.60 / ML 6.11c
  per module), `oracle.py` (golden-ISO baseline extraction), `parallel.py`,
  `progress.py`.

### Formats — `formats/`
Dependency-free parsers: **NE** (`ne.py`), **PE** (`pe.py` + `pe_seg`/`pe_strings`/
`pe_trace`), **LE/W3** (`le.py`/`le_seg`/`leloader`/`w3.py` — VxD + VMM32), **OMF**
(`objextract.py`).

### Disassembly — `disasm/`
`disasm16.py` (capstone recursive-descent code/data classifier), `disasm.py`
(16-bit NE via objdump), `reachability.py`, `trace.py` (call-graph tracer).

### IR — `ir/`
`il.py` — the **CodeBreaker IL**: lifts each instruction to typed register-transfer
IR with explicit x86 flags, runs opt passes (dead-flag/const/copy-prop, DSE), emits
C. `validate_il.py` (differential validator vs an independent interpreter),
`decompile.py` (batch `[AUTO]` C, enforces the HONESTY RULE: machine C is
scaffolding until a human promotes it). Raiser track: `lift`/`cfg`/`structure2`/
`framerecover`/`calleesaved`.

### Reconstruction engines — `engines/`

| Engine | Entry | Purpose | Status (measured) |
|---|---|---|---|
| **inverse** | `wt inverse` | Bytes → byte-exact C by modeling a compiler's exact codegen + reversing its optimizer. Four families: Lattice-C 2.12 / MS-C 2.03 (16-bit), MSC 6.0a (16-bit NE), MSVC 2.0 / C2.EXE (32-bit), and a large VxD/LE subsystem. Declines what it can't model. | 16-bit core mature (self-tests 19/19, 10/10, 25/25, 48/48). MSVC-2.0 32-bit early ("phase 1 map", ~3.8% direct). |
| **recon** | `wt --target <era> conduct` | The deterministic byte-exact ASM back-end: carve → disasm → MASM with a `db` fallback that **guarantees** byte-identity. Metric = "% live" (real instrs vs `db`). `conduct.py` orchestrates a whole module, resumable, fanned across cores. | Mature; the core grinder. Byte-exact by construction. |
| **msc6** | `wt msc6` | Byte-exact 16-bit NE DLLs built with MSC 6.0a: auto-detect the recipe, rebuild source that re-links to shipped bytes via the authentic toolchain. | LZEXPAND.DLL 8/12 code segments byte-identical; data/DOS-stub/NE-structure exact. |
| **recompile** | `wt recompile` | Take the IR's `[AUTO]` C up three rungs: (1) compiles under GCC gnu89 + prelude, (2) links across the whole OS, (3) byte-exact is a separate track. | Rung 1: **1115/1115 modules, 0 errors**. Rung 2: ~213,500 fns → objects; ~305,000 symbols linked. |
| **build** | `wt --target <era> buildmod` | Whole-image "build from source" + byte-provenance: prove headers, imports/exports, resources, and data are *reconstructed*, not retained. `headergen`/`impexp`/`resrc`/`datagen`/`datacode`/`iso9660`. | Working; each tool re-packs and byte-compares. |
| **behavior** | `wt behavior` | **Dynamic** reconstruction: run the function in Unicorn, record structured evidence, experiment, build hypotheses, synthesize + differential-test C. A validated engine **cascade** routes each function: `synth`/`multisynth`/`piecewise` (pure expressions), `struct`/`effect` (pointer-arg struct + field writes), `globals`/`gptr` (global state — byte/word/dword-typed, array-loop collapse), `loops` (memset/fill/strlen/memcpy), `cfg`/`cfgstruct` (branch predicates + the structured skeleton with calls placed in blocks), `interproc` (caller reconstruction — call sequence, SDK-exact args, value flow `r0=f(x); g(r0)`), `envprov` (provision OS state so null-faulting functions run). Readability/rigor across the emitters: `datarec` (constant->string literal), type propagation (SDK types), switch/jump-table, vtable/COM (`vcall_N`), SEH (`__try/__except`), `dataflow` (SSA-lite: name predicate operands), `structverify` (single-path->verified), `constants` (magic->SDK flag/enum names), register-args/thiscall (ecx/edx -> `/*this=*/`), `fpsynth` (x87 float reconstruction), `typeunify` (interprocedural type propagation), `stackframe` (typed local decls). **Structure + naming + SIMD:** `structure` (lift the goto/label CFG into real `if`/`if-else`/`while`/`do-while`/`for` for the reducible cases it can prove, goto only where irreducible; short-circuit `&&`/`||` recovery via De Morgan — cfgstruct emits through it), `fpblock` (embedded x87 float expressions per block — `dst = a*b + c;`), `simdblock` (MMX/SSE -> `_mm_mul_ps(...)`; ~zero on Win95 by era), `callconv` (stdcall/cdecl/thiscall/fastcall -> `__stdcall` on the signature), `callgraph` (static call graph: fan-in/out, SCC cycles, reverse-topo order), `interproc_v3` (bottom-up whole-module — names callees before callers), `funcname` (name from API fingerprint), `fieldname` (`p->f_4` -> `p->hwnd`). **Origin triage + typing + readability (latest):** `crtsig` (identify CRT/compiler helpers — `_chkstk`/`__allmul`/`__ftol`/memcpy-intrinsic — so recon marks them toolchain and does NOT reconstruct them), `libsig` (FLIRT-style statically-linked library identification, signatures built from OMF .OBJ/.LIB via FIXUPP masks), `fmtargs` (type printf-family varargs from the format string), `ssa` (cross-block value resolution — `if (a0 == a1)` not `if (esi == edi)`), `localname` (`local_8` -> `i`/`p` from use), `rettype` (void/value/pointer return inference), plus string-loop idioms `strcpy`/`strcat`/`strcmp`/`wcslen`/`memcmp` and switch case-value/default recovery. **C++ / advanced (latest):** `demangle` (MSVC-mangled names -> readable C++ signatures, `?AddRef@CFoo@@UAGKXZ` -> `public: virtual unsigned long __stdcall CFoo::AddRef(void)`), `rtti` (MSVC RTTI -> real class names + inheritance: `class CFoo : public CObject`, pairs with classlayout), `structsynth` (synthesize a novel struct from a pointer's field-access pattern when it isn't a known SDK type), `globalname` (name/type module globals from use -> `g_hInstance`/`g_count`/`g_ptr`), `tailcall` (import-thunk `jmp [IAT]` identified not reconstructed; tail-call `jmp f` -> `return f(...)`), `strdispatch` (a chain of `strcmp` against literals -> the keyword->handler verb table). **MFC + cross-function (latest):** `mfcmap` (AFX_MSGMAP arrays -> the authored `BEGIN_MESSAGE_MAP` block: ON_WM_*/ON_COMMAND/ON_MESSAGE handler wiring), template demangling in `demangle` (`?$CArray@H@@` -> `CArray<int>`), `arity` (pin a function's parameter count by consensus across all its call sites + `ret N`), bit-fields in `structsynth` (mask/shift field -> `unsigned x : n`), `compiler_id` (linker version + Rich header + CRT strings -> the toolchain, guiding libsig), `enumrec` (constants a variable is compared against -> a recovered `enum`), `sigrefine` (varargs `...`, BOOL params, identical-function folding). **MFC + C++ EH + navigation (latest):** `mfcord` (MFC ordinal-only imports -> real C++ names from the period .def/.lib), `mfcmap` (message / interface / dispatch maps -> BEGIN_MESSAGE_MAP / INTERFACE_MAP / DISPATCH_MAP), `mfcclass` (CRuntimeClass -> class names + inheritance without RTTI), `cxxeh` (`__CxxFrameHandler` FuncInfo -> `try { } catch (T) { }`), `stackstring` (inline byte/dword stores -> a string literal), `xref` (whole-program reverse call graph + string/import indices), `constprop` (forward constant propagation through registers). **16-bit / driver frontier (latest):** `win16ord` (Win16 ordinal-only imports USER/GDI/KRNL386/... -> real API names from the shipped export .def tables; NE IMPORTORDINAL fixups resolved), `vxdsvc` (VxD `INT 20h` + service-id -> named VMM/VxD ring-0 services from the DDK service tables), `dosint` (DOS/BIOS interrupts INT 21h/10h/13h/16h/1Ah/33h -> named system calls), `neidiom` (Win16 far/exported prologs, __loadds, MakeProcInstance thunks, INTERNALREF fixups), `recon/peextra` (delay-load imports, TLS callbacks, RCDATA classification), `recon/neresources` (**16-bit NE resources -> .rc: STRINGTABLE/MENU/DIALOG/ACCELERATORS + BITMAP/ICON/CURSOR** -- the Win16 UI surface), `vxdddb` (**VxD Device Descriptor Block -> device name/ID/version + entry points, and control-message handlers**), `thunk32` (32<->16 thunk-layer identification), `vbxmodel` (VBX control property/event names), `recon/installer` (installer/packaging family + embedded INF). **16-bit CODE cascade (latest):** `tracer16` (16-bit Unicorn oracle, now capturing per-call Pascal args), `recon16` (leaf synth grammars + delegation templates + the whole-module driver), `interproc16` (**reconstruct Win16 callers as their named static call sequence** -- fixups in a function's range resolved through the NE relocation table, Win16 APIs named via the ordinal oracle; `all_functions` enumerates every function via the entry table + far (INTERNALREF) + near (E8, >=2 sites) call targets, so INTERNAL functions -- the bulk of 16-bit code -- are covered, not just exports; **function NAMES from the NE resident/non-resident name tables** (MAINWINDOWPROC), **call ARGUMENTS from a sentinel-arg dynamic trace** mapped to params/consts/strings). **callback signatures** (`callbacksig16`: a `*PROC` name / behavior -> `LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM)`, `BOOL CALLBACK DlgProc(...)`), and **WM_ dispatch naming** (`wmdispatch16`: which WM_ messages a WndProc handles, byte-scanned conservatively). Supporting: `recon/datarec16` (**DGROUP string recovery** + the DS-offset->string resolver), `recon/datainit16` (**DGROUP pointer tables + globals as C**), `symbol/win16proto` (**Win16 API prototype DB from the SDK WINDOWS.H** -- 679 protos: types each call's args by exact arity and gates string resolution to string parameters). **resxref16** (**cross-references 16-bit code with resources**: WM_COMMAND ids->menu items, LoadString ids->strings, GetDlgItem ids->dialog controls, DialogBox->DlgProc). `recon` orchestrates (origin triage FIRST); `sweep` reports the frontier. Plus data-side recon engines: `recon/resources.py` (PE .rsrc -> .rc: STRINGTABLE, **VS_VERSION_INFO, ACCELERATORS, MESSAGETABLE decoded; RT_BITMAP/ICON/CURSOR extracted to real .bmp/.ico/.cur**), `recon/dialogs.py` (**RT_DIALOG/RT_MENU -> authored .rc**), `recon/typelib.py` (**type library -> IDL: MSFT full, SLTG partial — IUnknown/IDispatch recovered**), `classlayout` (**vtables -> C++/COM class structs**), and `recon/datainit.py` (initialized data -> C initializers). | Leaf engines mature; the floor engines recover **~93% of previously-floored functions**. A WndProc returns typed with named calls; single-path callers verify; cfgstruct lifts proven if/while/for structure, recovers embedded x87/SIMD, stamps the real convention and return type, names locals, and resolves values across blocks; CRT/library code is identified not reconstructed; resources come back as real .rc + images + IDL. |
| **diff** | `wt diff` | Cross-version function identity: a canonical fingerprint (CFG-topology 30% / opcode 40% / call-vector 30%) that survives 16→32-bit, then match + propagate names. Git-style browser UI. | Validated on win101 (3,633 fns) / win310 (18,004 fns); `enrich` named 3,747 win310 fns. |
| **match** | `wt --target <era> <cmd>` | Byte-exact "compile our C" matcher: census the frontier (`frontier`/`matchbatch`), pin compiler flags (`oracle`/`oraclebake`), close near-misses (`matchsearch`/`permute`/`raiser`). | Active; trivial tier ~99% byte-match. |
| **symbol** | `wt --target <era> <cmd>` | Naming oracle by **trust tier** (EXPORT = ground truth; SDK/CodeView/midage = hypothesis; never overrides a byte-verified fact). `names`/`genordinals`/`impresolve`/`codeview`/`sdksyms`/`symdb`/`sigscan`/`annotate`. | Mature; `symdb` holds 3,184 prototypes / 1,126 struct layouts. |
| **coverage** | `wt --target <era> <cmd>` | Measure (not estimate) reconstruction: `ledger` (bytes cited + fns carved vs written), `linkcov` (whole-corpus link coverage + % live), `goldens` (regression guard). | Mature guard layer. |

### Toolchain — `toolchain/`
- **backbone** (`wt backbone`) — one uniform interface per tool *role* (compiler/
  assembler/linker/disassembler); adapters for MSC-6.0a, MASM 5.1/6.13, LINK
  5.20/5.60, capstone. Honest availability (never substitutes a different tool).
- **sdk** (`wt sdk`) — ingest an SDK/DDK → typed signatures that feed the inverse
  (right param layout → recompiles to shipped bytes). `scan/detect/plan/api/recon/
  emit/refine/sweep/kits`.
- **reference** — score + pull the target-relevant slice of an external reference
  source tree (`refscan`/`refpull`).
- **ghidra** — high-accuracy Ghidra decompilation for the **32-bit+ functional**
  track (PE + LE/PE VxDs), explicitly *not* the byte-exact NE track.

### UI — `studio/`
`wt ui` → `winforms_app.py` (native Windows Forms via pythonnet) with a
dependency-free GDI fallback (`app.py`). An Analyze front door (drop a binary →
auto-detect + action cards), a command runner that streams any `wt` subsystem into
a console, and a Settings/Doctor panel.

### Xbox 360 — `xbox/`
`wt xbox` — a self-contained XEX2/PowerPC workbench: parse the XEX container,
AES-decrypt, de-block + SHA-1, LZX-decompress to the embedded PPC PE, disassemble
(capstone PPC), byte-exact `.long`-backed reconstruction. `xex`/`ppcdisasm`/
`ppcrecon`/`ppcsym`/`ppccc`/`permute`/`vmx128`/`retool`/`crypto-*`. Proven
byte-exact on retail Tetris (all 18 PE sections); VMX128 100% operand coverage on
retail Minecraft. Roadmap: static recompiler to runnable C++ (Pipeline B).

### Per-era targets — `targets/<era>/`
Only what genuinely differs per platform, loaded by path as DI overrides.
- **win1.0** — the broadest early-DOS/16-bit NE teardown set.
- **win3.1** — LE-era + KWAJ/CAB.
- **win98** — LE VxD (carryover from win95).
- **win95** (richest) — three layers: (a) **LE/VxD byte-exact recon**
  (`le`/`w3`/`asmrecon32`/`ml32`/`vmmsvc`/`buildmod`); (b) the **Layer-2 recompile
  pipeline** (`classify32`→`emit_c`/`emit_cpp`→ML floor→`l2pipe`/`l2sweep`/
  `l2report`/`l2predict`) — see [`../src/targets/win95/L2_PIPELINE.md`](../src/targets/win95/L2_PIPELINE.md)
  and [`WIN95_MAP.md`](../src/targets/win95/WIN95_MAP.md); (c) **packaging/
  boot** (`cab`/`isowrite`/`disc_build`/`bootchain`).

### Build-time tooling — `tools/` (source only)
- **nasm-masm** — a NASM fork adding `--masm`: assembles MASM 6.11 syntax and
  reproduces MASM's *encoding* (`xor eax,eax` → `33 c0`) so output matches `ML.EXE`
  — the fast native verification backbone (~10–20× vs DOSBox). Build it yourself.
- **vmm32boot** — the end-to-end boot proof: rebuild the 32-bit kernel + static VxD
  stack from recovered disassembly and boot it in a real Win95 VM. Includes `w4.py`,
  a pure-Python W4 codec that reproduces `vmm32.vxd` byte-for-byte.

---

## 4. Proven results (measured, byte-exact)

| Target | Result |
|---|---|
| **Win95 Ring-0 (VxD/VMM)** | **222/222 modules** and **903/903 objects byte-exact**; all 222 rebuild from source (authentic toolchain); the reconstructed VMM kernel + static VxD stack **boot Windows 95 to the logon** (VKD standalone-boot-verified). |
| **Win95 recompile** | 1115/1115 modules compile (0 errors); ~305,000 symbols link across the whole OS. |
| **16-bit inverse** | Local-var round-trip 19/19, real-LC2 10/10, frame-allocator 25/25, forward model 48/48. |
| **16-bit NE (msc6)** | LZEXPAND.DLL 8/12 code segments byte-identical (rest differ only in non-deterministic register tie-breaks). |
| **Cross-version (diff)** | win101 (3,633 fns) + win310 (18,004 fns) fingerprinted; 3,747 win310 fns named from exports. |
| **Win95 Layer-2 C++** | `emit_cpp` reconstructs ~418 `__thiscall` methods corpus-wide byte-exact (the leaf ceiling); `emit_c` carries the C-API veneer. |
| **Xbox 360** | Retail Tetris byte-exact (18/18 PE sections); VMX128 100% operand coverage on retail Minecraft `.text`. |

---

## 5. Doc index

- [`README.md`](../README.md) — front door / getting started.
- [`docs/NOT_IN_REPO.md`](NOT_IN_REPO.md) — the git-ignored data home (corpus, toolchains, VMs) + re-attach steps.
- [`docs/NATIVE_ASSEMBLER.md`](NATIVE_ASSEMBLER.md) — the `--masm` NASM fast path.
- Win95 Layer-2: [`L2_PIPELINE.md`](../src/targets/win95/L2_PIPELINE.md), [`WIN95_MAP.md`](../src/targets/win95/WIN95_MAP.md).
- Win95 architecture & VxD reference docs moved to the **Windows95 project repo** under `design/` (`WIN95_ARCHITECTURE_VERIFIED.md`, `win95/*`, the VxD chain: `VXD_COVERAGE.md`, `VXD_FROM_SOURCE.md`, `WIN95_VXD_TOOLCHAIN.md`) — see [`docs/NOT_IN_REPO.md`](NOT_IN_REPO.md).
- Per-engine detail: `src/engines/*/README.md` and `*/docs/`.

> Doc-debt still open (content, not paths): the `inverse` README/STATUS/ROADMAP
> describe only the 16-bit core and predate the MSC6/MSVC2/VxD tracks now in that
> directory; `recompile/BEAUTIFICATION.md` has unfilled `<<COUNT>>` placeholders and
> counts that don't reconcile with its README. Paths across all docs are current as
> of this writing.
