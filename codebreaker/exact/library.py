"""library.py -- the library modules a program was linked with, found in it; the rest of its library part as stand-ins.

    python -m codebreaker exact PROJECT library [--write] [-v]

A linked program holds each library module it took byte for byte, but for the words the linker
filled in. Every module of the candidate libraries (the toolchain profile's for the project's
model, then the project's own "libraries") is looked for in the program: its code (each code
segment, the filled words left out) among the program's code, its data in the data segment.

  - code found at one place places the module; where several modules have the same bytes there
    (one routine in several libraries, or entry points that share a body) the one whose public
    name the program's callers use (SYMBOLS) is taken, else the earliest library's; the others are
    noted ("also")
  - a module of data only is placed by its data, else by its names' places (SYMBOLS, or the words
    of the placed modules' code that name them)
  - the library part is the run of placed modules at the end of the code: it begins after the last
    gap longer than `gap` bytes (project "library_gap", default 2048) before them - a lone module
    among the program's own functions is not its start (project "library_start" says it outright)
  - inside the library part, code no module accounts for becomes a stand-in (RAW: the program's own
    bytes), with the public names the placed modules' calls into it show

The linker takes modules in the order it comes to need them, which hangs on the order of names in
the objects that need them, not ours - so a rebuild links them as objects in the program's order:
src/CRTLIB, one line each:

    [TAG:]NAME        # code 03bf8 (334 bytes)  data 1520  (also ...)
    RAW name code=LO-HI pub=NAME@OFF ...        # (no library module has these bytes)

    library.find(project) -> Map (.modules [Placed], .start, .raws, .lines())
"""
import os
import struct

from codebreaker.exact import omf

W = lambda b, k: struct.unpack_from("<H", b, k)[0]


class Module(object):
    def __init__(self, tag, name, raw, rank):
        self.tag, self.name, self.raw, self.rank = tag, name, raw, rank
        self.o = o = omf.read(raw)
        self.code = [(s, o.seg[s][:o.filled.get(s, 0)], omf.holes(o, s)) for s in omf.code_segments(o) if o.filled.get(s)]
        d = o.seg.get("_DATA", b"")[:o.filled.get("_DATA", 0)]
        self.data = (d, omf.holes(o, "_DATA")) if d else None
        self.pubs = {n: (s, off) for n, s, off in o.pubs}

    @property
    def label(self):
        return (self.tag + ":" if self.tag else "") + self.name

    def seglen(self, seg):
        for n, cls, ln, al in self.o.segs:
            if n == seg:
                return ln, {1: 1, 2: 2, 3: 16, 4: 256}.get(al, 1)
        return 0, 1


class Placed(object):
    def __init__(self, m, code=None, data=None, why=""):
        self.m, self.code, self.data, self.why = m, code, data, why
        self.also = []

    @property
    def end(self):
        return self.code + sum(len(b) for s, b, h in self.m.code[:1]) if self.code is not None else None


def candidates(prj):
    """[(tag, path)] the libraries to look in, in the order of preference"""
    from codebreaker.exact import toolchain
    out = []
    tc = prj.toolchain()
    model = prj.cfg.get("model", "S")
    for x in tc.p.get("libraries", {}).get(model, []):
        out.append((x.get("tag", ""), toolchain.kit_path(tc.p["kit"], x["path"])))
    for x in prj.cfg.get("libraries", []):
        out.append((x.get("tag", ""), toolchain.kit_path(None, x["path"], prj.dir)))
    return out


def modules(prj):
    out = []
    for rank, (tag, path) in enumerate(candidates(prj)):
        for name, raw in omf.library(open(path, "rb").read()):
            try:
                out.append(Module(tag, name, raw, rank))
            except Exception:                               # (a record this reader does not know: not a candidate)
                pass
    return out


