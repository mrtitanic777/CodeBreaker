# Rebuilding a Windows 95 VxD From Source

*A research specification for reconstructing a stripped Win95 VxD to assembly and re-linking it, with
the authentic period DDK toolchain, into a container the Win95 loader accepts and boots.*

Companion documents: [`WIN95_VXD_TOOLCHAIN.md`](WIN95_VXD_TOOLCHAIN.md) (exact tools, versions, paths,
invocations), and the engine modules `engine/inverse_engine/vxd_from_source.py` and `vxd_link.py`.

---

## Abstract

`VKD.VXD` — the Windows 95 virtual keyboard driver — is a stripped Ring‑0 Linear Executable with no
symbols and no source. This work disassembles it, re‑emits it as human‑readable DDK‑macro assembly,
and rebuilds it with the **authentic period DDK toolchain** (`ML` 6.11 → `LINK` 2.60.5046 → `RC` 3.11 →
`ADRC2VXD`) into a `.VXD` that **boots real Windows 95 to the logon prompt with a working keyboard** —
with **no bytes copied from the original binary** and **no Microsoft tool reimplemented**.

The result is significant because the barrier was never the code. Recovered code re‑assembles faithfully.
The barrier is the **container**: making a real assembler and a real linker emit an LE image the Win95
loader accepts, from reconstructed source. The requirements to do that are undocumented. This document
specifies them.

The single most important correction this document makes to earlier understanding: **there is no need to
patch container fields or reimplement any tool. The DDK ships the correct linker.** Every field the
loader requires is produced by an authentic period tool when the *right* one is used.

---

## 1. Summary of findings

A VxD reconstructed from faithful source and linked "normally" produces a *valid* LE — objects resolve,
every fixup resolves, the DDB decodes, every dispatch handler points at real code — and it **still
faults** with a diagnostic‑free `Windows protection error`. Five conditions must all hold for it to load
and initialise:

1. **Segment model.** The DDB object's recovered code must go in `_LTEXT` with the control dispatch; the
   service table and `Declare_Virtual_Device` in `_LDATA`; and the DDB object's post‑DDB **tail must be
   emitted as data**, not disassembled as code. (§4.1)
2. **Flat model.** Every custom VxD segment must be declared in the flat model
   (`<NAME> SEGMENT DWORD PUBLIC _FLAT '<NAME>CODE'`), not `USE32`. (§4.2)
3. **The correct linker.** The VxD device id (LE `0xc0`) and the version resource (`0xb8`/`0xbc`) are
   loader‑required. The VC++ 2.0 linker (**LINK 2.50**) leaves the device id blank and permutes the page
   map. The DDK's **LINK 2.60.5046** was shipped specifically to fix VxD linking: it stamps the device id
   from the exported DDB and writes an identity page map. Use it. (§4.3, §5)
4. **Identity page map.** The object page map must be the identity permutation; force it by marking every
   segment `PRELOAD NONDISCARDABLE` in the `.def`. (§4.4)
5. **Version resource.** Add the Windows version resource with the DDK's own `RC` + `ADRC2VXD`, from a
   reconstructed `.rc` — not by copying the original's resource block. (§4.5)

`LINK386`, the OS/2 linker, is **not** required and is not applicable: it targets OS/2 LX and does not
stamp a Win95 device id.

---

## 2. The VxD LE container

A Win95 VxD is an **MZ stub** whose `e_lfanew` points at an **LE** (Linear Executable) header, followed
by the tables the loader walks to map and relocate the image before any code runs.

Header fields relevant here (offsets from the LE signature):

| Offset | Field | Notes |
|---|---|---|
| `0x14` | number of logical pages | |
| `0x18` / `0x1c` | EIP object / EIP | unused for VxDs; the DDB is the entry |
| `0x28` | page size | `0x1000` for VKD; `0x200` and `0x1000` both ship (§5) |
| `0x2c` | last‑page size | valid bytes in the final logical page |
| `0x40` / `0x44` | object table offset / count | |
| `0x48` | object **page map** offset | §4.4 |
| `0x58` | resident‑name table offset | module name |
| `0x5c` | **entry table** offset | exports; the DDB is exported `@1` |
| `0x68` / `0x6c` | fixup page table / fixup record table | |
| `0x80` | data‑pages offset | where object page data begins in the file |
| `0x88` / `0x8c` | non‑resident name table offset / length | |
| `0xb8` / `0xbc` | **version resource offset / length** | §4.5 — absolute file offset |
| `0xc0` | **VxD device id** | §4.3 |
| `0xc4` | DDK version | |

