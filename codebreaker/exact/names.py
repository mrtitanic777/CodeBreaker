"""names.py -- a program's functions and data named after another version of it that has names.

    python -m codebreaker exact PROJECT names --from OTHER [--write] [-v]

Most of a program's functions are the same source in its neighbouring versions; their code is the same but
for where things are. Each function's instructions are read with the places masked - call and jump targets,
the addresses of data, the words the linker fills - so that code that moved is still the same code:

  1. a function whose masked code is the same as one function of OTHER's (and of no other function of
     either) is that function: it takes its name
  2. from a pair, the calls each makes at the same instruction name each other's callees (a function whose
     own code changed is still found, by who calls it); the same for the data words at the same places
  3. a name OTHER has that this program already gives another function is not given twice

OTHER's names: its src/SYMBOLS (C code, D data) and its "place_by" file (name fXXXXX / g_XXXX). --write puts the
names into this project's src/SYMBOLS (a name it has already is kept); without it they are listed.

    names.carry(project, other) -> {address: (name, how)}, {DS offset: (name, how)}
"""
import os
import re

from codebreaker.exact import carve, project as projmod, x86

ADDR = re.compile(r"0x[0-9a-f]{3,}")


CODE_NAME = re.compile(r"(?:f|P)([0-9a-fA-F]{4,5})")
DATA_NAME = re.compile(r"(?:g_|G)([0-9a-fA-F]{1,4})")


def known(prj, starts=None):
    """({code address: name}, {DS offset: name}) the project has: its SYMBOLS (a place of no kind is code where a
    function begins), its "place_by" file and its "names" file - lines NAME fXXXXX|g_XXXX, or PXXXXX|GXXXX NAME"""
    code, data = {}, {}
    starts = set(a for a, n in functions(prj)) if starts is None else starts
    for n, (t, a) in prj.symbols().items():
        if ":" in n or "@" in n:
            continue
        if t == "C" or t == "?" and a in starts:
            code.setdefault(a, n)
        elif t == "D" or t == "?":
            data.setdefault(a, n)
    for key in ("place_by", "names"):
        f = prj.cfg.get(key)
        p = (prj.path(f) if key == "place_by" else os.path.join(prj.dir, f)) if f else None
        if not p or not os.path.exists(p):
            continue
        for l in open(p, encoding="utf-8"):
            c = l.split("#")[0].split()
            if len(c) < 2:
                continue
            for x, n in ((c[1], c[0]), (c[0], c[1])):
                m = CODE_NAME.fullmatch(x)
                if m and not CODE_NAME.fullmatch(n):
                    code.setdefault(int(m.group(1), 16), n)
                    break
                m = DATA_NAME.fullmatch(x)
                if m and not DATA_NAME.fullmatch(n):
                    data.setdefault(int(m.group(1), 16), n)
                    break
    return code, data


def functions(prj):
    try:
        fs = [(a, n) for a, n, w in carve.functions(prj)]
    except Exception:
        fs = []
    if not fs:                                              # (no carver for the toolchain: the places named)
        code, _ = known(prj, starts=set())
        starts = sorted(code)
        fs = [(a, (starts[k + 1] if k + 1 < len(starts) else min(len(prj.image), a + 0x400)) - a) for k, a in enumerate(starts)]
    return fs


def read(prj, a, ln):
    """[(masked instruction, call target or None, data place or None)]"""
    out = []
    for i in x86.disasm(prj.image, a, a + ln, False):
        ops = i.op_str
        tgt = None
        if i.mnemonic == "call" and re.fullmatch(r"0x[0-9a-f]+", ops):
            tgt = int(ops, 16)
            ops = "T"
        elif i.mnemonic.startswith("j") or i.mnemonic.startswith("loop"):
            ops = "J"                                       # (a jump within the function: its shape is the code)
        dat = None
        m = re.search(r"\[(0x[0-9a-f]+)\]", ops)
        if m:
            dat = int(m.group(1), 16)
        out.append((i.mnemonic + " " + ADDR.sub("N", ops), tgt, dat))
    return out


