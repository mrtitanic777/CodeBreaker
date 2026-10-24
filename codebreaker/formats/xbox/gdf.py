#!/usr/bin/env python3
"""GDF / XDVDFS — Xbox 360 disc filesystem reader (and SVOD device).

XDVDFS (a.k.a. GDF, "Game Disc Format") is the filesystem on Xbox 360 discs and
inside SVOD (games-on-demand / installed) content. It has a volume descriptor at
sector 32 and a directory of binary-tree entries; files are runs of 0x800-byte
sectors. A clean-room Python reader written to the public/documented format --
the disc-image half of what the GPL X360 library does -- as original code.

Reads are streamed from the file (disc images are gigabytes), never loaded whole.
The XDVDFS volume sits at a base offset that depends on the disc layout; common
XGD offsets are auto-detected. SVOD content splits the same filesystem across
Data#### files at a fixed base; a SvodDevice presents them as one stream.

Commands:
    gdf list    <image.iso|svod-dir>          the on-disc file tree
    gdf extract <image.iso|svod-dir> <path> [out]   extract one file
    gdf info    <image.iso|svod-dir>          volume descriptor summary
"""
DESCRIPTION = "read Xbox 360 disc images / SVOD (XDVDFS/GDF): info/list/extract"

import os
import struct
import sys

SECTOR = 0x800
MAGIC = b"MICROSOFT*XBOX*MEDIA"
# candidate XDVDFS base offsets (raw, XGD1, XGD2, XGD3, redump)
XGD_BASES = (0, 0xFD90000, 0x2080000, 0xFB20000, 0x18300000, 0x10000)


class _FileDevice:
    """A seekable byte source over a single disc-image file."""
    def __init__(self, path):
        self.f = open(path, "rb")
    def read_at(self, offset, length):
        self.f.seek(offset)
        return self.f.read(length)
    def close(self):
        self.f.close()


_MAX_DIR = 1 << 20          # the largest directory believed (bytes)


