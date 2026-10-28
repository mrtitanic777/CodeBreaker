"""Synthesize an LE VxD directly from the recovered disassembly -- object bytes from D.code, the
relocation table from D.fx -- instead of assembling + linking. This sidesteps LINK/MASM entirely,
so cross-object relocations (which MASM/COFF cannot express as a near call across sections) are
trivial: the fixup records are emitted by hand. The file itself is written by formats/le.py's
write_image, the one place LE files are written, told which of the original linker's layout
choices to keep.

The emitted file round-trips through le.py to the same objects, object bytes, and fixups, AND is
accepted by the real Win95 VMM loader -- a VKD.VXD synthesized this way boots Windows 95 to the
logon. It reproduces the oracle byte-for-byte except for the order LINK386 happened to emit the
fixup records in (a linker artifact that does not affect loading): the recovered code, fixups,
object/module names, device id, and version resource all match exactly.
"""
import struct

from codebreaker.formats import le


def _src_type(D, oi, off, ends, code):
    """LE fixup source type for the field at (oi, off): 0x08 (32-bit self-relative) when the 4-byte
    field is the rel32 of a near branch -- E8/E9 call/jmp (5 bytes) or 0F 8x jcc (6 bytes) -- else
    0x07 (a 32-bit offset). A decoded branch ending at this field is authoritative; otherwise, when
    NO decoded instruction ends at off+4 (the field is not a clean instruction tail -- an undecoded
    or mis-decoded branch, including code past the object's declared vsize), trust the raw opcode
    byte. `ends` is the set of instruction-end offsets; `code` the object's full (untruncated) bytes.

    A recovered fix-up that says its own source type (D.fx's internal references do) keeps it: the
    shape of the code cannot always tell -- VMM has `cmp eax, imm32` fields relocated self-relative."""
    told = (D.fx.get((oi, off)) or {}).get("srctype")
    if told is not None:
        return told
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


def _oracle_pages(v, first=None):
    """The oracle's data pages in page-map order, each found by its number in the page map (a
    linker may number them out of order: the loader fetches a page, and its fix-up row, by that
    number): ([(number, the page's place in the file as bytes)], the highest number). A place runs a
    whole page; the highest-numbered page holds only last_page_bytes of it. `first`: where the
    data pages start (default: as the header says)."""
    if first is None:
        first = struct.unpack_from("<I", v.d, v.lfa + 0x80)[0]
    ps = v.pagesize
    pm = le.page_map(v)
    return [(n, bytes(v.d[first + (n - 1) * ps: first + n * ps])) for n, _k in pm], max([n for n, _k in pm] or [0])


def _emit_fixups(D, v):
    """LE fixup page table + record table from D.fx. Sources on a page that share one target (same
    source type + target) are combined into a single source-list record (src|0x20, count, target,
    then the ascending source-offset list) exactly as LINK386 emitted them -- Win95's LE loader
    caps the fixup records per page, so the one-record-per-source form overflows and the module is
    rejected as "damaged". Records are ordered by ascending first-source offset. Returns {page:
    [record bytes]} by the page's place in the page map (le.write_image puts each page's records in
    the row of the page's number, as the loader reads them), encoded and laid into the fix-up tables
    by formats/le.py as every LE's are."""
    ends = {}                                               # per-object instruction-end offsets
    for (oi, start), r in D.insns.items():
        ends.setdefault(oi, set()).add(start + r["size"])
    ps = v.pagesize                                         # object bytes from its first page to the end
    o_dp = struct.unpack_from("<I", v.d, v.lfa + 0x80)[0]   # of the module's data pages -- covers a fixup
    # whose offset overruns the object's own pages into a later object. Derive the end from the page
    # layout (container-absolute), NOT the nres field: W3 bundling leaves nres module-relative, which
    # would make this empty for a bundled module and break every src_type read.
    data_end = min(o_dp + (v.mpages - 1) * ps + v.last_page_bytes, len(v.d))
    # the pages in page-map order (each found by its number), the highest-numbered one cut at data_end
    found, top = _oracle_pages(v)
    logical = [b if n != top else b[:max(0, data_end - (o_dp + (n - 1) * ps))] for n, b in found]
    code_of = {o["idx"]: b"".join(logical[o["pmidx"] - 1:]) for o in v.objects}
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
    records = {}
    for pg in range(1, v.mpages + 1):
        groups = by_page.get(pg, {})
        records[pg] = []
        for key in sorted(groups, key=lambda k: min(groups[k])):
            st, tflags, kind, a, b = key
            offs = sorted(groups[key])
            for i in range(0, len(offs), 255):              # source count is one byte -> spill past 255
                chunk = offs[i:i + 255]
                rec = {"source": st, "source_flags": 0x20 if len(chunk) > 1 else 0, "flags": tflags,
                       "sources": chunk}
                if kind == "int":                           # target: obj# then 16/32-bit offset
                    rec.update(kind="internal", obj=a, target=b)
                else:                                       # import: module# then 8-bit ordinal
                    rec.update(kind="import", module=a, ordinal=b)
                records[pg].append(le.encode_fixup(rec))
    return records


