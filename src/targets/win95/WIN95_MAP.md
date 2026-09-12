# Windows 95 structure — a navigation map for the L2 reconstruction

Why this doc: the Layer-2 pipeline (`classify32` → `emit_c` / `emit_cpp` → `ml32`
floor) routes every function of every module. *Which* route a module's functions
land on is not arbitrary — it follows Win95's actual architecture. This maps that
architecture onto our routes, so a sweep's shape is predictable and the yield
levers are obvious. **Every architectural claim below is cross-validated against four
primary Microsoft sources (+ byte-exact binary confirmation) in
`WIN95_ARCHITECTURE_VERIFIED.md` (in the Windows95 project repo,
`GitHub/Windows95/design/WIN95_ARCHITECTURE_VERIFIED.md`, with its `win95/*` sub-docs) — no
assumptions.** Authoritative source: Logan's cleaned Win95 SDK/DDK design doc
`GitHub/Windows95/design/Win95Design.md`; background in the reference books.

**Authoritative Win32 functional taxonomy** (from the design doc — use it for the
`l2predict`/`bsweep` tiers instead of ad-hoc grouping): **GDI** (`gdi32`) · **Window
Management** (`user32`) · **System Services** (`kernel32`, `advapi32` — memory / file /
process / DLL / registry) · **Multimedia** (`winmm`, `msvfw32`) · **RPC** (`rpcrt4`).
Extension libs: Common Controls (`comctl32`) · Common Dialogs (`comdlg32`) · Compression
(`lz32`) · DDEML. High-level: **OLE/COM** (`ole32`, `oleaut32`) · TAPI · MAPI · Pen. The
16↔32 seam is the **Thunk Compiler**; Win32→ring-0 is **IOCTL/DeviceIoControl** + VxD
service calls.

## The two worlds (Schulman's thesis)

Win95 is not one 32-bit OS; it is **two architecturally separate components**:

1. **The ring-0 OS** — `VMM32.VXD` (the real kernel) + a stack of **VxDs**, with a
   built-in **DPMI DOS extender**. Memory, scheduling, interrupts, I/O, the
   installable file system. This is where "DOS" actually lives: most `INT 21h`
   calls never reach real-mode DOS — they are serviced by 32-bit protected-mode
   VxDs (`IFSMGR`, `IOS`). Only a small residue hits pure real-mode code.
2. **The Windows GUI** — the ring-3 module tiers below, riding on top of that OS.

The **32-bit claim is a marketing exaggeration**: the GUI makes constant 16-bit
calls on behalf of Win32 apps, serialized behind the **Win16Mutex** (née
Win16Lock). That 16↔32 traffic is **thunking**, and it is a first-class citizen
of the binary, not an edge case.

## Ring 3 module tiers → our routes

| Module(s) | What it is | PE | Dominant L2 route |
|---|---|---|---|
| `KERNEL32.DLL` | Win32 kernel API; forwards heavy lifting to `VWIN32.VXD` | 32-bit | **c** (forwarders/wrappers) + human logic |
| `USER32.DLL` | Win32 windowing/messaging; thunks down to 16-bit `USER.EXE` | 32-bit | **c** + thunk stubs |
| `GDI32.DLL` | Win32 graphics; thunks down to 16-bit `GDI.EXE` | 32-bit | **c** + thunk |
| `ADVAPI32`, `COMCTL32`, `COMDLG32`, `SHLWAPI` | registry / common-controls / dialogs | 32-bit | **c** + human |
| **`OLE32`, `OLEAUT32`, `SHELL32` + shell extensions, file viewers/parsers** | **COM in-process servers (`InProcServer32`): objects with VTABLES of `__thiscall` methods** | 32-bit | **cpp** (vtable/virtual `__thiscall`) + human |
| **`MFC40.DLL`, `MFCO40`, `MFC30`** | **C++ app framework, exported by ordinal** | 32-bit | **cpp** (`__thiscall` methods) + human |
| `KRNL386.EXE`, `USER.EXE`, `GDI.EXE` | the **16-bit** Win16 kernel/GUI | 16-bit NE | **not this pipeline** — cc16 path |
| `VMM32.VXD`, `*.VXD` | ring-0 kernel + drivers (LE/LX) | VxD | thunk / `vxd_idiom` symbolization |

Key consequence for the sweep:
- **`emit_c` territory** = the 32-bit C DLLs (`kernel32`/`user32`/`gdi32`/…). Their
  bulk is thin API surface: const-returns, forwarders to a VxD/16-bit worker,
  arg-marshalling wrappers — exactly the mechanical classes `emit_c` models.
- **`emit_cpp` territory** = MFC **and all of COM**. The design doc makes explicit that
  OLE, `ole32`/`oleaut32`, shell extensions, and file viewers/parsers are **COM
  in-process servers** — objects whose methods are dispatched through a **vtable** with
  `this` in `ecx`. So the C++ `__thiscall` population is far larger than MFC alone, and
  the **vtable/virtual-dispatch** shape (`mov eax,[ecx]; call [eax+off]`) is a *first-
  class* target, not a deferred edge case. MFC methods are unnamed `sub_<rva>` (ordinal-
  exported, 0 mangled names) reachable only by `__thiscall` modeling; COM objects add a
  large, vtable-structured cohort. This re-prioritizes `emit_cpp` v3 (virtual `__thiscall`)
  — corroborated by the fused-corpus data (`struct`/`effect` already carry `ole32`/`shell32`).
