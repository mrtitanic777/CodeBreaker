#!/usr/bin/env python3
"""wt --target win95 disasm <module> [-o out.asm]

CodeBreaker's own x86 disassembler engine (native/decompiler/win95dec) -- no
Ghidra, no objdump. Loads the NE module and emits a per-segment instruction
listing (addr: bytes  mnem ops) with call/branch targets resolved to names.
"""
import argparse
import sys

import target
from src.targets.win95 import nativedec

_HINT = ("win95dec not built: "
         "cmake --build native/decompiler/build --config Release --target win95dec")


def main(argv):
    ap = argparse.ArgumentParser(prog="disasm", description=__doc__.splitlines()[0])
    ap.add_argument("module", help="module name or path (e.g. LZEXPAND, gdi.exe)")
    ap.add_argument("-o", "--out", help="write the listing to this file instead of stdout")
    a = ap.parse_args(argv)
    if not nativedec.available():
        sys.exit(_HINT)
    path = target.resolve(a.module)
    listing = nativedec.disassemble_module(path)
    if a.out:
        with open(a.out, "w", encoding="utf-8", newline="\n") as f:
            f.write(listing)
        print("wrote %s (%d lines)" % (a.out, listing.count("\n")))
    else:
        sys.stdout.write(listing)


if __name__ == "__main__":
    main(sys.argv[1:])
