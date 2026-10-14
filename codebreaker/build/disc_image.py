"""disc_image.py -- write an ISO-9660 CD image from a folder.

The writer produces a plain level-1 image (8.3 upper-case names, no Joliet or Rock Ridge
extensions), optionally bootable through an El Torito boot record with floppy emulation. File
contents are copied through in pieces, never held in memory whole. Names are made to fit:
upper-cased, characters other than A-Z 0-9 _ become _, cut to 8 characters plus a 3-character
extension; if two names in one folder end up the same the write is refused and the clashes
are listed (DiscError), instead of producing an image with two files of one name.

    from codebreaker.build import disc_image
    disc_image.write(folder, out_path, volume_id="CODEBREAKER", boot_image=None)  # -> bytes

Reading an image back (and floppy and hard-disk images) is codebreaker/formats/media.py; the
--list and --extract forms of the command below go there.

    python -m codebreaker disc <folder> <out.iso> [--volume NAME] [--boot IMG]
    python -m codebreaker disc --list <image.iso>
    python -m codebreaker disc --extract <image.iso> <outdir>
"""
import os
import struct
import sys
import time

from codebreaker.formats import media

SECTOR = 2048
_ALLOWED = set("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_")
# El Torito media type by boot image size: 1.2 MB, 1.44 MB and 2.88 MB floppies are emulated;
# any other size is loaded as-is ("no emulation").
_FLOPPY = {1228800: 1, 1474560: 2, 2949120: 3}


class DiscError(ValueError):
    pass


def _both16(v):
    return struct.pack("<H", v) + struct.pack(">H", v)


def _both32(v):
    return struct.pack("<I", v) + struct.pack(">I", v)


def _sectors(size):
    return (size + SECTOR - 1) // SECTOR


def _clean(text, limit):
    return "".join(c if c in _ALLOWED else "_" for c in text.upper())[:limit]


def level1_name(name, is_dir):
    """The name a file or folder gets on the disc, as (base, extension)."""
    if is_dir:
        return _clean(name, 8) or "_", ""
    base, dot, ext = name.rpartition(".")
    if not dot:
        base, ext = name, ""
    return _clean(base, 8) or "_", _clean(ext, 3)


class _Node:
    __slots__ = ("source", "key", "ident", "is_dir", "children", "parent", "number",
                 "lba", "size", "stamp")

    def __init__(self, source, key, is_dir, parent):
        self.source, self.key, self.is_dir, self.parent = source, key, is_dir, parent
        # a file's identifier always has the dot and a version number: "README.;1"
        self.ident = (key[0] if is_dir else "%s.%s;1" % key).encode("ascii")
        self.children, self.number, self.lba = [], 0, 0
        info = os.stat(source)
        self.size = 0 if is_dir else info.st_size
        t = time.gmtime(info.st_mtime)
        self.stamp = bytes([max(0, min(255, t.tm_year - 1900)), t.tm_mon, t.tm_mday,
                            t.tm_hour, t.tm_min, t.tm_sec, 0])


def _scan(folder):
    """The tree to write. Children of every folder are in the order the standard asks of
    directory records (by name, then extension). Returns (root, clashes)."""
    root = _Node(folder, ("", ""), True, None)
    root.parent = root
    clashes = []

    def fill(node, depth):
        if depth > 8:
            raise DiscError("%s: folders nest deeper than the 8 levels ISO-9660 allows"
                            % node.source)
        seen = {}
        for entry in sorted(os.scandir(node.source), key=lambda e: e.name):
            is_dir = entry.is_dir()
            if not is_dir and not entry.is_file():
                continue
            key = level1_name(entry.name, is_dir)
            seen.setdefault((is_dir, key), []).append(entry.name)
            node.children.append(_Node(entry.path, key, is_dir, node))
        for (is_dir, key), names in seen.items():
            if len(names) > 1:
                clashes.append("%s: %s all become %s" % (
                    node.source, ", ".join(names), key[0] if is_dir else "%s.%s" % key))
        node.children.sort(key=lambda n: n.key)
        for child in node.children:
            if child.is_dir:
                fill(child, depth + 1)
    fill(root, 1)
    return root, clashes


