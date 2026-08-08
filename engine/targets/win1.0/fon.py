#!/usr/bin/env python3
"""
fon.py — Parse Windows 1.0 raster fonts (.FON) and render their glyphs.

A .FON is an NE module whose FONT resources each hold a GDI v1 (.FNT, version
0x0100) bitmap font. A v1 font is one shared "strike" bitmap: dfPixHeight
scanlines, each dfWidthBytes bytes (row-major, MSB = leftmost pixel), with every
character laid side by side. The character table is a flat array of WORD pixel
offsets into the strike; a glyph's width is the gap to the next offset. This
reads that and renders glyphs as ASCII art.

Usage:
    python3 fon.py FILE                 # list the fonts inside
    python3 fon.py FILE --show "TEXT"   # render TEXT in the first font
    python3 fon.py FILE --index N --show "TEXT"
    python3 fon.py FILE --index N --sample   # render the printable ASCII set
"""
import struct, sys
from engine.format_loaders.ne import NEFile

def parse_fnt(data, base):
    u16 = lambda o: struct.unpack_from("<H", data, base+o)[0]
    u32 = lambda o: struct.unpack_from("<I", data, base+o)[0]
    b   = lambda o: data[base+o]
    f = dict(
        version   = u16(0x00),
        size      = u32(0x02),
        copyright = data[base+0x06:base+0x06+60].split(b"\0")[0].decode("latin1","replace"),
        ftype     = u16(0x42),
        points    = u16(0x44),
        ascent    = u16(0x4A),
        pixWidth  = u16(0x56),
        pixHeight = u16(0x58),
        avgWidth  = u16(0x5B),
        maxWidth  = u16(0x5D),
        firstChar = b(0x5F),
        lastChar  = b(0x60),
        widthBytes= u16(0x63),
    )
    f["bitsOffset"] = u32(0x71)
    nchars = f["lastChar"] - f["firstChar"] + 1
    # v1 stores one shared "strike" bitmap: dfPixHeight scanlines, each
    # dfWidthBytes bytes (row-major, MSB=leftmost), all characters side by side.
    # Two sub-formats:
    #   * fixed-pitch  (dfPixWidth != 0): no char table; glyph i sits at column
    #     i*dfPixWidth, and the strike begins immediately (dfBitsOffset).
    #   * proportional (dfPixWidth == 0): a flat array of WORD pixel offsets into
    #     the strike (nchars+1 of them) precedes the strike; width = gap to next.
    # The big display/plotter fonts (ROMAN, MODERN) use a third, per-glyph
    # variant (dfWidthBytes == 0, each glyph its own bitmap). We decode the two
    # shared-strike variants that cover the readable UI fonts and flag the rest.
    f["variant"] = ("fixed" if f["pixWidth"]
                    else "proportional" if f["widthBytes"]
                    else "per-glyph (display font, not decoded)")
    if f["widthBytes"] == 0 and f["pixWidth"] == 0:
        f["offsets"] = []
        return f
    if f["pixWidth"]:
        f["offsets"] = [i * f["pixWidth"] for i in range(nchars + 1)]
        f["charTableStart"] = None
    else:
        f["offsets"] = []
        for hdr in (0x75, 0x76, 0x74):
            offs, ok = [], True
            for i in range(nchars + 1):
                o = base + hdr + i*2
                if o+2 > len(data): ok=False; break
                offs.append(struct.unpack_from("<H", data, o)[0])
            if ok and offs and all(offs[i] <= offs[i+1] for i in range(len(offs)-1)) \
               and offs[-1] <= f["widthBytes"]*8:
                f["charTableStart"] = hdr
                f["offsets"] = offs
                break
    return f

def render_glyph(data, base, f, char_index):
    """Extract one glyph from the shared strike as a list of '#'/' ' rows."""
    H  = f["pixHeight"]; wb = f["widthBytes"]; bits = base + f["bitsOffset"]
    o0 = f["offsets"][char_index]
    o1 = f["offsets"][char_index+1]
    rows = []
    for y in range(H):
        rowbase = bits + y*wb
        line = []
        for x in range(o0, o1):
            byte = data[rowbase + (x >> 3)]
            line.append("#" if (byte >> (7-(x & 7))) & 1 else " ")
        rows.append("".join(line))
    return rows

def fonts_in(path):
    ne = NEFile(path)
    out = []
    for r in ne.resources():
        if r["type"] == "FONT":
            f = parse_fnt(ne.data, r["off"])
            f["res_off"] = r["off"]; f["res_id"] = r["id"]
            out.append(f)
    return ne, out

def render_text(ne, f, text):
    base = f["res_off"]; H = f["pixHeight"]
    lines = [""]*H
    for ch in text:
        c = ord(ch)
        if not (f["firstChar"] <= c <= f["lastChar"]):
            c = f["firstChar"]
        g = render_glyph(ne.data, base, f, c - f["firstChar"])
        for y in range(H):
            lines[y] += g[y] + " "
    return lines

def main():
    a = sys.argv[1:]
    path = a[0]
    ne, fonts = fonts_in(path)
    print(f"{path}: {len(fonts)} font(s)")
    for i,f in enumerate(fonts):
        print(f"  [{i}] id={f['res_id']} {f['points']}pt  {f['pixHeight']}px tall  "
              f"chars {f['firstChar']}..{f['lastChar']}  [{f['variant']}]  \"{f['copyright']}\"")
    idx = int(a[a.index("--index")+1]) if "--index" in a else 0
    if idx >= len(fonts): return
    f = fonts[idx]
    if not f.get("offsets"):
        print(f"\n  [{idx}] is a {f['variant']} font — renderer supports the "
              f"proportional and fixed-pitch UI fonts."); return
    if "--show" in a:
        text = a[a.index("--show")+1]
        print(f"\nrendering {text!r} in [{idx}] {f['points']}pt:\n")
        for line in render_text(ne, f, text):
            print("  " + line.replace(" ", "·") if False else "  " + line)
    if "--sample" in a:
        print(f"\nprintable ASCII in [{idx}] {f['points']}pt:\n")
        chunk = "".join(chr(c) for c in range(max(32,f['firstChar']), min(127,f['lastChar']+1)))
        for k in range(0, len(chunk), 16):
            for line in render_text(ne, f, chunk[k:k+16]):
                print("  " + line)
            print()

if __name__ == "__main__":
    main()
