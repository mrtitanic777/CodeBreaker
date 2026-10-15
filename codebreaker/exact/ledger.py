"""ledger.py -- every one-function source of a project compiled by its toolchain and put against the program.

    python -m codebreaker exact PROJECT ledger [--learn] [-v] [--write]

Each src/fADDR.c is compiled on its own; its first function is held to the program at ADDR, the
others at their places in SYMBOLS. --learn: the places identical functions show are added to
SYMBOLS (a name is only learnt from a file all of whose placed functions are identical). --write:
src/LEDGER is written (otherwise only the counts are printed).

    ledger.run(project, learn=False) -> [(file, function, address, length, verdict, notes)]
        verdict: identical | differs | no place | does not compile
"""
import os

from codebreaker.exact import compare, omf


def check_file(prj, tc, path, at, syms, learnt):
    src = open(path, "rb").read()
    raw, con = tc.compile(src, headers=prj.headers(), include=prj.include, switches=prj.switches)
    if raw is None:
        return [(os.path.basename(path), None, at, 0, "does not compile", [con.strip().splitlines()[-1:] and con.strip().splitlines()[-1] or ""])]
    o = omf.read(raw)
    out = []
    got = {}
    for k, (name, off, ln) in enumerate(omf.functions(o)):
        a = at if k == 0 else (syms[name][1] if name in syms and syms[name][0] == "C" else None)
        if a is None:
            out.append((os.path.basename(path), name, None, ln, "no place", []))
            continue
        bad, notes = compare.function(o, prj.image, name, off, ln, a, prj, syms, got)
        ok = compare.identical(bad, notes)
        out.append((os.path.basename(path), name, a, ln, "identical" if ok else "differs", notes))
    if all(r[4] == "identical" for r in out if r[2] is not None):
        learnt.update({k: v for k, v in got.items() if ":" not in k})
    return out


def run(prj, learn=False, verbose=False):
    tc = prj.toolchain()
    syms = prj.symbols()
    learnt = {}
    rows = []
    for at, path in prj.sources():
        r = check_file(prj, tc, path, at, dict(syms), learnt)
        rows.extend(r)
        if verbose:
            for x in r:
                print("%-10s %-24s %s %s" % (x[0], x[1], x[4], "; ".join(x[5][:2])))
    if learn and learnt:
        s = prj.symbols()
        for k, v in learnt.items():
            s.setdefault(k, v)
        prj.save_symbols(s)
    return rows


def write(prj, rows):
    with open(prj.path("LEDGER"), "w", newline="\n") as f:
        f.write("# file, function, bytes, compared with %s (codebreaker exact ledger)\n" % os.path.basename(prj.exe))
        for fn, name, a, ln, v, notes in rows:
            f.write("%s %s %d %s\n" % (fn, name or "-", ln, v))


def main(prj, argv):
    rows = run(prj, learn="--learn" in argv, verbose="-v" in argv)
    if "--write" in argv:
        write(prj, rows)
    n = len(rows)
    same = sum(1 for r in rows if r[4] == "identical")
    print("%d functions, %d identical, %d differ, %d no place, %d files do not compile" % (
        n, same, sum(1 for r in rows if r[4] == "differs"), sum(1 for r in rows if r[4] == "no place"),
        sum(1 for r in rows if r[4] == "does not compile")))
    return 0
