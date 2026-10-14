#!/usr/bin/env python3
"""merge.py -- fold one module of the package into another, or rename one.

    python tools/merge.py codebreaker.formats.pe_trace codebreaker.formats.pe_functions
    python tools/merge.py --check a.b.c a.b.d        only say whether names would collide

The first module's definitions are appended to the second (created if it does not exist), its
own imports are carried over, every import of it anywhere in the repository is pointed at the
second, and the first file is removed. Refused when both define the same top-level name: that
is a decision for a person, not a tool. Run tools/tidy.py --apply and the tests afterwards.
"""
import ast
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def _path(mod):
    return os.path.join(ROOT, *mod.split(".")) + ".py"


def _top_names(tree):
    out = set()
    for node in tree.body:
        if isinstance(node, (ast.FunctionDef, ast.ClassDef)):
            out.add(node.name)
        elif isinstance(node, ast.Assign):
            out |= {t.id for t in node.targets if isinstance(t, ast.Name)}
    return out


def _split(text):
    """(docstring, [import lines], rest) of a module."""
    tree = ast.parse(text)
    lines = text.split("\n")
    doc, start = "", 0
    body = tree.body
    if body and isinstance(body[0], ast.Expr) and isinstance(getattr(body[0], "value", None), ast.Constant) \
            and isinstance(body[0].value.value, str):
        doc = "\n".join(lines[body[0].lineno - 1:body[0].end_lineno])
        start = body[0].end_lineno
        body = body[1:]
    imports, cut = [], []
    for node in body:
        if isinstance(node, (ast.Import, ast.ImportFrom)):
            imports.append("\n".join(lines[node.lineno - 1:node.end_lineno]))
            cut.append((node.lineno - 1, node.end_lineno))
    rest = lines[start:]
    for a, b in sorted(cut, reverse=True):
        del rest[a - start:b - start]
    return doc, imports, "\n".join(rest).strip("\n")


def main(argv):
    check = "--check" in argv
    args = [a for a in argv if not a.startswith("--")]
    if len(args) != 2:
        print(__doc__)
        return 2
    src, dst = args
    ps, pd = _path(src), _path(dst)
    text_s = open(ps, encoding="utf-8").read()
    have = os.path.isfile(pd)
    text_d = open(pd, encoding="utf-8").read() if have else ""
    clash = _top_names(ast.parse(text_s)) & (_top_names(ast.parse(text_d)) if have else set())
    if clash:
        print("both define: " + ", ".join(sorted(clash)))
        return 1
    if check:
        print("no collisions")
        return 0
    doc_s, imp_s, rest_s = _split(text_s)
    # what the first module called the second (`from pkg import second as T` ... `T.name`) is now
    # simply `name`
    d_pkg, d_name = re.escape(dst.rsplit(".", 1)[0]), re.escape(dst.rsplit(".", 1)[1])
    aliases = re.findall(r"from %s import %s as (\w+)" % (d_pkg, d_name), text_s)
    if re.search(r"from %s import %s\b(?! as)" % (d_pkg, d_name), text_s):
        aliases.append(dst.rsplit(".", 1)[1])
    for alias in aliases:
        rest_s = re.sub(r"(?<![\w.])%s\.(?=[A-Za-z_])" % re.escape(alias), "", rest_s)
    if have:
        doc_d, imp_d, rest_d = _split(text_d)
        # ...and what the second called the first (`from pkg import first as V` ... `V.name`)
        s_pkg, s_name = re.escape(src.rsplit(".", 1)[0]), re.escape(src.rsplit(".", 1)[1])
        back = re.findall(r"from %s import %s as (\w+)" % (s_pkg, s_name), text_d)
        if re.search(r"from %s import %s\b(?! as)" % (s_pkg, s_name), text_d):
            back.append(src.rsplit(".", 1)[1])
        for alias in set(back):
            rest_d = re.sub(r"(?<![\w.])%s\.(?=[A-Za-z_])" % re.escape(alias), "", rest_d)
        rest_d = re.sub(r"(?m)^\s*from %s import %s(?: as \w+)?\s*(?:#.*)?\n" % (s_pkg, s_name), "", rest_d)
    else:
        doc_d, imp_d, rest_d = doc_s, [], ""
    self_import = re.compile(r"from %s import %s\b|import %s\b" % (
        re.escape(dst.rsplit(".", 1)[0]), re.escape(dst.rsplit(".", 1)[1]), re.escape(dst)))
    from_self = re.compile(r"from (?:%s|%s) import " % (re.escape(dst), re.escape(src)))
    imports = [i for i in imp_d if not from_self.match(i.strip())] + \
              [i for i in imp_s if i not in imp_d and not self_import.search(i) and not from_self.match(i.strip())]
    os.makedirs(os.path.dirname(pd), exist_ok=True)
    with open(pd, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(x for x in (doc_d, "\n".join(imports), "", "", rest_d, "", "", rest_s) if x is not None)
                .replace("\n\n\n\n\n", "\n\n\n").rstrip() + "\n")
    subprocess.run(["git", "rm", "-q", "-f", "--cached", os.path.relpath(ps, ROOT)], cwd=ROOT, capture_output=True)
    os.remove(ps)
    # point every user at the new module
    sp, sn = src.rsplit(".", 1)
    dp, dn = dst.rsplit(".", 1)
    n = 0
    for base, dirs, files in os.walk(ROOT):
        dirs[:] = [d for d in dirs if d not in (".git", "__pycache__", "native", "state")]
        for f in files:
            if not f.endswith((".py", ".md", ".json")) or f in ("ported.json", "merge.py"):
                continue
            p = os.path.join(base, f)
            t = open(p, encoding="utf-8", errors="replace").read()
            u = re.sub(r"from %s import %s as (\w+)" % (re.escape(sp), re.escape(sn)),
                       lambda m: "from %s import %s as %s" % (dp, dn, m.group(1)), t)
            u = re.sub(r"from %s import %s\b(?! as)" % (re.escape(sp), re.escape(sn)),
                       "from %s import %s%s" % (dp, dn, "" if dn == sn else " as " + sn), u)
            u = u.replace(src, dst)
            if u != t:
                n += 1
                with open(p, "w", encoding="utf-8", newline="\n") as fh:
                    fh.write(u)
    print("%s -> %s (%d files updated)" % (src, dst, n))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
