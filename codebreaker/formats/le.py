r"""le.py -- Linear Executable (LE) loader for Win95 Virtual Device Drivers (.vxd / .386).

VxDs are Ring-0 32-bit drivers in the LE format (not the 16-bit NE of the system DLLs), so
they need their own loader. This parses the LE header, the object table, and extracts each
object's bytes from the data pages, and it locates + decodes the DDB (Device Descriptor Block)
-- the VxD's identity: its 8-char device name, DDK version, device id, and control procedure.
That identity is the anchor for reconstructing the driver as clean MASM (its VxD_Desc_Block,
Begin_Control_Dispatch, and service table).

    from codebreaker.formats import le
    v = le.LEFile.from_file("vmouse.vxd")
    print(v.ddb())                       # {'name': 'VMD', 'version': 0x40a, 'device_id': ...}
    code = v.object_bytes(v.objects[0])  # 32-bit code of the first object
"""
import struct


class LEFile:
    def __init__(s, data, lfa):
        s.d = data; s.lfa = lfa
        u = lambda o: struct.unpack_from("<I", data, lfa + o)[0]
        h = lambda o: struct.unpack_from("<H", data, lfa + o)[0]
        s.sig = data[lfa:lfa + 2]
        s.cpu = h(0x08); s.ostype = h(0x0a)
        s.mpages = u(0x14); s.eip_obj = u(0x18); s.eip = u(0x1c)
        s.pagesize = u(0x28); s.last_page_bytes = u(0x2c)
        s.objtab = lfa + u(0x40); s.nobj = u(0x44); s.pagemap = lfa + u(0x48)
        s.resnames = lfa + u(0x58); s.entrytab = lfa + u(0x5c)
        s.fixup_page = lfa + u(0x68); s.fixup_rec = lfa + u(0x6c)
        s.datapages = u(0x80)                          # offset from top of file (NOT 0x7c)
        s.objects = []
        for i in range(s.nobj):
            vsize, base, flags, pmidx, pcnt = struct.unpack_from("<IIIII", data, s.objtab + i * 24)
            s.objects.append({"idx": i + 1, "vsize": vsize, "base": base, "flags": flags,
                              "pmidx": pmidx, "pcnt": pcnt,
                              "exec": bool(flags & 0x04), "read": bool(flags & 0x01),
                              "write": bool(flags & 0x02)})

    @classmethod
    def from_buffer(cls, data):
        lfa = struct.unpack_from("<I", data, 0x3c)[0]
        if data[lfa:lfa + 2] not in (b"LE", b"LX"):
            raise ValueError("not an LE/LX image (sig=%r)" % data[lfa:lfa + 2])
        return cls(data, lfa)

    @classmethod
    def from_file(cls, path):
        return cls.from_buffer(open(path, "rb").read())

    def object_bytes(s, obj):
        """Raw bytes of one object, assembled from its data pages (contiguous in practice for
        VxDs), truncated to the object's virtual size."""
        off = s.datapages + (obj["pmidx"] - 1) * s.pagesize
        return s.d[off:off + obj["vsize"]]

    def page_to_obj(s, pg):
        """Logical page number (1-based) -> (object, byte offset of the page within the object)."""
        for obj in s.objects:
            if obj["pmidx"] <= pg < obj["pmidx"] + obj["pcnt"]:
                return obj, (pg - obj["pmidx"]) * s.pagesize
        return None, 0

    def fixups(s):
        """Parse the LE fixup page + record tables into {(obj_idx, off_in_obj): fixup}. Each
        fixup is {'kind':'internal', 'obj', 'target'} (an intra-image code/data reference -> a
        label) or {'kind':'import', 'module', 'ordinal'} (a VMM/VxD service imported by
        module:ordinal -> a named call). This is what turns raw operands into symbols."""
        d = s.d; out = {}
        pageoffs = [struct.unpack_from("<I", d, s.fixup_page + i * 4)[0] for i in range(s.mpages + 1)]
        for pg in range(s.mpages):                          # fixups for logical page pg+1
            obj, pbase = s.page_to_obj(pg + 1)
            p = s.fixup_rec + pageoffs[pg]; end = s.fixup_rec + pageoffs[pg + 1]
            while p < end:
                src = d[p]; tflags = d[p + 1]; p += 2
                srclist = bool(src & 0x20)                  # SRC bit5: multiple sources, ONE target
                if srclist:
                    cnt = d[p]; p += 1                      # source count (offset list is at record END)
                else:
                    soff = struct.unpack_from("<h", d, p)[0]; p += 2
                tt = tflags & 0x03
                fx = None
                if tt == 0x00:                              # internal reference
                    objn = struct.unpack_from("<H", d, p)[0] if (tflags & 0x40) else d[p]
                    p += 2 if (tflags & 0x40) else 1
                    if (src & 0x0f) == 0x02:                # 16-bit selector, no offset
                        tv = None
                    elif tflags & 0x10:
                        tv = struct.unpack_from("<I", d, p)[0]; p += 4
                    else:
                        tv = struct.unpack_from("<H", d, p)[0]; p += 2
                    fx = {"kind": "internal", "obj": objn, "target": tv, "srctype": src & 0x0f}
                elif tt == 0x01:                            # import by ordinal (VMM/VxD service)
                    mod = struct.unpack_from("<H", d, p)[0] if (tflags & 0x40) else d[p]
                    p += 2 if (tflags & 0x40) else 1
                    if tflags & 0x80:
                        ordv = d[p]; p += 1
                    elif tflags & 0x10:
                        ordv = struct.unpack_from("<I", d, p)[0]; p += 4
                    else:
                        ordv = struct.unpack_from("<H", d, p)[0]; p += 2
                    fx = {"kind": "import", "module": mod, "ordinal": ordv}
                else:                                       # import-by-name / entry -- stop this page
                    break
                if tflags & 0x04:                           # additive value follows the target
                    p += 4 if (tflags & 0x20) else 2
                if srclist:                                 # the SRC offset list lives at the record END
                    offs = [struct.unpack_from("<h", d, p + j * 2)[0] for j in range(cnt)]; p += cnt * 2
                else:
                    offs = [soff]
                if obj is not None:
                    for o in offs:
                        out[(obj["idx"], pbase + o)] = fx
        return out

    def import_modules(s):
        """The imported module names (VMM, VPICD, ...) from the Imported Modules Name Table."""
        import struct as _s
        off = s.lfa + _s.unpack_from("<I", s.d, s.lfa + 0x70)[0]
        cnt = _s.unpack_from("<I", s.d, s.lfa + 0x74)[0]
        names = []; p = off
        for _ in range(cnt):
            n = s.d[p]; names.append(s.d[p + 1:p + 1 + n].decode("latin1", "replace")); p += 1 + n
        return names

    def resident_name(s):
        """The module's own name (first entry of the resident-name table, length-prefixed)."""
        n = s.d[s.resnames]
        return s.d[s.resnames + 1:s.resnames + 1 + n].decode("latin1", "replace")

    def header(s):
        """The header's fields by name -- what an inspector shows (used by formats/inspect.py)."""
        u = lambda o: struct.unpack_from("<I", s.d, s.lfa + o)[0] if s.lfa + o + 4 <= len(s.d) else 0
        h = lambda o: struct.unpack_from("<H", s.d, s.lfa + o)[0] if s.lfa + o + 2 <= len(s.d) else 0
        return {"signature": s.sig.decode("latin1"), "cpu": s.cpu, "os": s.ostype, "module_version": u(0x0c),
                "module_flags": u(0x10), "pages": s.mpages, "entry_object": s.eip_obj, "entry": s.eip,
                "stack_object": u(0x20), "stack": u(0x24), "page_size": s.pagesize,
                "last_page_bytes": s.last_page_bytes, "fixup_section_size": u(0x30),
                "loader_section_size": u(0x38), "objects": s.nobj, "imported_modules": u(0x74),
                "data_pages_offset": s.datapages, "device_id": h(0xC0), "ddk_version": h(0xC2)}

    def description(s):
        """The module's description line: the first entry of the non-resident name table ('' when
        there is none)."""
        off = struct.unpack_from("<I", s.d, s.lfa + 0x88)[0] if s.lfa + 0x8c <= len(s.d) else 0
        if not 0 < off < len(s.d):
            return ""
        return s.d[off + 1:off + 1 + s.d[off]].decode("latin1", "replace")

    def ddb(s):
        """Locate and decode the Device Descriptor Block. Scan each object's bytes for the DDB
        signature: a dword-aligned DDK version (0x03xx/0x04xx) followed by device id, a
        major/minor byte pair, flags, and an 8-byte device name of printable ASCII. Returns the
        decoded DDB or None. The name is what `Declare_Virtual_Device <name>` emits."""
        def decode(obj, b, off):
            name = b[off + 8:off + 16]
            if not name or not all(32 <= c < 127 for c in name):
                return None
            if not (name[0:1].isalpha() or name[0:1].isdigit()):
                return None
            return {"obj": obj["idx"], "rva": off - 4,
                    "version": b[off] | (b[off + 1] << 8),
                    "device_id": b[off + 2] | (b[off + 3] << 8),
                    "major": b[off + 4], "minor": b[off + 5],
                    "flags": b[off + 6] | (b[off + 7] << 8),
                    "name": name.rstrip(b" \x00").decode("latin1", "replace"),
                    "init_order": struct.unpack_from("<I", b, off + 16)[0],
                    "control_proc": struct.unpack_from("<I", b, off + 20)[0]}
        for obj in s.objects:                                  # strict DDK version stamps -- the common case
            b = s.object_bytes(obj)
            for off in range(0, len(b) - 26, 4):
                if (b[off] | (b[off + 1] << 8)) not in (0x030a, 0x0400, 0x040a):
                    continue
                d = decode(obj, b, off)
                if d:
                    return d
        # Fallback: some VxDs stamp a DDK version outside the common set (0x0300 DSVXD,
        # 0x030b DECCORE/VNetSup). Accept any 0x03xx/0x04xx stamp, but require the candidate's
        # device id to equal the one in the LE header (+0xC0) -- this both broadens the version
        # match and rejects a stray version-looking dword (deccore has a 0x03d0/name-like blob
        # whose device id does not match). Runs only when the strict scan found nothing.
        try:
            hdr_dev = struct.unpack_from("<H", s.d, s.lfa + 0xC0)[0]
        except Exception:
            hdr_dev = 0
        if hdr_dev:
            for obj in s.objects:
                b = s.object_bytes(obj)
                for off in range(0, len(b) - 26, 4):
                    ver = b[off] | (b[off + 1] << 8)
                    if (ver >> 8) not in (3, 4):
                        continue
                    if (b[off + 2] | (b[off + 3] << 8)) != hdr_dev:
                        continue
                    d = decode(obj, b, off)
                    if d:
                        return d
        return None


