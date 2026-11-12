"""gen_listings.py -- makes golden/lib220_*.lst and golden/listings.json for asm16_test.cpp.

    python native/cb/port/testdata/asm16/gen_listings.py MASM.EXE        (from the repository root)

Runs the period assembler (MASM 5.10) under CodeBreaker's emulator, as codebreaker.port.asm16.listing_offsets does, over the
three small modules of the old C library, and keeps each listing (the .LST) and what the Python original read from it
(labels, and the lines with their offsets, lengths, text and bytes).
"""
import json
import os
import shutil
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.abspath(os.path.join(HERE, "..", "..", "..", "..", "..")))

from codebreaker.port import asm16  # noqa: E402

kept = {}
real_rmtree = shutil.rmtree


def grab(path, ignore_errors=False):          # (the temporary folder goes; its listing is kept first)
    p = os.path.join(path, "T.LST")
    if os.path.exists(p):
        kept["lst"] = open(p, "rb").read()
    real_rmtree(path, ignore_errors=ignore_errors)


def main():
    masm = sys.argv[1]
    shutil.rmtree = grab
    out = {}
    for n in ("strlen", "string", "movbytes"):
        r = asm16.listing_offsets(os.path.join(HERE, "lib220_%s.asm" % n), masm)
        open(os.path.join(HERE, "golden", "lib220_%s.lst" % n), "wb").write(kept["lst"])
        out[n] = {"labels": [[k, list(v)] for k, v in r.items() if k != "@lines"],
                  "lines": [list(x[:4]) + [x[4].hex()] for x in r["@lines"]]}
    json.dump(out, open(os.path.join(HERE, "golden", "listings.json"), "w", newline="\n"), indent=1)


if __name__ == "__main__":
    main()
