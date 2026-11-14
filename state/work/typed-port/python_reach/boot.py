"""boot.py: the Python closure of the all-15 boot drivers, and who outside it imports each member."""
import json, sys
P = "state/work/typed-port/python_reach/"
g = json.load(open(P + "graph.json"))
graph, ext, files = g["graph"], g["ext"], g["files"]
live = json.load(open(P + "live3.json"))
roots = [m for m in graph if m.startswith("codebreaker.verify.authored") or m.startswith("codebreaker.verify.gate16")
         or m == "codebreaker.verify.lanes.powerpc" or m == "codebreaker.source.module_writer"]
seen, st = set(), list(roots)
while st:
    m = st.pop()
    if m in seen or m not in graph: continue
    seen.add(m); st.extend(graph[m])
lines = live["lines"]
print("roots", len(roots), "closure", len(seen), sum(lines[m] for m in seen))
for m in sorted(seen):
    out = sorted(x[11:] for x in graph if m in graph[x] and x not in seen)
    print("%-42s %5d  outside-importers: %s | ext: %s" % (m[11:], lines[m], out, sorted(ext.get(m, []))))
json.dump(sorted(seen), open(P + "bootset.json", "w"))
