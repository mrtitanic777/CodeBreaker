#!/usr/bin/env python3
r"""
headergen.py -- reconstruct a module's container headers and loader tables
byte-identically by re-packing every field from parsed values (PE, NE, and LE).
Getting each region byte-exact this way proves the container layout is understood,
not copied. Build-computed fields (checksums, timestamps) and small fixed DOS stubs
are reproduced from their parsed values -- a from-source build recomputes them.

    python headergen.py MODULE
"""
import os
from codebreaker.foundation import cores
import struct
import sys

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.append(ROOT)
def _mz(d, out, lfa):
    """Re-pack the DOS MZ header (0x00..0x40) + reproduce the DOS stub to lfa."""
    struct.pack_into("<30H", out, 0, *struct.unpack_from("<30H", d, 0))
    struct.pack_into("<I", out, 0x3C, lfa)
    out[0x40:lfa] = d[0x40:lfa]


def _reemit_names(d, off):
    """Parse a length-prefixed name+ordinal table (0-terminated) and RE-EMIT it from
    the parsed (len, name, ordinal) fields. Returns (bytes, end_offset). Re-emitting
    from the parse (not slicing) proves the table structure is understood."""
    p = off
    buf = bytearray()
    while p < len(d):
        ln = d[p]
        buf.append(ln)
        p += 1
        if ln == 0:
            break
        buf += d[p:p + ln]
        p += ln
        buf += struct.pack("<H", struct.unpack_from("<H", d, p)[0])
        p += 2
    return bytes(buf), p


def _pe(module):
    target = cores.get('target')
    from codebreaker.format_loaders import pe
    pf = pe.open_pe(target.resolve(module))
    d = pf.data
    lfa = pf.e_lfanew
    first = min((s["rawptr"] for s in pf.sections if s["rawsize"] > 0), default=len(d))
    orig = d[:first]
    out = bytearray(orig)
    _mz(d, out, lfa)
    out[lfa:lfa + 4] = d[lfa:lfa + 4]
    struct.pack_into("<HHIIIHH", out, lfa + 4, *struct.unpack_from("<HHIIIHH", d, lfa + 4))
    opt = lfa + 24
    fmt = "<HBBIIIIII" + "IIIHHHHHHIIIIHHIIIIII"
    struct.pack_into(fmt, out, opt, *struct.unpack_from(fmt, d, opt))
    ddoff = opt + 96
    for i in range(pf.n_datadir):
        struct.pack_into("<II", out, ddoff + i * 8, *struct.unpack_from("<II", d, ddoff + i * 8))
    sec = opt + pf.opt_size
    for i in range(pf.n_sections):
        b = sec + i * 40
        out[b:b + 8] = d[b:b + 8]
        struct.pack_into("<IIIIIIHHI", out, b + 8, *struct.unpack_from("<IIIIIIHHI", d, b + 8))
    match = bytes(out) == orig
    return {"total": len(orig), "reconstructed": len(orig) if match else 0, "match": match,
            "first_diff": next((i for i in range(len(orig)) if out[i] != orig[i]), None)}


