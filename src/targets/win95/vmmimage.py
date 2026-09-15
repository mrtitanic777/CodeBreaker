#!/usr/bin/env python3
r"""vmmimage.py -- pre-relocate the VMM LE into one flat bootable image (VMM32 seam V3, Python half).

The winboot seam (Windows95/msdos/winboot) enters 32-bit flat ring-0 PM (winmin1, V1). To ENTER VMM
it must map VMM's 22 LE objects at real flat bases and apply the LE internal fixups -- the hard half.
This does exactly that in tested Python (le.py exposes the object table + parsed fixup records), so the
bootstrap asm (winmin2) stays trivial: read vmm.img to LOAD_BASE, jmp to the entry linear in vmm.map.

  * places every object contiguously, 4K-page-aligned, from LOAD_BASE (flat, no paging);
  * applies every INTERNAL fixup (patch the 32-bit site to target_objbase + target_off);
  * leaves IMPORT fixups (VMM/VxD service calls -- INT 20h VxDcall, self-dispatched at runtime);
  * entry linear = objbase[eip_obj] + eip.

    wt --target win95 vmmimage [--base 0xNNN] [--out DIR]     # emit vmm.img + vmm.map

Honest scope: this produces the correctly-RELOCATED image + entry. VMM's init protocol
(Sys_Critical_Init + the VxD device-init chain) is the environment frontier winmin2/V3+ must supply.
"""
import os
import struct

from src.formats import le as LE

LOAD_BASE = 0x00100000       # 1 MB: flat ring-0, clear of real-mode + the winboot loader
PAGE = 0x1000


def _align(n, a=PAGE):
    return (n + a - 1) & ~(a - 1)


def _vmm_le():
    """The VMM LEFile out of the retail vmm32.vxd W3 bundle (via the tested W3/LE parser)."""
    from src.targets.win95 import origintriage as OT
    from src.formats import le_load
    root = OT._corpus_root()
    path = os.path.join(root, "WINDOWS", "SYSTEM", "vmm32.vxd")
    if not os.path.exists(path):
        for dp, _d, fs in os.walk(root):
            for f in fs:
                if f.lower() == "vmm32.vxd":
                    path = os.path.join(dp, f)
    units = le_load.le_modules(path)                    # [(name, LEFile)] from the W3 bundle
    for n, lf in units:
        if n.upper() == "VMM":
            return lf
    return units[0][1]


def build(base=LOAD_BASE):
    """Return (image_bytes, load_base, entry_linear, layout) for the relocated VMM."""
    lf = _vmm_le()
    # assign each object a flat, page-aligned base, contiguous from LOAD_BASE
    objbase = {}
    cur = base
    for o in lf.objects:
        objbase[o["idx"]] = cur
        cur = _align(cur + max(o["vsize"], PAGE))
    total = cur - base
    img = bytearray(total)
    for o in lf.objects:
        b = lf.object_bytes(o)
        pos = objbase[o["idx"]] - base
        img[pos:pos + len(b)] = b
    # apply internal fixups: patch the 32-bit reference site to target_objbase + target_off
    patched = 0
    for (obj_idx, off), fx in lf.fixups().items():
        if fx.get("kind") != "internal" or fx.get("target") is None:
            continue
        tob = objbase.get(fx["obj"])
        if tob is None:
            continue
        site = (objbase[obj_idx] - base) + off
        if 0 <= site <= len(img) - 4:
            struct.pack_into("<I", img, site, (tob + fx["target"]) & 0xFFFFFFFF)
            patched += 1
    entry = objbase[lf.eip_obj] + lf.eip
    layout = {"objects": len(lf.objects), "fixups_applied": patched, "size": total,
              "eip_obj": lf.eip_obj, "eip": lf.eip}
    return bytes(img), base, entry, layout


def main(argv):
    base = int(argv[argv.index("--base") + 1], 0) if "--base" in argv else LOAD_BASE
    outdir = argv[argv.index("--out") + 1] if "--out" in argv else \
        os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(__file__)))),
                     "..", "Windows95", "msdos", "winboot")
    img, ldbase, entry, layout = build(base)
    os.makedirs(outdir, exist_ok=True)
    with open(os.path.join(outdir, "vmm.img"), "wb") as f:
        f.write(img)
    with open(os.path.join(outdir, "vmm.map"), "w", newline="\n") as f:
        f.write("load_base 0x%08X\nentry     0x%08X\nsize      0x%08X\nobjects   %d\n"
                "fixups    %d\n" % (ldbase, entry, layout["size"], layout["objects"],
                                    layout["fixups_applied"]))
    print("=== vmmimage (VMM32 seam V3, relocated image) ===")
    print("  load_base 0x%08X   entry 0x%08X   size 0x%X (%d KB)"
          % (ldbase, entry, layout["size"], layout["size"] // 1024))
    print("  objects %d   internal fixups applied %d" % (layout["objects"], layout["fixups_applied"]))
    print("  wrote vmm.img + vmm.map to %s" % os.path.normpath(outdir))


if __name__ == "__main__":
    import sys
    main(sys.argv[1:])
