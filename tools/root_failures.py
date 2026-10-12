#!/usr/bin/env python3
"""root_failures.py -- which failing functions are wrong themselves, and which only inherit it?

    python tools/root_failures.py comctl32.dll [--top N] [--show NAME]

A function that calls a wrong function fails too, so most failures in a module are downstream of
a few. This separates the ROOTS (failing functions none of whose callees fail) from the rest and
ranks each root by how many failures sit above it -- the order in which to fix things.
`--show NAME` prints that function's reconstruction as it was compiled.

Works on the embedded core's lane (pe-core) and the text of its last build.
"""
import glob
import os
import re
import sys
from collections import Counter, defaultdict

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))


def load(module):
    from src.targets.win95 import bodyverify as B
    from src.targets.win95.verification import store
    d = B._scratch("pe-core", module)
    cs = sorted(glob.glob(os.path.join(d, "g*.c")), key=os.path.getmtime)
    if not cs:
        raise SystemExit("no core build found for %s (run module_source first)" % module)
    src = open(cs[-1], encoding="utf-8", errors="replace").read()
    bodies = dict(re.findall(r"(?ms)^/\*@@ (\S+) \*/\n(.*?)(?=^/\*@@ |\Z)", src))
    bodies.pop("(stand-ins)", None)
    db = store.open()
    st, det = {}, {}
    q = "SELECT fname, verdict, reason, detail FROM body WHERE lane='pe-core' AND module=?"
    for fn, v, r, d2 in db.db.execute(q, (module,)):
        st[fn], det[fn] = v, (r or "", d2 or "")
    return bodies, st, det


def main(argv):
    if not argv:
        print(__doc__)
        return 2
    import why_unverified as W
    module = os.path.basename(argv[0]).lower()
    top = int(argv[argv.index("--top") + 1]) if "--top" in argv else 15
    bodies, st, det = load(module)
    if "--show" in argv:
        n = argv[argv.index("--show") + 1]
        print("/* %s: %s %s */\n%s" % (n, st.get(n), det.get(n, ("", ""))[1], bodies.get(n, "(not in the last build)")))
        return 0
    names = set(bodies) | set(st)
    calls = {n: (set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", bodies.get(n, "").split("{", 1)[-1])) & names) - {n}
             for n in names}
    bad = {n for n in names if st.get(n) in ("divergent", "no-compile")}
    roots = {n for n in bad if not (calls[n] & bad)}
    callers = defaultdict(set)
    for n in names:
        for m in calls[n]:
            callers[m].add(n)

    def above(r):
        seen, work = set(), [r]
        while work:
            x = work.pop()
            for y in callers[x]:
                if y in bad and y not in seen:
                    seen.add(y)
                    work.append(y)
        return seen
    print("%s: %d failing; %d are roots, %d fail only above another failure" % (
        module, len(bad), len(roots), len(bad) - len(roots)))
    ranked = sorted(((len(above(r)), r) for r in roots), reverse=True)
    for n, r in ranked[:top]:
        print("%4d above  %-22s %-10s %s" % (n, r, st.get(r), (det[r][1] or det[r][0])[:130]))
    print("roots by cause:")
    for k, n in Counter(W.cause(*det[r]) if st[r] == "divergent" else "does not compile: " + det[r][0][:40]
                        for r in roots).most_common(10):
        print("%5d  %s" % (n, k))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
