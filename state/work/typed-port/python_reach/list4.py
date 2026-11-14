import json, collections
P = "state/work/typed-port/python_reach/"
rows = {r["m"]: r for r in json.load(open(P + "rows.json"))}
g = json.load(open(P + "live3.json"))["graph"]
pool = set(json.load(open(P + "pool3.json"))["dead"])
tests = lambda m: set(t[5:-3] for t in rows[m]["tests"])
adj = collections.defaultdict(set)
for m in pool:
    for x in g[m]:
        if x in pool:
            adj[m].add(x)
            adj[x].add(m)
for m in pool:
    for t in rows[m]["tests"]:
        adj[m].add("T:" + t)
        adj["T:" + t].add(m)
st = [m for m in pool if "test_platforms.py" in rows[m]["tests"]]
seen = set(st)
while st:
    x = st.pop()
    for y in adj[x]:
        if y not in seen:
            seen.add(y)
            st.append(y)
mods = sorted(m for m in seen if not m.startswith("T:"))
print(len(mods))
for m in mods:
    r = rows[m]
    print("%-52s %5d %s %-40s %s" % (m[11:], r["lines"], "BOOT" if r["boot"] else "    ", (r["rep"] or "-")[:40], ",".join(sorted(tests(m)))))
