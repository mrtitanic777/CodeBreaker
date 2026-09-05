# Boot a fully reconstructed Windows 95 kernel

This directory rebuilds Windows 95's entire 32-bit kernel + static driver stack from the recovered
disassembly and boots it in a real Win95 VM. It is the end-to-end proof of the VxD reconstruction:
the recovered code is not just verified byte-exact against the oracle — it *runs*, as the live
kernel, all the way to the logon.

## What it does

`vmm32.vxd` is Windows 95's 32-bit core: the VMM kernel plus the statically-loaded VxDs
(VPICD, VTD, VDMAD, CONFIGMG, IFSMGR, VFAT, VCACHE, IOS, …), packed into one loadable image. On an
installed machine it is a compressed, machine-specific **W4** container; the reconstruction oracle
is the uncompressed **W3** form from the install media.

Two facts make a full reconstruction bootable without touching the W4 compression:

1. Win95's loader accepts an **uncompressed W3** `vmm32.vxd` — no decompression required.
2. For any registry-listed static VxD **not** found in the bundle, the loader falls back to a
   standalone **`\WINDOWS\SYSTEM\VMM32\<name>.vxd`** (a missing one prints
   `Cannot find device file … VMM32\CONFIGMG.VXD`).

So the machine-specific bundle is fully replaceable: ship a reconstructed **VMM-only W3** as
`vmm32.vxd` and every other static VxD as a reconstructed standalone file. The loader then loads
the whole kernel + driver set from the reconstructions.

## Use it

```bash
# 1) reconstruct every VxD into a staging directory (byte-exact bar fixup order; all round-trip)
python build_vmm32.py --out /tmp/vmm32stage

# 2) clone a Win95 disk, replace vmm32.vxd + drop in the standalone VxDs (needs Linux/WSL:
#    qemu-img + mtools). SRC is read only.
./inject_and_boot.sh  path/to/Win95.vmdk  /tmp/vmm32stage  /tmp/Win95-reconstructed.vmdk

# 3) point a VM at the OUT disk and boot -> the reconstructed kernel runs to the logon
```

`inject_and_boot.sh` **keeps the guest's own newer standalone drivers** (an updated IOS/NTKERN/
VFBACKUP that post-date the oracle) rather than overwriting them — replacing a newer IOS with the
older reconstruction throws a Windows protection error during IOS init.

## Rebuild the machine's *exact* bundle

The flow above boots a *replacement* kernel (VMM-only W3 + standalone VxDs). **`rebuild_bundle.py`**
does the stronger thing: it takes the machine's own `vmm32.vxd` — the compressed **W4** an installed
Win95 actually runs — decompresses it to its W3 payload, and re-synthesizes **every** bundled module
*in place* from the recovered disassembly, keeping the same module set and layout:

```bash
# W4 (installed machine) or W3 (install media) in -> uncompressed-W3 bundle out, every module rebuilt
python rebuild_bundle.py  vmm32.vxd  vmm32-rebuilt.vxd  --check
#   rebuilt 43/43 LE modules in place, 43 byte-exact-except-fixup-order
#   relocation set identical to original: 43/43 modules

# drop it in as VMM32.VXD (loader takes the uncompressed-W3 path) and boot -> runs to logon
./inject_and_boot.sh  path/to/Win95.vmdk  <dir-with-vmm32-rebuilt.vxd-as-vmm32.vxd>  out.vmdk
```

Each module's object bytes come from `VxDDisasm(...).run().code` (byte-exact) and its fixup tables
are re-emitted by the LE writer; `--check` confirms the re-emitted fixups decode to the identical
relocation set (they differ from the originals only in LINK386's intra-page record order). One wrinkle
the engine now handles: a W3 sub-image leaves its non-resident-name offset module-relative after
bundling, so the data-page extent is derived from the page layout, not that header field — otherwise
the source-type analysis reads an empty code window and every fixup comes out wrong.

## Notes

- **Nothing here contains Microsoft code.** `build_vmm32.py` reads VxDs you supply under
  `binaries/win95/oracle/` and writes reconstructions; the disk image and its contents are yours.
- The reconstructions differ from the oracle only in the order LINK386 emitted intra-page fixup
  records — a linker artifact that does not affect loading or execution.
- The uncompressed-W3 + standalone-fallback route above is all you need to boot; it sidesteps the
  compressed **W4** container installed machines actually use.
- If you *do* want an installed machine's exact bundle, **`w4.py`** decompresses a W4 vmm32.vxd to
  its W3 payload and lists the bundled modules: `python w4.py vmm32.vxd out.w3`. Each 8 KB page is
  compressed with an MRCI/DoubleSpace-family bit code (literals + 6/8/12-bit-distance back-references
  at base 0/64/320, max 4415 = end-of-page, gamma length from 2). The decompressor is the routine
  inside the W4's *own* real-mode stub; `w4.py` locates it and runs it under the Unicorn CPU emulator,
  so the output is Microsoft's own, bit-for-bit. (Needs `pip install unicorn`. Re-*compressing* to a
  byte-exact W4 is unnecessary — the loader accepts an uncompressed W3.)
