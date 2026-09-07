# Booting a Reconstructed Windows 95 VxD From Source

*The undocumented container requirements that make a re-linked Win95 VxD actually load.*

---

## TL;DR

We took `VKD.VXD` (the Win95 virtual keyboard driver), a **stripped Ring-0 LE binary** with no
symbols and no source, disassembled it, re-emitted it as **human-readable DDK-macro assembly**,
assembled it with the period **ML.EXE** and linked it with a period **MS LINK**, and **booted the
result in real Windows 95 to the logon prompt** — with **zero bytes copied from the original**.

That last clause is the point. It is easy to make a VxD "boot" by transplanting the original's
container and dropping recovered code into it. Making the *toolchain itself* — assembler + linker —
emit a container the Win95 loader accepts, from reconstructed source, is a different problem, and the
requirements to do it are **not documented anywhere**. This is that documentation.

The four things that matter, none of which the DDK or any reference we found spells out:

1. **The segment model must match**, down to which recovered bytes land in `_LTEXT` vs `_LDATA`.
2. **Custom VxD segments must be declared in the flat model (`_FLAT`), not `USE32`.**
3. **The loader requires two container fields a modern `LINK /VXD` leaves blank:** the VxD device id
   and the Windows version resource.
4. **The object page map must be an identity map.** The Win95 static VxD loader reads object pages by
   *logical slot* and ignores a permuted map — and modern linkers permute it by grouping discardable
   pages.

Get those wrong and you get one of two failure modes, both of which read as
`Windows protection error` on a black screen with no diagnostics: a load-time rejection, or (more
insidiously) a garbage-page init crash after the splash screen.

---

## 1. Two reconstruction paths, and why "from source" is the hard one

There are two ways to rebuild a loadable VxD from a recovered disassembly:

| Path | How | What it proves |
|---|---|---|
| **Byte-exact writer** | Synthesize the LE container in code (a Python `build_le`), placing recovered object bytes and fixups directly. | The *bytes* are recovered correctly. |
| **From source** | Emit `.ASM`, run `ML` → `LINK /VXD`. | The *source* is recovered correctly — the reconstruction is a real program a real toolchain compiles into a working driver. |

The writer path works and is valuable, but it sidesteps the toolchain: it transplants the original's
exact container layout and drops code into it. The from-source path is the real test of a
reconstruction, and it fails in ways the writer never exposes, because **the linker, not you, lays out
the container** — and a period-substitute linker lays it out differently from the one Microsoft used.

Everything below is about closing the gap between "a valid LE that a modern linker produces" and "the
LE the Win95 loader accepts."

---

## 2. The VxD LE container, in brief

A Win95 VxD is an **MZ stub** whose `e_lfanew` points at an **LE** (Linear Executable) header. The LE
header is followed by tables that the loader walks to map and relocate the image before any code runs.

Header fields that matter here (offsets from the LE signature):

| Offset | Field | Notes |
|---|---|---|
| `0x14` | # logical pages | |
| `0x18` / `0x1c` | EIP object # / EIP | For VxDs, effectively unused; the DDB is the entry. |
| `0x28` | page size | `0x1000` |
| `0x2c` | last-page size | valid bytes in the final logical page |
| `0x40` / `0x44` | object table offset / count | |
| `0x48` | object **page map** offset | **critical — see §5** |
| `0x58` | resident-name table offset | the module name lives here |
| `0x5c` | **entry** table offset | exports; the DDB is exported `@1` |
| `0x68` / `0x6c` | fixup page table / fixup record table | |
| `0x80` | data-pages offset | where object page data begins in the file |
| `0x88` / `0x8c` | non-resident name table offset / length | |
| `0xb8` / `0xbc` | **Windows version resource offset / length** | **required — see §4** |
| `0xc0` | **VxD device id** | **required — see §4** |
| `0xc4` | DDK version | |

**Object table entry** (24 bytes): `vsize`, `reloc_base`, `flags`, `page_map_index`, `page_count`,
and a 4-byte trailing field the VxD linker fills with the segment's **class name truncated to 4 chars**
(`LCODE`→`LCOD`). This 4-char tag is *not* used by the loader (we proved this — patching it changes
nothing), but it is a perfect fingerprint of the segment structure, which is how we recovered it.

