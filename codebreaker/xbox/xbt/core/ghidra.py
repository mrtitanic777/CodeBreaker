#!/usr/bin/env python3
"""
ghidra.py — attach Ghidra's decompiler as a HYPOTHESIS source for a module.

The deterministic lifter (`lift.py`) transliterates 16-bit NE faithfully but
leaves flag predicates as "condition not recovered" and cannot touch PE. Ghidra
fills both gaps: its decompiler recovers real control flow / expressions, runs
on x86:LE:16 Real Mode (NE) *and* PE, and produces readable C. But that C is a
GUESS (invented names, segmented-pointer slips) — a cross-check to confirm
against the retail bytes, never truth (CONTEXT.md). Unlike win1.0 this needs no
flat-image manifest: Win95 modules are ordinary NE/PE files Ghidra loads itself.

Runs Ghidra headless (`analyzeHeadless`) on `files/<module>`, imports + analyzes
it once into a reusable project, and runs `ghidra_scripts/win95_decomp.py` to
dump each function's decompiled C to `reports/ghidra/<module>/`.

Requires a JDK (Ghidra 11.3.2 needs 17+) on PATH or JAVA_HOME.

    python ghidra.py USER.EXE
    python ghidra.py SHELL32.DLL --keep     # keep the Ghidra project
"""
import argparse
import os
import subprocess
import sys

import target

ROOT = os.path.dirname(os.path.abspath(__file__))
import config as _config                                       # same-dir sibling
GHIDRA = _config.tool("ghidra")
HEADLESS = os.path.join(GHIDRA, "support", "analyzeHeadless.bat")
PROJDIR = os.path.join(ROOT, "ghidra_proj")


def run(module, keep=False, timeout=1800):
    path = os.path.abspath(target.resolve(module))
    base = os.path.basename(path)
    outdir = os.path.join(ROOT, "reports", "ghidra", base)
    os.makedirs(PROJDIR, exist_ok=True)
    os.makedirs(outdir, exist_ok=True)
    if not os.path.exists(HEADLESS):
        sys.exit("analyzeHeadless not found: %s" % HEADLESS)

    cmd = [HEADLESS, PROJDIR, "win95",
           "-import", path, "-overwrite",
           "-scriptPath", os.path.join(ROOT, "ghidra_scripts"),
           "-postScript", "win95_decomp.py", outdir]
    if keep:
        cmd.remove("-overwrite")
    print("# %s" % " ".join('"%s"' % c if " " in c else c for c in cmd))
    try:
        r = subprocess.run(cmd, capture_output=True, text=True, timeout=timeout,
                           errors="replace")
    except subprocess.TimeoutExpired:
        sys.exit("Ghidra timed out after %ds (big module? raise --timeout)" % timeout)
    tail = (r.stdout + r.stderr).strip().splitlines()
    for line in tail[-12:]:
        print("  " + line)
    n = len([f for f in os.listdir(outdir) if f.endswith(".c")])
    print("# %s: %d decompiled functions -> %s" % (base, n, outdir))
    if n == 0 and ("JAVA_HOME" in (r.stdout + r.stderr) or "java" in (r.stdout + r.stderr).lower()):
        print("# (looks like a JDK issue — Ghidra 11.3.2 needs JDK 17+ on PATH/JAVA_HOME)")
    return outdir, n


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("module")
    ap.add_argument("--keep", action="store_true", help="keep project (don't overwrite)")
    ap.add_argument("--timeout", type=int, default=1800)
    args = ap.parse_args()
    run(args.module, keep=args.keep, timeout=args.timeout)


if __name__ == "__main__":
    main()
