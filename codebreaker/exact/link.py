"""link.py -- the program linked from its sources by the period linker, and compared with the original file.

    python -m codebreaker exact PROJECT link [-v] [--keep DIR] [--map]

What goes in, in this order:
    src/FIRSTOBJ   the kit's objects linked before the program's own (one a line)
    src/MODULES    the program's modules (exact/modules.py): compiled whole, a function not yet the
                   program's made a stand-in of its bytes (exact/standin.py); NAME.OBJ: the kit's
    src/BSS        the variables without a value at their places: "range LO HI", "hole ADDR LEN" (a
                   library module's own _BSS between), "name ADDR"; an object before the library
                   part, one after each hole
    src/CRTLIB     the library's modules in the program's order (exact/library.py): those the rest
                   needs are linked as objects; RAW lines are the program's bytes
    src/LINKOPT    the linker's switches;  src/EXEMOD  the kit's EXEMOD run after, one line each

The linker and the files it is run with are the toolchain profile's "link" part:
    {"kit": kit, "run": "disk3/LINK.EXE", "dir": kit folder of libraries and objects,
     "libs": {model: [library files]}, "exemod": "disk3/EXEMOD.EXE", "psp": paragraph}

The result is compared with the original: header, relocation table, load image; where they part,
the first difference is put to the stage that makes that part (a function's code, the data, the
library's part, the variables without a value, the header).
"""
import os
import re
import shutil
import struct
import tempfile

from codebreaker.exact import library, modules, standin
from codebreaker.formats import omf
from codebreaker.exact import toolchain as tcm


def _lp(prj):
    tc = prj.toolchain()
    lk = tc.p.get("link")
    if not lk:
        raise SystemExit("the toolchain %s has no linker (its profile's \"link\")" % tc.name)
    return tc, lk


def _kitfile(tc, lk, path):
    return tcm.kit_path(lk.get("kit", tc.p["kit"]), path)


def objects(prj, verbose=False):
    """[(name, bytes)] of the program's own modules, [stand-ins]"""
    tc, lk = _lp(prj)
    rows = modules.rows(prj)
    starts = sorted(a for m, a, e, d, x in rows if a is not None) + [_library_start(prj)]
    if prj.cfg.get("link_order") == "name":                 # (the objects named to the linker in the order of their
        rows = sorted(rows, key=lambda r: os.path.splitext(r[0])[0].lower())     # names - the data segment shows it)
    out = []
    stand = []
    tcs = [t for lb, t in prj.toolchains()]
    for row in rows:
        mod, a, e, d, x = row
        title = os.path.splitext(mod)[0].upper()
        if mod.upper().endswith(".OBJ"):
            out.append((title, open(_kitfile(tc, lk, os.path.join(lk["dir"], mod.upper())), "rb").read()))
            continue
        raw, con, addrs = modules.compile_module(prj, row, tcs[0])
        if raw is None:
            raise SystemExit("%s does not compile:\n%s" % (mod, con[-1500:]))
        if a is not None and addrs:
            alts = []
            for t in tcs[1:]:
                r2, c2, _ = modules.compile_module(prj, row, t)
                if r2 is not None:
                    alts.append(r2)
            end = min([s for s in starts if s > a] + ([e] if e else []))
            raw, st = standin.rebuild(prj, raw, addrs, end, title, alts)
            stand += st
            if verbose and st:
                print("   %s: stand-ins %s" % (mod, " ".join(st)))
        if a is not None and prj.cfg.get("recut"):          # (its code's records cut as the program's relocations show)
            from codebreaker.exact import recut
            raw = recut.recut(prj, raw)
        out.append((title, raw))
    return out, stand


def _library_start(prj):
    """where the program's own code ends: the first code place src/CRTLIB names (a pad before the library is the
    last module's when CRTLIB has no RAW piece for it), else the end carve finds"""
    p = prj.path("CRTLIB")
    at = []
    if os.path.exists(p):
        for l in open(p):
            at += [int(x, 16) for x in re.findall(r"\bcode[= ]([0-9a-f]{5})\b", l)]
    return min(at) if at else prj.own_end()


