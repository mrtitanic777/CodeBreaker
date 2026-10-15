"""compare.py -- a compiled function against the program's bytes at its place.

Every byte must be the program's, except the words a linker fills in; those must come to the
program's word when the place of what they name is put in. Names whose place is known are in the
project's SYMBOLS; a name met for the first time is learnt from the program (the place its word
implies), and the next function that names it is held to that. A word naming the file's own data
must point, in the program's data segment, at what the file's data has there.

    bad, notes = compare.function(obj, image, name, offset, length, at, project, symbols, learnt)
        bad    offsets (in the function) of bytes that differ
        notes  what is wrong with the linker's words; a note starting with "?" is a warning only
"""
import struct

W = lambda b, k: struct.unpack_from("<H", b, k)[0]


def function(o, img, name, off, ln, at, prj, syms, learn):
    text = o.seg["_TEXT"]
    skip = set()
    notes = []
    dg = prj.dgroup
    mask = prj.code_mask

    def bind(nm, t, a):
        if nm not in syms:
            syms[nm] = (t, a)
            learn[nm] = (t, a)
        elif syms[nm] != (t, a):
            notes.append("%s is %s %04x in SYMBOLS, here %s %04x" % (nm, syms[nm][0], syms[nm][1], t, a))

    for seg, fo, kind, tgt, disp in o.fix:
        if seg != "_TEXT" or not (off <= fo < off + ln):
            continue
        k = fo - off
        skip |= {k, k + 1}
        mine = (W(text, fo) + disp) & 0xFFFF
        theirs = W(img, at + k)
        if kind == "rel":
            dest = (at + k + 2 + theirs) & mask          # (near code past 64K in one segment: the low word)
            if tgt[0] == "ext":
                bind(tgt[1], "C", (dest - mine) & mask)
            else:
                want = (at - off + mine) & mask
                if dest != want:
                    notes.append("+%x: calls %04x, this file's code would be at %04x" % (k, dest, want))
        elif kind == "off":
            if tgt[0] == "ext":
                bind(tgt[1], syms[tgt[1]][0] if tgt[1] in syms else "D", (theirs - mine) & 0xFFFF)
            elif tgt[1] == "_TEXT":
                if theirs != (at - off + mine) & 0xFFFF:
                    notes.append("+%x: a code address %04x, expected %04x" % (k, theirs, (at - off + mine) & 0xFFFF))
            else:
                far = [f for f in o.fix if f[0] == tgt[1] and f[1] == mine and f[2] == "base" and f[3][0] == "ext"]
                if far:                                     # the segment of a far item, kept among the constants
                    if dg + theirs not in prj.relocs():
                        notes.append("+%x: DS:%04x is not a segment the loader sets" % (k, theirs))
                    else:
                        bind(far[0][3][1] + "@seg", "S", W(img, dg + theirs))
                    continue
                d = o.seg.get(tgt[1], b"")
                e = mine
                while e < len(d) and d[e] != 0:
                    e += 1
                s = d[mine:e + 1] if tgt[1] in ("CONST", "_DATA") else b""
                holes = {f[1] + x for f in o.fix if f[0] == tgt[1] for x in ((0, 1, 2, 3) if f[2] == "ptr" else (0, 1))}
                if s and (dg + theirs + len(s) > len(img) or any(img[dg + theirs + x] != s[x] for x in range(len(s)) if mine + x not in holes)):
                    notes.append("+%x: %s+%x (%r) is not at DS:%04x (%r)" % (k, tgt[1], mine, s[:24], theirs, bytes(img[dg + theirs:dg + theirs + min(len(s), 24)])))
                elif len(s) > 2 and (mine == 0 or d[mine - 1] == 0) and theirs and 0x20 <= img[dg + theirs - 1] < 0x7F:
                    notes.append("? +%x: %s+%x (%r) is the end of another string at DS:%04x - a number, not a string?" % (k, tgt[1], mine, s[:24], theirs))
                bind("%s:%s+%x" % (name, tgt[1], mine), "D", theirs)
        elif kind == "ptr" and tgt[0] == "ext":           # a far call: segment and offset in the image
            skip |= {k + 2, k + 3}
            bind(tgt[1], "C", (W(img, at + k + 2) * 16 + theirs - mine) & 0xFFFFF)
        else:
            notes.append("+%x: fixup %s to %s" % (k, kind, tgt))
    bad = [k for k in range(ln) if k not in skip and (at + k >= len(img) or img[at + k] != text[off + k])]
    return bad, notes


def identical(bad, notes):
    return not bad and not [n for n in notes if not n.startswith("?")]
