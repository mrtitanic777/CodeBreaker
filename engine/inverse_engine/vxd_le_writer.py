#!/usr/bin/env python3
"""Synthesize an LE VxD directly from the recovered disassembly -- object bytes from D.code, the
relocation table from D.fx -- instead of assembling + linking. This sidesteps LINK/MASM entirely,
so cross-object relocations (which MASM/COFF cannot express as a near call across sections) are
trivial: the fixup records are emitted by hand.

Stage 1 (this module) is FUNCTIONAL: the emitted file round-trips through le.py to the same objects,
object bytes, and fixups. Loadability by the real Win95 loader + whole-file byte-exactness are the
hardening targets on top of this.
"""
import struct


def _src_type(D, oi, off):
    """LE fixup source type for the field at (oi, off): 0x08 (32-bit self-relative) when it is the
    rel32 of a near call/jmp, else 0x07 (32-bit offset)."""
    r = D.insns.get((oi, off - 1))
    if r and D.code[oi][off - 1] in (0xE8, 0xE9) and r["size"] == 5:
        return 0x08
    return 0x07


def _emit_fixups(D, v):
    """LE fixup page table + record table from D.fx -- one record per fixup (single source), grouped
    by logical page. Returns (page_table_bytes, record_bytes)."""
    by_page = {}
    for (oi, off), f in D.fx.items():
        if f["kind"] == "internal" and f["target"] is None:
            continue                                        # selector-only fixup, no 32-bit field
        obj = next((o for o in v.objects if o["idx"] == oi), None)
        if not obj or off < 0:
            continue
        pg = obj["pmidx"] + off // v.pagesize
        by_page.setdefault(pg, []).append((off % v.pagesize, _src_type(D, oi, off), f))
    recs = bytearray()
    pageoffs = []
    for pg in range(1, v.mpages + 1):
        pageoffs.append(len(recs))
        for po, st, f in sorted(by_page.get(pg, [])):
            if f["kind"] == "internal":
                tflags = 0x10 if f["target"] > 0xffff else 0x00
                recs += struct.pack("<BBhB", st, tflags, po, f["obj"])
                recs += struct.pack("<I", f["target"]) if (tflags & 0x10) else struct.pack("<H", f["target"])
            else:                                           # import by ordinal
                recs += struct.pack("<BBhBB", st, 0x01 | 0x80, po, f["module"], f["ordinal"] & 0xff)
    pageoffs.append(len(recs))
    return b"".join(struct.pack("<I", o) for o in pageoffs), bytes(recs)


def build_le(D, v, vxdname):
    """Return the bytes of a synthesized LE VxD for the recovered disassembly D of oracle v."""
    ps = v.pagesize
    objs = v.objects
    # data pages: mpages physical pages by index (unassigned gap pages zero-filled; last page as-is)
    page_content = {}
    for o in objs:
        for k in range(o["pcnt"]):
            page_content[o["pmidx"] + k] = D.code[o["idx"]][k * ps:(k + 1) * ps]
    data = bytearray()
    for pg in range(1, v.mpages + 1):
        b = bytes(page_content.get(pg, b""))
        data += b if pg == v.mpages else b + b"\x00" * (ps - len(b))
    last_page_bytes = v.last_page_bytes

    objtab = b"".join(struct.pack("<IIIII", o["vsize"], o["base"], o["flags"], o["pmidx"], o["pcnt"])
                      + b"\x00\x00\x00\x00" for o in objs)
    pagemap = b"".join(struct.pack(">HBB", (pg >> 8) & 0xffff, pg & 0xff, 0) for pg in range(1, v.mpages + 1))
    nm = vxdname.encode("latin1")
    resnames = struct.pack("<B", len(nm)) + nm + struct.pack("<H", 0) + b"\x00"
    db = D.ddb
    entrytab = struct.pack("<BBH", 1, 0x03, db["obj"] if db else 1) \
        + struct.pack("<BI", 0, db["rva"] if db else 0) + b"\x00"
    fxpage, fxrec = _emit_fixups(D, v)

    lfa = 0x80
    hdr_len = 0xc4
    pos = hdr_len
    off_objtab = pos; pos += len(objtab)
    off_pagemap = pos; pos += len(pagemap)
    off_resnames = pos; pos += len(resnames)
    off_entrytab = pos; pos += len(entrytab)
    off_fxpage = pos; pos += len(fxpage)
    off_fxrec = pos; pos += len(fxrec)
    off_impmod = pos; pos += 1
    datapages = (pos + lfa + 0xf) & ~0xf

    H = bytearray(hdr_len)
    struct.pack_into("<2sBBI", H, 0, b"LE", 0, 0, 0)
    struct.pack_into("<HH", H, 0x08, v.cpu, v.ostype)
    struct.pack_into("<I", H, 0x14, v.mpages)
    struct.pack_into("<II", H, 0x18, v.eip_obj, v.eip)
    struct.pack_into("<II", H, 0x28, ps, last_page_bytes)
    struct.pack_into("<I", H, 0x40, off_objtab); struct.pack_into("<I", H, 0x44, len(objs))
    struct.pack_into("<I", H, 0x48, off_pagemap)
    struct.pack_into("<I", H, 0x58, off_resnames)
    struct.pack_into("<I", H, 0x5c, off_entrytab)
    struct.pack_into("<I", H, 0x68, off_fxpage)
    struct.pack_into("<I", H, 0x6c, off_fxrec)
    struct.pack_into("<I", H, 0x70, off_impmod); struct.pack_into("<I", H, 0x74, 0)
    struct.pack_into("<I", H, 0x80, datapages)
    # header fields the real Win95 loader reads, preserved from the oracle (module version/flags, the
    # initial ESP, preload page count) -- these are recovered values, not linker artifacts
    for foff in (0x0c, 0x10, 0x20, 0x24, 0x84):
        struct.pack_into("<I", H, foff, struct.unpack_from("<I", v.d, lfa + foff)[0])
    # non-resident name table (the module description string): preserved, placed after the data pages
    nres_off = struct.unpack_from("<I", v.d, lfa + 0x88)[0]
    nres_sz = struct.unpack_from("<I", v.d, lfa + 0x8c)[0]
    nres = v.d[nres_off:nres_off + nres_sz] if (nres_off and nres_sz and nres_off + nres_sz <= len(v.d)) else b""
    if nres and not (0 < nres[0] < 64 and all(31 < c < 127 or c == 9 for c in nres[1:1 + nres[0]])):
        nres = b""                                          # not a plausible description (e.g. a W3 module's container-relative offset)
    if nres:
        struct.pack_into("<I", H, 0x88, datapages + len(data))
        struct.pack_into("<I", H, 0x8c, len(nres))

    mz = bytearray(0x80)
    mz[0:2] = b"MZ"
    struct.pack_into("<I", mz, 0x3c, lfa)
    body = bytes(mz) + bytes(H) + objtab + pagemap + resnames + entrytab + fxpage + fxrec + b"\x00"
    body += b"\x00" * (datapages - len(body)) + bytes(data) + nres
    return bytes(body)
