"""media.py -- read media and containers: floppy, hard-disk and CD images, module bundles, and
the Xbox / Xbox 360 containers (formats/xbox/volumes.py: STFS, discs, GoD, FATX, ZAR, XZP, PCK).

    from codebreaker.formats import media
    for v in media.volumes(path):          every volume inside an image file (or GoD folder)
        v.kind                             "fat12" | "fat16" | "fat32" | "iso9660" | "bundle" |
                                           "stfs" | "xdvdfs" | "god" | "fatx" | "zar" | "xzp" | "pck"
        v.label, v.where                   its name; where in the image it is
        v.boot                             {field: value} of its boot sector / volume descriptor
        for e in v.files():                every file and folder: e.path  e.size  e.is_dir  e.when
            data = v.read(e)
    media.extract(path, folder, names=())  write the files out; -> number written
    media.put(path, {"WINDOWS/SYSTEM/X.VXD": data})   write files into an image's FAT volume in
                                           place (replaced or added; folders made as needed)

    python -m codebreaker media list <image> [--boot] [--plain]
    python -m codebreaker media extract <image> <folder> [name ...]

One image file can hold several volumes: a floppy image is one FAT volume; a hard-disk image
(also a "flat" virtual disk) is a partition table and a FAT volume per partition, logical
drives included; a CD image is an ISO-9660 volume and, when it is bootable by floppy emulation
(El Torito), the boot floppy inside it as a FAT volume of its own; and a Windows 1.x boot image
(WIN100.BIN with its .OVL) is a bundle of system modules, each written out as an ordinary
one-file executable (codebreaker.formats.ne does the taking apart).

FAT: 12-, 16- and 32-bit tables told apart by the number of clusters, as DOS does; long file
names are read (`--plain` shows the 8.3 names) and put() writes by the 8.3 ones; floppies from before DOS 2.0 carry no parameter
block, so their layout is taken from the media byte and the image size. ISO-9660: the primary
names, or the long (Joliet) names when the disc has them (`--plain` for the primary ones);
images with raw 2352-byte sectors are read too. Files are read on demand, never the whole image.
"""
import fnmatch
import os
import struct

# floppies with no parameter block: media byte -> (sectors per cluster, root entries, sectors per FAT, total)
_OLD_FLOPPIES = {0xFE: (1, 64, 1, 320), 0xFF: (2, 112, 1, 640), 0xFC: (1, 64, 2, 360), 0xFD: (2, 112, 2, 720)}
_FAT_PARTITIONS = (0x01, 0x04, 0x06, 0x0B, 0x0C, 0x0E, 0x11, 0x14, 0x16, 0x1B, 0x1C, 0x1E)
_EXTENDED = (0x05, 0x0F, 0x85)


class Entry(object):
    """One file or folder of a volume."""
    def __init__(self, path, size, is_dir, start, when="", short=None, attributes=0):
        self.path, self.size, self.is_dir, self.start = path, size, is_dir, start
        self.when, self.short, self.attributes = when, short or path, attributes

    def __repr__(self):
        return "Entry(%r, %d)" % (self.path, self.size)


class _Source(object):
    """Bytes of an image file from an offset on, read as asked for."""
    def __init__(self, path, offset=0):
        self.path, self.offset = path, offset

    def read(self, at, n):
        with open(self.path, "rb") as f:
            f.seek(self.offset + at)
            return f.read(n)


def _text(raw):
    return raw.decode("cp437", "replace").rstrip(" \0")