def find(img, d, holes, lo, hi):
    """every place in img[lo:hi] where d stands, the holes left out"""
    runs = []
    k = 0
    while k < len(d):
        if k in holes:
            k += 1
            continue
        e = k
        while e < len(d) and e not in holes:
            e += 1
        runs.append((e - k, k))
        k = e
    if not runs:
        return []
    ln, at = max(runs)
    key = bytes(d[at:at + ln])
    out = []
    p = img.find(key, lo, hi)
    while p >= 0:
        s = p - at
        if s >= lo and s + len(d) <= hi and all(img[s + x] == d[x] for x in range(len(d)) if x not in holes):
            out.append(s)
        p = img.find(key, p + 1, hi)
    return out


class Map(object):
    pass


def locate(prj, verbose=False):
    img = prj.image
    dg = prj.dgroup
    syms = prj.symbols()
    code_at = {a: n for n, (t, a) in syms.items() if t == "C" and ":" not in n}
    data_at = {a: n for n, (t, a) in syms.items() if t == "D" and ":" not in n}
    mods = modules(prj)
    hits = {}                                               # code place -> [modules]
    multi = []
    dhits = {}
    for m in mods:
        if m.code:
            s, b, h = m.code[0]
            if len(b) - len(h) < 3:
                continue
            ps = find(img, b, h, 0, dg)
            if len(ps) == 1:
                hits.setdefault(ps[0], []).append(m)
            elif ps:
                multi.append((m, ps))
        if m.data and len(m.data[0]) - len(m.data[1]) >= 4:
            dhits[id(m)] = [p - dg for p in find(img, m.data[0], m.data[1], dg, len(img))]

    def callers_name(m, at):
        s0 = m.code[0][0]
        return any(code_at.get(at + off) == n for n, (s, off) in m.pubs.items() if s == s0)

    placed = {}
    for at, ms in hits.items():
        ms = sorted(ms, key=lambda m: (not callers_name(m, at), m.rank))
        dp = dhits.get(id(ms[0]), [])
        p = Placed(ms[0], at, dp[0] if len(dp) == 1 else None)
        p.also = ms[1:]
        placed[at] = p
    for m, ps in multi:                                     # (a module whose bytes stand in several places)
        free = [a for a in ps if a not in placed]
        named = [a for a in free if callers_name(m, a)]
        if len(named) == 1 or len(free) == 1:
            a = (named or free)[0]
            placed[a] = Placed(m, a, why="(by %s)" % ("its name in SYMBOLS" if named else "the one place left"))
    refs = _refs(prj, placed.values())
    # a module too short to be told by its bytes (a jump to another routine): placed by its names' places
    for m in mods:
        if not m.code or any(m in [p.m] + p.also for p in placed.values()):
            continue
        s0, b, h = m.code[0]
        if len(b) - len(h) >= 3 and not any(m is x for x, ps in multi):
            continue
        for n, (s, off) in m.pubs.items():
            src = "SYMBOLS" if n in syms and syms[n][0] == "C" else "a call of the library's" if n in refs and refs[n][0] == "C" else None
            if s != s0 or not src:
                continue
            a = (syms[n] if src == "SYMBOLS" else refs[n])[1] - off
            if a not in placed and a + len(b) <= len(img) and all(img[a + x] == b[x] for x in range(len(b)) if x not in h):
                placed[a] = Placed(m, a, why="(by %s, which %s names)" % (n, src))
                break
    coded = [placed[a] for a in sorted(placed)]
    # the library part: the run of modules at the end of the code
    gap = prj.cfg.get("library_gap", 2048)
    start = prj.cfg.get("library_start")
    start = int(start, 0) if isinstance(start, str) else start
    if start is None and coded:
        k = len(coded) - 1
        while k > 0 and coded[k].code - coded[k - 1].end <= gap:
            k -= 1
        start = coded[k].code
    coded = [p for p in coded if start is None or p.code >= start]
    refs = _refs(prj, coded)
    taken = {id(p.m) for p in coded} | {id(m) for p in coded for m in p.also}
    # modules of data only (by their _DATA, or another data segment's bytes: the 8087 fixups' CDATA)
    donly = []
    first = []
    for m in mods:
        if m.code or id(m) in taken:
            continue
        got = None
        dp = dhits.get(id(m), [])
        if len(dp) == 1:
            got = (dp[0], "")
        else:
            for n, (s, off) in m.pubs.items():
                if s != "_DATA":
                    continue
                a = syms[n][1] if n in syms and syms[n][0] == "D" else (refs[n][1] if n in refs and refs[n][0] == "D" else None)
                if a is None:
                    continue
                base = a - off
                ok = n in syms or m.data and all(dg + base + x < len(img) and img[dg + base + x] == m.data[0][x]
                                                 for x in range(len(m.data[0])) if x not in m.data[1])
                if ok:
                    got = (base, "(by %s, %04x%s)" % (n, a, " in SYMBOLS" if n in syms else ": a word of the code"))
                    break
        if got:
            donly.append(Placed(m, None, got[0], got[1]))
            taken.add(id(m))
            continue
        other = [(s, m.o.seg[s][:m.o.filled[s]], omf.holes(m.o, s)) for s, cls, ln, al in m.o.segs
                 if s != "_DATA" and m.o.filled.get(s)]
        if other and all(len(find(img, d, h, dg, len(img))) == 1 for s, d, h in other if len(d) - len(h) >= 4) \
                and any(len(d) - len(h) >= 4 for s, d, h in other):
            first.append(Placed(m, None, None, "(no code: its %s found in the data)" % ", ".join(s for s, d, h in other)))
            taken.add(id(m))
    # a module its bytes cannot tell (none, or zeros the linker fills) is taken
    #  - when a placed module names one of its publics and nothing placed (or the kit's objects linked first)
    #    defines it: the 8087 fixups' FIxRQQ, which the emulator names
    #  - when it names a placed module's public that nothing else placed names (it is what took that module):
    #    __fltused's module, which names fakcvt's routines
    defined = set(_first_object_names(prj))
    named = {}
    for p in coded + donly + first:
        for m in [p.m] + p.also:
            defined |= set(m.pubs)
        for e in p.m.o.ext:                                 # (of a group of the same bytes, the one taken)
            named.setdefault(e, []).append(p.m.label)
    byteless = lambda m: not m.code and not any(m.o.seg[s][:m.o.filled.get(s, 0)].strip(bytes(1)) for s in m.o.seg)
    for m in mods:
        if not byteless(m) or id(m) in taken or not m.pubs:
            continue
        why = [n for n in sorted(m.pubs) if n in named and n not in defined]
        if why:
            first.append(Placed(m, None, None, "(its bytes tell nothing: %s, which %s names)" % (why[0], named[why[0]][0])))
            taken.add(id(m))
            defined |= set(m.pubs)
    for p in coded + donly:
        own = {p.m.label} | {m.label for m in p.also}
        alone = [n for m in [p.m] + p.also for n in m.pubs if not set(named.get(n, [])) - own]
        for m in mods:
            if not byteless(m) or id(m) in taken or not m.pubs:
                continue
            took = [e for e in m.o.ext if e in alone and e not in m.pubs]
            if took:
                first.append(Placed(m, None, None, "(its bytes tell nothing: it took %s, for %s, which no other module names)" % (p.m.label, took[0])))
                taken.add(id(m))
    first.sort(key=lambda p: (p.m.rank, mods.index(p.m)))
    # names the placed modules need that nothing placed defines, and the library modules that would: another
    # version of those is in the program (a stand-in), or the original link left them unresolved too
    for p in first:
        defined |= set(p.m.pubs)
    missing = {}
    for n, who in sorted(named.items()):
        if n not in defined and not n.startswith(("FI", "FJ")):
            missing[n] = (who[0], sorted({m.label for m in mods if n in m.pubs}))
    # stand-ins for the gaps: not zeros (a pad, a segment of zeros), nor a placed module's far data
    far = []
    for p in coded:
        for s, cls, ln, al in p.m.o.segs:
            d = p.m.o.seg.get(s, b"")[:p.m.o.filled.get(s, 0)]
            if s not in [c[0] for c in p.m.code] and cls.upper() not in ("DATA", "BSS", "STACK", "CONST") and d.strip(bytes(1)):
                ps = find(img, d, omf.holes(p.m.o, s), start, dg)
                if len(ps) == 1:
                    far.append((ps[0], ps[0] + ln))
    raws = []
    if coded:
        end = prj.code_end or _code_end(img, dg)
        if far:                                             # (the classes after CODE - far data - begin there)
            end = min(end, min(a for a, b in far))
        segs = [p.m.code[0][0] for p in coded]
        main_seg = max(set(segs), key=segs.count)
        if segs[-1] != main_seg:                            # (the last is a code segment of its own, after the main
            end = min(end, coded[-1].end)                   #  one: what follows is of the classes after CODE)
        pubs_all = {n for q in coded for m in [q.m] + q.also for n in m.pubs}
        pos = start
        for p in coded + [None]:
            nxt = p.code if p else end
            if nxt > pos:
                lo = pos
                for a, b in sorted(far):
                    if a <= lo < b or lo < a < nxt and not img[lo:a].strip(bytes(1)):
                        lo = max(lo, b)
                hi = nxt
                while hi - lo > 1 and img[hi - 1] == 0:
                    hi -= 1                                 # (zeros before the next module: pads, not the stand-in's)
                if hi & 1 and hi < nxt and (lo & 1) == 0:
                    hi += 1                                 # (but a module's code of its own is of even length)
                if img[lo:nxt].strip(bytes(1)):
                    pos = lo
                    known = dict(refs)
                    known.update({n: v for n, v in syms.items() if ":" not in n})
                    pubs = sorted((a - pos, n) for n, (t, a) in known.items() if t == "C" and pos <= a < hi and n not in pubs_all)
                    raws.append((pos, hi, pubs))
            if p:
                pos = max(pos, p.end)
    mp = Map()
    mp.start, mp.coded, mp.donly, mp.first, mp.raws, mp.refs, mp.missing = start, coded, donly, first, raws, refs, missing
    mp.order = [(None, "first", p) for p in first] + _order(coded, donly, raws)
    return mp


