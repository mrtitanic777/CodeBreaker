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
    gap longer than `gap` bytes (project "library_gap", default 4096) before them - a lone module
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
    if getattr(prj, "_library", None) is None:
        prj._library = _locate(prj)
    return prj._library


def _locate(prj):
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
    gap = prj.cfg.get("library_gap", 4096)
    start = prj.cfg.get("library_start")
    start = int(start, 0) if isinstance(start, str) else start
    if start is None and coded:
        k = len(coded) - 1
        while k > 0 and coded[k].code - coded[k - 1].end <= gap:
            k -= 1
        start = coded[k].code
    lone = [p for p in coded if start is not None and p.code < start]   # (a library module among the program's own)
    lib_pubs = {}
    for p in coded:
        if start is not None and p.code >= start:
            for m in [p.m] + p.also:
                for n, (s, off) in m.pubs.items():
                    lib_pubs.setdefault(n, p.code + off)

    def calls_library(p):
        """a lone module counts when what it calls is in the library part (a wrapper of the program's own that has
        a library routine's bytes calls the program's code: P2's f05a85, fscanf's bytes)"""
        for n, (t, a) in _refs(prj, [p]).items():
            if t == "C" and (a < start or n in lib_pubs and lib_pubs[n] != a):
                return False
        return True
    lone = [p for p in lone if calls_library(p)]
    coded = [p for p in coded if start is None or p.code >= start]
    # every module of the same bytes at a place is one of its group (atoi and atol: one jump to __catox) ...
    inplace = {id(m) for p in coded for m in [p.m] + p.also}
    for p in coded:
        s0, b, h = p.m.code[0]
        for m in mods:
            if id(m) in inplace or not m.code or len(m.code[0][1]) != len(b):
                continue
            mb, mh = m.code[0][1], m.code[0][2]
            if mh == h and all(mb[k] == b[k] for k in range(len(b)) if k not in h):
                p.also.append(m)
                inplace.add(id(m))
    # ... and of a group the one taken is the one whose calls go where the modules they name are
    for p in coded:
        if not p.also:
            continue

        def agrees(m, p=p):
            n = 0
            for nm, (t, a) in _refs(prj, [Placed(m, p.code)]).items():
                if t == "C" and nm in lib_pubs:
                    n += 1 if lib_pubs[nm] == a else -1
            return n
        group = [p.m] + p.also
        best = max(group, key=lambda m: (agrees(m), m is p.m))
        if best is not p.m:
            p.also = [m for m in group if m is not best]
            p.m = best
            p.why = (p.why + " " if p.why else "") + "(of the same bytes, the one whose calls go where their names are)"
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
            same = [d for d in donly if d.data == got[0] and d.m.seglen("_DATA") == m.seglen("_DATA")]
            if same:                                        # (the same data in another library: one module)
                same[0].also.append(m)
            else:
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
        from codebreaker.exact import modules as mdl
        later = [a for a in mdl.starts(prj) if a > start]
        if later:                                           # (the program's own modules after the library: a code
            end = min(end, min(later))                      #  segment of their own, placed later by the linker)
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
    mp.lone = lone
    mp.allrefs = _refs(prj, coded + donly)
    mp.objects = _objects(prj, img, dg)
    mp.dataraws = _data_standins(prj, mods, taken, missing, mp.allrefs, coded, donly)
    for d in mp.dataraws:
        for n in d.pubs:
            missing.pop(n, None)
    mp.rawdata = _raw_data(prj, mods, raws, img[dg:])
    for lo, hi, pubs in raws:
        for o, n in pubs:
            missing.pop(n, None)
    mp.order = [(None, "first", p) for p in first] + _order(coded, donly + mp.dataraws, raws, mp.rawdata)
    for d in [d for d in mp.dataraws if d.by_next]:          # (the modules after the stand-in, before the next known
        k = [x[2] for x in mp.order].index(d)               #  data, have their _DATA there too: it ends before them)
        room = 0
        for a, kind, x in mp.order[k + 1:]:
            if getattr(x, "data", None) is not None:
                break
            if kind == "mod":
                ln, al = x.m.seglen("_DATA")
                room += ln + (room & 1 if ln and al == 2 else 0)
        d.end -= room
        d.pubs = {n: o for n, o in d.pubs.items() if o < d.end - d.data}
    return mp


