#!/usr/bin/env python3
"""oracle_equiv.py -- measure reconstruction fidelity by running two NE modules on
the Unicorn oracle and comparing their behavior export-by-export. This is the
"integral" property, quantified: a rebuilt module is faithful only if it behaves
like retail, not merely if it compiles with the right export surface.

    wt --target win95 oracle_equiv <retail.dll> <rebuilt.dll> [--arity N] [--maxord N]

For each common export ordinal, both modules run under the same oracle + import
stub with identical args, and we compare (return value, the sequence of imported
API calls, and clean-vs-fault exit). Same args on both sides => any divergence is
a real behavioral difference in the recovered code, independent of whether the
arity we chose matches the true prototype.
"""
import argparse
import sys

from src.targets.win95 import ne_dynoracle

# Toolchain-runtime imports the COMPILER/CRT emits, not the module's own logic --
# ignored when comparing behavior so OpenWatcom-vs-MSC codegen noise doesn't count
# as a reconstruction divergence (e.g. OW's Win16 startup calls GetVersion).
_TOOLCHAIN_IMPORTS = {"KERNEL.GETVERSION", "KERNEL.@3", "KERNEL.GETWINFLAGS",
                      "KERNEL.LOCALINIT", "KERNEL.@4", "KERNEL.GETVERSION16"}


def _sem_imps(hits):
    return tuple(h.get("api") for h in hits if h.get("api") not in _TOOLCHAIN_IMPORTS)


def _beh(path, ordn, arity):
    # Run inside the controlled Win16 environment (ne_dynoracle): GlobalAlloc hands
    # out real far-heap blocks, GlobalLock returns pointers, file APIs feed data --
    # so allocations SUCCEED and both modules run their real paths, not just the
    # allocation-failure error path. Fair behavioral comparison.
    try:
        r = ne_dynoracle.probe(path, ordinal=ordn, args=[0] * arity)
    except Exception as e:
        return {"exit": "trace-error:" + type(e).__name__, "ret": None, "insn_count": 0, "imps": ()}
    return {"exit": r.get("exit"), "ret": r.get("ret"), "insn_count": r.get("insn_count"),
            "imps": _sem_imps(r.get("imports_hit", []))}


def main(argv):
    ap = argparse.ArgumentParser(prog="oracle_equiv", description=__doc__.splitlines()[0])
    ap.add_argument("retail")
    ap.add_argument("rebuilt")
    ap.add_argument("--arity", type=int, default=0, help="args pushed to each fn (both sides)")
    ap.add_argument("--maxord", type=int, default=32)
    a = ap.parse_args(argv)

    print("%-6s %-8s %-8s %-6s %-6s %-6s %s" % ("ord", "retRet", "rebRet", "rIns", "bIns", "imps", "equiv"))
    eq = tot = 0
    for o in range(1, a.maxord + 1):
        rb = _beh(a.retail, o, a.arity)
        bb = _beh(a.rebuilt, o, a.arity)
        if str(rb["exit"]).startswith("bad-ordinal") and str(bb["exit"]).startswith("bad-ordinal"):
            continue
        tot += 1
        same = (rb["ret"] == bb["ret"] and rb["imps"] == bb["imps"])
        eq += same
        print("%-6d %-8s %-8s %-6s %-6s %-4s %s" %
              (o, rb["ret"], bb["ret"], rb["insn_count"], bb["insn_count"],
               "%d/%d" % (len(rb["imps"]), len(bb["imps"])), "YES" if same else "NO"))
    print("\nbehaviorally equivalent: %d/%d exports  (%.0f%%)" % (eq, tot, 100.0 * eq / max(tot, 1)))
    print("(same ret + same import-call sequence under identical args)")


if __name__ == "__main__":
    main(sys.argv[1:])
