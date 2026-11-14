import json, collections, sys
P = "docs/history/python-port/python_reach/"
d = json.load(open(P + "live.json"))
g = d["cutgraph"]
live = set(d["live"])
rows = {r["m"]: r for r in json.load(open(P + "rows.json"))}
dead = {m for m in rows if m not in live}
print("dead", len(dead), sum(rows[m]["lines"] for m in dead))
par = {}
def f(x):
    while par.setdefault(x, x) != x:
        par[x] = par[par[x]]
        x = par[x]
    return x
def u(a, b):
    par[f(a)] = f(b)
for m in dead:
    f(m)
    for x in g[m]:
        if x in dead:
            u(m, x)
    for t in rows[m]["tests"]:
        u(m, "TEST:" + t)
comp = collections.defaultdict(list)
for x in list(par):
    comp[f(x)].append(x)
for k, v in sorted(comp.items(), key=lambda kv: -len(kv[1])):
    mods = [x for x in v if not x.startswith("TEST:")]
    tests = sorted(x[5:] for x in v if x.startswith("TEST:"))
    ln = sum(rows[m]["lines"] for m in mods)
    pk = collections.Counter(m.split(".")[1] for m in mods)
    norep = [m[11:] for m in mods if not rows[m]["rep"] or not rows[m]["ntest"]]
    print(len(mods), ln, dict(pk), "\n   tests:", tests, "\n   boot:", [m[11:] for m in mods if rows[m]["boot"]], "norep/notest:", norep)
