"""Batch 3 pool: modules no live root reaches (ui/main/tools), grouped with the tests that hold them (connected components)."""
import json, re, os, collections, sys
P = "docs/history/python-port/python_reach/"
d = json.load(open(P + "live3.json"))
g = d["graph"]; lines = d["lines"]; tools = d["tools"]; tests = d["tests"]
RET = set(a + ".py" for a in sys.argv[1:])
g["codebreaker.base.corpus"] = ["codebreaker", "codebreaker.base", "codebreaker.base.settings"]


def closure(rs):
    seen = set(); st = list(rs)
    while st:
        m = st.pop()
        if m in seen or m not in g:
            continue
        seen.add(m); st.extend(g[m])
    return seen


roots = {"codebreaker", "codebreaker.__main__", "codebreaker.base.corpus", "codebreaker.base.settings", "codebreaker.ui", "codebreaker.ui.terminal", "codebreaker.ui.client"}
tool_roots = {m for m in g if [t for t in tools[m] if t not in RET]}
live = closure(roots) | closure(tool_roots)
dead = {m for m in g if m not in live}
print("pool", len(dead), sum(lines[m] for m in dead))
par = {}


def f(x):
    while par.setdefault(x, x) != x:
        par[x] = par[par[x]]
        x = par[x]
    return x


for m in dead:
    f(m)
    for x in g[m]:
        if x in dead:
            par[f(m)] = f(x)
    for t in tests[m]:
        par[f(m)] = f("TEST:" + t)
comp = collections.defaultdict(list)
for x in list(par):
    comp[f(x)].append(x)
out = []
for k, v in sorted(comp.items(), key=lambda kv: -len(kv[1])):
    mods = sorted(x for x in v if not x.startswith("TEST:"))
    ts = sorted(x[5:] for x in v if x.startswith("TEST:"))
    print(len(mods), sum(lines[m] for m in mods), "tests:", ts)
    if len(mods) < 40:
        print("    ", [m[11:] for m in mods])
json.dump({"dead": sorted(dead), "live": sorted(live)}, open(P + "pool3.json", "w"))