**The DDB (VxD_Desc_Block)** lives inside a code object and is decoded by the loader/VMM:

| Offset | Field |
|---|---|
| `+0x00` | `DDB_Next` (link, 0 at rest) |
| `+0x04` | SDK version (`0x0400`) |
| `+0x06` | Device_Number (VKD = `0x000D`) |
| `+0x0A` | Flags |
| `+0x0C` | Name[8] (`"VKD     "`) |
| `+0x14` | Init_Order |
| `+0x18` | Control_Proc |
| `+0x1C` / `+0x20` | V86 / PM API procs |
| `+0x30` / `+0x34` | Service_Table_Ptr / _Size |

---

## 3. The oracle's real segment structure (recovered from the object table)

`VKD.VXD` is not one blob. It is **ten named segments**, and their 4-char class tags survive in the
stripped binary's object table:

```
obj  name  vsize    flags    pages   role
 1   LCOD  0x1540   0x2045   2       locked code: dispatch + functions + service table + DDB + data
 2   MCOD  0x0115   0xa055   1       message-string table (discardable, preload, +0x8000)
 3   ICOD  0x08d0   0x2015   1       init code (discardable)
 4   PCOD  0x06ac   0x2005   1       pageable code
 5   SYSE  0x0126   0x2005   1       VKD-specific
 6   RARE  0x044c   0x2005   1       rarely-used code
 7   VMCR  0x010d   0x2005   1       VM-create handler
 8   VMDE  0x00ca   0x2005   1       VM-destroy handler
 9   VMSU  0x0010   0x2005   1       VM-suspend handler
 10  DOSV  0x0128   0x2005   1       DOS-box handler
```

`LCOD`/`ICOD`/`PCOD` are the standard DDK classes (`LCODE`/`ICODE`/`PCODE`). The other seven are
**VKD's own custom segments**, created with the DDK's `MakeCodeSeg` macro.

Object flag bits (LE): `0x1` read, `0x2` write, `0x4` exec, `0x10` discardable, `0x40` preload,
`0x2000` USE32(big), `0x8000` (VxD "IOPL"/type bit; cosmetic for loading).

### obj1's internal layout is the crux

`LCOD` (obj1) is **two DDK segments merged by the linker into one LCODE object**:

```
_LTEXT : [ control dispatch ][ ...all the locked functions... ]
_LDATA : [ service table ][ DDB ][ locked data + a jump table + BSS tail ]
```

giving the on-object order `[dispatch][code][service][DDB][data]`. The linker merges same-class
(`'LCODE'`) segments in `.def` order, so `_LTEXT` (code) precedes `_LDATA` (service+DDB+data).

**This is the single most important structural fact, and getting it wrong is the first way a
from-source build fails** (§4.1).

---

## 4. Fix set — what the loader actually requires

### 4.0 Why it faults at all

A from-source VxD linked "normally" produces a *valid* LE — objects resolve, all fixups resolve, the
DDB decodes, every control-dispatch handler points at real code — and it **still faults**. We proved,
object by object, that the reconstructed code is faithful (differences are only commutative
re-encodings: SIB base/index order `[esi+ebx]≡[ebx+esi]`, `test`/`xchg` operand order). The fault is
entirely in **how the linker lays out the container** versus how Microsoft's linker did.

### 4.1 Segment model: emit each object into its *real* segment

The mistake that dooms most attempts: emitting recovered code into synthetic per-object segments
(`VSEG01 … VSEG10`) to control object boundaries. The linker then orders
`_LTEXT`(dispatch) → `_LDATA`(service+DDB) → `VSEG01`(code), i.e. `[dispatch][service][DDB][code]` —
the service table and DDB float to the *front* of obj1 (page 1), where the original has code, and the
DDB lands ~0x340 bytes early.

