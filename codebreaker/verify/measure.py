"""measure.py -- judge one module and print the tally. Nothing is written to the source tree.

    python -m codebreaker measure <module>                 every decompiler, each asked only about
                                                           what an earlier one did not verify
    python -m codebreaker measure <module> core            one decompiler, asked about everything
    python -m codebreaker measure <module> core native     these, in this order

This is the number to watch while changing a decompiler or the verifier.
"""
import collections
import sys
import time


def main(argv):
    if not argv:
        print(__doc__)
        return 2
    from codebreaker.base import corpus
    from codebreaker.decompile import engines as E
    from codebreaker.store import functions as F
    from codebreaker.verify import reconstruct
    path = corpus.retail(argv[0])
    if not path:
        raise SystemExit("no module %r in the corpus" % argv[0])
    chosen = tuple(argv[1:]) or None
    for e in chosen or ():
        if e not in E.names():
            raise SystemExit("no decompiler %r (have: %s)" % (e, ", ".join(E.names())))
    fs = F.open()
    t = time.time()
    sha, digest = reconstruct.run(path, engines=chosen, fs=fs, cascade=not (chosen and len(chosen) == 1))
    for engine in chosen or E.DEFAULT:
        rows = fs.candidates(sha, digest, engine)
        tally = collections.Counter(F.classify(r["verdict"], r["limits"])[0] for r in rows if r["verdict"])
        if rows:
            print("  %-7s %s" % (engine, ", ".join("%s %d" % (F.TITLE[k], tally[k]) for k in F.ORDER if tally[k])))
        why = collections.Counter(r["reason"] for r in rows if r["verdict"] == "divergent")
        if why:
            print("          unverified by cause: " + ", ".join("%s %d" % kv for kv in why.most_common()))
    best = fs.tally(digest, sha, prefer=E.ORDER)
    print("%s in %.0fs, best of all: %s" % (argv[0], time.time() - t,
                                            ", ".join("%s %d" % (F.TITLE[k], best[k]) for k in F.ORDER)))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
