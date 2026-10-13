#!/usr/bin/env python3
"""unreachable.py -- modules of the package that no command, test or tool can reach.

    python tools/unreachable.py [--delete]

Starts from what a person can actually run -- the commands listed in codebreaker/__main__.py,
the tests and the tools -- and follows imports (including those made inside functions and the
names handed to the registry). Anything in the package not reached that way is not part of the
program: nothing runs it and nothing checks it. `--delete` removes those files (git keeps them).
"""
import ast
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)


def _path(mod):
    p = os.path.join(ROOT, *mod.split("."))
    if os.path.isfile(p + ".py"):
        return p + ".py"
    if os.path.isfile(os.path.join(p, "__init__.py")):
        return os.path.join(p, "__init__.py")
    return None


def _imports(path, mod):
    text = open(path, encoding="utf-8", errors="replace").read()
    out = set()
    tree = ast.parse(text)
    for node in ast.walk(tree):
        # a string that is exactly a module name: `-m`, import_module(...), a command table
        if isinstance(node, ast.Constant) and isinstance(node.value, str) and \
                re.fullmatch(r"codebreaker(?:\.[A-Za-z_]\w*)+", node.value):
            out.add(node.value)
        if isinstance(node, ast.ImportFrom) and node.module and node.module.startswith("codebreaker"):
            out.add(node.module)
            out |= {node.module + "." + a.name for a in node.names}
    # registry.get("name") -> the shared module behind that name, and the target's profile
    from codebreaker.base import registry
    for name in re.findall(r"""(?:registry|cores|_registry)\.get\(\s*['"](\w+)['"]""", text):
        out.add(registry._SHARED.get(name, "codebreaker.targets.%s.profile" % registry.DEFAULT_TARGET))
    return out


def reachable():
    roots = ["codebreaker.__main__", "codebreaker"]
    from codebreaker.__main__ import COMMANDS
    roots += [m for m, _a in COMMANDS.values()]
    seen, work = set(), list(roots)
    for d in ("tests", "tools"):
        for f in os.listdir(os.path.join(ROOT, d)):
            if f.endswith(".py"):
                work += list(_imports(os.path.join(ROOT, d, f), d))
    while work:
        mod = work.pop()
        while mod and not _path(mod):                        # codebreaker.a.b.func -> codebreaker.a.b
            mod = mod.rpartition(".")[0]
        if not mod or mod in seen:
            continue
        seen.add(mod)
        parent = mod.rpartition(".")[0]
        if parent:
            work.append(parent)
        work += list(_imports(_path(mod), mod))
    return seen


def main(argv):
    seen = {os.path.normcase(_path(m)) for m in reachable()}
    dead = []
    for base, dirs, files in os.walk(os.path.join(ROOT, "codebreaker")):
        dirs[:] = [d for d in dirs if d != "__pycache__"]
        for f in files:
            p = os.path.join(base, f)
            if f.endswith(".py") and f != "__init__.py" and os.path.normcase(p) not in seen:
                dead.append(p)
    total = 0
    for p in sorted(dead):
        n = sum(1 for _ in open(p, encoding="utf-8", errors="replace"))
        total += n
        print("%5d  %s" % (n, os.path.relpath(p, ROOT).replace("\\", "/")))
        if "--delete" in argv:
            os.remove(p)
    print("%d modules, %d lines%s" % (len(dead), total, " -- deleted" if "--delete" in argv else " not reachable"))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
