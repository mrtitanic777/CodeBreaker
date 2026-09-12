#!/usr/bin/env python3
r"""l2report.py -- aggregate the Layer-2 sweep output into a corpus report.

Reads every <mod>.l2ledger.json in an emit dir (works mid-sweep or after) and
summarises: corpus totals, per-category counts, and the modules ranked by clean-C
yield. Run it any time to see where the automated reconstruction stands.

    python -m engine.targets.win95.l2report [emit_dir]
"""
import collections
import glob
import json
import os
import sys

DEFAULT = os.path.join("src", "reconstructed", "auto")


def load(emit_dir):
    mods = {}
    for p in sorted(glob.glob(os.path.join(emit_dir, "*.l2ledger.json"))):
        try:
            with open(p) as f:
                mods[os.path.basename(p)[:-len(".l2ledger.json")]] = json.load(f)
        except Exception:
            pass
    return mods


def report(emit_dir):
    mods = load(emit_dir)
    tot = collections.Counter()
    cats = collections.Counter()
    per_mod = []
    for m, led in mods.items():
        kinds = collections.Counter(r["kind"] for r in led)
        for r in led:
            cats[r["cat"]] += 1
        nc = kinds.get("clean-c", 0)
        ncpp = kinds.get("clean-cpp", 0)                  # __thiscall methods (emit_cpp)
        floored = kinds.get("asm", 0) + kinds.get("asm-pending", 0)
        cov = nc + ncpp + kinds.get("asm", 0)
        tot["modules"] += 1
        tot["functions"] += len(led)
        tot["clean_c"] += nc
        tot["clean_cpp"] += ncpp
        tot["asm"] += kinds.get("asm", 0)
        tot["pending"] += kinds.get("asm-pending", 0)
        tot["escalate"] += kinds.get("escalate", 0)
        tot["drafts"] += sum(1 for r in led if r.get("draft"))
        per_mod.append((nc, len(led), m))

    fns = max(1, tot["functions"])
    print("=" * 66)
    print("CORPUS LAYER-2 REPORT  (%s)" % emit_dir)
    print("=" * 66)
    print("  modules processed     : %d" % tot["modules"])
    print("  functions             : %d" % tot["functions"])
    print("  clean C (byte-exact)  : %d  (%.1f%%)" % (tot["clean_c"], 100.0 * tot["clean_c"] / fns))
    print("  clean C++ (byte-exact): %d  (%.1f%%)  [__thiscall methods]" % (tot["clean_cpp"], 100.0 * tot["clean_cpp"] / fns))
    print("  clean-C review drafts : %d" % tot["drafts"])
    print("  byte-exact ASM floor  : %d" % tot["asm"])
    if tot["pending"]:
        print("  ASM floor pending     : %d  (--no-floor run)" % tot["pending"])
    print("  escalated             : %d" % tot["escalate"])
    covered = tot["clean_c"] + tot["clean_cpp"] + tot["asm"]
    denom = tot["functions"] - tot["pending"]
    if denom > 0:
        print("  COVERED buildable     : %d/%d  (%.1f%%)" % (covered, denom, 100.0 * covered / denom))
    print("\n  top modules by clean-C yield:")
    for nc, n, m in sorted(per_mod, reverse=True)[:20]:
        if nc:
            print("    %-22s %4d clean-C / %d fns" % (m, nc, n))
    print("\n  function categories (corpus):")
    for c, n in cats.most_common():
        print("    %-16s %d" % (c, n))
    return tot


def main():
    emit = sys.argv[1] if len(sys.argv) > 1 else DEFAULT
    if not os.path.isdir(emit):
        try:
            import pe32util as H
            H.ensure_target()
        except Exception:
            pass
    report(emit)


if __name__ == "__main__":
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    main()
