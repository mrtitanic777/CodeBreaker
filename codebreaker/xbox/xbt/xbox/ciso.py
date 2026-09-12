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
import struct
import sys

import _lz4

DESCRIPTION = "Xbox CSO/CCI compressed disc images: info/list/extract + convert to/from ISO"

BLOCK_SIZE = 2048
INDEX_ALIGNMENT = 2                       # file offsets stored >> 2

CSO_MAGIC = b"CISO"
CSO_HEADER = struct.Struct("<4sLQLBB2x")  # magic, hsize, usize, bsize, ver, align, pad
CSO_HEADER_SIZE = 0x18

CCI_MAGIC = b"CCIM"
CCI_HEADER = struct.Struct("<4sLQQLBBH")  # magic, hsize, usize, ioff, bsize, ver, align, resv
CCI_HEADER_SIZE = 32
CCI_COMPRESS_THRESHOLD = 2040


class CisoError(Exception):
    pass


def _align4(n):
    return (n + 3) & ~3


# ── readers (expose read_at/size/close, so gdf can walk them) ───────────
class _CompReader:
    def __init__(self, path):
        self._f = open(path, "rb")
        self._cache_idx = -1
        self._cache = b""
        self._parse()

    def close(self):
        try:
            self._f.close()
        except Exception:
            pass

    def _index(self, index_offset, nblocks):
        self._f.seek(index_offset)
        raw = self._f.read((nblocks + 1) * 4)
        if len(raw) < (nblocks + 1) * 4:
            raise CisoError("truncated index")
        vals = struct.unpack("<%dI" % (nblocks + 1), raw)
        self._offs = [(v & 0x7FFFFFFF) << INDEX_ALIGNMENT for v in vals]
        self._flags = [bool(v & 0x80000000) for v in vals]
        if self._flags[nblocks]:
            raise CisoError("index terminator has compressed bit set")

    def read_at(self, offset, length):
        out = bytearray()
        end = min(offset + length, self.size)
        while offset < end:
            bi = offset // BLOCK_SIZE
            blk = self._block(bi)
            o = offset % BLOCK_SIZE
            take = min(BLOCK_SIZE - o, end - offset)
            out += blk[o:o + take]
            offset += take
        return bytes(out)

    def _block(self, bi):
        if bi == self._cache_idx:
            return self._cache
        blk = self._decode_block(bi)
        self._cache_idx, self._cache = bi, blk
        return blk


class CsoReader(_CompReader):
    def _parse(self):
        head = self._f.read(CSO_HEADER_SIZE)
        magic, hsize, usize, bsize, ver, align = CSO_HEADER.unpack(head)
        if magic != CSO_MAGIC or hsize != CSO_HEADER_SIZE or bsize != BLOCK_SIZE:
            raise CisoError("not a CSO image")
        self.size = usize
        self._nblocks = usize // BLOCK_SIZE
        self._index(CSO_HEADER_SIZE, self._nblocks)

    def _decode_block(self, bi):
        self._f.seek(self._offs[bi])
        if not self._flags[bi]:
            return self._f.read(BLOCK_SIZE)
        (word,) = struct.unpack("<I", self._f.read(4))
        length = word & 0x7FFFFFFF
        data = self._f.read(length)
        if word & 0x80000000:                 # raw-frame flag inside the prefix
            return data
        return _lz4.decode_block(data, BLOCK_SIZE)


class CciReader(_CompReader):
    def _parse(self):
        head = self._f.read(CCI_HEADER_SIZE)
        magic, hsize, usize, ioff, bsize, ver, align, _ = CCI_HEADER.unpack(head)
        if magic != CCI_MAGIC or hsize != CCI_HEADER_SIZE or bsize != BLOCK_SIZE:
            raise CisoError("not a CCI image")
        self.size = usize
        self._nblocks = usize // BLOCK_SIZE
        self._index(ioff, self._nblocks)

    def _decode_block(self, bi):
        stored = self._offs[bi + 1] - self._offs[bi]
        self._f.seek(self._offs[bi])
        if not self._flags[bi] and stored == BLOCK_SIZE:
            return self._f.read(BLOCK_SIZE)
        rec = self._f.read(stored)
        pad = rec[0]
        payload = rec[1:stored - pad]
        return _lz4.decode_block(payload, BLOCK_SIZE)


