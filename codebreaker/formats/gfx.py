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
import re
import struct
import sys
import zlib


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


def find_movies(data):
    """Yield (offset, magic, body) for each *valid* GFx (uncompressed) / CFX (zlib) movie in data.

    Validates the version byte, declared length, that the tag stream parses and ends in an End
    tag -- so incidental "GFX"/"CFX" byte runs (e.g. in shader source) are not reported as movies.
    """
    n = len(data)
    for m in re.finditer(rb"GFX|CFX", data):
        i = m.start()
        if i + 8 > n:
            continue
        if not 1 <= data[i + 3] <= 20:                  # GFx/SWF version byte is small
            continue
        flen = u32(data, i + 4)
        if not 8 <= flen <= n - i:                      # plausible declared length
            continue
        body = data[i + 8:i + 8 + flen]
        if data[i:i + 3] == b"CFX":
            try:
                body = zlib.decompress(body)
            except Exception:
                continue
        try:
            codes = {c for c, _ in parse_tags(body)}
        except Exception:
            continue
        if 0 not in codes or 1 not in codes:               # real movie: ShowFrame(1) + End(0)
            continue
        yield i, data[i:i + 3].decode("latin1"), body


def wrapper_name(data, gfx_off):
    """Best-effort movie name from the GoodEngine wrapper preceding the GFX magic.
    Returns "" instead of junk when there's no clean identifier (e.g. for FONT.ho)."""
    chunk = data[max(0, gfx_off - 512):gfx_off]
    for s in reversed(re.findall(rb"[\x20-\x7e]{2,}", chunk)):
        t = s.decode("latin1").strip()
        if re.fullmatch(r"[A-Za-z0-9_.\-]{2,48}", t) and re.search(r"[A-Za-z]", t) and len(set(t)) > 1:
            return t
    return ""


def parse_tags(body):
    br = Bits(body, 0)
    nb = br.read(5)
    for _ in range(4):
        br.read(nb)
    br.align()
    return read_tags(body, br.p + 4)          # skip frameRate(u16) + frameCount(u16)


def read_tags(body, p):
    tags = []
    while p + 2 <= len(body):
        tc = u16(body, p); p += 2
        code, length = tc >> 6, tc & 0x3F
        if length == 0x3F:
            length = u32(body, p); p += 4
        tags.append((code, body[p:p + length])); p += length
        if code == 0:
            break
    return tags


def ext_images(body):
    """DefineExternalImage (1001/1008/1009): (charId, w, h, name)."""
    out = []
    for code, d in parse_tags(body):
        if code in (1001, 1008, 1009) and len(d) >= 10:
            cid, w, h = u16(d, 0), u16(d, 4), u16(d, 6)
            nlen = d[8]
            out.append((cid, w, h, d[9:9 + nlen].decode("latin1", "replace")))
    return out


def fonts(body):
    """DefineFont2/3: (version, fontId, name, numGlyphs, bold, hasLayout).
    `version` is the SWF font version (2 or 3), not the raw tag code (48/75)."""
    out = []
    for code, d in parse_tags(body):
        if code in (48, 75) and len(d) >= 6:
            fid, flags, nlen = u16(d, 0), d[2], d[4]
            name = d[5:5 + nlen].split(b"\x00")[0].decode("latin1", "replace")
            ng = u16(d, 5 + nlen)
            out.append((2 if code == 48 else 3, fid, name, ng, bool(flags & 1), bool(flags & 0x80)))
    return out


def parse_matrix(br):
    """SWF MATRIX -> (scaleX, scaleY, translateX_px, translateY_px)."""
    sx = sy = 1.0
    if br.read(1):
        n = br.read(5); sx = br.read(n) / 65536.0; sy = br.read(n) / 65536.0
    if br.read(1):
        n = br.read(5); br.read(n); br.read(n)
    n = br.read(5)
    tx = br.reads(n); ty = br.reads(n)
    return sx, sy, tx / 20.0, ty / 20.0


def placements(body):
    """PlaceObject2(26)/PlaceObject3(70): (depth, charId, matrix)."""
    out = []
    for code, d in parse_tags(body):
        if code not in (26, 70):
            continue
        try:
            flags = d[0]; p = 2 if code == 70 else 1
            depth = u16(d, p); p += 2
            cid = None
            if flags & 0x02:
                cid = u16(d, p); p += 2
            mat = parse_matrix(Bits(d, p)) if flags & 0x04 else None
            out.append((depth, cid, mat))
        except Exception:
            pass
    return out


s16 = lambda d, o: struct.unpack_from("<h", d, o)[0]


def _glyph_contours(data, start):
    """Parse one SWF glyph SHAPE into [[(op, *pts)...]] contours (m/l/q, SWF coords)."""
    br = Bits(data, start)
    nfill, nline = br.read(4), br.read(4)
    x = y = 0
    contours, cur = [], None
    while True:
        if br.read(1) == 0:                               # style-change / end
            f = br.read(5)
            if f == 0:
                break
            if f & 0x01:                                  # moveTo (absolute)
                mb = br.read(5); x = br.reads(mb); y = br.reads(mb)
                if cur:
                    contours.append(cur)
                cur = [("m", x, y)]
            if f & 0x02:
                br.read(nfill)
            if f & 0x04:
                br.read(nfill)
            if f & 0x08:
                br.read(nline)
            if f & 0x10:
                break
        else:                                             # edge
            if br.read(1):                                # straight
                nb = br.read(4) + 2
                if br.read(1):
                    dx, dy = br.reads(nb), br.reads(nb)
                elif br.read(1):
                    dx, dy = 0, br.reads(nb)
                else:
                    dx, dy = br.reads(nb), 0
                x += dx; y += dy; cur.append(("l", x, y))
            else:                                         # quadratic curve
                nb = br.read(4) + 2
                cdx, cdy, adx, ady = br.reads(nb), br.reads(nb), br.reads(nb), br.reads(nb)
                cx, cy = x + cdx, y + cdy; x = cx + adx; y = cy + ady
                cur.append(("q", cx, cy, x, y))
    if cur:
        contours.append(cur)
    return contours


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
