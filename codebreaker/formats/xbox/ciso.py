"""CSO (Project Stellar / CISO v2) and CCI (Cerbios) — block-compressed Xbox
disc images. Both wrap a raw XDVDFS ISO as 2048-byte LZ4 sectors indexed by a
u32 offset table (top bit = compressed). Reimplemented clean-room from the
documented formats (facts cross-checked against xVerter, MIT).

  CSO: magic 'CISO', 24-byte header, index at 0x18, block = [u32 len][lz4].
  CCI: magic 'CCIM', 32-byte header, index at EOF, block = [u8 pad][lz4][pad].

The decoded stream is a plain ISO, so `gdf` lists/extracts it directly.

Commands: info / list / extract / toiso / tocso / tocci.
"""

import os
import sys

from codebreaker.formats import native
from codebreaker.formats.xbox import gdf as _gdf

DESCRIPTION = "Xbox CSO/CCI compressed disc images: info/list/extract + convert to/from ISO"

CSO_MAGIC = b"CISO"
CCI_MAGIC = b"CCIM"


class CisoError(Exception):
    pass


class Lz4Error(Exception):
    """A damaged LZ4 block inside a CSO / CCI image."""


native.RAISES["CisoError"] = CisoError
native.RAISES["Lz4Error"] = Lz4Error


# ── readers (expose read_at/size/close, so gdf can walk them; formats.dll's) ───────────
class _CompReader(_gdf.Device):
    """size: the uncompressed bytes; _nblocks: its 2048-byte blocks; compressed: how many are compressed."""
    def __init__(self, path):
        self._id, kind, self.size, self._nblocks, self.compressed = native.call("xdev.open_image", None, os.fspath(path))
        if kind != self.KIND:
            self.close()
            raise CisoError("not a %s image" % self.KIND)


class CsoReader(_CompReader):
    KIND = "CSO"


class CciReader(_CompReader):
    KIND = "CCI"


def open_image(path):
    magic = open(path, "rb").read(4)
    if magic == CSO_MAGIC:
        return CsoReader(path)
    if magic == CCI_MAGIC:
        return CciReader(path)
    raise CisoError("%s is not a CSO or CCI image" % path)


# ── writers (stream ISO sectors; patch the index afterwards) ───────────
@native.answered("ciso.write_cso", data_arg=None)
def write_cso(iso_path, out_path):
    """The ISO as a CSO image; -> the uncompressed size."""


@native.answered("ciso.write_cci", data_arg=None)
def write_cci(iso_path, out_path):
    """The ISO as a CCI image; -> the uncompressed size."""


@native.answered("ciso.to_iso", data_arg=None)
def to_iso(image_path, out_path):
    """A CSO / CCI image as a plain ISO; -> the bytes written."""


# ── CLI ────────────────────────────────────────────────────────────────
def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage:\n"
              "  ciso info    <image.cso|.cci>\n"
              "  ciso list    <image.cso|.cci>\n"
              "  ciso extract <image.cso|.cci> <outdir>\n"
              "  ciso toiso   <image.cso|.cci> <out.iso>\n"
              "  ciso tocso   <in.iso> <out.cso>\n"
              "  ciso tocci   <in.iso> <out.cci>")
        return
    op = a[0]
    if op == "tocso":
        print("wrote CSO (%d MB uncompressed) -> %s" % (write_cso(a[1], a[2]) // (1 << 20), a[2])); return
    if op == "tocci":
        print("wrote CCI (%d MB uncompressed) -> %s" % (write_cci(a[1], a[2]) // (1 << 20), a[2])); return
    if op == "toiso":
        print("wrote %d bytes -> %s" % (to_iso(a[1], a[2]), a[2])); return
    r = open_image(a[1])
    if op == "info":
        kind, comp = r.KIND, r.compressed
        print("%s image: %s" % (kind, a[1]))
        print("uncompressed: %d bytes (%d blocks); %d compressed, %d raw; on-disk %d bytes" % (
            r.size, r._nblocks, comp, r._nblocks - comp, os.path.getsize(a[1])))
    elif op in ("list", "extract"):
        g = _gdf.GDF(r)
        if op == "list":
            for p, sz in g.files():
                print("  %12d  %s" % (sz, p))
        else:
            outdir = a[2]
            for p, sz in g.files():
                dst = os.path.join(outdir, p.replace("/", os.sep))
                os.makedirs(os.path.dirname(dst) or ".", exist_ok=True)
                with open(dst, "wb") as f:
                    f.write(g.read(p))
            print("extracted -> %s" % outdir)
    r.close()


if __name__ == "__main__":
    main()
