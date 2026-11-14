"""batch 6: after retiring the named tools, which modules are held only by tools / only by tests / by nothing."""
import json, sys, os
P = "state/work/typed-port/python_reach/"
d = json.load(open(P + "graph.json")); graph, ext, files = d["graph"], d["ext"], d["files"]
RET = set(sys.argv[1:])
graph["codebreaker.base.corpus"] = ["codebreaker", "codebreaker.base", "codebreaker.base.settings"]
graph["codebreaker.__main__"] = ["codebreaker.base.corpus"]


def closure(rs):
    seen, st = set(), list(rs)
    while st:
        m = st.pop()
        if m in seen or m not in graph:
            continue
        seen.add(m)
        st.extend(graph[m])
    return seen


roots = {"codebreaker", "codebreaker.__main__", "codebreaker.base.corpus", "codebreaker.base.settings", "codebreaker.ui",
         "codebreaker.ui.terminal", "codebreaker.ui.client", "codebreaker.compare.image"}
live = closure(roots)


def users(m, pre):
    return sorted({f for f in ext.get(m, []) if f.startswith(pre) and os.path.basename(f)[:-3] not in RET})


tool_roots = {m for m in graph if users(m, "tools/")}
test_roots = {m for m in graph if users(m, "tests/")}
tcl = closure(tool_roots)
scl = closure(test_roots)
lines = {m: sum(1 for _ in open(files[m], encoding="utf-8", errors="replace")) for m in graph}


def show(name, s):
    print(name, len(s), sum(lines[m] for m in s))


show("live", live)
show("tools-only", tcl - live - scl)
show("tests", scl - live)
show("neither", set(graph) - live - tcl - scl)
json.dump({"live": sorted(live), "tool": sorted(tcl), "test": sorted(scl), "graph": graph, "lines": lines}, open(P + "pool6.json", "w"))
print(sorted(m[11:] for m in tcl - live - scl))
print(sorted(m[11:] for m in set(graph) - live - tcl - scl))
