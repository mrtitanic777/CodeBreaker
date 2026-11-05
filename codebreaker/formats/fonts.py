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

from codebreaker.formats import native


class Font(object):
    """One font. `widths` is {character code: pixels} for raster fonts. Read by formats.dll
    (native/formats/win/fonts.cc), which also draws glyph() and render()."""

    def __init__(self, data, at):
        self.data, self.at = data, at
        self.__dict__.update(native.call("fonts.font", data, at))

    @property
    def kind(self):
        return "vector" if self.vector else "fixed" if self.width else "proportional"

    def glyph(self, ch):
        """One character as `height` strings of '#' and ' '. A character the font does not have
        is drawn as its default character; a vector font draws nothing."""
        return native.call("fonts.glyph", self.data, self.at, ord(ch) if isinstance(ch, str) else ch)

    def render(self, text):
        return native.call("fonts.render", self.data, self.at, text)

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
