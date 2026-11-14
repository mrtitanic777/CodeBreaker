"""Module import graph of codebreaker/, plus who in tests/ and tools/ imports what; live-set closure."""
import ast, os, sys, json, re, collections

ROOT = os.getcwd()


def modname(path):
    p = os.path.relpath(path, ROOT)[:-3].replace(os.sep, ".")
    return p[:-9] if p.endswith(".__init__") else p


def walk_py(top):
    out = {}
    for d, _, fs in os.walk(os.path.join(ROOT, top)):
        for f in fs:
            if f.endswith(".py"):
                out[os.path.join(d, f)] = 1
    return list(out)


mods = {modname(p): p for p in walk_py("codebreaker")}


def resolve(name):
    parts = name.split(".")
    out = []
    for i in range(1, len(parts) + 1):
        n = ".".join(parts[:i])
        if n in mods:
            out.append(n)
    return out


def imports(path, me):
    try:
        tree = ast.parse(open(path, encoding="utf-8", errors="replace").read())
    except Exception:
        return set()
    is_pkg = path.endswith("__init__.py")
    found = set()
    for n in ast.walk(tree):
        if isinstance(n, ast.Import):
            for a in n.names:
                found.update(resolve(a.name))
        elif isinstance(n, ast.ImportFrom):
            if n.level:
                parts = me.split(".")
                if not is_pkg:
                    parts = parts[:-1]
                if n.level > 1:
                    parts = parts[:len(parts) - (n.level - 1)]
                base = ".".join(parts + ([n.module] if n.module else []))
            else:
                base = n.module or ""
            found.update(resolve(base))
            for a in n.names:
                found.update(resolve(base + "." + a.name))
        elif isinstance(n, ast.Constant) and isinstance(n.value, str):
            v = n.value
            if re.fullmatch(r"codebreaker(\.\w+)+", v):
                found.update(resolve(v))
    found.discard(me)
    return found


graph = {m: imports(p, m) for m, p in mods.items()}
rev = collections.defaultdict(set)
for m, deps in graph.items():
    for d in deps:
        rev[d].add(m)

# external importers (tests, tools, native/cb generators, anything else .py outside codebreaker)
ext = collections.defaultdict(set)
ext_files = []
for top in ("tests", "tools", "native", "experiments", "targets", "docs"):
    for p in walk_py(top):
        rel = os.path.relpath(p, ROOT).replace(os.sep, "/")
        if "/build/" in rel or "/third_party/" in rel:
            continue
        ext_files.append(rel)
        for m in imports(p, "__ext__"):
            ext[m].add(rel)

json.dump({"graph": {m: sorted(d) for m, d in graph.items()},
           "ext": {m: sorted(f) for m, f in ext.items()},
           "files": {m: os.path.relpath(p, ROOT).replace(os.sep, "/") for m, p in mods.items()}},
          open(os.path.join(ROOT, "state", "work", "typed-port", "python_reach", "graph.json"), "w"), indent=0)
print(len(mods), "modules,", len(ext_files), "external py files")