**Object table entry** (24 bytes): `vsize`, `reloc_base`, `flags`, `page_map_index`, `page_count`, and a
4‑byte trailing field the linker fills with the segment's **class name truncated to 4 characters**
(`LCODE`→`LCOD`). The loader does not use that tag, but it is an exact fingerprint of the original
segment structure and is how that structure was recovered from the stripped binary.

**Object flags** (LE): `0x1` read, `0x2` write, `0x4` execute, `0x10` discardable, `0x20` shared,
`0x40` preload, `0x2000` big/USE32, `0x4000` conforming, `0x8000` VxD type bit.

**The DDB** (`VxD_Desc_Block`) lives inside a code object and is decoded by VMM:

| Offset | Field |
|---|---|
| `+0x00` | `DDB_Next` (0 at rest) |
| `+0x04` | SDK version (`0x0400`) |
| `+0x06` | Device number (VKD = `0x000D`) |
| `+0x0A` | Flags |
| `+0x0C` | Name[8] (`"VKD     "`) |
| `+0x14` | Init order |
| `+0x18` | Control proc |
| `+0x1C` / `+0x20` | V86 / PM API procs |
| `+0x30` / `+0x34` | Service table pointer / size |

The device number at DDB `+0x06` is the value the correct linker copies into LE header `0xc0`.

---

## 3. The oracle's segment structure

`VKD.VXD` is ten named segments; their 4‑character class tags survive in the object table:

```
obj  name  vsize    flags    pages  role
 1   LCOD  0x1540   0x2045   2      locked code: dispatch + functions + service table + DDB + data
 2   MCOD  0x0115   0xa055   1      message-string table (discardable, preload)
 3   ICOD  0x08d0   0x2015   1      init code (discardable)
 4   PCOD  0x06ac   0x2005   1      pageable code
 5   SYSE  0x0126   0x2005   1      VKD-specific
 6   RARE  0x044c   0x2005   1      rarely-used code
 7   VMCR  0x010d   0x2005   1      VM-create handler
 8   VMDE  0x00ca   0x2005   1      VM-destroy handler
 9   VMSU  0x0010   0x2005   1      VM-suspend handler
 10  DOSV  0x0128   0x2005   1      DOS-box handler
```

`LCOD`/`ICOD`/`PCOD` are the standard DDK classes (`LCODE`/`ICODE`/`PCODE`); the other seven are VKD's
own custom segments, created with the DDK's `MakeCodeSeg` macro.

### obj1 internal layout

`LCOD` (obj1) is two DDK segments merged by the linker into one `LCODE` object:

```
_LTEXT : [ control dispatch ][ ...locked functions... ]
_LDATA : [ service table ][ DDB ][ locked data + jump table + BSS tail ]
```

The original's on‑object order is `[dispatch][code][service][DDB][data]`, with the DDB at obj1 offset
`0x11bc`. Reproducing that exact order depends on the linker's same‑class ordering rule (§5), which is
the one respect in which the authentic DDK linker diverges from the linker that built the shipped binary.

---

## 4. The five requirements

### 4.0 Why it faults at all

A from‑source VxD linked normally produces a valid LE that still faults. The reconstructed code is
faithful — differences from the original are only commutative re‑encodings (SIB base/index order
`[esi+ebx] ≡ [ebx+esi]`, `test`/`xchg` operand order). The fault is entirely in how the container is laid
out and which container fields are present.

### 4.1 Segment model

Emitting recovered code into synthetic per‑object segments (`VSEG01…VSEG10`) causes the linker to order
`_LTEXT`(dispatch) → `_LDATA`(service+DDB) → `VSEG01`(code): the service table and DDB float to the front
of obj1 where the original has code. Fix: emit obj1's code into `_LTEXT` (`VxD_LOCKED_CODE_SEG`) with the
dispatch; let the service‑table and `Declare_Virtual_Device` macros place those in `_LDATA`.

