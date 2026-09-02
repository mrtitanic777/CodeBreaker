"""Xbox 360 texture untiling + DDS export.

Xbox 360 GPU textures are stored in a tiled (swizzled) memory layout for cache
coherency, and DXT colour/index words are 16-bit byte-swapped versus PC. This
converts a tiled Xbox 360 surface to a linear one and wraps it in a standard
DDS so any PC tool can open it.

The tiled-address math is the well-established Gildor/UModel
`XGAddress2DTiledOffset` formula (also used by Noesis and UEViewer) —
reimplemented here as original Python. Full PC-side texture *decoding* (DXT ->
RGBA) is left to the DDS consumer; this produces a correct DDS container.

Commands:
    texture untile <in> <w> <h> <dxt1|dxt5|dxt3> <out.dds> [--no-swap]
    texture dds    <linear.bin> <w> <h> <fmt> <out.dds>   (already-linear data)
"""

import os
import struct
import sys

# format -> (bytes per 4x4 block, log2 of that, dds fourcc)
FORMATS = {
    "dxt1": (8, 3, b"DXT1"),
    "dxt3": (16, 4, b"DXT3"),
    "dxt5": (16, 4, b"DXT5"),
}


def tiled_offset_2d(x: int, y: int, width_blocks: int, log2bpp: int) -> int:
    """Block offset of tiled block (x, y) — Gildor/UModel XGAddress2DTiledOffset.
    x, y and width are in blocks; log2bpp = log2(bytes-per-block)."""
    aligned_width = (width_blocks + 31) & ~31
    macro = ((x >> 5) + (y >> 5) * (aligned_width >> 5)) << (log2bpp + 7)
    micro = ((x & 7) + ((y & 6) << 2)) << log2bpp
    offset = (macro + ((micro & ~0xF) << 1) + (micro & 0xF) +
              ((y & 8) << (3 + log2bpp)) + ((y & 1) << 4))
    return (((offset & ~0x1FF) << 3) +
            ((y & 16) << 7) +
            ((offset & 0x1C0) << 2) +
            (((((y & 8) >> 2) + (x >> 3)) & 3) << 6) +
            (offset & 0x3F))


def _blocks(w, h):
    return (w + 3) // 4, (h + 3) // 4


def untile(data: bytes, w: int, h: int, fmt: str) -> bytes:
    """Tiled Xbox 360 surface -> linear block layout."""
    block_bytes, log2bpp, _ = FORMATS[fmt]
    bw, bh = _blocks(w, h)
    out = bytearray(bw * bh * block_bytes)
    for y in range(bh):
        for x in range(bw):
            src = tiled_offset_2d(x, y, bw, log2bpp) * block_bytes
            dst = (y * bw + x) * block_bytes
            out[dst:dst + block_bytes] = data[src:src + block_bytes]
    return bytes(out)


def tile(data: bytes, w: int, h: int, fmt: str) -> bytes:
    """Linear block layout -> tiled Xbox 360 surface (inverse of untile)."""
    block_bytes, log2bpp, _ = FORMATS[fmt]
    bw, bh = _blocks(w, h)
    max_off = 0
    coords = []
    for y in range(bh):
        for x in range(bw):
            off = tiled_offset_2d(x, y, bw, log2bpp)
            coords.append((x, y, off))
            max_off = max(max_off, off)
    out = bytearray((max_off + 1) * block_bytes)
    for x, y, off in coords:
        src = (y * bw + x) * block_bytes
        out[off * block_bytes:off * block_bytes + block_bytes] = data[src:src + block_bytes]
    return bytes(out)


def swap16(data: bytes) -> bytes:
    """Byte-swap every 16-bit word (Xbox 360 DXT endian fix)."""
    b = bytearray(data)
    b[0::2], b[1::2] = b[1::2], b[0::2]
    return bytes(b)


def dds_header(w: int, h: int, fmt: str) -> bytes:
    """Standard 128-byte DDS header for a DXT-compressed surface."""
    block_bytes, _, fourcc = FORMATS[fmt]
    bw, bh = _blocks(w, h)
    linear_size = bw * bh * block_bytes
    DDSD = 0x1 | 0x2 | 0x4 | 0x1000 | 0x80000            # caps|h|w|pixelformat|linearsize
    # magic, size, flags, height, width, pitch, depth, mipMapCount
    h1 = struct.pack("<4sIIIIIII", b"DDS ", 124, DDSD, h, w, linear_size, 0, 0)
    h1 += struct.pack("<11I", *([0] * 11))               # dwReserved1[11]
    # DDS_PIXELFORMAT: size, flags(0x4 fourcc), fourcc, then 5 zeros
    pf = struct.pack("<II4sIIIII", 32, 0x4, fourcc, 0, 0, 0, 0, 0)
    caps = struct.pack("<IIIII", 0x1000, 0, 0, 0, 0)      # DDSCAPS_TEXTURE
    return h1 + pf + caps


def to_dds(linear: bytes, w: int, h: int, fmt: str) -> bytes:
    return dds_header(w, h, fmt) + linear


DESCRIPTION = "Xbox 360 texture untile + DDS export (DXT1/3/5)"


# ------------------------------------------------------------------------ CLI
def main():
    a = sys.argv[1:]
    swap = True
    if "--no-swap" in a:
        swap = False
        a.remove("--no-swap")
    if not a or a[0] in ("-h", "--help") or a[0] not in ("untile", "dds"):
        print("usage: texture untile <in> <w> <h> <dxt1|dxt3|dxt5> <out.dds> [--no-swap]\n"
              "       texture dds    <linear.bin> <w> <h> <fmt> <out.dds>")
        return
    op, inp, w, h, fmt, out = a[0], a[1], int(a[2]), int(a[3]), a[4].lower(), a[5]
    if fmt not in FORMATS:
        print("unknown format %r (dxt1/dxt3/dxt5)" % fmt); sys.exit(2)
    data = open(inp, "rb").read()
    linear = untile(data, w, h, fmt) if op == "untile" else data
    if swap:
        linear = swap16(linear)
    open(out, "wb").write(to_dds(linear, w, h, fmt))
    print("%s %dx%d %s -> %s (%d bytes)" % (op, w, h, fmt, out, os.path.getsize(out)))


if __name__ == "__main__":
    main()