class W3File:
    r"""W3 container (e.g. vmm32.vxd): a bundle of concatenated LE images. The header is 'W3',
    a version word, a module count dword, then `count` 16-byte entries (8-char name + dword file
    offset + dword size) starting at +0x10. Win95 builds vmm32.vxd by packing VMM (and, on some
    machines, the statically-loaded VxDs) into one W3 so the loader maps them together."""
    def __init__(s, data, lfa):
        s.d = data; s.lfa = lfa
        s.version = struct.unpack_from("<H", data, lfa + 2)[0]
        s.count = struct.unpack_from("<I", data, lfa + 4)[0]
        s.entries = []
        p = lfa + 0x10
        for _ in range(s.count):
            name = data[p:p + 8].rstrip(b" \x00").decode("latin1", "replace")
            off, size = struct.unpack_from("<II", data, p + 8)
            s.entries.append({"name": name, "off": off, "size": size})
            p += 16

    def modules(s):
        """[(name, LEFile), ...] for each embedded LE image (offsets are the module's LE header)."""
        return [(e["name"], LEFile(s.d, e["off"])) for e in s.entries
                if s.d[e["off"]:e["off"] + 2] in (b"LE", b"LX")]

    @classmethod
    def from_file(cls, path):
        d = open(path, "rb").read()
        return cls(d, struct.unpack_from("<I", d, 0x3c)[0])


