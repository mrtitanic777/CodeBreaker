"""omf.py -- Intel/Microsoft OMF-86 object modules: read one whole, write the few kinds exact makes.

    o = omf.read(data)
        o.seg      {segment name: bytes}            (LEDATA and LIDATA laid out)
        o.segs     [(name, class, length, align)]   in the order defined
        o.groups   {group: [segment names]}
        o.pubs     [(name, segment or None, offset)]
        o.ext      [names]                          (EXTDEF and COMDEF)
        o.comm     {name: (size, far)}              the communal variables (COMDEF)
        o.fix      [(segment, offset, kind, target, displacement)]
                   kind: 'off' 'base' 'ptr' 'lo' 'rel' (self-relative); target: ('ext', name) /
                   ('seg', name) / ('grp', name)
        o.raw      the bytes read
    omf.functions(o, seg="_TEXT")  -> [(name, offset, length)] the segment's publics, in order
    omf.code_segments(o)           -> the object's code segments
    omf.holes(o, seg)              -> offsets of the segment the linker fills in
    omf.library(data)              -> [(module name, bytes)] of a .LIB
    omf.records(data)              -> [(type, payload)]
    omf.record(type, payload)      -> one record's bytes, checksum included
    omf.name(s)                    -> a length-prefixed name
"""
import struct


def _idx(r, q):
    return (((r[q] & 0x7F) << 8) | r[q + 1], q + 2) if r[q] & 0x80 else (r[q], q + 1)


def records(data):
    out = []
    p = 0
    while p + 3 <= len(data):
        t = data[p]
        n = data[p + 1] | data[p + 2] << 8
        out.append((t, bytes(data[p + 3:p + 3 + n - 1])))
        p += 3 + n
        if t in (0x8A, 0x8B):
            break
    return out


def record(t, payload):
    b = bytes([t]) + struct.pack("<H", len(payload) + 1) + bytes(payload)
    return b + bytes([(-sum(b)) & 0xFF])


def name(s):
    s = s.encode("latin1") if isinstance(s, str) else s
    return bytes([len(s)]) + s


class Obj(object):
    pass