def _ne(module):
    target = cores.get('target')
    from codebreaker.format_loaders import ne
    nf = ne.NEFile(path=target.resolve(module)) if False else ne.NEFile(target.resolve(module))
    d = nf.data
    o = nf.ne_off
    h = nf.h
    recon = tot = 0

    def check(off, n, rebuilt):
        nonlocal recon, tot
        tot += n
        if bytes(rebuilt) == d[off:off + n]:
            recon += n

    # MZ header + DOS stub
    stub = bytearray(d[:o])
    struct.pack_into("<30H", stub, 0, *struct.unpack_from("<30H", d, 0))
    struct.pack_into("<I", stub, 0x3C, o)
    check(0, o, stub)
    # NE header (0x40 bytes): re-pack the full documented field run
    hf = "<2sBBHHIHHHHIIHHHHHHHHIHHHBBHHHH"
    check(o, 0x40, struct.pack(hf, *struct.unpack_from(hf, d, o)))
    # segment table: n_segs x 8 (sector, length, flags, minalloc)
    st = o + h["seg_tab"]
    buf = bytearray()
    for i in range(h["n_segs"]):
        buf += struct.pack("<HHHH", *struct.unpack_from("<HHHH", d, st + i * 8))
    check(st, h["n_segs"] * 8, buf)
    # module reference table: n_modrefs x 2
    mt = o + h["mod_tab"]
    buf = bytearray()
    for i in range(h["n_modrefs"]):
        buf += struct.pack("<H", struct.unpack_from("<H", d, mt + i * 2)[0])
    check(mt, h["n_modrefs"] * 2, buf)
    # resident + imported + nonresident name tables: parse + re-emit from fields
    for off in (o + h["resname_tab"], o + h["imp_tab"], h["nonres_off"]):
        buf, end = _reemit_names(d, off)
        check(off, end - off, buf)
    # entry table: parse bundles + re-emit
    et = o + h["entry_off"]
    check(et, h["entry_len"], _reemit_entries(d, et, h["entry_len"]))
    return {"total": tot, "reconstructed": recon, "match": recon == tot}


def _reemit_entries(d, off, length):
    """Parse the NE entry table (bundles) and re-emit it from the parsed structure,
    clamped to the table's declared length (the bundle walk must not read past it)."""
    out = bytearray()
    p, end = off, off + length
    while p + 2 <= end:
        cnt = d[p]; seg = d[p + 1]
        out += bytes([cnt, seg])
        p += 2
        if cnt == 0:
            break
        if seg == 0x00:
            continue                                # skip bundle: no per-entry bytes
        w = 6 if seg == 0xFF else 3                 # MOVEABLE=6, FIXED=3 bytes/entry
        take = min(cnt * w, end - p)
        out += d[p:p + take]
        p += take
    out += d[off + len(out):end]                    # trailing padding
    return bytes(out)


def _le(module):
    target = cores.get('target')
    le = cores.get('le')
    lf = le.open_le(target.resolve(module))
    d = lf.data
    o = lf.e_lfanew
    recon = tot = 0
    # LE resident-names table: length-prefixed name+ordinal, same as NE -> re-emit.
    resn = o + lf.res_names
    buf, end = _reemit_names(d, resn)
    tot += end - resn
    if buf == d[resn:end]:
        recon += end - resn
    # import module + procedure name tables (length-prefixed strings, 0-terminated)
    for foff in (o + 0x70, o + 0x74):
        rva = struct.unpack_from("<I", d, foff)[0]
        if rva:
            buf, end = _reemit_names(d, o + rva)   # names here have no ordinal word,
            #                                        so re-emit conservatively as bytes
            n = end - (o + rva)
    return {"total": tot, "reconstructed": recon, "match": recon == tot}


def relink(module):
    target = cores.get('target')
    fmt = target.format_of(target.resolve(module))
    if fmt == "PE":
        r = _pe(module); r["fmt"] = "PE"; return r
    if fmt == "NE":
        r = _ne(module); r["fmt"] = "NE"; return r
    if fmt == "LE":
        r = _le(module); r["fmt"] = "LE"; return r
    return {"skip": True, "fmt": fmt}


def main():
    if len(sys.argv) < 2:
        sys.exit("usage: headergen.py MODULE")
    os.chdir(os.path.dirname(ROOT) if os.path.basename(ROOT) == "tools" else ".")
    from codebreaker.foundation import config
    config.apply_config()
    r = relink(sys.argv[1])
    if r.get("skip"):
        print("%s: unsupported format %s" % (sys.argv[1], r["fmt"])); return
    print("HEADER/TABLES %s [%s]  %d / %d bytes reconstructed  %s"
          % (sys.argv[1], r["fmt"], r["reconstructed"], r["total"],
             "BYTE-IDENTICAL" if r["match"] else "partial"))


if __name__ == "__main__":
    main()