def open_vxds(path):
    """[(name, LEFile), ...] for any VxD file: one entry for a plain LE/LX, all embedded modules
    for a W3 container. The single entry point for feeding the disassembler."""
    d = open(path, "rb").read()
    lfa = struct.unpack_from("<I", d, 0x3c)[0]
    sig = d[lfa:lfa + 2]
    if sig == b"W3":
        return W3File(d, lfa).modules()
    if sig in (b"LE", b"LX"):
        import os as _os
        return [(_os.path.basename(path), LEFile(d, lfa))]
    raise ValueError("not an LE/LX/W3 image (sig=%r)" % sig)


# ================================================================================================
# The rest of what an LE image holds: every header field, the loader's tables (page map, entry
# table, names, every kind of fix-up record), the device descriptor block in full with its
# service table and entry points, service calls, compressed containers -- and writing an image
# back.
#
#     from codebreaker.formats import le
#     for name, lf in le.members("vmm32.vxd"):         # any driver file; a container's members
#         d = le.descriptor(lf)                         # every field of the descriptor block
#         le.entry_points(lf)                           # {"control": (obj, off), "service_0": ...}
#         le.service_calls(lf)                          # [(obj, off, device, service, is_jump)]
#     image = le.read_image(lf); data = le.write_image(image)       # round trip
# ================================================================================================
HEADER_SIZE = 0xC4
MODULE_STATIC, MODULE_DYNAMIC, MODULE_OLD = 0x28000, 0x38000, 0x8020     # module flags of a driver
_FIELDS = (("byte_order", 0x02, "B"), ("word_order", 0x03, "B"), ("level", 0x04, "I"), ("cpu", 0x08, "H"),
           ("os", 0x0A, "H"), ("version", 0x0C, "I"), ("module_flags", 0x10, "I"), ("pages", 0x14, "I"),
           ("eip_object", 0x18, "I"), ("eip", 0x1C, "I"), ("esp_object", 0x20, "I"), ("esp", 0x24, "I"),
           ("page_size", 0x28, "I"), ("last_page_bytes", 0x2C, "I"), ("fixup_size", 0x30, "I"),
           ("loader_size", 0x38, "I"), ("object_table", 0x40, "I"), ("objects", 0x44, "I"),
           ("page_map", 0x48, "I"), ("resources", 0x50, "I"), ("resident_names", 0x58, "I"),
           ("entry_table", 0x5C, "I"), ("fixup_pages", 0x68, "I"), ("fixup_records", 0x6C, "I"),
           ("import_modules", 0x70, "I"), ("import_module_count", 0x74, "I"), ("import_names", 0x78, "I"),
           ("data_pages", 0x80, "I"), ("preload_pages", 0x84, "I"), ("nonresident_names", 0x88, "I"),
           ("nonresident_size", 0x8C, "I"), ("debug_info", 0x98, "I"), ("debug_size", 0x9C, "I"),
           ("resource_data", 0xB8, "I"), ("resource_size", 0xBC, "I"), ("device_id", 0xC0, "H"),
           ("kit_version", 0xC2, "H"))
DESCRIPTOR = (("next", 0x00, "I"), ("kit_version", 0x04, "H"), ("device_id", 0x06, "H"), ("major", 0x08, "B"),
              ("minor", 0x09, "B"), ("flags", 0x0A, "H"), ("init_order", 0x14, "I"), ("control", 0x18, "I"),
              ("v86_api", 0x1C, "I"), ("pm_api", 0x20, "I"), ("v86_api_address", 0x24, "I"),
              ("pm_api_address", 0x28, "I"), ("reference_data", 0x2C, "I"), ("service_table", 0x30, "I"),
              ("service_count", 0x34, "I"))
_DESCRIPTOR_AT = {name: off for name, off, _fmt in DESCRIPTOR}


def header_fields(lf):
    """The LE header's fields as a dict (table offsets are as stored: relative to the header,
    except data_pages, nonresident_names, debug_info and resource_data, which are file offsets)."""
    return {name: struct.unpack_from("<" + fmt, lf.d, lf.lfa + off)[0] for name, off, fmt in _FIELDS}


