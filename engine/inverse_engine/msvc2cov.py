#!/usr/bin/env python3
r"""msvc2cov.py -- REAL-CORPUS coverage of the MSVC-2.0 inverse (msvc2inv).

Runs msvc2inv over actual shipped Windows 95 functions (their real linked bytes, via the match
engine) and reports the honest byte-exact recovery rate -- the fraction of real win95 that the
compiler-inverse turns back into byte-exact C. A function counts only if invert() returns C whose
model bytes equal the shipped bytes with relocation slots masked (byte-exact by construction).

    wt --target win95 <...>  # (run from the win95 data root, or with WT_DATA_ROOT set)
    python -m engine.inverse_engine.msvc2cov [limit] [module]
"""
import os
import sys
from collections import Counter


def _setup():
    """Point the engine at the win95 target + data root (matchbatch resolves relative to cwd)."""
    from engine.foundation import cores
    here = os.path.dirname(os.path.abspath(__file__))
    cb = os.path.dirname(os.path.dirname(here))
    if cores._active_dir is None:
        cores.set_target(os.path.join(cb, "engine", "targets", "win95"))
    roots = ([os.path.join(os.environ["WT_DATA_ROOT"], "win95")] if os.environ.get("WT_DATA_ROOT") else [])
    roots += [os.path.join(os.path.dirname(cb), "win95"), os.getcwd()]
    for r in roots:
        if os.path.isdir(os.path.join(r, "src")) or os.path.isdir(os.path.join(r, "reconstructed")):
            os.chdir(r)
            break


def run(limit=2000, module=None, show=0):
    _setup()
    from engine.match_engine import matchbatch, matchsearch
    from engine.inverse_engine import msvc2inv
    tot = inv = 0
    shapes = Counter()
    bylen = Counter()
    for mod, seg, off in matchsearch.iter_corpus(module, limit):
        ap = matchbatch._asm_for(mod, seg, off)
        if not ap:
            continue
        try:
            sb = matchbatch.shipped_bytes(ap)
        except Exception:
            continue
        if not sb:
            continue
        tot += 1
        r = msvc2inv.invert(sb)
        if r:
            inv += 1
            shapes[r["expr"][0] if r["expr"] else "?"] += 1
            bylen[len(sb) // 8 * 8] += 1
            if show and inv <= show:
                print("  %-16s (%2dB) -> %s" % ("%s:%s:%s" % (mod, seg, off), len(sb), r["c"]))
    print("\n" + "=" * 64)
    print("MSVC-2.0 INVERSE -- real-win95 coverage")
    print("=" * 64)
    print("  real functions sampled:            %d" % tot)
    print("  byte-exact recovered by msvc2inv:  %d  (%.1f%%)" % (inv, 100 * inv / max(1, tot)))
    print("  recovered-shape breakdown:  " + ", ".join("%s=%d" % kv for kv in shapes.most_common()))
    return inv, tot


def main():
    a = sys.argv[1:]
    limit = int(a[0]) if a and a[0].isdigit() else 2000
    module = next((x for x in a if not x.isdigit() and not x.startswith("-")), None)
    show = 20 if "--show" in a else 0
    run(limit=limit, module=module, show=show)


if __name__ == "__main__":
    main()
