import json, sys
P = "docs/history/python-port/python_reach/"
d = json.load(open(P + "live.json"))
live = set(d["live"])
g = d["cutgraph"]
rows = {r["m"]: r for r in json.load(open(P + "rows.json"))}
dead = {m for m in rows if m not in live}
for k in sys.argv[1:]:
    print("==", k)
    for m in sorted(dead):
        if m.startswith("codebreaker." + k):
            r = rows[m]
            print(m[11:], r["lines"], r["tests"], r["gens"], [x[11:] for x in g[m] if x in dead and x != m][:4], "T" if r["ntest"] else "-", "B" if r["boot"] else "")