# FAT ------------------------------------------------------------------------------------------
class FatVolume(object):
    def __init__(self, source, where="", size=None):
        self.src, self.where = source, where
        b = source.read(0, 512)
        if len(b) < 512:
            raise ValueError("no boot sector")
        (bps, spc, reserved, fats, roots, total16, media, spf16, spt, heads, hidden, total32) = \
            struct.unpack_from("<HBHBHHBHHHII", b, 11)
        ok = bps in (512, 1024, 2048, 4096) and spc and spc & (spc - 1) == 0 and 1 <= fats <= 2 and reserved \
            and media >= 0xF0 and (total16 or total32) and b[0] in (0xEB, 0xE9, 0x69)
        self.old = False
        if not ok:                                            # before DOS 2.0: no parameter block
            fat0 = source.read(512, 3)
            if len(fat0) == 3 and fat0[0] in _OLD_FLOPPIES and fat0[1:] == b"\xFF\xFF" \
                    and (size is None or size <= 512 * 720):
                spc, roots, spf16, total16 = _OLD_FLOPPIES[fat0[0]]
                bps, reserved, fats, media, spt, heads, hidden, total32 = 512, 1, 2, fat0[0], 8 + (fat0[0] & 2 == 0), 0, 0, 0
                self.old = True
            else:
                raise ValueError("not a FAT volume")
        spf = spf16 or struct.unpack_from("<I", b, 36)[0]
        total = total16 or total32
        self.bps, self.spc, self.roots, self.fats, self.spf = bps, spc, roots, fats, spf
        self.fat_at = reserved * bps
        self.root_at = self.fat_at + fats * spf * bps
        self.data_at = self.root_at + (roots * 32 + bps - 1) // bps * bps
        clusters = self.clusters = (total * bps - self.data_at) // (spc * bps)
        self.bits = 12 if clusters < 4085 else 16 if clusters < 65525 else 32
        self.kind = "fat%d" % self.bits
        self.root_cluster = struct.unpack_from("<I", b, 44)[0] if self.bits == 32 else 0
        ext = 64 if self.bits == 32 else 36                   # the extended fields follow the table size
        has_ext = not self.old and b[ext + 2] in (0x28, 0x29)
        self.boot = {
            "system": "(no parameter block)" if self.old else _text(b[3:11]), "bytes per sector": bps,
            "sectors per cluster": spc, "reserved sectors": reserved, "fats": fats, "root entries": roots,
            "total sectors": total, "media": "%02X" % media, "sectors per fat": spf, "sectors per track": spt,
            "heads": heads, "hidden sectors": hidden, "clusters": clusters,
            "serial": "%08X" % struct.unpack_from("<I", b, ext + 3)[0] if has_ext else "",
            "label": _text(b[ext + 7:ext + 18]) if has_ext else "",
            "file system": _text(b[ext + 18:ext + 26]) if has_ext and b[ext + 2] == 0x29 else "",
            "signature": "%02X%02X" % (b[510], b[511]), "boot code": "" if self.old else _jump(b)}
        self.fat = source.read(self.fat_at, spf * bps)
        self.label = self.boot["label"]
        self._entries = None

    def _next(self, c):
        f = self.fat
        if self.bits == 12:
            if c * 3 // 2 + 2 > len(f):
                return 0xFFF
            v = f[c * 3 // 2] | f[c * 3 // 2 + 1] << 8
            return v >> 4 if c & 1 else v & 0xFFF
        if self.bits == 16:
            return struct.unpack_from("<H", f, c * 2)[0] if c * 2 + 2 <= len(f) else 0xFFFF
        return struct.unpack_from("<I", f, c * 4)[0] & 0x0FFFFFFF if c * 4 + 4 <= len(f) else 0x0FFFFFFF

    def _chain(self, cluster, size=None):
        """The bytes of a cluster chain (all of it, or the first `size` bytes)."""
        last = (1 << self.bits if self.bits < 32 else 1 << 28) - 16
        step, out, got, seen = self.spc * self.bps, [], 0, set()
        while 2 <= cluster < last and cluster not in seen and (size is None or got < size):
            seen.add(cluster)
            out.append(self.src.read(self.data_at + (cluster - 2) * step, step))
            got += step
            cluster = self._next(cluster)
        data = b"".join(out)
        return data if size is None else data[:size]

    def _folder(self, raw, prefix, short_prefix):
        long_parts, out = [], []
        for i in range(0, len(raw) - 31, 32):
            e = raw[i:i + 32]
            if e[0] == 0:
                break
            attr = e[11]
            if e[0] == 0xE5:
                long_parts = []
                continue
            if attr & 0x3F == 0x0F:                           # a piece of a long name; the last comes first
                if e[0] & 0x40:
                    long_parts = []
                long_parts.insert(0, e[1:11] + e[14:26] + e[28:32])
                continue
            base, ext = _text(bytes([0xE5 if e[0] == 5 else e[0]]) + e[1:8]), _text(e[8:11])
            short = base + ("." + ext if ext else "")
            name = b"".join(long_parts).decode("utf-16-le", "replace").split("\0")[0] if long_parts else short
            long_parts = []
            if attr & 0x08:                                   # the volume label
                self.label = self.label or _text(e[0:11])
                continue
            if short in (".", ".."):
                continue
            time_, date = struct.unpack_from("<HH", e, 22)
            when = "%04d-%02d-%02d %02d:%02d" % (1980 + (date >> 9), date >> 5 & 15, date & 31, time_ >> 11, time_ >> 5 & 63)
            cluster = struct.unpack_from("<H", e, 26)[0] | (struct.unpack_from("<H", e, 20)[0] << 16 if self.bits == 32 else 0)
            out.append(Entry(prefix + name, struct.unpack_from("<I", e, 28)[0], bool(attr & 0x10), cluster, when,
                             short_prefix + short, attr))
        return out

    def files(self):
        """Every file and folder, folders before what is in them."""
        if self._entries is None:
            root = self._chain(self.root_cluster) if self.bits == 32 else self.src.read(self.root_at, self.roots * 32)
            out, work, seen = [], self._folder(root, "", ""), set()
            while work:
                e = work.pop(0)
                out.append(e)
                if e.is_dir and e.start not in seen and len(seen) < 20000:
                    seen.add(e.start)
                    work[0:0] = self._folder(self._chain(e.start), e.path + "/", e.short + "/")
            self._entries = out
        return self._entries

    def read(self, entry):
        return b"" if entry.is_dir else self._chain(entry.start, entry.size)


def _jump(b):
    """Where a boot sector's first instruction jumps to (its code starts there)."""
    if b[0] == 0xEB:
        return "jmp %03Xh" % (2 + b[1])
    return "jmp %03Xh" % ((3 + struct.unpack_from("<H", b, 1)[0]) & 0xFFFF) if b[0] == 0xE9 else ""


def _partitions(source, size):
    """[(first sector, sectors, type)] of the FAT partitions of a hard-disk image, logical drives
    inside extended partitions included."""
    out, tables, seen, extended_base = [], [0], set(), None
    while tables:
        at = tables.pop(0)
        if at in seen or len(seen) > 64:
            continue
        seen.add(at)
        b = source.read(at * 512, 512)
        if len(b) < 512 or b[510:512] != b"\x55\xAA":
            continue
        for i in range(4):
            kind = b[446 + 16 * i + 4]
            first, count = struct.unpack_from("<II", b, 446 + 16 * i + 8)
            if not kind or not count:
                continue
            if kind in _EXTENDED:
                if extended_base is None:
                    extended_base = first
                    tables.append(first)
                else:
                    tables.append(extended_base + first)
            elif kind in _FAT_PARTITIONS and (at + first) * 512 < size:
                out.append((at + first, count, kind))
    return out


# ISO 9660 ---------------------------------------------------------------------------------------
class IsoVolume(object):
    kind = "iso9660"

    def __init__(self, source, sector=2048, skip=0, long_names=True):
        """`sector`/`skip`: 2048/0 for a plain image; 2352/16 or 2352/24 for raw sectors."""
        self.src, self.sector, self.skip, self.where = source, sector, skip, ""
        primary = joliet = None
        self.boot_catalog = None
        for n in range(16, 48):
            d = self._sectors(n, 1)
            if len(d) < 2048 or d[1:6] != b"CD001" or d[0] == 255:
                break
            if d[0] == 1 and primary is None:
                primary = d
            elif d[0] == 2 and d[88:91] in (b"%/@", b"%/C", b"%/E"):
                joliet = d
            elif d[0] == 0 and d[7:30] == b"EL TORITO SPECIFICATION":
                self.boot_catalog = struct.unpack_from("<I", d, 71)[0]
        if primary is None:
            raise ValueError("not an ISO-9660 volume")
        self.long_names = bool(joliet) and long_names
        d = joliet if self.long_names else primary
        self.block = struct.unpack_from("<H", primary, 128)[0] or 2048
        self.label = (d[40:72].decode("utf-16-be", "replace") if self.long_names else _text(d[40:72])).strip()
        self.root = struct.unpack_from("<I", d, 158)[0], struct.unpack_from("<I", d, 166)[0]
        self.boot = {"system": _text(primary[8:40]), "label": _text(primary[40:72]),
                     "blocks": struct.unpack_from("<I", primary, 80)[0], "block size": self.block,
                     "created": _text(primary[813:829]), "long names": "yes" if joliet else "no",
                     "bootable": "yes" if self.boot_catalog else "no"}
        self._entries = None

    def _sectors(self, n, count):
        if self.sector == 2048:
            return self.src.read(n * 2048, count * 2048)
        return b"".join(self.src.read((n + i) * self.sector + self.skip, 2048) for i in range(count))

    def _bytes(self, block, size):
        return self._sectors(block, (size + 2047) // 2048)[:size]

    def files(self):
        if self._entries is None:
            out, work, seen = [], [(self.root[0], self.root[1], "")], set()
            while work:
                block, size, prefix = work.pop(0)
                if block in seen or len(seen) > 50000:
                    continue
                seen.add(block)
                d, i, found = self._bytes(block, size), 0, []
                while i < len(d):
                    n = d[i]
                    if n == 0:                                # records do not cross a block: on to the next
                        i = (i // 2048 + 1) * 2048
                        continue
                    r = d[i:i + n]
                    i += n
                    raw = r[33:33 + r[32]]
                    if raw in (b"\0", b"\1") or len(r) < 34:
                        continue
                    name = raw.decode("utf-16-be", "replace") if self.long_names else raw.decode("latin1")
                    name = name.split(";")[0]
                    name = name[:-1] if name.endswith(".") else name
                    y, mo, dd, h, mi = r[18], r[19], r[20], r[21], r[22]
                    e = Entry(prefix + name, struct.unpack_from("<I", r, 10)[0], bool(r[25] & 2),
                              struct.unpack_from("<I", r, 2)[0] + r[1], "%04d-%02d-%02d %02d:%02d" % (1900 + y, mo, dd, h, mi))
                    if found and found[-1].path == e.path and not e.is_dir:      # a file in several extents
                        found[-1].extents.append((e.start, e.size))
                        found[-1].size += e.size
                        continue
                    e.extents = [(e.start, e.size)]
                    found.append(e)
                for e in found:
                    out.append(e)
                    if e.is_dir:
                        work.append((e.start, e.size, e.path + "/"))
            order = {}
            for e in out:                                     # folders before what is in them
                order.setdefault(e.path.rsplit("/", 1)[0] if "/" in e.path else "", []).append(e)

            def walk(prefix):
                for e in order.get(prefix, []):
                    yield e
                    if e.is_dir:
                        for x in walk(e.path):
                            yield x
            self._entries = list(walk(""))
        return self._entries

    def read(self, entry):
        return b"" if entry.is_dir else b"".join(self._bytes(b, n) for b, n in entry.extents)

    def boot_image(self):
        """(offset in the image file, bytes, emulated media) of the El Torito boot image: media
        0 none (a loader), 1 / 2 / 3 a 1.2 / 1.44 / 2.88 MB floppy, 4 a hard disk. None when
        the disc is not bootable."""
        if not self.boot_catalog:
            return None
        c = self._sectors(self.boot_catalog, 1)
        if c[0] != 1 or c[30:32] != b"\x55\xAA" or c[32] != 0x88:
            return None
        media_type, count, block = c[33] & 15, struct.unpack_from("<H", c, 38)[0], struct.unpack_from("<I", c, 40)[0]
        size = {1: 1228800, 2: 1474560, 3: 2949120}.get(media_type, count * 512)
        return block * self.sector + self.skip, size, media_type


# a bundle of system modules ---------------------------------------------------------------------
class BundleVolume(object):
    """A Windows 1.x boot image: system modules stored together (see formats.ne.open_boot_image)."""
    kind = "bundle"

    def __init__(self, path):
        from codebreaker.formats import ne
        self._ne, self.where = ne, ""
        self.modules = ne.open_boot_image(path)
        if len(self.modules) < 2:
            raise ValueError("not a bundle of modules")
        self.label = os.path.basename(path)
        self.boot = {"modules": len(self.modules)}
        self._built = {}

    def files(self):
        return [Entry(name + ".EXE", len(self._bytes(name)), False, m.ne_off, short=name + ".EXE")
                for name, m in self.modules.items()]

    def _bytes(self, name):
        if name not in self._built:
            self._built[name] = self._ne.standalone(self.modules[name])
        return self._built[name]

    def read(self, entry):
        return self._bytes(entry.path[:-4])


# any image ------------------------------------------------------------------------------------
def volumes(path, long_names=True):
    """Every volume inside an image file. ValueError when nothing in it is recognised."""
    from codebreaker.formats.xbox import volumes as xbox             # first: an Xbox 360 disc also
    found = xbox.volumes(path, long_names)                           # carries an ISO-9660 video part
    if found:
        return found
    if os.path.isdir(path):
        raise ValueError("%s: a folder, and not a Games-on-Demand package" % os.path.basename(path))
    size = os.path.getsize(path)
    src = _Source(path)
    out = []
    for sector, skip in ((2048, 0), (2352, 16), (2352, 24)):
        if src.read(16 * sector + skip + 1, 5) == b"CD001":
            iso = IsoVolume(src, sector, skip, long_names)
            out.append(iso)
            boot = iso.boot_image()
            if boot and boot[2] in (1, 2, 3) and sector == 2048:
                try:
                    v = FatVolume(_Source(path, boot[0]), "boot floppy inside the disc", boot[1])
                    out.append(v)
                except ValueError:
                    pass
            return out
    try:
        return [FatVolume(src, "", size)]
    except ValueError:
        pass
    for first, count, kind in _partitions(src, size):
        try:
            out.append(FatVolume(_Source(path, first * 512), "partition at sector %d, type %02X" % (first, kind), count * 512))
        except ValueError:
            pass
    if out:
        return out
    try:
        return [BundleVolume(path)]
    except (ValueError, struct.error, IndexError, KeyError):
        raise ValueError("%s: no FAT volume, partition table, ISO-9660 volume or module bundle found" % os.path.basename(path))



# Writing into a FAT volume ----------------------------------------------------------------------
class _FatWriter(object):
    """A FAT volume of an image file opened for writing: its table held here and written back to
    every copy by close(); folders and file data written where they lie."""
    def __init__(self, path, vol):
        if not isinstance(vol, FatVolume) or vol.old:
            raise ValueError("not a FAT volume with a parameter block")
        self.v, self.bits, self.base = vol, vol.bits, vol.src.offset
        self.f = open(path, "r+b")
        self.fat = bytearray(vol.fat)
        self.step = vol.spc * vol.bps
        self.end = 0xFFF if vol.bits == 12 else 0xFFFF if vol.bits == 16 else 0x0FFFFFFF
        self.free_from = 2

    def close(self):
        for i in range(self.v.fats):
            self.f.seek(self.base + self.v.fat_at + i * self.v.spf * self.v.bps)
            self.f.write(self.fat)
        self.f.close()

    def get(self, c):
        return FatVolume._next(self, c)

    def set(self, c, value):
        if self.bits == 12:
            at = c * 3 // 2
            old = self.fat[at] | self.fat[at + 1] << 8
            new = (old & 0x000F) | (value & 0xFFF) << 4 if c & 1 else (old & 0xF000) | (value & 0xFFF)
            self.fat[at], self.fat[at + 1] = new & 0xFF, new >> 8
        elif self.bits == 16:
            struct.pack_into("<H", self.fat, c * 2, value & 0xFFFF)
        else:
            keep = struct.unpack_from("<I", self.fat, c * 4)[0] & 0xF0000000
            struct.pack_into("<I", self.fat, c * 4, keep | (value & 0x0FFFFFFF))

    def chain(self, c):
        out, seen = [], set()
        while 2 <= c < self.end - 7 and c not in seen:
            out.append(c)
            seen.add(c)
            c = self.get(c)
        return out

    def allocate(self, n):
        """n free clusters, linked into one chain, the last marked as its end."""
        got, c = [], self.free_from
        while len(got) < n:
            if c >= self.v.clusters + 2:
                raise ValueError("the volume is full")
            if self.get(c) == 0:
                got.append(c)
            c += 1
        self.free_from = c
        for a, b in zip(got, got[1:] + [self.end]):
            self.set(a, b)
        return got

    def write_clusters(self, chain, data):
        for i, c in enumerate(chain):
            self.f.seek(self.base + self.v.data_at + (c - 2) * self.step)
            self.f.write(data[i * self.step:(i + 1) * self.step].ljust(self.step, b"\0"))

    def slots(self, start):
        """[(where in the image, the 32-byte entry)] of a folder; start 0 is the root folder."""
        if start == 0 and self.bits != 32:
            at = [self.base + self.v.root_at + i * 32 for i in range(self.v.roots)]
        else:
            at = [self.base + self.v.data_at + (c - 2) * self.step + i
                  for c in self.chain(start or self.v.root_cluster) for i in range(0, self.step, 32)]
        out = []
        for a in at:
            self.f.seek(a)
            out.append((a, self.f.read(32)))
        return out

    def find(self, start, short):
        for a, e in self.slots(start):
            if e[0] == 0:
                break
            if e[0] != 0xE5 and e[11] & 0x3F != 0x0F and not e[11] & 0x08 and e[:11] == short:
                return a, bytearray(e)
        return None, None

    def new_slot(self, start):
        for a, e in self.slots(start):
            if e[0] in (0, 0xE5):
                return a
        if start == 0 and self.bits != 32:
            raise ValueError("the root folder is full")
        c = self.allocate(1)[0]                               # a folder grows by a cluster
        self.set(self.chain(start or self.v.root_cluster)[-1], c)
        self.write_clusters([c], b"")
        return self.base + self.v.data_at + (c - 2) * self.step

    def start_of(self, e):
        return struct.unpack_from("<H", e, 26)[0] | (struct.unpack_from("<H", e, 20)[0] << 16 if self.bits == 32 else 0)

    def set_start(self, e, cluster):
        struct.pack_into("<H", e, 26, cluster & 0xFFFF)
        if self.bits == 32:
            struct.pack_into("<H", e, 20, cluster >> 16)

    def write_entry(self, at, e):
        self.f.seek(at)
        self.f.write(bytes(e))

    def mkdir(self, folder, short, when):
        """A new folder in `folder`, its "." and ".." in it. -> its first cluster"""
        c = self.allocate(1)[0]
        dot = bytearray(32)
        dot[:11], dot[11] = b".          ", 0x10
        struct.pack_into("<HH", dot, 22, *when)
        self.set_start(dot, c)
        dotdot = bytearray(dot)
        dotdot[:11] = b"..         "
        self.set_start(dotdot, 0 if folder == self.v.root_cluster else folder)
        self.write_clusters([c], bytes(dot + dotdot))
        e = bytearray(dot)
        e[:11] = short
        self.write_entry(self.new_slot(folder), e)
        return c

    def write_file(self, folder, short, data, when):
        """Replace or add one file of `folder`. -> "replaced" | "added" """
        at, e = self.find(folder, short)
        if e is not None:
            if e[11] & 0x10:
                raise ValueError("%s is a folder" % short.decode("cp437"))
            for c in self.chain(self.start_of(e)):
                self.set(c, 0)
            self.free_from, how = 2, "replaced"
        else:
            at, e, how = self.new_slot(folder), bytearray(32), "added"
            e[:11], e[11] = short, 0x20
        n = (len(data) + self.step - 1) // self.step
        chain = self.allocate(n) if n else []
        self.write_clusters(chain, data)
        self.set_start(e, chain[0] if chain else 0)
        struct.pack_into("<HH", e, 22, *when)
        struct.pack_into("<I", e, 28, len(data))
        self.write_entry(at, e)
        return how


def _short(name):
    """An 8.3 name as a directory entry holds it ("vtd.vxd" -> b"VTD     VXD")."""
    base, _, ext = name.upper().partition(".")
    if not base or len(base) > 8 or len(ext) > 3 or any(c in ' ."/\\[]:;=,+*?<>|' or ord(c) < 33 for c in base + ext):
        raise ValueError("not an 8.3 name: %r" % name)
    return (base.ljust(8) + ext.ljust(3)).encode("cp437")


def put(path, files, volume=0):
    """Write `files` {"FOLDER/NAME.EXT": bytes} into FAT volume number `volume` of the image at
    `path`, in place. Paths are 8.3 names in any case, folder by folder. A file that is there is
    replaced -- its clusters freed; its directory entry, attributes and long name kept, with the
    new start, size and time -- and one that is not is added, a missing folder made. Every copy
    of the FAT is written. -> [(path, "replaced" | "added")]"""
    import time
    t = time.localtime()
    when = (t.tm_hour << 11 | t.tm_min << 5 | t.tm_sec // 2, (t.tm_year - 1980) << 9 | t.tm_mon << 5 | t.tm_mday)
    w = _FatWriter(path, volumes(path)[volume])
    done = []
    try:
        for name, data in files.items():
            parts = [p for p in name.replace("\\", "/").split("/") if p]
            folder = w.v.root_cluster if w.bits == 32 else 0
            for part in parts[:-1]:
                _at, e = w.find(folder, _short(part))
                if e is None:
                    folder = w.mkdir(folder, _short(part), when)
                elif e[11] & 0x10:
                    folder = w.start_of(e)
                else:
                    raise ValueError("%s: %s is not a folder" % (name, part))
            done.append(("/".join(parts), w.write_file(folder, _short(parts[-1]), data, when)))
    finally:
        w.close()
    return done


def _safe(path):
    parts = [p for p in path.replace("\\", "/").split("/") if p not in ("", ".", "..")]
    return os.path.join(*["".join(c if c not in '<>:"|?*' and ord(c) >= 32 else "_" for c in p) for p in parts])


def extract(path, folder, names=(), long_names=True):
    """Write the files of an image under `folder` (each volume after the first in a folder of
    its own). `names` limits it to matching names or paths (wildcards allowed). -> files written"""
    n = 0
    for i, v in enumerate(volumes(path, long_names)):
        base = folder if i == 0 else os.path.join(folder, "volume%d" % (i + 1))
        for e in v.files():
            wanted = not names or any(fnmatch.fnmatch(e.path.lower(), w.lower()) or
                                      fnmatch.fnmatch(e.path.rsplit("/", 1)[-1].lower(), w.lower()) for w in names)
            if e.is_dir or not wanted:
                continue
            out = os.path.join(base, _safe(e.path))
            os.makedirs(os.path.dirname(out) or ".", exist_ok=True)
            with open(out, "wb") as f:
                f.write(v.read(e))
            n += 1
    return n


def main(argv):
    """python -m codebreaker media list <image> [--boot] [--plain]
    python -m codebreaker media extract <image> <folder> [name ...] [--plain]

    List or extract the files of a floppy, hard-disk or CD image, or of a Windows 1.x boot
    image. --boot also prints each volume's boot sector fields; --plain uses 8.3 / primary names."""
    flags = [a for a in argv if a.startswith("--")]
    args = [a for a in argv if not a.startswith("--")]
    if len(args) < 2 or args[0] not in ("list", "extract") or (args[0] == "extract" and len(args) < 3):
        print(main.__doc__)
        return 2
    long_names = "--plain" not in flags
    if args[0] == "extract":
        print("%d files written to %s" % (extract(args[1], args[2], args[3:], long_names), args[2]))
        return 0
    for v in volumes(args[1], long_names):
        files = v.files()
        print("%s volume %r%s: %d files, %d folders, %d bytes" % (
            v.kind, v.label, " (%s)" % v.where if v.where else "", sum(1 for e in files if not e.is_dir),
            sum(1 for e in files if e.is_dir), sum(e.size for e in files if not e.is_dir)))
        if "--boot" in flags:
            for k, value in v.boot.items():
                print("    %-20s %s" % (k, value))
        for e in files:
            print("  %10s  %-16s  %s" % ("<folder>" if e.is_dir else e.size, e.when, e.path if long_names else e.short))
    return 0
