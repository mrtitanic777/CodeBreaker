"""compare.py -- a compiled function against the program's bytes at its place.

Every byte must be the program's, except the words a linker fills in; those must come to the
program's word when the place of what they name is put in. Names whose place is known are in the
project's SYMBOLS; a name met for the first time is learnt from the program (the place its word
implies), and the next function that names it is held to that. A word naming the file's own data
must point, in the program's data segment, at what the file's data has there.

The program's code may be in several segments (the project's code_segments: their image offsets);
an offset within one wraps at 64K. Its segment words are those of a load at the project's loadseg.

    bad, notes = compare.function(obj, image, name, offset, length, at, project, symbols, learnt,
                                  text="_TEXT", where=None)
        text   the object's code segment the function is in
        where  where(x): the image address of the segment's offset x (each function of the file at its
               own place); by default the file's code lies together from `at`
        bad    offsets (in the function) of bytes that differ
        notes  what is wrong with the linker's words; a note starting with "?" is a warning only
"""
import struct

W = lambda b, k: struct.unpack_from("<H", b, k)[0]
S = lambda b, k: struct.unpack_from("<h", b, k)[0]


def function(o, img, name, off, ln, at, prj, syms, learn, text="_TEXT", where=None):
    code = o.seg[text]
    skip = set()
    notes = []
    dg = prj.dgroup
    base_of = prj.code_base
    ls = prj.loadseg
    placed = where is not None                              # (each function of the file at a place of its own)
    if where is None:
        where = lambda x: at - off + x
    base = base_of(at)

    def bind(nm, t, a):
        if nm not in syms:
            syms[nm] = (t, a)
            learn[nm] = (t, a)
        elif syms[nm] != (t, a) and not (syms[nm][0] == "?" and syms[nm][1] == a):     # ("?": a place, kind unsaid)
            notes.append("%s is %s %04x in SYMBOLS, here %s %04x" % (nm, syms[nm][0], syms[nm][1], t, a))

    def far(k):                                             # the image's segment:offset at +k, as an image address
        return (W(img, at + k + 2) - ls) * 16 + W(img, at + k)

    for seg, fo, kind, tgt, disp in o.fix:
        if seg != text or not (off <= fo < off + ln):
            continue
        k = fo - off
        skip |= {k, k + 1}
        mine = (W(code, fo) + disp) & 0xFFFF
        theirs = W(img, at + k)
        if kind == "rel":
            dest = base + ((at + k + 2 + theirs - base) & 0xFFFF)
            if tgt[0] == "ext":
                bind(tgt[1], "C", dest - mine)
            elif dest != where(mine):
                notes.append("+%x: calls %05x, this file's code would be at %05x" % (k, dest, where(mine)))
        elif kind == "ptr":                                 # a far call or a far address: offset and segment
            skip |= {k + 2, k + 3}
            dest = far(k)
            if tgt[0] == "ext":
                bind(tgt[1], "C", (dest - mine) & 0xFFFFF)
            elif tgt[1] == text:
                if dest != where(mine):
                    notes.append("+%x: a far place of this file's code %05x, expected %05x" % (k, dest, where(mine)))
            elif tgt[1] in o.seg and tgt[1].endswith("_TEXT"):      # another code segment of the file
                fs = sorted((of, nm) for nm, sg, of in o.pubs if sg == tgt[1] and of <= mine)
                exp = syms[fs[-1][1]][1] + mine - fs[-1][0] if fs and fs[-1][1] in syms else None
                if dest != exp:
                    notes.append("+%x: far place %05x in %s, expected %s" % (k, dest, tgt[1], exp and "%05x" % exp))
            else:
                notes.append("+%x: far fixup to %s" % (k, tgt))
        elif kind == "base":                                # a segment word alone
            if tgt == ("seg", text):
                sb = (theirs - ls) * 16
                if not (sb <= at < sb + 0x10000):
                    notes.append("+%x: segment %04x is not this code's" % (k, theirs))
            elif tgt[0] != "ext":                           # (an external's: checked with the offset beside it)
                if prj.relocs() and at + k not in prj.relocs():     # (another segment of the file's: a word the
                    notes.append("+%x: segment of %s, not a word the loader sets" % (k, tgt))   # loader sets)
                else:
                    bind("%s@seg" % tgt[1], "S", theirs)
        elif kind == "off":
            if tgt[0] == "ext":
                segw = [W(img, at + f[1] - off) for f in o.fix
                        if f[0] == text and f[2] == "base" and f[3] == tgt and off <= f[1] < off + ln]
                if segw:                                    # a function's address in two halves
                    bind(tgt[1], "C", (segw[0] - ls) * 16 + ((theirs - mine) & 0xFFFF))
                elif tgt[1] in syms and syms[tgt[1]][0] == "C":
                    a = syms[tgt[1]][1]
                    if (a - base_of(a)) & 0xFFFF != (theirs - mine) & 0xFFFF:
                        notes.append("+%x: %s is at %05x, here offset %04x" % (k, tgt[1], a, (theirs - mine) & 0xFFFF))
                else:
                    bind(tgt[1], syms[tgt[1]][0] if tgt[1] in syms else "D", (theirs - mine) & 0xFFFF)
            elif tgt[1] == text:                            # a place of this file's code (a switch's table)
                if (where(mine) - base) & 0xFFFF != theirs:
                    notes.append("+%x: the place %04x, this file's code would be at %04x" % (k, theirs, (where(mine) - base) & 0xFFFF))
            else:                                           # the file's own data: what the word points at must be there
                fs = [f for f in o.fix if f[0] == tgt[1] and f[1] == mine and f[2] == "base" and f[3][0] == "ext"]
                if fs:                                      # the segment of a far item, kept among the constants
                    if not prj.relocs():                    # (an image without its relocation table: EXEPACK's)
                        bind(fs[0][3][1] + "@seg", "S", W(img, dg + theirs))
                    elif dg + theirs not in prj.relocs():
                        notes.append("+%x: DS:%04x is not a segment the loader sets" % (k, theirs))
                    else:
                        bind(fs[0][3][1] + "@seg", "S", W(img, dg + theirs))
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
        else:
            notes.append("+%x: fixup %s to %s" % (k, kind, tgt))
    if placed and where(off) == at:
        _calls_within(code, img, off, ln, at, base, where, skip)
    bad = [k for k in range(ln) if k not in skip and (at + k >= len(img) or img[at + k] != code[off + k])]
    return bad, notes


