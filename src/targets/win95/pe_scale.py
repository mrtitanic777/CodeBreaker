#!/usr/bin/env python3
"""pe_scale.py -- run CodeBreaker's native decompiler across every code-bearing
PE (32-bit) module in the corpus: the win32 side of the same engine that ne_scale
drives for NE. Writes each module's C to <design>/pe_lane/ and a scoreboard.

    wt --target win95 pe_scale [--limit N] [--timeout S] [--only substr]

This is the NATIVE engine's PE pass (PE loader -> x86 decode -> 32-bit lift ->
DecompileToC), distinct from the behavior_engine's dynamic PE deep lane -- readable
static C for all 638 PE modules, no Ghidra. Decompiled C is staging output (curated
into the one source tree later), so it lands under design/pe_lane.
"""
import argparse
import csv
import os
import struct
import sys
import time

from src.targets.win95 import nativedec

# fonts / typelibs / VB controls carry no reconstructable code; .mbx/.cpl are PE
# but scanning by header (below) already screens format, so we only skip by ext.
SKIP_EXT = {".fon", ".tlb", ".vbx"}


def _is_pe(p):
    try:
        with open(p, "rb") as f:
            h = f.read(0x40)
            if len(h) < 0x40 or h[:2] != b"MZ":
                return False
            e = struct.unpack_from("<H", h, 0x3c)[0]
            f.seek(e)
            if f.read(4) != b"PE\0\0":
                return False
            f.read(20)                                  # skip COFF header
            return struct.unpack("<H", f.read(2))[0] == 0x10B   # optional-header magic: PE32 only
    except OSError:
        return False


def _corpus():
    from src.core import corpus
    return corpus.root()


def find_pe(corpus, only=None):
    out = []
    for dp, _, fs in os.walk(corpus):
        for fn in fs:
            if os.path.splitext(fn)[1].lower() in SKIP_EXT:
                continue
            if only and only.lower() not in fn.lower():
                continue
            p = os.path.join(dp, fn)
            if _is_pe(p):
                out.append(p)
    return sorted(out)


def main(argv):
    ap = argparse.ArgumentParser(prog="pe_scale", description=__doc__.splitlines()[0])
    ap.add_argument("--limit", type=int, default=0)
    ap.add_argument("--timeout", type=int, default=300)
    ap.add_argument("--only", default=None, help="only modules whose name contains this")
    a = ap.parse_args(argv)
    if not os.path.exists(nativedec._PEBIN):
        sys.exit("win95pedec not built: cmake --build native/decompiler/build --config Release --target win95pedec")

    corpus = _corpus()
    design = os.path.dirname(os.path.dirname(corpus)) if corpus.lower().rstrip("\\/").endswith("files") \
        else os.path.dirname(corpus)
    outdir = os.path.join(design, "pe_lane")
    os.makedirs(outdir, exist_ok=True)

    mods = find_pe(corpus, a.only)
    if a.limit:
        mods = mods[:a.limit]
    print("PE modules to decompile: %d  ->  %s" % (len(mods), outdir), flush=True)

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
