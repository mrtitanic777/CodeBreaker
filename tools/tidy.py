#!/usr/bin/env python3
"""tidy.py -- keep the package free of what nothing uses.

    python tools/tidy.py             report: unused imports, undefined names, unused definitions,
                                     modules nothing reaches   (exit code 1 if there are any)
    python tools/tidy.py --apply     remove them, repeating until nothing more comes loose

Removing one thing frees another: an import nothing uses kept a module reachable, that module
held the only callers of a function elsewhere, and so on. `--apply` runs the three steps in turn
until a whole pass removes nothing:

    1. imports a file does not use                       (pyflakes finds them)
    2. top-level functions, classes and constants whose name appears nowhere else   (tools/prune.py)
    3. modules no command, test or tool can reach        (tools/unreachable.py)

Undefined names are reported and never "fixed": they are bugs. Run the tests afterwards.
"""
import ast
import io
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import prune                                                            # noqa: E402
import unreachable                                                      # noqa: E402

_MSG = re.compile(r"^(.*?):(\d+):\d+:? (.*)$")


def flakes():
    """[(path, line, message)] from pyflakes over the package, tests and tools."""
    r = subprocess.run([sys.executable, "-m", "pyflakes", "codebreaker", "tests", "tools"], cwd=ROOT,
                       capture_output=True, text=True)
    out = []
    for line in (r.stdout or "").splitlines():
        m = _MSG.match(line)
        if m and not os.path.relpath(os.path.join(ROOT, m.group(1)), ROOT).replace("\\", "/").startswith(prune.VENDORED):
            out.append((os.path.join(ROOT, m.group(1)), int(m.group(2)), m.group(3)))
    return out


def _drop_import(path, wanted):
    """Remove the unused names `wanted` {line: {local name}} from a file's import statements."""
    text = open(path, encoding="utf-8").read()
    tree = ast.parse(text)
    lines = text.split("\n")
    edits = []
    for node in ast.walk(tree):
        if not isinstance(node, (ast.Import, ast.ImportFrom)) or node.lineno not in wanted:
            continue
        gone = wanted[node.lineno]
        keep = [a for a in node.names if (a.asname or a.name.split(".")[0] if isinstance(node, ast.Import)
                                           else a.asname or a.name) not in gone]
        if len(keep) == len(node.names):
            continue
        indent = re.match(r"\s*", lines[node.lineno - 1]).group(0)
        tail = lines[node.end_lineno - 1][node.end_col_offset:]
        if keep:
            names = ", ".join(a.name + (" as " + a.asname if a.asname else "") for a in keep)
            new = ("%sfrom %s%s import %s" % (indent, "." * node.level, node.module or "", names)
                   if isinstance(node, ast.ImportFrom) else "%simport %s" % (indent, names)) + tail
            edits.append((node.lineno, node.end_lineno, [new]))
        else:
            # an import that was the whole body of a block leaves `pass`
            edits.append((node.lineno, node.end_lineno, [indent + "pass" + tail] if _alone(tree, node) else []))
    for first, last, new in sorted(edits, reverse=True):
        lines[first - 1:last] = new
    with open(path, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(lines))
    return len(edits)


def _alone(tree, node):
    for parent in ast.walk(tree):
        for field in ("body", "orelse", "finalbody"):
            body = getattr(parent, field, None)
            if isinstance(body, list) and node in body:
                return len(body) == 1
    return False


def _noqa(path, line):
    """Is the import at `line` marked `# noqa` (an import made for what loading the module does)?
    pyflakes does not read those marks; the import statement may span lines, the mark is on its first."""
    with open(path, encoding="utf-8", errors="replace") as f:
        for n, text in enumerate(f, 1):
            if n == line:
                return "# noqa" in text
    return False


def unused_imports(apply=False):
    found = {}
    for path, line, msg in flakes():
        m = re.match(r"'(.+?)' imported but unused", msg)
        if m and not _noqa(path, line):
            local = m.group(1).split(" as ")[-1].split(".")[-1] if " as " in m.group(1) else m.group(1).split(".")[-1]
            found.setdefault(path, {}).setdefault(line, set()).add(local)
            # `import a.b` binds `a`; `from x import y` binds `y`
            found[path][line].add(m.group(1).split(" as ")[-1].split(".")[0])
    n = sum(len(v) for v in found.values())
    if apply:
        for path, wanted in found.items():
            _drop_import(path, wanted)
    return n, found


def main(argv):
    apply = "--apply" in argv
    total = 0
    for _pass in range(12):
        n_imp, found = unused_imports(apply)
        defs = prune.find()
        if apply and defs:
            prune.apply(defs)
        buf, old = io.StringIO(), sys.stdout
        sys.stdout = buf
        try:
            unreachable.main(["--delete"] if apply else [])
        finally:
            sys.stdout = old
        dead = [ln for ln in buf.getvalue().splitlines() if re.match(r"\s*\d+  codebreaker/", ln)]
        step = n_imp + len(defs) + len(dead)
        total += step
        if not apply:
            for path, wanted in sorted(found.items()):
                for line, names in sorted(wanted.items()):
                    print("unused import   %s:%d" % (os.path.relpath(path, ROOT).replace("\\", "/"), line))
            for p, n, a, b in defs:
                print("unused          %s  %s (%d lines)" % (os.path.relpath(p, ROOT).replace("\\", "/"), n, b - a))
            for ln in dead:
                print("unreachable    " + ln)
            break
        if not step:
            break
    undefined = [(p, ln, m) for p, ln, m in flakes() if "undefined name" in m or "unable to detect" in m]
    for p, ln, m in undefined:
        print("UNDEFINED       %s:%d %s" % (os.path.relpath(p, ROOT).replace("\\", "/"), ln, m))
    print("%d %s, %d undefined names" % (total, "removed" if apply else "to remove", len(undefined)))
    return 1 if undefined or (total and not apply) else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
