#!/usr/bin/env python3
"""carve.py -- remove functions that a real run never calls and that nothing live refers to.

    python tools/carve.py            report
    python tools/carve.py --apply    remove them (then run tools/tidy.py --apply and the tests)

tools/prune.py removes a definition only when its name appears nowhere else. A module carried
over whole keeps whole clusters alive that way: its own `main` calls a dozen helpers that call
each other, and none of it is ever run. This looks at what RUNS:

  1. every command is exercised from scratch on small modules of each kind, in one process, with
     empty caches, while every call into the package is recorded;
  2. starting from the functions that were called -- plus whatever the tests, the tools, the
     command table and module-level code refer to -- everything they mention by name is kept,
     and what that mentions, and so on;
  3. a top-level function or class outside that set is removed.

Kept on purpose even if not called in this run: anything a called function mentions (error
handling, rarely taken branches), so removing by this report cannot break a path that is
reachable from code that runs.
"""
import ast
import os
import re
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PKG = os.path.join(ROOT, "codebreaker")
sys.path.insert(0, ROOT)


def exercise():
    """Run the commands on small inputs with cold caches; -> {(file, function name)} called."""
    state = tempfile.mkdtemp(prefix="cbcarve")
    os.environ["CB_STATE"] = state
    os.environ["CB_FUNCTIONS_DB"] = os.path.join(state, "functions.sqlite")
    os.environ["CB_JOBS"] = "1"                              # in this process, so calls are seen
    called = set()
    pkg = os.path.normcase(PKG)

    def prof(frame, event, arg):
        if event == "call":
            f = frame.f_code.co_filename
            if os.path.normcase(f).startswith(pkg):
                called.add((os.path.normcase(f), frame.f_code.co_name))
    from codebreaker.__main__ import main
    out = tempfile.mkdtemp(prefix="cbcarveout")
    runs = [["module", "version.dll", "--out", out], ["module", "lzexpand.dll", "--out", out],
            ["module", "version.dll", "--no-verify", "--out", out],
            ["measure", "lzexpand.dll"], ["decompile", "lzexpand.dll", "--out", os.path.join(out, "l.c")],
            ["decompile", "version.dll", "--out", os.path.join(out, "v.c")],
            ["sweep", "--only", "version.dll,lzexpand.dll", "--all"], ["sweep", "--status"], ["scoreboard", "--out", out]]
    sample = os.path.join(ROOT, "tests", "data", "powerpc.bin")
    if os.path.isfile(sample):
        runs.append(["decompile", sample, "--out", os.path.join(out, "p.c")])
    sys.setprofile(prof)
    try:
        for argv in runs:
            try:
                main(argv)
            except SystemExit:
                pass
    finally:
        sys.setprofile(None)
    return called


def _module_file(dotted):
    p = os.path.join(ROOT, *dotted.split("."))
    return os.path.normcase(p + ".py") if os.path.isfile(p + ".py") else None


def _aliases(tree):
    """What a file's imports bind: {local name: module file} for imported modules and
    {local name: (module file, name)} for names imported from a module."""
    mods, names = {}, {}
    for n in ast.walk(tree):
        if isinstance(n, ast.ImportFrom) and n.module and n.module.startswith("codebreaker"):
            for a in n.names:
                as_module = _module_file(n.module + "." + a.name)
                if as_module:
                    mods[a.asname or a.name] = as_module
                elif _module_file(n.module):
                    names[a.asname or a.name] = (_module_file(n.module), a.name)
    return mods, names


def _mentions(node, key_file, mods, names):
    """The definitions (file, name) a piece of code refers to, resolved through the file's
    imports: a bare name is this file's own or one imported by name; `alias.name` is a name in
    the module `alias` stands for."""
    out = set()
    for n in ast.walk(node):
        if isinstance(n, ast.Name):
            out.add(names.get(n.id, (key_file, n.id)))
        elif isinstance(n, ast.Attribute) and isinstance(n.value, ast.Name) and n.value.id in mods:
            out.add((mods[n.value.id], n.attr))
        elif isinstance(n, ast.Attribute):
            out.add((key_file, n.attr))                      # a method or attribute of its own classes
        elif isinstance(n, ast.Constant) and isinstance(n.value, str) and re.fullmatch(r"[A-Za-z_]\w*", n.value):
            out.add((key_file, n.value))                     # a name handed over as a string
    return out


