#!/usr/bin/env python3
"""why_unverified.py -- group a module's unverified functions by exactly what differed.

    python tools/why_unverified.py comctl32.dll [--examples N]

The verifier records, for each function that behaves differently, the first thing that differed
(which call, which argument, which return value). Grouping those gives the list of defects worth
fixing, largest first -- so work goes where it moves the most functions.
"""
import os
import re
import sys
from collections import Counter, defaultdict

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def kind_of(tok):
    if re.fullmatch(r"a\d", tok):
        return "an input"
    if tok == "S":
        return "a local's address"
    if re.fullmatch(r"-?\d+", tok):
        return "a small number"
    return "an address"


def cause(reason, detail):
    d = (detail or "").split("  [")[0]
    if reason == "deps-differ":
        m = re.match(r"call #\d+ ([^:]+): (.*) vs (.*)$", d)
        if not m:
            return "calls differ"
        kind, ra, rb = m.groups()
        if "argument" in kind:
            aa = ra[ra.find("(") + 1:-1].split(",")
            ab = rb[rb.find("(") + 1:-1].split(",")
            for x, y in zip(aa, ab):
                if x != y:
                    return "API argument: original passes %s, rebuilt passes %s" % (kind_of(x), kind_of(y))
        return {"rebuilt stops calling": "rebuilt makes fewer API calls",
                "rebuilt calls extra": "rebuilt makes extra API calls",
                "different call": "rebuilt calls a different API at the same point"}.get(kind, kind)
    if reason == "exit-differs":
        a, _, b = d[5:].partition(" vs ")
        return "ends differently: original %s, rebuilt %s" % (a, b)
    if reason == "ret-differs":
        a, _, b = d[4:].partition(" vs ")
        return "return value: original %s, rebuilt %s" % (kind_of(a), kind_of(b))
    if reason == "writes-differ":
        a, _, b = d[7:].partition(" vs ")
        return "memory written: " + ("only the rebuilt writes" if a == "none" else
                                     "only the original writes" if b == "none" else "both write, differently")
    if reason == "stack-differs":
        return "stack on return: " + d
    if reason == "registers-differ":
        return "changes a register it must preserve"
    return reason or "(no reason)"


def main(argv):
    if not argv:
        print(__doc__)
        return 2
    from src.targets.win95.verification import pipeline, store
    module = os.path.basename(argv[0]).lower()
    n_ex = int(argv[argv.index("--examples") + 1]) if "--examples" in argv else 2
    db = store.open()
    digest = pipeline.profile("pe")[0]
    rows = db.db.execute("SELECT fname, reason, detail FROM body WHERE lane='pe' AND module=? AND "
                         "verdict='divergent' AND profile=?", (module, digest)).fetchall()
    if not rows:
        print("no unverified functions recorded for %s on the current verifier" % module)
        return 1
    count, ex = Counter(), defaultdict(list)
    for fn, reason, detail in rows:
        k = cause(reason, detail)
        count[k] += 1
        if len(ex[k]) < n_ex:
            ex[k].append("%s: %s" % (fn, (detail or "")[:150]))
    print("%s: %d unverified functions, by cause" % (module, len(rows)))
    for k, n in count.most_common():
        print("%5d  %s" % (n, k))
        for e in ex[k]:
            print("         " + e)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
