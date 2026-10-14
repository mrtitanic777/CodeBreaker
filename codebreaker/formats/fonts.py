"""fonts.py -- raster fonts: the .FNT format, and the .FON files that hold several of them.

    from codebreaker.formats import fonts
    fonts.fonts_in(path)        [Font] -- every font in a .FON, or the one in a bare .FNT
    fonts.parse(data, at)       Font   -- one .FNT image inside `data`
    font.glyph("A")             rows of '#' and ' ' for one character (raster fonts)
    font.render("Hello")        the same for a line of text

    python -m codebreaker fonts FILE [--index N] [--show TEXT]

A .FON is a 16-bit module with no code whose FONT resources are each a .FNT image. The .FNT
header is the same in every version up to the character table; what follows differs:

    1.x  (version 0x0100)  one bitmap for the whole font ("strike"): `height` rows of
                           `row_bytes` bytes, the characters side by side. Fixed-pitch fonts
                           have no table (character i starts at column i * width); proportional
                           ones have a table of column offsets, one word per character plus one.
    2.x  (version 0x0200)  a table of (width, offset:16) per character at 0x76; each glyph is its
                           own bitmap, stored a byte-column at a time (`height` bytes per column).
    3.x  (version 0x0300)  the same with (width, offset:32) at 0x94.

Fonts whose type has bit 0 set are stroke ("vector") fonts -- ROMAN, MODERN, SCRIPT: they are
listed with their metrics but have no bitmaps to draw.
"""
import struct


class Font(object):
    """One font. `widths` is {character code: pixels} for raster fonts."""

    def __init__(self, data, at):
        def u16(o):
            return struct.unpack_from("<H", data, at + o)[0]

        def u32(o):
            return struct.unpack_from("<I", data, at + o)[0]
        self.data, self.at = data, at
        self.version, self.size = u16(0), u32(2)
        self.copyright = data[at + 6:at + 66].split(b"\0")[0].decode("latin1", "replace")
        self.vector = bool(u16(0x42) & 1)
        self.points, self.ascent = u16(0x44), u16(0x4A)
        self.italic, self.weight, self.charset = bool(data[at + 0x50]), u16(0x53), data[at + 0x55]
        self.width, self.height = u16(0x56), u16(0x58)          # width 0: proportional
        self.average_width, self.max_width = u16(0x5B), u16(0x5D)
        self.first, self.last, self.default = data[at + 0x5F], data[at + 0x60], data[at + 0x61]
        self.row_bytes = u16(0x63)
        face = u32(0x69)
        end = data.find(b"\0", at + face, at + face + 64) if face and at + face < len(data) else -1
        self.name = data[at + face:end].decode("latin1", "replace") if end > 0 else ""
        self.bits = u32(0x71)
        n = self.last - self.first + 1
        self._where = {}                                         # code -> (width, where its bits are)
        if self.vector or self.last < self.first:
            pass
        elif self.version < 0x0200:
            self._strike(n)
        else:
            table, step = (0x76, 4) if self.version < 0x0300 else (0x94, 6)
            for i in range(n):
                p = at + table + step * i
                if p + step > len(data):
                    break
                w = struct.unpack_from("<H", data, p)[0]
                off = struct.unpack_from("<H" if step == 4 else "<I", data, p + 2)[0]
                self._where[self.first + i] = (w, off)
        self.widths = {c: w for c, (w, _o) in self._where.items()}

    def _strike(self, n):
        if self.width:
            offs = [i * self.width for i in range(n + 1)]
        else:
            offs = []
            for table in (0x75, 0x76, 0x74):                    # the header's length varied in 1.x
                if self.at + table + 2 * (n + 1) > len(self.data):
                    continue
                got = struct.unpack_from("<%dH" % (n + 1), self.data, self.at + table)
                if all(a <= b for a, b in zip(got, got[1:])) and got[-1] <= self.row_bytes * 8:
                    offs = list(got)
                    break
        for i in range(len(offs) - 1):
            self._where[self.first + i] = (offs[i + 1] - offs[i], offs[i])

    @property
    def kind(self):
        return "vector" if self.vector else "fixed" if self.width else "proportional"

    def glyph(self, ch):
        """One character as `height` strings of '#' and ' '. A character the font does not have
        is drawn as its default character; a vector font draws nothing."""
        code = ord(ch) if isinstance(ch, str) else ch
        if code not in self._where:
            code = self.first + self.default
        if code not in self._where:
            return [""] * self.height
        w, off = self._where[code]
        d, rows = self.data, []
        for y in range(self.height):
            line = []
            for x in range(w):
                if self.version < 0x0200:                        # shared strike, a row at a time
                    p = self.at + self.bits + y * self.row_bytes + ((off + x) >> 3)
                    bit = (off + x) & 7
                else:                                            # own bitmap, a byte-column at a time
                    p = self.at + off + (x >> 3) * self.height + y
                    bit = x & 7
                line.append("#" if p < len(d) and (d[p] >> (7 - bit)) & 1 else " ")
            rows.append("".join(line))
        return rows

    def render(self, text):
        rows = [""] * self.height
        for ch in text:
            for y, line in enumerate(self.glyph(ch)):
                rows[y] += line + " "
        return rows

    def describe(self):
        return "%-14s %2dpt %2dpx  %-12s chars %d..%d  version %x.%02x" % (
            self.name or "?", self.points, self.height, self.kind, self.first, self.last,
            self.version >> 8, self.version & 0xFF)


def parse(data, at=0):
    """The font whose .FNT image starts at `at`."""
    if at + 0x76 > len(data) or struct.unpack_from("<H", data, at)[0] not in (0x0100, 0x0200, 0x0300):
        raise ValueError("no font at %#x" % at)
    return Font(data, at)


def fonts_in(path):
    """[Font] -- the FONT resources of a .FON (or of any 16-bit module), or a bare .FNT file's one
    font. A file that holds none gives an empty list."""
    from codebreaker.formats import detect
    from codebreaker.formats.ne import NEFile
    if detect.format_of(path) == "NE":
        nef = NEFile(path)
        out = []
        for r in nef.resources():
            if r["type"] == "FONT":
                try:
                    out.append(parse(nef.data, r["off"]))
                except (ValueError, struct.error):
                    pass
        return out
    with open(path, "rb") as f:
        data = f.read()
    try:
        return [parse(data)]
    except (ValueError, struct.error):
        return []


def main(argv):
    """python -m codebreaker fonts FILE [--index N] [--show TEXT]

    List the fonts in a .FON or .FNT file (a module name of the selected corpus will do), and
    draw TEXT in one of them."""
    import os
    if not argv:
        print(main.__doc__)
        return 2
    from codebreaker.base import corpus
    path = argv[0] if os.path.isfile(argv[0]) else corpus.retail(argv[0])
    if not path:
        raise SystemExit("no file %r" % argv[0])
    found = fonts_in(path)
    print("%s: %d font(s)" % (os.path.basename(path), len(found)))
    for i, f in enumerate(found):
        print("  [%d] %s" % (i, f.describe()))
    if "--show" in argv and found:
        f = found[int(argv[argv.index("--index") + 1]) if "--index" in argv else 0]
        if f.vector:
            print("  a vector font has no bitmaps to draw")
            return 0
        for line in f.render(argv[argv.index("--show") + 1]):
            print("  " + line)
    return 0
