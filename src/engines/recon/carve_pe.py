#!/usr/bin/env python3
r"""carve_pe.py -- fast per-function disassembly carve of a PE module, for building a
cross-version diff twin.

The full byte-exact path (recon_engine/conduct.py) assembles every function through MASM to
prove byte-exactness -- correct, but far more than the diff corpus needs. The corpus fingerprints
STRUCTURE (CFG topology, normalized opcodes, call vector), which a faithful disassembly already
carries; the byte-exact db-fallback only changes emitted bytes, not that structure. So to stand up
a twin (e.g. win98 gdi32 to match against win95 gdi32) this carves each function straight from the
capstone/export-driven disassembly -- no assembler, seconds not minutes.

Offsets are section-relative (the same space enrich._pe_names joins export RVAs into), and the
filenames match conduct's (`sec<i>_<off>.asm`), so the output ingests identically.

    wt --target win98 carve_pe gdi32.dll                 # -> that target's asm tree
    wt --target win98 carve_pe gdi32.dll <out_dir>       # -> an explicit dir
"""
import os
import sys

from src.core import cores, layout
from src.formats.pe import open_pe
from src.formats.pe_seg import PeSeg
from src.formats.pe_trace import PeCarve


def carve(module, outdir=None):
    """Emit one .asm per carved function of a PE module. Returns (count, outdir)."""
    target = cores.get("target")
    asmrecon32 = cores.get("asmrecon32")
    path = target.resolve(module)
    if outdir is None:
        outdir = os.path.join(layout.asm_dir(), os.path.basename(module).lower())
    os.makedirs(outdir, exist_ok=True)
    sweep = PeCarve(path)                                  # one export/capstone sweep, reused
    written = 0
    for sec_i, (_va, _code, name) in enumerate(open_pe(path).code_sections(), start=1):
        sg = PeSeg(module, section=name, carve=sweep)
        for start, f in sorted(sg.funcs.items()):
            s, e = f["start"], f["end"]
            if e <= s:
                continue
            asm = asmrecon32.wrap32(asmrecon32.emit_function32(sg, s, e, set()))
            open(os.path.join(outdir, "sec%d_%05x.asm" % (sec_i, s)),
                 "w", newline="\r\n").write(asm)
            written += 1
    return written, outdir


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("-")]
    if not args:
        sys.exit("usage: carve_pe.py MODULE [OUT_DIR]")
    module = args[0]
    outdir = args[1] if len(args) > 1 else None
    n, where = carve(module, outdir)
    print("carved %s: %d functions -> %s" % (module, n, where))


if __name__ == "__main__":
    main()
