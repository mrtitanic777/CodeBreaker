#!/usr/bin/env python3
"""
asmratio.py — measure how much of Windows 1.01 was written in C and how much in
assembler, per module, from the binaries themselves.

WHY THIS IS A MEASUREMENT AND NOT A GUESS, AND WHERE IT STOPS BEING ONE

Microsoft C 3.0 opens any function that has parameters or locals with exactly
    55        push bp
    8B EC     mov  bp,sp
and reaches its arguments as [bp+n]. It has no other way of doing it. So a
function that does NOT start that way was either written in assembler or is a C
function with no frame at all — and C 3.0 emits a frame for anything taking
arguments, which in a Windows module is nearly everything.

The converse is weaker, and that is the honest limit of this tool: the SDK's
assembler macros (cmacros.inc — `cProc`/`cBegin`, which the SDK's own INDOS.ASM
uses) generate the SAME prologue. So:

    frame%   is an UPPER BOUND on how much of a module is C
    no-frame is a LOWER BOUND on how much is assembler

A module at 90% frames is mostly C. A module at 20% cannot be. That is enough
to answer "which parts of the OS were C" without pretending to more precision
than the evidence carries.

The app layer has a harder answer that needs no inference at all: the SDK ships
CARDFILE's real sources — 16 C modules and 2 .ASM modules (INDOS.ASM/INDOS2.ASM,
the DOS-interrupt shims). That is the shape of a 1985 Windows application.

Usage:  python asmratio.py            (all system modules)
        python asmratio.py KERNEL GDI
"""
import sys

import idord
import ledger

FRAME = bytes([0x55, 0x8B, 0xEC])          # push bp; mov bp,sp
ENTER = 0xC8                                # 186 `enter` — C 3.0 does not use it


def classify(ne, fns):
    """-> (n_frame, n_noframe, total_bytes_frame, total_bytes_noframe)"""
    segs = {s["idx"]: s for s in ne.segments()}
    nf = nn = bf = bn = 0
    for (seg, off), size in fns.items():
        s = segs.get(seg)
        if not s:
            continue
        base = s["file_off"] + off
        head = ne.data[base:base + 3]
        if head == FRAME:
            nf += 1; bf += size
        else:
            nn += 1; bn += size
    return nf, nn, bf, bn


def main():
    want = [a.upper() for a in sys.argv[1:] if not a.startswith("-")]
    mods = want or ledger.SYSMODS
    print("%-10s %6s %8s %8s   %-24s" %
          ("MODULE", "fns", "C-frame", "no-frame", "frame% (upper bound on C)"))
    print("-" * 72)
    tf = tn = 0
    for m in mods:
        try:
            ne = idord.open_module(m)
            fns = ledger.carve(ne)
        except Exception as e:
            print("%-10s  (skipped: %s)" % (m, e)); continue
        nf, nn, bf, bn = classify(ne, fns)
        tot = nf + nn
        if not tot:
            continue
        tf += nf; tn += nn
        pct = 100.0 * nf / tot
        bar = "#" * int(pct / 4)
        print("%-10s %6d %8d %8d   %5.1f%%  %s" % (m, tot, nf, nn, pct, bar))
    if tf + tn:
        print("-" * 72)
        print("%-10s %6d %8d %8d   %5.1f%%"
              % ("TOTAL", tf + tn, tf, tn, 100.0 * tf / (tf + tn)))
    print("\nframe%% is an UPPER bound on C (SDK cmacros emit the same prologue);\n"
          "no-frame is a LOWER bound on assembler. See the module docstring.")


if __name__ == "__main__":
    main()
