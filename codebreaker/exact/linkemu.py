"""linkemu.py -- a linker's work done here, as it did it, for a linker that cannot be run (or is the program itself).

    link24(objects, symbols, log=None) -> the run file's bytes

link24: Microsoft's LINK 2.4x as XENIX C built it (runfile.c), for a small-model program:

  the segments   in the order the modules first name them (crt0's: _TEXT, then the group DGROUP - CRT,
                 _DATA, _BSS, c_common, STACK); each module's piece of a segment begins where its alignment
                 says, the group on a paragraph. Names are the linker's without regard to case.
  the communal   variables (an external with a size; the greatest asked for): c_common. The linker placed
  variables      them in the order of its table of names, a hash of the names - and most of the names are
                 not known. So their order is taken from SYMBOLS, the record of where each was found (each
                 must begin where the one before ends).
  the run file   a header of 512 bytes with the relocations (the paragraph of DGROUP, each a place in its
                 segment: the code's or DGROUP's), the segments as far as there is data, the checksum that
                 makes the file's words add up to FFFF.

objects: [(name, object bytes, odd)] - odd: the code ends on an odd byte (the compiler's filler is not there:
the module after it is byte aligned). Either an OMF module or a Version 7 a.out (exact/aout.py reads both alike).
"""
import struct

W = lambda b, k: struct.unpack_from("<H", b, k)[0]
ALIGN = {0: 1, 1: 1, 2: 2, 3: 16, 4: 256, 5: 4}


class Piece(object):
    def __init__(self, seg, align, data, length, hasdata):
        self.seg, self.align, self.data, self.length, self.hasdata, self.at = seg, align, bytearray(data), length, hasdata, None
        self.stack = False


def load(raw, odd=False, real=None):
    """(pieces {segment: Piece}, defined {name: (segment, offset)}, common {name: length}, fixes, start)
    a fix: (segment, offset, how, target, addend) - how 'abs' (the address), 'rel' (from the end of the word),
    'seg' (a paragraph); target ('sym', name) or ('seg', segment) or ('grp',)"""
    from codebreaker.exact import aout, omf
    o = omf.read(raw)
    even = aout.is_aout(raw)                                # (a C object's pieces each begin at an even address)
    name = lambda s: real.setdefault(s.upper(), s) if real is not None else s
    pieces = {}
    for s, cls, ln, al in o.segs:
        al = 2 if even else al
        d = bytes(o.seg.get(s, b""))[:o.filled.get(s, 0)]
        if odd and cls.upper() == "CODE" and d:
            d = d[:-1]                                      # (the compiler's filler, which is not there)
            ln = len(d)
        p = Piece(name(s), ALIGN.get(al, 1), d, ln, bool(o.filled.get(s)))
        p.stack = getattr(o, "combine", {}).get(s) == 5 or cls.upper() == "STACK"
        pieces[name(s)] = p
    defined = {n.lower(): (name(s), off) for n, s, off in o.pubs if s}
    common = {n.lower(): v[0] for n, v in o.comm.items()}
    fixes = []
    for s, off, kind, tgt, disp in o.fix:
        add = (W(o.seg[s], off) + disp) & 0xFFFF
        tg = ("sym", tgt[1].lower()) if tgt[0] == "ext" else ("seg", name(tgt[1])) if tgt[0] == "seg" else ("grp",)
        if kind == "base":
            fixes.append((name(s), off, "seg", tg, add))
        elif kind == "rel":
            fixes.append((name(s), off, "rel", tg, (add - off - 2) & 0xFFFF))
        else:
            fixes.append((name(s), off, "abs", tg, add))
    start = (name(o.start[0]), o.start[1]) if getattr(o, "start", None) else None
    return pieces, defined, common, fixes, start


