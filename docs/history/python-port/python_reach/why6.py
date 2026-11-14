"""why6.py <module-suffix>: a shortest import path from the kept roots to the module."""
import json, sys, collections
P = "docs/history/python-port/python_reach/"
d = json.load(open(P + "pool6.json"))
g = d["graph"]
KEEP = ("verify.authored", "verify.gate16", "verify.lanes.powerpc", "source.module_writer", "ui")
roots = [m for m in g if any(m == "codebreaker." + k or m.startswith("codebreaker." + k + ".") for k in KEEP)] + d["live"]
prev = {r: None for r in roots}
q = collections.deque(roots)
while q:
    m = q.popleft()
    for n in g.get(m, []):
        if n not in prev:
            prev[n] = m
            q.append(n)
for t in sys.argv[1:]:
    m = "codebreaker." + t
    path = []
    while m:
        path.append(m[11:])
        m = prev.get(m)
    print(t, " <- ".join(path) if path[0] in [x[11:] for x in prev] else "not reached")
