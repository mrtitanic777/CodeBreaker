"""free6.py <prefix,...>: modules of the prefixes outside the kept closure, with line counts and their outside holders (tests, tools, other modules)."""
import json, sys
P = "state/work/typed-port/python_reach/"
d = json.load(open(P + "pool6.json"))
g, lines = d["graph"], d["lines"]
keep = set(json.load(open(P + "keep6.json")))
ext = json.load(open(P + "graph.json"))["ext"]
pre = ["codebreaker." + x for x in sys.argv[1].split(",")]
S = sorted(m for m in g if any(m == p or m.startswith(p + ".") for p in pre) and m not in keep)
print(len(S), sum(lines[m] for m in S))
for m in S:
    mods = [x[11:] for x in g if m in g[x] and x not in S]
    ts = [f.split("/")[-1] for f in ext.get(m, [])]
    print("%-44s %5d  mods=%s tests=%s" % (m[11:], lines[m], mods, ts))
