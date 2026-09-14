#!/usr/bin/env python3
"""origin_triage.py -- classify every decompiled NE function by ORIGIN, so the
reconstruction surface (app/OS-specific code) is separated from what must be
IDENTIFIED not reconstructed (CRT, compiler helpers, hardware asm, stubs).

    wt --target win95 origin_triage [--dir <ne_lane>]

Core methodology (identify-don't-reconstruct-crt-compiler): only app/OS-specific
code is a reconstruction target; CRT + compiler-generated code belongs to the
toolchain and is identified by signature, not re-authored.

This first pass works on the ne_lane/*.c output using signals we can compute
reliably: export-name ground truth, CRT/compiler NAME patterns, and body shape
(asm-dominant = hardware/driver asm; trivial = stub; structured = app). It writes
TRIAGE.csv + a histogram. PRECISION UPGRADE (noted): fold in the symbol engine's
crtsig (FLIRT-style CRT/compiler DB from the period 16-bit CRT lib) and sigscan
to separate the sub_ CRT/helper bulk from genuine app-internal leaf functions.
"""
import argparse
import csv
import os
import re
import sys

from src.core import paths

# CRT / compiler-generated helper name signatures (MSC/Watcom 16-bit).
CRT_RE = re.compile(r"""^_{1,3}(
    chkstk|aullshr|aullrem|aulldiv|allmul|allshl|allshr|alldiv|allrem|
    aFldiv|aFlmul|aFldrem|aFuldiv|aFulrem|aNlmul|aNldiv|aNlshl|aNlshr|
    ahshift|aFptr|aNptr|fptrap|EH_prolog|EH_epilog|ftol|ftbol|
    fmemcpy|fmemset|fmemcmp|fstrlen|fstrcpy|fstrcat|fmalloc|ffree|
    setargv|setenvp|astart|cinit|amsg_exit|nullcheck|dosmaperr|
    Fmemcpy|Fmemset)$""", re.X | re.I)


def classify(name, body):
    """Return (origin, why)."""
    lines = body.count("\n") + 1
    asm = body.count("__asm")
    stmts = body.count(";")
    calls = len(re.findall(r"\b[A-Z][A-Z0-9_]{2,}\s*\(", body))   # UPPERCASE api-style calls
    fields = body.count("->")
    named = not name.startswith("sub_") and not name.startswith("export_")

    if CRT_RE.match(name):
        return "crt/compiler", "name"
    # asm-dominant: hardware/driver port I/O, genuine asm -> identify, keep as asm
    if asm >= 3 and asm * 3 >= max(stmts, 1):
        return "asm/hardware", "asm-heavy"
    # trivial stub (e.g. `return 1;`)
    if stmts <= 2 and calls == 0 and fields == 0:
        return "stub", "trivial"
    if named:
        return "app-export", "named export"
    # structured internal logic -> app-internal (reconstruction target)
    if stmts >= 4 or calls >= 1 or fields >= 1:
        return "app-internal", "structured"
    return "unknown", "unclassified"


FUNC_RE = re.compile(
    r"^[A-Za-z_].*?\b__fastcall\s+([A-Za-z_]\w*)\s*\([^)]*\)\s*\n\{", re.M)


def split_functions(text):
    """Yield (name, body) for each function in a ne_lane .c file (brace-matched)."""
    for m in FUNC_RE.finditer(text):
        name = m.group(1)
        i = text.find("{", m.start())
        depth = 0; j = i
        while j < len(text):
            c = text[j]
            if c == "{": depth += 1
            elif c == "}":
                depth -= 1
                if depth == 0:
                    break
            j += 1
        yield name, text[i:j + 1]


def main(argv):
    ap = argparse.ArgumentParser(prog="origin_triage", description=__doc__.splitlines()[0])
    ap.add_argument("--dir", default=None, help="ne_lane dir (default: <design>/ne_lane)")
    a = ap.parse_args(argv)
    d = a.dir
    if not d:
        corpus = os.environ.get("WT_DATA_ROOT", "")
        design = os.path.dirname(os.path.dirname(corpus)) if corpus else paths.design_root()
        d = os.path.join(design, "ne_lane")
    if not os.path.isdir(d):
        sys.exit("no ne_lane dir at %s (run ne_scale first)" % d)

    rows = []
    from collections import Counter
    hist = Counter(); per_mod = {}
    for fn in sorted(os.listdir(d)):
        if not fn.endswith(".c"):
            continue
        mod = fn[:-2]
        text = open(os.path.join(d, fn), encoding="utf-8", errors="replace").read()
        mc = Counter()
        for name, body in split_functions(text):
            origin, why = classify(name, body)
            rows.append((mod, name, origin, why))
            hist[origin] += 1; mc[origin] += 1
        per_mod[mod] = mc

    with open(os.path.join(d, "TRIAGE.csv"), "w", newline="") as f:
        w = csv.writer(f); w.writerow(["module", "function", "origin", "why"]); w.writerows(rows)

    total = sum(hist.values())
    print("=== origin histogram (%d functions across %d modules) ===" % (total, len(per_mod)))
    for origin, n in hist.most_common():
        print("  %-14s %6d  %5.1f%%" % (origin, n, 100.0 * n / max(total, 1)))
    targets = hist["app-export"] + hist["app-internal"]
    print("\nreconstruction targets (app-export + app-internal): %d  (%.1f%%)"
          % (targets, 100.0 * targets / max(total, 1)))
    print("identify-only (crt/compiler + asm/hardware + stub): %d"
          % (hist["crt/compiler"] + hist["asm/hardware"] + hist["stub"]))
    print("TRIAGE.csv -> %s" % os.path.join(d, "TRIAGE.csv"))


if __name__ == "__main__":
    main(sys.argv[1:])