- **thunk stubs** (`classify32` route `thunk`: `mov cl/cx, <idx>` …) are the
  visible 16↔32 boundary — the flat/generic thunk entry points. Mechanical asm.
- **16-bit modules** are a different binary format (NE) and a different oracle
  (cc16), out of the 32-bit L2 path entirely.

## Thunking (why so many stubs)

- **Flat thunks** — the Win95 16↔32 bridge inside a cooperating module pair
  (`gdi32`↔`gdi`, `user32`↔`user`). Small fixed-shape stubs. Route `thunk`.
- **Generic thunks** — the documented cross-process/other-direction path
  (`*ThunkConnect*`, callbacks). More structure; some land `human`.
- The mutex serialization (Win16Mutex) is behavioral, not shape — it doesn't
  change how a function reconstructs, but it explains the pervasive
  `Enter/LeaveSysLevel`-style call pairs around GUI forwarders.

## The ring-0 side (VMM / VxD) — the internals half

Not a Win32-PE reconstruction problem, but it frames the imports the DLLs call:
- `VWIN32.VXD` — the ring-0 primitives `KERNEL32` leans on (thread create, block
  on a sync object, new memory context) via **Win32 VxD service calls**.
- `VMM` proper + `IFSMGR`/`IOS` (file system), `CONFIGMG` (Plug-and-Play),
  `VMCPD`, `VPICD`, etc. — the service tables `vxd_idiom` symbolizes.
- Reference depth for this layer: Pietrek (VMM/thunking/thread internals),
  Schulman (DOS↔protected-mode boundary), Oney (writing VxDs / Ring-0 env). App-
  level API naming/typing for the ring-3 exports: Petzold. See
  [[win95-internals-refs]].

## Byte-level anatomy (grounded in the corpus)

The architecture is visible in the opcodes. What the pipeline actually sees:

**The shared arena.** Win95 maps the system DLLs and the 16-bit world into the
shared address region above ~3 GB (`0xBFFxxxxx`). `kernel32` itself bases at
**`0xBFF70000`**. So in a `kernel32` function:
- `call 0xbff7169c` — an intra-`kernel32` call (target inside its own image).
- `call ptr cs:[0xbffbc004]` — a **far call through a fixed thunk-table slot** in
  the arena. This is the **flat thunk down to 16-bit `KRNL386`**: the stub shuffles
  the return address (`mov eax,[esp+4]; pop [esp]`) and far-calls the 16↔32
  transition entry. A `0xBFFB….` indirect target = "this call leaves 32-bit land."

**kernel32 forwarder = API veneer over a worker.** e.g. `Ord43`: frame, re-push
the caller's args (`push [ebp+0x18]…[ebp+8]`), `call <worker>`. That is exactly
`emit_c._forward_call`'s model — a thin `__stdcall` forward. The "worker" is often
another arena address (16-bit thunk) or a `VWIN32.VXD` service.

**VxD service calls (ring-3 → ring-0).** `KERNEL32` primitives (thread create,
wait on a sync object, commit memory) don't implement themselves — they issue a
**Win32 VxD service call** into `VWIN32`/`VMM`. On the wire that's an `int 2Eh`/
`int 30h`-style transition or a fixed indirect through the service dispatch. When a
forwarder's tail is an indirect `call`/`jmp` to a non-IAT arena slot, that's the
ring-0 boundary — correctly left on the asm floor, not faked as C.

**MFC `__thiscall` method, the dominant shape** (`this` in `ecx`):
- `push esi ; mov esi, ecx` — **save `this`** into a callee-saved register so it
  survives the calls. Seeing `mov esi,ecx` at entry is the tell.
- `mov eax, dword ptr [esi]` — load the **vtable pointer** (first dword of the
  object).
- `call dword ptr [eax + 0xa4]` — **virtual dispatch**: call the method at vtable
  offset `0xa4`.
- `mov ecx, esi ; call <sub>` — **re-pass `this`** to a non-virtual member call.
Corpus counts (mfc40, ≤120 b, thiscall + ≥1 call): ~402 two-call `this`-savers,
~284 single-call+return, ~277 three-call — the `emit_cpp` **v2** target set. v2
must (a) declare the callee as a `__thiscall`/`__stdcall` method, (b) model
`this->vtable[off]` for the virtual-dispatch shape, (c) type args as `this`-fields
or incoming args. Harder than v1's straight-line getters/setters; same oracle gate.

## Reading a sweep with this map

- A 32-bit C DLL with a **high `c`-route fraction** = healthy: it's mostly API
  veneer. Low clean-C there means the *modelers* are missing a shape, not that the
  module is intrinsically hard.
- An MFC/C++ module with most functions on the **ASM floor** = expected until
  `emit_cpp` grows: the yield lever is `__thiscall` method classes (getters/
  setters done in v1; forwarders/multicall are v2).
- A pile of tiny `thunk`-route stubs = the 16↔32 boundary; mechanical, already
  covered by the asm floor.