def link24(objects, symbols, log=None):
    say = log or (lambda *a: None)
    from codebreaker.exact import omf
    real = {}
    mods = [(n,) + load(raw, odd, real) for n, raw, odd in objects]
    order = []                                              # (the segments: a group's in the order the group lists
    for n, raw, odd in objects:                             #  them - an assembler writes its SEGDEFs in the order of
        for g, mem in omf.read(raw).groups.items():         #  its names, its GRPDEF as the source says - the others
            for sg in mem:                                  #  in the order the modules name them)
                if real.get(sg.upper(), sg) not in order:
                    order.append(real.get(sg.upper(), sg))
    for n, pieces, d, c, f, s in mods:
        for sg in pieces:
            if sg not in order:
                order.insert(0, sg) if sg.upper().endswith("TEXT") else order.append(sg)
    code = [sg for sg in order if any(sg in pc and pc[sg].hasdata and sg.upper().endswith("TEXT") for n, pc, d, c, f, s in mods)] or order[:1]
    segat = {}

    def place(seg, base):
        at = base
        first = True
        for n, pieces, defined, common, fixes, start in mods:
            p = pieces.get(seg)
            if p is None:
                continue
            at = (at + p.align - 1) // p.align * p.align
            if first:
                segat[seg] = at
                first = False
            p.at = at
            at += p.length
        if first:
            segat[seg] = at
        return at
    at = 0
    for sg in code:
        at = place(sg, at)
    end = at
    dgroup = (end + 15) & ~15
    at = dgroup
    rest = [sg for sg in order if sg not in code]
    comm_seg = next((sg for sg in rest if sg.lower() == "c_common"), None)
    for sg in rest[:rest.index(comm_seg)] if comm_seg else rest:
        if sg.upper() != "STACK":
            at = place(sg, at)
    # the communal variables: in the order of their places in SYMBOLS, one after another
    common, names, gdef = {}, set(), {}
    known = {k.lower(): v[1] for k, v in symbols.items()}
    for n, pieces, defined, com, fixes, start in mods:
        for k, v in com.items():
            common[k] = max(common.get(k, 0), v)
        for k in defined:
            if k in names and not k.startswith("l"):
                raise SystemExit("%s is defined twice (%s)" % (k, n))
            names.add(k)
    common = {k: v for k, v in common.items() if k not in names}
    missing = [k for k in common if k not in known]
    if missing:
        raise SystemExit("communal variables SYMBOLS has no place for: %s" % " ".join(sorted(missing)))
    if comm_seg:
        p = next(pc[comm_seg] for n, pc, d, c, f, s in mods if comm_seg in pc)
        at = (at + p.align - 1) // p.align * p.align
        segat[comm_seg] = at
        p.at = at
        for k in sorted(common, key=lambda k: known[k]):
            if known[k] != at - dgroup:
                say("  %s: SYMBOLS has it at %04x; the variables before it end at %04x" % (k, known[k], at - dgroup))
            gdef[k] = at - dgroup
            at += (common[k] + 1) & ~1
        for sg in rest[rest.index(comm_seg) + 1:]:
            if sg.upper() != "STACK":
                at = place(sg, at)
    stseg = next((sg for sg in rest if sg.upper() == "STACK"), None)
    if stseg:
        at = place(stseg, at)
    top = at
    for n, pieces, defined, com, fixes, start in mods:
        for k, (seg, off) in defined.items():
            gdef[k] = pieces[seg].at + off - (dgroup if seg not in code else 0)
    stack = [pc[sg] for n, pc, d, c, f, s in mods for sg in pc if pc[sg].stack]
    stack = stack[0] if stack else None
    # the image goes as far as the last segment that has data in it does - to its end
    withdata = {p.seg for n, pc, d, c, f, s in mods for p in pc.values() if p.hasdata and p.length}
    lastdata = max(p.at + p.length for n, pc, d, c, f, s in mods for p in pc.values() if p.seg in withdata and p.at is not None)
    image = bytearray(lastdata)
    relocs = []
    entry = None
    frame = lambda seg: dgroup if seg not in code else 0
    for n, pieces, defined, com, fixes, start in mods:
        for p in pieces.values():
            if p.hasdata:
                image[p.at:p.at + len(p.data)] = p.data
        if start:
            entry = pieces[start[0]].at + start[1]
        for seg, off, how, tg, add in fixes:
            loc = pieces[seg].at + off
            if tg[0] == "sym":
                if tg[1] not in gdef:
                    raise SystemExit("%s: %s is not defined" % (n, tg[1]))
                v = gdef[tg[1]]
            elif tg[0] == "seg":
                v = pieces[tg[1]].at - frame(tg[1])
            else:
                v = 0
            if how == "seg":
                v = dgroup >> 4
                relocs.append(loc)
            elif how == "rel":
                v = v - (pieces[seg].at - frame(seg)) + add
            else:
                v = v + add
            struct.pack_into("<H", image, loc, v & 0xFFFF)
    far = lambda r: (r, 0) if r < dgroup else (r - dgroup, dgroup >> 4)
    cbrlc = 30 + 4 * len(relocs)
    hdrlen = (cbrlc + 511) & ~511
    size = hdrlen + len(image)
    hdr = struct.pack("<15H", 0x5A4D, size & 511, (size + 511) >> 9, len(relocs), hdrlen >> 4, (top - len(image) + 15) >> 4, 0xFFFF,
                      (stack.at >> 4) if stack else 0, stack.length if stack else 0, 0, far(entry or 0)[0], far(entry or 0)[1], 30, 0, 1)
    exe = bytearray(hdr) + b"".join(struct.pack("<HH", *far(r)) for r in relocs)
    exe += bytes(hdrlen - len(exe)) + image
    struct.pack_into("<H", exe, 18, ~sum(struct.unpack("<%dH" % ((len(exe) + 1) // 2), exe + bytes(len(exe) & 1))) & 0xFFFF)
    link24.mods, link24.dgroup = mods, dgroup                # (where each module's pieces went: a map)
    say("  the text ends at %05x, DGROUP is at %05x; the image is %04x bytes, the program ends at %05x" % (end, dgroup, len(image), top))
    return bytes(exe)
