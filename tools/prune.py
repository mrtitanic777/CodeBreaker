#!/usr/bin/env python3
"""prune.py -- remove what nothing uses, until nothing more can be removed.

    python tools/prune.py            list top-level functions, classes and constants nothing uses
    python tools/prune.py --apply    remove them, repeat until none are left, then remove modules
                                     nothing can reach (tools/unreachable.py)

A top-level name is unused when it appears nowhere in the package, the tests or the tools except
where it is defined (any appearance counts, also inside a string, so something reached by name
is kept; the package's .json files count too: a toolchain profile names its plugin class). Removing one can leave another unused, hence the repetition. Left alone: `main`,
tests, names starting and ending with two underscores, and anything in an `__init__.py`.

Run the tests afterwards; git has everything that was removed.
"""
import ast
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
WORD = re.compile(r"[A-Za-z_][A-Za-z0-9_]*")
# Third-party code kept as its authors wrote it: never pruned, its imports never trimmed.
VENDORED = ("codebreaker/formats/xbox/crypto/",)


def _files(kinds=(".py",)):
    for d in ("codebreaker", "tests", "tools"):
        for base, dirs, files in os.walk(os.path.join(ROOT, d)):
            dirs[:] = [x for x in dirs if x != "__pycache__"]
            for f in files:
                if f.endswith(kinds):
                    yield os.path.join(base, f)


def find():
    """[(path, name, first line, last line)] of unused top-level definitions in the package."""
    count, texts = {}, {}
    for p in _files((".py", ".json")):
        texts[p] = open(p, encoding="utf-8", errors="replace").read()
        for w in WORD.findall(texts[p]):
            count[w] = count.get(w, 0) + 1
    out = []
    for p, text in texts.items():
        rel = os.path.relpath(p, ROOT).replace("\\", "/")
        if not rel.startswith("codebreaker/") or not rel.endswith(".py") or rel.endswith("__init__.py") or rel.endswith("__main__.py") or rel.startswith(VENDORED):
            continue
        try:
            tree = ast.parse(text)
        except SyntaxError:
            continue
        lines = text.split("\n")
        for node in tree.body:
            if isinstance(node, (ast.FunctionDef, ast.ClassDef)):
                names = [node.name]
            elif isinstance(node, ast.Assign) and all(isinstance(t, ast.Name) for t in node.targets):
                names = [t.id for t in node.targets]
            else:
                continue
            if any(n == "main" or n.startswith("test") or (n.startswith("__") and n.endswith("__")) for n in names):
                continue
            own = len(re.findall(r"\b(?:%s)\b" % "|".join(map(re.escape, names)),
                                 "\n".join(lines[node.lineno - 1:node.end_lineno])))
            if sum(count.get(n, 0) for n in names) > own:
                continue                                     # used somewhere outside its own body
            first = node.lineno - 1
            if getattr(node, "decorator_list", None):
                first = node.decorator_list[0].lineno - 1
            while first > 0 and lines[first - 1].lstrip().startswith("#"):
                first -= 1
            out.append((p, names[0], first, node.end_lineno))
    return out


def apply(found):
    by_file = {}
    for p, _n, a, b in found:
        by_file.setdefault(p, []).append((a, b))
    for p, cuts in by_file.items():
        lines = open(p, encoding="utf-8", errors="replace").read().split("\n")
        for a, b in sorted(cuts, reverse=True):
            del lines[a:b]
        text = re.sub(r"\n{4,}", "\n\n\n", "\n".join(lines))
        with open(p, "w", encoding="utf-8", newline="\n") as f:
            f.write(text)


def main(argv):
    removed = lines = 0
    while True:
        found = find()
        if not found:
            break
        if "--apply" not in argv:
            for p, n, a, b in sorted(found, key=lambda x: x[2] - x[3]):
                print("%5d  %s  %s" % (b - a, os.path.relpath(p, ROOT).replace("\\", "/"), n))
            print("%d unused definitions, %d lines (first pass; more may follow)" % (
                len(found), sum(b - a for _p, _n, a, b in found)))
            return 0
        apply(found)
        removed += len(found)
        lines += sum(b - a for _p, _n, a, b in found)
    print("removed %d definitions, %d lines" % (removed, lines))
    if "--apply" in argv:
        sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
        import unreachable
        unreachable.main(["--delete"])
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