def _calls_within(code, img, off, ln, at, base, where, skip):
    """near calls and jumps from the function to another of its file have no fixup (the compiler wrote them):
    with the file's functions at places of their own, each must reach its callee's place"""
    from capstone import Cs, CS_ARCH_X86, CS_MODE_16
    for i in Cs(CS_ARCH_X86, CS_MODE_16).disasm(bytes(code[off:off + ln]), off):
        if i.bytes[0] in (0xE8, 0xE9) and i.size == 3 and i.address - off + 1 not in skip:
            k = i.address - off + 1
            mine = (i.address + 3 + S(i.bytes, 1)) & 0xFFFF
            if off <= mine < off + ln:
                continue
            if at + k + 2 <= len(img) and base + ((at + k + 2 + S(img, at + k) - base) & 0xFFFF) == where(mine):
                skip |= {k, k + 1}
    # (and a call the disassembly does not see - data after a call, as MS-Pascal's entry helper has it, puts it
    # out of step: an E8 on both sides whose targets are the same place)
    for k in range(1, ln - 1):
        if k in skip or code[off + k - 1] != 0xE8 or at + k + 1 >= len(img) or img[at + k - 1] != 0xE8:
            continue
        mine = (off + k + 2 + S(code, off + k)) & 0xFFFF
        if off <= mine < off + ln:
            continue
        w = where(mine)
        if w >= 0 and base + ((at + k + 2 + S(img, at + k) - base) & 0xFFFF) == w:
            skip |= {k, k + 1}


def identical(bad, notes):
    return not bad and not [n for n in notes if not n.startswith("?")]
