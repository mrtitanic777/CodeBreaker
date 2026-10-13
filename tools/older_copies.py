#!/usr/bin/env python3
"""older_copies.py -- per-version copies of a shared module, by how little they add to it.

    python tools/older_copies.py [--top N] [--show N]

A tool directory for one Windows version (src/targets/<ver>/) or for the Xbox tool
(src/xbox/xbt/*) often holds an older copy of a module that has since moved to the shared
package and grown there. This lists every such copy with the number of code lines it has that
the shared module does not: a copy with none is simply an old version and can become a pointer
(tools/alias_fork.py); one with a handful shows exactly what must be carried over first.
Comments, blank lines and docstring wording are ignored.
"""
import difflib
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import alias_fork                                                       # noqa: E402

ROOT = alias_fork.ROOT
_SKIP = ("__pycache__", "PEExplorerV2", "/samples/")
_PER_VERSION = ("/xbox/xbt", "/targets/")
_NOT_SHARED = _PER_VERSION + ("/xbox", "engines/inverse")


def _code(path):
    out = []
    for line in open(path, encoding="utf-8", errors="replace").read().splitlines():
        s = line.strip()
        if s and not s.startswith("#"):
            out.append(s)
    return out


def scan():
    shared, copies = {}, []
    for base, _dirs, files in os.walk(os.path.join(ROOT, "src")):
        b = "/" + os.path.relpath(base, ROOT).replace("\\", "/")
        if any(s in b + "/" for s in _SKIP):
            continue
        for f in files:
            if not f.endswith(".py") or f == "__init__.py":
                continue
            rel = b[1:] + "/" + f
            if any(s in b for s in _PER_VERSION):
                copies.append(rel)
            elif not any(s in b for s in _NOT_SHARED):
                shared.setdefault(f, []).append(rel)
    rows = []
    for rel in copies:
        f = os.path.basename(rel)
        if len(shared.get(f, ())) != 1 or alias_fork.is_alias(os.path.join(ROOT, rel)):
            continue
        mine, theirs = _code(os.path.join(ROOT, rel)), _code(os.path.join(ROOT, shared[f][0]))
        only = [x[2:] for x in difflib.ndiff(theirs, mine) if x.startswith("+ ")]
        rows.append((len(only), len(mine), rel, shared[f][0], only))
    return sorted(rows)


def main(argv):
    def opt(flag, default):
        return int(argv[argv.index(flag) + 1]) if flag in argv else default
    rows = scan()
    show = opt("--show", 7)
    for n, size, rel, canon, only in rows[:opt("--top", 40)]:
        print("%4d of %4d lines only in the copy   %s  ->  %s" % (n, size, rel[4:], canon[4:]))
        if n <= show:
            for line in only:
                print("          + " + line[:118])
    print("%d copies of a shared module remain" % len(rows))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