def build_le(D, v, vxdname):
    """Return the bytes of a synthesized LE VxD for the recovered disassembly D of oracle v: its
    object pages from D.code and its fix-up records from D.fx, written by le.write_image in the
    original linker's (LINK386's) layout, whose choices are read from the oracle. The header goes
    at 0x80, behind the oracle's DOS stub, and that layout is read from the oracle's header at
    0x80 too -- for the few files whose header sits elsewhere, those are the bytes their rebuilt
    drivers have always had."""
    ps = v.pagesize
    lfa = 0x80
    at = lambda off: struct.unpack_from("<I", v.d, lfa + off)[0]       # noqa: E731
    o_dp = at(0x80)
    # data pages: each recovered object's pages come from D.code, laid out by logical page index.
    # A logical page that no object maps ("gap" page) is not recovered code -- LINK386 left stale
    # buffer bytes there; those are copied from the oracle (the loader ignores them, they are not
    # derivable). The last page holds only last_page_bytes (any tail is BSS, not stored).
    # The oracle's own page bytes are those at each page's NUMBER in its page map (LINK386 numbered
    # some drivers' pages out of order); le.write_image stores each page at its number again.
    page_content = {}
    for o in v.objects:
        for k in range(o["pcnt"]):
            page_content[o["pmidx"] + k] = D.code[o["idx"]][k * ps:(k + 1) * ps]
    found, top = _oracle_pages(v, o_dp)
    pages = []
    for pg, (number, own) in enumerate(found, 1):
        if pg in page_content:
            b = bytes(page_content[pg])
        else:
            b = own                                                 # gap page: orphaned linker bytes
        if len(b) < ps and number != top:                           # object ends mid-page: the rest of
            b += own[len(b):]                                       # the page is oracle leftover
        pages.append((0, b[:v.last_page_bytes] if number == top else b + b"\x00" * (ps - len(b))))
    # trailing bytes between the last object page and the non-resident name table (LINK386 zero-padding
    # or stale bytes): recovered from the oracle so the whole data region is byte-exact
    stored = sum(len(b) for _kind, b in pages)
    data_end = at(0x88) or at(0xb8) or len(v.d)
    names_gap = bytes(v.d[o_dp + stored: data_end]) if 0 <= stored < data_end - o_dp else b""

    otab_abs = v.lfa + struct.unpack_from("<I", v.d, v.lfa + 0x40)[0]
    objects = [{"vsize": o["vsize"], "base": o["base"], "flags": o["flags"], "first_page": o["pmidx"],
                "page_count": o["pcnt"],
                "reserved": bytes(v.d[otab_abs + i * 24 + 20: otab_abs + i * 24 + 24])}   # 4-char object name
               for i, o in enumerate(v.objects)]                                          # (recovered)
    # page map: recovered verbatim from the oracle (each 4-byte entry's page number + flag byte are
    # LINK386's; the numbers are a permutation for some modules, and write_image stores each page and
    # its fix-up row at its number)
    pm_abs = v.lfa + struct.unpack_from("<I", v.d, v.lfa + 0x48)[0]
    # resident-name table (the module's own name + any exported names): recovered verbatim from the
    # oracle so it is byte-exact regardless of the file name the caller injects under
    rn_off = struct.unpack_from("<I", v.d, v.lfa + 0x58)[0]
    et_off = struct.unpack_from("<I", v.d, v.lfa + 0x5c)[0]
    resident = bytes(v.d[v.lfa + rn_off: v.lfa + et_off]) if et_off > rn_off > 0 else [(vxdname, 0)]
    # entry table (the export directory: DDB @1 + any further exports): recovered verbatim from the
    # oracle -- the DDB obj/rva it points at is independently verified by the byte-exact object bytes
    fp_off = struct.unpack_from("<I", v.d, v.lfa + 0x68)[0]
    if fp_off > et_off > 0:
        entrytab = bytes(v.d[v.lfa + et_off: v.lfa + fp_off])
    else:
        db = D.ddb
        entrytab = struct.pack("<BBH", 1, 0x03, db["obj"] if db else 1) \
            + struct.pack("<BI", 0x03, db["rva"] if db else 0) + b"\x00"   # flags 0x03: EXPORTED entry
    # loader-section tail after the fixup records -- the import module + procedure name tables (always
    # empty/count 0 in this corpus) plus whatever dead symbol strings LINK386 left in the gap up to the
    # data pages: recovered verbatim from the oracle as the tables' tail (the loader ignores it; it is
    # not derivable), zero-padded up to the data pages. So the import tables are empty, and the fixup
    # section size counts the fixup tables alone.
    o_impmod = lfa + at(0x70)
    imp_region = bytes(v.d[o_impmod:o_dp]) if o_dp > o_impmod else b""   # empty when impmod == datapages

    H = bytearray(le.HEADER_SIZE)
    struct.pack_into("<2sBBI", H, 0, b"LE", 0, 0, 0)
    struct.pack_into("<HH", H, 0x08, v.cpu, v.ostype)
    struct.pack_into("<II", H, 0x18, v.eip_obj, v.eip)
    struct.pack_into("<I", H, 0x28, ps)
    # header fields the real Win95 loader reads, preserved from the oracle (module version/flags, the
    # initial ESP, preload count, and the VxD device id + DDK version packed at 0xc0) -- recovered values
    for foff in (0x0c, 0x10, 0x20, 0x24, 0x84, 0x94, 0xc0):
        struct.pack_into("<I", H, foff, at(foff))
    # non-resident name table (the module description string): preserved, placed after the data pages
    nres_off, nres_sz = at(0x88), at(0x8c)
    nres = v.d[nres_off:nres_off + nres_sz] if (nres_off and nres_sz and nres_off + nres_sz <= len(v.d)) else b""
    if nres and not (0 < nres[0] < 64 and all(31 < c < 127 or c == 9 for c in nres[1:1 + nres[0]])):
        nres = b""                                          # not a plausible description (e.g. a W3 module's container-relative offset)
    # Windows resource block (the VxD's version resource): recovered from the oracle and re-pointed.
    # winresoff/winreslen (0xb8/0xbc) locate it; it sits at end-of-file, just past the non-res table.
    wro, wrl = at(0xb8), at(0xbc)
    winres = v.d[wro:wro + wrl] if (wro and wrl and wro + wrl <= len(v.d)) else b""
    if winres:
        struct.pack_into("<I", H, 0xbc, len(winres))

    mz = bytearray(v.d[:lfa]) if (v.d[:2] == b"MZ" and len(v.d) >= lfa) else bytearray(lfa)
    mz[0:2] = b"MZ"                                          # DOS stub preserved from the oracle
    struct.pack_into("<I", mz, 0x3c, lfa)
    # the data pages start at the oracle's recorded offset (LINK386's own alignment varies with page
    # size), at a 0x200 alignment only if our loader content would not fit under it; a file that comes
    # out shorter than the oracle ends with the oracle's own end-of-file padding
    return le.write_image({
        "stub": bytes(mz), "header": bytes(H), "objects": objects, "pages": pages,
        "page_map": bytes(v.d[pm_abs: pm_abs + v.mpages * 4]), "last_page_bytes": v.last_page_bytes,
        "resident": resident, "nonresident": bytes(nres), "entry_table": entrytab,
        "fixups": _emit_fixups(D, v), "import_modules": [], "import_names": b"",
        "tables_tail": imp_region, "align": 0x200, "data_pages": o_dp, "gaps": (b"", names_gap),
        "after": bytes(winres),
        "moved": {"debug_info": 0, "resource_data": wro if winres else 0, "from": wro},
        "fill_from": bytes(v.d)})