def is_member(lf):
    """Is this image one inside a container (its header is not the one the file points at)?"""
    return struct.unpack_from("<I", lf.d, 0x3C)[0] != lf.lfa


def page_map(lf):
    """[(page in the file, kind)] for each of the image's pages in order. kind: 0 present,
    1 iterated, 2 invalid, 3 zero-filled."""
    out = []
    for i in range(lf.mpages):
        rec = lf.d[lf.pagemap + 4 * i:lf.pagemap + 4 * i + 4]
        out.append((int.from_bytes(rec[:3], "big"), rec[3]))
    return out


def _page(lf, number, kind, last):
    if kind in (2, 3) or not number:
        return b""
    off = lf.datapages + (number - 1) * lf.pagesize
    return bytes(lf.d[off:off + (lf.last_page_bytes if number == last and lf.last_page_bytes else lf.pagesize)])


def object_data(lf, obj):
    """One object's bytes as loaded: its pages found through the page map (they need not be in
    order or all present in the file), cut or zero-filled to its virtual size."""
    pm = page_map(lf)
    last = max([n for n, _k in pm] or [0])
    out = bytearray()
    for number, kind in pm[obj["pmidx"] - 1:obj["pmidx"] - 1 + obj["pcnt"]]:
        out += _page(lf, number, kind, last).ljust(lf.pagesize, b"\0")
    return bytes(out[:obj["vsize"]].ljust(obj["vsize"], b"\0"))


def file_offset(lf, obj_index, off):
    """Where byte `off` of object `obj_index` (1-based) is in the file, or None."""
    obj = lf.objects[obj_index - 1]
    pm = page_map(lf)
    i = obj["pmidx"] - 1 + off // lf.pagesize
    if off >= obj["vsize"] or i >= len(pm) or pm[i][1] != 0 or not pm[i][0]:
        return None
    return lf.datapages + (pm[i][0] - 1) * lf.pagesize + off % lf.pagesize


def _names(d, p, end=None):
    out = []
    while 0 < p < len(d) and d[p] and (end is None or p < end):
        n = d[p]
        if p + n + 3 > len(d):
            break
        out.append((bytes(d[p + 1:p + 1 + n]).decode("latin1"), struct.unpack_from("<H", d, p + 1 + n)[0]))
        p += n + 3
    return out


def names(lf):
    """([(name, ordinal)] resident, [(name, ordinal)] non-resident). The first resident name
    (ordinal 0) is the module's own, the first non-resident one its description; the others
    name its exports. A container member's non-resident table is not looked for (its stored
    offset no longer means anything)."""
    h = header_fields(lf)
    non = []
    if h["nonresident_names"] and h["nonresident_size"] and not is_member(lf):
        p = h["nonresident_names"]
        non = _names(lf.d, p, p + h["nonresident_size"])
    return _names(lf.d, lf.resnames), non


def entries(lf):
    """[(ordinal, object, offset, flags)] from the entry table: the places the image exports.
    A driver has one, its device descriptor block."""
    d, p, out, ordinal = lf.d, lf.entrytab, [], 1
    while p + 2 <= len(d) and d[p]:
        count, kind = d[p], d[p + 1] & 0x7F
        p += 2
        if kind == 0:                                        # a run of unused ordinals
            ordinal += count
            continue
        size = {1: 3, 2: 5, 3: 5, 4: 7}.get(kind)
        if size is None or p + 2 + count * size > len(d):
            break
        obj = struct.unpack_from("<H", d, p)[0]
        p += 2
        for _ in range(count):
            off = struct.unpack_from("<I" if kind in (3, 4) else "<H", d, p + (3 if kind == 4 else 1))[0]
            out.append((ordinal, obj if kind != 4 else 0, off, d[p]))
            ordinal += 1
            p += size
    return out


def _entry_table_size(lf):
    d, p = lf.d, lf.entrytab
    while p + 2 <= len(d) and d[p]:
        count, kind = d[p], d[p + 1] & 0x7F
        size = {0: 0, 1: 3, 2: 5, 3: 5, 4: 7}.get(kind)
        if size is None:
            break
        p += 2 + (0 if kind == 0 else 2 + count * size)
    return p + 1 - lf.entrytab


def exports(lf):
    """[(name, ordinal, object, offset)] -- the entry table joined with the name tables."""
    res, non = names(lf)
    by = {o: n for n, o in res[1:] + non[1:]}
    return [(by.get(o, ""), o, obj, off) for o, obj, off, _f in entries(lf)]


