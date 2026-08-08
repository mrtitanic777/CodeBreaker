#!/usr/bin/env python3
"""
dropdecls.py — remove our own prototypes for APIs the 1.01 header declares.

`dropshims.py` removed the signature-free `extern long Foo();` placeholders.
This removes the next layer: single-line `extern` declarations that DO give a
signature, but a different one from Microsoft's —

    ours:      extern void FAR *GlobalLock(HANDLE);
    1.01 SDK:  LPSTR FAR PASCAL GlobalLock(HANDLE);

Both describe a far pointer. The difference is that **`void *` did not exist in
1985** — it arrived with ANSI C in 1989 — which is why every "generic far
pointer" in this SDK is an `LPSTR`. Writing `void FAR *` is an anachronism of
the same kind as `HGLOBAL`, and the header is authoritative for what these
functions are.

Only removes a declaration when the header declares that name. A DEFINITION (a
declaration with a body) is never touched: KERNEL's own implementation of
GlobalLock lives in this tree and has to be corrected, not deleted.

Usage:
    python dropdecls.py [--apply]
"""
import re, glob, sys
from engine.recon_engine import argcheck
from engine.foundation import layout

DECL = re.compile(r"(?m)^[ \t]*extern[^;{\n]*?\b(\w+)\s*\([^;{)]*\)\s*;[ \t]*(?:/\*[^\n]*)?\n")


def main():
    known = set(argcheck.prototypes())
    total = 0
    for f in sorted(glob.glob(layout.recon_dir() + "/*.c") + glob.glob(layout.recon_sub("system") + "/*.c")):
        src = open(f, encoding="utf-8").read()
        drop = [m for m in DECL.finditer(src) if m.group(1) in known]
        if not drop:
            continue
        total += len(drop)
        print("%-42s %2d  (%s)" % (f, len(drop),
                                   ", ".join(sorted({m.group(1) for m in drop}))[:70]))
        if "--apply" not in sys.argv:
            continue
        out, last = [], 0
        for m in drop:
            out.append(src[last:m.start()]); last = m.end()
        out.append(src[last:])
        open(f, "w", encoding="utf-8").write("".join(out))
        back = open(f, encoding="utf-8").read()
        for m in drop:
            assert m.group(0) not in back, "still present in %s" % f
    print("\n%d redundant prototypes %s"
          % (total, "removed" if "--apply" in sys.argv else "found"))
    if "--apply" not in sys.argv:
        print("(report only; pass --apply)")


if __name__ == "__main__":
    main()
