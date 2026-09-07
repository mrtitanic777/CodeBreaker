# Windows 95 VxD Coverage — Complete Inventory and Status

*Which Ring-0 Virtual Device Drivers of Windows 95 are reconstructed, how completeness was established,
and the verification status of each class. Companion to [`VXD_FROM_SOURCE.md`](VXD_FROM_SOURCE.md) and
[`WIN95_VXD_TOOLCHAIN.md`](WIN95_VXD_TOOLCHAIN.md).*

---

## Summary

| Metric | Value |
|---|---|
| Reconstructed drivers | **222** |
| Objects byte-exact verified | **903 / 903** |
| Modules fully byte-exact | **222 / 222** |
| Objects needing whole-object byte preservation | **0** |
| Idiomatic sources that assemble through DDK `ML` | 222 / 222 |
| From-source loadable + booted (authentic toolchain) | 1 (VKD; capability, §6) |

Every reconstructed driver has two coordinated views in `Windows95/vmm32/<name>/`: the **idiomatic**
whole-driver source (`<name>.asm`, DDK macros) and the **byte-exact** per-object source
(`<name>_<class>_<obj>.asm`), plus a `<name>.txt` index. Byte-exactness means each LE object, re-assembled
from its recovered source with the period DDK `ML.EXE`, is identical to the oracle object; differences the
assembler will not emit are byte-preserved per instruction.

---

## 1. How completeness was established

The Windows 95 install (OSR2 cab-extracted oracle set) was enumerated for **every LE / LX / W3 / W4
Ring-0 module** — not only files with a `.vxd` extension. That enumeration yields **224 distinct driver
stems**, reconciled as follows:

| Group | Count | Status |
|---|---|---|
| `.vxd` / `.386` Virtual Device Drivers | 215 | reconstructed |
| `.pdr` IOS port drivers | 4 | reconstructed (`scsiport`, `esdi_506`, `hsflop`, `rmm`) |
| misc-extension VxDs (`.sys`, `.drv`, `.bin`) | 3 | reconstructed (`csmapper`, `bigmem`, `drvspace`) |
| `vmm32.vxd` (the W3/W4 bundle **container**) | 1 | its member `vmm` is reconstructed (§3) |
| LE **executables**, not drivers | 2 | out of scope (`drwatson.exe`, `emm386.exe`) |

The 222 reconstructed drivers are the 215 + 4 + 3. The bundle container and the two LE executables are the
only LE Ring-0 files in the install that are not device drivers; both are accounted for above.

**Why the `.pdr`/`.sys`/`.drv` drivers were initially missed.** The corpus batch driver enumerated only
`*.vxd` and `*.386`. IOS port drivers ship as `.pdr`, mini-port drivers as `.mpd`, and a few core drivers
under `.sys`/`.drv`/`.bin` — but all are ordinary LE VxD modules (a DDB, a control proc, LE objects). The
enumeration now includes `.pdr`/`.mpd` plus the specific misc-extension VxDs, filtered by LE/W3/W4
signature so a real-mode `.sys` is not swept in.

---

## 2. Driver classes

| Extension | Role | Loaded by | Examples |
|---|---|---|---|
| `.vxd` | Virtual Device Driver | VMM / VXDLDR | `vkd`, `vmm`, `vpicd`, `configmg` |
| `.386` | VxD (legacy extension) | VMM | a handful of older-named drivers |
| `.pdr` | IOS **port driver** (block I/O layer) | IOS | `scsiport`, `esdi_506`, `hsflop`, `rmm` |
| `.mpd` | IOS **mini-port driver** | IOS | (none present in this install) |
| `.sys`/`.drv`/`.bin` | VxD under a role-specific name | various | `csmapper`, `bigmem`, `drvspace` |

`scsiport.pdr` and `esdi_506.pdr` carry real device ids (`0x35`, `0x8d`) in their DDBs; the rest are
dynamically loaded (device id `0`). All reconstruct byte-exact identically to the `.vxd` drivers.