def fixup_records(lf):
    """Every fix-up record of the image, page by page: [(page, record)] with page 1-based and
    record a dict -- raw (its bytes), source (kind of place patched: 7 = 32-bit offset, 8 =
    32-bit self-relative, 2 = selector...), sources (offsets within the page; one may be
    negative when the place starts on the previous page), kind ('internal', 'import',
    'import-name', 'entry'), and for that kind: obj + target, module + ordinal, module + name,
    or entry; plus addend when the record carries one. Unlike LEFile.fixups this stops at
    nothing: all four kinds are read."""
    d, out = lf.d, []
    offs = [struct.unpack_from("<I", d, lf.fixup_page + 4 * i)[0] for i in range(lf.mpages + 1)]
    for pg in range(lf.mpages):
        p, end = lf.fixup_rec + offs[pg], lf.fixup_rec + offs[pg + 1]
        while p + 4 <= end:
            p0, src, fl = p, d[p], d[p + 1]
            p += 2
            count = None
            if src & 0x20:
                count = d[p]
                p += 1
                sources = []
            else:
                sources = [struct.unpack_from("<h", d, p)[0]]
                p += 2
            wide = 2 if fl & 0x40 else 1
            first = struct.unpack_from("<H", d, p)[0] if wide == 2 else d[p]
            p += wide
            rec = {"source": src & 0x0F, "source_flags": src & 0xF0, "flags": fl}
            what = fl & 3
            if what == 0:
                rec.update(kind="internal", obj=first, target=None)
                if src & 0x0F != 2:
                    n = 4 if fl & 0x10 else 2
                    rec["target"] = int.from_bytes(d[p:p + n], "little")
                    p += n
            elif what == 1:
                n = 1 if fl & 0x80 else (4 if fl & 0x10 else 2)
                rec.update(kind="import", module=first, ordinal=int.from_bytes(d[p:p + n], "little"))
                p += n
            elif what == 2:
                n = 4 if fl & 0x10 else 2
                rec.update(kind="import-name", module=first, name=int.from_bytes(d[p:p + n], "little"))
                p += n
            else:
                rec.update(kind="entry", entry=first)
            if fl & 0x04:
                n = 4 if fl & 0x20 else 2
                rec["addend"] = int.from_bytes(d[p:p + n], "little")
                p += n
            if count is not None:
                sources = [struct.unpack_from("<h", d, p + 2 * j)[0] for j in range(count)]
                p += 2 * count
            rec["sources"] = sources
            rec["raw"] = bytes(d[p0:p])
            out.append((pg + 1, rec))
    return out


def encode_fixup(rec):
    """The bytes of a fix-up record as fixup_records gives it (or one built the same way)."""
    fl, src = rec["flags"], rec["source"] | rec.get("source_flags", 0)
    many = bool(src & 0x20)
    out = bytearray([src, fl])
    out += bytes([len(rec["sources"])]) if many else struct.pack("<h", rec["sources"][0])
    wide = 2 if fl & 0x40 else 1
    first = {"internal": "obj", "import": "module", "import-name": "module", "entry": "entry"}[rec["kind"]]
    out += rec[first].to_bytes(wide, "little")
    if rec["kind"] == "internal" and rec["source"] != 2:
        out += rec["target"].to_bytes(4 if fl & 0x10 else 2, "little")
    elif rec["kind"] == "import":
        out += rec["ordinal"].to_bytes(1 if fl & 0x80 else (4 if fl & 0x10 else 2), "little")
    elif rec["kind"] == "import-name":
        out += rec["name"].to_bytes(4 if fl & 0x10 else 2, "little")
    if fl & 0x04:
        out += rec["addend"].to_bytes(4 if fl & 0x20 else 2, "little")
    if many:
        out += b"".join(struct.pack("<h", s) for s in rec["sources"])
    return bytes(out)


def references(lf):
    """[(object, offset, source kind, target object, target offset)] -- every place in the
    image the loader patches with the address of another place in it (source kind 7: the
    32-bit address itself; 8: its distance from the end of the patched field, as in a call)."""
    out = []
    for pg, rec in fixup_records(lf):
        if rec["kind"] != "internal" or rec["target"] is None:
            continue
        obj, base = lf.page_to_obj(pg)
        if obj is not None:
            for s in rec["sources"]:
                out.append((obj["idx"], base + s, rec["source"], rec["obj"], rec["target"] + rec.get("addend", 0)))
    return out


def pointers(lf):
    """{(object, offset): (target object, target offset)} for every place that holds the
    32-bit address of another place in the image."""
    return {(o, a): (to, ta) for o, a, kind, to, ta in references(lf) if kind == 7}


def _plausible(b, at):
    """Does a device descriptor block start at `at`?"""
    if at < 0 or at + 0x38 > len(b):
        return False
    name = b[at + 0x0C:at + 0x14]
    version = b[at + 4] | (b[at + 5] << 8)
    return 0x0300 <= version <= 0x04FF and name[:1].isalnum() and all(32 <= c < 127 for c in name)


def descriptor(lf):
    """The device descriptor block, every field, or None: where it is (obj, offset -- from the
    entry table, which exports exactly it; else found by its shape), the numbers it holds
    (kit_version, device_id, major, minor, flags, name, init_order, service_count), and each
    address it holds as (object, offset) or None: control, v86_api, pm_api, service_table,
    reference_data (the stored number is kept as <field>_raw). `size` is 0x50 for the block
    of this system, 0x34 for the older one; `bytes` is the block itself."""
    where = None
    for _ordinal, obj, off, _flags in entries(lf):
        if 0 < obj <= len(lf.objects) and _plausible(object_data(lf, lf.objects[obj - 1]), off):
            where = (obj, off)
            break
    if where is None:
        old = lf.ddb()
        if not old:
            return None
        where = (old["obj"], old["rva"])
    obj, at = where
    b = object_data(lf, lf.objects[obj - 1])
    out = {"obj": obj, "offset": at, "name": b[at + 0x0C:at + 0x14].rstrip(b" \0").decode("latin1")}
    for field, off, fmt in DESCRIPTOR:
        out[field] = struct.unpack_from("<" + fmt, b, at + off)[0]
    ptr = pointers(lf)
    for field in ("control", "v86_api", "pm_api", "service_table", "reference_data"):
        out[field + "_raw"] = out[field]
        out[field] = ptr.get((obj, at + _DESCRIPTOR_AT[field]))
    out["size"] = 0x50 if b[at + 0x3C:at + 0x40] == b"verP" and at + 0x50 <= len(b) else 0x34
    out["bytes"] = bytes(b[at:at + out["size"]])
    return out


