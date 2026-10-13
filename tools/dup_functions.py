#!/usr/bin/env python3
"""dup_functions.py -- find functions defined more than once across the package.

    python tools/dup_functions.py [--min-lines N] [--top N] [--near]

Whole-file forks are found by stale_forks.py. This looks one level down: a helper that was
copied into several modules instead of being imported. Two functions are the same when their
bodies are the same code once names of local variables, docstrings and formatting are ignored
(`--near` also ignores the values of constants). Largest duplication first, so consolidation
goes where it removes the most.
"""
import ast
import hashlib
import os
import sys
from collections import defaultdict

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SKIP = ("__pycache__", "PEExplorerV2", os.path.join("engines", "inverse"))


class _Norm(ast.NodeTransformer):
    """Rename every local name by order of first appearance; optionally blank constants."""

    def __init__(self, near):
        self.names, self.near = {}, near

    def _n(self, name):
        return self.names.setdefault(name, "v%d" % len(self.names))

    def visit_arg(self, node):
        node.arg, node.annotation = self._n(node.arg), None
        return node

    def visit_Name(self, node):
        if isinstance(node.ctx, ast.Store) or node.id in self.names:
            node.id = self._n(node.id)
        return node

    def visit_Constant(self, node):
        if self.near and not isinstance(node.value, (bool, type(None))):
            node.value = 0
        return node


def _body_key(fn, near):
    body = list(fn.body)
    if body and isinstance(body[0], ast.Expr) and isinstance(getattr(body[0], "value", None), ast.Constant) \
            and isinstance(body[0].value.value, str):
        body = body[1:]                                     # the docstring is not code
    if not body:
        return None
    clone = ast.FunctionDef(name="f", args=fn.args, body=body, decorator_list=[], returns=None,
                            lineno=0, col_offset=0)
    clone = _Norm(near).visit(ast.fix_missing_locations(ast.parse(ast.unparse(clone))))
    return hashlib.sha1(ast.dump(clone, annotate_fields=False).encode()).hexdigest()


def scan(min_lines=8, near=False):
    groups = defaultdict(list)
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import alias_fork
    if True:
        for rel in alias_fork.tracked("src"):
            path = os.path.join(ROOT, rel)
            if not rel.endswith(".py") or any(s.replace(os.sep, "/") in rel for s in SKIP):
                continue
            try:
                text = open(path, encoding="utf-8", errors="replace").read()
                if "sys.modules[__name__]" in text and len(text) < 600:
                    continue                                # an alias shim, not a copy
                tree = ast.parse(text)
            except (SyntaxError, OSError):
                continue
            for node in ast.walk(tree):
                if isinstance(node, (ast.FunctionDef, ast.AsyncFunctionDef)):
                    lines = (node.end_lineno or node.lineno) - node.lineno + 1
                    if lines < min_lines:
                        continue
                    try:
                        key = _body_key(node, near)
                    except Exception:
                        continue
                    if key:
                        groups[key].append((os.path.relpath(path, ROOT).replace("\\", "/"), node.name,
                                            node.lineno, lines))
    return {k: v for k, v in groups.items() if len({p for p, _n, _l, _s in v}) > 1}


def main(argv):
    def opt(flag, default):
        return int(argv[argv.index(flag) + 1]) if flag in argv else default
    groups = scan(opt("--min-lines", 8), "--near" in argv)
    ranked = sorted(groups.values(), key=lambda v: -(len(v) - 1) * v[0][3])
    waste = sum((len(v) - 1) * v[0][3] for v in ranked)
    print("%d functions are defined in more than one file; %d lines are copies" % (len(ranked), waste))
    by_pair = defaultdict(int)
    for v in ranked:
        dirs = sorted({os.path.dirname(p) for p, _n, _l, _s in v})
        by_pair[" + ".join(dirs)] += (len(v) - 1) * v[0][3]
    print("\nwhere the copies are (lines):")
    for k, n in sorted(by_pair.items(), key=lambda x: -x[1])[:15]:
        print("%6d  %s" % (n, k))
    print("\nlargest:")
    for v in ranked[:opt("--top", 25)]:
        print("%4d lines x%d  %s" % (v[0][3], len(v), v[0][1]))
        for p, n, ln, _s in sorted(v):
            print("          %s:%d %s" % (p, ln, n))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