def read(data):
    from codebreaker.exact import aout
    if aout.is_aout(data):                                  # (a Version 7 a.out: XENIX's compiler)
        return aout.read(data)
    o = Obj()
    names = []
    segs = []
    seginfo = []
    buf = {}
    o.pubs, o.ext, o.fix, o.groups, o.comm = [], [], [], {}, {}
    last = None
    thr = {}
    filled = {}
    for t, r in records(data):
        if t == 0x96:
            q = 0
            while q < len(r):
                names.append(r[q + 1:q + 1 + r[q]].decode("latin1"))
                q += 1 + r[q]
        elif t == 0x98:
            align = r[0] >> 5
            q = 1 if align else 4
            ln = r[q] | r[q + 1] << 8
            if r[0] & 2:
                ln = 0x10000
            ni, q = _idx(r, q + 2)
            ci, q = _idx(r, q)
            segs.append(names[ni - 1])
            seginfo.append((names[ni - 1], names[ci - 1] if ci else "", ln, align))
            buf.setdefault(len(segs), bytearray(ln))
        elif t == 0x9A:
            gi, q = _idx(r, 0)
            mem = []
            while q < len(r):
                si, q = _idx(r, q + 1)
                mem.append(segs[si - 1])
            o.groups[names[gi - 1]] = mem
        elif t in (0x8C, 0xB0):
            q = 0
            while q < len(r):
                nm = r[q + 1:q + 1 + r[q]].decode("latin1")
                o.ext.append(nm)
                q += 1 + r[q]
                _, q = _idx(r, q)
                if t == 0xB0:                       # a communal variable: its kind and size follow
                    far = r[q] == 0x61
                    q += 1
                    vals = []
                    for _ in range(2 if far else 1):
                        c = r[q]
                        k = {0x81: 2, 0x84: 3, 0x88: 4}.get(c, 0)
                        vals.append(int.from_bytes(r[q + 1:q + 1 + k], "little") if k else c)
                        q += 1 + k
                    o.comm[nm] = (vals[0] * vals[1] if far else vals[0], far)
        elif t == 0x90:
            gi, q = _idx(r, 0)
            si, q = _idx(r, q)
            if si == 0:
                q += 2
            while q < len(r):
                nm = r[q + 1:q + 1 + r[q]].decode("latin1")
                q += 1 + r[q]
                off = r[q] | r[q + 1] << 8
                _, q = _idx(r, q + 2)
                o.pubs.append((nm, segs[si - 1] if si else None, off))
        elif t == 0xA0:
            si, q = _idx(r, 0)
            off = r[q] | r[q + 1] << 8
            d = r[q + 2:]
            b = buf[si]
            if off + len(d) > len(b):
                b.extend(bytes(off + len(d) - len(b)))
            b[off:off + len(d)] = d
            last = (si, off)
            filled[si] = max(filled.get(si, 0), off + len(d))
        elif t == 0xA2:
            si, q = _idx(r, 0)
            off = r[q] | r[q + 1] << 8
            q += 2

            def block(q):
                rep = r[q] | r[q + 1] << 8
                cnt = r[q + 2] | r[q + 3] << 8
                q += 4
                if cnt == 0:
                    return bytes(r[q + 1:q + 1 + r[q]]) * rep, q + 1 + r[q]
                s = b""
                for _ in range(cnt):
                    x, q = block(q)
                    s += x
                return s * rep, q
            out = b""
            while q < len(r):
                x, q = block(q)
                out += x
            b = buf[si]
            if off + len(out) > len(b):
                b.extend(bytes(off + len(out) - len(b)))
            b[off:off + len(out)] = out
            last = (si, off)
            filled[si] = max(filled.get(si, 0), off + len(out))
        elif t == 0x9C:
            q = 0
            while q < len(r):
                c = r[q]
                if not c & 0x80:                     # a thread
                    q += 1
                    d = (c >> 6) & 1
                    meth = (c >> 2) & 7
                    ix = None
                    if not (d and meth >= 4):
                        ix, q = _idx(r, q)
                    thr[(d, c & 3)] = (meth, ix)
                    continue
                segrel = (c >> 6) & 1
                loc = (c >> 2) & 7
                off = ((c & 3) << 8) | r[q + 1]
                fd = r[q + 2]
                q += 3
                if fd & 0x80:
                    fm, fi = thr[(1, (fd >> 4) & 3)]
                else:
                    fm = (fd >> 4) & 7
                    fi = None
                    if fm < 3:
                        fi, q = _idx(r, q)
                if fd & 8:
                    tm, ti = thr[(0, fd & 3)]
                else:
                    tm = fd & 3
                    ti, q = _idx(r, q)
                disp = 0
                if not fd & 4:
                    disp = r[q] | r[q + 1] << 8
                    q += 2
                tgt = (("seg", segs[ti - 1]) if tm & 3 == 0 else
                       ("grp", names[ti - 1] if ti and ti - 1 < len(names) else ti) if tm & 3 == 1 else
                       ("ext", o.ext[ti - 1]))
                kind = {1: "off", 2: "base", 3: "ptr", 0: "lo", 5: "off"}.get(loc, "?%d" % loc)
                if not segrel:
                    kind = "rel"
                o.fix.append((segs[last[0] - 1], last[1] + off, kind, tgt, disp))
    o.seg = {segs[i - 1]: bytes(d) for i, d in buf.items()}
    o.segs = seginfo
    o.filled = {segs[i - 1]: n for i, n in filled.items()}
    o.raw = bytes(data)
    return o


def library(data):
    """[(name, module bytes)] of an OMF library (.LIB), in its order; a name met again is NAME#2, #3 ..."""
    if data[0] != 0xF0:
        return [("(object)", data)]
    page = (data[1] | data[2] << 8) + 3
    dict_off = struct.unpack_from("<I", data, 3)[0]
    out = []
    seen = {}
    p = page
    while p < dict_off:
        if data[p] != 0x80:
            p += page - (p % page) if p % page else page
            continue
        q = p
        while q + 3 <= len(data):
            t = data[q]
            q += 3 + (data[q + 1] | data[q + 2] << 8)
            if t in (0x8A, 0x8B):
                break
        nm = data[p + 4:p + 4 + data[p + 3]].decode("latin1")
        seen[nm] = seen.get(nm, 0) + 1
        out.append((nm + ("#%d" % seen[nm] if seen[nm] > 1 else ""), data[p:q]))
        p = q
        if p % page:
            p += page - p % page
    return out


FIXLEN = {"lo": 1, "off": 2, "base": 2, "ptr": 4, "rel": 2}


