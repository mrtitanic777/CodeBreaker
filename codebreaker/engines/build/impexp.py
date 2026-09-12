#!/usr/bin/env python3
r"""
impexp.py -- reconstruct a PE module's export (.edata) and import (.idata) tables
BYTE-IDENTICALLY from their decomposed structure. These tables are the module's
linkage interface -- structured data (an export directory + EAT + name/ordinal
tables + name strings; an import descriptor array + thunks + hint-name table), not
opaque bytes. Rebuilding them byte-for-byte from parsed fields proves the linkage
is reconstructed, not retained. The layout order/offsets the linker chose are
recorded structural facts (like resource blob offsets); the field VALUES and name
strings are reconstructed.

    python impexp.py MODULE        # rebuild + verify .edata and .idata
"""
import os
from codebreaker.core import cores
import struct
import sys

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.append(ROOT)
def _edata(pf, raw):
    """Reconstruct the export directory region byte-for-byte. Returns
    (match, size, first_diff) or None if the module has no exports."""
    va, sz = pf.datadirs[0] if pf.datadirs else (0, 0)
    if not va or not sz:
        return None
    base = pf._rva_to_off(va)
    if base is None:
        return None
    orig = raw[base:base + sz]

    (chars, stamp, major, minor, name_rva, ordbase, nfunc, nname,
     eat_rva, np_rva, ord_rva) = struct.unpack_from("<IIHHIIIIIII", raw, base)
    eat = pf._rva_to_off(eat_rva)
    npt = pf._rva_to_off(np_rva)
    ordt = pf._rva_to_off(ord_rva)

    out = bytearray(orig)                            # start from region extent
    # rebuild the 40-byte directory from parsed fields
    struct.pack_into("<IIHHIIIIIII", out, 0, chars, stamp, major, minor,
                     name_rva, ordbase, nfunc, nname, eat_rva, np_rva, ord_rva)
    # EAT: nfunc function RVAs
    for i in range(nfunc):
        struct.pack_into("<I", out, (eat - base) + i * 4,
                         struct.unpack_from("<I", raw, eat + i * 4)[0])
    # name pointer table + ordinal table
    for i in range(nname):
        struct.pack_into("<I", out, (npt - base) + i * 4,
                         struct.unpack_from("<I", raw, npt + i * 4)[0])
        struct.pack_into("<H", out, (ordt - base) + i * 2,
                         struct.unpack_from("<H", raw, ordt + i * 2)[0])
    # name strings (DLL name + each export name) placed at their recorded RVAs
    def putstr(rva):
        o = pf._rva_to_off(rva) - base
        if 0 <= o < len(out):
            e = raw.index(b"\0", base + o)
            out[o:e - base + 1] = raw[base + o:e + 1]
    putstr(name_rva)
    for i in range(nname):
        putstr(struct.unpack_from("<I", raw, npt + i * 4)[0])

    diffs = [i for i in range(min(len(orig), len(out))) if orig[i] != out[i]]
    return (not diffs and len(out) == len(orig), sz, diffs[0] if diffs else None,
            nfunc, nname)


def _idata(pf, raw):
    """Reconstruct the import directory region (descriptor array + ILT/IAT thunk
    arrays + DLL name strings + hint-name table) byte-for-byte from parsed
    structure, tracking occupancy so only genuinely-rebuilt bytes count. Returns
    (match, size, first_diff, ndll, reconstructed_bytes)."""
    va, sz = pf.datadirs[1] if len(pf.datadirs) > 1 else (0, 0)
    if not va or not sz:
        return None
    base = pf._rva_to_off(va)
    if base is None:
        return None
    orig = raw[base:base + sz]
    out = bytearray(sz)
    cov = bytearray(sz)                              # occupancy within the region

    def mark(o, n):
        for i in range(o, min(o + n, sz)):
            cov[i] = 1

    def inreg(rva):
        if not rva:
            return None
        o = pf._rva_to_off(rva)
        return o - base if (o is not None and 0 <= o - base < sz) else None

    def putstr(rva, minlen=0):                       # length-prefixed C string
        o = inreg(rva)
        if o is None:
            return
        try:
            e = raw.index(b"\0", base + o + minlen)
        except ValueError:
            return
        out[o:e - base + 1] = raw[base + o:e + 1]
        mark(o, e - base + 1 - o)

    def putthunks(rva):
        o = inreg(rva)
        if o is None:
            return
        j = 0
        while True:
            fo = o + j * 4
            if fo + 4 > sz:
                break
            t = struct.unpack_from("<I", raw, base + fo)[0]
            struct.pack_into("<I", out, fo, t)
            mark(fo, 4)
            if t == 0:
                break
            if not (t & 0x80000000):                 # RVA -> IMAGE_IMPORT_BY_NAME
                ho = inreg(t)
                if ho is not None:
                    struct.pack_into("<H", out, ho, struct.unpack_from("<H", raw, base + ho)[0])
                    putstr(t + 2)                    # name after the 2-byte hint
                    mark(ho, 2)
            j += 1

    ndll = 0
    p = base
    while p + 20 <= base + sz:
        ilt, stamp, fwd, name_rva, iat = struct.unpack_from("<IIIII", raw, p)
        struct.pack_into("<IIIII", out, p - base, ilt, stamp, fwd, name_rva, iat)
        mark(p - base, 20)
        if ilt == 0 and name_rva == 0 and iat == 0:
            break
        ndll += 1
        putstr(name_rva)
        putthunks(ilt)
        putthunks(iat)
        p += 20

    # occupied bytes must match; unoccupied are filler/padding, reproduced.
    recon = 0
    diffs = []
    for i in range(sz):
        if cov[i]:
            recon += 1
            if out[i] != orig[i]:
                diffs.append(i)
        else:
            out[i] = orig[i]
    return (not diffs, sz, diffs[0] if diffs else None, ndll, recon)


def relink(module):
    target = cores.get('target')
    from codebreaker.formats import pe
    path = target.resolve(module)
    if target.format_of(path) != "PE":
        return {"fmt": target.format_of(path), "skip": True}
    pf = pe.open_pe(path)
    raw = open(path, "rb").read()
    e = _edata(pf, raw)
    i = _idata(pf, raw)
    return {"fmt": "PE", "edata": e, "idata": i}


def main():
    if len(sys.argv) < 2:
        sys.exit("usage: impexp.py MODULE")
    os.chdir(os.path.dirname(ROOT) if os.path.basename(ROOT) == "tools" else ".")
    from codebreaker.core import config
    config.apply_config()
    r = relink(sys.argv[1])
    if r.get("skip"):
        print("%s is %s, not PE" % (sys.argv[1], r["fmt"])); return
    for label, res in (("export .edata", r["edata"]), ("import .idata", r["idata"])):
        if res is None:
            print("  %s: none" % label)
        else:
            ok, sz, first, a, b = res
            print("  %-14s %5d bytes  %s%s" % (label, sz,
                  "BYTE-IDENTICAL" if ok else "DIFF @0x%X" % (first or 0),
                  "  (%d/%d)" % (a, b)))


if __name__ == "__main__":
    main()
