import json, sys, collections
P = "state/work/typed-port/python_reach/"
g = json.load(open(P + "live3.json"))["graph"]
ext = json.load(open(P + "graph.json"))["ext"]
rows = {r["m"]: r for r in json.load(open(P + "rows.json"))}
S = set("codebreaker." + x for x in sys.argv[1].split(","))
bad = [m for m in S if m not in g]
print("unknown", bad)
for m in sorted(S):
    imp = [x for x in g if m in g[x] and x not in S]
    users = ext.get(m, [])
    print("%-44s importers-outside: %s\n%48s ext: %s" % (m[11:], [i[11:] for i in imp], "", users))
