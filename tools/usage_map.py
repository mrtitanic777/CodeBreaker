#!/usr/bin/env python3
"""usage_map.py -- measure how CodeBreaker's Python code is actually wired.

Builds the static import graph over every tracked src/**/*.py and reports, per package:
how many files and lines it has, how many of its files nothing else imports, and how much
is duplicated between the per-era targets and the shared engines. It is the evidence for
restructuring decisions: what to merge, what is a standalone command, what looks dead.

    python tools/usage_map.py                # summary tables
    python tools/usage_map.py --json out.json   # full per-file data

Edges resolved: absolute `src.` imports, relative imports, bare sibling imports (the
sys.path style used inside target dirs), `cores.get("name")` registry lookups, and literal
`importlib.import_module("src....")` calls. Dynamic dispatch by `wt` (a file name used as a
sub-command) is NOT an import, so a file with no importers may still be a live command;
those are reported separately from files that also have no `__main__` entry.
"""
import ast
import difflib
import json
import os
import subprocess
import sys
from collections import defaultdict

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def tracked():
    out = subprocess.run(["git", "ls-files", "src/*.py", "wt.py"], cwd=ROOT,
                         capture_output=True, text=True).stdout.split("\n")
    return [p for p in out if p.endswith(".py")]


def modname(path):
    m = path[:-3].replace("/", ".")
    return m[:-9] if m.endswith(".__init__") else m


def package(path):
    """Reporting bucket: src/<a>/<b> for targets/engines/xbox/toolchain, else src/<a>."""
    parts = path.split("/")
    if parts[0] != "src" or len(parts) < 3:
        return parts[0]
    if parts[1] in ("targets", "engines", "xbox", "toolchain") and len(parts) > 3:
        return "/".join(parts[1:3])
    return parts[1]


def main(argv):
    files = tracked()
    by_mod = {modname(p): p for p in files}
    by_dir = defaultdict(dict)                 # dir -> {basename: path}
    by_base = defaultdict(list)                # basename -> [paths]
    for p in files:
        d, b = os.path.split(p)
        by_dir[d][b[:-3]] = p
        by_base[b[:-3]].append(p)

    info = {}
    edges = defaultdict(set)                   # importer path -> {imported path}
    for p in files:
        try:
            src = open(os.path.join(ROOT, p), encoding="utf-8", errors="replace").read()
            tree = ast.parse(src)
        except SyntaxError:
            info[p] = {"lines": 0, "main": False, "syntax_error": True}
            continue
        has_main = any(isinstance(n, ast.If) and isinstance(n.test, ast.Compare)
                       and getattr(n.test.left, "id", "") == "__name__" for n in tree.body)
        info[p] = {"lines": src.count("\n") + 1, "main": has_main}
        d = os.path.dirname(p)
        pkg_parts = modname(p).split(".")
        if not p.endswith("__init__.py"):
            pkg_parts = pkg_parts[:-1]

        def link(mod):
            if mod in by_mod and by_mod[mod] != p:
                edges[p].add(by_mod[mod]); return True
            return False

        for n in ast.walk(tree):
            if isinstance(n, ast.Import):
                for a in n.names:
                    if not link(a.name) and "." not in a.name and a.name in by_dir[d]:
                        edges[p].add(by_dir[d][a.name])          # bare sibling import
            elif isinstance(n, ast.ImportFrom):
                base = n.module or ""
                if n.level:
                    up = pkg_parts[:len(pkg_parts) - (n.level - 1)]
                    base = ".".join(up + ([n.module] if n.module else []))
                elif base and "." not in base and base in by_dir[d]:
                    edges[p].add(by_dir[d][base]); continue
                link(base)
                for a in n.names:
                    link(base + "." + a.name)
            elif isinstance(n, ast.Call) and n.args and isinstance(n.args[0], ast.Constant) \
                    and isinstance(n.args[0].value, str):
                f = n.func
                attr = getattr(f, "attr", getattr(f, "id", ""))
                owner = getattr(getattr(f, "value", None), "id", "")
                s = n.args[0].value
                if attr == "get" and owner in ("cores", "_cores"):
                    for q in by_base.get(s, []):                 # every override + the default
                        if q != p:
                            edges[p].add(q)
                elif attr == "import_module":
                    link(s)

    importers = defaultdict(set)
    for a, bs in edges.items():
        for b in bs:
            importers[b].add(a)

    # ---- per-package table -------------------------------------------------------------
    pk = defaultdict(lambda: {"files": 0, "lines": 0, "no_importer_cmd": 0, "no_importer_nomain": 0,
                              "ext_importers": set()})
    for p, i in info.items():
        k = package(p); r = pk[k]
        r["files"] += 1; r["lines"] += i["lines"]
        imp = importers.get(p, set())
        if not imp and not p.endswith("__init__.py"):
            r["no_importer_cmd" if i.get("main") else "no_importer_nomain"] += 1
        r["ext_importers"] |= {package(q) for q in imp if package(q) != k}
    print("%-22s %5s %7s  %-14s %-12s %s" % ("package", "files", "lines", "unimported:cmd",
                                             "unimp:no-main", "imported by (other packages)"))
    for k, r in sorted(pk.items(), key=lambda kv: -kv[1]["lines"]):
        print("%-22s %5d %7d  %-14d %-12d %s" % (k, r["files"], r["lines"], r["no_importer_cmd"],
              r["no_importer_nomain"], ", ".join(sorted(r["ext_importers"]))[:70] or "-"))
    tot_l = sum(i["lines"] for i in info.values())
    print("%-22s %5d %7d" % ("TOTAL", len(info), tot_l))

    # ---- duplication: same file name in several places ---------------------------------
    def text(p):
        return open(os.path.join(ROOT, p), encoding="utf-8", errors="replace").read().splitlines()

    dups = []
    for base, paths in by_base.items():
        if base in ("__init__", "_template") or len(paths) < 2:
            continue
        ref = max(paths, key=lambda q: info[q]["lines"])
        rt = text(ref)
        for q in paths:
            if q == ref:
                continue
            ratio = difflib.SequenceMatcher(None, rt, text(q), autojunk=False).ratio()
            dups.append((ratio, info[q]["lines"], q, ref))
    dups.sort(reverse=True)
    near = [d for d in dups if d[0] >= 0.6]
    print("\nSame-named files: %d pairs; %d are >=60%% identical, holding %d duplicated lines" % (
        len(dups), len(near), sum(int(d[0] * d[1]) for d in near)))
    for ratio, lines, q, ref in near[:30]:
        print("  %3d%%  %5d lines  %-42s ~ %s" % (ratio * 100, lines, q[4:], ref[4:]))

    if "--json" in argv:
        out = {p: dict(info[p], package=package(p), importers=sorted(importers.get(p, ())),
                       imports=sorted(edges.get(p, ()))) for p in info}
        json.dump(out, open(argv[argv.index("--json") + 1], "w"), indent=1)


if __name__ == "__main__":
    main(sys.argv[1:])