**Fix:** emit obj1's recovered code into `_LTEXT` (`VxD_LOCKED_CODE_SEG`) *with* the dispatch; let the
service-table and `Declare_Virtual_Device` macros put those in `_LDATA`. Emit each other object into a
segment carrying its real class.

There is a second, subtler placement bug inside obj1. The tail of `LCOD` (past the DDB) is **data** —
mostly zeros plus one relocated jump table — but a recursive-descent disassembler over-reads it as
code (procedures that *straddle* the DDB). Emitted as code in `_LTEXT`, it inserts ~816 bytes ahead of
`_LDATA` and shoves the DDB from its true offset (`0x11bc`) to `0x14f0`. **Fix:** classify the
post-DDB tail as data and emit it into `_LDATA` after the DDB. With both fixes, obj1 is byte-exact bar
the commutative re-encodings, and the DDB is back at `0x11bc`.

### 4.2 The `_FLAT` vs `USE32` gotcha (custom segments)

VKD's seven custom segments are not standard DDK segments, so the reconstruction must declare them.
The DDK declares its own flat segments as:

```asm
_LTEXT  SEGMENT DWORD PUBLIC _FLAT 'LCODE'
```

where, under MASM6, `_FLAT EQU FLAT`. **The `_FLAT` (flat-group) attribute is mandatory.** Declare a
custom VxD segment as `USE32` instead and it is *not* part of the flat model; every cross-segment
`call`/`jmp` becomes an implicit far reference and the assembler dies with **A2107**
("cannot have implicit far jump or call to near label"). This is exactly what the DDK's `MakeCodeSeg`
macro expands to; declaring the segment directly with `_FLAT` and the class `'<NAME>CODE'` is
equivalent and reproduces the object's 4-char name tag.

### 4.3 Device id and version resource (the linker leaves them blank)

Microsoft's `LINK386` wrote two VxD-specific container fields that a modern `LINK /VXD` does **not**:

* **`0xc0` — VxD device id.** Left `0`; must be the DDB's device number (VKD = `0x000D`).
* **`0xb8`/`0xbc` — Windows version resource.** Left `0`; the original carries a ~0x1e8-byte block
  (`anyName\0Microsoft…FileVersion 4.00.950…`).

Both are **loader-required**: without them the module is rejected before any code runs. If the
substitute linker can't emit them, they must be supplied by a small post-link fixup (patch `0xc0`,
append the version resource, point `0xb8/0xbc` at it). This is container metadata, not code — the code
is still 100% from source.

### 4.4 The identity page map (the one nobody knows)

This is the finding that took the longest and is the most surprising.

The LE **object page map** (`hdr+0x48`) maps each *logical* page to a *physical* page in the file. Its
entries are 4 bytes: a 24-bit **big-endian** page number plus a flag byte.

The original's page map is the **identity** permutation: logical page *N* is physical page *N*.

A modern `LINK /VXD` **permutes it.** To let the OS discard init/pageable memory as a unit, the linker
groups **discardable** segments' pages at the *end* of the file and records their logical positions in
the page map — e.g. `[1,2,10,11,3,4,5,6,7,8,9]`.

That permutation is legal LE. It is also **fatal to a Win95 static VxD**, because the Win95 static VxD
loader **reads object pages by logical slot and ignores the page map's physical remap.** So logical
page 3 (obj2/`MCOD`) and logical page 4 (obj3/`ICOD`) get loaded from whatever physical bytes sit in
slots 3 and 4 — a *different* object's page. The init code (`ICOD`) loads as garbage, and VMM faults
with `While initializing device VKD: Windows protection error`.

**Fix:** force an identity page map. The clean way is to make the linker not group discardable pages —
mark every segment **`PRELOAD NONDISCARDABLE`** in the `.def`. The linker then writes pages in logical
order, yielding an identity map (and, as a bonus, it stops relocating the discardable `ICOD` to the end
of the object table, so the object order matches too). The cost is that init/message code stays
resident instead of being discarded after boot — a few KB of RAM, and it boots. (Object flags do not
gate loading; we verified this independently.)

