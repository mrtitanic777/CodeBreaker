"""gfx.py -- Scaleform GFx user-interface movies, and their fonts as TrueType.

    python -m codebreaker gfx scan|images|fonts|placements <file>
    python -m codebreaker gfx font-ttf <file> <font id> <out.ttf> [family]
    from codebreaker.formats import gfx
    for offset, magic, body in gfx.find_movies(data): gfx.ext_images(body), gfx.fonts(body), gfx.placements(body)
    gfx.font_to_ttf(data, font_id, "font.ttf")

GFx is Scaleform's SWF-derived format ("GFX" uncompressed, "CFX" zlib, then an SWF tag stream),
which many console games draw their menus with. A file may hold several movies, each wrapped by
its engine (Heavy Iron's GoodEngine puts an engine header, the movie's name and asset ids before
the GFX body), so movies are found by their magic and kept only where the tag stream parses and
ends in an End tag -- an incidental "GFX" in shader source is not a movie. Read: external images
(DefineExternalImage 1001/1008/1009), embedded fonts (DefineFont2/3), placements (PlaceObject2/3).

A DefineFont3 font is vector outlines (SWF shape records: moves, lines, quadratic curves on a
20480-unit em), a code table and layout; font_to_ttf rebuilds it as a TrueType file (fontTools).
"""
import struct
import sys

from codebreaker.formats import native


class Bits:
    """Reads bits most-significant-first from a bytes buffer, SWF convention."""

    __slots__ = ("d", "p", "bit")

    def __init__(self, d, p=0):
        self.d = d
        self.p = p
        self.bit = 0

    def read(self, n):
        """Read n bits as an unsigned integer."""
        v = 0
        for _ in range(n):
            v = (v << 1) | ((self.d[self.p] >> (7 - self.bit)) & 1)
            self.bit += 1
            if self.bit == 8:
                self.bit = 0
                self.p += 1
        return v

    def reads(self, n):
        """Read n bits as a signed (two's-complement) integer."""
        v = self.read(n)
        return v - (1 << n) if n and (v >> (n - 1)) else v

    def align(self):
        """Advance to the next whole byte boundary."""
        if self.bit:
            self.bit = 0
            self.p += 1


u16 = lambda d, o: struct.unpack_from("<H", d, o)[0]
u32 = lambda d, o: struct.unpack_from("<I", d, o)[0]


@native.answered("gfx.find_movies")
def _find_movies(data):
    """[(offset, magic, body)] of each *valid* GFx (uncompressed) / CFX (zlib) movie in data: the version byte, the
    declared length, and a tag stream that parses and ends in an End tag (so incidental "GFX"/"CFX" byte runs, e.g. in
    shader source, are not movies)."""


def find_movies(data):
    """Yield (offset, magic, body) for each *valid* GFx (uncompressed) / CFX (zlib) movie in data."""
    yield from _find_movies(data)


@native.answered("gfx.wrapper_name")
def wrapper_name(data, gfx_off):
    """Best-effort movie name from the GoodEngine wrapper preceding the GFX magic.
    Returns "" instead of junk when there's no clean identifier (e.g. for FONT.ho)."""


@native.answered("gfx.parse_tags", data_arg=0)
def parse_tags(body):
    """The SWF tag stream of a movie's body: [(code, bytes)]."""


@native.answered("gfx.ext_images")
def ext_images(body):
    """DefineExternalImage (1001/1008/1009): (charId, w, h, name)."""


@native.answered("gfx.fonts")
def fonts(body):
    """DefineFont2/3: (version, fontId, name, numGlyphs, bold, hasLayout).
    `version` is the SWF font version (2 or 3), not the raw tag code (48/75)."""


@native.answered("gfx.placements")
def placements(body):
    """PlaceObject2(26)/PlaceObject3(70): (depth, charId, matrix); matrix is (scaleX, scaleY, translateX_px, translateY_px)."""


s16 = lambda d, o: struct.unpack_from("<h", d, o)[0]


@native.answered("gfx.glyph_contours")
def _glyph_contours(data, start):
    """Parse one SWF glyph SHAPE into [[(op, *pts)...]] contours (m/l/q, SWF coords)."""


def _find_font_tag(data, font_id):
    for off, mg, body in find_movies(data):
        for code, d in parse_tags(body):
            if code == 75 and u16(d, 0) == font_id:
                return d
    return None