There is a second placement bug inside obj1. The tail of `LCOD` past the DDB is **data** (mostly zeros
plus one relocated jump table), but a recursive‑descent disassembler over‑reads it as code (procedures
that straddle the DDB). Emitted as code, it inserts ~816 bytes ahead of `_LDATA` and displaces the DDB.
Fix: classify the post‑DDB tail as data and emit it into `_LDATA` after the DDB.

### 4.2 Flat model vs USE32 for custom segments

VKD's seven custom segments must be declared by the reconstruction. Declare them in the flat model:

```asm
<NAME> SEGMENT DWORD PUBLIC _FLAT '<NAME>CODE'
```

where under MASM 6, `_FLAT EQU FLAT`. The flat‑group attribute is mandatory. Declared `USE32` instead,
the segment is not part of the flat model; every cross‑segment `call`/`jmp` becomes an implicit far
reference and the assembler fails with **A2107** ("cannot have implicit far jump or call to near label").
This is exactly what the DDK's `MakeCodeSeg` macro expands to; declaring the segment directly with
`_FLAT` and class `'<NAME>CODE'` is equivalent and reproduces the object's 4‑character class tag.

### 4.3 The device id and the correct linker

LE header `0xc0` holds the VxD device id (VKD = `0x000D`), equal to the DDB device number. It is
loader‑required. Its presence is entirely a function of **which linker** builds the image:

* **LINK 2.50** (the VC++ 2.0 linker, `msvc-2.0/BIN/LINK.EXE`) leaves `0xc0` at `0`. A module with a
  zero device id is rejected.
* **LINK 2.60.5046** (`Microsoft Windows 95 DDK\MSVC20\LINK.EXE`) stamps `0xc0` from the exported
  `<NAME>_DDB`. The DDK shipped this linker for exactly this reason. Its `README.TXT`:

  > "This new linker should only be used by ddk developers experiencing problems with the MS VC++ 2.0
  > linker w.r.t. linking VxDs."

The `.def` exports the DDB as ordinal 1 (`EXPORTS  <NAME>_DDB @1`) and declares the module a VxD
(`VXD <NAME>`); the linker reads the DDB through that export and copies the device number into `0xc0`.
No sample sets a numeric device id in the `.def`; the id always derives from the DDB.

**Operational note.** LINK 2.60's `/VXD` mode re‑executes the linker to build the exports file, and it
does so **without quoting its own module path**. Because the DDK's install path contains spaces, the
self‑spawn shatters (`…\Microsoft Windows 95 DDK\… → Windows.obj`, error LNK1181/LNK1141). Run the linker
from a space‑free directory (stage `LINK.EXE`, `DBI.DLL`, `LINK.ERR`, `IMAGEHLP.DLL` into one). See
[`WIN95_VXD_TOOLCHAIN.md`](WIN95_VXD_TOOLCHAIN.md).

### 4.4 The identity page map

The LE object page map (`hdr+0x48`) maps each logical page to a physical page in the file; entries are
4 bytes (a 24‑bit big‑endian page number plus a flag byte). The original's page map is the identity
permutation: logical page *N* is physical page *N*.

To let the OS discard init/pageable memory as a unit, a linker groups discardable segments' pages at the
end of the file and records their logical positions — e.g. `[1,2,10,11,3,4,5,6,7,8,9]`. That permutation
is legal LE, but **fatal to a Win95 static VxD**: the static loader reads object pages by *logical slot*
and ignores the physical remap, so a permuted map loads each object from the wrong bytes, and VMM faults
during init.

Fix: force an identity map by marking every segment `PRELOAD NONDISCARDABLE` in the `.def`. The linker
then writes pages in logical order. The cost is that init/message code stays resident (a few KB of RAM).
Do not attempt to repair a permuted map by hand: the last logical page is partial (`hdr+0x2c` valid
bytes), and moving it as a full page sweeps up the trailing name and resource tables.

