#!/usr/bin/env python3
r"""bsweep.py -- corpus-wide behavioral reconstruction sweep (dynamic-lane analogue of
l2sweep). Runs the `recon` router across many Win95 PE modules and aggregates the
READABLE-SOURCE YIELD: how many functions the behavioral lane reconstructs to verified C,
by method, ranked by module. This is the measurement that says where the lane pays off
and which sharpener to build next -- with data, not guesses.

Each module is traced + routed (synth/piecewise/effect/struct/cfg/globals); per-module PE
caches are evicted between modules to keep memory flat across the corpus. A full sweep is
expensive (every function is emulated many times), so it is bounded by --modules and
--per-module and is happy to run in the background; --emit writes each module's
<module>.behav.c plus an aggregate summary.

    wt behavior bsweep [--modules N] [--per-module N] [--trace N] [--start K] [--emit DIR]
"""
import glob
import json
import os
import struct
import sys
from collections import Counter

from src.engines.behavior import recon, tracer, graph
from src.cli.common import opt as _opt


def _i386_pe(path):
    """Cheap MZ -> PE\\0\\0 -> machine==0x14c (i386) header check, no carving."""
    try:
        with open(path, "rb") as f:
            d = f.read(0x400)
        if d[:2] != b"MZ":
            return False
        pe = struct.unpack_from("<I", d, 0x3c)[0]
        if pe + 6 <= len(d) and d[pe:pe + 4] == b"PE\0\0":
            return struct.unpack_from("<H", d, pe + 4)[0] == 0x14c
        with open(path, "rb") as f:
            f.seek(pe); head = f.read(6)
        return head[:4] == b"PE\0\0" and struct.unpack_from("<H", head, 4)[0] == 0x14c
    except Exception:
        return False


def pe_modules():
    """32-bit PE modules (basename, deduped) under the win95 data tree -- the same universe
    l2sweep uses, enumerated locally to keep the behavior engine decoupled from the target."""
    root = os.environ.get("WT_DATA_ROOT") or os.getcwd()
    seen, out = set(), []
    for sub in ("files/WINDOWS/SYSTEM", "files/WINDOWS/SYSTEM32", "files/WINDOWS"):
        for ext in ("dll", "exe", "drv", "cpl"):
            for p in sorted(glob.glob(os.path.join(root, sub, "*." + ext))):
                nm = os.path.basename(p).lower()
                if nm in seen:
                    continue
                seen.add(nm)
                if _i386_pe(p):
                    out.append(nm)
    return out


def _evict():
    """Drop per-module caches so memory stays flat across the corpus."""
    for c in (getattr(tracer, "_PE_CACHE", None), getattr(graph, "_CACHE", None)):
        if isinstance(c, dict):
            c.clear()


def sweep(limit_modules=None, per_module=15, trace_n=25, start=0, emit=None):
    mods = pe_modules()[start:]
    if limit_modules:
        mods = mods[:limit_modules]
    print("behavioral sweep: %d PE modules (per-module<=%d, trace=%d)\n" % (len(mods), per_module, trace_n))
    tot = Counter()
    by_method = Counter()
    per = []
    if emit:
        os.makedirs(emit, exist_ok=True)
    for m in mods:
        try:
            rep = recon.reconstruct_module(m, limit=per_module, trace_n=trace_n)
        except Exception as e:
            per.append({"module": m, "error": str(e)[:120]})
            print("  %-20s ERROR %s" % (m, str(e)[:70]))
            _evict(); continue
        tot["attempted"] += rep["attempted"]
        tot["reconstructed"] += rep["reconstructed"]
        tot["verified"] += rep["verified"]
        for r in rep["results"]:
            by_method[r["method"] or "unresolved"] += 1
        per.append({"module": m, "attempted": rep["attempted"], "reconstructed": rep["reconstructed"],
                    "verified": rep["verified"], "yield_pct": rep["yield_pct"]})
        print("  %-20s %2d/%-2d reconstructed (%.0f%%), %d verified"
              % (m, rep["reconstructed"], rep["attempted"], rep["yield_pct"], rep["verified"]))
        if emit and rep["reconstructed"]:
            stem = m.replace(".", "_")
            with open(os.path.join(emit, stem + ".behav.c"), "w", encoding="utf-8") as f:
                f.write("/* %s -- behavioral reconstruction (wt behavior bsweep) */\n\n" % m)
                for r in rep["results"]:
                    if r.get("c"):
                        f.write("/* sub_%x [%s%s] */\n%s\n\n"
                                % (r["func_va"], r["method"],
                                   " VERIFIED" if r["verified"] else "", r["c"].rstrip()))
        _evict()
    att = max(1, tot["attempted"])
    print("\n" + "=" * 62)
    print("CORPUS BEHAVIORAL SWEEP -- %d modules" % len([p for p in per if "error" not in p]))
    print("  functions attempted   : %d" % tot["attempted"])
    print("  reconstructed (C)     : %d  (%.1f%%)" % (tot["reconstructed"], 100.0 * tot["reconstructed"] / att))
    print("  verified byte-exact   : %d" % tot["verified"])
    print("  by method             : %s" % ", ".join("%s=%d" % kv for kv in by_method.most_common()))
    top = sorted((p for p in per if "error" not in p and p["reconstructed"]),
                 key=lambda p: -p["reconstructed"])[:12]
    print("\n  top modules by reconstructed count:")
    for p in top:
        print("    %-20s %d/%d (%.0f%%)" % (p["module"], p["reconstructed"], p["attempted"], p["yield_pct"]))
    if emit:
        with open(os.path.join(emit, "_bsweep_summary.json"), "w") as f:
            json.dump({"totals": dict(tot), "by_method": dict(by_method), "modules": per}, f, indent=1)
        print("\n  wrote per-module .behav.c + _bsweep_summary.json to %s" % emit)
    return per


def main():
    argv = sys.argv[1:]
    if argv and argv[0] in ("-h", "--help"):
        print(__doc__); return
    lim = _opt(argv, "--modules"); lim = int(lim) if lim is not None else None
    per = int(_opt(argv, "--per-module", 15))
    trace_n = int(_opt(argv, "--trace", 25))
    start = int(_opt(argv, "--start", 0))
    emit = _opt(argv, "--emit")
    sweep(limit_modules=lim, per_module=per, trace_n=trace_n, start=start, emit=emit)


if __name__ == "__main__":
    main()
