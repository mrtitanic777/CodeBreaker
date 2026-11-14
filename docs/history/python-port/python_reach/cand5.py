"""cand5.py <prefix,prefix,...>: the candidate set (modules whose name starts with one of the prefixes, relative to codebreaker.) and who outside it imports each."""
import json, sys
P = "docs/history/python-port/python_reach/"
g = json.load(open(P + "live3.json"))["graph"]
ext = json.load(open(P + "graph.json"))["ext"]
pre = ["codebreaker." + x for x in sys.argv[1].split(",")]
S = {m for m in g if any(m == p or m.startswith(p + ".") for p in pre)}
print(len(S), "modules")
for m in sorted(S):
    imp = [x[11:] for x in g if m in g[x] and x not in S]
    users = ext.get(m, [])
    print("%-40s importers-outside: %s\n%44s ext: %s" % (m[11:], imp, "", users))
