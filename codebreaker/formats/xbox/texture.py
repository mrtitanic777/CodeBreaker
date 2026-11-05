"""Xbox 360 texture untiling + DDS export.

Xbox 360 GPU textures are stored in a tiled (swizzled) memory layout for cache
coherency, and DXT colour/index words are 16-bit byte-swapped versus PC. This
converts a tiled Xbox 360 surface to a linear one and wraps it in a standard
DDS so any PC tool can open it.

The tiled-address math is the well-established Gildor/UModel
`XGAddress2DTiledOffset` formula (also used by Noesis and UEViewer) —
reimplemented here as original Python. decode_dxt() turns a DXT1/DXT3/DXT5 surface (tiled or
linear) into an RGBA image for any program that does not read DDS.

Commands:
    texture untile <in> <w> <h> <dxt1|dxt5|dxt3> <out.dds> [--no-swap]
    texture dds    <linear.bin> <w> <h> <fmt> <out.dds>   (already-linear data)
"""

import os
import sys

from codebreaker.formats import native

# format -> (bytes per 4x4 block, log2 of that, dds fourcc)
FORMATS = {
    "dxt1": (8, 3, b"DXT1"),
    "dxt3": (16, 4, b"DXT3"),
    "dxt5": (16, 4, b"DXT5"),
}


@native.answered("texture.tiled_offset_2d", data_arg=None)
def tiled_offset_2d(x: int, y: int, width_blocks: int, log2bpp: int) -> int:
    """Byte offset of tiled block (x, y) — Gildor/UModel XGAddress2DTiledOffset. x, y and width
    are in blocks; log2bpp = log2(bytes per block), which the result is already scaled by."""


def _blocks(w, h):
    return (w + 3) // 4, (h + 3) // 4


@native.answered("texture.untile")
def untile(data: bytes, w: int, h: int, fmt: str) -> bytes:
    """Tiled Xbox 360 surface -> linear block layout."""


@native.answered("texture.tile")
def tile(data: bytes, w: int, h: int, fmt: str) -> bytes:
    """Linear block layout -> tiled Xbox 360 surface (inverse of untile)."""


@native.answered("texture.swap16")
def swap16(data: bytes) -> bytes:
    """Byte-swap every 16-bit word (Xbox 360 DXT endian fix)."""


@native.answered("texture.dds_header", data_arg=None)
def dds_header(w: int, h: int, fmt: str) -> bytes:
    """Standard 128-byte DDS header for a DXT-compressed surface."""


def to_dds(linear: bytes, w: int, h: int, fmt: str) -> bytes:
    return dds_header(w, h, fmt) + linear


def decode_dxt(data, w, h, fmt, tiled=True, swapped=True, force_opaque=False):
    """A DXT1/DXT3/DXT5 surface as a Pillow RGBA image: un-tiled and its 16-bit words swapped as
    the Xbox 360 keeps them (tiled / swapped False for a linear PC surface). force_opaque ignores
    alpha (an opaque mask stored as DXT5).  The blocks are decoded by formats.dll."""
    from PIL import Image
    width, height, rgba = native.call("texture.decode_rgba", data, w, h, fmt, tiled, swapped, force_opaque)
    return Image.frombytes("RGBA", (width, height), rgba).crop((0, 0, w, h))


DESCRIPTION = "Xbox 360 texture untile, DDS export and DXT decoding (DXT1/3/5)"


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
