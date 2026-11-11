"""bench.py -- the inverse model held to a project whose sources are known: what it drafts from no source, beside
the real source, with the compiler's verdict. The measure of the model, and where it fails.

    python -m codebreaker exact PROJECT bench [ADDR..] [--show] [--out FILE]

Every function the project has a one-function source for (src/fADDR.c) is drafted from the program's bytes
alone and proven as the draft stage proves it. The verdicts are counted; --show prints each draft beside its
source; --out writes one line a function (address, verdict) for comparing two runs.
"""
import collections
import importlib

from codebreaker.exact import carve, draft


def run(prj, want=None, show=False):
    tc = prj.toolchain()
    inv = importlib.import_module("codebreaker.invert." + tc.p["inverse"])
    names = draft._names(prj)
    img = prj.image
    fs = {a: ln for a, ln, w in carve.functions(prj)}
    have = {a: p for a, p in prj.sources() if a in fs}
    rows = []
    for a in sorted(want or have):
        if a not in fs:
            continue
        ln = fs[a]
        try:
            texts = draft.model_texts(inv, bytes(img[a:a + ln]), "f%05x" % a, a)
        except Exception as e:
            rows.append((a, "declined: %s" % str(e)[:80], ""))
            continue
        try:
            src, v = draft.prove_texts(prj, tc, inv, texts, a, ln, names)
        except Exception as e:                              # (a pass that fails on one function: noted, not fatal)
            import traceback
            src, v = traceback.format_exc(), "failed: %s: %s" % (type(e).__name__, str(e)[:80])
        rows.append((a, v, src))
        if show:
            print("== %05x %d bytes: %s\n%s" % (a, ln, v, src))
            if a in have:
                print("-- the source:\n" + open(have[a], encoding="latin1").read())
    return rows


def main(prj, argv):
    want = [int(x, 16) for k, x in enumerate(argv)        # (the file --out names is not an address, whatever its name is made of)
            if x and not x.startswith("-") and all(c in "0123456789abcdefABCDEF" for c in x) and not (k and argv[k - 1] == "--out")]
    rows = run(prj, want or None, "--show" in argv)
    tally = collections.Counter(v.split(":")[0] for a, v, s in rows)
    if "--out" in argv:
        with open(argv[argv.index("--out") + 1], "w") as f:
            for a, v, s in rows:
                f.write("%05x %s\n" % (a, v))
    print("%d functions: %s" % (len(rows), ", ".join("%d %s" % (n, k) for k, n in tally.most_common())))
    return 0