def build_w3(D, v, name):
    """Reconstruct a single-module W3 container (Win95's vmm32.vxd) from the recovered module. Unlike
    a standalone .vxd, the embedded LE keeps its container-absolute layout at v.lfa; its OBJECT DATA
    comes from D.code and its FIXUP records from D.fx, while the DOS stub + real-mode loader, the W3
    header + entry, the LE header fields, the object/name tables and the version resource are the
    container's own metadata.

    The member is written by le.write_image, like build_le's drivers, in front of it the
    container's own program and directory; the directory is then packed again with the member's
    size as written. D.fx gives more fix-up records than LINK386 wrote (Win95's VMM: 3824 against
    3804, 98 bytes more; Win98's: 48 bytes more), so the fix-up section, the import-table offsets
    and the member's size grow with them, and where the tables then run into the data pages
    (Win95's VMM) the pages move to the next 0x200, the non-resident names and resource with them."""
    L = v.lfa
    U = lambda o: struct.unpack_from("<I", v.d, L + o)[0]       # noqa: E731
    ps = v.pagesize
    first = U(0x80)                                             # data pages, container-absolute
    # data pages: the container's own in page-map order (each found by its number), each recovered
    # object's bytes laid over them at its first page, in object order (an object whose recovered
    # bytes run past its pages -- its uninitialised tail -- spills into the next page, and the next
    # object is laid over that); le.write_image stores each page at its number again
    found, top = _oracle_pages(v)
    data = bytearray(b"".join(b.ljust(ps, b"\0") for _n, b in found))
    for o in v.objects:
        at = (o["pmidx"] - 1) * ps
        cb = D.code[o["idx"]]
        data[at:at + len(cb)] = cb
    pages = [(0, bytes(data[i * ps:(i + 1) * ps])[:v.last_page_bytes if n == top else ps])
             for i, (n, _b) in enumerate(found)]
    otab = L + U(0x40)
    objects = [{"vsize": o["vsize"], "base": o["base"], "flags": o["flags"], "first_page": o["pmidx"],
                "page_count": o["pcnt"], "reserved": bytes(v.d[otab + i * 24 + 20: otab + i * 24 + 24])}
               for i, o in enumerate(v.objects)]
    # the loader's tables and the names as the container stores them, but the fix-up tables rebuilt
    # from D.fx; the container's own bytes behind them up to the data pages are kept where they lie
    # (the data pages move to the next 0x200 only when the rebuilt tables run into them). The
    # non-resident names and the version resource sit among the data pages, and move with them.
    fixups = _emit_fixups(D, v)
    fix_pages, fix_records = le.fixup_tables(fixups, v.mpages)
    tables_end = L + U(0x68) + len(fix_pages) + len(fix_records)
    nres_off, nres_sz = U(0x88), U(0x8c)
    out = bytearray(le.write_image({
        "stub": bytes(v.d[:L]), "header": bytes(v.d[L:L + le.HEADER_SIZE]), "objects": objects,
        "pages": pages, "page_map": bytes(v.d[L + U(0x48): L + U(0x48) + v.mpages * 4]),
        "last_page_bytes": v.last_page_bytes,
        "resident": bytes(v.d[L + U(0x58): L + U(0x5c)]), "entry_table": bytes(v.d[L + U(0x5c): L + U(0x68)]),
        "nonresident": bytes(v.d[nres_off:nres_off + nres_sz]), "fixups": fixups,
        "import_modules": [], "import_names": b"", "tables_tail": bytes(v.d[tables_end:first]),
        "align": 0x200, "data_pages": first, "gaps": (b"", b""),
        "in_pages": {"nonresident_names": nres_off, "resource_data": U(0xb8)},
        "after": b"", "moved": {"debug_info": U(0x98), "resource_data": 0, "from": len(v.d)},
        "fill_from": bytes(v.d)}))
    # the container's directory, each member's size being its header and loader tables as written
    c = le.container(v.d)
    loader_end = lambda at: struct.unpack_from("<I", out, at + 0x68)[0] + struct.unpack_from("<I", out, at + 0x30)[0]   # noqa: E731
    c["members"] = [(nm, off, loader_end(off) if off == L else sz) for nm, off, sz in c["members"]]
    directory = le.pack_directory(c)
    out[c["at"]:c["at"] + len(directory)] = directory
    return bytes(out)
