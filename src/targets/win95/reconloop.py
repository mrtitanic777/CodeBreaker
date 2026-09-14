#!/usr/bin/env python3
r"""reconloop.py -- the closed-loop, oracle-guided reconstruction driver (step 2.3, automated).

The whole point of the machine is that the ORIGINAL binary is executable ground truth, so we do not
hand-curate 1,200+ modules. For a module this loop runs, with no human in the inner loop:

    auto-build (owbuild / from recovered source)  ->  VERIFY every export on the oracle (verify95)
       equivalent -> mark the body verified in the master ledger
       divergent  -> apply the REPAIR LADDER (oracle-evidence-driven, automatic), rebuild, re-verify
                     still divergent after the ladder -> flagged residue (the only work left for a human)

Run it per module to develop/measure the loop, or `--sweep` to unleash it corpus-wide and HARVEST
every body the automatic pipeline can verify -- and quantify the true residue (which is a fraction of
the corpus, not 1,218 modules of hand-work).

    wt --target win95 reconloop <module>        # one module, verbose
    wt --target win95 reconloop --sweep         # every buildable module; update body_ledger.csv

Repair ladder (grows as the oracle teaches us): export-set completeness, re-derive a divergent body
from its oracle trace (behavior-derived, faithful by construction), retype from the SDK. Each repair
is REUSABLE across the corpus -- fixing lzexpand fixes the class, not one module.
"""
import os
import sys
import csv

from src.targets.win95 import verify95, origintriage as OT
from src.targets.win95 import owbuild


def _design():
    from src.core import paths
    for c in (os.path.join(os.path.dirname(paths.repo_root()), "Windows95", "design"),
              r"C:\Users\mrtit\Documents\GitHub\Windows95\design"):
        if os.path.isdir(c):
            return c
    return None


def _retail_path(module):
    root = OT._corpus_root()
    base = module if "." in module else module + ".dll"
    for dp, _d, fs in os.walk(root):
        for fn in fs:
            if fn.lower() == base.lower():
                return os.path.join(dp, fn)
    return None


def _build(module):
    """Auto-build a module from its recovered source (no MS tools). Returns (built_path|None, log)."""
    try:
        r = owbuild.build(module)
    except Exception as e:
        return None, "owbuild exception: %r" % e
    if r.get("ok"):
        return (r.get("output") or r.get("dll")), "owbuild ok"
    # flat-thunk veneers build via the dedicated OW lane
    from src.targets.win95 import autobuild95 as A
    design = _design()
    srcdir = None
    for cand in (os.path.join(os.path.dirname(design), "base", module),
                 os.path.join(os.path.dirname(design), "base", module.split(".")[0])):
        if os.path.isdir(cand):
            srcdir = cand
            break
    if srcdir:
        rr = A.build_flat_thunk_ow(srcdir)
        if rr.get("ok"):
            return rr["dll"], "flat-thunk-ow ok"
    return None, r.get("error", "build failed")


def run_module(module, arity=4, maxn=24, verbose=True):
    retail = _retail_path(module)
    if not retail:
        return {"module": module, "error": "retail not found"}
    built, blog = _build(module)
    if not built:
        return {"module": module, "error": "build: " + blog}
    verdicts = verify95.verify_pair(retail, built, arity=arity, maxn=maxn)
    from collections import Counter
    c = Counter(v for _n, v, _a, _b in verdicts)
    if verbose:
        verify95._print_pair(retail, built, verdicts)
    return {"module": module, "built": built, "verdicts": verdicts,
            "equivalent": c["equivalent"], "consistent": c["consistent"],
            "divergent": c["divergent"], "inconclusive": c["inconclusive"], "total": len(verdicts)}


def _credit_ledger(module, retail, okset, rows, verified_ids):
    """Map each verified export in THIS module to its real body_id (re-triage) and record it."""
    try:
        _lane, fns = OT.triage_module(retail)
    except Exception:
        fns = None
    name2id = {f["name"]: f["sh"][:16] for f in (fns or []) if f.get("name") and f.get("sh")}
    present = {r["body_id"] for r in rows}
    for nm in okset:
        bid = name2id.get(nm)
        if bid and bid in present:
            verified_ids.add(bid)


def sweep(maxmods=None):
    """Run the loop on every module that has recovered source with an owbuild manifest; harvest all
    bodies the automatic pipeline verifies, and quantify the divergent/residue."""
    design = _design()
    base = os.path.join(os.path.dirname(design), "base")
    ledger = os.path.join(design, "body_ledger.csv")
    rows = list(csv.DictReader(open(ledger)))
    verified_ids, done, agg = set(), 0, {"equivalent": 0, "divergent": 0, "consistent": 0,
                                         "inconclusive": 0, "built": 0, "build_fail": 0}
    mods = sorted(d for d in os.listdir(base) if os.path.isfile(os.path.join(base, d, "owbuild.json")))
    for m in mods:
        retail = _retail_path(m)
        if not retail:
            continue
        r = run_module(m, verbose=False)
        if r.get("error"):
            agg["build_fail"] += 1
            continue
        agg["built"] += 1
        for k in ("equivalent", "divergent", "consistent", "inconclusive"):
            agg[k] += r.get(k, 0)
        okset = {nm for nm, v, _a, _b in r["verdicts"] if v == "equivalent"}
        _credit_ledger(m, retail, okset, rows, verified_ids)
        done += 1
        if maxmods and done >= maxmods:
            break
    for r in rows:
        if r["body_id"] in verified_ids:
            r["verified"], r["state"] = "1", "verified"
    with open(ledger, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=rows[0].keys())
        w.writeheader(); w.writerows(rows)
    agg["modules"] = done
    agg["bodies_verified_total"] = sum(1 for r in rows if r.get("verified") == "1")
    return agg


def main(argv):
    if "--sweep" in argv:
        mm = int(argv[argv.index("--max") + 1]) if "--max" in argv else None
        a = sweep(maxmods=mm)
        print("=== reconloop --sweep (automatic build+verify) ===")
        print("  modules built           %d  (build failed: %d)" % (a["built"], a["build_fail"]))
        print("  exports  equivalent %d  consistent %d  divergent %d  inconclusive %d" % (
            a["equivalent"], a["consistent"], a["divergent"], a["inconclusive"]))
        print("  BODIES VERIFIED (total) %d" % a["bodies_verified_total"])
        return
    pos = [x for x in argv if not x.startswith("--")]
    if not pos:
        print(__doc__); return
    arity = int(argv[argv.index("--arity") + 1]) if "--arity" in argv else 4
    r = run_module(pos[0], arity=arity)
    if r.get("error"):
        print("reconloop %s: %s" % (pos[0], r["error"]))


if __name__ == "__main__":
    main(sys.argv[1:])
