"""Two function stores compared verdict by verdict -- what a change to the decompiler did to whether
its functions behave as the original.

    python tools/verdict_diff.py <before.sqlite> <after.sqlite> [--show N]

Each store is a reconstruction run (verify/reconstruct.py: decompile, compile, run beside the
original in the emulator) of the same modules, e.g. one with the decompiler as it was and one with
it as it is now (CB_FUNCTIONS_DB, CB_DECOMPILER_BIN, CB_LANGUAGES_DIR). Per function the verdict
in each, and the counts of each change: better (unverified -> verified ...), worse, the same. A
function that got worse is listed with both verdicts' reasons, so it can be looked at.
"""
import collections
import os
import sqlite3
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from codebreaker.verify import verdicts as V   # noqa: E402

RANK = {V.EQUIVALENT: 3, V.CONSISTENT: 2, V.INCONCLUSIVE: 1, V.DIVERGENT: 0, V.NO_COMPILE: -1,
        V.NO_SOURCE: -2, None: -3}


def verdicts(path):
    db = sqlite3.connect(path)
    q = ("SELECT m.name, f.addr, c.name, v.verdict, v.reason, c.source FROM function f "
         "JOIN module m ON m.sha=f.module JOIN candidate c ON c.function=f.id "
         "JOIN verdict v ON v.candidate=c.id")
    out = {}
    for mod, addr, name, verdict, reason, source in db.execute(q):
        out[(mod, addr)] = (name, V.verdict(verdict), reason or "", source or "")     # (older words read as now)
    return out


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return 2
    show = int(argv[argv.index("--show") + 1]) if "--show" in argv else 10
    a, b = verdicts(argv[0]), verdicts(argv[1])
    counts = collections.Counter()
    moves = collections.Counter()
    worse = []
    for k in sorted(set(a) | set(b)):
        va = a.get(k, (None, None, "", ""))
        vb = b.get(k, (None, None, "", ""))
        if va[1] == vb[1]:
            counts["same verdict" + (" (text changed)" if va[3] != vb[3] else "")] += 1
            continue
        moves["%s -> %s" % (va[1], vb[1])] += 1
        if RANK.get(vb[1], 0) > RANK.get(va[1], 0):
            counts["better"] += 1
        elif RANK.get(vb[1], 0) < RANK.get(va[1], 0):
            counts["worse"] += 1
            worse.append((k, va, vb))
        else:
            counts["changed, same rank"] += 1
    for k, n in counts.most_common():
        print("%-32s %6d" % (k, n))
    print()
    for k, n in moves.most_common():
        print("  %-30s %5d" % (k, n))
    for (mod, addr), va, vb in worse[:show]:
        print("\n--- WORSE %s %s %s: %s -> %s" % (mod, addr, vb[0] or va[0], va[1], vb[1]))
        print("    before: %s" % va[2][:200])
        print("    after:  %s" % vb[2][:200])
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
