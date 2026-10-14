#!/usr/bin/env python3
"""move.py -- move named top-level definitions from one module to another.

    python tools/move.py codebreaker.verify.win32 codebreaker.verify.compare compare first_difference ...

The definitions (functions, classes, assignments, with the comments above them) are cut from the
first module and appended to the second (created with the first's imports if it does not
exist). References elsewhere are NOT rewritten -- which alias a caller uses for which module is
the caller's business; pyflakes (tools/tidy.py) then lists every place that needs pointing at
the new home, and those are fixed by hand or with a one-line substitution.
"""
import ast
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def _path(mod):
    return os.path.join(ROOT, *mod.split(".")) + ".py"


def main(argv):
    if len(argv) < 3:
        print(__doc__)
        return 2
    src, dst, names = argv[0], argv[1], set(argv[2:])
    ps, pd = _path(src), _path(dst)
    text = open(ps, encoding="utf-8").read()
    tree = ast.parse(text)
    lines = text.split("\n")
    cuts, found = [], set()
    for node in tree.body:
        own = []
        if isinstance(node, (ast.FunctionDef, ast.ClassDef)):
            own = [node.name]
        elif isinstance(node, ast.Assign):
            own = [t.id for t in node.targets if isinstance(t, ast.Name)]
        if not (set(own) & names):
            continue
        found |= set(own) & names
        first = node.lineno - 1
        if getattr(node, "decorator_list", None):
            first = node.decorator_list[0].lineno - 1
        while first > 0 and lines[first - 1].lstrip().startswith("#"):
            first -= 1
        cuts.append((first, node.end_lineno))
    if names - found:
        print("not found in %s: %s" % (src, ", ".join(sorted(names - found))))
        return 1
    moved = "\n\n\n".join("\n".join(lines[a:b]) for a, b in sorted(cuts))
    imports = [("\n".join(lines[n.lineno - 1:n.end_lineno])) for n in tree.body
               if isinstance(n, (ast.Import, ast.ImportFrom))]
    for a, b in sorted(cuts, reverse=True):
        del lines[a:b]
    with open(ps, "w", encoding="utf-8", newline="\n") as f:
        f.write(re.sub(r"\n{4,}", "\n\n\n", "\n".join(lines)))
    if os.path.isfile(pd):
        old = open(pd, encoding="utf-8").read().rstrip() + "\n\n\n"
    else:
        os.makedirs(os.path.dirname(pd), exist_ok=True)
        old = '"""%s"""\n' % dst.rsplit(".", 1)[1] + "\n".join(imports) + "\n\n\n"
    with open(pd, "w", encoding="utf-8", newline="\n") as f:
        f.write(old + moved + "\n")
    print("moved %d definitions: %s -> %s" % (len(cuts), src, dst))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
