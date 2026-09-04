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

## Notes

- **Nothing here contains Microsoft code.** `build_vmm32.py` reads VxDs you supply under
  `binaries/win95/oracle/` and writes reconstructions; the disk image and its contents are yours.
- The reconstructions differ from the oracle only in the order LINK386 emitted intra-page fixup
  records — a linker artifact that does not affect loading or execution.
- The compressed **W4** container (used on installed machines) is *not* handled here and is not
  needed: its per-block compression is an LZSS-family scheme whose decompressor lives in the
  real-mode loader (IO.SYS/WINBOOT), and the uncompressed-W3 + standalone-fallback route above
  sidesteps it entirely.