def service_table(lf, d=None):
    """[(object, offset) or None] -- where each service the driver offers is, by service number
    (None: a number kept free). Empty for a driver that offers none."""
    d = d or descriptor(lf)
    if not d or not d["service_table"] or not 0 < d["service_count"] < 0x1000:
        return []
    ptr = pointers(lf)
    obj, off = d["service_table"]
    return [ptr.get((obj, off + 4 * i)) for i in range(d["service_count"])]


def entry_points(lf):
    """{label: (object, offset)} -- everywhere the system or another driver can enter this one:
    'control' (the control procedure), 'service_N', 'v86_api', 'pm_api'."""
    d = descriptor(lf)
    out = {}
    if not d:
        return out
    for field in ("control", "v86_api", "pm_api"):
        if d[field]:
            out[field] = d[field]
    for i, where in enumerate(service_table(lf, d)):
        if where:
            out["service_%d" % i] = where
    return out


def service_calls(lf):
    """[(object, offset, device, service, is_jump)] -- every `int 20h` service call in the
    executable objects (CD 20, service number, device number; bit 15 of the service number
    says jump instead of call and is returned apart)."""
    out = []
    for o in lf.objects:
        if not o["exec"]:
            continue
        img = object_data(lf, o)
        i = img.find(b"\xCD\x20")
        while i != -1 and i + 6 <= len(img):
            svc, dev = struct.unpack_from("<HH", img, i + 2)
            if 0 < dev < 0x600 and (svc & 0x7FFF) < 0x400:
                out.append((o["idx"], i, dev, svc & 0x7FFF, bool(svc & 0x8000)))
                i = img.find(b"\xCD\x20", i + 6)
            else:
                i = img.find(b"\xCD\x20", i + 2)
    return out


# Writing an image back ----------------------------------------------------------------------------
def read_image(lf):
    """An LE image taken apart into what write_image puts together again: {"stub" (what is in
    front of the header), "header" (the header's own bytes), "objects" [{vsize, base, flags,
    reserved, first_page, page_count}], "pages" [(kind, bytes)] (all of them, in page-map
    order; an object owns a run of them), "resident" / "nonresident" [(name, ordinal)],
    "entry_table" (bytes), "fixups" {page: [record bytes]}, "import_modules" [names],
    "import_names" (bytes), "align" (what the data pages are aligned to in the file), "gaps"
    (the filler in front of the data pages and in front of the non-resident names), "after"
    (everything behind the data pages and the non-resident names: debug information,
    resources)}. Change a part, write it back."""
    d, h = lf.d, header_fields(lf)
    member = is_member(lf)
    pm = page_map(lf)
    last = max([n for n, _k in pm] or [0])
    objects = [{"vsize": o["vsize"], "base": o["base"], "flags": o["flags"], "first_page": o["pmidx"],
                "page_count": o["pcnt"], "reserved": bytes(d[lf.objtab + 24 * i + 20:lf.objtab + 24 * i + 24])}
               for i, o in enumerate(lf.objects)]
    # the file keeps pages in the order of their numbers; that is the page-map order in every
    # image seen, and write_image numbers them that way again
    pages = [(kind, _page(lf, number, kind, last)) for number, kind in pm]
    fix = {}
    for pg, rec in fixup_records(lf):
        fix.setdefault(pg, []).append(rec["raw"])
    res, non = names(lf)
    names_end = (h["nonresident_names"] + h["nonresident_size"]) if non else 0
    pages_end = lf.datapages + ((last - 1) * lf.pagesize + (lf.last_page_bytes or lf.pagesize) if last else 0)
    align = 1
    while align < 0x1000 and lf.datapages % (align * 2) == 0:
        align *= 2
    ip, ie = lf.lfa + h["import_names"], lf.lfa + h["fixup_size"] + h["fixup_pages"]
    stub = bytes(d[:lf.lfa]) if not member else (b"MZ" + bytes(0x3A) + struct.pack("<I", 0x40))
    return {"stub": stub, "header": bytes(d[lf.lfa:lf.lfa + HEADER_SIZE]), "objects": objects, "pages": pages,
            "resident": res, "nonresident": non, "entry_table": bytes(d[lf.entrytab:lf.entrytab + _entry_table_size(lf)]),
            "fixups": fix, "import_modules": lf.import_modules(),
            "import_names": bytes(d[ip:ie]) if h["import_names"] and lf.lfa < ip < ie <= len(d) else b"",
            "align": align, "last_page_bytes": lf.last_page_bytes,
            "gaps": (bytes(d[ie:lf.datapages]) if lf.lfa < ie <= lf.datapages and not member else b"",
                     bytes(d[pages_end:h["nonresident_names"]]) if non and pages_end <= h["nonresident_names"] else b""),
            "after": b"" if member else bytes(d[max(pages_end, names_end):]),
            "moved": {"debug_info": h["debug_info"], "resource_data": h["resource_data"],
                      "from": max(pages_end, names_end)}}


def _name_table(pairs):
    return b"".join(bytes([len(n)]) + n.encode("latin1") + struct.pack("<H", o) for n, o in pairs) + b"\0"