def first_objects(prj):
    tc, lk = _lp(prj)
    p = prj.path("FIRSTOBJ")
    if not os.path.exists(p):
        return []
    out = []
    for l in open(p):
        f = l.split("#")[0].strip()
        if f:
            out.append((os.path.splitext(f)[0], open(_kitfile(tc, lk, os.path.join(lk["dir"], f)), "rb").read()))
    return out


def bss_objects(prj):
    p = prj.path("BSS")
    if not os.path.exists(p):
        return []
    names, rng, holes, seg = {}, None, [], None
    for l in open(p):
        f = l.split("#")[0].split()
        if not f:
            continue
        if f[0] == "segment":                               # (communal names in a segment of their own: after all)
            seg = (f[1], "para" in f[2:])
        elif f[0] == "range":
            rng = (int(f[1], 16), int(f[2], 16))
        elif f[0] == "hole":
            holes.append((int(f[1], 16), int(f[2], 16)))
        else:
            names[f[0]] = int(f[1], 16)
    cuts = [rng[0]] + [x for a, n in holes for x in (a, a + n)] + [rng[1]]
    out = []
    for k in range(0, len(cuts), 2):
        lo, hi = cuts[k], cuts[k + 1]
        nm = "bssvar%d" % (k // 2)
        out.append((nm, omf.bss_object({n: a for n, a in names.items() if lo <= a < hi}, lo, hi, nm,
                                       seg[0] if seg else "_BSS", seg[1] if seg else False)))
    bss_objects.after = bool(seg)
    return out


def crt(prj, objs):
    """the library's modules the rest needs, as objects in the program's order (src/CRTLIB); names left unresolved"""
    p = prj.path("CRTLIB")
    crt.conflicts = []                                      # (not the last call's)
    if not os.path.exists(p):
        return [], []
    mods = {m.label: m.raw for m in library.modules(prj)}
    img = prj.image
    dg = prj.dgroup
    order, always = [], []
    group = {}                                              # (modules of the same bytes at one place: one is taken)
    proven = set()
    for l in open(p):
        if l.startswith("#"):
            continue
        m = re.search(r"\s#", l)                            # (the note begins at " #": a name may hold a #, NAME#2)
        body, note = (l[:m.start()], l[m.end():]) if m else (l, "")
        f = body.split()
        if not f:
            continue
        if f[0].lower().endswith(".asm") and ":" not in f[0] and f[0] not in mods:     # (a transcription of the
            from codebreaker.exact import modules as mdl                                #  program's bytes: src/crt)
            part, tc = mdl.set_of(prj, f[0])
            raw, con = tc.compile(open(os.path.join(prj.src, "crt", f[0]), "rb").read(), headers=part.headers(), include=part.include)
            if raw is None:
                raise SystemExit("crt/%s does not assemble:\n%s" % (f[0], con[-600:]))
            if prj.cfg.get("recut"):
                from codebreaker.exact import recut
                raw = recut.recut(prj, raw)
            mods[f[0]] = raw
        if len(f) > 1 and f[1].startswith("!"):             # TAG:NAME !SEG+OFF=HEX: a byte or two of its data as the
            key = f[0] + " " + f[1]                         #  program has them (the version linked had them)
            if key not in mods and f[0] in mods:
                mods[key] = omf.patched(mods[f[0]], f[1][1:])
            f = [key]
        mc = re.search(r"\bcode ([0-9a-f]{5})\b", note)
        if not mc and re.match(r"\s*([0-9a-f]{5}\b|\(data only)", note) and f[0] != "RAW":    # (a place alone, or
            always.append(f[0])                                 #  "data only": a module written down as found in the program)
        md = re.match(r"\s*data [0-9a-f]{4}\b", note)
        if f[0] != "RAW" and (mc or md):                    # (found in the program by its bytes: it is there)
            ms = group.setdefault(mc.group(1) if mc else "data:" + f[0], [])
            if "the same bytes as" in note:                 # (the one the library stage took goes first)
                ms.append(f[0])
            else:
                ms.insert(0, f[0])
            if "the one whose calls go where" in note:      # (told apart by their calls: that one is the program's,
                proven.add(f[0])                            #  whatever the objects name)
        if f[0] == "RAW":                                   # RAW NAME code=LO-HI [data=LO-HI] [pub=N@OFF ..] [dpub=..]
            kv = {}
            for x in f[2:]:
                k, _, v = x.partition("=")
                kv.setdefault(k, []).append(v)
            rng = lambda s: tuple(int(x, 16) for x in s.split("-"))
            c0, c1 = rng(kv["code"][0]) if "code" in kv else (0, 0)     # (a stand-in of data only)
            d0, d1 = rng(kv["data"][0]) if "data" in kv else (0, 0)
            ps = lambda vs: {y.split("@")[0]: int(y.split("@")[1], 16) for v in vs for y in v.split(",") if y}
            mods[f[1]] = omf.raw_object(f[1], img[c0:c1], ps(kv.get("pub", [])), img[dg + d0:dg + d1] if d1 else b"",
                                        ps(kv.get("dpub", [])), dalign=2 if d1 and d0 % 2 == 0 else 1)
            order.append(f[1])
            if "pub" not in kv and "dpub" not in kv:
                always.append(f[1])                         # (a piece no name calls for: always taken)
            continue
        order.append(f[0])
    unknown = [m for m in order if m not in mods]
    if unknown:
        raise SystemExit("CRTLIB names modules no library of the profile has: " + " ".join(unknown))
    have, need = set(), set()
    for n, raw in objs:
        pb, e = omf.names_of(raw)
        have |= pb
        need |= e
    take = []

    def add(m):
        nonlocal have, need
        take.append(m)
        pb, e = omf.names_of(mods[m])
        have |= pb
        need |= e

    others = {m for ms in group.values() if any(x in proven for x in ms) for m in ms if m not in proven}

    def close():
        while True:
            miss = need - have
            nxt = next((m for m in order if m not in take and m not in others and omf.names_of(mods[m])[0] & miss), None)
            if not nxt:
                return
            add(nxt)
    for m in always:
        add(m)
    close()
    # what the library stage found by its bytes is in the program: a group of the same bytes none of whose
    # members anything names is taken all the same (its first: a module only a stand-in calls)
    for ms in group.values():
        if not any(m in take for m in ms):
            add(ms[0])
            close()
    crt.conflicts = []
    for at, ms in group.items():                            # (the program has one routine where ours names two)
        both = [m for m in ms if m in take]
        if len(both) > 1:
            crt.conflicts.append("the program has one routine at %s for %s; ours names %s - the sources call it by "
                                 "another name than the program's (a name the library's modules use is the program's)" % (
                                     at, " and ".join(both), " and ".join(sorted(min(omf.names_of(mods[m])[0]) for m in both))))
    return [(m, mods[m]) for m in order if m in take], sorted(need - have)


def dosname(n):
    return re.sub(r"[^A-Z0-9]", "", n.upper())[:8] or "M"


def run_link(prj, objs, keep=None, opts=""):
    from codebreaker.exact.emu import dos
    tc, lk = _lp(prj)
    w = keep or tempfile.mkdtemp(prefix="exlink_")
    os.makedirs(w, exist_ok=True)
    try:
        shutil.copy(_kitfile(tc, lk, lk["run"]), os.path.join(w, "LINK.EXE"))
        libs = lk.get("libs", {}).get(prj.cfg.get("model", "S"), [])
        if os.path.exists(prj.path("CRTLIB")):              # (the library's part goes in as objects: a name left open
            libs = []                                       #  - one only a stand-in used - must not pull a module in)
        for l in set(libs) | set(lk.get("copy", [])):
            shutil.copy(_kitfile(tc, lk, os.path.join(lk["dir"], l)), w)
        names = []
        for n, b in objs:
            dn = dosname(n)
            k = 1
            while dn in names:                              # (DOS names: _exit and exit)
                dn = dosname(n)[:6] + "%02d" % k
                k += 1
            names.append(dn)
            open(os.path.join(w, dn + ".OBJ"), "wb").write(b)
        lines = ["+".join(names[k:k + 6]) + ("+" if k + 6 < len(names) else "") for k in range(0, len(names), 6)]
        exe = "OUT.EXE"
        rsp = "\r\n".join(lines) + "\r\n%s\r\nOUT.MAP\r\n%s%s;\r\n" % (exe, "+".join(libs), opts)
        open(os.path.join(w, "L.LNK"), "wb").write(rsp.encode())
        env = b"PATH=C:\\\x00TMP=C:\\\x00"
        psp = lk.get("psp", "0x1000")
        psp = int(psp, 0) if isinstance(psp, str) else psp
        con = dos.run(os.path.join(w, "LINK.EXE"), w, "@L.LNK", env=env, psp=psp).console.decode("latin1").replace("\r", "")
        mf = prj.path("EXEMOD")
        if os.path.exists(os.path.join(w, exe)) and os.path.exists(mf) and lk.get("exemod"):
            shutil.copy(_kitfile(tc, lk, lk["exemod"]), os.path.join(w, "EXEMOD.EXE"))
            for l in open(mf):                              # (the header changed after the link, by the kit's EXEMOD:
                a = l.split("#")[0].strip()                 #  the checksum stays the link's)
                if a:
                    con += dos.run(os.path.join(w, "EXEMOD.EXE"), w, exe + " " + a, env=env, psp=psp).console.decode("latin1")
        b = open(os.path.join(w, exe), "rb").read() if os.path.exists(os.path.join(w, exe)) else None
        mp = open(os.path.join(w, "OUT.MAP"), encoding="latin1").read() if os.path.exists(os.path.join(w, "OUT.MAP")) else ""
        return b, con, mp
    finally:
        if not keep:
            shutil.rmtree(w, ignore_errors=True)


def parts(exe):
    h = struct.unpack_from("<14H", exe, 0)
    size = h[2] * 512 - ((512 - h[1]) if h[1] else 0)
    rel = [struct.unpack_from("<HH", exe, h[12] + 4 * k) for k in range(h[3])]
    return h, exe[h[4] * 16:size], rel


FIELDS = "magic lastpage pages relocs hdrparas minalloc maxalloc ss sp csum ip cs reloctab ovl".split()


def diagnose(prj, ref, exe, mp=None):
    """-> [lines]: where ours parts from the original, and which stage makes that part"""
    out = []
    hb, ib, rb = parts(exe)
    if ref is None:                                         # (the original packed: ours, linked unpacked, against the
        ha, ia = None, prj.image[:prj.packed.dest * 16]     #  unpacked image - its segment words as loaded at loadseg -
        ra = list(prj.packed.relocs)                        #  and the relocations EXEPACK kept, in their order)
        rb = [s * 16 + o for o, s in rb]
        ib = bytearray(ib)
        for a in rb:
            if a + 2 <= len(ib):
                struct.pack_into("<H", ib, a, (struct.unpack_from("<H", ib, a)[0] + prj.loadseg) & 0xFFFF)
        ib = bytes(ib) + bytes(max(0, len(ia) - len(ib)))  # (its variables: zeros in the unpacked image)
        if prj.packed.ss * 16 + prj.packed.sp != hb[7] * 16 + hb[8]:
            out.append("the stack: theirs %04x:%04x, ours %04x:%04x (LINKOPT /STACK, the variables' length)" % (
                prj.packed.ss, prj.packed.sp, hb[7], hb[8]))
    else:
        ha, ia, ra = parts(ref)
    for f, x, y in zip(FIELDS, ha or (), hb):
        if x != y and f != "csum":
            out.append("header %-8s theirs %04x ours %04x%s" % (f, x, y, "  (EXEMOD, LINKOPT: /STACK)" if f in ("minalloc", "maxalloc", "sp") else ""))
    if sorted(ra) != sorted(rb):
        out.append("the relocations differ (theirs %d, ours %d): the program's or the library's far references" % (len(ra), len(rb)))
    elif ra != rb:
        out.append("the relocations are the same but in another order: the order of the objects and their records (fixups first, CRTLIB)")
    if len(ia) != len(ib):
        out.append("the image is %d bytes, ours %d" % (len(ia), len(ib)))
    bad = [x for x in range(min(len(ia), len(ib))) if ia[x] != ib[x]]
    if bad:
        first = bad[0]
        dg = prj.dgroup
        own = prj.own_end()
        lib0 = library.locate(prj).start or own
        where = ("the program's own code (a module's functions or their order: MODULES)" if first < min(own, lib0) else
                 "the library's part (CRTLIB: a module, its order, a stand-in)" if first < dg else
                 "the data segment (the modules' data: dADDR.c, the library's data, BSS)")
        runs = []
        s = p = bad[0]
        for x in bad[1:]:
            if x > p + 8:
                runs.append((s, p))
                s = x
            p = x
        runs.append((s, p))
        out.append("%d bytes of the image differ, in %d places; the first at %05x: %s" % (len(bad), len(runs), first, where))
        shift = None
        for d in range(-64, 65):                            # (ours the same bytes, moved: something before is too long)
            if d and all(0 <= first + d + k < len(ib) and first + k < len(ia) and ib[first + d + k] == ia[first + k] for k in range(32)):
                shift = d
                break
        if shift:
            out.append("   from there ours is the original moved by %+d bytes: what comes before it is %d bytes %s" % (
                shift, abs(shift), "too long" if shift > 0 else "too short"))
        for a, b in runs[:12]:
            out.append("   %05x..%05x  theirs %s  ours %s" % (a, b, ia[a:min(b + 1, a + 10)].hex(), ib[a:min(b + 1, a + 10)].hex()))
        # the data segment on its own (a shift there moves every word of the code that points into it)
        da, db = ia[dg:], ib[dg:] if len(ib) > dg else b""
        dbad = next((k for k in range(min(len(da), len(db))) if da[k] != db[k]), None)
        if dbad is not None:
            who = [r for r in modules.rows(prj) if r[3] is not None and r[3] <= dbad]
            mod = max(who, key=lambda r: r[3])[0] if who else "?"
            lib = [x for x in library.locate(prj).order if getattr(x[2], "data", None) is not None and x[2].data <= dbad]
            lname = max(lib, key=lambda x: x[2].data)[2].m.label if lib else None
            out.append("the data segment parts at DS:%04x (after the module %s%s)" % (
                dbad, mod, (", the library's %s" % lname) if lname and (not who or max(lib, key=lambda x: x[2].data)[2].data > max(who, key=lambda r: r[3])[3]) else ""))
            for d in range(-64, 65):
                if d and all(0 <= dbad + d + k < len(db) and dbad + k < len(da) and db[dbad + d + k] == da[dbad + k] for k in range(24)):
                    out.append("   from there ours is the program's moved by %+d: what comes before is %d bytes %s" % (
                        d, abs(d), "too long" if d > 0 else "too short"))
                    break
            out.append("   theirs %s\n   ours   %s" % (da[dbad:dbad + 24].hex(), db[dbad:dbad + 24].hex()))
            out += ["   " + l for l in drift(da, db)]
    return out


def drift(a, b, step=2, span=12, reach=160):
    """where b runs ahead of or behind a: [lines] 'DS:XXXX on, ours is +N' at each change (runs of few distinct
    bytes - zeros, a table of one value - say nothing and are skipped)"""
    out = []
    prev = "?"
    for k in range(0, len(a) - span, step):
        seg = a[k:k + span]
        if len(set(seg)) <= 3:
            continue
        d = None
        for dd in [0] + [x for r in range(1, reach // 2) for x in (2 * r, -2 * r)]:
            if k + dd >= 0 and b[k + dd:k + dd + span] == seg:      # (a place before the start is none: a negative index would wrap)
                d = dd
                break
        if d is None:
            continue                                        # (a word that points elsewhere: not a move)
        if d != prev:
            out.append("DS:%04x on, ours is %+d" % (k, d))
            prev = d
    return out[:16]


def opts_of(prj):
    """the linker's switches (src/LINKOPT)"""
    lf = prj.path("LINKOPT")
    return " ".join(l.split("#")[0].strip() for l in open(lf) if l.split("#")[0].strip()) if os.path.exists(lf) else ""


def build(prj, keep=None, verbose=False, quiet=False, opts=None):
    """-> (the linked file or None, LINK's words, its map, [stand-ins])"""
    objs, stand = objects(prj, verbose)
    objs = first_objects(prj) + objs
    bss = bss_objects(prj)
    after = []
    for f in prj.cfg.get("after_library", []):              # (whole files linked after the library: a module of the
        from codebreaker.exact import modules as mdl        #  communal variables at their places, as c_common is last)
        part, tc = mdl.set_of(prj, f)
        raw, con = tc.compile(open(os.path.join(prj.src, f), "rb").read(), headers=part.headers(), include=part.include)
        if raw is None:
            raise SystemExit("%s does not compile:\n%s" % (f, con[-600:]))
        after.append((os.path.splitext(f)[0], raw))
    lib, miss = crt(prj, objs + bss + after)
    comm = set()                                            # (communal names are the linker's to place, and _edata,
    for n, b in objs + bss + after + lib:                   #  _end its own: not left open)
        comm |= set(omf.read(b).comm)
    miss = [m for m in miss if m not in comm and m not in prj.toolchain().p.get("link", {}).get("names", ["_edata", "_end"])]
    if miss and not quiet:
        print("   not resolved: " + " ".join(miss))
    for c in getattr(crt, "conflicts", []) if not quiet else []:
        print("   " + c)
    objs = objs + (lib + bss if getattr(bss_objects, "after", False) else bss[:1] + lib + bss[1:])
    objs += after
    opts = opts_of(prj) if opts is None else opts
    exe, con, mp = run_link(prj, objs, keep, opts)
    return exe, con, mp, stand


def fit(prj, rounds=8):
    """the data stand-ins' lengths fitted by the link: each round the first place where ours runs behind or ahead
    of the program in the data segment is put to the stand-in of a guessed length just before it, which is made
    that much longer or shorter (project.json "data_sizes", kept). -> the lines of what was done"""
    import json
    out = []
    for r in range(rounds):
        prj._library = None
        with open(prj.path("CRTLIB"), "w", newline="\n") as f:      # (the library stage's, with the sizes so far)
            f.write("\n".join(library.lines(library.locate(prj))) + "\n")
        exe, con, mp, stand = build(prj, quiet=True)
        if exe is None:
            out.append("round %d: no program was written" % r)
            break
        dg = prj.dgroup
        moves = [(int(m.group(1), 16), int(m.group(2))) for l in drift(prj.image[dg:], parts(exe)[1][dg:])
                 for m in [re.match(r"DS:([0-9a-f]+) on, ours is ([+-]\d+)", l)] if m]
        first = next(((p, d) for p, d in moves if d), None)
        if first is None:
            out.append("round %d: the data segment agrees" % r)
            break
        p, d = first
        lib = library.locate(prj)
        sizes = prj.cfg.get("data_sizes", {})
        cands = [x for x in lib.dataraws if x.end <= p <= x.end + 32 and ("a guess" in x.why or any(n in sizes for n in x.pubs))]
        if not cands:
            ds = prj.image[dg:]
            q = p
            while q > 0 and ds[q - 1] == 0:
                q -= 1
            if d < 0 and p - q >= -d:                       # (zeros no module accounts for: a pad, where they begin)
                at = q + (q & 1)
                cfg = json.load(open(os.path.join(prj.dir, "project.json")))
                cfg.setdefault("data_pads", {})["%04x" % at] = -d
                with open(os.path.join(prj.dir, "project.json"), "w") as f:
                    json.dump(cfg, f, indent=1)
                prj.cfg = cfg
                out.append("round %d: at DS:%04x ours is %+d, after %d zeros: a pad of %d bytes at DS:%04x" % (r, p, d, p - q, -d, at))
                continue
            out.append("round %d: at DS:%04x ours is %+d, and no stand-in of a guessed length is before it" % (r, p, d))
            break
        x = max(cands, key=lambda x: x.end)
        name = list(x.pubs)[0]
        size = x.end - x.data - d
        cfg = json.load(open(os.path.join(prj.dir, "project.json")))
        cfg.setdefault("data_sizes", {})[name] = size
        with open(os.path.join(prj.dir, "project.json"), "w") as f:
            json.dump(cfg, f, indent=1)
        prj.cfg = cfg
        out.append("round %d: at DS:%04x ours is %+d: %s's stand-in made %d bytes" % (r, p, d, name, size))
        with open(prj.path("CRTLIB"), "w", newline="\n") as f:
            prj._library = None
            f.write("\n".join(library.lines(library.locate(prj))) + "\n")
    return out


def main(prj, argv):
    from codebreaker.exact import linkwhole
    if linkwhole.wanted(prj):                               # (a program of whole source files: exact/linkwhole.py)
        return linkwhole.main(prj, argv)
    keep = argv[argv.index("--keep") + 1] if "--keep" in argv else None
    v = "-v" in argv
    if "--fit" in argv:
        for l in fit(prj):
            print("   " + l)
    exe, con, mp, stand = build(prj, keep, v)
    msgs = [l for l in con.split("\n") if l.strip() and "Microsoft" not in l and "Copyright" not in l]
    for l in msgs[:20]:
        print("   LINK: " + l)
    if "--map" in argv:
        print(mp)
    if exe is None:
        print("no program was written")
        return 1
    ref = prj.packed_file if prj.packed else prj.file      # (an unpacked image: the file is held to the packed one)
    same = exe == ref
    diag = []
    if not same and prj.packed:                             # (where ours parts from it: linked again, not packed, and
        un = re.sub(r"\s*/E(XEPACK|XE|X)?\b", "", opts_of(prj), flags=re.I)   #  held to the unpacked image)
        e2, c2, m2, s2 = build(prj, keep, False, quiet=True, opts=un)
        if e2:
            diag = diagnose(prj, None, e2, m2)
        if not same and exe and e2 and not [l for l in diag if "differ" in l or "stack" in l or "image is" in l]:
            diag.append("unpacked, ours is the program; the packed files differ: EXEPACK's work (its table's order: the order of the fixups)")
    elif not same:
        diag = diagnose(prj, ref, exe, mp)
    for l in diag:
        print("   " + l)
    ok = same or not [l for l in diag if "image" in l or "header" in l or "relocations differ" in l]
    print("%s linked from the sources: %d bytes; %s" % (os.path.basename(prj.exe), len(exe),
          "the FILE IDENTICAL" if same else ("the image, header and relocations identical" if ok else "not identical")))
    print("   %d functions are stand-ins of the program's bytes%s" % (len(stand), (": " + " ".join(stand)) if stand else ""))
    return 0 if ok else 1
