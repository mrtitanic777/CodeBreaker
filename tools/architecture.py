#!/usr/bin/env python3
"""architecture.py -- the size, shape and coupling of every subsystem, measured.

    python tools/architecture.py [--json out.json] [--edges] [--hot N]

For each subsystem (a directory of the package, or a native tree) it reports: files and lines
of real code, how many files are only pointers to a shared module, how many are scripts run by
name, lines that are copies of code elsewhere, lines nothing can reach, which subsystems it
imports and which import it, its largest files and its longest functions. Read top-down it
answers where the code is; read by column it answers where the waste and the knots are.
"""
import ast
import json
import os
import re
import sys
from collections import Counter, defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import alias_fork                                                       # noqa: E402
import dead_code                                                        # noqa: E402
import dup_functions                                                    # noqa: E402

ROOT = alias_fork.ROOT
_NATIVE_EXT = (".c", ".cc", ".cpp", ".h", ".hh", ".hpp")
_VENDORED = ("native/ghidra_core/cpp", "native/ghidra_core/zlib", "native/ghidra_core/languages",
             "third_party", "/vendor/", "src/PEExplorerV2")


def subsystem(rel):
    """The subsystem a tracked file belongs to."""
    p = rel.split("/")
    if p[0] == "src" and len(p) > 2:
        if p[1] in ("engines", "toolchain", "targets") and len(p) > 3:
            return "/".join(p[:3])
        if p[1] == "xbox":
            return "/".join(p[:4]) if len(p) > 4 and p[2] == "xbt" else "src/xbox"
        return "/".join(p[:2])
    if p[0] == "native" and len(p) > 2:
        return "/".join(p[:2])
    return p[0] if len(p) > 1 else "(root)"


def _code_lines(text):
    return sum(1 for ln in text.split("\n") if ln.strip() and not ln.strip().startswith(("#", "//")))


def _imports(rel, tree, by_name):
    """Subsystems `rel` imports: `src.a.b` paths, and bare names resolved through the package."""
    out = set()
    for node in ast.walk(tree):
        names = []
        if isinstance(node, ast.Import):
            names = [a.name for a in node.names]
        elif isinstance(node, ast.ImportFrom) and node.module:
            names = [node.module] + ["%s.%s" % (node.module, a.name) for a in node.names]
        elif isinstance(node, ast.Call) and getattr(node.func, "attr", "") == "get" \
                and getattr(getattr(node.func, "value", None), "id", "") == "cores" and node.args \
                and isinstance(node.args[0], ast.Constant):
            names = [str(node.args[0].value)]
        for name in names:
            if name.startswith("src."):
                path = "/".join(name.split("."))
                for cand in (path + ".py", path + "/__init__.py"):
                    if os.path.isfile(os.path.join(ROOT, cand)):
                        out.add(subsystem(cand))
                        break
            elif name in by_name and len(by_name[name]) == 1:
                out.add(subsystem(by_name[name][0]))
    out.discard(subsystem(rel))
    return out


