"""cand6.py <prefix,...>: who outside the candidate set imports it (modules from pool6.json, tests/tools from graph.json)."""
import json, sys
P = "docs/history/python-port/python_reach/"
g = json.load(open(P + "pool6.json"))["graph"]
ext = json.load(open(P + "graph.json"))["ext"]
pre = ["codebreaker." + x for x in sys.argv[1].split(",")]
S = {m for m in g if any(m == p or m.startswith(p + ".") for p in pre)}
print(len(S), "modules")
outs = {}
for m in sorted(S):
    for x in g:
        if m in g[x] and x not in S:
            outs.setdefault("mod:" + x[11:], []).append(m[11:])
    for u in ext.get(m, []):
        outs.setdefault(u, []).append(m[11:])
for k in sorted(outs):
    print(k, "->", len(outs[k]), outs[k][:4])