def _defs():
    """({(file, name): entry}, root mentions). An entry is a top-level function, class or
    assignment of the package: (first line, end line, what it mentions, inner function names,
    path, kind). Roots are what is mentioned by code that runs on import, the tests and the tools."""
    defs, roots = {}, set()
    for d in ("codebreaker", "tests", "tools"):
        for base, dirs, files in os.walk(os.path.join(ROOT, d)):
            dirs[:] = [x for x in dirs if x != "__pycache__"]
            for f in files:
                if not f.endswith(".py"):
                    continue
                p = os.path.join(base, f)
                text = open(p, encoding="utf-8", errors="replace").read()
                try:
                    tree = ast.parse(text)
                except SyntaxError:
                    continue
                lines = text.split("\n")
                key_file = os.path.normcase(p)
                mods, names = _aliases(tree)
                for node in tree.body:
                    words = _mentions(node, key_file, mods, names)
                    in_pkg = d == "codebreaker" and not f.startswith("__")
                    if in_pkg and isinstance(node, (ast.FunctionDef, ast.ClassDef)):
                        first = node.lineno - 1
                        if node.decorator_list:
                            first = node.decorator_list[0].lineno - 1
                        while first > 0 and lines[first - 1].lstrip().startswith("#"):
                            first -= 1
                        inner = {n.name for n in ast.walk(node) if isinstance(n, (ast.FunctionDef, ast.ClassDef))}
                        defs[(key_file, node.name)] = (first, node.end_lineno, words - {(key_file, node.name)},
                                                       inner, p, "code")
                    elif in_pkg and isinstance(node, ast.Assign) and all(isinstance(t, ast.Name) for t in node.targets):
                        # a table or constant: it mentions things, but only matters if something
                        # live mentions IT
                        for t in node.targets:
                            defs[(key_file, t.id)] = (node.lineno - 1, node.end_lineno, words - {(key_file, t.id)},
                                                      set(), p, "data")
                    elif isinstance(node, (ast.Import, ast.ImportFrom)):
                        continue
                    elif not (isinstance(node, ast.If) and "__main__" in ast.dump(node.test)):
                        roots |= words                       # runs on import; or a test, or a tool
    return defs, roots


def find():
    called = exercise()
    defs, roots = _defs()
    from codebreaker.__main__ import COMMANDS                # a `main` is live only if it is a command
    commands = {_module_file(m) for m, _a in COMMANDS.values()}
    live, work = set(), []
    for key, (_a, _b, _w, inner, _p, _kind) in defs.items():
        f, name = key
        if key in roots or (name == "main" and f in commands) or any((f, n) in called for n in inner):
            work.append(key)
    while work:
        key = work.pop()
        if key in live or key not in defs:
            continue
        live.add(key)
        work.extend(defs[key][2])
    return [(defs[k][4], k[1], defs[k][0], defs[k][1]) for k in defs if k not in live and defs[k][5] == "code"]


def main(argv):
    dead = find()
    by_file = {}
    for p, name, a, b in sorted(dead, key=lambda x: (x[0], -x[2])):
        by_file.setdefault(p, []).append((a, b, name))
    total = 0
    for p, cuts in sorted(by_file.items(), key=lambda kv: -sum(b - a for a, b, _n in kv[1])):
        n = sum(b - a for a, b, _n in cuts)
        total += n
        print("%5d lines, %3d definitions  %s" % (n, len(cuts), os.path.relpath(p, ROOT).replace("\\", "/")))
        if "--names" in argv:
            print("        " + ", ".join(sorted(nm for _a, _b, nm in cuts)))
        if "--apply" in argv:
            lines = open(p, encoding="utf-8", errors="replace").read().split("\n")
            for a, b, _n in sorted(cuts, reverse=True):
                del lines[a:b]
            with open(p, "w", encoding="utf-8", newline="\n") as f:
                f.write(re.sub(r"\n{4,}", "\n\n\n", "\n".join(lines)))
    print("%d definitions, %d lines never run and not referred to by anything that is%s" % (
        len(dead), total, " -- removed" if "--apply" in argv else ""))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
