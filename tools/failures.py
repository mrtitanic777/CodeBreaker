#!/usr/bin/env python3
"""failures.py -- a module's unverified functions, grouped by exactly what differed.

    python tools/failures.py <module> [--each N]

Reads the function store: for every function judged to behave differently, the verifier kept
the first thing that differed (which call, which value). Grouped, largest first, that is the
list of defects worth fixing.
"""
import collections
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def main(argv):
    if not argv:
        print(__doc__)
        return 2
    from codebreaker.base import corpus
    from codebreaker.store import functions as F
    from codebreaker.verify import reconstruct
    path = corpus.retail(argv[0]) or argv[0]
    each = int(argv[argv.index("--each") + 1]) if "--each" in argv else 3
    fs = F.open()
    sha, digest = reconstruct.identity(path)
    rows = [r for r in fs.candidates(sha, digest) if r["verdict"] == "divergent"]
    if not rows:
        print("no unverified functions recorded for %s under the current settings" % os.path.basename(path))
        return 1
    groups = collections.defaultdict(list)
    for r in rows:
        d = (r["detail"] or "").split("  [")[0]
        kind = re.sub(r"\b[0-9a-f]{4,}\b", "N", re.sub(r"#\d+", "#k", d))[:70] if d else ""
        groups[(r["reason"], kind)].append(r)
    print("%s: %d unverified" % (os.path.basename(path), len(rows)))
    for (reason, kind), rs in sorted(groups.items(), key=lambda kv: -len(kv[1])):
        print("%4d  %s  %s" % (len(rs), reason, kind))
        for r in rs[:each]:
            print("        %s %s: %s" % (r["addr"], r["name"], (r["detail"] or "")[:130]))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
