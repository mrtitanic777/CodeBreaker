"""g7.py: import graph (static + string-literal module names) of codebreaker/, tests/, tools/.
usage: g7.py free  <deleted-file-globs...>   -> modules unreachable from kept roots once files are gone
       g7.py why <module>
"""
import ast, os, sys, json, fnmatch, collections
R = "."
def modname(path):
    p = path[:-3].replace("\\", "/").lstrip("./")
    parts = p.split("/")
    if parts[-1] == "__init__": parts = parts[:-1]
    return ".".join(parts)
files = {}
for top in ("codebreaker", "tests", "tools"):
    for d, _, fs in os.walk(top):
        if "__pycache__" in d: continue
        for f in fs:
            if f.endswith(".py"):
                p = os.path.join(d, f).replace("\\", "/")
                files[modname(p)] = p
def graph(deleted):
    g, weak = {}, {}
    mods = {m for m, p in files.items() if p not in deleted}
    for m in mods:
        p = files[m]
        try: t = ast.parse(open(p, encoding="utf-8", errors="replace").read())
        except Exception as e: continue
        pkg = m if p.endswith("__init__.py") else m.rsplit(".", 1)[0] if "." in m else ""
        out, wk = set(), set()
        def add(n, s=out):
            while n:
                if n in mods: s.add(n)
                n = n.rsplit(".", 1)[0] if "." in n else ""
        for n in ast.walk(t):
            if isinstance(n, ast.Import):
                for a in n.names: add(a.name)
            elif isinstance(n, ast.ImportFrom):
                base = n.module or ""
                if n.level:
                    parts = pkg.split(".") if pkg else []
                    parts = parts[:len(parts) - (n.level - 1)]
                    base = ".".join(parts + ([n.module] if n.module else []))
                add(base)
                for a in n.names: add(base + "." + a.name)
            elif isinstance(n, ast.Constant) and isinstance(n.value, str):
                v = n.value.split(":")[0]
                if v.startswith("codebreaker.") and len(v) < 120 and " " not in v: add(v, wk)
        if m == "codebreaker.java.projects":      # loaded by name (importlib 'codebreaker.java.projects.%s')
            wk |= {x for x in mods if x.startswith(m + ".") and x.count(".") == 3}
        g[m], weak[m] = out, wk
    return g, weak, mods
def closure(g, roots):
    seen, st = set(), list(roots)
    while st:
        m = st.pop()
        if m in seen or m not in g: continue
        seen.add(m); st.extend(g[m])
    return seen
def roots_of(mods):
    r = {m for m in mods if m.startswith("tests.") or m.startswith("tools.") or m in ("tests","tools")}
    r |= {m for m in mods if m == "codebreaker" or m == "codebreaker.__main__" or m.startswith("codebreaker.ui")}
    r |= {m for m in mods if m.startswith("codebreaker.platforms.")}                 # descriptors, loaded by file name
    return r
if __name__ == "__main__":
    cmd = sys.argv[1]
    dels = set()
    for pat in sys.argv[2:]:
        for p in files.values():
            if fnmatch.fnmatch(p, pat): dels.add(p)
    g, weak, mods = graph(dels)
    # tests: only those that are suite members matter, assume all present tests are roots
    for m in g: g[m] = g[m] | weak[m]
    seen = closure(g, roots_of(mods))
    if cmd == "free":
        base_g, base_w, base_m = graph(set())
        for m in base_g: base_g[m] |= base_w[m]
        base = closure(base_g, roots_of(base_m))
        free = sorted(m for m in base_g if m not in seen and m.startswith("codebreaker"))
        n = 0
        for m in free:
            l = sum(1 for _ in open(files[m], encoding="utf-8", errors="replace")); n += l
            print("%-60s %5d" % (files[m], l))
        print(len(free), "modules", n, "lines; deleted-by-glob files", len(dels))
    elif cmd == "why":
        t = sys.argv[2]
        pass
