"""twin7.py FILE...: each python test function -> the best native CB_TEST name by shared words (a hint, not proof)."""
import ast, re, sys, glob, os
nat = {}
for f in glob.glob("native/cb/**/*_test.cpp", recursive=True):
    t = open(f, encoding="utf-8", errors="replace").read()
    for m in re.finditer(r'CB_TEST\("([^"]+)"', t):
        nat[m.group(1)] = f
words = lambda s: {w for w in re.split(r"[^a-z0-9]+", s.lower()) if len(w) > 2 and w not in ("test", "the", "and", "with", "from", "for", "that", "its")}
nw = {n: words(n) for n in nat}
tot = bad = 0
for path in sys.argv[1:]:
    t = ast.parse(open(path, encoding="utf-8").read())
    fns = []
    for n in ast.walk(t):
        if isinstance(n, ast.FunctionDef) and n.name.startswith("test"):
            fns.append(n.name)
    miss = []
    for fn in fns:
        w = words(fn)
        best = max(nw, key=lambda n: len(w & nw[n]) / max(1, len(w)))
        sc = len(w & nw[best]) / max(1, len(w))
        tot += 1
        if sc < 0.5:
            miss.append((fn, round(sc, 2), best)); bad += 1
    print("%-45s %3d cases, %d weak" % (os.path.basename(path), len(fns), len(miss)))
    for m in miss: print("      ", m)
print(tot, "cases,", bad, "weak")
