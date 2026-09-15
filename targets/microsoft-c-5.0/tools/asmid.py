#!/usr/bin/env python3
"""asmid.py -- identify hand-written MS C 5.0 asm runtime helpers by fixed byte
patterns. These lack the C prologue (55 8B EC) so carve/idlib's whole-body matcher
misses them, yet they're the most-called toolchain routines. Naming them completes the
origin triage (they are [ID], never reconstruction targets).

    python asmid.py <unpacked.exe>       # report helper addresses
    python asmid.py --all                # scan every unpacked binary
"""
import sys, os, glob, struct, re

# (name, regex over raw bytes). Patterns taken from the STARTUP/*.ASM sources shipped on
# the distribution and confirmed in the images.
HELPERS = [
    # __chkstk: pop cx; mov bx,sp; sub bx,ax; jb; cmp bx,[heaptop]; jb; mov sp,bx; jmp cx
    ("__chkstk",  rb"\x59\x8b\xdc\x2b\xd8\x72.\x3b\x1e..\x72.\x8b\xe3\xff\xe1"),
    # __aNchkstk / far variant: retf-based
    ("__chkstk_far", rb"\x5a\x59\x8b\xc4\x2b\xc1\x72.\x3b\x06..\x72"),
    # __aFmul/@aulmul style long-multiply helpers are model-tagged; skip for now.
]

def load(path):
    d = open(path, "rb").read()
    hdr = struct.unpack_from("<H", d, 8)[0]*16
    return d[hdr:]

def scan(path):
    img = load(path)
    hits = []
    for name, pat in HELPERS:
        for m in re.finditer(pat, img, re.DOTALL):
            hits.append((m.start(), name))
    return sorted(hits)

if __name__ == "__main__":
    if "--all" in sys.argv:
        root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        for p in sorted(glob.glob(os.path.join(root, "unpacked", "*.EXE"))):
            hits = scan(p)
            names = ", ".join(f"{n}@{o:#x}" for o, n in hits) or "-"
            print(f"{os.path.basename(p):14} {names}")
    else:
        for o, n in scan(sys.argv[1]):
            print(f"{n} @ {o:#06x}")