def _first_object_names(prj):
    """the publics of the kit's objects the program links before the library (project "first_objects")"""
    from codebreaker.exact import toolchain
    tc = prj.toolchain()
    out = set()
    for x in prj.cfg.get("first_objects", []):
        o = omf.read(open(toolchain.kit_path(tc.p["kit"], x), "rb").read())
        out |= {n for n, s, off in o.pubs}
    return out


def _refs(prj, placed):
    """{external name: (C or D, address)}: what the words of the placed modules' code that name externals show"""
    img = prj.image
    refs = {}
    for p in placed:
        s0 = p.m.code[0][0]
        b = p.m.o.seg[s0]
        for seg, fo, kind, tgt, disp in p.m.o.fix:
            if seg != s0 or tgt[0] != "ext" or fo + 2 > len(b) or p.code + fo + 2 > len(img):
                continue
            mine = (W(b, fo) + disp) & 0xFFFF
            theirs = W(img, p.code + fo)
            if kind == "rel":
                base = prj.code_base(p.code)
                refs.setdefault(tgt[1], ("C", base + ((p.code + fo + 2 + theirs - base) & 0xFFFF) - mine))
            elif kind == "off":
                refs.setdefault(tgt[1], ("D", (theirs - mine) & 0xFFFF))
    return refs


