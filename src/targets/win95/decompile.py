#!/usr/bin/env python3
"""wt --target win95 decompile <module> [-o out.c]

CodeBreaker's own x86 decompiler engine (native/xexmagic/win95dec, the
XEXMagic-derived pipeline ported to Win95 x86) -- no Ghidra. Loads the NE
module, decodes + lifts every function to the SSA IL, and emits readable C.
"""
import argparse
import sys

import target
from src.targets.win95 import nativedec

_HINT = ("win95dec not built: "
         "cmake --build native/xexmagic/build --config Release --target win95dec")


def main(argv):
    ap = argparse.ArgumentParser(prog="decompile", description=__doc__.splitlines()[0])
    ap.add_argument("module", help="module name or path (e.g. LZEXPAND, gdi.exe)")
    ap.add_argument("-o", "--out", help="write C to this file instead of stdout")
    a = ap.parse_args(argv)
    if not nativedec.available():
        sys.exit(_HINT)
    path = target.resolve(a.module)
    c = nativedec.decompile_module(path)
    if a.out:
        with open(a.out, "w", encoding="utf-8", newline="\n") as f:
            f.write(c)
        print("wrote %s (%d lines)" % (a.out, c.count("\n")))
    else:
        sys.stdout.write(c)


if __name__ == "__main__":
    main(sys.argv[1:])
