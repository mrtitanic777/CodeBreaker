#!/usr/bin/env python3
"""
le.py — an LE / LX (Linear Executable) reader for the Windows 95 VxD stack.

Win95's 16-bit world is NE (ne.py), its 32-bit world is PE (pe.py) — and its
ring-0 virtual-machine world is the third format: LE. Every `.VXD` / `.386`
virtual device driver is an LE module, and the kernel core `VMM32.VXD` is a
**W3 container** bundling ~100 LE modules (VMM + the boot VxDs), the direct
descendant of Windows 3.1's WIN386.EXE `W3` archive.

This is the parser that opens that world, deliberately minimal and dependency-
free like its NE/PE siblings: enough to tier and measure the corpus (object
table, code size) and to enumerate what a W3 container holds. Reconstruction of
LE code is a later concern; this reads structure.

    python le.py adaptec.vxd            # one LE module
    python le.py VMM32.VXD --w3         # list the modules a W3 container bundles
"""
import struct
import sys

# object flags
OBJ_READABLE, OBJ_WRITABLE, OBJ_EXECUTABLE = 0x1, 0x2, 0x4
_CPU = {0x01: "286", 0x02: "386", 0x03: "486", 0x04: "586"}
_OS = {0x01: "OS/2", 0x02: "Windows", 0x03: "DOS 4.x", 0x04: "Windows 386"}