def _code_end(img, dg):
    """where the code ends: before the data segment, less the zeros that pad to its paragraph"""
    e = dg
    while e > dg - 16 and e > 0 and img[e - 1] == 0:
        e -= 1
    return e


def _order(coded, donly, raws):
    """the modules in the linker's order. By code; a module of data only where its data stands among the coded
    modules' data: the library's data is laid out module after module (the lengths and alignments of their
    SEGDEFs), a data-only module going in where the layout reaches its place; of the starts for the library's
    data, the one that puts the most modules whose data's place is known at that place"""
    rows = [(p.code, "mod", p) for p in coded] + [(lo, "raw", (lo, hi, pubs)) for lo, hi, pubs in raws]
    rows.sort(key=lambda r: r[0])
    blocks = sorted(donly, key=lambda d: d.data)
    if not blocks:
        return rows

    def lay(s):
        out = []
        score = 0
        pos = s
        bi = 0
        for r in rows:
            ln, al = r[2].m.seglen("_DATA") if r[1] == "mod" else (0, 1)
            while bi < len(blocks) and blocks[bi].data <= pos + (pos & 1 if al == 2 else 0):
                out.append((None, "data", blocks[bi]))
                pos = max(pos, blocks[bi].data + blocks[bi].m.seglen("_DATA")[0])
                bi += 1
            if ln and al == 2 and pos & 1:
                pos += 1
            if r[1] == "mod" and r[2].data is not None and ln:
                score += r[2].data == pos
            out.append(r)
            pos += ln
        out += [(None, "data", b) for b in blocks[bi:]]
        return score, out
    known = [r[2].data for r in rows if r[1] == "mod" and r[2].data is not None] + [b.data for b in blocks]
    best = None
    for s in range(max(0, min(known) - 0x800), max(known) + 1):
        sc, out = lay(s)
        if best is None or sc > best[0]:
            best = (sc, out)
    return best[1]