### 4.5 The version resource

LE header `0xb8`/`0xbc` point at the Windows version resource (a ~0x1e8‑byte block: company, description,
file version, copyright). It is loader‑required. Add it with the DDK's own tools, per the DDK's
`BASE\VXDRES\MAKEFILE`:

```
rc -r <name>.rc            # RC 3.11 compiles the .rc to a 16-bit (ANSI) .res
adrc2vxd <name>.vxd <name>.res   # DDK tool embeds the resource, sets hdr 0xb8/0xbc
```

The `.rc` is reconstructed source (§7): the device id and internal name come from the recovered DDB;
the human‑authored strings (file description, copyright years) are recovered from the original's resource
string table and recompiled — metadata, not transplanted bytes. `RC` must be the **16‑bit DOS RC 3.11**
(`msvc-1.52c`), which emits ANSI resources; a Win32 RC emits UTF‑16, which `ADRC2VXD` rejects. `RC`
requires its preprocessor `RCPP.EXE` alongside it, and the DDK `INC16` headers (`version.h`,
`common.ver`, `ver.h`) on the include path.

---

## 5. LINK 2.60 vs the shipping linker — the segment‑ordering finding

Using the DDK's LINK 2.60.5046, the container diverges from the original in exactly one structural respect
beyond the deliberate `NONDISCARDABLE` choice: **the order of same‑class segments within an object.**

**LINK 2.60 orders segments of the same class alphabetically by segment name**, overriding both source
definition order and `.def` order. This was established by controlled experiment: with two `LCODE`
segments, reversing their `.def` order changed nothing, and a code segment *defined first* but *named to
sort later* (`_ZCODE`) still linked *after* a data segment named `_ADATA`.

Consequence for obj1: its two `LCODE` segments are `_LTEXT` (dispatch + code) and `_LDATA` (service
table + DDB + tail). Because `"_LDATA" < "_LTEXT"`, LINK 2.60 places `_LDATA` first, moving the DDB to
obj1 offset `0x58`. The linker that built the **1995 shipping** `VKD.VXD` preserved definition order,
keeping code first with the DDB at `0x11bc`.

Both images boot: the loader locates the DDB through its `@1` export, not by offset. But it means a
from‑source VxD built with the authentic DDK linker is **functionally faithful, not byte‑identical** in
obj1 layout — an inherent difference between the 1996 DDK's LINK 2.60 and the 1995 shipping linker, not a
defect. Because the DDK service‑table and `Declare_Virtual_Device` macros place their output in `_LDATA`
by name, and `_LDATA` always sorts before `_LTEXT`, this ordering cannot be changed without abandoning the
DDK macros for hand‑emitted tables in a differently‑named segment.

Divergences from the original, complete:

| Aspect | Shipping VKD | DDK LINK 2.60 `/VXD` | Consequence | Handling |
|---|---|---|---|---|
| Device id `@0xc0` | device number | device number (from DDB) | — | native |
| Version resource `@0xb8` | present | absent at link; added by `ADRC2VXD` | — | §4.5 |
| Page map | identity | identity (with `NONDISCARDABLE`) | — | §4.4 |
| Same‑class segment order | definition order | **alphabetical by name** | obj1 DDB at `0x58` vs `0x11bc`; still boots | inherent |
| Page size (`/ALIGN`) | `0x1000` | `0x200` default; `0x1000` via `/ALIGN:0x1000` | none (both ship) | `/ALIGN:0x1000` |
| Fixup record byte order | its own | its own | none (loader is order‑independent) | — |

Object bytes, fixup targets, DDB contents, service table, and entry table all come out correct from
faithful source.

---

## 6. The pipeline

