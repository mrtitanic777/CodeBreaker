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


class GDF:
    def __init__(self, dev, base=None):
        self.dev = dev
        self.base = base if base is not None else self._find_base()
        if self.base is None:
            raise ValueError("no XDVDFS volume found")
        vd = self.dev.read_at(self.base + 32 * SECTOR, SECTOR)
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

    def _walk_dir(self, sector, size, prefix=""):
        """Read a directory's sectors and walk its entry tree in offset order."""
        data = self._sector(sector, (size + SECTOR - 1) // SECTOR * SECTOR)
        out = []
        # entries are addressed by 4-byte-unit offset from the start of the dir;
        # walk them linearly by advancing past each entry (the tree is laid out
        # densely), skipping padding (0xFF / 0x00 filler between sectors).
        pos = 0
        n = len(data)
        while pos + 0x0E <= n:
            left, right, start, fsize = struct.unpack_from("<HHII", data, pos)
            attr = data[pos + 0x0C]
            nlen = data[pos + 0x0D]
            if nlen == 0 or nlen == 0xFF:
                # advance to next 4-byte boundary / next sector's start
                pos = (pos // SECTOR + 1) * SECTOR
                continue
            name = data[pos + 0x0E:pos + 0x0E + nlen].decode("latin-1", "replace")
            full = prefix + "/" + name if prefix else name
            is_dir = bool(attr & 0x10)
            out.append((full, is_dir, start, fsize))
            if is_dir and fsize:
                out += self._walk_dir(start, fsize, full)
            entry_len = 0x0E + nlen
            pos += (entry_len + 3) & ~3                    # 4-byte align
        return out

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
    names = list(files)
    # lay out: sector 32 = volume descriptor, 33 = root dir, 34.. = file data
    root_sector = 33
    data_sector = 34
    entries = bytearray()
    sec = data_sector
    placed = []
    for i, nm in enumerate(names):
        data = files[nm]
        nb = (len(data) + SECTOR - 1) // SECTOR
        nbytes = nm.encode("latin-1")
        # left/right subtree = 0 (flat list is a degenerate tree walked linearly)
        e = struct.pack("<HHII", 0, 0, sec, len(data)) + bytes([0x20, len(nbytes)]) + nbytes
        e += b"\x00" * ((-len(e)) & 3)                # 4-byte align
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