def _numbered(root):
    """Folders in path-table order -- by level, then by parent's number, then by name -- each
    given its 1-based number."""
    order, level = [], [root]
    while level:
        order += level
        level = [c for node in level for c in node.children if c.is_dir]
    if len(order) > 0xFFFF:
        raise DiscError("more than 65535 folders")
    for number, node in enumerate(order, 1):
        node.number = number
    return order


def _record(ident, lba, size, is_dir, stamp):
    length = 33 + len(ident) + (1 - len(ident) % 2)          # padded to an even length
    record = bytearray(length)
    record[0] = length
    record[2:10], record[10:18], record[18:25] = _both32(lba), _both32(size), stamp
    record[25] = 2 if is_dir else 0
    record[28:32] = _both16(1)
    record[32] = len(ident)
    record[33:33 + len(ident)] = ident
    return bytes(record)


def _directory(node):
    """The bytes of one folder's directory: "." and "..", then its children. A record never
    crosses a sector boundary; the whole is padded to full sectors."""
    out = bytearray(_record(b"\0", node.lba, node.size, True, node.stamp))
    out += _record(b"\1", node.parent.lba, node.parent.size, True, node.parent.stamp)
    for child in node.children:
        record = _record(child.ident, child.lba, child.size, child.is_dir, child.stamp)
        if len(out) % SECTOR + len(record) > SECTOR:
            out += bytes(-len(out) % SECTOR)
        out += record
    return bytes(out) + bytes(-len(out) % SECTOR)


def _path_table(folders, big_endian):
    out = bytearray()
    for node in folders:
        ident = node.ident or b"\0"
        out += bytes([len(ident), 0])
        out += struct.pack(">IH" if big_endian else "<IH", node.lba, node.parent.number)
        out += ident + bytes(len(ident) % 2)
    return bytes(out)