class DataRaw(object):
    """a data-only module of another version than the kit's: its data as the program has it (a stand-in)"""
    def __init__(self, m, lo, hi, pubs, why):
        self.m, self.data, self.end, self.pubs, self.why = m, lo, hi, pubs, why
        self.code = None
        self.also = []

    def seglen(self, seg):
        return (self.end - self.data, 2) if seg == "_DATA" else (0, 1)


def _data_standins(prj, mods, taken, missing, refs, coded, donly):
    """for each data-only module whose publics the program needs but whose bytes are not the kit's: where its
    data stands (the publics' places, the words of the placed modules show them, less their offsets in the kit's
    module: the first agree), to the next data whose place is known; its publics: those placed by words, and
    those before the first that moved at the kit's offsets"""
    out = []
    starts = sorted({p.data for p in coded + donly if p.data is not None})
    for m in mods:
        if m.code or id(m) in taken or not m.data or not any(n in missing for n in m.pubs):
            continue
        kit = sorted((off, n) for n, (s, off) in m.pubs.items() if s == "_DATA")
        placed = [(off, n, refs[n][1]) for off, n in kit if n in refs and refs[n][0] == "D"]
        if not placed:
            continue
        lo = placed[0][2] - placed[0][0]
        span = m.seglen("_DATA")[0]                         # (a public placed far off is not in this version's module)
        placed = [(off, n, a) for off, n, a in placed if lo <= a < lo + span + 16]
        moved = min([off for off, n, a in placed if a - lo != off] or [1 << 16])
        shifts = {a - lo - off for off, n, a in placed if off >= moved}
        hi = min([s for s in starts if s > lo] or [1 << 16])
        by_next = True
        if len(shifts) <= 1:                                # (what moved, moved alike: the version's length is the
            own = lo + m.seglen("_DATA")[0] + (shifts.pop() if shifts else 0)        # kit's and that much)
            if own <= hi:
                hi, by_next = own, False
        pubs = {n: a - lo for off, n, a in placed if lo <= a < hi}
        for off, n in kit:                                  # (a public the words place elsewhere is not this one's)
            if off < moved and n not in pubs and n not in refs:
                pubs[n] = off
        why = "(another version of %s: its data as the program has it%s)" % (
            m.label, ", the publics from +%x on moved" % moved if moved < 1 << 16 else "")
        out.append(DataRaw(m, lo, hi, pubs, why))
        out[-1].by_next = by_next
        taken.add(id(m))
    # a needed name the words place where no module's data is (another version keeps it in a module of its own):
    # a stand-in of its own, as long as the kit's module gives it (the link's drift map tells if it is more)
    covered = [(d.data, d.end) for d in out]
    have = {n for d in out for n in d.pubs}
    sizes = prj.cfg.get("data_sizes", {})
    for n in sorted(missing):
        if n in have or n not in refs or refs[n][0] != "D":
            continue
        a = refs[n][1]
        if any(lo <= a < hi for lo, hi in covered):
            continue
        by = [m for m in mods if n in m.pubs and m.pubs[n][0] == "_DATA"]
        if not by:
            continue
        m = by[0]
        offs = sorted(off for nm, (s, off) in m.pubs.items() if s == "_DATA")
        off = m.pubs[n][1]
        size = int(sizes.get(n, min([o for o in offs if o > off] + [m.seglen("_DATA")[0]]) - off))
        d = DataRaw(m, a, a + size, {n: 0}, "(%s, which %s defines in the kit: here in a module of its own; %d bytes%s)" % (
            n, m.label, size, "" if n in sizes else ", a guess"))
        d.by_next = False
        d.m = _Named(n.lstrip("_"), m)
        out.append(d)
    # zeros of the data no module accounts for, which the link's fit found (project "data_pads": {DS: length})
    for at, ln in sorted(prj.cfg.get("data_pads", {}).items()):
        a = int(at, 16)
        d = DataRaw(None, a, a + int(ln), {}, "(zeros no module of the kit accounts for: a pad the link's fit found)")
        d.by_next = False
        d.m = _Named("pad%04x" % a, None)
        out.append(d)
    return out