def lines(mp, title=""):
    out = ["# %s" % title if title else "# the library's part of the program, in the order the linker took its modules",
           "# (codebreaker exact library): where each module's code (or data) stands in the program."]
    for a, kind, x in mp.order:
        if kind == "raw":
            lo, hi, pubs = x
            nm = pubs[0][1].lstrip("_$") if pubs else "lib%05x" % lo
            out.append("RAW %s code=%05x-%05x%s  # (no library module has these bytes)" % (
                nm, lo, hi, "".join(" pub=%s@%x" % (n, o) for o, n in pubs)))
            continue
        p = x
        note = []
        if p.code is not None:
            note.append("code %05x (%d bytes)" % (p.code, p.end - p.code))
        if p.data is not None:
            note.append("data %04x" % p.data)
        if p.why:
            note.append(p.why)
        group = sorted([p.m] + p.also, key=lambda m: m.label)  # (the same bytes: the build takes the one it needs)
        for m in group:
            if m is p.m:
                out.append("%-16s # %s" % (p.m.label, "  ".join(note)))
            else:
                out.append("%-16s # code %05x  (the same bytes as %s)" % (m.label, p.code, p.m.label))
    return out


def main(prj, argv):
    mp = locate(prj, verbose="-v" in argv)
    txt = "\n".join(lines(mp)) + "\n"
    print(txt)
    print("library part from %05x: %d modules with code, %d of data only, %d stand-ins (%d bytes)" % (
        mp.start or 0, len(mp.coded), len(mp.donly), len(mp.raws), sum(hi - lo for lo, hi, p in mp.raws)))
    for n, (who, by) in sorted(mp.missing.items()):
        if by:
            print("   needed, not in the program: %s (named by %s; defined by %s) - another version there, or an object" % (n, who, ", ".join(by)))
    own = sorted(n for n, (who, by) in mp.missing.items() if not by)
    if own:
        print("   the program's or the linker's (no library module defines them): " + ", ".join(own))
    if "--write" in argv:
        with open(prj.path("CRTLIB"), "w", newline="\n") as f:
            f.write(txt)
    return 0
