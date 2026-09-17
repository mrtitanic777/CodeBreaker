#!/usr/bin/env python3
r"""buildfix.py -- triage every module owlink FAILS to build, grouped by ROOT CAUSE.

A module that will not link cannot be shipped, so "some don't build" is not acceptable for a full
reconstruction. This re-links the failing set (in parallel), captures the compile + link diagnostics,
and classifies each failure into an actionable category (invalid object = a piece the compiler crashed
on; segment-too-large; optimizer bug; unresolved symbol; C++ mangling; ...) so the fixes can be made
by class. Reports category -> count -> modules, and writes BUILDFAIL.csv.

    wt --target win95 buildfix [--limit N] [-j N] [--only mod,mod] [--verbose mod]
"""
import csv
import os
import re
import subprocess
import sys
from concurrent.futures import ProcessPoolExecutor, as_completed

from src.targets.win95 import authsrc, owlink


def _classify_error(module):
    """Re-link `module`, capturing compile + link output; return (ok, category, detail)."""
    binpath = owlink._find_binary(module)
    if not binpath:
        return (False, "no-binary", "no source binary")
    try:
        r = owlink.link_module(module)
    except Exception as e:                                            # noqa: BLE001
        return (False, "exception", str(e)[:120])
    if r.get("ok"):
        return (True, "ok", "")
    err = (r.get("error") or "") + " " + (r.get("log") or "")
    # categorize by the strongest signal
    m = re.search(r"E2146: (\S+) is an invalid object file", err)
    if m:
        return (False, "invalid-obj", "compiler produced a bad obj: %s" % m.group(1))
    if "compile produced no obj" in err:
        return (False, "no-obj", "no piece compiled")
    if "E1118" in err or "segment too large" in err.lower():
        return (False, "segment-too-large", "a piece overruns 64K")
    if "E1119" in err:
        return (False, "optimizer-bug", "wcc optimizer (E1119)")
    m = re.search(r"E2028: (\S+) is an undefined reference", err)
    if m:
        return (False, "undefined-ref", "unresolved: %s" % m.group(1))
    m = re.search(r"E2093|cannot open (\S+)", err)
    if m:
        return (False, "cannot-open", err[-100:].strip())
    m = re.search(r"(E\d{4})", err)
    if m:
        return (False, "wlink-%s" % m.group(1), err[-120:].strip())
    return (False, "other", err[-120:].strip())


def _job(module):
    ok, cat, detail = _classify_error(module)
    fmt = "?"
    b = owlink._find_binary(module)
    if b:
        fmt = authsrc.classify(b)[0]
    return (module, fmt, ok, cat, detail)


def _fail_modules():
    """Modules that FAILED to build in the last export-parity sweep (fast path); else all modules."""
    dr = authsrc._dr()
    p = os.path.join(dr, "recon", "authored", "EXPORTPARITY.csv")
    if os.path.isfile(p):
        return [r["module"] for r in csv.DictReader(open(p)) if r["build"] == "FAIL"]
    cdir = os.path.join(dr, "recon", "authored", "c")
    return [f[:-2] for f in sorted(os.listdir(cdir)) if f.endswith(".c")]


def main(argv):
    jobs_n = int(argv[argv.index("-j") + 1]) if "-j" in argv else min((os.cpu_count() or 4), 12)
    if "--verbose" in argv:
        module = argv[argv.index("--verbose") + 1]
        r = owlink.link_module(module)
        print("module %s ok=%s" % (module, r.get("ok")))
        print("error:", r.get("error"))
        print("log:\n", r.get("log"))
        return 0
    if "--only" in argv:
        mods = [m.strip() for m in argv[argv.index("--only") + 1].split(",")]
    else:
        mods = _fail_modules()
    if "--limit" in argv:
        mods = mods[:int(argv[argv.index("--limit") + 1])]

    print("== buildfix triage: %d failing modules (%d workers) ==" % (len(mods), jobs_n))
    rows = []
    from collections import Counter
    cats = Counter()
    with ProcessPoolExecutor(max_workers=jobs_n) as ex:
        futs = [ex.submit(_job, m) for m in mods]
        for k, fut in enumerate(as_completed(futs)):
            module, fmt, ok, cat, detail = fut.result()
            rows.append((module, fmt, "OK" if ok else "FAIL", cat, detail))
            cats[cat] += 1
            if (k + 1) % 25 == 0:
                print("  ... %d/%d" % (k + 1, len(mods)))

    dr = authsrc._dr()
    out = os.path.join(dr, "recon", "authored", "BUILDFAIL.csv")
    with open(out, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["module", "fmt", "status", "category", "detail"])
        for r in sorted(rows, key=lambda x: (x[3], x[0])):
            w.writerow(r)

    still = sum(1 for r in rows if r[2] == "FAIL")
    print("\n== build-failure categories ==")
    for cat, n in cats.most_common():
        print("  %-20s %d" % (cat, n))
    print("\n  now building OK (was fail): %d" % (len(rows) - still))
    print("  still failing: %d" % still)
    # a couple of example modules per category
    bycat = {}
    for module, fmt, st, cat, detail in rows:
        if st == "FAIL":
            bycat.setdefault(cat, []).append(module)
    print("\n  examples per category:")
    for cat, ms in sorted(bycat.items()):
        print("    %-20s %s" % (cat, ", ".join(ms[:8]) + (" ..." if len(ms) > 8 else "")))
    print("  -> %s" % out)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
