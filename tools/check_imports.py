#!/usr/bin/env python3
"""check_imports.py -- import every module of the package and list what fails.

    python tools/check_imports.py

Also reports imports written by bare name (`import target`) -- the old tool-directory style,
which only works when that directory happens to be on sys.path -- and any remaining reference
to the old package name.
"""
import ast
import importlib
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)


def modules():
    for base, dirs, files in os.walk(os.path.join(ROOT, "codebreaker")):
        dirs[:] = [d for d in dirs if d != "__pycache__"]
        for f in sorted(files):
            if f.endswith(".py"):
                rel = os.path.relpath(os.path.join(base, f), ROOT)[:-3].replace(os.sep, ".")
                yield rel[:-9] if rel.endswith(".__init__") else rel, os.path.join(base, f)


def dangling():
    """Imports of the package's own modules, anywhere (also inside functions, also in the tests
    and tools), that name a module or a name that is not there."""
    out = []
    for d in ("codebreaker", "tests", "tools"):
        for base, dirs, files in os.walk(os.path.join(ROOT, d)):
            dirs[:] = [x for x in dirs if x != "__pycache__"]
            for f in files:
                if not f.endswith(".py"):
                    continue
                p = os.path.join(base, f)
                try:
                    tree = ast.parse(open(p, encoding="utf-8", errors="replace").read())
                except SyntaxError:
                    continue
                for node in ast.walk(tree):
                    if not (isinstance(node, ast.ImportFrom) and node.module and node.module.startswith("codebreaker")):
                        continue
                    try:
                        mod = importlib.import_module(node.module)
                    except Exception as e:
                        out.append("%s:%d  %s: %s" % (os.path.relpath(p, ROOT), node.lineno, node.module, type(e).__name__))
                        continue
                    for a in node.names:
                        if hasattr(mod, a.name):
                            continue
                        try:
                            importlib.import_module(node.module + "." + a.name)
                        except Exception:
                            out.append("%s:%d  %s has no %s" % (os.path.relpath(p, ROOT), node.lineno, node.module, a.name))
    return out


def main():
    local = {os.path.basename(p)[:-3] for _m, p in modules()} - {"__init__"}
    bad = bare = old = 0
    for mod, path in modules():
        text = open(path, encoding="utf-8", errors="replace").read()
        for m in re.finditer(r"\bsrc\.(?:core|engines|targets|formats|toolchain|ir|disasm|cli|studio)\b[\w.]*", text):
            old += 1
            print("old name   %s: %s" % (mod, m.group(0)))
        try:
            tree = ast.parse(text)
        except SyntaxError as e:
            print("SYNTAX     %s: %s" % (mod, e))
            bad += 1
            continue
        std = getattr(sys, "stdlib_module_names", ())
        for node in ast.walk(tree):
            names = [a.name for a in node.names] if isinstance(node, ast.Import) else (
                [node.module] if isinstance(node, ast.ImportFrom) and node.module and not node.level else [])
            for n in names:
                top = n.split(".")[0]
                if top in local and top not in std and top != "codebreaker":
                    bare += 1
                    print("bare name  %s:%d import %s" % (mod, node.lineno, n))
        try:
            importlib.import_module(mod)
        except Exception as e:
            bad += 1
            print("FAILS      %s: %s: %s" % (mod, type(e).__name__, str(e)[:110]))
    lost = dangling()
    for line in lost:
        print("dangling   " + line.replace("\\", "/"))
    print("%d passed, %d failed  (%d fail to import, %d bare-name imports, %d old names, %d dangling imports)" % (
        sum(1 for _ in modules()) - bad, bad + bare + old + len(lost), bad, bare, old, len(lost)))
    return 1 if bad or bare or old or lost else 0


if __name__ == "__main__":
    sys.exit(main())