def open_image(path):
    magic = open(path, "rb").read(4)
    if magic == CSO_MAGIC:
        return CsoReader(path)
    if magic == CCI_MAGIC:
        return CciReader(path)
    raise CisoError("%s is not a CSO or CCI image" % path)


# ── writers (stream ISO sectors; patch the index afterwards) ───────────
def _iso_blocks(iso_path):
    size = os.path.getsize(iso_path)
    usize = (size + BLOCK_SIZE - 1) // BLOCK_SIZE * BLOCK_SIZE
    return usize, usize // BLOCK_SIZE


def write_cso(iso_path, out_path):
    usize, n = _iso_blocks(iso_path)
    entries = []
    with open(iso_path, "rb") as src, open(out_path, "wb") as out:
        out.write(CSO_HEADER.pack(CSO_MAGIC, CSO_HEADER_SIZE, usize, BLOCK_SIZE, 2, INDEX_ALIGNMENT))
        out.write(b"\x00" * ((n + 1) * 4))            # index placeholder
        for _ in range(n):
            raw = src.read(BLOCK_SIZE)
            if len(raw) < BLOCK_SIZE:
                raw = raw + b"\x00" * (BLOCK_SIZE - len(raw))
            pos = out.tell()
            comp = _lz4.compress_block(raw)
            if len(comp) + 4 < BLOCK_SIZE - 12:        # worth compressing
                out.write(struct.pack("<I", len(comp)) + comp)
                entries.append((pos >> 2) | 0x80000000)
            else:
                out.write(raw)
                entries.append(pos >> 2)
            pad = (-out.tell()) % 4
            if pad:
                out.write(b"\x00" * pad)
        entries.append(out.tell() >> 2)               # terminator
        out.seek(CSO_HEADER_SIZE)
        out.write(struct.pack("<%dI" % (n + 1), *entries))
    return usize


def write_cci(iso_path, out_path):
    usize, n = _iso_blocks(iso_path)
    entries = []
    with open(iso_path, "rb") as src, open(out_path, "wb") as out:
        out.write(CCI_HEADER.pack(CCI_MAGIC, CCI_HEADER_SIZE, usize, 0, BLOCK_SIZE, 1, INDEX_ALIGNMENT, 0))
        for _ in range(n):
            raw = src.read(BLOCK_SIZE)
            if len(raw) < BLOCK_SIZE:
                raw = raw + b"\x00" * (BLOCK_SIZE - len(raw))
            pos = out.tell()                           # already 4-aligned
            comp = _lz4.compress_block(raw)
            if len(comp) < CCI_COMPRESS_THRESHOLD:
                total = _align4(1 + len(comp))
                pad = total - 1 - len(comp)
                out.write(bytes([pad]) + comp + b"\x00" * pad)
                entries.append((pos >> 2) | 0x80000000)
            else:
                out.write(raw)
                entries.append(pos >> 2)
        index_offset = out.tell()
        entries.append(index_offset >> 2)              # terminator
        out.write(struct.pack("<%dI" % (n + 1), *entries))
        out.seek(0)
        out.write(CCI_HEADER.pack(CCI_MAGIC, CCI_HEADER_SIZE, usize, index_offset,
                                  BLOCK_SIZE, 1, INDEX_ALIGNMENT, 0))
    return usize


def to_iso(image_path, out_path):
    r = open_image(image_path)
    with open(out_path, "wb") as out:
        off = 0
        while off < r.size:
            chunk = r.read_at(off, 4 * 1024 * 1024)
            if not chunk:
                break
            out.write(chunk)
            off += len(chunk)
    r.close()
    return off


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
        kind = "CSO" if isinstance(r, CsoReader) else "CCI"
        comp = sum(1 for fl in r._flags[:-1] if fl)
        print("%s image: %s" % (kind, a[1]))
        print("uncompressed: %d bytes (%d blocks); %d compressed, %d raw; on-disk %d bytes" % (
            r.size, r._nblocks, comp, r._nblocks - comp, os.path.getsize(a[1])))
    elif op in ("list", "extract"):
        import gdf
        g = gdf.GDF(r)
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
