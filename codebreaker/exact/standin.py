"""standin.py -- a source file's object module for the link of the whole program, with stand-ins.

A module is compiled whole. A function of it that is not yet the program's would be of another length
and push everything behind it out of place; so, for the link, its code in the object module is
replaced by THE PROGRAM'S BYTES for that function - a stand-in, with no words for the linker to fill
(they are filled already) - and the functions behind it are moved, the module's records written
again. What the linked program then proves is everything else: the order of the files, the data,
the place of every variable, the library's part, the header and the relocation table. A stand-in
proves nothing about its function; the ledger is what counts the functions.

Where a project has several toolchains, a function another toolchain's object of the same file
gives is taken from that object (its names renumbered) before the program's bytes are.

    raw, stand = standin.rebuild(prj, raw, addrs, end, title, alts=[])
        addrs   where the module's functions stand in the program, in order; end: where its code ends
        stand   the names of the functions that are stand-ins
"""
import re
import struct

from codebreaker.exact import compare
from codebreaker.formats import omf


def _ix(n):
    return bytes([n]) if n < 0x80 else bytes([0x80 | (n >> 8), n & 0xFF])


def _idx(r, q):
    return (r[q], q + 1) if r[q] < 0x80 else (((r[q] & 0x7F) << 8) | r[q + 1], q + 2)


def fixups(r, thr):
    """a FIXUPP record's fixups, the threads resolved -> [dict]; thr is kept from record to record"""
    out = []
    q = 0
    while q < len(r):
        c = r[q]
        if not c & 0x80:
            q += 1
            d = (c >> 6) & 1
            meth = (c >> 2) & 7
            ix = None
            if not (d and meth >= 4):
                ix, q = _idx(r, q)
            thr[(d, c & 3)] = (meth, ix)
            continue
        f = dict(M=(c >> 6) & 1, loc=(c >> 2) & 0xF, off=((c & 3) << 8) | r[q + 1])
        fd = r[q + 2]
        q += 3
        if fd & 0x80:
            f["fm"], f["fi"] = thr[(1, (fd >> 4) & 3)]
        else:
            f["fm"] = (fd >> 4) & 7
            f["fi"] = None
            if f["fm"] < 3:
                f["fi"], q = _idx(r, q)
        if fd & 8:
            f["tm"], f["ti"] = thr[(0, fd & 3)]
        else:
            f["tm"] = fd & 3
            f["ti"], q = _idx(r, q)
        f["P"] = (fd >> 2) & 1
        f["disp"] = 0
        if not f["P"]:
            f["disp"] = r[q] | r[q + 1] << 8
            q += 2
        out.append(f)
    return out


def fixrec(fs, base):
    """the fixups (their places as offsets in the segment) written for a data record that begins at base"""
    b = b""
    for f in fs:
        o = f["at"] - base
        assert 0 <= o < 1024
        b += bytes([0x80 | f["M"] << 6 | f["loc"] << 2 | o >> 8, o & 0xFF, f["fm"] << 4 | f["P"] << 2 | f["tm"]])
        if f["fm"] < 3:
            b += _ix(f["fi"])
        b += _ix(f["ti"])
        if not f["P"]:
            b += struct.pack("<H", f["disp"])
    return omf.record(0x9C, b)


def _model(raw):
    names, segs, exts, thr, last, allfix, grps = [], [], [], {}, None, {}, []
    for t, r in omf.records(raw):
        if t == 0x96:
            q = 0
            while q < len(r):
                names.append(r[q + 1:q + 1 + r[q]].decode("latin1"))
                q += 1 + r[q]
        elif t == 0x98:
            q = 1 if r[0] >> 5 else 4
            ni, _ = _idx(r, q + 2)
            segs.append(names[ni - 1])
        elif t == 0x9A:
            gi, _ = _idx(r, 0)
            grps.append(names[gi - 1])
        elif t == 0x8C:
            q = 0
            while q < len(r):
                exts.append(r[q + 1:q + 1 + r[q]].decode("latin1"))
                q += 1 + r[q]
                _, q = _idx(r, q)
        elif t in (0xA0, 0xA2):
            si, q = _idx(r, 0)
            last = (si, r[q] | r[q + 1] << 8)
        elif t == 0x9C:
            for f in fixups(r, thr):
                f["seg"] = last[0]
                f["at"] = last[1] + f["off"]
                allfix.setdefault(last[0], []).append(f)
    o = omf.read(raw)
    text = omf.code_segments(o)[0]
    return dict(o=o, segs=segs, exts=exts, grps=grps, fix=allfix, ti=segs.index(text) + 1, textname=text,
                text=bytearray(o.seg[text]), fns=omf.functions(o, text))


def _threads(raw):
    thr = {}
    for t, r in omf.records(raw):
        if t == 0x9C:
            fixups(r, thr)
    return thr


