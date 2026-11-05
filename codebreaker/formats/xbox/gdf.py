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
import sys

from codebreaker.formats import native

SECTOR = 0x800
MAGIC = b"MICROSOFT*XBOX*MEDIA"
# (the candidate XDVDFS base offsets -- raw, XGD1, XGD2, XGD3, redump -- are tried by formats.dll)


class Device:
    """A byte source held by formats.dll (native/formats/xbox/discs.cc): `read_at(offset, length)`,
    `close()`.  _FileDevice, god.SvodReader and ciso's readers are the three kinds."""
    _id = None

    def read_at(self, offset, length):
        return native.call("xdev.read", None, self._id, offset, length)

    def close(self):
        if self._id is not None:
            native.call("xdev.close", None, self._id)
            self._id = None

    def __del__(self):
        try:
            self.close()
        except Exception:
            pass


class _FileDevice(Device):
    """A seekable byte source over a single disc-image file."""
    def __init__(self, path):
        self._id = native.call("xdev.open_file", None, os.fspath(path))


class GDF:
    def __init__(self, dev, base=None, descriptor=None):
        self.dev = dev
        if descriptor is not None:                           # (where the volume descriptor is, when
            self._descriptor = descriptor                    # not at the volume's sector 32)
        self.base, self.root_sector, self.root_size = native.call("gdf.open", None, dev._id, base, descriptor)

    def _sector(self, sec, length):
        return self.dev.read_at(self.base + sec * SECTOR, length)

    def _walk_dir(self, sector, size, prefix="", seen=None):
        """A directory's entries and, under each folder, its own: [(path, is folder, sector, size)].
        The entries of a directory are a binary tree -- each names the offsets (in 4-byte units)
        of the entries before and after it in name order, 0xFFFF for none -- so it is walked as
        one, from the first entry (a folder of a large Games-on-Demand title once "held" itself,
        without end, when the sectors were read in a row)."""
        return native.call("gdf.walk", None, self.dev._id, self.base, sector, size, prefix)

    def files(self):
        """Return [(path, size)] for every file (not directory)."""
        return native.call("gdf.files", None, self.dev._id, self.base, self.root_sector, self.root_size)

    def read(self, path):
        data = native.call("gdf.read", None, self.dev._id, self.base, self.root_sector, self.root_size, path)
        if data is None:
            raise KeyError(path)
        return data


@native.answered("gdf.build_xdvdfs", data_arg=None)
def build_xdvdfs(files):
    """Build a minimal XDVDFS/GDF disc volume (bytes) from {name: data}. Volume
    descriptor at sector 32, a flat root directory, then the file data. Enough
    for wrapping a default.xex (xex2god); readable by GDF above."""


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