Do **not** try to fix a permuted map by hand-rearranging the data pages: the **last logical page is
partial** (`hdr+0x2c` bytes), and moving a partial page as if it were full sweeps up the non-resident
name table and the version resource that follow it, corrupting everything. Force identity at link time
instead.

---

## 5. LINK 2.50 vs LINK386 — the differences that matter

Using a period MS LINK (2.50, from VC++ 2.0, which supports `/VXD` and unlike the DDK's LINK 2.60 does
not need an external `LIB.EXE`) as a stand-in for the era's `LINK386`, the container diverges from the
original in exactly these ways — and *only* these:

| Aspect | LINK386 (original) | Modern LINK /VXD | Consequence | Fix |
|---|---|---|---|---|
| Device id `@0xc0` | device number | `0` | load reject | post-link patch |
| Version resource `@0xb8` | present | absent | load reject | post-link patch |
| Page map | identity | discardable-grouped permutation | init garbage-fault | `.def` `NONDISCARDABLE` |
| Discardable object order | in place | relocated to end | (cosmetic once map is identity) | `.def` `NONDISCARDABLE` |
| data-pages align | sector (`0x200`) | page (`0x1000`) | none (self-describing) | — |
| Fixup record byte-order | its own | its own | none (loader is order-independent) | — |

Everything else — object bytes, fixup targets, DDB, service table, entry table — comes out correct
from faithful source.

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
   │   • obj2..N → '<NAME> SEGMENT DWORD PUBLIC _FLAT "<NAME>CODE"'  (flat model!)
   ▼
ML.EXE  -coff  →  .OBJ
   │   .def: every SEGMENTS entry  PRELOAD NONDISCARDABLE   (identity page map + oracle order)
   ▼
LINK /VXD  →  .VXD
   │  post-link container fixup:
   │   • device id  → hdr 0xc0
   │   • append Windows version resource, point hdr 0xb8/0xbc at it
   ▼
loadable VKD.VXD  →  boots Win95 to logon
```

---

## 7. How each requirement was pinned (the boot ladder)

Every fix was isolated by a real boot in VMware, because the loader gives *no* diagnostics — just
`Windows protection error`. The progression is the proof:

| Build | Change | Result | Lesson |
|---|---|---|---|
| real-segment obj1 | `_LTEXT`/`_LDATA` + tail→`_LDATA` | obj1 byte-exact, DDB @0x11bc; **still faults** | the fault is *not* obj1 content |
| + device id + version resource | patch `0xc0`, `0xb8/0xbc` | reaches the **splash screen**, then triple-faults | those two fields are loader-required |
| + object order | `ICOD` PRELOAD | clean **"While initializing device VKD"** | order affects how VMM's error handler reports |
| + identity page map | all segments `NONDISCARDABLE` | **LOGON** | the permuted page map was loading ICOD as garbage |

The middle step is diagnostic gold: the moment the container is *almost* right, the fault moves from a
silent early reject to VMM's named init-error handler, which finally tells you *which device* and
*which phase*.

---

## 8. Why this is not common knowledge

VxD development in its heyday used the exact toolchain the header fields were designed for; nobody had
to reverse the loader's tolerances because `LINK386` simply produced conforming output. Modern VxD work
is almost entirely *analysis* (disassembly, patching existing binaries), not *reconstruction to a
booting driver through a substitute toolchain*. The failure signature — a diagnostic-free
`Windows protection error` that can mean "container rejected" or "init code was garbage" — actively
hides which of the four requirements you've missed. The single most non-obvious of them, the
**identity-page-map requirement**, only bites when a linker *permutes* the map, which it only does when
it has *discardable* segments to group, which a naive reconstruction may or may not reproduce — so it
surfaces intermittently and looks like a code bug.

The net, stated plainly for anyone who comes after:

> A reconstructed Win95 VxD linked from faithful source will not boot until its container carries the
> **device id (`0xc0`)** and **version resource (`0xb8/0xbc`)** the modern linker omits, and its object
> **page map is the identity permutation** — which you force by linking every segment
> **`PRELOAD NONDISCARDABLE`**. The code being correct is necessary but not sufficient; the container
> is the gate.