class LEFile:
    def __init__(self, path):
        self.path = path
        with open(path, "rb") as f:
            self.data = f.read()
        d = self.data
        if d[:2] != b"MZ":
            raise ValueError(f"{path}: no MZ header")
        self.e_lfanew = struct.unpack("<I", d[0x3C:0x40])[0]
        self.sig = d[self.e_lfanew:self.e_lfanew + 2]
        if self.sig not in (b"LE", b"LX"):
            raise ValueError(f"{path}: not an LE/LX image (sig {self.sig!r})")
        self._parse()

    @classmethod
    def from_buffer(cls, data, e_lfanew=0, path="<buffer>"):
        """Parse a raw LE/LX image from bytes -- e.g. an LE module embedded in a W3
        container (VMM32.VXD) that has no MZ header of its own. `e_lfanew` is where
        the LE/LX signature sits within `data` (0 for a bare LE)."""
        self = cls.__new__(cls)
        self.path = path
        self.data = data
        self.e_lfanew = e_lfanew
        self.sig = data[e_lfanew:e_lfanew + 2]
        if self.sig not in (b"LE", b"LX"):
            raise ValueError(f"{path}: not an LE/LX image (sig {self.sig!r})")
        self._parse()
        return self

    def _parse(self):
        d = self.data
        o = self.e_lfanew                        # LE header is relative to here
        self.byte_order = d[o + 2]
        self.word_order = d[o + 3]
        (self.cpu, self.os) = struct.unpack("<HH", d[o + 8:o + 12])
        self.module_flags = struct.unpack("<I", d[o + 0x10:o + 0x14])[0]
        self.n_pages = struct.unpack("<I", d[o + 0x14:o + 0x18])[0]
        self.eip_object = struct.unpack("<I", d[o + 0x18:o + 0x1C])[0]
        self.eip = struct.unpack("<I", d[o + 0x1C:o + 0x20])[0]
        self.page_size = struct.unpack("<I", d[o + 0x28:o + 0x2C])[0]
        (self.obj_tab, self.n_objects, self.obj_page_map) = struct.unpack(
            "<III", d[o + 0x40:o + 0x4C])
        (self.res_names, self.entry_tab) = struct.unpack(
            "<II", d[o + 0x58:o + 0x60])
        self.data_pages = struct.unpack("<I", d[o + 0x80:o + 0x84])[0]
        (self.nonres_names, self.nonres_len) = struct.unpack(
            "<II", d[o + 0x88:o + 0x90])

    # -- objects ----------------------------------------------------------
    def objects(self):
        """[{vsize, base, flags, page_index, page_count, exec}] — the LE analogue
        of NE segments / PE sections. Each is 24 bytes in the object table."""
        out = []
        base = self.e_lfanew + self.obj_tab
        for i in range(self.n_objects):
            rec = self.data[base + i * 24: base + i * 24 + 24]
            if len(rec) < 24:
                break
            vsize, reloc_base, flags, page_idx, page_cnt, _res = struct.unpack(
                "<IIIIII", rec)
            out.append(dict(vsize=vsize, base=reloc_base, flags=flags,
                            page_index=page_idx, page_count=page_cnt,
                            exec=bool(flags & OBJ_EXECUTABLE)))
        return out

    def code_bytes(self):
        """Sum of virtual sizes of EXECUTABLE objects — the coverage denominator,
        mirroring pe.code_bytes / NE code segment sizing."""
        return sum(ob["vsize"] for ob in self.objects() if ob["exec"])

    def _page_file_off(self, entry_index):
        """(file_offset, flag) of the page-map entry `entry_index` (1-based). LE
        page-map entry = 3-byte BIG-endian physical page number + 1 flag byte
        (0=plain, 1=iterated, 2=invalid, 3=zero-fill)."""
        o = self.e_lfanew + self.obj_page_map + (entry_index - 1) * 4
        rec = self.data[o:o + 4]
        phys = int.from_bytes(rec[:3], "big")
        flag = rec[3] if len(rec) == 4 else 0
        return self.data_pages + (phys - 1) * self.page_size, flag

    def object_bytes(self, obj):
        """The linear bytes of one object (its pages concatenated, clamped to
        vsize). Handles plain (flag 0) pages — the case for VMM32 and the VxD
        corpus; iterated/zero pages fall back to what is on disk / zeros."""
        out = bytearray()
        for k in range(obj["page_count"]):
            off, flag = self._page_file_off(obj["page_index"] + k)
            if flag in (0,):                       # plain page
                out += self.data[off:off + self.page_size]
            elif flag == 3:                        # zero-filled
                out += b"\0" * self.page_size
            else:                                  # iterated/other: best-effort
                out += self.data[off:off + self.page_size]
        return bytes(out[:obj["vsize"]])

    def module_name(self):
        """First resident-name-table entry is the module's own name."""
        off = self.e_lfanew + self.res_names
        try:
            n = self.data[off]
            return self.data[off + 1:off + 1 + n].decode("latin1", "replace")
        except Exception:
            return ""

    # -- fixups + DDB service table --------------------------------------
    def _fixup_tables(self):
        o, d = self.e_lfanew, self.data
        self.fixup_page_tab = o + struct.unpack_from("<I", d, o + 0x68)[0]
        self.fixup_record_tab = o + struct.unpack_from("<I", d, o + 0x6C)[0]

    def resolve_fixup(self, obj_idx, off):
        """Resolve the INTERNAL fixup at (object[1-based], byte-offset) -> (target_obj,
        target_off) or None. LE stores intra-module pointers as fixups the loader relocates;
        this reads the per-page fixup records to recover the target address that would be
        written -- the mechanism a DDB service table (an array of such fixups) needs."""
        if not hasattr(self, "fixup_page_tab"):
            self._fixup_tables()
        d, ps, objs = self.data, self.page_size, self.objects()
        ob = objs[obj_idx - 1]
        gp = ob['page_index'] + (off // ps)                # 1-based global page
        soff = off % ps
        p = self.fixup_record_tab + struct.unpack_from("<I", d, self.fixup_page_tab + (gp - 1) * 4)[0]
        end = self.fixup_record_tab + struct.unpack_from("<I", d, self.fixup_page_tab + gp * 4)[0]
        while p < end:
            src, fl = d[p], d[p + 1]
            p += 2
            if src & 0x20:                                 # SRCLIST: count byte + word offsets
                c = d[p]; p += 1
                so = [struct.unpack_from("<h", d, p + 2 * i)[0] for i in range(c)]; p += 2 * c
            else:
                so = [struct.unpack_from("<h", d, p)[0]]; p += 2
            tt = fl & 3
            if tt == 0:                                     # internal ref
                objn = struct.unpack_from("<H", d, p)[0] if fl & 0x40 else d[p]
                p += 2 if fl & 0x40 else 1
                toff = struct.unpack_from("<I", d, p)[0] if fl & 0x10 else struct.unpack_from("<H", d, p)[0]
                p += 4 if fl & 0x10 else 2
                if soff in so:
                    return (objn, toff)
            elif tt == 1:                                   # import by ordinal
                p += (2 if fl & 0x40 else 1) + (2 if fl & 0x80 else 1)
            elif tt == 2:                                   # import by name
                p += (2 if fl & 0x40 else 1) + (4 if fl & 0x80 else 2)
            else:
                p += 4
        return None

    def find_ddb(self, name=None):
        """Locate a VxD DDB by its 8-byte DDB_Name (+0x0C), validated by a resolvable
        DDB_Service_Table_Ptr (+0x30). -> (obj_idx, ddb_off, service_count) | None.
        `name` defaults to this module's own name; pass a device name to find a DDB whose
        name differs from the module (SCSIPORT/PCI, or a static VxD bundled in a W3
        container's LE). NB: the base VMM's service table is NOT a standard fixup array
        (flat linear addresses in the LE preamble), so it is not discoverable this way."""
        name = (name or self.module_name()).strip().upper().encode('latin1')
        if not name:
            return None
        for i, ob in enumerate(self.objects()):
            b = self.object_bytes(ob)
            pos = 0
            while True:
                pp = b.find(name, pos)
                if pp < 0:
                    break
                pos = pp + 1
                ddb = pp - 0x0C
                if ddb < 0 or ddb + 0x38 > len(b):
                    continue
                if b[ddb + 0x0C:ddb + 0x14].rstrip(b' \x00').upper() != name:
                    continue
                cnt = struct.unpack_from("<I", b, ddb + 0x34)[0]
                if 0 < cnt < 4000 and self.resolve_fixup(i + 1, ddb + 0x30):
                    return (i + 1, ddb, cnt)
        return None

    def ddb_services(self, name=None):
        """[(index, obj, offset)] -- each VxD service's handler location, resolved through the
        DDB service table (a fixup array). These are the VxD's external entry points (the
        targets other modules reach via VxDCall (Device_ID<<16)|index). `name` selects which
        DDB (default: this module's own); pass a device name for a bundled/foreign DDB."""
        dd = self.find_ddb(name)
        if not dd:
            return []
        oi, ddb, cnt = dd
        st = self.resolve_fixup(oi, ddb + 0x30)
        if not st:
            return []
        sto, stoff = st
        out = []
        for k in range(cnt):
            h = self.resolve_fixup(sto, stoff + k * 4)
            if h:
                out.append((k, h[0], h[1]))
        return out

    def find_all_ddbs(self, names=None):
        """{DEVICE_NAME: (obj_idx, ddb_off, service_count)} for EVERY standard DDB present --
        not just the module's own. Tries the full DDK device-name set (vmmsvc.DEVICE_IDS) plus
        the module name, so it finds DDBs whose name != module_name: SCSIPORT/PCI (standalone
        but self-named), and the static VxDs a W3 container bundles into one LE. `names` adds
        extra candidates. Only standard fixup-table DDBs appear (see find_ddb's VMM caveat)."""
        cands = set(names or ())
        try:
            from codebreaker.symbols import vmm_services as vmmsvc
            cands |= set(vmmsvc.DEVICE_IDS)
        except Exception:
            pass
        mn = self.module_name().strip().upper()
        if mn:
            cands.add(mn)
        out = {}
        for nm in sorted(cands):
            r = self.find_ddb(nm)
            if r:
                out[nm] = r
        return out

    def all_ddb_services(self, names=None):
        """{DEVICE_NAME: [(index, obj, offset)]} -- ddb_services() for every DDB find_all_ddbs
        locates. For a plain VxD this is just its own device; for a W3-bundled LE it is every
        static VxD whose service table is a standard fixup array."""
        return {nm: self.ddb_services(nm) for nm in self.find_all_ddbs(names)}

    def _linear_object(self, lin):
        """Map a flat LINEAR offset to (obj_idx, obj_offset). LE objects have base 0 (auto)
        and load at sequential, page-granular linear addresses -- so linear space is just the
        objects concatenated in order. -> (obj, off) | None."""
        base = 0
        for i, ob in enumerate(self.objects(), 1):
            span = ob["page_count"] * self.page_size
            if base <= lin < base + span:
                return (i, lin - base)
            base += span
        return None

    def vmm_services(self):
        """The base VMM's service table -- VMM is special and needs a bespoke decode. Unlike a
        normal VxD (see ddb_services), VMM's DDB lives in the LE PREAMBLE (before the page data),
        so the object-scanning find_ddb never sees it; and DDB_Service_Table_Ptr (+0x30) is a
        flat LINEAR offset into the code, not a fixup. Each of the ~406 table slots IS an internal
        fixup to its handler; ~38 slots are null (removed/reserved services). Returns
        [(index, obj, offset)] -- same shape/index space as ddb_services (VxDCall service index)
        -- or [] if this LE carries no VMM DDB (i.e. it isn't the VMM core, e.g. vmm32.vxd's
        bundled LE). Names for the indices come from vmmsvc / docs/reference/vmm_services.txt."""
        data, dp = self.data, self.data_pages
        name = b"VMM" + b"\x20" * 5                    # DDB_Name is 8 bytes, space-padded
        ddb = ptr = cnt = None
        pos = 0
        while True:
            pp = data.find(name, pos, dp)             # scan the preamble only
            if pp < 0:
                break
            pos = pp + 1
            cand = pp - 0x0C                           # DDB_Name is at DDB+0x0C
            if cand < 0 or cand + 0x38 > len(data):
                continue
            c = struct.unpack_from("<I", data, cand + 0x34)[0]
            if 0 < c < 4000:                           # DDB_Service_Table_Size sanity
                ddb = cand
                ptr = struct.unpack_from("<I", data, cand + 0x30)[0]
                cnt = c
                break
        if ddb is None:
            return []
        tbl = self._linear_object(ptr)                # service table location
        if tbl is None:
            return []
        out = []
        for k in range(cnt):                           # each slot is an internal fixup -> handler
            r = self.resolve_fixup(tbl[0], tbl[1] + k * 4)
            if r:
                out.append((k, r[0], r[1]))
        return out

    def cpu_name(self):
        return _CPU.get(self.cpu, "%#x" % self.cpu)

    def os_name(self):
        return _OS.get(self.os, "%#x" % self.os)


# --------------------------------------------------------------------------
# W3 container — VMM32.VXD bundles many LE modules (the Win95 kernel/VxD core).
# --------------------------------------------------------------------------
class W3File:
    """A `W3` archive: a header + a table of (name, file-offset) for each bundled
    module. Used to unbundle VMM32.VXD into its constituent LE modules."""

    def __init__(self, path):
        self.path = path
        with open(path, "rb") as f:
            self.data = f.read()
        d = self.data
        if d[:2] != b"MZ":
            raise ValueError(f"{path}: no MZ header")
        self.e_lfanew = struct.unpack("<I", d[0x3C:0x40])[0]
        if d[self.e_lfanew:self.e_lfanew + 2] != b"W3":
            raise ValueError(f"{path}: not a W3 container")
        # W3 header: sig(2), version(1?), ... , count at +0x04, then entries.
        # Entry = 8-byte name (space padded) + u32 file offset + u32 size.
        self.count = struct.unpack("<H", d[self.e_lfanew + 4:self.e_lfanew + 6])[0]

    def modules(self):
        """[(name, offset, size)] for each bundled module."""
        d = self.data
        out = []
        base = self.e_lfanew + 0x10               # entry table follows the header
        for i in range(self.count):
            rec = d[base + i * 16: base + i * 16 + 16]
            if len(rec) < 16:
                break
            name = rec[:8].rstrip(b" \0").decode("latin1", "replace")
            off, size = struct.unpack("<II", rec[8:16])
            out.append((name, off, size))
        return out


def _w3_sig(path):
    with open(path, "rb") as f:
        h = f.read(0x40)
        if h[:2] != b"MZ":
            return None
        e = struct.unpack("<I", h[0x3C:0x40])[0]
        f.seek(e)
        return f.read(2)


def main():
    argv = sys.argv[1:]
    if not argv:
        sys.exit(__doc__)
    from codebreaker.base import registry as _registry
    target = _registry.get("target")
    path = target.resolve(argv[0]) if "/" not in argv[0] and "\\" not in argv[0] else argv[0]
    if "--w3" in argv or _w3_sig(path) == b"W3":
        w3 = W3File(path)
        mods = w3.modules()
        print("%s: W3 container, %d bundled module(s)"
              % (path, len(mods)))
        for name, off, size in mods[:60]:
            print("  %-10s @ %#010x  %d bytes" % (name, off, size))
        return
    le = LEFile(path)
    objs = le.objects()
    print("%s: %s  cpu=%s os=%s  %d object(s)  %d pages  code=%d bytes  module=%r"
          % (path, le.sig.decode(), le.cpu_name(), le.os_name(),
             len(objs), le.n_pages, le.code_bytes(), le.module_name()))
    for i, ob in enumerate(objs, 1):
        f = "".join(c for c, b in (("R", OBJ_READABLE), ("W", OBJ_WRITABLE),
                                    ("X", OBJ_EXECUTABLE)) if ob["flags"] & b)
        print("  obj%d  base=%#010x  vsize=%-7d  %-3s  pages=%d"
              % (i, ob["base"], ob["vsize"], f, ob["page_count"]))


if __name__ == "__main__":
    main()
