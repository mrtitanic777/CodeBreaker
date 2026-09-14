#!/usr/bin/env python3
"""origin_triage.py -- classify every decompiled NE function by ORIGIN, so the
reconstruction surface (app/OS-specific code) is separated from what must be
IDENTIFIED not reconstructed (CRT, compiler helpers, hardware asm, stubs).

    wt --target win95 origin_triage [--lane ne|pe|le|mz] [--sig] [--dir <lane>]

Core methodology (identify-don't-reconstruct-crt-compiler): only app/OS-specific
code is a reconstruction target; CRT + compiler-generated code belongs to the
toolchain and is identified by signature, not re-authored.

Works on the <lane>_lane/*.c output using signals computed reliably from the C:
export-name ground truth, CRT/compiler NAME patterns, body shape (asm-dominant =
hardware/driver asm; trivial = stub; VxDCall_-dominant = OS service shim;
structured = app). Writes TRIAGE.csv + a histogram.

--sig folds in the BINARY-level CRT/compiler floor via nativesig (crtsig
reloc-masked byte-prefix matching against the shipped Win95 CRT DLLs + sigscan
structural x86 shapes). This is the precision upgrade: it separates the sub_
CRT/helper bulk from genuine app leaf functions using the actual bytes, not names.
Byte-prefix matching is 32-bit (PE + LE); 16-bit lanes (NE/MZ) get sigscan shapes
only (a 16-bit CRT byte DB needs an OMF-lib reader -- a follow-up).
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


def classify(name, body, crt_hit=None):
    """Return (origin, why). crt_hit=(matched_name, source) from nativesig (crtsig
    byte-prefix or sigscan shape) is ground truth from the binary and wins over the
    name/shape heuristics -- this is the CRT/compiler floor, identified not
    reconstructed."""
    if crt_hit:
        return "crt/compiler", "%s:%s" % (crt_hit[1], crt_hit[0])
    lines = body.count("\n") + 1
    asm = body.count("__asm")
    stmts = body.count(";")
    calls = len(re.findall(r"\b[A-Z][A-Z0-9_]{2,}\s*\(", body))   # UPPERCASE api-style calls
    fields = body.count("->")
    svc = body.count("VxDCall_")                                  # VxD service invocations
    named = not name.startswith("sub_") and not name.startswith("export_")

    if CRT_RE.match(name):
        return "crt/compiler", "name"
    # VxD service shim: body dominated by VMM/VxD service calls -> OS-service code,
    # identified (like a thunk), not a from-scratch reconstruction target.
    if svc >= 2 and svc * 3 >= max(stmts, 1):
        return "vxd/service", "service-heavy"
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


_ADDR_RE = re.compile(r"@ 0x([0-9A-Fa-f]+)")


def split_functions(text):
    """Yield (name, addr, body) for each function (brace-matched). addr is the linear
    address from the decompiler's `// <name>  @ 0x...` header (int), or None."""
    for m in FUNC_RE.finditer(text):
        name = m.group(1)
        # the emitted function header just above the signature carries `@ 0x<addr>`
        am = None
        for am in _ADDR_RE.finditer(text, max(0, m.start() - 200), m.start()):
            pass
        addr = int(am.group(1), 16) if am else None
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
        yield name, addr, text[i:j + 1]


def main(argv):
    ap = argparse.ArgumentParser(prog="origin_triage", description=__doc__.splitlines()[0])
    ap.add_argument("--lane", choices=("ne", "pe", "le", "mz"), default="ne",
                    help="which decompile lane to triage (default: ne)")
    ap.add_argument("--dir", default=None, help="lane dir (default: <design>/<lane>_lane)")
    ap.add_argument("--sig", action="store_true",
                    help="identify the CRT/compiler floor on the BINARIES via "
                         "nativesig (crtsig byte-prefix + sigscan shapes), not names")
    a = ap.parse_args(argv)
    corpus = os.environ.get("WT_DATA_ROOT")
    if not corpus or not os.path.isdir(corpus):
        corpus = r"C:\Users\mrtit\Documents\GitHub\Windows95\design\corpus\files"
    d = a.dir
    if not d:
        # Resolve <design>/<lane>_lane the SAME way the *_scale sweeps do, so triage
        # reads exactly where the sweep wrote.
        design = os.path.dirname(os.path.dirname(corpus)) \
            if corpus.lower().rstrip("\\/").endswith("files") else os.path.dirname(corpus)
        d = os.path.join(design, a.lane + "_lane")
    if not os.path.isdir(d):
        sys.exit("no %s_lane dir at %s (run %s_scale first)" % (a.lane, d, a.lane))

    # --sig: index the corpus binaries by name so each <module>.c maps to its binary,
    # which nativesig identifies the CRT/compiler floor on.
    binindex = {}
    nativesig = None
    if a.sig:
        from src.targets.win95 import nativesig as _ns
        nativesig = _ns
        for dp, _, fs in os.walk(corpus):
            for fnm in fs:
                binindex.setdefault(fnm.lower(), os.path.join(dp, fnm))

    rows = []
    from collections import Counter
    hist = Counter(); per_mod = {}
    for fn in sorted(os.listdir(d)):
        if not fn.endswith(".c"):
            continue
        mod = fn[:-2]
        text = open(os.path.join(d, fn), encoding="utf-8", errors="replace").read()
        crtmap = {}
        if nativesig:
            binp = binindex.get(mod.lower())
            if binp:
                try:
                    crtmap = nativesig.identify_module(binp)
                except Exception:
                    crtmap = {}
        mc = Counter()
        for name, addr, body in split_functions(text):
            origin, why = classify(name, body, crtmap.get(addr) if addr is not None else None)
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
    print("identify-only (crt/compiler + vxd/service + asm/hardware + stub): %d"
          % (hist["crt/compiler"] + hist["vxd/service"] + hist["asm/hardware"] + hist["stub"]))
    print("TRIAGE.csv -> %s" % os.path.join(d, "TRIAGE.csv"))


if __name__ == "__main__":
    main(sys.argv[1:])
