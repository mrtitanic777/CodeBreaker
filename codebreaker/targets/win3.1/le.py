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


def open_le(path):
    return LEFile(path)


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
    import target
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
