#!/usr/bin/env python3
r"""exportparity.py -- corpus-wide EXPORT-PARITY gate: does every module export everything retail did?

Linking proves the source builds; this proves the built module's export surface MATCHES the original --
every ordinal the retail binary exported is present in ours (code exports resolved by name or address,
data exports reconstructed by the DGROUP data lane). It links every authored module (in parallel) and
records, per module: linked OK, retail export count, and any retail export we FAILED to reproduce
(missing_exports from owlink, with the reason). A module is at PARITY when nothing is missing.

    wt --target win95 exportparity [--limit N] [-j N] [--gaps]

Writes recon/authored/EXPORTPARITY.csv and prints the scoreboard. `--gaps` also lists every residual
missing export (module, ordinal, name, reason) so the remaining work is explicit.
"""
import csv
import os
import sys
from concurrent.futures import ProcessPoolExecutor, as_completed

from src.targets.win95 import authsrc, owlink


def _job(module):
    """Link one module; return (module, fmt, ok, retail_exports, missing_list). Never raises."""
    try:
        r = owlink.link_module(module)
        me = r.get("missing_exports", []) or []
        # retail export total = what we emitted that maps to a retail name + what we missed. Simpler and
        # authoritative: re-read the retail export table count.
        binp = owlink._find_binary(module)
        fmt = r.get("fmt", "?")
        retail_n = len(owlink._exports(binp, fmt)) if binp and fmt in ("NE", "PE") else 0
        return (module, fmt, bool(r.get("ok")), retail_n, me)
    except Exception as e:                                            # noqa: BLE001
        return (module, "?", False, 0, [("-", "-", "exception: %s" % e)])


def main(argv):
    limit = int(argv[argv.index("--limit") + 1]) if "--limit" in argv else None
    jobs_n = int(argv[argv.index("-j") + 1]) if "-j" in argv else min((os.cpu_count() or 4), 12)
    show_gaps = "--gaps" in argv
    dr = authsrc._dr()

    cdir = os.path.join(dr, "recon", "authored", "c")
    mods = [f[:-2] for f in sorted(os.listdir(cdir)) if f.endswith(".c")]
    if limit:
        mods = mods[:limit]

    print("== export-parity sweep: %d modules (%d workers) ==" % (len(mods), jobs_n))
    rows, gaps = [], []
    n_ok = n_parity = n_fail = 0
    n_ret = n_missing = 0
    by_fmt = {}
    with ProcessPoolExecutor(max_workers=jobs_n) as ex:
        futs = {ex.submit(_job, m): m for m in mods}
        for k, fut in enumerate(as_completed(futs)):
            module, fmt, ok, retail_n, me = fut.result()
            miss = len(me)
            parity = ok and miss == 0 and (retail_n > 0 or fmt not in ("NE", "PE"))
            if ok:
                n_ok += 1
            else:
                n_fail += 1
            if ok and miss == 0:
                n_parity += 1
            n_ret += retail_n
            n_missing += miss
            fmt = fmt or "?"
            d = by_fmt.setdefault(fmt, [0, 0, 0])          # [modules, retail_exports, missing]
            d[0] += 1; d[1] += retail_n; d[2] += miss
            rows.append((module, fmt, "OK" if ok else "FAIL", retail_n, miss,
                         "PARITY" if (ok and miss == 0) else ("GAP" if ok else "BUILD-FAIL")))
            for o, nm, why in me:
                gaps.append((module, o, nm, why))
            if (k + 1) % 100 == 0:
                print("  ... %d/%d  (parity %d, gaps %d, fail %d)" %
                      (k + 1, len(mods), n_parity, n_ok - n_parity, n_fail))

    outcsv = os.path.join(dr, "recon", "authored", "EXPORTPARITY.csv")
    with open(outcsv, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["module", "fmt", "build", "retail_exports", "missing", "verdict"])
        for r in sorted(rows, key=lambda x: (-x[4], x[0])):
            w.writerow(r)
    if show_gaps or True:
        gapcsv = os.path.join(dr, "recon", "authored", "EXPORTGAPS.csv")
        with open(gapcsv, "w", newline="") as f:
            w = csv.writer(f)
            w.writerow(["module", "ordinal", "name", "reason"])
            for g in sorted(gaps):
                w.writerow(g)

    print("\n== export parity ==")
    print("  modules linked:        %d / %d  (%d build-fail)" % (n_ok, len(mods), n_fail))
    print("  modules at FULL parity: %d / %d  (%.1f%%)" % (n_parity, len(mods), 100.0 * n_parity / max(len(mods), 1)))
    print("  retail exports reproduced: %d / %d  (%d missing)" % (n_ret - n_missing, n_ret, n_missing))
    print("  by format (modules / retail-exports / missing):")
    for fmt, (m, e, mi) in sorted(by_fmt.items()):
        print("    %-4s %5d mods  %7d exports  %5d missing" % (fmt, m, e, mi))
    if show_gaps and gaps:
        print("\n  residual missing exports (%d):" % len(gaps))
        for module, o, nm, why in sorted(gaps)[:80]:
            print("    %-12s ord %-5s %-16s %s" % (module, o, nm, why))
        if len(gaps) > 80:
            print("    ... and %d more (see EXPORTGAPS.csv)" % (len(gaps) - 80))
    print("  -> %s" % outcsv)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
