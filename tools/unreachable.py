#!/usr/bin/env python3
"""unreachable.py -- modules of the package that no command, test or tool can reach.

    python tools/unreachable.py [--delete]

Starts from what a person can actually run -- the commands listed in codebreaker/__main__.py,
the tests, the tools (and their subfolders), and every module that runs as a program of its own
(`if __name__ == "__main__"`: `python -m codebreaker.x.y`) -- and follows imports, including those
made inside functions and those made by name:

    "codebreaker.a.b"            a string that is a module's name (`-m`, import_module, a command table)
    "codebreaker.a.b:name"       a plugin, in a string or a toolchain profile (exact/toolchains/*.json)
    "codebreaker.a." + name      a package whose modules are loaded by name: all of them

Anything in the package not reached that way is not part of the program: nothing runs it and
nothing checks it. `--delete` removes those files (git keeps them).
"""
import ast
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from prune import VENDORED                                               # noqa: E402
sys.path.insert(0, ROOT)


def _path(mod):
    p = os.path.join(ROOT, *mod.split("."))
    if os.path.isfile(p + ".py"):
        return p + ".py"
    if os.path.isfile(os.path.join(p, "__init__.py")):
        return os.path.join(p, "__init__.py")
    return None


_NAMED = re.compile(r"codebreaker(?:\.[A-Za-z_]\w*)+(?=:[A-Za-z_])")       # "codebreaker.a.b:name"


def _package(name):
    """Every module of a package, for a package whose modules are loaded by name."""
    init = _path(name)
    if not init or not init.endswith("__init__.py"):
        return set()
    return {name + "." + f[:-3] for f in os.listdir(os.path.dirname(init))
            if f.endswith(".py") and f != "__init__.py"}


def _imports(path, mod):
    text = open(path, encoding="utf-8", errors="replace").read()
    if path.endswith(".json"):
        return set(_NAMED.findall(text))
    out = set()
    tree = ast.parse(text)
    for node in ast.walk(tree):
        if isinstance(node, ast.Constant) and isinstance(node.value, str):
            # a string that is exactly a module name: `-m`, import_module(...), a command table
            if re.fullmatch(r"codebreaker(?:\.[A-Za-z_]\w*)+", node.value):
                out.add(node.value)
            # a package a name is added to: "codebreaker.platforms." + m
            elif re.fullmatch(r"codebreaker(?:\.[A-Za-z_]\w*)+\.", node.value):
                out |= _package(node.value[:-1])
            out |= set(_NAMED.findall(node.value))
        if isinstance(node, ast.ImportFrom) and node.module and node.module.startswith("codebreaker"):
            out.add(node.module)
            out |= {node.module + "." + a.name for a in node.names}
    return out


_MAIN = re.compile(r"""^if __name__ == ["']__main__["']\s*:""", re.M)


def reachable():
    roots = ["codebreaker.__main__", "codebreaker"]
    from codebreaker.__main__ import COMMANDS
    roots += [m for m, _a in COMMANDS.values()]
    seen, work = set(), list(roots)
    for d in ("tests", "tools"):
        for base, dirs, files in os.walk(os.path.join(ROOT, d)):
            dirs[:] = [x for x in dirs if x != "__pycache__"]
            work += [m for f in files if f.endswith(".py") for m in _imports(os.path.join(base, f), d)]
    # what the package loads by name from data (toolchain profiles), and every module that is a
    # program of its own
    for base, dirs, files in os.walk(os.path.join(ROOT, "codebreaker")):
        dirs[:] = [x for x in dirs if x != "__pycache__"]
        for f in files:
            p = os.path.join(base, f)
            if f.endswith(".json"):
                work += list(_imports(p, ""))
            elif f.endswith(".py") and _MAIN.search(open(p, encoding="utf-8", errors="replace").read()):
                work.append(os.path.relpath(p, ROOT)[:-3].replace(os.sep, ".").replace("/", "."))
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
            rel = os.path.relpath(p, ROOT).replace("\\", "/")
            if f.endswith(".py") and f != "__init__.py" and os.path.normcase(p) not in seen and not rel.startswith(VENDORED):
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
