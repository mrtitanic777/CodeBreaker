#!/usr/bin/env python3
r"""
isowrite.py -- write an ISO9660 image (the inverse of iso9660.py's reader).

Builds a valid ISO9660 (8.3 names, no Joliet -- matching the Win95 OSR2 disc) from
a directory tree, with optional El Torito boot so the CD is bootable. Used to
assemble our self-hosted Windows 95 install disc: our reconstructed SETUP + our
byte-exact binaries packed into functional CABs (cabwrite) + the data files. The
ISO container is a delivery wrapper, not a byte-exact reconstruction.

    python isowrite.py OUT.ISO SRCDIR [--boot BOOTIMG] [--volid WIN95]
    python isowrite.py --selftest
"""
import os
import struct
import sys

SECTOR = 2048


def _both16(v):
    return struct.pack("<H", v) + struct.pack(">H", v)


def _both32(v):
    return struct.pack("<I", v) + struct.pack(">I", v)


def _dt7(y=1997, mo=1, d=1, h=0, mi=0, s=0, gmt=0):
    return bytes([y - 1900, mo, d, h, mi, s, gmt])


def _dtstr(y=1997):
    return ("%04d01010000000000" % y).encode() + b"\x00"


def _iso_name(name, is_dir):
    """8.3 upper-case ISO name; files get ';1'. '.' and '..' handled by caller."""
    name = name.upper()
    if is_dir:
        nm = name[:8]
    else:
        base, _, ext = name.partition(".")
        nm = base[:8] + (("." + ext[:3]) if ext else "")
        nm += ";1"
    return nm.encode("ascii", "replace")


class Node:
    def __init__(self, name, is_dir, data=b"", path=None):
        self.name = name
        self.is_dir = is_dir
        self.data = data
        self.path = path
        self.children = []
        self.lba = 0
        self.length = 0
        self.dir_num = 0            # 1-based index in the path table (dirs only)


def _build_tree(srcdir):
    root = Node("", True)

    def walk(node, fsdir):
        for entry in sorted(os.listdir(fsdir)):
            fp = os.path.join(fsdir, entry)
            if os.path.isdir(fp):
                child = Node(entry, True)
                node.children.append(child)
                walk(child, fp)
            elif os.path.isfile(fp):
                node.children.append(Node(entry, False, path=fp,
                                          data=None))
    walk(root, srcdir)
    return root


def _dir_record(name_bytes, lba, length, is_dir, name_self_parent=None):
    """A directory record. name_self_parent: 0 for '.', 1 for '..'."""
    if name_self_parent is not None:
        name_bytes = bytes([name_self_parent])
    ln = 33 + len(name_bytes)
    if ln % 2:
        ln += 1                                    # pad to even
    rec = bytearray(ln)
    rec[0] = ln
    rec[1] = 0                                     # ext attr len
    rec[2:10] = _both32(lba)
    rec[10:18] = _both32(length)
    rec[18:25] = _dt7()
    rec[25] = 0x02 if is_dir else 0x00            # flags
    rec[26] = 0                                    # file unit size
    rec[27] = 0                                    # interleave gap
    rec[28:32] = _both16(1)                        # volume seq
    rec[32] = len(name_bytes)
    rec[33:33 + len(name_bytes)] = name_bytes
    return bytes(rec)