class GDF:
    def __init__(self, dev, base=None, descriptor=None):
        self.dev = dev
        self.base = base if base is not None else self._find_base()
        if descriptor is not None:                           # (where the volume descriptor is, when
            self._descriptor = descriptor                    # not at the volume's sector 32)
        if self.base is None:
            raise ValueError("no XDVDFS volume found")
        vd = self.dev.read_at(getattr(self, "_descriptor", self.base + 32 * SECTOR), SECTOR)
        assert vd[:20] == MAGIC
        self.root_sector = struct.unpack_from("<I", vd, 0x14)[0]
        self.root_size = struct.unpack_from("<I", vd, 0x18)[0]

    def _find_base(self):
        for b in XGD_BASES:
            if self.dev.read_at(b + 32 * SECTOR, 20) == MAGIC:
                return b
        return None

    def _sector(self, sec, length):
        return self.dev.read_at(self.base + sec * SECTOR, length)

    def _walk_dir(self, sector, size, prefix="", seen=None):
        """A directory's entries and, under each folder, its own: [(path, is folder, sector, size)].
        The entries of a directory are a binary tree -- each names the offsets (in 4-byte units)
        of the entries before and after it in name order, 0xFFFF for none -- so it is walked as
        one, from the first entry. (Reading the sectors in a row instead takes what is left over
        after the last entry for more entries: a folder of a large Games-on-Demand title then
        "held" itself, without end.)"""
        seen = set() if seen is None else seen
        if sector in seen or not 0 < size <= _MAX_DIR:   # (a directory is never megabytes: an entry
            return []                                    # that says so is not one, or not this volume's)
        seen.add(sector)
        data = self._sector(sector, (size + SECTOR - 1) // SECTOR * SECTOR)
        out, todo, visited = [], [0], set()
        while todo:
            pos = todo.pop()
            if pos in visited or pos + 0x0E > len(data):
                continue
            visited.add(pos)
            left, right, start, fsize = struct.unpack_from("<HHII", data, pos)
            attr, nlen = data[pos + 0x0C], data[pos + 0x0D]
            if nlen in (0, 0xFF) or pos + 0x0E + nlen > len(data):
                continue
            name = data[pos + 0x0E:pos + 0x0E + nlen].decode("latin-1", "replace")
            full = prefix + "/" + name if prefix else name
            is_dir = bool(attr & 0x10)
            out.append((full, is_dir, start, fsize))
            if is_dir:
                out += self._walk_dir(start, fsize, full, seen)
            for nxt in (left, right):
                if nxt != 0xFFFF:
                    todo.append(nxt * 4)
        return sorted(out, key=lambda e: e[0].lower())

    def files(self):
        """Return [(path, size)] for every file (not directory)."""
        return [(p, sz) for p, is_dir, st, sz in
                self._walk_dir(self.root_sector, self.root_size) if not is_dir]

    def read(self, path):
        for p, is_dir, start, size in self._walk_dir(self.root_sector, self.root_size):
            if not is_dir and p.lower() == path.lower():
                return self._sector(start, size)
        raise KeyError(path)


def build_xdvdfs(files):
    """Build a minimal XDVDFS/GDF disc volume (bytes) from {name: data}. Volume
    descriptor at sector 32, a flat root directory, then the file data. Enough
    for wrapping a default.xex (xex2god); readable by GDF above."""
    names = sorted(files, key=lambda n: n.upper())       # (the tree is in the console's name order)
    # lay out: sector 32 = volume descriptor, 33 = root dir, 34.. = file data
    root_sector = 33
    data_sector = 34 + (sum(0x0E + len(n) + 3 for n in names) + SECTOR - 1) // SECTOR
    entries = bytearray()
    sec = data_sector
    placed = []
    for i, nm in enumerate(names):
        data = files[nm]
        nb = (len(data) + SECTOR - 1) // SECTOR
        nbytes = nm.encode("latin-1")
        size = (0x0E + len(nbytes) + 3) & ~3
        if len(entries) // SECTOR != (len(entries) + size - 1) // SECTOR:   # (no entry crosses a sector)
            entries += b"\xff" * ((-len(entries)) % SECTOR)
        # the entries as a tree, each the next one's parent on its right (a sorted chain); the
        # last points nowhere (0xFFFF)
        here = len(entries)
        right = 0xFFFF if i == len(names) - 1 else (here + size) // 4
        e = struct.pack("<HHII", 0xFFFF, right, sec, len(data)) + bytes([0x20, len(nbytes)]) + nbytes
        e += b"\xff" * ((-len(e)) & 3)                   # 4-byte align
        entries += e
        placed.append((sec, data))
        sec += nb
    root_size = len(entries)
    total_sectors = sec
    vol = bytearray(total_sectors * SECTOR)
    # volume descriptor at sector 32
    vd = 32 * SECTOR
    vol[vd:vd + 20] = MAGIC
    struct.pack_into("<II", vol, vd + 0x14, root_sector, root_size)
    vol[vd + 0x7EC:vd + 0x800] = MAGIC[:0x14]
    # root directory
    vol[root_sector * SECTOR:root_sector * SECTOR + len(entries)] = entries
    # file data
    for st, data in placed:
        vol[st * SECTOR:st * SECTOR + len(data)] = data
    return bytes(vol)


def _open(path):
    if os.path.isdir(path):
        # a GoD/SVOD .data directory: use the hash-block-aware deinterleaver
        from codebreaker.formats.xbox import god
        return GDF(god.SvodReader(path), base=0)
    return GDF(_FileDevice(path))


# ------------------------------------------------------------------------ CLI
def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: gdf {info|list|extract} <image.iso|svod-dir> [path] [out]"); return
    op = a[0]
    if len(a) < 2:
        print("usage: gdf %s <image|svod-dir>" % op); sys.exit(2)
    g = _open(a[1])
    if op == "info":
        fs = g.files()
        print("volume base : 0x%X" % g.base)
        print("root sector : %d  size 0x%X" % (g.root_sector, g.root_size))
        print("files       : %d, %d bytes total" % (len(fs), sum(s for _, s in fs)))
    elif op == "list":
        fs = g.files()
        for p, sz in fs:
            print("  %12d  %s" % (sz, p))
        print("total: %d files" % len(fs))
    elif op == "extract":
        if len(a) < 3:
            print("usage: gdf extract <image> <path-on-disc> [out]"); sys.exit(2)
        data = g.read(a[2])
        out = a[3] if len(a) > 3 else os.path.basename(a[2].replace("/", os.sep))
        with open(out, "wb") as f:
            f.write(data)
        print("extracted %s (%d bytes) -> %s" % (a[2], len(data), out))
    else:
        print("unknown op %r" % op); sys.exit(2)


if __name__ == "__main__":
    main()
