"""Batch 3 reach: every command but `ui` is native, so the roots are the UI, __main__'s corpus option, compare.image (typed manifest),
tools/*.py still used, and the tests (the exe's gate)."""
import json, re, collections, sys, os
P = "state/work/typed-port/python_reach/"
d = json.load(open(P + "graph.json"))
graph, ext, files = d["graph"], d["ext"], d["files"]
RETIRED_TOOLS = set(sys.argv[1:])  # tool basenames treated as retired


def closure(rs):
    seen, st = set(), list(rs)
    while st:
        m = st.pop()
        if m in seen or m not in graph:
            continue
        seen.add(m)
        st.extend(graph[m])
    return seen


graph["codebreaker.__main__"] = ["codebreaker.base.corpus"]
roots = {"codebreaker", "codebreaker.__main__", "codebreaker.base.corpus", "codebreaker.base.settings",
         "codebreaker.ui", "codebreaker.ui.terminal", "codebreaker.ui.client", "codebreaker.compare.image"}
live_ui = closure(roots)


def tools_of(m):
    return sorted({os.path.basename(f) for f in ext.get(m, []) if f.startswith("tools/") and os.path.basename(f)[:-3] not in RETIRED_TOOLS})


def tests_of(m):
    return sorted({os.path.basename(f) for f in ext.get(m, []) if f.startswith("tests/")})


tool_roots = {m for m in graph if tools_of(m)}
tool_cl = closure(tool_roots)
test_roots = {m for m in graph if tests_of(m)}
test_cl = closure(test_roots)
lines = {m: sum(1 for _ in open(files[m], encoding="utf-8", errors="replace")) for m in graph}
dead_nt = [m for m in graph if m not in live_ui and m not in tool_cl and m not in test_cl]
print("modules", len(graph), sum(lines.values()))
print("live by ui/main:", len(live_ui), sum(lines[m] for m in live_ui))
print("held by tools only:", len(tool_cl - live_ui), sum(lines[m] for m in tool_cl - live_ui))
print("held by tests (not ui/tools):", len(test_cl - live_ui - tool_cl), sum(lines[m] for m in test_cl - live_ui - tool_cl))
print("no holder at all:", len(dead_nt), sum(lines[m] for m in dead_nt), sorted(m[11:] for m in dead_nt))
json.dump({"ui": sorted(live_ui), "tool": sorted(tool_cl), "test": sorted(test_cl), "graph": graph, "lines": lines,
           "tests": {m: tests_of(m) for m in graph}, "tools": {m: tools_of(m) for m in graph}}, open(P + "live3.json", "w"))