def measure():
    files = alias_fork.tracked()
    by_name = defaultdict(list)
    for rel in files:
        if rel.startswith("src/") and rel.endswith(".py") and not any(v in rel for v in _VENDORED) \
                and not alias_fork.is_alias(os.path.join(ROOT, rel)) and "/targets/" not in rel \
                and "/xbox/" not in rel:
            by_name[os.path.basename(rel)[:-3]].append(rel)
    S = defaultdict(lambda: dict(files=0, lines=0, aliases=0, commands=0, native=0, vendored=0, copies=0,
                                 dead=0, uses=set(), big=[], long=[]))
    for rel in files:
        if not rel.endswith((".py",) + _NATIVE_EXT):
            continue
        sub = subsystem(rel)
        path = os.path.join(ROOT, rel)
        try:
            text = open(path, encoding="utf-8", errors="replace").read()
        except OSError:
            continue
        n = _code_lines(text)
        s = S[sub]
        if any(v in "/" + rel for v in _VENDORED):
            s["vendored"] += n
            continue
        if not rel.endswith(".py"):
            s["native"] += n
            s["files"] += 1
            s["big"].append((n, rel))
            continue
        if alias_fork.is_alias(path):
            s["aliases"] += 1
            continue
        s["files"] += 1
        s["lines"] += n
        s["big"].append((n, rel))
        if "__main__" in text:
            s["commands"] += 1
        try:
            tree = ast.parse(text)
        except SyntaxError:
            continue
        s["uses"] |= _imports(rel, tree, by_name)
        for node in ast.walk(tree):
            if isinstance(node, (ast.FunctionDef, ast.AsyncFunctionDef)):
                ln = (node.end_lineno or node.lineno) - node.lineno + 1
                if ln >= 120:
                    s["long"].append((ln, "%s:%d %s" % (rel, node.lineno, node.name)))
    for group in dup_functions.scan(8, False).values():
        for path, _name, _line, size in sorted(group)[1:]:           # all but one are the copies
            S[subsystem(path)]["copies"] += size
    uses, defs = dead_code.scan()
    mods, funcs = dead_code.dead(uses, defs)
    for kind, size, rel, _why in dead_code.classify(mods):
        if kind != "command":
            S[subsystem(rel)]["dead"] += size
    for size, rel, _name, _line in funcs:
        S[subsystem(rel)]["dead"] += size
    used_by = defaultdict(set)
    for sub, s in S.items():
        for u in s["uses"]:
            used_by[u].add(sub)
    for sub, s in S.items():
        s["used_by"] = used_by.get(sub, set())
    return S


def main(argv):
    S = measure()
    hot = int(argv[argv.index("--hot") + 1]) if "--hot" in argv else 12
    rows = sorted(S.items(), key=lambda kv: -(kv[1]["lines"] + kv[1]["native"]))
    tot = Counter()
    print("%-28s %5s %7s %7s %5s %5s %6s %5s %4s %4s" % (
        "subsystem", "files", "python", "native", "ptrs", "cmds", "copies", "dead", "uses", "used"))
    for sub, s in rows:
        if not (s["lines"] or s["native"] or s["aliases"]):
            continue
        print("%-28s %5d %7d %7d %5d %5d %6d %5d %4d %4d" % (
            sub, s["files"], s["lines"], s["native"], s["aliases"], s["commands"], s["copies"], s["dead"],
            len(s["uses"]), len(s["used_by"])))
        for k in ("files", "lines", "native", "aliases", "commands", "copies", "dead", "vendored"):
            tot[k] += s[k]
    print("%-28s %5d %7d %7d %5d %5d %6d %5d" % ("TOTAL", tot["files"], tot["lines"], tot["native"],
                                                 tot["aliases"], tot["commands"], tot["copies"], tot["dead"]))
    print("(vendored third-party code, not counted above: %d lines)" % tot["vendored"])
    big = sorted((x for _s, s in rows for x in s["big"]), reverse=True)[:hot]
    print("\nlargest files:")
    for n, rel in big:
        print("  %6d  %s" % (n, rel))
    longf = sorted((x for _s, s in rows for x in s["long"]), reverse=True)
    print("\nfunctions of 120+ lines: %d; longest:" % len(longf))
    for n, where in longf[:hot]:
        print("  %6d  %s" % (n, where))
    if "--edges" in argv:
        print("\nwho imports whom:")
        for sub, s in rows:
            if s["uses"]:
                print("  %-28s -> %s" % (sub, ", ".join(sorted(x.replace("src/", "") for x in s["uses"]))))
    if "--json" in argv:
        out = {sub: {k: (sorted(v) if isinstance(v, set) else v) for k, v in s.items()
                     if k not in ("big", "long")} for sub, s in S.items()}
        with open(argv[argv.index("--json") + 1], "w", encoding="utf-8") as f:
            json.dump(out, f, indent=1)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
