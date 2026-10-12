#!/usr/bin/env python3
"""ne_scale.py -- run CodeBreaker's native decompiler across every code-bearing
NE (16-bit) module in the corpus, the native engine's home turf (where Ghidra
dropped functions). Writes each module's C to <design>/ne_lane/ and a scoreboard.

    wt --target win95 ne_scale [--limit N] [--timeout S] [--only substr]

Metrics per module: functions, __asm blocks (unhandled/real asm), lines, seconds,
status (ok/empty/timeout/error). Decompiled C is staging output (curated into the
one source tree later), so it lands under design/ne_lane, not the root tree.
"""
import argparse
import csv
import os
import struct
import sys
import time

from src.core import paths
from src.targets.win95 import nativedec

SKIP_EXT = {".fon", ".tlb", ".vbx"}     # fonts / typelibs / VB controls: no reconstructable code


def _is_ne(p):
    try:
        with open(p, "rb") as f:
            h = f.read(0x40)
            if len(h) < 0x40 or h[:2] != b"MZ":
                return False
            e = struct.unpack_from("<H", h, 0x3c)[0]
            f.seek(e)
            return f.read(2) == b"NE"
    except OSError:
        return False


def _corpus():
    from src.core import corpus
    return corpus.root()


def find_ne(corpus, only=None):
    out = []
    for dp, _, fs in os.walk(corpus):
        for fn in fs:
            if os.path.splitext(fn)[1].lower() in SKIP_EXT:
                continue
            if only and only.lower() not in fn.lower():
                continue
            p = os.path.join(dp, fn)
            if _is_ne(p):
                out.append(p)
    return sorted(out)


def main(argv):
    ap = argparse.ArgumentParser(prog="ne_scale", description=__doc__.splitlines()[0])
    ap.add_argument("--limit", type=int, default=0)
    ap.add_argument("--timeout", type=int, default=180)
    ap.add_argument("--only", default=None, help="only modules whose name contains this")
    a = ap.parse_args(argv)
    if not nativedec.available():
        sys.exit("win95dec not built: cmake --build native/decompiler/build --config Release --target win95dec")

    corpus = _corpus()
    design = os.path.dirname(os.path.dirname(corpus)) if corpus.lower().rstrip("\\/").endswith("files") \
        else os.path.dirname(corpus)
    outdir = os.path.join(design, "ne_lane")
    os.makedirs(outdir, exist_ok=True)

    mods = find_ne(corpus, a.only)
    if a.limit:
        mods = mods[:a.limit]
    print("NE modules to decompile: %d  ->  %s" % (len(mods), outdir), flush=True)

    rows = []
    for i, p in enumerate(mods):
        name = os.path.basename(p)
        t0 = time.time()
        status = "ok"; funcs = asm = lines = 0
        try:
            c = nativedec.decompile_module(p, timeout=a.timeout)
            funcs = c.count("__fastcall"); asm = c.count("__asm"); lines = c.count("\n")
            if funcs == 0:
                status = "empty"
            with open(os.path.join(outdir, name + ".c"), "w", encoding="utf-8", errors="replace") as f:
                f.write(c)
        except Exception as e:                       # timeout, decode, subprocess
            status = type(e).__name__
        dt = time.time() - t0
        rows.append((name, status, funcs, asm, lines, round(dt, 1)))
        print("[%d/%d] %-22s %-12s funcs=%-5d asm=%-5d %.1fs" %
              (i + 1, len(mods), name, status, funcs, asm, dt), flush=True)

    with open(os.path.join(outdir, "SCOREBOARD.csv"), "w", newline="") as f:
        w = csv.writer(f); w.writerow(["module", "status", "funcs", "asm", "lines", "secs"]); w.writerows(rows)

    ok = sum(1 for r in rows if r[1] == "ok")
    tot_funcs = sum(r[2] for r in rows)
    print("\n=== %d/%d modules OK; %d functions total; scoreboard -> %s\\SCOREBOARD.csv ===" %
          (ok, len(rows), tot_funcs, outdir))
    bad = [r for r in rows if r[1] not in ("ok",)]
    if bad:
        print("non-OK modules:")
        for r in bad:
            print("  %-22s %s" % (r[0], r[1]))


if __name__ == "__main__":
    main(sys.argv[1:])
