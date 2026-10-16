"""recut.py -- a module's code records cut again, as the original program's order of relocations shows.

The linker lists a program's relocations in the order it meets the fixups that need them: each data record's
fixups in the order the record lists them, the records in their order. A compiler cut a file's code into records
by the source's lines and listed a record's fixups as it pleased (Microsoft C: the last first); a source put
together from pieces does not cut where the original's did. The bytes do not depend on it - the relocation table
does (and so EXEPACK's). So for the link, a code segment is written again in records cut where the original's
relocations show one ended (where the next lies after the one before: within a record they run down), each
record listing its relocations in the original's order. It proves nothing about the source's lines.

    recut.recut(project, raw) -> the object module's bytes (unchanged when its relocations are not the program's)
    project "recut": true asks the link stage for it
"""
import struct

from codebreaker.exact import omf
from codebreaker.exact.standin import _idx, _ix, fixrec, fixups


def order(prj):
    """the program's relocations, image addresses in the order of its table (EXEPACK's when it is packed)"""
    if getattr(prj, "packed", None):
        return list(prj.packed.relocs)
    h = prj.header
    if not h:
        return []
    return [s * 16 + o for o, s in struct.iter_unpack("<HH", prj.file[h[12]:h[12] + 4 * h[3]])]


def recut(prj, raw):
    syms = prj.symbols()
    o = omf.read(raw)
    recs = omf.records(raw)
    names, segs, thr, last, fix = [], [], {}, None, {}
    for t, r in recs:
        if t == 0x96:
            q = 0
            while q < len(r):
                names.append(r[q + 1:q + 1 + r[q]].decode("latin1"))
                q += 1 + r[q]
        elif t == 0x98:
            q = 1 if r[0] >> 5 else 4
            ni, _ = _idx(r, q + 2)
            segs.append(names[ni - 1])
        elif t in (0xA0, 0xA2):
            si, q = _idx(r, 0)
            last = (si, r[q] | r[q + 1] << 8)
        elif t == 0x9C:
            for f in fixups(r, thr):
                f["at"] = last[1] + f["off"]
                fix.setdefault(last[0], []).append(f)
    orig = order(prj)
    plan = {}
    for si, sn in enumerate(segs, 1):
        if sn not in omf.code_segments(o) or not len(o.seg.get(sn, b"")):
            continue
        pubs = sorted((off, nm) for nm, sg, off in o.pubs if sg == sn and nm in syms and syms[nm][0] in ("C", "?"))
        if not pubs:
            continue
        base = syms[pubs[0][1]][1] - pubs[0][0]
        code = bytes(o.seg[sn][:o.filled.get(sn, len(o.seg[sn]))])
        n = len(code)
        relw = lambda f: f["at"] + (2 if f["loc"] == 3 else 0)    # (the word of a segment the fixup makes)
        fl = fix.get(si, [])
        rel = {relw(f): f for f in fl if f["loc"] in (2, 3)}
        seq = [x - base for x in orig if base <= x < base + n and x - base in rel]
        if sorted(seq) != sorted(rel):
            return raw                                      # (not the program's relocations: left as it is)
        runs = []
        for x in seq:
            if runs and x < runs[-1][-1]:
                runs[-1].append(x)
            else:
                runs.append([x])
        spans = [(f["at"], f["at"] + (4 if f["loc"] == 3 else 2)) for f in fl]
        ok = lambda c: all(not (a < c < b) for a, b in spans)  # (a cut splits no fixup's word)
        cuts = [0]
        for k in range(1, len(runs)):
            lo = max(runs[k - 1]) + 2
            hi = min(runs[k]) - (2 if rel[min(runs[k])]["loc"] == 3 else 0)
            c = next((c for c in range(hi, lo - 1, -1) if ok(c)), None)
            if c is None:
                return raw
            cuts.append(c)
        cuts.append(n)
        bounds = []
        for a, b in zip(cuts, cuts[1:]):                    # (no record over a kilobyte)
            while b - a > 1024:
                c = next(c for c in range(a + 1024, a, -1) if ok(c))
                bounds.append((a, c))
                a = c
            bounds.append((a, b))
        plan[si] = (code, bounds, fl, {x: k for k, x in enumerate(seq)})
    if not plan:
        return raw
    out = b""
    done = set()
    last = None
    for t, r in recs:
        if t in (0xA0, 0xA2):
            si, q = _idx(r, 0)
            last = si
            if si in plan:
                if si not in done:
                    done.add(si)
                    code, bounds, fl, rank = plan[si]
                    for a, b in bounds:
                        out += omf.record(0xA0, _ix(si) + struct.pack("<H", a) + code[a:b])
                        here = [f for f in fl if a <= f["at"] < b]
                        relf = sorted((f for f in here if f["loc"] in (2, 3)), key=lambda f: rank[f["at"] + (2 if f["loc"] == 3 else 0)])
                        other = [f for f in here if f["loc"] not in (2, 3)]
                        both = relf + other
                        for k in range(0, len(both), 100):  # (a FIXUPP record of no more than a kilobyte or so)
                            out += fixrec(both[k:k + 100], a)
                continue
        if t == 0x9C and last in plan:
            continue
        out += omf.record(t, r)
    return out
