#!/usr/bin/env python3
"""The XNA lane's content side: XNB files (formats/xnb.py) with the LZX decoder (formats/lzx.py), and
the DXT decoder they share with the Xbox 360 textures (formats/xbox/texture.py). An XNB texture
built here reads back pixel for pixel in both byte orders; a DXT1 block whose first colour is not
above its second gives transparent black at index 3; and -- when Arcadecraft's unpacked game is on
this machine (../Arcadecraft/Arcadecraft) -- every texture and font of it converts.

    python tests/test_xna.py"""
import glob
import os
import struct
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.formats import xnb                  # noqa: E402
from codebreaker.formats.xbox import texture         # noqa: E402

GAME = os.path.join(os.path.dirname(ROOT), "Arcadecraft", "Arcadecraft")
RESULTS = []


class Skip(Exception):
    pass


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "ok", ""))
    except Skip as s:
        RESULTS.append((fn.__name__, "SKIP", str(s)))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


def _7bit(n):
    out = bytearray()
    while True:
        b = n & 0x7F
        n >>= 7
        out.append(b | (0x80 if n else 0))
        if not n:
            return bytes(out)


def _texture_xnb(platform, pixels, w, h):
    """An uncompressed XNA 4.0 Texture2D: the reader list, no shared objects, the object."""
    reader = b"Microsoft.Xna.Framework.Content.Texture2DReader"
    body = _7bit(1) + _7bit(len(reader)) + reader + struct.pack("<i", 0) + _7bit(0)
    body += _7bit(1) + struct.pack("<iIII", 0, w, h, 1) + struct.pack("<I", len(pixels)) + pixels
    return b"XNB" + platform.encode() + bytes([5, 0]) + struct.pack("<I", 10 + len(body)) + body


def test_texture_both_byte_orders():
    w, h = 2, 1
    want = [(255, 0, 0, 255), (0, 128, 255, 64)]
    pc = b"".join(bytes(p) for p in want)                                   # R, G, B, A
    xbox = b"".join(bytes((p[3], p[2], p[1], p[0])) for p in want)          # A, B, G, R
    with tempfile.TemporaryDirectory() as d:
        for platform, pixels in (("w", pc), ("x", xbox)):
            path = os.path.join(d, "t_%s.xnb" % platform)
            with open(path, "wb") as f:
                f.write(_texture_xnb(platform, pixels, w, h))
            rec = xnb.convert(path, d)
            assert rec.get("kind") == "texture", rec
            from PIL import Image
            img = Image.open(os.path.join(d, "t_%s.png" % platform)).convert("RGBA")
            assert [img.getpixel((x, 0)) for x in range(w)] == want, platform


def test_dxt1_punch_through():
    # c0 = 0x0000 <= c1 = 0xFFFF: three colours and transparent black; every pixel index 3
    block = struct.pack("<HHI", 0x0000, 0xFFFF, 0xFFFFFFFF)
    img = texture.decode_dxt(block, 4, 4, "dxt1", tiled=False, swapped=False)
    assert img.getpixel((0, 0)) == (0, 0, 0, 0)
    opaque = struct.pack("<HHI", 0xFFFF, 0x0000, 0xFFFFFFFF)                 # c0 > c1: index 3 is a colour
    assert texture.decode_dxt(opaque, 4, 4, "dxt1", tiled=False, swapped=False).getpixel((0, 0))[3] == 255


def test_real_game_converts():
    files = sorted(glob.glob(os.path.join(GAME, "*.xnb")))
    if not files:
        raise Skip("no ../Arcadecraft/Arcadecraft here")
    with tempfile.TemporaryDirectory() as d:
        kinds = {}
        for p in files[:120]:
            rec = xnb.convert(p, d)
            assert "error" not in rec, rec
            kinds[rec["kind"]] = kinds.get(rec["kind"], 0) + 1
        assert kinds.get("texture"), kinds


def main():
    for fn in (test_texture_both_byte_orders, test_dxt1_punch_through, test_real_game_converts):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d skipped, %d failed" % (sum(1 for r in RESULTS if r[1] == "ok"),
                                                sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
