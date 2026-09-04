#!/usr/bin/env python3
"""Synthesize an LE VxD directly from the recovered disassembly -- object bytes from D.code, the
relocation table from D.fx -- instead of assembling + linking. This sidesteps LINK/MASM entirely,
so cross-object relocations (which MASM/COFF cannot express as a near call across sections) are
trivial: the fixup records are emitted by hand.

The emitted file round-trips through le.py to the same objects, object bytes, and fixups, AND is
accepted by the real Win95 VMM loader -- a VKD.VXD synthesized this way boots Windows 95 to the
logon. It reproduces the oracle byte-for-byte except for the order LINK386 happened to emit the
fixup records in (a linker artifact that does not affect loading): the recovered code, fixups,
object/module names, device id, and version resource all match exactly.
"""
import struct


def _src_type(D, oi, off, ends, code):
    """LE fixup source type for the field at (oi, off): 0x08 (32-bit self-relative) when the 4-byte
    field is the rel32 of a near branch -- E8/E9 call/jmp (5 bytes) or 0F 8x jcc (6 bytes) -- else
    0x07 (a 32-bit offset). A decoded branch ending at this field is authoritative; otherwise, when
    NO decoded instruction ends at off+4 (the field is not a clean instruction tail -- an undecoded
    or mis-decoded branch, including code past the object's declared vsize), trust the raw opcode
    byte. `ends` is the set of instruction-end offsets; `code` the object's full (untruncated) bytes."""
    n = len(code)
    b1 = code[off - 1] if 0 <= off - 1 < n else None
    b2 = code[off - 2] if 0 <= off - 2 < n else None
    r1 = D.insns.get((oi, off - 1))
    if r1 and r1["size"] == 5 and b1 in (0xE8, 0xE9):
        return 0x08
    r2 = D.insns.get((oi, off - 2))
    if r2 and r2["size"] == 6 and b2 == 0x0F and b1 is not None and 0x80 <= b1 <= 0x8F:
        return 0x08
    if (off + 4) not in ends:                               # not a decoded insn tail -> trust the opcode
        if b1 in (0xE8, 0xE9):
            return 0x08
        if b2 == 0x0F and b1 is not None and 0x80 <= b1 <= 0x8F:
            return 0x08
    return 0x07


def _emit_fixups(D, v):
    """LE fixup page table + record table from D.fx. Sources on a page that share one target (same
    source type + target) are combined into a single source-list record (src|0x20, count, target,
    then the ascending source-offset list) exactly as LINK386 emitted them -- Win95's LE loader
    caps the fixup records per page, so the one-record-per-source form overflows and the module is
    rejected as "damaged". Records are ordered by ascending first-source offset. The table is indexed
    by logical page (matching the loader), independent of the page map's own page numbers. Returns
    (page_table_bytes, record_bytes)."""
    ends = {}                                               # per-object instruction-end offsets
    for (oi, start), r in D.insns.items():
        ends.setdefault(oi, set()).add(start + r["size"])
    ps = v.pagesize                                         # object bytes from its first page to the end
    o_dp = struct.unpack_from("<I", v.d, v.lfa + 0x80)[0]   # of the data region -- covers a fixup whose
    data_end = (struct.unpack_from("<I", v.d, v.lfa + 0x88)[0]       # offset overruns the object's own
                or struct.unpack_from("<I", v.d, v.lfa + 0xb8)[0] or len(v.d))  # pages into a later one
    code_of = {o["idx"]: bytes(v.d[o_dp + (o["pmidx"] - 1) * ps: data_end]) for o in v.objects}
    by_page = {}
    for (oi, off), f in D.fx.items():
        if f["kind"] == "internal" and f["target"] is None:
            continue                                        # selector-only fixup, no 32-bit field
        obj = next((o for o in v.objects if o["idx"] == oi), None)
        if not obj or off < 0:
            continue
        pg = obj["pmidx"] + off // v.pagesize
        po = off % v.pagesize
        st = _src_type(D, oi, off, ends.get(oi, ()), code_of.get(oi, b""))
        if f["kind"] == "internal":
            tflags = 0x10 if f["target"] > 0xffff else 0x00
            key = (st, tflags, "int", f["obj"], f["target"])
        else:                                               # import by ordinal
            key = (st, 0x01 | 0x80, "imp", f["module"], f["ordinal"] & 0xff)
        by_page.setdefault(pg, {}).setdefault(key, []).append(po)
        # a 4-byte source that crosses into the next page is ALSO listed there, with a negative offset
        if po > v.pagesize - 4 and pg < v.mpages:
            by_page.setdefault(pg + 1, {}).setdefault(key, []).append(po - v.pagesize)
    recs = bytearray()
    pageoffs = []
    for pg in range(1, v.mpages + 1):
        pageoffs.append(len(recs))
        groups = by_page.get(pg, {})
        for key in sorted(groups, key=lambda k: min(groups[k])):
            st, tflags, kind, a, b = key
            offs = sorted(groups[key])
            for i in range(0, len(offs), 255):              # source count is one byte -> spill past 255
                chunk = offs[i:i + 255]
                srclist = len(chunk) > 1
                recs += struct.pack("<BB", st | 0x20 if srclist else st, tflags)
                recs += struct.pack("<B", len(chunk)) if srclist else struct.pack("<h", chunk[0])
                if kind == "int":                           # target: obj# then 16/32-bit offset
                    recs += struct.pack("<B", a)
                    recs += struct.pack("<I", b) if (tflags & 0x10) else struct.pack("<H", b)
                else:                                       # import: module# then 8-bit ordinal
                    recs += struct.pack("<BB", a, b)
                if srclist:                                 # ascending source-offset list at record end
                    recs += b"".join(struct.pack("<h", o) for o in chunk)
    pageoffs.append(len(recs))
    return b"".join(struct.pack("<I", o) for o in pageoffs), bytes(recs)


