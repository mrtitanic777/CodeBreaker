#!/usr/bin/env python3
r"""l2sweep.py -- run the Layer-2 conductor (l2pipe) across many PE modules.

Enumerates the 32-bit PE modules of the corpus and drives each to buildable
byte-exact source unattended, aggregating a corpus coverage summary. This is the
"point it at the whole system" wrapper -- the scale-up the manual per-function
work was blocking.

    python -m engine.targets.win95.l2sweep [--limit N] [--emit DIR] [--start K]
"""
import argparse
import glob
import json
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import pe32util as H
import l2pipe
import cc32


def _evict():
    """Drop per-module caches between modules. They only help WITHIN a module
    (emit_module / shipped_pe_function hit them many times for one module); across
    the corpus they'd otherwise accumulate 549 full PE carves and bloat the process
    into GC-thrash (~module 500). Clearing here is free and keeps memory flat."""
    for c in (H._CARVE, H._NAMED, H._IMPORTS, cc32._PE_CACHE, l2pipe._SG):
        c.clear()


def _i386_pe(path):
    """Cheap header check: MZ -> e_lfanew -> PE\\0\\0 -> machine == 0x14c (i386),
    without carving the whole module."""
    import struct
    try:
        with open(path, "rb") as f:
            d = f.read(0x400)
        if d[:2] != b"MZ":
            return False
        pe = struct.unpack_from("<I", d, 0x3c)[0]
        if pe + 6 > len(d) or d[pe:pe + 4] != b"PE\0\0":
            with open(path, "rb") as f:
                f.seek(pe); head = f.read(6)
            return head[:4] == b"PE\0\0" and struct.unpack_from("<H", head, 4)[0] == 0x14c
        return struct.unpack_from("<H", d, pe + 4)[0] == 0x14c
    except Exception:
        return False


def pe_modules():
    """32-bit PE modules (i386) under the system tree, de-duplicated by basename.
    Fast header check only -- carving happens during the sweep itself."""
    H.ensure_target()
    seen, out = set(), []
    for sub in ("files/WINDOWS/SYSTEM", "files/WINDOWS/SYSTEM32", "files/WINDOWS"):
        for ext in ("dll", "exe", "drv", "cpl"):
            for p in sorted(glob.glob(os.path.join(sub, "*." + ext))):
                nm = os.path.basename(p).lower()
                if nm in seen:
                    continue
                seen.add(nm)
                if _i386_pe(p):
                    out.append(nm)
    return out


def sweep(limit=None, emit=None, start=0, floor=True):
    mods = pe_modules()[start:]
    if limit:
        mods = mods[:limit]
    print("Layer-2 sweep: %d PE modules (floor=%s)\n" % (len(mods), floor))
    tot = dict(mods=0, fns=0, cleanc=0, asm=0, pending=0, escalate=0, review=0, err=0)
    summary = []
    for m in mods:
        try:
            led = l2pipe.run(m, emit=emit, floor=floor)
            nc = sum(1 for r in led if r["kind"] == "clean-c")
            na = sum(1 for r in led if r["kind"] == "asm")
            np_ = sum(1 for r in led if r["kind"] == "asm-pending")
            ne = sum(1 for r in led if r["kind"] == "escalate")
            rv = sum(1 for r in led if r.get("was_c") and r["kind"] in ("asm", "asm-pending"))
            tot["mods"] += 1; tot["fns"] += len(led); tot["cleanc"] += nc
            tot["asm"] += na; tot["pending"] += np_; tot["escalate"] += ne; tot["review"] += rv
            summary.append(dict(module=m, fns=len(led), cleanc=nc, asm=na, pending=np_, escalate=ne, review=rv))
        except Exception as e:
            tot["err"] += 1
            summary.append(dict(module=m, error=str(e)[:200]))
            print("  %-20s ERROR %s" % (m, str(e)[:80]))
        _evict()                                          # keep memory flat across the corpus
    print("\n" + "=" * 60)
    print("CORPUS LAYER-2 SWEEP -- %d modules, %d functions" % (tot["mods"], tot["fns"]))
    cov = tot["cleanc"] + tot["asm"]
    print("  clean C (byte-exact)  : %d" % tot["cleanc"])
    if floor:
        print("  byte-exact ASM        : %d" % tot["asm"])
        print("  COVERED buildable     : %d/%d (%.1f%%)" % (cov, tot["fns"], 100.0 * cov / max(1, tot["fns"])))
        print("  escalated             : %d" % tot["escalate"])
    else:
        print("  clean C rate          : %d/%d (%.1f%%)" % (tot["cleanc"], tot["fns"], 100.0 * tot["cleanc"] / max(1, tot["fns"])))
        print("  ASM floor pending     : %d" % tot["pending"])
    print("  clean-C review worklist: %d" % tot["review"])
    print("  module errors         : %d" % tot["err"])
    if emit:
        with open(os.path.join(emit, "_corpus_l2_summary.json"), "w") as f:
            json.dump(dict(totals=tot, modules=summary), f, indent=1)
        print("  wrote _corpus_l2_summary.json")
    return summary


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--limit", type=int)
    ap.add_argument("--start", type=int, default=0)
    ap.add_argument("--emit")
    ap.add_argument("--no-floor", action="store_true",
                    help="clean-C only; skip the (slow, emitter-independent) byte-exact ASM floor")
    ap.add_argument("--list", action="store_true", help="just list the PE modules")
    a = ap.parse_args()
    if a.list:
        for m in pe_modules():
            print(m)
        return
    sweep(a.limit, a.emit, a.start, floor=not a.no_floor)


if __name__ == "__main__":
    main()