def write_image(image):
    """The file for an image as read_image describes it: the stub, the header with every
    offset and size worked out again, the object table, the page map, the name and entry
    tables, the fix-up tables, the import names, the data pages, the non-resident names and
    whatever came after. Pages are stored and numbered in page-map order."""
    hdr = bytearray(image["header"].ljust(HEADER_SIZE, b"\0"))
    page_size = struct.unpack_from("<I", hdr, 0x28)[0]
    table, pmap, data, number, last_len = bytearray(), bytearray(), bytearray(), 0, 0
    kinds = image["pages"]
    for o in image["objects"]:
        table += struct.pack("<IIIII", o["vsize"], o["base"], o["flags"], o["first_page"], o["page_count"])
        table += o.get("reserved", b"\0\0\0\0")
    stored = [i for i, (kind, body) in enumerate(kinds) if kind not in (2, 3) and body]
    for i, (kind, body) in enumerate(kinds):
        if i in stored:
            number += 1
            pmap += number.to_bytes(3, "big") + bytes([kind])
            last_len = len(body) if i == stored[-1] else page_size
            data += body if i == stored[-1] else body.ljust(page_size, b"\0")
        else:
            pmap += bytes(3) + bytes([kind])
    resident = _name_table(image["resident"])
    fix_pages, fix_records = bytearray(), bytearray()
    for pg in range(1, len(kinds) + 1):
        fix_pages += struct.pack("<I", len(fix_records))
        fix_records += b"".join(image["fixups"].get(pg, ()))
    fix_pages += struct.pack("<I", len(fix_records))
    modules = b"".join(bytes([len(n)]) + n.encode("latin1") for n in image["import_modules"])
    at = HEADER_SIZE
    where = {}
    for key, part in (("object_table", table), ("page_map", pmap), ("resident_names", resident),
                      ("entry_table", image["entry_table"]), ("fixup_pages", fix_pages),
                      ("fixup_records", fix_records), ("import_modules", modules),
                      ("import_names", image["import_names"])):
        where[key] = at
        at += len(part)
    base = len(image["stub"])
    pages_at = -(-(base + at) // image["align"]) * image["align"]
    put = lambda off, v: struct.pack_into("<I", hdr, off, v)        # noqa: E731
    put(0x14, len(kinds)), put(0x2C, last_len if image.get("last_page_bytes") or last_len != page_size else 0)
    put(0x30, len(fix_pages) + len(fix_records) + len(modules) + len(image["import_names"]))
    put(0x38, where["fixup_pages"] - HEADER_SIZE)
    put(0x40, where["object_table"]), put(0x44, len(image["objects"])), put(0x48, where["page_map"])
    put(0x58, where["resident_names"]), put(0x5C, where["entry_table"])
    put(0x68, where["fixup_pages"]), put(0x6C, where["fixup_records"])
    put(0x70, where["import_modules"]), put(0x74, len(image["import_modules"]))
    put(0x78, where["import_names"]), put(0x80, pages_at)
    out = bytearray(image["stub"]) + hdr + table + pmap + resident + image["entry_table"] + fix_pages + \
        fix_records + modules + image["import_names"]
    gap, names_gap = image.get("gaps") or (b"", b"")
    out += (gap if len(gap) == pages_at - len(out) else bytes(pages_at - len(out))) + data
    non = _name_table(image["nonresident"]) if image["nonresident"] else b""
    out += names_gap if non and len(names_gap) < 16 else b""
    put(0x88, len(out) if non else 0), put(0x8C, len(non))
    out += non
    shift = len(out) - image["moved"]["from"]
    for off, key in ((0x98, "debug_info"), (0xB8, "resource_data")):
        put(off, image["moved"][key] + shift if image["moved"][key] and image["after"] else image["moved"][key])
    out += image["after"]
    out[base:base + HEADER_SIZE] = hdr
    return bytes(out)


# Containers -------------------------------------------------------------------------------------
def container(data):
    """The directory of a container of drivers (VMM32.VXD, WIN386.EXE), or None when `data` is
    not one: {"kind": 'W3' or 'W4', "at": where its header is, "version", "members":
    [(name, offset of the member's LE header, size of its header and tables)]}. A W4 holds the
    same thing compressed: expand(data) gives the W3."""
    if data[:2] != b"MZ" or len(data) < 0x40:
        return None
    at = struct.unpack_from("<I", data, 0x3C)[0]
    kind = bytes(data[at:at + 2])
    if kind == b"W4":
        return {"kind": "W4", "at": at, "version": struct.unpack_from("<H", data, at + 2)[0], "members": []}
    if kind != b"W3":
        return None
    count = struct.unpack_from("<H", data, at + 4)[0]
    found = []
    for i in range(count):
        p = at + 0x10 + 16 * i
        found.append((bytes(data[p:p + 8]).rstrip(b" \0").decode("latin1"),) + struct.unpack_from("<II", data, p + 8))
    return {"kind": "W3", "at": at, "version": struct.unpack_from("<H", data, at + 2)[0],
            "reserved": bytes(data[at + 6:at + 0x10]), "members": found}


def pack_directory(c):
    """The header and directory bytes of a W3 container, from container()'s dict."""
    out = bytearray(b"W3" + struct.pack("<HH", c["version"], len(c["members"])) + c["reserved"])
    for name, off, size in c["members"]:
        out += name.encode("latin1").ljust(8, b" ") + struct.pack("<II", off, size)
    return bytes(out)


class _Bits(object):
    def __init__(self, buf):
        self.buf, self.pos = buf, 0

    def take(self, n=1):
        v = 0
        for i in range(n):
            v |= ((self.buf[self.pos >> 3] >> (self.pos & 7)) & 1) << i
            self.pos += 1
        return v


def expand_page(buf, size=0x2000):
    """One compressed page of a W4 container -> its bytes. Bits are read low first. Two equal
    bits start a copy from earlier in the page (00: distance in 6 bits; 110: 8 bits + 64;
    111: 12 bits + 320, all ones marking the end of a 512-byte stretch), two unequal bits a
    literal byte; a copy's length is a run of zero bits, a one, and that many more bits."""
    br, out = _Bits(buf), bytearray()
    while len(out) < size:
        b0, b1 = br.take(), br.take()
        if b0 != b1:
            out.append((b0 << 7) | br.take(7))
            continue
        if b0 == 0:
            dist = br.take(6)
        elif br.take() == 0:
            dist = br.take(8) + 64
        else:
            dist = br.take(12)
            if dist == 0xFFF:
                continue
            dist += 320
        k = 0
        while br.take() == 0:
            k += 1
        for _ in range((1 << k) + br.take(k) + 1):
            out.append(out[-dist])
    return bytes(out)


def compress_page(page):
    """One page -> compressed bytes expand_page turns back into it (longest earlier match,
    never across a 512-byte boundary, each stretch closed with the end marker)."""
    bits = []

    def put(v, n):
        bits.extend((v >> i) & 1 for i in range(n))

    n, heads, prev, i = len(page), {}, [-1] * len(page), 0
    while i < n:
        room = (i // 512 + 1) * 512 - i
        best, dist = 1, 0
        if room >= 2 and i + 1 < n:
            p = heads.get(page[i:i + 2], -1)
            while p >= max(0, i - 4414):
                length = 0
                while length < room and i + length < n and page[p + length] == page[i + length]:
                    length += 1
                if length > best:
                    best, dist = length, i - p
                    if length == room:
                        break
                p = prev[p]
        if best >= 2:
            if dist <= 63:
                put(0, 2), put(dist, 6)
            elif dist <= 319:
                put(3, 3), put(dist - 64, 8)
            else:
                put(7, 3), put(dist - 320, 12)
            k = (best - 1).bit_length() - 1
            put(0, k), put(1, 1), put(best - 1 - (1 << k), k)
        else:
            top = page[i] >> 7
            put(top | ((1 - top) << 1), 2), put(page[i] & 0x7F, 7)
        for j in range(i, i + best):
            if j + 1 < n:
                prev[j] = heads.get(page[j:j + 2], -1)
                heads[page[j:j + 2]] = j
        i += best
        if i % 512 == 0:
            put(7, 3), put(0xFFF, 12)
    if n % 512:
        put(7, 3), put(0xFFF, 12)
    bits += [0] * (-len(bits) % 8)
    return bytes(sum(bits[k + j] << j for j in range(8)) for k in range(0, len(bits), 8))


def expand(data):
    """A compressed (W4) container as the plain (W3) one it holds; anything else unchanged.
    The result keeps the program in front of the header, so member offsets are as stored."""
    c = container(data)
    if not c or c["kind"] != "W4":
        return bytes(data)
    at = c["at"]
    size, count = struct.unpack_from("<HH", data, at + 4)
    offs = [struct.unpack_from("<I", data, at + 0x10 + 4 * i)[0] for i in range(count)] + [len(data)]
    body = b"".join(bytes(data[offs[i]:offs[i + 1]]) if offs[i + 1] - offs[i] == size
                    else expand_page(data[offs[i]:offs[i + 1]], size) for i in range(count))
    return bytes(data[:at]) + body


def compress(data, like):
    """A plain (W3) container `data` as a compressed one, reusing the program and header in
    front of `like` (a W4 container: its loader is what expands the pages at start-up)."""
    at = container(like)["at"]
    size = struct.unpack_from("<H", like, at + 4)[0]
    body = bytes(data[at:])
    pages = [compress_page(body[i:i + size].ljust(size, b"\0")) for i in range(0, len(body), size)]
    head = bytearray(like[:at + 0x10])
    struct.pack_into("<H", head, at + 6, len(pages))
    pos, table = at + 0x10 + 4 * len(pages), bytearray()
    for pg in pages:
        table += struct.pack("<I", pos)
        pos += len(pg)
    return bytes(head) + bytes(table) + b"".join(pages)


def members(path):
    """[(name, LEFile)] for any file of drivers: one for a plain LE image (also one behind a
    DOS program, like DRVSPACE.BIN), every member of a W3 container, and the same for a
    compressed W4 container after expanding it."""
    with open(path, "rb") as f:
        data = f.read()
    c = container(data)
    if c is None:
        return open_vxds(path)
    data = expand(data)
    return [(name, LEFile(data, off)) for name, off, _size in container(data)["members"]
            if data[off:off + 2] in (b"LE", b"LX")]


def corpus_files():
    """Every file in the corpus that holds drivers (LE images and containers), one per file
    name, smallest first."""
    import os
    from codebreaker.base import corpus
    out = []
    for _name, cands in corpus.index().items():
        try:
            with open(cands[0], "rb") as f:
                head = f.read(0x40)
                if head[:2] != b"MZ" or len(head) < 0x40:
                    continue
                f.seek(struct.unpack_from("<I", head, 0x3C)[0])
                if f.read(2) in (b"LE", b"LX", b"W3", b"W4"):
                    out.append(cands[0])
        except OSError:
            continue
    return sorted(out, key=lambda p: (os.path.getsize(p), p.lower()))
