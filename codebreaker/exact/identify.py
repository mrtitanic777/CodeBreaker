"""identify.py -- which toolchain made the program: the one that makes the most of its sources' functions its bytes.

    python -m codebreaker exact PROJECT identify [--try NAME[:VARIANT][/SWITCHES] ..] [--sample N] [--write]

A program's sources - seeded from another version (exact/seed.py), drafted, or written - are compiled by each
candidate toolchain and held to the program as the ledger holds them. The candidates are every toolchain profile
whose sources are of the project's kind (the files' extension), each with its default switches and the
alternatives its profile lists ("try_switches": a compiler's switches that change the code - C 3.00's -Gs),
or those --try names. --write sets the project's toolchain, variant and switches to the best.
"""
import json
import os
import random

from codebreaker.exact import ledger, toolchain as tcm


def candidates(prj, tries=None):
    if tries:
        out = []
        for t in tries:
            nv, _, sw = t.partition("/")
            n, _, v = nv.partition(":")
            out.append((n, v or None, sw or None))
        return out
    ext = {os.path.splitext(p)[1].lower() for at, p in prj.sources()}
    out = []
    for n in tcm.profiles():
        p = json.load(open(os.path.join(tcm.HERE, "toolchains", n + ".json")))
        se = (p.get("source_ext") or (".pas" if "pascal" in n else ".plm" if "plm" in n else ".c")).lower()
        if se not in ext:
            continue
        for v in [None]:                                    # (the default variant; --try names others)
            out.append((n, v, None))
            for sw in p.get("try_switches", []):
                out.append((n, v, sw))
    return out


def score(prj, cand, files):
    n, v, sw = cand
    try:
        tc = tcm.load(n, v)
    except Exception as e:
        return -1, "%s" % e
    syms = prj.symbols()
    same = 0
    total = 0
    old = prj.switches
    prj.switches = sw if sw is not None else old
    try:
        for at, path in files:
            for r in ledger.check_file(prj, tc, path, at, dict(syms), {}):
                total += 1
                same += r[4] == "identical"
    finally:
        prj.switches = old
    return same, "%d of %d" % (same, total)


def main(prj, argv):
    tries = [argv[k + 1] for k, a in enumerate(argv) if a == "--try" and k + 1 < len(argv)]
    n = int(argv[argv.index("--sample") + 1]) if "--sample" in argv else 12
    files = sorted(prj.sources())
    random.Random(0).shuffle(files)
    files = files[:n]
    best = None
    for c in candidates(prj, tries):
        s, txt = score(prj, c, files)
        print("   %-10s %-8s %-8s %s" % (c[0], c[1] or "", c[2] or "", txt))
        if best is None or s > best[0]:
            best = (s, c)
    if not best or best[0] <= 0:
        print("no toolchain makes any of the sample's functions the program's")
        return 1
    s, (name, var, sw) = best
    print("the best: %s%s%s" % (name, ":" + var if var else "", " " + sw if sw else ""))
    if "--write" in argv:
        p = os.path.join(prj.dir, "project.json")
        c = json.load(open(p))
        c["toolchain"] = name
        if var:
            c["variant"] = var
        else:
            c.pop("variant", None)
        if sw:
            c["switches"] = sw
        json.dump(c, open(p, "w"), indent=1)
        print("written to", p)
    return 0
