#!/usr/bin/env python3
r"""bodyledger.py -- the operational cockpit over the step-2 master body ledger.

origintriage writes design/body_ledger.csv: one row per unique Win95 function body (identity + origin
+ pipeline state). This reads it and reports progress toward "every body verified-correct" -- overall,
per lane, per origin, and per module -- and lists a chosen module's work-list so a curator (or a
recovery pass) knows exactly which bodies remain and in what state.

    wt --target win95 bodyledger                     # corpus status: states x lanes x origins
    wt --target win95 bodyledger --module kernel32   # one module's bodies + their states
    wt --target win95 bodyledger --state decompiled --lane pe --limit 40
    wt --target win95 bodyledger --boot              # boot-critical modules' readiness

State machine: decompiled -> raised -> typed -> VERIFIED  (genuine bodies);
shared / crt / thunk / documented = identify-once classes (sourced, not per-module reconstructed).
"""
import os
import csv
import sys

from src.core import paths

_STATES = ["decompiled", "raised", "typed", "verified"]
_IDENTIFY = ["shared", "crt", "thunk", "documented"]
_BOOT = ["kernel32", "gdi32", "user32", "advapi32", "krnl386", "gdi", "user",
         "vmm32", "system", "vga", "keyboard", "mouse", "explorer"]


def _ledger_path():
    from src.core import corpus
    c = os.path.join(corpus.design_dir(), "body_ledger.csv")
    return c if os.path.isfile(c) else None


def _load(path):
    with open(path, newline="") as f:
        return list(csv.DictReader(f))


def _bar(done, total, width=28):
    if total <= 0:
        return "-" * width
    n = int(width * done / total)
    return "#" * n + "-" * (width - n)


def main(argv):
    path = _ledger_path()
    if not path:
        print("body_ledger.csv not found -- run: wt --target win95 origintriage")
        return
    rows = _load(path)
    limit = int(argv[argv.index("--limit") + 1]) if "--limit" in argv else 40
    lane = argv[argv.index("--lane") + 1] if "--lane" in argv else None

    if "--module" in argv:
        mod = argv[argv.index("--module") + 1].lower()
        sel = [r for r in rows if mod in r["example_module"].lower()]
        print("=== %s -- %d bodies ===" % (mod, len(sel)))
        by_state = {}
        for r in sel:
            by_state[r["state"]] = by_state.get(r["state"], 0) + 1
        print("  states: " + "  ".join("%s=%d" % (s, by_state[s]) for s in sorted(by_state)))
        for r in sorted(sel, key=lambda r: -int(r["instances"]))[:limit]:
            print("  %-16s %-10s %-11s x%-4s %s" % (
                r["body_id"], r["origin"], r["state"], r["instances"], r["example_name"] or "(sub_)"))
        return

    if "--boot" in argv:
        print("=== boot-critical modules -- body readiness ===")
        for m in _BOOT:
            sel = [r for r in rows if r["example_module"].lower().startswith(m + ".")
                   or r["example_module"].lower() == m + ".exe" or r["example_module"].lower() == m + ".dll"]
            if not sel:
                print("  %-12s (no bodies attributed -- shared/other module)" % m)
                continue
            ver = sum(1 for r in sel if r["state"] == "verified")
            print("  %-12s %s %d/%d verified" % (m, _bar(ver, len(sel)), ver, len(sel)))
        return

    # corpus status
    sel = [r for r in rows if (not lane or r["lane"] == lane)]
    total = len(sel)
    by_state, by_lane, by_origin = {}, {}, {}
    for r in sel:
        by_state[r["state"]] = by_state.get(r["state"], 0) + 1
        by_lane[r["lane"]] = by_lane.get(r["lane"], 0) + 1
        by_origin[r["origin"]] = by_origin.get(r["origin"], 0) + 1
    genuine = by_origin.get("genuine", 0) + by_origin.get("documented-api", 0) + by_origin.get("documented", 0)
    verified = by_state.get("verified", 0)
    print("=== body ledger status%s ===" % (" [lane=%s]" % lane if lane else ""))
    print("  unique bodies           %d" % total)
    print("  genuine targets         %d" % genuine)
    print("  VERIFIED                %d  %s  %.1f%% of genuine" % (
        verified, _bar(verified, genuine), 100.0 * verified / max(genuine, 1)))
    print("  --- pipeline state ---")
    for s in _STATES + _IDENTIFY:
        if s in by_state:
            print("    %-12s %8d" % (s, by_state[s]))
    print("  --- by lane ---")
    for k in sorted(by_lane):
        print("    %-6s %8d" % (k, by_lane[k]))
    print("  --- by origin ---")
    for k in sorted(by_origin):
        print("    %-16s %8d" % (k, by_origin[k]))
    print("  ledger -> %s" % path)


if __name__ == "__main__":
    main(sys.argv[1:])
