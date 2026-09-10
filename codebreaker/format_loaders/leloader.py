#!/usr/bin/env python3
r"""
leloader.py -- reconstruct an LE/LX (VxD) module's loader tables byte-identically.

An LE module's loader section is the container structure Windows' VMM reads to map
the VxD: the object table (what maps where), the object page map (which physical
page backs each logical page), and the fixup page table (where each page's
relocation records live). These are fixed-format numeric tables -- parseable and
rebuildable from their fields, exactly like PE import/export tables. Reconstructing
them moves LE's large loader section from "retained" to genuine structure.

    python leloader.py MODULE      # rebuild + verify the LE loader tables
"""
import os
from codebreaker.foundation import cores
import struct
import sys

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.append(ROOT)
def relink(module):
    """Return {table: (match, bytes)} for the reconstructable LE loader tables."""
    target = cores.get('target')
    le = cores.get('le')
    path = target.resolve(module)
    if target.format_of(path) != "LE":
        return {"skip": True}
    lf = le.open_le(path)
    d = lf.data
    o = lf.e_lfanew
    out = {}

    # object table: n_objects x 24-byte records (6 u32 fields), re-packed from parse
    ot = o + lf.obj_tab
    orig = d[ot:ot + lf.n_objects * 24]
    buf = bytearray()
    for i in range(lf.n_objects):
        vsize, base, flags, pidx, pcnt, res = struct.unpack_from("<IIIIII", d, ot + i * 24)
        buf += struct.pack("<IIIIII", vsize, base, flags, pidx, pcnt, res)
    out["objtable"] = (bytes(buf) == orig, len(orig))

    # object page map: n_pages x 4 (3-byte BE physical page number + 1 flag byte)
    pm = o + lf.obj_page_map
    orig = d[pm:pm + lf.n_pages * 4]
    buf = bytearray()
    for i in range(lf.n_pages):
        rec = d[pm + i * 4:pm + i * 4 + 4]
        phys = int.from_bytes(rec[:3], "big")
        buf += phys.to_bytes(3, "big") + bytes([rec[3] if len(rec) == 4 else 0])
    out["pagemap"] = (bytes(buf) == orig, len(orig))

    # fixup page table: (n_pages + 1) x u32 offsets into the fixup record table
    fixpage = struct.unpack_from("<I", d, o + 0x68)[0]
    if fixpage:
        fp = o + fixpage
        n = (lf.n_pages + 1) * 4
        orig = d[fp:fp + n]
        buf = bytearray()
        for i in range(lf.n_pages + 1):
            buf += struct.pack("<I", struct.unpack_from("<I", d, fp + i * 4)[0])
        out["fixuppage"] = (bytes(buf) == orig, len(orig))
    return out


def main():
    if len(sys.argv) < 2:
        sys.exit("usage: leloader.py MODULE")
    os.chdir(os.path.dirname(ROOT) if os.path.basename(ROOT) == "tools" else ".")
    from codebreaker.foundation import config
    config.apply_config()
    r = relink(sys.argv[1])
    if r.get("skip"):
        print("%s is not LE" % sys.argv[1]); return
    tot = ok = 0
    for name, (match, nb) in r.items():
        tot += nb
        if match:
            ok += nb
        print("  %-12s %7d bytes  %s" % (name, nb, "BYTE-IDENTICAL" if match else "DIFF"))
    print("  reconstructed %d / %d loader-table bytes" % (ok, tot))


if __name__ == "__main__":
    main()