def carry(prj, other):
    mine = {a: read(prj, a, n) for a, n in functions(prj)}
    theirs = {a: read(other, a, n) for a, n in functions(other)}
    ocode, odata = known(other)
    key = lambda r: tuple(x[0] for x in r)
    by_m, by_t = {}, {}
    for a, r in mine.items():
        by_m.setdefault(key(r), []).append(a)
    for a, r in theirs.items():
        by_t.setdefault(key(r), []).append(a)
    pairs = {}
    how = {}
    for k, ms in by_m.items():                              # 1. the same code, once on each side
        ts = by_t.get(k, [])
        if len(ms) == 1 and len(ts) == 1 and len(k) >= 4:
            pairs[ms[0]] = ts[0]
            how[ms[0]] = "the same code"
    mcode0, _ = known(prj)
    for a, n in mcode0.items():                             # (anchors: what both name alike - the library's)
        b = next((b for b, m in ocode.items() if m == n), None)
        if a in mine and b in theirs and a not in pairs and b not in pairs.values() and not re.fullmatch(r"_?f[0-9a-f]{5}", n):
            pairs[a] = b
            how[a] = "named so on both sides"
    dpairs = {}
    calls = lambda r: [c for x, c, d in r if c is not None]

    def spread():
        work = list(pairs.items())
        while work:                                         # 2. calls and data at the same places
            a, b = work.pop()
            ra, rb = mine[a], theirs[b]
            same = key(ra) == key(rb)
            ca_, cb_ = calls(ra), calls(rb)
            if same or len(ca_) == len(cb_):                # (the same code, or as many calls in it)
                for ca, cb in zip(ca_, cb_):
                    alike = same or ca in mine and cb in theirs and len(calls(mine[ca])) == len(calls(theirs[cb])) and \
                        0.8 <= len(mine[ca]) / max(len(theirs[cb]), 1) <= 1.25     # (the callees alike: a caller
                    if alike and ca in mine and cb in theirs and ca not in pairs and cb not in pairs.values():   # of the
                        # same code may call another function there in this version)
                        pairs[ca] = cb
                        how[ca] = "called where %s calls it" % (ocode.get(b) or "f%05x" % b)
                        work.append((ca, cb))
            if same:
                for (xa, ca, da), (xb, cb, db) in zip(ra, rb):
                    if da is not None and db is not None and da not in dpairs:
                        dpairs[da] = db
    spread()
    while True:                                             # 3. who calls the paired ones, in the same order
        inv = {b: a for a, b in pairs.items()}
        sig_m, sig_t = {}, {}
        for a, r in mine.items():
            if a not in pairs:
                s = tuple(pairs.get(c, "?") for c in calls(r))
                if sum(1 for x in s if x != "?") >= 2:
                    sig_m.setdefault(s, []).append(a)
        for b, r in theirs.items():
            if b not in inv:
                s = tuple(c if c in inv else "?" for c in calls(r))
                if sum(1 for x in s if x != "?") >= 2:
                    sig_t.setdefault(s, []).append(b)
        new = [(ms[0], sig_t[s][0]) for s, ms in sig_m.items() if len(ms) == 1 and len(sig_t.get(s, [])) == 1]
        if not new:
            break
        for a, b in new:
            pairs[a] = b
            how[a] = "calls what %s calls, in its order" % (ocode.get(b) or "f%05x" % b)
        spread()
    names, dnames = {}, {}
    used = set()
    mcode, mdata = known(prj)
    for a, b in sorted(pairs.items()):
        n = ocode.get(b)
        if n and re.fullmatch(r"_?f[0-9a-f]{5}|P[0-9A-F]{5}", n):     # (an address-name is no name to carry)
            continue
        if n and n not in used and not (a in mcode and mcode[a] != n and not re.fullmatch(r"_?f[0-9a-f]{5}", mcode[a])):
            names[a] = (n, how[a])
            used.add(n)
    dused = set()
    for a, b in sorted(dpairs.items()):
        n = odata.get(b)
        if n and n not in dused:
            dnames[a] = (n, "named where the same code names it")
            dused.add(n)
    carry.pairs, carry.dpairs = pairs, dpairs               # (this program's place -> the other's: exact/seed.py)
    return names, dnames


def main(prj, argv):
    if "--from" not in argv:
        print("names: --from OTHER (a project of another version of the program, with names)")
        return 1
    other = projmod.load(os.path.join(os.getcwd(), argv[argv.index("--from") + 1]) if not os.path.isabs(argv[argv.index("--from") + 1]) else argv[argv.index("--from") + 1])
    names, dnames = carry(prj, other)
    mcode, mdata = known(prj)
    agree = sum(1 for a, (n, h) in names.items() if mcode.get(a) == n)
    clash = [(a, n, mcode[a]) for a, (n, h) in names.items() if a in mcode and mcode[a] != n and not re.fullmatch(r"_?f[0-9a-f]{5}", mcode[a])]
    if "-v" in argv:
        for a, (n, h) in sorted(names.items()):
            print("  f%05x %-24s %s%s" % (a, n, h, "" if a not in mcode else "  (has: %s)" % mcode[a]))
    print("%d functions named (%d of them as the project names them already, %d it names otherwise), %d data" % (
        len(names), agree, len(clash), len(dnames)))
    for a, n, m in clash[:10]:
        print("   f%05x: %s here, %s by the other" % (a, m, n))
    if "--write" in argv:
        s = prj.symbols()
        for a, (n, h) in names.items():
            s.setdefault(n, ("C", a))
        for a, (n, h) in dnames.items():
            s.setdefault(n, ("D", a))
        prj.save_symbols(s)
        print("written to", prj.path("SYMBOLS"))
    return 0
