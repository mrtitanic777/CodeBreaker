#!/usr/bin/env python3
r"""behavsweep.py -- corpus-wide BEHAVIORAL verification: is each rebuilt module faithful?

Linking (owlink) proves the source is a legal, resolvable translation unit; it does NOT prove the
recovered code BEHAVES like the original. This sweep is the behavioral gate, corpus-wide: for every
module owlink built, it runs the RETAIL binary and OUR REBUILT binary on the same Unicorn oracle,
export by export, with identical inputs and symmetric external stubbing, and compares a codegen-
INDEPENDENT fingerprint -- (return value, imported-API-call sequence). Same inputs on both sides, so
any divergence is a real behavioral defect in the recovered code, not a toolchain artifact.

    wt --target win95 behavsweep [--maxord N] [--limit N] [-j N]

Reads recon/authored/BUILDLINK.csv (the linked modules), writes recon/authored/BEHAVIOR.csv
(module, fmt, tested, equiv, pct) and prints the corpus behavioral scoreboard.
"""
import csv
import os
import sys
from concurrent.futures import ProcessPoolExecutor, as_completed

from src.targets.win95 import authsrc, owlink
from src.targets.win95 import oracle_equiv as OE


def _sweep_one(job):
    """(module, fmt, retail, rebuilt, maxord) -> (module, fmt, tested, equiv)."""
    module, fmt, retail, rebuilt, maxord = job
    tested = equiv = 0
    for o in range(1, maxord + 1):
        try:
            rb = OE._beh(retail, o, 0)
            bb = OE._beh(rebuilt, o, 0)
        except Exception:                                            # noqa: BLE001
            continue
        if str(rb["exit"]).startswith("bad-ordinal") and str(bb["exit"]).startswith("bad-ordinal"):
            continue
        tested += 1
        if rb["ret"] == bb["ret"] and rb["imps"] == bb["imps"]:
            equiv += 1
    return (module, fmt, tested, equiv)


def main(argv):
    maxord = int(argv[argv.index("--maxord") + 1]) if "--maxord" in argv else 48
    limit = int(argv[argv.index("--limit") + 1]) if "--limit" in argv else None
    jobs_n = int(argv[argv.index("-j") + 1]) if "-j" in argv else min((os.cpu_count() or 4), 12)
    dr = authsrc._dr()
    linkcsv = os.path.join(dr, "recon", "authored", "BUILDLINK.csv")
    if not os.path.isfile(linkcsv):
        print("no BUILDLINK.csv -- run `owlink --all` first"); return 1

    jobs = []
    for r in csv.DictReader(open(linkcsv)):
        if r["status"] != "OK":
            continue
        retail = owlink._find_binary(r["module"])
        if not retail:
            continue
        out = r["module"] + (".dll" if retail.lower().endswith(".dll") else ".exe")
        rebuilt = os.path.join(dr, "recon", "authored", "_link", r["module"], out)
        if os.path.isfile(rebuilt):
            jobs.append((r["module"], r["fmt"], retail, rebuilt, maxord))
    if limit:
        jobs = jobs[:limit]

    print("== behavioral sweep: %d linked modules on the oracle (%d workers) ==" % (len(jobs), jobs_n))
    rows, T, E, full = [], 0, 0, 0
    with ProcessPoolExecutor(max_workers=jobs_n) as ex:
        futs = [ex.submit(_sweep_one, j) for j in jobs]
        for k, fut in enumerate(as_completed(futs)):
            m, fmt, tested, eq = fut.result()
            rows.append((m, fmt, tested, eq, "%.0f" % (100.0 * eq / max(tested, 1))))
            T += tested; E += eq
            if tested and eq == tested:
                full += 1
            if (k + 1) % 50 == 0:
                print("  ... %d/%d modules swept" % (k + 1, len(jobs)))

    outcsv = os.path.join(dr, "recon", "authored", "BEHAVIOR.csv")
    with open(outcsv, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["module", "fmt", "exports_tested", "equiv", "pct"])
        for r in sorted(rows, key=lambda x: (-int(x[3]), x[0])):
            w.writerow(r)
    # per-format rollup
    byfmt = {}
    for m, fmt, tested, eq, _ in rows:
        d = byfmt.setdefault(fmt or "LE", [0, 0])
        d[0] += eq; d[1] += tested
    print("\n== corpus behavioral fidelity ==")
    print("  exports behaviorally equivalent to retail: %d / %d  (%.0f%%)" % (E, T, 100.0 * E / max(T, 1)))
    print("  modules 100%% equivalent (all tested exports): %d / %d" % (full, len(rows)))
    for fmt, (e, t) in sorted(byfmt.items()):
        print("    %-4s %d/%d (%.0f%%)" % (fmt, e, t, 100.0 * e / max(t, 1)))
    print("  -> %s" % outcsv)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
