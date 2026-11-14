"""native_closure.py: from the sources of `regress all15` (and the files the exe wires for it), follow every #include
"cb/..." header to its same-stem .cpp (and the unity-split siblings by prefix), and report any file in the closure that
includes the Python-imitation layer (py*.hpp, native/typed, native/ported, compat shims, py_bridge) or uses py:: / pyengine."""
import os, re, sys, glob
ROOT = os.getcwd()
CB = os.path.join(ROOT, "native", "cb")
inc = re.compile(r'#\s*include\s+[<"]([^>"]+)[>"]')
seeds = ["verify/regress.cpp", "app/codebreaker_main.cpp", "engine/lane_kit.cpp"]
seen, st = set(), list(seeds)
bad = []
def siblings(rel):
    base = os.path.splitext(rel)[0]
    d, stem = os.path.split(base)
    out = [rel]
    for ext in (".cpp", ".hpp"):
        if os.path.exists(os.path.join(CB, base + ext)): out.append(base + ext)
    # unity splits: stem_*.cpp
    for p in glob.glob(os.path.join(CB, d, stem + "_*.cpp")):
        r = os.path.relpath(p, CB).replace(os.sep, "/")
        if not r.endswith("_test.cpp"): out.append(r)
    return out
while st:
    rel = st.pop()
    if rel in seen: continue
    seen.add(rel)
    p = os.path.join(CB, rel)
    if not os.path.exists(p): continue
    txt = open(p, encoding="utf-8", errors="replace").read()
    flag = []
    for m in inc.finditer(txt):
        h = m.group(1)
        if re.search(r'(^|/)py[a-z_]*\.hpp$|^codebreaker/|ported/|pyrt|py_bridge|compat/|old_', h):
            flag.append(h)
        if h.startswith("cb/"):
            for s in siblings(h[3:]):
                if s not in seen and not s.endswith("_test.cpp"): st.append(s)
    if re.search(r'\bpy::|pyengine|native/typed', txt): flag.append("py:: / pyengine text")
    if flag: bad.append((rel, flag))
print("files in closure:", len(seen))
for r, f in sorted(bad): print("  FLAG", r, f)
if not bad: print("no file in the closure includes the Python layer or uses py::")
areas = {}
for r in seen: areas[r.split("/")[0]] = areas.get(r.split("/")[0], 0) + 1
print(sorted(areas.items()))