def _boot_catalog(image_lba, image_size):
    """El Torito boot catalog: a validation entry (its 16-bit words sum to zero) and the
    default entry naming the boot image."""
    check = bytearray(32)
    check[0] = 1                                             # header; platform 0 = x86
    check[4:15] = b"CODEBREAKER"
    check[30:32] = b"\x55\xAA"
    total = sum(struct.unpack("<16H", check))
    check[28:30] = struct.pack("<H", -total & 0xFFFF)
    media = _FLOPPY.get(image_size, 0)
    count = 1 if media else min(0xFFFF, (image_size + 511) // 512)   # 512-byte sectors to load
    entry = struct.pack("<BBHBBHI", 0x88, media, 0, 0, 0, count, image_lba)
    return bytes(check) + entry + bytes(SECTOR - 32 - len(entry))


def _descriptor(kind, body=b""):
    return bytes([kind]) + b"CD001\x01" + body + bytes(SECTOR - 7 - len(body))


def write(folder, out_path, volume_id="CODEBREAKER", boot_image=None):
    """Write the contents of `folder` as an ISO-9660 image; returns the image size in bytes.
    `boot_image` (a floppy image, normally 1474560 bytes) makes the disc bootable."""
    if not os.path.isdir(folder):
        raise DiscError("%s is not a folder" % folder)
    root, clashes = _scan(folder)
    if clashes:
        raise DiscError("names that clash once cut to 8.3:\n  " + "\n  ".join(clashes))
    folders = _numbered(root)
    files = [c for node in folders for c in node.children if not c.is_dir]
    big = [f.source for f in files if f.size >= 1 << 32]
    if big:
        raise DiscError("too large for one ISO-9660 file (4 GB): " + ", ".join(big))

    # layout: 16 system sectors, descriptors, both path tables, boot, directories, files
    boot_size = os.path.getsize(boot_image) if boot_image else 0
    lba = 17 + (1 if boot_image else 0) + 1
    table_size = len(_path_table(folders, False))
    little_at, big_at = lba, lba + _sectors(table_size)
    lba = big_at + _sectors(table_size)
    catalog_at, boot_at = lba, lba + 1
    if boot_image:
        lba += 1 + _sectors(boot_size)
    for node in folders:
        node.size = len(_directory(node))
        node.lba = lba
        lba += node.size // SECTOR
    for node in files:
        node.lba = lba
        lba += _sectors(node.size)
    total = lba

    now = time.strftime("%Y%m%d%H%M%S00", time.gmtime()).encode() + b"\0"
    unset = b"0" * 16 + b"\0"
    volume = bytearray(SECTOR - 7)                           # offsets below are sector offset - 7
    volume[1:33] = b" " * 32                                 # system identifier
    volume[33:65] = (_clean(volume_id, 32) or "CODEBREAKER").ljust(32).encode()
    volume[73:81] = _both32(total)
    volume[113:117], volume[117:121], volume[121:125] = _both16(1), _both16(1), _both16(SECTOR)
    volume[125:133] = _both32(table_size)
    volume[133:137], volume[141:145] = struct.pack("<I", little_at), struct.pack(">I", big_at)
    volume[149:183] = _record(b"\0", root.lba, root.size, True, root.stamp)
    volume[183:806] = b" " * 623                             # set, publisher, preparer, ... names
    volume[806:823], volume[823:840], volume[840:857], volume[857:874] = now, now, unset, unset
    volume[874] = 1

    with open(out_path, "wb") as out:
        def put(at, data):
            out.seek(at * SECTOR)
            out.write(data)
        put(16, _descriptor(1, bytes(volume)))
        if boot_image:
            put(17, _descriptor(0, b"EL TORITO SPECIFICATION".ljust(64, b"\0")
                                + struct.pack("<I", catalog_at)))
            put(catalog_at, _boot_catalog(boot_at, boot_size))
            files = [_BootFile(boot_image, boot_at, boot_size)] + files
        put(17 + (1 if boot_image else 0), _descriptor(255))
        put(little_at, _path_table(folders, False))
        put(big_at, _path_table(folders, True))
        for node in folders:
            put(node.lba, _directory(node))
        for node in files:
            out.seek(node.lba * SECTOR)
            left = node.size
            with open(node.source, "rb") as source:
                while left:
                    piece = source.read(min(left, 1 << 20))
                    if not piece:
                        raise DiscError("%s shrank while it was being copied" % node.source)
                    out.write(piece)
                    left -= len(piece)
        out.truncate(total * SECTOR)
    return total * SECTOR


class _BootFile:
    def __init__(self, source, lba, size):
        self.source, self.lba, self.size = source, lba, size


def main(argv):
    """python -m codebreaker disc <folder> <out.iso> [--volume NAME] [--boot IMG]
    python -m codebreaker disc --list <image.iso>
    python -m codebreaker disc --extract <image.iso> <outdir>"""
    args, options = [], {}
    rest = list(argv)
    while rest:
        a = rest.pop(0)
        if a in ("--volume", "--boot") and rest:
            options[a] = rest.pop(0)
        elif a in ("--list", "--extract"):
            options[a] = True
        else:
            args.append(a)
    try:
        if "--list" in options and len(args) == 1:
            return media.main(["list", args[0], "--boot"])
        if "--extract" in options and len(args) == 2:
            print("extracted %d files to %s" % (media.extract(args[0], args[1]), args[1]))
            return 0
        if len(args) == 2 and "--list" not in options and "--extract" not in options:
            size = write(args[0], args[1], volume_id=options.get("--volume", "CODEBREAKER"),
                         boot_image=options.get("--boot"))
            print("wrote %s: %d bytes%s" % (args[1], size,
                                            " (bootable)" if "--boot" in options else ""))
            return 0
    except (DiscError, OSError) as problem:
        print("disc: %s" % problem, file=sys.stderr)
        return 1
    print(main.__doc__, file=sys.stderr)
    return 2