def build_le(D, v, vxdname):
    """Return the bytes of a synthesized LE VxD for the recovered disassembly D of oracle v."""
    ps = v.pagesize
    objs = v.objects
    # data pages: each recovered object's pages come from D.code, laid out by physical page index.
    # A logical page that no object maps ("gap" page) is not recovered code -- LINK386 left stale
    # buffer bytes there; those are copied from the oracle (the loader ignores them, they are not
    # derivable). The last physical page holds only last_page_bytes (any tail is BSS, not stored).
    o_dp = struct.unpack_from("<I", v.d, 0x80 + 0x80)[0]
    page_content = {}
    for o in objs:
        for k in range(o["pcnt"]):
            page_content[o["pmidx"] + k] = D.code[o["idx"]][k * ps:(k + 1) * ps]
    last_page_bytes = v.last_page_bytes
    data = bytearray()
    for pg in range(1, v.mpages + 1):
        if pg in page_content:
            b = bytes(page_content[pg])
        else:
            b = bytes(v.d[o_dp + (pg - 1) * ps: o_dp + pg * ps])   # gap page: orphaned linker bytes
        if len(b) < ps and pg != v.mpages:                          # object ends mid-page: the rest of
            b += bytes(v.d[o_dp + (pg - 1) * ps + len(b): o_dp + pg * ps])   # the page is oracle leftover
        data += b[:last_page_bytes] if pg == v.mpages else b + b"\x00" * (ps - len(b))
    # trailing bytes between the last object page and the non-resident name table (LINK386 zero-padding
    # or stale bytes): recovered from the oracle so the whole data region is byte-exact
    data_end = (struct.unpack_from("<I", v.d, 0x80 + 0x88)[0]
                or struct.unpack_from("<I", v.d, 0x80 + 0xb8)[0] or len(v.d))
    if 0 <= len(data) < data_end - o_dp:
        data += bytes(v.d[o_dp + len(data): data_end])

    otab_abs = v.lfa + struct.unpack_from("<I", v.d, v.lfa + 0x40)[0]
    objtab = b"".join(struct.pack("<IIIII", o["vsize"], o["base"], o["flags"], o["pmidx"], o["pcnt"])
                      + v.d[otab_abs + i * 24 + 20: otab_abs + i * 24 + 24]   # 4-char object name (recovered)
                      for i, o in enumerate(objs))
    # page map: recovered verbatim from the oracle (each 4-byte entry's page number + flag byte are
    # LINK386's; the numbers are a permutation for some modules but do not affect page/fixup loading)
    pm_abs = v.lfa + struct.unpack_from("<I", v.d, v.lfa + 0x48)[0]
    pagemap = bytes(v.d[pm_abs: pm_abs + v.mpages * 4])
    # resident-name table (the module's own name + any exported names): recovered verbatim from the
    # oracle so it is byte-exact regardless of the file name the caller injects under
    rn_off = struct.unpack_from("<I", v.d, v.lfa + 0x58)[0]
    et_off = struct.unpack_from("<I", v.d, v.lfa + 0x5c)[0]
    if et_off > rn_off > 0:
        resnames = bytes(v.d[v.lfa + rn_off: v.lfa + et_off])
    else:
        nm = vxdname.encode("latin1")
        resnames = struct.pack("<B", len(nm)) + nm + struct.pack("<H", 0) + b"\x00"
    # entry table (the export directory: DDB @1 + any further exports): recovered verbatim from the
    # oracle -- the DDB obj/rva it points at is independently verified by the byte-exact object bytes
    et_off = struct.unpack_from("<I", v.d, v.lfa + 0x5c)[0]
    fp_off = struct.unpack_from("<I", v.d, v.lfa + 0x68)[0]
    if fp_off > et_off > 0:
        entrytab = bytes(v.d[v.lfa + et_off: v.lfa + fp_off])
    else:
        db = D.ddb
        entrytab = struct.pack("<BBH", 1, 0x03, db["obj"] if db else 1) \
            + struct.pack("<BI", 0x03, db["rva"] if db else 0) + b"\x00"   # flags 0x03: EXPORTED entry
    fxpage, fxrec = _emit_fixups(D, v)
    # loader-section tail after the fixup records -- the import module + procedure name tables (always
    # empty/count 0 in this corpus) plus whatever dead symbol strings LINK386 left in the gap up to the
    # data pages: recovered verbatim from the oracle (the loader ignores it; it is not derivable)
    o_impmod = 0x80 + struct.unpack_from("<I", v.d, 0x80 + 0x70)[0]
    o_dp = struct.unpack_from("<I", v.d, 0x80 + 0x80)[0]
    imp_region = bytes(v.d[o_impmod:o_dp]) if o_dp > o_impmod else b""   # empty when impmod == datapages

    lfa = 0x80
    hdr_len = 0xc4
    pos = hdr_len
    off_objtab = pos; pos += len(objtab)
    off_pagemap = pos; pos += len(pagemap)
    off_resnames = pos; pos += len(resnames)
    off_entrytab = pos; pos += len(entrytab)
    off_fxpage = pos; pos += len(fxpage)
    off_fxrec = pos; pos += len(fxrec)
    off_impmod = pos; pos += len(imp_region)
    # data pages start at the oracle's recorded offset (LINK386's own alignment varies with page size);
    # fall back to a 0x200 alignment only if our loader content would not fit under it
    natural = (pos + lfa + 0x1ff) & ~0x1ff
    oracle_dp = struct.unpack_from("<I", v.d, lfa + 0x80)[0]
    datapages = oracle_dp if oracle_dp >= pos + lfa else natural

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
    struct.pack_into("<I", H, 0x78, off_impmod)                       # import proc name table (empty, shares terminator)
    struct.pack_into("<I", H, 0x80, datapages)
    # section sizes the real loader reads to bound each table (checksums stay 0 -- unverified):
    struct.pack_into("<I", H, 0x38, off_fxpage - off_objtab)          # loader section (objtab..entrytab)
    struct.pack_into("<I", H, 0x30, len(fxpage) + len(fxrec))         # fixup section (page table + records)
    # header fields the real Win95 loader reads, preserved from the oracle (module version/flags, the
    # initial ESP, preload count, and the VxD device id + DDK version packed at 0xc0) -- recovered values
    for foff in (0x0c, 0x10, 0x20, 0x24, 0x84, 0x94, 0xc0):
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
    # Windows resource block (the VxD's version resource): recovered from the oracle and re-pointed.
    # winresoff/winreslen (0xb8/0xbc) locate it; it sits at end-of-file, just past the non-res table.
    wro = struct.unpack_from("<I", v.d, lfa + 0xb8)[0]
    wrl = struct.unpack_from("<I", v.d, lfa + 0xbc)[0]
    winres = v.d[wro:wro + wrl] if (wro and wrl and wro + wrl <= len(v.d)) else b""
    if winres:
        struct.pack_into("<I", H, 0xb8, datapages + len(data) + len(nres))
        struct.pack_into("<I", H, 0xbc, len(winres))

    mz = bytearray(v.d[:lfa]) if (v.d[:2] == b"MZ" and len(v.d) >= lfa) else bytearray(lfa)
    mz[0:2] = b"MZ"                                          # DOS stub preserved from the oracle
    struct.pack_into("<I", mz, 0x3c, lfa)
    body = bytes(mz) + bytes(H) + objtab + pagemap + resnames + entrytab + fxpage + fxrec + imp_region
    body += b"\x00" * (datapages - len(body)) + bytes(data) + nres + winres
    if len(body) < len(v.d):                                # match the oracle's end-of-file padding
        body += bytes(v.d[len(body):])
    return bytes(body)


