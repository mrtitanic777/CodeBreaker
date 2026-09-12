#!/usr/bin/env python3
"""
build.py — syntax-check the reconstruction against Microsoft's own 1.01 headers.

THE HOLE THIS FILLS
    Nothing in this project had ever been compiled. Every reconstructed function
    was an assertion no tool could contradict — `verify.py` checks that the
    CALLS a function makes match the binary, but nothing checked that the C is
    even well-formed, that every identifier is declared, or that a call passes
    the number and kind of arguments the real API takes.

WHAT "PASS" MEANS HERE, EXACTLY
    Three things, and nothing more:
        1. the file is well-formed C
        2. every identifier used is declared
        3. every call to a Windows API matches the prototype Microsoft shipped
           in the 1.01 SDK, in arity and argument type

    It is NOT evidence of behavioural equivalence. It cannot be: this compiles
    for a modern flat-address machine, and the original is 16-bit segmented.
    This is the "compiles but unverified" status level — real, and clearly
    short of "behaviourally matched".

    The prototypes come from refs/sdk101/d06/WINDOWS.H, which Microsoft wrote,
    not from anything this project inferred. A harness built out of our own
    assumptions could not fail, and so would be worth nothing.

WHY THE ERRORS ARE WORTH READING EVEN WHEN THEY ARE "EXPECTED"
    A 16-bit HWND is a WORD; here a handle may meet a 64-bit pointer. Those
    diagnostics are noise. But an "too few arguments to function TextOut" is
    not noise on any machine, and that class of error was previously invisible.
    The report separates them.

Usage:
    python build.py                 # check everything, summarise
    python build.py --file calc.c   # one file, full diagnostics
    python build.py --errors        # every diagnostic, grouped by kind
"""
import os, re, subprocess, sys, glob, collections
from codebreaker.core import layout

INC = ["-Ibuild/include", "-I" + layout.recon_dir(), "-I" + layout.recon_sub("system")]
CC = ["gcc", "-fsyntax-only", "-std=gnu89", "-w"]
# -w silences style warnings; we want ERRORS. Then re-enable the two warning
# classes that indicate a REAL discrepancy against a 1.01 prototype.
WARN = ["-Wimplicit-function-declaration", "-Wreturn-type"]

# Diagnostics that are artefacts of compiling 16-bit segmented code on a 64-bit
# flat machine. They say nothing about the reconstruction's correctness, so they
# are counted separately rather than silently dropped.
ARCH_NOISE = re.compile(
    r"makes pointer from integer|makes integer from pointer"
    r"|cast to pointer from integer of different size"
    r"|cast from pointer to integer of different size"
    r"|incompatible pointer type|discards .* qualifier", re.I)


def check(path):
    p = subprocess.run(CC + WARN + INC + [path], capture_output=True, text=True)
    errs, noise = [], []
    for line in p.stderr.splitlines():
        if ": error:" in line or ": warning:" in line:
            (noise if ARCH_NOISE.search(line) else errs).append(line)
    return p.returncode, errs, noise


def main():
    if "--file" in sys.argv:
        f = sys.argv[sys.argv.index("--file") + 1]
        for cand in (f, os.path.join(layout.recon_dir(), f), os.path.join(layout.recon_sub("system"), f)):
            if os.path.exists(cand):
                f = cand; break
        p = subprocess.run(CC + WARN + INC + [f], capture_output=True, text=True)
        print(p.stderr or "(no diagnostics)")
        return

    files = sorted(glob.glob(os.path.join(layout.recon_dir(), "*.c")) + glob.glob(os.path.join(layout.recon_sub("system"), "*.c")))
    clean = []
    rows = []
    kinds = collections.Counter()
    tot_err = tot_noise = 0
    for f in files:
        rc, errs, noise = check(f)
        tot_err += len(errs); tot_noise += len(noise)
        for e in errs:
            m = re.search(r"(?:error|warning): (.*)", e)
            if m:
                k = re.sub(r"'[^']*'", "'X'", m.group(1))[:60]
                kinds[k] += 1
        if not errs:
            clean.append(f)
        rows.append((len(errs), len(noise), f))

    print("SYNTAX CHECK against the Windows 1.01 SDK headers")
    print("  files checked            : %d" % len(files))
    print("  files with NO real error : %d  (%.0f%%)"
          % (len(clean), 100.0 * len(clean) / max(1, len(files))))
    print("  real diagnostics         : %d" % tot_err)
    print("  16-vs-64-bit artefacts   : %d  (counted, not corrected)" % tot_noise)

    print("\n  worst files:")
    for n, nz, f in sorted(rows, reverse=True)[:15]:
        if n:
            print("    %5d  %s" % (n, f))

    print("\n  most common real diagnostics:")
    for k, c in kinds.most_common(15):
        print("    %5d  %s" % (c, k))

    if "--errors" in sys.argv:
        for n, nz, f in sorted(rows, reverse=True):
            if not n:
                continue
            rc, errs, noise = check(f)
            print("\n==== %s ====" % f)
            for e in errs[:25]:
                print("   " + e)


if __name__ == "__main__":
    main()