def _dir_size(node):
    """Bytes the directory's records occupy (incl '.' and '..'), sector-aligned."""
    size = len(_dir_record(b"", 0, 0, True, 0)) + len(_dir_record(b"", 0, 0, True, 1))
    for c in node.children:
        rec = _dir_record(_iso_name(c.name, c.is_dir), 0, 0, c.is_dir)
        # records may not span a sector boundary
        if (size % SECTOR) + len(rec) > SECTOR:
            size = (size // SECTOR + 1) * SECTOR
        size += len(rec)
    return (size + SECTOR - 1) // SECTOR * SECTOR


def _all_dirs(root):
    """Directories breadth-first (root first) -> path-table order + dir numbers."""
    out, q = [], [(root, 1)]                       # (node, parent_dir_num)
    root.parent_num = 1
    order = []
    queue = [root]
    while queue:
        nxt = []
        for n in queue:
            order.append(n)
            for c in n.children:
                if c.is_dir:
                    c.parent = n
                    nxt.append(c)
        queue = nxt
    for i, n in enumerate(order, 1):
        n.dir_num = i
    return order


def write_iso(srcdir, out_path, volid="WIN95", boot_img=None):
    root = _build_tree(srcdir)
    dirs = _all_dirs(root)

    # ---- pass 1: assign LBAs ----
    lba = 16                                        # 0-15 system area
    lba += 1                                        # PVD
    boot_vd = None
    if boot_img:
        boot_vd = lba
        lba += 1                                    # El Torito boot record VD
    lba += 1                                        # VDST terminator

    # path tables (L + M): compute size
    pt = bytearray()
    for n in dirs:
        di = bytes([0]) if n is root else _iso_name(n.name, True)
        pt += bytes([len(di), 0]) + b"\x00\x00\x00\x00" + b"\x00\x00" + di
        if len(di) % 2:
            pt += b"\x00"
    pt_size = len(pt)
    pt_sectors = (pt_size + SECTOR - 1) // SECTOR
    lpt_lba = lba; lba += pt_sectors
    mpt_lba = lba; lba += pt_sectors

    # boot catalog + boot image
    boot_cat_lba = boot_img_lba = 0
    boot_img_data = b""
    if boot_img:
        boot_cat_lba = lba; lba += 1
        boot_img_data = open(boot_img, "rb").read()
        boot_img_lba = lba
        lba += (len(boot_img_data) + SECTOR - 1) // SECTOR

    for n in dirs:                                 # directory extents
        n.lba = lba
        n.length = _dir_size(n)
        lba += n.length // SECTOR

    for n in dirs:                                 # file extents
        for c in n.children:
            if not c.is_dir:
                c.data = open(c.path, "rb").read()
                c.lba = lba
                c.length = len(c.data)
                lba += (c.length + SECTOR - 1) // SECTOR
    total_sectors = lba

    # ---- pass 2: emit ----
    out = bytearray(total_sectors * SECTOR)

    def put(lba0, data):
        out[lba0 * SECTOR:lba0 * SECTOR + len(data)] = data

    # PVD
    pvd = bytearray(SECTOR)
    pvd[0] = 1
    pvd[1:6] = b"CD001"
    pvd[6] = 1
    pvd[8:40] = b" " * 32                           # system id
    pvd[40:72] = ("%-32s" % volid).encode()[:32]    # volume id
    pvd[80:88] = _both32(total_sectors)
    pvd[120:124] = _both16(1) + _both16(1)          # set size / seq
    pvd[128:132] = _both16(SECTOR)                  # logical block size
    pvd[132:140] = _both32(pt_size)
    pvd[140:144] = struct.pack("<I", lpt_lba)       # L path table
    pvd[148:152] = struct.pack(">I", mpt_lba)       # M path table
    pvd[156:190] = _dir_record(b"", root.lba, root.length, True, 0)  # root dir record
    for off in (318, 446, 574, 702):
        pvd[off:off + 128] = b" " * 128             # publisher/preparer/etc.
    pvd[813:830] = _dtstr(); pvd[830:847] = _dtstr()
    pvd[847:864] = b"0" * 16 + b"\x00"
    pvd[881] = 1
    put(16, pvd)

    # El Torito boot record VD
    if boot_img:
        bvd = bytearray(SECTOR)
        bvd[0] = 0
        bvd[1:6] = b"CD001"
        bvd[6] = 1
        bvd[7:39] = b"EL TORITO SPECIFICATION" + b"\x00" * 9
        bvd[71:75] = struct.pack("<I", boot_cat_lba)
        put(boot_vd, bvd)
        # boot catalog: validation entry + default entry
        cat = bytearray(SECTOR)
        val = bytearray(32)
        val[0] = 1; val[1] = 0                       # header id / platform x86
        val[28] = 0x55; val[29] = 0xAA
        chk = 0
        for i in range(0, 32, 2):
            if i not in (28,):
                chk = (chk + struct.unpack_from("<H", val, i)[0]) & 0xFFFF
        val[26:28] = struct.pack("<H", (-chk) & 0xFFFF)
        cat[0:32] = val
        ent = bytearray(32)
        ent[0] = 0x88                               # bootable
        ent[1] = 0x02                               # 1.44MB floppy emulation
        ent[2:4] = struct.pack("<H", 0)             # load segment (0=7C0)
        n_sec = (len(boot_img_data) + 511) // 512
        ent[6:8] = struct.pack("<H", 1)             # sector count (emul: 1 vd)
        ent[8:12] = struct.pack("<I", boot_img_lba)
        cat[32:64] = ent
        put(boot_cat_lba, cat)
        put(boot_img_lba, boot_img_data)

    # VDST
    vdst = bytearray(SECTOR)
    vdst[0] = 255; vdst[1:6] = b"CD001"; vdst[6] = 1
    put(16 + 1 + (1 if boot_img else 0), vdst)

    # path tables
    def emit_pt(be):
        buf = bytearray()
        for n in dirs:
            di = bytes([0]) if n is root else _iso_name(n.name, True)
            ext = struct.pack(">I" if be else "<I", n.lba)
            par = struct.pack(">H" if be else "<H", (n.parent.dir_num if n is not root else 1))
            buf += bytes([len(di), 0]) + ext + par + di
            if len(di) % 2:
                buf += b"\x00"
        return bytes(buf)
    put(lpt_lba, emit_pt(False))
    put(mpt_lba, emit_pt(True))

    # directory extents
    for n in dirs:
        buf = bytearray()
        buf += _dir_record(b"", n.lba, n.length, True, 0)
        parent = n.parent if n is not root else n
        buf += _dir_record(b"", parent.lba, parent.length, True, 1)
        for c in sorted(n.children, key=lambda c: _iso_name(c.name, c.is_dir)):
            rec = _dir_record(_iso_name(c.name, c.is_dir), c.lba, c.length, c.is_dir)
            if (len(buf) % SECTOR) + len(rec) > SECTOR:
                buf += b"\x00" * (SECTOR - (len(buf) % SECTOR))
            buf += rec
        put(n.lba, bytes(buf))

    # files
    for n in dirs:
        for c in n.children:
            if not c.is_dir:
                put(c.lba, c.data)

    with open(out_path, "wb") as f:
        f.write(out)
    return total_sectors * SECTOR


def _selftest():
    import tempfile, sys as _s
    d = tempfile.mkdtemp()
    os.makedirs(os.path.join(d, "src", "WIN95"))
    open(os.path.join(d, "src", "READ.ME"), "wb").write(b"hello iso\n" * 50)
    open(os.path.join(d, "src", "WIN95", "SETUP.EXE"), "wb").write(bytes(range(256)) * 40)
    iso = os.path.join(d, "test.iso")
    write_iso(os.path.join(d, "src"), iso, volid="TESTVOL")
    print("wrote %s (%d bytes)" % (iso, os.path.getsize(iso)))
    _s.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from codebreaker.engines.build.iso9660 import Iso9660
    it = Iso9660(iso)
    found = {}
    for path, rec in it.walk():
        found[path.lstrip("/").split(";")[0].upper()] = it.read_file(rec)
    ok = (found.get("READ.ME") == b"hello iso\n" * 50 and
          found.get("WIN95/SETUP.EXE") == bytes(range(256)) * 40)
    print("read back files:", sorted(found))
    print("SELFTEST:", "PASS" if ok else "FAIL")
    return ok


def main():
    if "--selftest" in sys.argv:
        sys.exit(0 if _selftest() else 1)
    if len(sys.argv) < 3:
        sys.exit("usage: isowrite.py OUT.ISO SRCDIR [--boot IMG] [--volid ID]")
    out, src = sys.argv[1], sys.argv[2]
    boot = sys.argv[sys.argv.index("--boot") + 1] if "--boot" in sys.argv else None
    volid = sys.argv[sys.argv.index("--volid") + 1] if "--volid" in sys.argv else "WIN95"
    n = write_iso(src, out, volid=volid, boot_img=boot)
    print("wrote %s: %d bytes%s" % (out, n, " (bootable)" if boot else ""))


if __name__ == "__main__":
    main()
