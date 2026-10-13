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
    print("%d passed, %d failed  (%d fail to import, %d bare-name imports, %d old names)" % (
        sum(1 for _ in modules()) - bad, bad + bare + old, bad, bare, old))
    return 1 if bad or bare or old else 0


if __name__ == "__main__":
    sys.exit(main())