class _Named(object):
    """a kit module's stand-in under another name (the name of the one public it keeps)"""
    def __init__(self, name, m):
        self.name, self.label, self.pubs, self.raw = name, name, {}, m.raw if m else b""

    def seglen(self, seg):
        return (0, 1)


def _objects(prj, img, dg):
    """[(code start, end, object)] the kit's loose objects (the profile's "objects" for the model: SVARSTCK.OBJ,
    SSETARGV.OBJ ...) whose code stands in the program at one place"""
    from codebreaker.exact import toolchain
    tc = prj.toolchain()
    out = []
    for x in tc.p.get("objects", {}).get(prj.cfg.get("model", "S"), []):
        try:
            m = Module("", x.split("/")[-1], open(toolchain.kit_path(tc.p["kit"], x), "rb").read(), -1)
        except OSError:
            continue
        if m.code and len(m.code[0][1]) - len(m.code[0][2]) >= 3:
            ps = find(img, m.code[0][1], m.code[0][2], 0, dg)
            if len(ps) == 1:
                out.append((ps[0], ps[0] + len(m.code[0][1]), m.label))
    return out


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
    """{external name: (C or D, address)}: what the words of the placed modules that name externals show - in
    their code, and in their data where its place is known (stdio's _iob names the buffers)"""
    img = prj.image
    refs = {}
    for p in placed:
        if p.code is not None:
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
        if p.data is not None:
            b = p.m.o.seg.get("_DATA", b"")
            for seg, fo, kind, tgt, disp in p.m.o.fix:
                at = prj.dgroup + p.data + fo
                if seg == "_DATA" and tgt[0] == "ext" and kind == "off" and fo + 2 <= len(b) and at + 2 <= len(img):
                    refs.setdefault(tgt[1], ("D", (W(img, at) - ((W(b, fo) + disp) & 0xFFFF)) & 0xFFFF))
    return refs


def _code_end(img, dg):
    """where the code ends: before the data segment, less the zeros that pad to its paragraph"""
    e = dg
    while e > dg - 16 and e > 0 and img[e - 1] == 0:
        e -= 1
    return e