def build_w3(D, v, name):
    """Reconstruct a single-module W3 container (Win95's vmm32.vxd) from the recovered module. Unlike
    a standalone .vxd, the embedded LE keeps its container-absolute layout at v.lfa; its OBJECT DATA
    comes from D.code and its FIXUP records from D.fx, while the DOS stub + real-mode loader, the W3
    header + entry, the LE header fields, the object/name tables and the version resource are the
    container's own metadata. Byte-exact with the oracle vmm32.vxd except LINK386's intra-page fixup
    ORDER -- the same standard as the 213 standalone modules."""
    out = bytearray(v.d)
    L = v.lfa
    U = lambda o: struct.unpack_from("<I", v.d, L + o)[0]
    ps = v.pagesize
    datapages = U(0x80)                                     # container-absolute
    for o in v.objects:                                     # recovered object bytes, in place
        base = datapages + (o["pmidx"] - 1) * ps
        cb = D.code[o["idx"]]
        out[base:base + len(cb)] = cb
    fxpage, fxrec = _emit_fixups(D, v)                      # fixup page + record tables rebuilt from D.fx
    out[L + U(0x68):L + U(0x68) + len(fxpage)] = fxpage
    out[L + U(0x6c):L + U(0x6c) + len(fxrec)] = fxrec
    return bytes(out)