def font_to_ttf(data, font_id, out_path, family=None):
    """Parse DefineFont3 `font_id` from `data` and write a TTF to out_path. Returns (name, glyphs)."""
    from fontTools.fontBuilder import FontBuilder
    from fontTools.pens.ttGlyphPen import TTGlyphPen

    fontdata = _find_font_tag(data, font_id)
    if not fontdata:
        raise KeyError("DefineFont3 id=%d not found" % font_id)
    flags = fontdata[2]; nlen = fontdata[4]
    name = fontdata[5:5 + nlen].split(b"\x00")[0].decode("latin1")
    p = 5 + nlen
    n = u16(fontdata, p); p += 2
    ot = p
    wide = flags & 0x08
    if wide:
        offs = [struct.unpack_from("<I", fontdata, ot + 4 * k)[0] for k in range(n + 1)]
    else:
        offs = [u16(fontdata, ot + 2 * k) for k in range(n + 1)]
    shapes = [_glyph_contours(fontdata, ot + offs[g]) for g in range(n)]
    ctp = ot + offs[n]
    codes = [u16(fontdata, ctp + 2 * k) for k in range(n)]
    lp = ctp + 2 * n
    ascent, descent = s16(fontdata, lp), s16(fontdata, lp + 2); lp += 6
    advances = [s16(fontdata, lp + 2 * k) for k in range(n)]

    EM, UPM = 20480, 2048
    sc = lambda v: round(v * UPM / EM)
    fb = FontBuilder(UPM, isTTF=True)
    fb.setupGlyphOrder([".notdef"] + ["g%d" % g for g in range(n)])
    glyfs = {".notdef": TTGlyphPen(None).glyph()}
    # (an advance below zero, which a font's layout table can hold, is zero in TrueType's)
    metrics = {".notdef": (max(0, sc(advances[0])) if advances else 600, 0)}
    cmap = {}
    for g in range(n):
        pen = TTGlyphPen(None); open_c = False
        for c in shapes[g]:
            for op in c:
                if op[0] == "m":
                    if open_c:
                        pen.closePath()
                    pen.moveTo((sc(op[1]), -sc(op[2]))); open_c = True
                elif op[0] == "l":
                    pen.lineTo((sc(op[1]), -sc(op[2])))
                else:
                    pen.qCurveTo((sc(op[1]), -sc(op[2])), (sc(op[3]), -sc(op[4])))
            if open_c:
                pen.closePath(); open_c = False
        glyfs["g%d" % g] = pen.glyph()
        metrics["g%d" % g] = (max(0, sc(advances[g])), 0)
        if codes[g] and codes[g] not in cmap:
            cmap[codes[g]] = "g%d" % g
    fb.setupGlyf(glyfs)
    fb.setupCharacterMap(cmap)
    fb.setupHorizontalMetrics(metrics)
    asc, dsc = sc(ascent), -abs(sc(descent))
    fb.setupHorizontalHeader(ascent=asc, descent=dsc)
    fam = family or name.replace("-", "")
    fb.setupNameTable({"familyName": fam, "styleName": "Regular", "psName": fam})
    fb.setupOS2(sTypoAscender=asc, sTypoDescender=dsc, usWinAscent=asc, usWinDescent=-dsc)
    fb.setupPost()
    fb.save(out_path)
    return name, n


_USAGE = "\n\n".join(__doc__.split("\n\n")[:2])


def main(argv):
    if len(argv) < 2:
        print(_USAGE)
        return 2
    command, path = argv[0], argv[1]
    with open(path, "rb") as f:
        data = f.read()
    if command == "font-ttf" and len(argv) > 3:
        name, n = font_to_ttf(data, int(argv[2]), argv[3], family=argv[4] if len(argv) > 4 else None)
        print("%r: %d glyphs -> %s" % (name, n, argv[3]))
        return 0
    movies = list(find_movies(data))
    if not movies:
        print("no GFx movies in %s" % path)
    for off, magic, body in movies:
        name = wrapper_name(data, off)
        if command == "scan":
            print("%s at $%X  %-28s %d images, %d fonts" % (magic, off, name, len(ext_images(body)), len(fonts(body))))
        elif command == "images":
            for cid, w, h, image in ext_images(body):
                print("%-28s character %-4d %4dx%-4d %s" % (name, cid, w, h, image))
        elif command == "fonts":
            for version, fid, font, glyphs, bold, layout in fonts(body):
                print("%-24s DefineFont%d id %d %-20r %d glyphs%s%s" % (name, version, fid, font, glyphs,
                                                                    " bold" if bold else "", " layout" if layout else ""))
        elif command == "placements":
            images = {cid: image for cid, w, h, image in ext_images(body)}
            for depth, cid, matrix in placements(body):
                if cid in images and matrix:
                    sx, sy, tx, ty = matrix
                    print("%-24s depth %-3d %-22s at (%.1f, %.1f) scale (%.3f, %.3f)" % (
                        name, depth, images[cid], tx, ty, sx, sy))
        else:
            print(_USAGE)
            return 2
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