def _raw_data(prj, mods, raws, ds):
    """{code stand-in's start: (data from, to, {public: offset}, note)} - a code stand-in is another version of the
    kit's module that defines its publics; that module's data, as far as the program has it (placed by its first
    run of bytes that are not zeros, as long as the bytes agree, its zeros at the end left out), is the stand-in's"""
    out = {}
    for lo, hi, pubs in raws:
        for o, n in pubs:
            for m in mods:
                if n not in m.pubs or not m.data:
                    continue
                kd, kh = m.data
                r0 = next((k for k in range(len(kd)) if kd[k] and k not in kh), None)
                if r0 is None:
                    continue
                r1 = r0
                while r1 < len(kd) and kd[r1] and r1 not in kh and r1 - r0 < 12:
                    r1 += 1
                if r1 - r0 < 4:
                    continue
                best = None
                p = ds.find(kd[r0:r1])
                while p >= 0:
                    base = p - r0
                    k = 0
                    while base >= 0 and k < len(kd) and base + k < len(ds) and (k in kh or ds[base + k] == kd[k]):
                        k += 1
                    if base >= 0 and (best is None or k > best[1]):
                        best = (base, k)
                    p = ds.find(kd[r0:r1], p + 1)
                if not best or best[1] < r1:
                    continue
                base, k = best
                while k > r1 and kd[k - 1] == 0:
                    k -= 1
                k += k & 1
                dp = {nm: off for nm, (s, off) in m.pubs.items() if s == "_DATA" and off < k}
                out[lo] = (base, base + k, dp, "the data of %s as far as the program has it" % m.label)
                break
            if lo in out:
                break
    # a stand-in no kit module's data fits: the data its own instructions address, where no placed module's is
    from codebreaker.exact import x86
    import re as _re
    img = prj.image
    for lo, hi, pubs in raws:
        if lo in out:
            continue
        hits = set()
        for i in x86.disasm(img, lo, hi):
            for v in _re.findall(r"\[0x([0-9a-f]{3,4})\]", i.op_str):
                hits.add(int(v, 16))
        known = prj.symbols()
        hits = sorted(h for h in hits if not any(t == "D" and a == h for n, (t, a) in known.items() if ":" not in n))
        runs = []
        for h in hits:
            if runs and h - runs[-1][1] <= 2:
                runs[-1][1] = h + 2
            else:
                runs.append([h, h + 2])
        runs = [r for r in runs if r[1] - r[0] <= 16]
        if len(runs) == 1:
            out[lo] = (runs[0][0], runs[0][1], {}, "its own data, which its instructions address")
    return out


def _order(coded, donly, raws, rawdata=None):
    """the modules in the linker's order. By code; a module of data only where its data stands among the coded
    modules' data: the library's data is laid out module after module (the lengths and alignments of their
    SEGDEFs), a data-only module going in where the layout reaches its place; of the starts for the library's
    data, the one that puts the most modules whose data's place is known at that place"""
    rawdata = rawdata or {}
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
            if r[1] == "mod":
                ln, al = r[2].m.seglen("_DATA")
            else:
                rd = rawdata.get(r[0])
                ln, al = ((rd[1] - rd[0]), 2) if rd else (0, 1)
            while bi < len(blocks) and blocks[bi].data <= pos + (pos & 1 if al == 2 else 0):
                out.append((None, "data", blocks[bi]))
                b = blocks[bi]
                pos = max(pos, b.data + (b.seglen("_DATA") if isinstance(b, DataRaw) else b.m.seglen("_DATA"))[0])
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
            rd = getattr(mp, "rawdata", {}).get(lo)
            dtxt = (" data=%04x-%04x" % (rd[0], rd[1]) + "".join(" dpub=%s@%x" % (n, o) for n, o in sorted(rd[2].items(), key=lambda kv: kv[1]))) if rd else ""
            out.append("RAW %s code=%05x-%05x%s%s  # (no library module has these bytes%s)" % (
                nm, lo, hi, "".join(" pub=%s@%x" % (n, o) for o, n in pubs), dtxt, "; " + rd[3] if rd else ""))
            continue
        if isinstance(x, DataRaw):
            out.append("RAW %s data=%04x-%04x%s  # %s" % (x.m.name, x.data, x.end, "".join(
                " dpub=%s@%x" % (n, o) for n, o in sorted(x.pubs.items(), key=lambda kv: (kv[1], kv[0]))), x.why))
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
                out.append("%-16s # %s  (the same bytes as %s)" % (
                    m.label, "code %05x" % p.code if p.code is not None else "data %04x" % p.data, p.m.label))
    return out


def main(prj, argv):
    mp = locate(prj, verbose="-v" in argv)
    txt = "\n".join(lines(mp)) + "\n"
    print(txt)
    for p in mp.lone:
        print("   among the program's own code: %s at %05x (%d bytes)" % (p.m.label, p.code, p.end - p.code))
    for a, b, nm in mp.objects:
        print("   the kit's object %s at %05x (%d bytes)" % (nm, a, b - a))
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
