"""XPR resource-bundle reader — pull textures out of Xbox `.xpr` packages.

Two container generations, both handled here:

  * XPR0 — original Xbox.  Little-endian.  Header is magic 'XPR0', total size,
    header size (where pixel data begins), followed by an Xbox D3DTexture
    resource whose packed Format dword encodes the nv2a texture format and the
    log2 width/height.  DXT surfaces are stored *linearly* (unswizzled), so we
    can slice them straight out and emit a DDS.

  * XPR2 — Xbox 360.  Big-endian.  Magic 'XPR2', header size, data size, then a
    resource directory.  The pixel data begins at header size and the 360 tiled
    DXT layout / 16-bit endian is handled by our texture module.  We parse the
    header + data region; per-resource dimensions come from the (optional)
    resource table when present, else must be supplied.

Format facts only — no third-party code.  Verified by a synthetic XPR0
round-trip in the selftest (build a bundle around a known DXT1 surface, read it
back, confirm format/size/pixels and a valid DDS).
"""

import struct
import sys

from codebreaker.formats import native

DESCRIPTION = "extract textures from Xbox .xpr resource bundles (XPR0 original / XPR2 360) -> DDS"

class XprError(Exception):
    pass


native.RAISES["XprError"] = XprError


def build_xpr0(pixels, width, height, xfmt=0x0C, mipmaps=1):
    """Assemble a minimal XPR0 bundle around a linear texture (for tests/tools)."""
    import math
    log2w, log2h = int(math.log2(width)), int(math.log2(height))
    fmtdw = ((xfmt & 0xFF) << 8) | (2 << 4) | ((mipmaps & 0xF) << 16) \
        | ((log2w & 0xF) << 20) | ((log2h & 0xF) << 24)
    header_size = 0x1C
    hdr = b"XPR0" + struct.pack("<II", header_size + len(pixels), header_size)
    hdr += struct.pack("<IIII", 0, header_size, 0, fmtdw)     # D3DTexture resource
    hdr += b"\x00" * (header_size - len(hdr))
    return hdr + pixels


@native.answered("xpr.parse")
def parse(data):
    """An XPR0 / XPR2 bundle -> {container, ... pixels}: XPR0 {xfmt, format_name, dds_fmt, width, height, mipmaps,
    cubemap, data_offset, pixels, total}, XPR2 {header_size, data_size, data_offset, pixels} (the 360 pixel region
    is tiled DXT: hand it to texture.untile + texture.to_dds with the surface's real dimensions).  XprError when it
    is neither (formats.dll's, native/formats/xbox/graphics.cc)."""


def to_dds(info, width=None, height=None):
    """Turn a parsed XPR into DDS bytes. XPR0 DXT is linear -> direct. XPR2 is
    360-tiled -> untile + endian-fix via the texture module (needs w/h)."""
    from codebreaker.formats.xbox import texture as tex
    if info["container"] == "XPR0":
        if not info["dds_fmt"]:
            raise XprError("XPR0 format %s is not a DXT surface (no DDS path yet)"
                           % info["format_name"])
        return tex.to_dds(info["pixels"], info["width"], info["height"], info["dds_fmt"])
    # XPR2: caller supplies dimensions + assumed dxt5 unless told otherwise
    w = width or info.get("width")
    h = height or info.get("height")
    if not (w and h):
        raise XprError("XPR2 needs --width/--height to untile (no resource dims)")
    linear = tex.untile(tex.swap16(info["pixels"]), w, h, "dxt5")
    return tex.to_dds(linear, w, h, "dxt5")


# ── self-verification ──────────────────────────────────────────────────────
def _selfcheck():
    # a 64x32 DXT1 surface = 64*32/2 = 1024 bytes; fill with a recognisable ramp
    w, h = 64, 32
    pixels = bytes((i * 7) & 0xFF for i in range(w * h // 2))
    blob = build_xpr0(pixels, w, h, xfmt=0x0C)
    info = parse(blob)
    if info["container"] != "XPR0" or info["dds_fmt"] != "dxt1":
        return False, "format decode wrong: %r" % info
    if info["width"] != w or info["height"] != h:
        return False, "dims wrong: %dx%d" % (info["width"], info["height"])
    if info["pixels"] != pixels:
        return False, "pixel round-trip mismatch"
    dds = to_dds(info)
    if dds[:4] != b"DDS " or dds[-len(pixels):] != pixels:
        return False, "DDS wrap wrong"
    # XPR2 header round-trip (magic + 2 sizes + 8 pad = 0x14 before pixels)
    x2 = b"XPR2" + struct.pack(">II", 0x14, len(pixels)) + b"\x00" * 8 + pixels
    i2 = parse(x2)
    if i2["data_offset"] != 0x14 or i2["pixels"] != pixels:
        return False, "XPR2 header parse wrong"
    return True, "XPR0 build/parse/DDS round-trip (64x32 DXT1) + XPR2 header"


def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage:\n"
              "  xpr info <file.xpr>              show container + texture info\n"
              "  xpr dds  <file.xpr> <out.dds> [W H]   extract texture to DDS\n"
              "  xpr --selftest")
        return
    if a[0] == "--selftest":
        ok, msg = _selfcheck()
        print(("PASS: " if ok else "FAIL: ") + msg)
        sys.exit(0 if ok else 1)
    op = a[0]
    data = open(a[1], "rb").read()
    info = parse(data)
    if op == "info":
        for k in ("container", "format_name", "dds_fmt", "width", "height",
                  "mipmaps", "cubemap", "data_offset", "header_size", "data_size"):
            if k in info:
                print("  %-12s %s" % (k, info[k]))
        return
    if op == "dds":
        out = a[2]
        w = int(a[3]) if len(a) > 3 else None
        h = int(a[4]) if len(a) > 4 else None
        dds = to_dds(info, w, h)
        open(out, "wb").write(dds)
        print("wrote %s (%d bytes, %s %sx%s)" %
              (out, len(dds), info.get("format_name", "?"),
               w or info.get("width"), h or info.get("height")))
        return
    print("unknown op: %s" % op)
    sys.exit(2)


if __name__ == "__main__":
    main()