```
stripped VKD.VXD (LE)
   │  recursive-descent disassembly (seed from DDB entry points; classify data vs code)
   ▼
idiomatic DDK-macro .ASM
   │   • obj1 code → VxD_LOCKED_CODE_SEG (_LTEXT), with the control dispatch
   │   • service table + Declare_Virtual_Device → _LDATA (macros)
   │   • obj1 post-DDB tail → _LDATA as data
   │   • obj2..N → '<NAME> SEGMENT DWORD PUBLIC _FLAT "<NAME>CODE"'  (flat model)
   ▼
ML.EXE 6.11  -coff  →  .OBJ                                         [DOSBox]
   │   .def: every SEGMENTS entry  PRELOAD NONDISCARDABLE  (identity page map + oracle object order)
   ▼
LINK.EXE 2.60.5046  /VXD /NOD /ALIGN:0x1000  →  .VXD                [native, space-free dir]
   │   device id (0xc0) stamped from the exported <NAME>_DDB; identity page map; 4096-byte pages
   ▼
RC.EXE 3.11  -r <name>.rc  →  .RES                                  [DOSBox]
ADRC2VXD.EXE <name>.vxd <name>.res  →  version resource embedded    [DOSBox]
   ▼
loadable VKD.VXD  →  boots Win95 to logon; keyboard live
```

Every stage is an authentic period tool. No container field is hand‑patched; no tool is reimplemented.

---

## 7. Provenance

The code, DDB, service table, fixups, and object bytes are 100% from the recovered source; only
commutative re‑encodings differ from the original. The version resource is likewise rebuilt from
recovered source: the `.rc` is reconstructed (device id and internal name from the DDB; file description
and copyright years recovered from the original's resource *strings* — metadata, not code) and the DDK's
real `RC` + `ADRC2VXD` recompile and embed it. Nothing is transplanted from the original binary, and no
Microsoft tool is reimplemented.

The engine reproduces the booted artifact deterministically: `build_from_source(v)` in
`engine/inverse_engine/vxd_from_source.py` returns the exact bytes that were injected and booted.

---

## 8. The boot ladder

Each requirement was isolated by a real boot in VMware, because the loader gives no diagnostics — only
`Windows protection error`. The progression is the proof:

| Build | Change | Result | Lesson |
|---|---|---|---|
| real‑segment obj1 | `_LTEXT`/`_LDATA` + tail→`_LDATA` | valid LE; **faults** | the fault is not obj1 content |
| + device id + version resource | present via fixups (early method) | **splash**, then triple‑fault | both fields are loader‑required |
| + object order | discardable segments preloaded | clean **"While initializing device VKD"** | VMM's named error appears only when the container is almost right |
| + identity page map | all segments `NONDISCARDABLE` | **logon** (LINK 2.50 + device‑id patch) | the permuted page map was loading objects as garbage |
| **authentic toolchain** | **LINK 2.60 device id + RC/ADRC2VXD resource** | **logon; keyboard live** | the DDK's own linker + resource tools replace every patch |

The intermediate "While initializing device VKD" is diagnostic gold: the moment the container is almost
correct, the fault moves from a silent early reject to VMM's named init‑error handler, which finally
reports *which* device and *which* phase.

---

## 9. Why this is not common knowledge

VxD development in its heyday used the exact toolchain the header fields were designed for. Nobody had to
reverse the loader's tolerances because the correct linker produced conforming output. The confusion this
work resolves is a **toolchain‑version** problem: the VC++ 2.0 linker (LINK 2.50) is the one most readily
available and it *looks* like it works — it produces a valid LE — but it silently omits the device id and
permutes the page map, and the failure is a diagnostic‑free `Windows protection error` that can mean
"container rejected" or "init code was garbage." The fix is not to patch around the linker but to use the
one the DDK shipped for the job, **LINK 2.60.5046**, which is present in the DDK and runs natively on a
modern 64‑bit host.

Stated plainly for anyone who comes after:

> A reconstructed Win95 VxD linked from faithful source boots when it is built with the DDK's own
> **LINK 2.60** (which stamps the device id from the exported DDB and writes an identity page map, aided
> by a `PRELOAD NONDISCARDABLE` `.def`) and given its version resource by the DDK's own **RC + ADRC2VXD**.
> No container field needs patching and no tool needs reimplementing. The one thing you cannot reproduce
> with the DDK linker is the shipped binary's exact obj1 byte layout, because LINK 2.60 orders same‑class
> segments alphabetically where the 1995 linker preserved definition order — the image is functionally
> faithful, not byte‑identical.
