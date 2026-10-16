"""bss.py -- the program's variables that begin without a value, each at its place (src/BSS).

    python -m codebreaker exact PROJECT bss [--write]

They lie from the end of the file's data (the load image ends there) to the stack's segment. In the
sources they were plain definitions the compiler hands the linker as communal names, which the linker
places in the order of its own table (a hash of the names) - ours are not the original names, so each
is given its place outright (exact/link.py makes an object of them). What is there:

  - the program's own: the data places SYMBOLS learnt from the identical functions, in the range; and
    the sources' names for data SYMBOLS has no place for (a variable only functions not yet identical
    use): the engine names data g_XXXX after its place, DS:XXXX
  - the library's: its modules' own _BSS segments (a "hole" the linker fills with them, found by the
    words of their code that point into it), and its communal names (stdio's buffers) by the words of
    the library's code that name them

    src/BSS:  range LO HI / hole ADDR LEN / name ADDR      (hex)
"""
import glob
import os
import re
import struct

from codebreaker.exact import library

W = lambda b, k: struct.unpack_from("<H", b, k)[0]


def propose(prj):
    img = prj.image
    dg = prj.dgroup
    h = prj.header
    edata = len(img) - dg
    stack = h[7] * 16 - dg if h else edata
    mp = library.locate(prj)
    lib_defined = {n for p in mp.coded + mp.donly + mp.first for m in [p.m] + p.also for n in m.pubs}
    linker = set(prj.toolchain().p.get("link", {}).get("names", ["_edata", "_end"]))
    funcs = {"_" + os.path.basename(f)[:-2] for f in glob.glob(os.path.join(prj.src, "f*.c"))}
    skip = lambda n: n in lib_defined or n in linker or n in funcs or ":" in n or "@" in n
    syms = prj.symbols()
    words = set()
    for f in glob.glob(os.path.join(prj.src, "*.[ch]")):
        words |= set(re.findall(r"\w+", open(f, encoding="latin1").read()))
    names = {}
    for n, (t, a) in syms.items():                          # (a name no source uses any more: not the program's)
        if t == "D" and edata <= a < stack and not skip(n) and n.lstrip("_") in words | {w.lstrip("_") for w in words}:
            names[n] = a
    defined, used = set(), set()
    for f in glob.glob(os.path.join(prj.src, "*.c")):
        text = open(f, encoding="latin1").read()
        b = os.path.basename(f)
        if re.fullmatch(r"[de][0-9a-f]{5}\.c", b) or b.endswith("-data.c"):
            defined |= set(re.findall(r"(?m)^(?:static )?(?:unsigned |struct \w+ |union \w+ )?\w+ \**(g_[0-9a-f]{4})\b", text))
        used |= set(re.findall(r"\bg_([0-9a-f]{4})\b", text))
    for x in used:
        n = "_g_" + x
        if "g_" + x not in defined and n not in names and not skip(n) and n not in syms and edata <= int(x, 16) < stack:
            names[n] = int(x, 16)
    # the library's communal names, by the words of its code and data that name them
    for n, (t, a) in mp.allrefs.items():
        if t == "D" and edata <= a < stack and n not in names and not skip(n):
            names[n] = a
    # the library modules' own _BSS: where their code points into it
    holes = []
    for p in mp.coded:
        ln, al = p.m.seglen("_BSS")
        if not ln:
            continue
        s0 = p.m.code[0][0]
        code = p.m.o.seg[s0]
        at = None
        for seg, fo, kind, tgt, disp in p.m.o.fix:
            if seg == s0 and kind == "off" and tgt == ("seg", "_BSS") and fo + 2 <= len(code):
                at = (W(img, p.code + fo) - ((W(code, fo) + disp) & 0xFFFF)) & 0xFFFF
                break
        if at is not None:
            holes.append((at, ln, p.m.label))
    holes.sort()
    merged = []
    for a, ln, nm in holes:
        if merged and a <= merged[-1][0] + merged[-1][1] + 1:
            pa, pl, pn = merged[-1]
            merged[-1] = (pa, max(pl, a + ln - pa), pn + [nm])
        else:
            merged.append((a, ln, [nm]))
    return edata, stack, names, merged


def lines(prj, edata, stack, names, holes):
    out = ["# the variables that begin without a value (codebreaker exact bss): their range; a library module's",
           "# own _BSS between them (hole: its place and length); then a name and its DS offset each (hex)",
           "range %04x %04x" % (edata, stack)]
    for a, ln, nms in holes:
        out.append("hole %04x %x     # the _BSS of the library's %s" % (a, ln, ", ".join(nms)))
    out += ["%s %04x" % (n, a) for n, a in sorted(names.items(), key=lambda kv: (kv[1], kv[0]))]
    return out


def main(prj, argv):
    edata, stack, names, holes = propose(prj)
    txt = "\n".join(lines(prj, edata, stack, names, holes)) + "\n"
    print(txt)
    if "--write" in argv:
        with open(prj.path("BSS"), "w", newline="\n") as f:
            f.write(txt)
    return 0