def rebuild(prj, raw, addrs, end, title=None, alts=()):
    from codebreaker.exact import x86
    img = prj.image
    syms = prj.symbols()
    thr_all = _threads(raw)
    recs = omf.records(raw)
    B = _model(raw)
    segs, ti = B["segs"], B["ti"]
    allfix = B["fix"]
    if len(B["fns"]) != len(addrs):
        raise ValueError("%s: %d functions compiled, %d places" % (title, len(B["fns"]), len(addrs)))
    dat = lambda m: [(s if not re.fullmatch(r"T\d+_DATA", s) else "far", bytes(d)) for s, d in m["o"].seg.items() if s != m["textname"]]
    As = []
    for x in alts:
        A = _model(x)
        if len(A["fns"]) == len(addrs) and dat(A) == dat(B):    # (only when the data is the same)
            As.append(A)
    new = bytearray()
    F = []
    stand = []
    for k, at in enumerate(addrs):
        nxt = addrs[k + 1] if k + 1 < len(addrs) else end
        src = None
        for tag, M in [("B", B)] + [("A", A) for A in As]:
            f_, off, ln = M["fns"][k]
            # (each of the file's functions at its own place: one before it may be of another length - a stand-in)
            where = lambda x, M=M: next((addrs[j] + x - o_ for j, (n_, o_, l_) in enumerate(M["fns"])
                                         if o_ <= x < o_ + max(l_, 1)), at - off + x)
            bad, notes = compare.function(M["o"], img, f_, off, ln, at, prj, dict(syms), {}, M["textname"], where)
            tail = k == len(addrs) - 1 and ln < nxt - at and not img[at + ln:nxt].strip(bytes(1))
            if compare.identical(bad, notes) and (ln == nxt - at or tail):     # (the last: zeros after it are not
                if tail:                                                        #  the module's - a segment of the
                    nxt = min(nxt, at + ln + ((at + ln) & 1))                   #  next ones begins with them - but
                                                                                #  its pad to an even place is)
                src = (tag, M, k)
                break
        F.append(dict(name=B["fns"][k][0], src=src, new=len(new), at=at, end=nxt))
        if src is None:
            new += img[at:nxt]
            stand.append(B["fns"][k][0])
        else:
            tag, M, idx = src
            _, off, ln = M["fns"][idx]
            new += M["text"][off:off + ln]
            if nxt > at + ln:                               # (the module's pad to an even place, behind its last)
                new += img[at + ln:nxt]

    def moved(M, v):
        for k, (fn, off, ln) in enumerate(M["fns"]):
            if off <= v < off + ln or (k == len(M["fns"]) - 1 and v == off + ln):
                return F[k]["new"] + v - off, k
        return None, None
    extnames = list(B["exts"])
    tfix = []
    # a stand-in's segment words (a far call's, DGROUP's): the program's relocations there are fixups again, so
    # that the linker lists them; the word itself is the linker's to fill
    rel = sorted(prj.packed.relocs) if getattr(prj, "packed", None) else sorted(prj.relocs())
    byaddr = {a: n for n, (t, a) in syms.items() if t == "C" and ":" not in n}
    for fd in F:
        if fd["src"] is not None:
            continue
        for s in rel:
            if not (fd["at"] <= s < fd["end"]) or s + 2 > fd["end"]:
                continue
            k = fd["new"] + s - fd["at"]
            w = struct.unpack_from("<H", img, s)[0]
            struct.pack_into("<H", new, k, 0)               # (the linker adds the segment to what is there)
            g = dict(M=1, loc=2, fm=5, fi=None, P=1, disp=0, at=k)
            if s >= 3 and img[s - 3] == 0x9A and s - 3 >= fd["at"]:     # a far call: the function it calls
                dest = (w - prj.loadseg) * 16 + struct.unpack_from("<H", img, s - 2)[0]
                nm = byaddr.get(dest)
                if nm is None:
                    raise ValueError("%s: a far call to %05x, which SYMBOLS has no name for" % (fd["name"], dest))
                if nm not in extnames:
                    extnames.append(nm)
                g.update(tm=2, ti=extnames.index(nm) + 1)
            elif (w - prj.loadseg) * 16 == prj.dgroup and "DGROUP" in B["grps"]:
                g.update(tm=1, ti=B["grps"].index("DGROUP") + 1)
            else:
                raise ValueError("%s: a segment word %04x at %05x of no kind known" % (fd["name"], w, s))
            tfix.append(g)
    for k, fd in enumerate(F):
        if fd["src"] is None:
            continue
        tag, M, idx = fd["src"]
        fn, off, ln = M["fns"][idx]
        for f in M["fix"].get(M["ti"], []):
            if not (off <= f["at"] < off + ln):
                continue
            g = dict(f)
            g["at"] = fd["new"] + f["at"] - off
            if f["tm"] == 0 and f["ti"] == M["ti"]:          # a place of this file's own code: it has moved
                v = struct.unpack_from("<H", M["text"], f["at"])[0]
                v2, _ = moved(M, v)
                struct.pack_into("<H", new, g["at"], (v2 if v2 is not None else v) & 0xFFFF)
            if tag != "B":                                   # (the other module numbers its names its own way)
                for key, meth in (("ti", "tm"), ("fi", "fm")):
                    if f[meth] == 2:
                        nm = M["exts"][f[key] - 1]
                        if nm not in extnames:
                            extnames.append(nm)
                        g[key] = extnames.index(nm) + 1
            tfix.append(g)
    fixed = {f["at"] for f in tfix}
    for k, fd in enumerate(F):                              # calls and jumps between the file's functions that
        if fd["src"] is None:                               # the compiler filled itself
            continue
        tag, M, idx = fd["src"]
        fn, off, ln = M["fns"][idx]
        for i in x86.disasm(M["text"], off, off + ln, False):
            if i.mnemonic in ("call", "jmp") and i.size == 3 and M["text"][i.address] in (0xE8, 0xE9):
                here = fd["new"] + i.address - off
                if here + 1 in fixed:
                    continue
                tgt = (i.address + 3 + struct.unpack_from("<h", M["text"], i.address + 1)[0]) & 0xFFFF
                if not (off <= tgt < off + ln):
                    t2, _ = moved(M, tgt)
                    if t2 is not None:
                        struct.pack_into("<h", new, here + 1, ((t2 - (here + 3) + 0x8000) & 0xFFFF) - 0x8000)
    newext = extnames[len(B["exts"]):]

    def where(x):
        v, k = moved(B, x)
        return v - x if v is not None else None

    class Out(object):
        """the records again; a file's code (and the segment words its code names, in CONST) before its data"""
        def __init__(self):
            self.head = self.code = self.data = self.tail = b""
            self.cur = "head"

        def add(self, b):
            setattr(self, self.cur, getattr(self, self.cur) + b)

        def all(self):
            return self.head + self.code + self.data + self.tail
    out = Out()
    ci = segs.index("CONST") + 1 if "CONST" in segs else 0
    done_text = skipfix = extdone = False
    lastseg, lastbase, segn = None, 0, 0
    for t, r in recs:
        if t == 0x80 and title:
            out.add(omf.record(0x80, omf.name(title)))
            continue
        if t == 0x96 and title:                             # (the far data's segment is named after the file)
            q = 0
            nb = b""
            while q < len(r):
                s = r[q + 1:q + 1 + r[q]]
                q += 1 + r[q]
                if re.fullmatch(rb"T\d+_DATA", s):
                    s = title.encode() + b"_DATA"
                elif re.fullmatch(rb"T\d+_TEXT", s):
                    s = title.encode() + b"_TEXT"
                nb += bytes([len(s)]) + s
            out.add(omf.record(t, nb))
            continue
        if t == 0x98:
            segn += 1
            if segn == ti:
                q = 1 if r[0] >> 5 else 4
                r = r[:q] + struct.pack("<H", len(new)) + r[q + 2:]
            out.add(omf.record(t, r))
            continue
        if t in (0x90, 0xA0, 0xA2, 0x8A) and newext and not extdone:
            extdone = True
            out.cur = "head"
            out.add(omf.record(0x8C, b"".join(omf.name(n) + b"\x00" for n in newext)))
        if t == 0x90:
            gi, q = _idx(r, 0)
            si, q = _idx(r, q)
            head = r[:q]
            if si == 0:
                head = r[:q + 2]
                q += 2
            body = b""
            while q < len(r):
                n = r[q]
                nm = r[q:q + 1 + n]
                q += 1 + n
                off = r[q] | r[q + 1] << 8
                ty, q2 = _idx(r, q + 2)
                if si == ti:
                    off += where(off) or 0
                body += nm + struct.pack("<H", off) + r[q + 2:q2]
                q = q2
            out.add(omf.record(t, head + body))
            continue
        if t in (0xA0, 0xA2):
            si, q = _idx(r, 0)
            lastseg = si
            lastbase = r[q] | r[q + 1] << 8
            out.cur = "code" if si in (ti, ci) else "data"
            if si == ti:
                skipfix = True
                if not done_text:
                    done_text = True
                    p = 0
                    fs = sorted(tfix, key=lambda f: f["at"])
                    while p < len(new):
                        e = min(p + 1000, len(new))
                        for f in fs:                        # (no fixup's word across two records)
                            w = 4 if f["loc"] == 3 else 2
                            if f["at"] < e < f["at"] + w:
                                e = f["at"]
                        out.add(omf.record(0xA0, _ix(ti) + struct.pack("<H", p) + bytes(new[p:e])))
                        here = [f for f in fs if p <= f["at"] < e]
                        if here:
                            out.add(fixrec(here, p))
                        p = e
                continue
            skipfix = False
            out.add(omf.record(t, r))
            continue
        if t == 0x9C:
            if skipfix:
                continue
            mine = fixups(r, dict(thr_all))
            if mine:
                for f in mine:
                    f["at"] = lastbase + f["off"]
                out.add(fixrec(mine, lastbase))
            continue
        if t in (0x8A, 0x8B):
            out.cur = "tail"
        elif t not in (0x9C, 0xA0, 0xA2):
            out.cur = "head"
        out.add(omf.record(t, r))
    return out.all(), stand
