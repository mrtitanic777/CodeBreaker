"""fixpoint.py: the deletion set of the boot batch as a fixed point.
D = modules dead once the tests that import a member of D (and the tools that do) are cut and `__main__`'s fallback table is dropped.
Keepers (roots): the terminal UI, base.settings, and batch 6's areas (translate, java, exact) -- and every other test/tool."""
import json, os, sys
P = "docs/history/python-port/python_reach/"
G = json.load(open(P + "graph.json"))
g = G["graph"]; ext = G["ext"]; files = G["files"]
lines = json.load(open(P + "live3.json"))["lines"]
g["codebreaker.__main__"] = []
SEED = [m for m in g if m.startswith("codebreaker.verify.authored") or m.startswith("codebreaker.verify.gate16")
        or m in ("codebreaker.verify.lanes.powerpc", "codebreaker.source.module_writer", "codebreaker.build.period", "codebreaker.build.module", "codebreaker.symbols.names")]
B6 = ("translate", "java", "exact")
def is_b6(m): return m.split(".")[1] in B6 if m.count(".") else False
# external files (tests/tools/native generators) -> modules they import
fileimp = {}
for m, fs in ext.items():
    for f in fs: fileimp.setdefault(f, set()).add(m)
def closure(rs):
    seen, st = set(), list(rs)
    while st:
        m = st.pop()
        if m in seen or m not in g: continue
        seen.add(m); st.extend(g[m])
    return seen
base_roots = {"codebreaker", "codebreaker.base.settings", "codebreaker.ui", "codebreaker.ui.terminal", "codebreaker.ui.client"}
b6 = {m for m in g if is_b6(m)}
D = set(SEED)
cut = set()
for it in range(20):
    cut = {f for f, ms in fileimp.items() if ms & D and not f.startswith("tools/") and f != "tests/test_platforms.py"}
    roots = set(base_roots) | b6
    for f, ms in fileimp.items():
        if f not in cut and not f.startswith("native/") and not f.startswith("docs"):
            roots |= ms
    # native generators keep modules too, but they are checked-in goldens' makers; report separately
    alive = closure(roots)
    newD = {m for m in g if m not in alive}
    if newD == D: break
    D = newD
print("iterations", it, "D", len(D), sum(lines[m] for m in D))
print("seed not dead:", sorted(m[11:] for m in SEED if m not in D))
for m in sorted(D): print("  ", m[11:], lines[m])
tests = sorted(f for f in cut if f.startswith("tests/"))
tools = sorted(f for f in cut if f.startswith("tools/"))
print("tests cut/owned:", tests)
print("tools:", tools)
print("native gens:", sorted(f for f in cut if f.startswith("native/")))
json.dump({"D": sorted(D), "tests": tests, "tools": tools}, open(P + "bootdel.json", "w"))
