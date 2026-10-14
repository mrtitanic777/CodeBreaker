#!/usr/bin/env python3
"""sample.py -- reconstruct and judge a spread of modules and print one line each, then a total.

    python tools/sample.py 16 [N]        N 16-bit modules, smallest first (default 12)
    python tools/sample.py 32 [N]        N 32-bit modules
    python tools/sample.py a.dll b.dll   these

A quick reading of where things stand across more than one module, without a full sweep.
"""
import os
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def main(argv):
    if not argv:
        print(__doc__)
        return 2
    from codebreaker.base import corpus
    from codebreaker.store import functions as F
    from codebreaker.verify import reconstruct
    if argv[0] in ("16", "32"):
        n = int(argv[1]) if len(argv) > 1 else 12
        paths = corpus.modules("ne" if argv[0] == "16" else "pe")
        paths = [p for p in paths if os.path.getsize(p) > 6000][:n]
    else:
        paths = [corpus.retail(a) or a for a in argv]
    fs = F.open()
    tot = dict.fromkeys(F.ORDER, 0)
    for p in paths:
        t = time.time()
        try:
            sha, digest = reconstruct.run(p, fs=fs, log=lambda s: None)
        except Exception as e:
            print("%-14s FAILED: %s: %s" % (os.path.basename(p).lower(), type(e).__name__, str(e)[:90]), flush=True)
            continue
        c = fs.tally(digest, sha)
        for k in F.ORDER:
            tot[k] += c[k]
        print("%-14s %s  (%.0fs)" % (os.path.basename(p).lower(), "  ".join(
            "%s %d" % (F.TITLE[k], c[k]) for k in F.ORDER if c[k]), time.time() - t), flush=True)
    n = sum(tot.values())
    print("TOTAL %d functions: %s" % (n, "  ".join("%s %d (%.0f%%)" % (F.TITLE[k], tot[k], 100.0 * tot[k] / max(1, n))
                                                    for k in F.ORDER if tot[k])))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