def holes(o, seg):
    """the offsets of a segment's bytes the linker fills in"""
    out = set()
    for s, at, kind, tgt, disp in o.fix:
        if s == seg:
            out.update(range(at, at + FIXLEN.get(kind, 2)))
    return out


def code_segments(o):
    """the object's code segments, the first named first (a compiler's NAME_TEXT, or more by #pragma alloc_text)"""
    return [s for s, cls, ln, al in o.segs if s.endswith("_TEXT") or cls.upper() == "CODE"]


def functions(o, seg="_TEXT"):
    """[(name, offset, length)] of a code segment, in order. The last ends where the segment's data end (a SEGDEF
    may count a byte more: C 5.0 at times counts one past a closing NOP); code before the first name is
    "(module start)" (an assembly module's)"""
    ps = sorted((off, nm) for nm, s, off in o.pubs if s == seg)
    n = min(len(o.seg.get(seg, b"")), o.filled.get(seg, 0))
    if ps and ps[0][0] > 0:
        ps.insert(0, (0, "(module start)"))
    return [(nm, off, (ps[k + 1][0] if k + 1 < len(ps) else n) - off) for k, (off, nm) in enumerate(ps)]


def rename_externs(data, mapping):
    """the object with external names changed (a compiler that calls its helpers by other names than the
    library the program was linked with)"""
    out = b""
    for t, r in records(data):
        if t == 0x8C:
            q = 0
            nb = b""
            while q < len(r):
                nm = r[q + 1:q + 1 + r[q]]
                q += 1 + r[q]
                ty = r[q:q + 1]
                q += 1
                nm = mapping.get(nm.decode("latin1"), nm.decode("latin1")).encode("latin1")
                nb += bytes([len(nm)]) + nm + ty
            r = nb
        out += record(t, r)
    return out


def raw_object(title, code, pubs, data=b"", dpubs=None, text="_TEXT", dalign=1):
    """an object module of the program's own bytes: a run of its code (and of its data), with the public names
    others call it by - a stand-in for a part no source and no library module gives. Its words for the linker
    are the program's already, so it stands only where it stood."""
    out = record(0x80, name(title))
    out += record(0x96, name("") + name("DGROUP") + name(text) + name("CODE") + name("_DATA") + name("DATA"))
    out += record(0x98, bytes([0x28]) + struct.pack("<H", len(code)) + bytes([3, 4, 1]))      # byte aligned, public
    out += record(0x98, bytes([0x48 if dalign == 2 else 0x28]) + struct.pack("<H", len(data)) + bytes([5, 6, 1]))
    out += record(0x9A, bytes([2, 0xFF, 2]))                                                  # DGROUP: _DATA
    for seg, b in ((1, code), (2, data)):
        for k in range(0, len(b), 1000):
            out += record(0xA0, bytes([seg]) + struct.pack("<H", k) + b[k:k + 1000])
    for grp, seg, ps in ((0, 1, pubs), (1, 2, dpubs or {})):
        if ps:
            body = bytes([grp, seg])
            for n, a in sorted(ps.items(), key=lambda kv: kv[1]):
                body += name(n) + struct.pack("<H", a) + b"\x00"
            out += record(0x90, body)
    return out + record(0x8A, b"\x00")


def bss_object(names, start, end, title="bssvars"):
    """an object module of variables without a value, each a public name at its place in _BSS (start..end): the
    linker places communal names by its own table's order (a hash of the names), and ours are not the original
    names - so they are given their places outright and nothing communal is left to place"""
    out = record(0x80, name(title))
    out += record(0x96, name("") + name("DGROUP") + name("_BSS") + name("BSS"))
    out += record(0x98, bytes([0x48]) + struct.pack("<H", end - start) + bytes([3, 4, 1]))    # word aligned, public
    out += record(0x9A, bytes([2, 0xFF, 1]))
    items = sorted(names.items(), key=lambda kv: (kv[1], kv[0]))
    k = 0
    while k < len(items):
        body = bytes([1, 1])
        while k < len(items) and len(body) < 900:
            n, a = items[k]
            body += name(n) + struct.pack("<H", a - start) + b"\x00"
            k += 1
        out += record(0x90, body)
    return out + record(0x8A, b"\x00")


def names_of(data):
    """(the public names an object module defines, the external names it needs)"""
    o = read(data)
    return {n for n, s, off in o.pubs}, set(o.ext)
