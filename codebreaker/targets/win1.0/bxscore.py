#!/usr/bin/env python3
"""
bxscore.py — byte-exact scorecard: aggregate the conductor's per-module ledgers
(reports/conduct/*.json) into a %-live scorecard across a tier or the whole app set.

    wt --target win1.0 bxscore              # scorecard over every conduct ledger
    wt --target win1.0 bxscore --gaps       # + the ranked db gap queue (top offenders)

"exact"  = functions that reassemble byte-identical to the shipped binary.
"%live"  = real instructions vs db-fallback (the db residue is where MASM will not
           reproduce the shipped encoding; it is byte-exact but not yet live).
Ledger keys collide across run styles (tier writes <file>.exe.json, a direct run
writes <module>.json); we keep, per app, whichever ledger has the MOST functions
(the most complete run).
"""
import glob
import json
import os
import sys

D = os.path.join("reports", "conduct")


def _load():
    apps = {}
    for p in glob.glob(os.path.join(D, "*.json")):
        b = os.path.basename(p)
        app = (b[:-9] if b.endswith(".exe.json") else b[:-5]).upper()
        try:
            d = json.load(open(p))
        except (OSError, ValueError):
            continue
        if d and len(d) > len(apps.get(app, {})):
            apps[app] = d
    return apps


def main():
    apps = _load()
    if not apps:
        print("no conduct ledgers under %s/ — run `conduct <mod>` first" % D)
        return
    rows = []
    for app, d in apps.items():
        ex = sum(1 for r in d.values() if r.get("status") == "exact")
        live = sum(r.get("live", 0) for r in d.values())
        tot = sum(r.get("total", 0) for r in d.values())
        db = sum(r.get("db", 0) for r in d.values())
        cb = sum(r.get("size", 0) for r in d.values())
        rows.append((app, len(d), ex, live, tot, db, cb))
    rows.sort(key=lambda r: -r[6])
    print("%-10s %6s %11s %8s %7s %10s" % ("APP", "fns", "exact", "%live", "db", "code B"))
    print("-" * 56)
    g = [0] * 5
    for app, f, e, l, t, db, cb in rows:
        print("%-10s %6d %11s %7.1f%% %7d %10d" % (
            app, f, "%d/%d" % (e, f), 100 * l / t if t else 0, db, cb))
        g[0] += f; g[1] += e; g[2] += l; g[3] += t; g[4] += db
    gcb = sum(r[6] for r in rows)
    print("-" * 56)
    print("%-10s %6d %11s %7.1f%% %7d %10d" % (
        "TOTAL", g[0], "%d/%d" % (g[1], g[0]), 100 * g[2] / g[3] if g[3] else 0, g[4], gcb))

    if "--gaps" in sys.argv:
        # rank functions by db count — the worst-reproduced (most refinement value)
        worst = []
        for app, d in apps.items():
            for k, r in d.items():
                if r.get("db", 0):
                    worst.append((r["db"], app, k, r.get("live", 0), r.get("total", 0)))
        worst.sort(reverse=True)
        print("\ndb gap queue — worst-reproduced functions (top 20):")
        print("  %-9s %-12s %6s  %s" % ("db", "app", "seg:off", "live/total"))
        for db, app, k, live, tot in worst[:20]:
            print("  %-9d %-12s %-8s %d/%d" % (db, app, k, live, tot))


if __name__ == "__main__":
    main()
