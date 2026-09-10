#!/usr/bin/env python3
"""
sysdisasm.py — annotated per-function disassembly for the system modules.

The 13 apps each have `asm/<app>/segN_0xOFF.asm` (one annotated, call-resolved
disassembly per function) — the working material a human reads to name a function.
The 9 system modules never got the same view: they live bundled inside WIN100.BIN,
so the app conductor skips them, and their only asm is the whole-module byte-exact
masmlift output (`src/reconstructed/<mod>.asm`, `db`-heavy and unsplit).

This emits the missing per-function view for them, matching the app layout: opens
each module via `cfg.open_any`, carves its functions, and writes each one's
annotated disassembly (calls resolved to `MODULE.ordinal`, DOS `INT 21h` calls
flagged) to `asm/<mod>/segN_0xOFF.asm`. It is a *readable rendering of the same
byte-exact code* — pure disassembly, no naming — so the by-hand interpretation pass
has, for every function, one page to read.

    wt --target win1.0 sysdisasm            # all 9 modules
    wt --target win1.0 sysdisasm KERNEL     # one module
"""
import io
import os
import sys
import contextlib

import cfg
import trace
from codebreaker.core import layout

SYSMODS = ["KERNEL", "GDI", "USER", "SYSTEM", "KEYBOARD",
           "MOUSE", "DISPLAY", "SOUND", "COMM"]


def emit_module(mod):
    ne = cfg.open_any(mod)
    o = trace.load_ordinals("ORDINALS.md")
    outdir = layout.asm_module_out_dir(mod.lower())
    n = 0
    for i, s in enumerate(ne.segments(), 1):
        if s["kind"] != "CODE":
            continue
        try:
            sg = trace.Seg(ne, i, o)
        except Exception as e:
            print("  seg%d: skipped (%s)" % (i, e))
            continue
        for start in sorted(sg.funcs):
            buf = io.StringIO()
            with contextlib.redirect_stdout(buf):
                trace.print_function(sg, start)
            path = os.path.join(outdir, "seg%d_%04x.asm" % (i, start))
            open(path, "w", encoding="utf-8").write(buf.getvalue())
            n += 1
    return n, outdir


def main():
    mods = [a.upper() for a in sys.argv[1:] if not a.startswith("-")] or SYSMODS
    total = 0
    for m in mods:
        if m not in SYSMODS:
            print("skip %s (not a system module)" % m)
            continue
        try:
            n, outdir = emit_module(m)
        except Exception as e:
            print("%-9s ERROR: %s" % (m, e))
            continue
        total += n
        print("%-9s %4d functions -> %s/" % (m, n, os.path.relpath(outdir)))
    print("emitted %d annotated per-function .asm across %d module(s)" % (total, len(mods)))


if __name__ == "__main__":
    main()
