#!/usr/bin/env python3
"""stale_forks.py -- find per-era files that are stale forks of a shared engine module.

A file in src/targets/<era>/ is a STALE FORK when a shared module of the same name exists and the
two differ only in how they import things (bare `import target` versus the registry's
`cores.get('target')`), in comments, and in the era named in prose. Such a file adds nothing: `wt`
and `cores.get` both fall back to the shared module when the era has no override.

A fork is only SAFE TO REMOVE when, in addition, no other file in that era's directory imports it
by bare name (those importers would need converting first); otherwise it is listed as BLOCKED.

    python tools/stale_forks.py            # report
    python tools/stale_forks.py --apply    # git rm the safe ones
"""
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ERA = re.compile(r"Win(?:dows)?[ -]?(?:95|98|3\.1x?|1\.0\w*|9x)|win(?:95|98|31|10\w*)", re.I)
IMPORTISH = re.compile(r"^(import |from |sys\.path\.|\w+ = cores\.get\(|[\w, ]+ = cores\.get\()")


def tracked(pattern):
    out = subprocess.run(["git", "ls-files", pattern], cwd=ROOT, capture_output=True, text=True)
    return [p for p in out.stdout.split("\n") if p.endswith(".py")]


def essence(path):
    """The file's code with import plumbing, comments, blank lines and era names factored out."""
    out = []
    for ln in open(os.path.join(ROOT, path), encoding="utf-8", errors="replace"):
        s = ln.strip()
        if not s or s.startswith("#") or IMPORTISH.match(s):
            continue
        out.append(ERA.sub("ERA", s))
    return out


def worklist(shared):
    """Every per-era or Xbox file that shares a name with a shared module (or, lacking one, with a
    file in another era), with the number of lines that really differ once import plumbing is
    ignored. Small numbers are quick merges: fold the difference into one shared module, then the
    fork is removable. Families with no shared module yet need one creating first."""
    import difflib
    groups = {}
    for p in tracked("src/targets/*.py") + tracked("src/xbox/*.py"):
        base = os.path.basename(p)
        if "/plugins/" in p or base == "__init__.py":
            continue
        groups.setdefault(base, []).append(p)
    rows = []
    for base, forks in groups.items():
        if base in shared:
            ref, kind = shared[base][0], "shared"
        elif len(forks) > 1:
            ref, kind = max(forks, key=lambda q: os.path.getsize(os.path.join(ROOT, q))), "no shared module"
        else:
            continue
        er = essence(ref)
        for p in forks:
            if p == ref:
                continue
            ep = essence(p)
            n = sum(1 for l in difflib.unified_diff(ep, er, lineterm="", n=0)
                    if l[:1] in "+-" and l[:3] not in ("+++", "---"))
            rows.append((n, len(ep), p, ref, kind))
    rows.sort()
    small = [r for r in rows if r[0] <= 10]
    print("\nWORKLIST: %d forked files (%d code lines). %d differ by <=10 real lines (%d code lines)."
          % (len(rows), sum(r[1] for r in rows), len(small), sum(r[1] for r in small)))
    print("  %-5s %-5s %-42s %s" % ("diff", "code", "fork", "reference"))
    for n, size, p, ref, kind in rows:
        print("  %-5d %-5d %-42s %s%s" % (n, size, p[4:], ref[4:],
                                         "" if kind == "shared" else "   [%s]" % kind))


def main(argv):
    shared = {}
    for p in tracked("src/*.py"):
        if p.startswith(("src/targets/", "src/xbox/")) or p.endswith("__init__.py"):
            continue
        shared.setdefault(os.path.basename(p), []).append(p)
    safe, blocked = [], []
    for p in tracked("src/targets/*.py"):
        base = os.path.basename(p)
        if "/plugins/" in p or base not in shared:
            continue
        mine = essence(p)
        match = [d for d in shared[base] if essence(d) == mine]
        if not match:
            continue
        era_dir = os.path.dirname(p)
        name = base[:-3]
        pat = re.compile(r"^\s*(?:import\s+(?:[\w, ]*\b)?%s\b|from\s+%s\s+import)" % (name, name))
        users = []
        for q in tracked(era_dir + "/*.py"):
            if q != p and any(pat.match(l) for l in open(os.path.join(ROOT, q), encoding="utf-8",
                                                         errors="replace")):
                users.append(os.path.basename(q))
        lines = sum(1 for _ in open(os.path.join(ROOT, p), encoding="utf-8", errors="replace"))
        (blocked if users else safe).append((p, match[0], lines, users))
    print("SAFE to remove (identical apart from import plumbing, no bare importers): %d files, %d lines"
          % (len(safe), sum(s[2] for s in safe)))
    for p, d, n, _u in safe:
        print("  %-40s %5d  -> %s" % (p[4:], n, d[4:]))
    print("BLOCKED (stale fork, but imported by bare name in its era dir): %d files, %d lines"
          % (len(blocked), sum(b[2] for b in blocked)))
    for p, d, n, u in blocked:
        print("  %-40s %5d  -> %-32s used by %s" % (p[4:], n, d[4:], ", ".join(u)[:60]))
    if "--worklist" in argv:
        worklist(shared)
    if "--apply" in argv and safe:
        subprocess.run(["git", "rm", "-q"] + [s[0] for s in safe], cwd=ROOT, check=True)
        print("removed %d files" % len(safe))


if __name__ == "__main__":
    main(sys.argv[1:])