---

## 3. The VMM32.VXD bundle

`VMM32.VXD` is a **W3 (or W4) container**: an MZ real-mode loader, a W3 header + entry table, and one or
more bundled LE modules. Its shape depends on the build:

* **Oracle (OSR2 cab-extracted):** a W3 container with a **single** member, `vmm` — the VMM core. The other
  core VxDs ship as separate `.vxd` files in this build.
* **Installed system (e.g. Logan95):** a W4 container the installer builds at setup time by bundling the
  machine's selected core VxDs — **43 modules** in the reference system. This bundle was decompressed and
  reconstructed in place and boots to the logon.

Either way, every bundled member is individually present in the 222-driver corpus (`vmm`, `ifsmgr`, `ios`,
`configmg`, `vpicd`, `vtd`, `vdd`, `dosmgr`, `vmcpd`, `vkd`, `vdmad`, `vxdldr`, …). The W3/W4 header + entry
table reconstruct byte-exact by re-packing their fields; each entry points at a standard LE module the
reconstruction already covers.

---

## 4. Verification

Every object is verified by re-assembly, not by inspection: its recovered source is assembled with the
period DDK `ML.EXE` (COFF) and the assembled image is diffed against the oracle LE object byte-for-byte.
The batch driver (`engine/inverse_engine/vxd_corpus.py`) runs this across the whole corpus in parallel
DOSBox sessions, checkpointing after each module so a run is resumable.

Result: **222 / 222 modules and 903 / 903 objects byte-exact**, with **0** objects requiring whole-object
byte preservation — every object re-assembles from source, byte-preserved per instruction only where the
assembler will not emit the original encoding (the `ADD` direction bit, `mov eAX,[disp]` `A1`-vs-`8B`,
redundant/segment prefixes, SIB base-index order). No object is byte-copied wholesale. A fresh full re-run
on the current engine (1,287,237 per-instruction re-encodings across the corpus) reproduces this exactly;
the object-level result is a property of the **assembler** and is independent of the linker.

Separately, all 222 **idiomatic** whole-driver sources assemble through `ML` (COFF) against the stock
`INC32` headers with zero errors — the macros expand to the recovered bytes.

---

## 5. Naming

Real DDK names are used where they can be recovered: **exported service names** and
**control-dispatch handler names** (named by message — `VKD_DEVICE_INIT`, …) for all drivers, and
**internal function names** for the core VxDs shipped with debug `.SYM` files
(VMM, VKD, VPICD, VDD, VDMAD, CONFIGMG, IOS, SCSIPORT, VFLATD, VXDLDR, …). Microsoft shipped no symbols for
the majority, whose internals are therefore `sub_<offset>`. Because a stripped LE binary carries no
original file names, files are named by segment class and role.

---

## 6. From-source loadable status

Byte-exact object recovery proves the **code** is correct. Producing a **loadable, bootable** VxD from that
recovered source — through the authentic period toolchain — is a separate result, documented in
[`VXD_FROM_SOURCE.md`](VXD_FROM_SOURCE.md). It is established for **VKD**: rebuilt entirely from recovered
source with `ML` + the DDK's `LINK 2.60.5046` + `RC` + `ADRC2VXD`, with no reimplementation and no bytes
copied from the original, it boots Windows 95 to the logon with a working keyboard.

The from-source loadable path applies in principle to any driver in the corpus; it has been carried through
to a boot for VKD specifically. Extending it driver-by-driver is the open work item. The distinction is
deliberate: **all 222 are byte-exact verified at the object level; one is boot-verified from source.**

---

## 7. Scope boundary

The only LE Ring-0 files in the install that are **not** reconstructed are `drwatson.exe` and `emm386.exe`
— LE-format *executables*, not device drivers — and the `vmm32.vxd` container file itself, whose members
are all covered. There are no `.mpd` mini-port drivers in this install. To the resolution of an
LE/W3/W4-signature enumeration of the OSR2 install, the VxD coverage of Windows 95 is complete.
