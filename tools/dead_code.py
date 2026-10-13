#!/usr/bin/env python3
"""dead_code.py -- modules nothing refers to, and functions nothing calls.

    python tools/dead_code.py [--modules] [--functions] [--top N]

A module is reported when its name appears nowhere else in the repository: not in an import,
not in a `cores.get('name')`, not in a command table, a test, a tool or a script string. Each
is then sorted into one of three kinds:

    broken      it cannot run: a syntax error, or it imports a module that no longer exists
    no entry    nothing refers to it and it has no `__main__` block, so nothing can reach it
    command     it is a script someone runs by name (`wt --target win95 <name>` runs any .py in
                a tool directory): unreferenced, but alive

Only the first two are dead. A
top-level function is reported when its name appears nowhere but at its own definition (and it
is not a `main`, a test or a private hook the module itself runs). Names are matched as whole
words in every .py/.json/.md/.toml/.cfg/.bat/.ps1/.sh file, so anything reached by a string --
a command name, a dynamic import -- still counts as used. What is listed is therefore safe to
look at first; it is a list of candidates, not a verdict: a script meant to be run by hand has
no caller either.
"""
import ast
import os
import re
import sys
from collections import defaultdict

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
_SKIP_DIRS = ("__pycache__", ".git", "PEExplorerV2", "node_modules", "build", "third_party", "vendor",
              "ghidra_core", "toolchains", "binaries")
_TEXT = (".py", ".json", ".md", ".toml", ".cfg", ".bat", ".ps1", ".sh", ".txt", ".spec", ".yml", ".yaml")
_WORD = re.compile(r"[A-Za-z_][A-Za-z0-9_]*")


def _files():
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import alias_fork
    for rel in alias_fork.tracked():
        if rel.endswith(_TEXT) and not any(d in rel.split("/") for d in _SKIP_DIRS):
            p = os.path.join(ROOT, rel)
            try:
                if os.path.getsize(p) < 3_000_000:
                    yield p
            except OSError:
                pass


def scan():
    """-> (uses, defs): uses[word] = set of files it occurs in; defs = [(file, kind, name, line, lines)]."""
    uses, defs = defaultdict(set), []
    for p in _files():
        try:
            text = open(p, encoding="utf-8", errors="replace").read()
        except OSError:
            continue
        rel = os.path.relpath(p, ROOT).replace("\\", "/")
        for w in set(_WORD.findall(text)):
            uses[w].add(rel)
        if not (rel.startswith("src/") and rel.endswith(".py")) or "/engines/inverse/" in rel:
            continue
        name = os.path.basename(rel)[:-3]
        if name != "__init__":
            defs.append((rel, "module", name, 1, text.count("\n") + 1))
        try:
            tree = ast.parse(text)
        except SyntaxError:
            continue
        for node in tree.body:
            if isinstance(node, (ast.FunctionDef, ast.AsyncFunctionDef)):
                defs.append((rel, "function", node.name, node.lineno,
                             (node.end_lineno or node.lineno) - node.lineno + 1))
    return uses, defs


def _own_uses(rel, name):
    """How often `name` occurs in its own file (a function called by its own module is used)."""
    try:
        text = open(os.path.join(ROOT, rel), encoding="utf-8", errors="replace").read()
    except OSError:
        return 0
    return len(re.findall(r"\b%s\b" % re.escape(name), text))


def dead(uses, defs):
    mods, funcs = [], []
    for rel, kind, name, line, size in defs:
        elsewhere = uses.get(name, set()) - {rel}
        if kind == "module":
            # same-named siblings in other version folders are copies, not users
            real = {u for u in elsewhere if os.path.basename(u) != os.path.basename(rel)}
            if not real:
                mods.append((size, rel))
        elif not elsewhere and not name.startswith("test") and name not in ("main", "_selftest", "selftest"):
            if _own_uses(rel, name) <= 1:
                funcs.append((size, rel, name, line))
    return sorted(mods, reverse=True), sorted(funcs, reverse=True)


def _missing_imports(rel, text):
    """Modules `rel` imports that cannot be found: neither in the package, beside the file, in a
    tool directory of the same family, nor installed."""
    import importlib.util
    try:
        tree = ast.parse(text)
    except SyntaxError as e:
        return ["syntax error line %s" % e.lineno]
    here = os.path.dirname(os.path.join(ROOT, rel))
    family = [here] + [os.path.join(os.path.dirname(here), d) for d in ("core", "win", "match", "ppc", "xbox",
                                                                           "crypto", "cli", "gui")]
    guarded = set()                                          # imports inside try: are optional
    for node in ast.walk(tree):
        if isinstance(node, ast.Try):
            for sub in ast.walk(node):
                if isinstance(sub, (ast.Import, ast.ImportFrom)):
                    guarded.add(id(sub))
    out = []
    for node in ast.walk(tree):
        if id(node) in guarded:
            continue
        names = []
        if isinstance(node, ast.Import):
            names = [a.name for a in node.names]
        elif isinstance(node, ast.ImportFrom) and node.module and not node.level:
            names = [node.module]
        for name in names:
            top = name.split(".")[0]
            if top == "src":
                path = os.path.join(ROOT, *name.split("."))
                if not (os.path.isfile(path + ".py") or os.path.isdir(path)):
                    out.append(name)
                continue
            if any(os.path.isfile(os.path.join(d, top + ".py")) or os.path.isdir(os.path.join(d, top))
                   for d in family):
                continue
            if top in sys.builtin_module_names or top in getattr(sys, "stdlib_module_names", ()):
                continue
            try:
                if importlib.util.find_spec(top) is None:
                    out.append(name)
            except (ImportError, ValueError):
                out.append(name)
    return sorted(set(out))


def classify(mods):
    """[(lines, rel)] -> [(kind, lines, rel, why)], kind in broken / no entry / command."""
    out = []
    for size, rel in mods:
        text = open(os.path.join(ROOT, rel), encoding="utf-8", errors="replace").read()
        missing = _missing_imports(rel, text)
        if missing:
            out.append(("broken", size, rel, "imports " + ", ".join(missing[:4])))
        elif "__main__" not in text:
            out.append(("no entry", size, rel, "no caller and no __main__"))
        else:
            out.append(("command", size, rel, ""))
    return out


def main(argv):
    top = int(argv[argv.index("--top") + 1]) if "--top" in argv else 40
    uses, defs = scan()
    mods, funcs = dead(uses, defs)
    both = "--modules" not in argv and "--functions" not in argv
    if both or "--modules" in argv:
        print("%d modules nothing refers to (%d lines):" % (len(mods), sum(s for s, _r in mods)))
        by_dir = defaultdict(lambda: [0, 0])
        for s, r in mods:
            d = os.path.dirname(r)
            by_dir[d][0] += 1
            by_dir[d][1] += s
        for d, (n, s) in sorted(by_dir.items(), key=lambda x: -x[1][1])[:20]:
            print("  %5d lines in %3d files  %s" % (s, n, d))
        kinds = classify(mods)
        for kind in ("broken", "no entry", "command"):
            rows = [k for k in kinds if k[0] == kind]
            print("  %s: %d modules, %d lines" % (kind, len(rows), sum(k[1] for k in rows)))
            for _k, s, r, why in rows[:top]:
                print("    %5d  %s%s" % (s, r, ("   -- " + why) if why and kind == "broken" else ""))
    if both or "--functions" in argv:
        print("%d top-level functions nothing calls (%d lines):" % (len(funcs), sum(f[0] for f in funcs)))
        for s, r, n, ln in funcs[:top]:
            print("    %5d  %s:%d %s" % (s, r, ln, n))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
