#!/usr/bin/env python3
"""
fixrawcalls.py — give the raw-address calls a name, per the project's own rule.

Some files call an internal routine by writing its address as a literal:

    (void)0x368f();          /* internal seg3:0x368f (save state, inferred) */

That is not C — you cannot call an integer — and it is also inconsistent with
the convention CONTEXT.md already sets for a call whose target has no confirmed
name: a deliberately non-committal identifier, so no unverified name is
asserted. `Call_MODULE_ord()` is that convention for imports; this is the same
situation for an intra-module call.

Rewrites `0xNNNN(` to `Sub_0xNNNN(` and declares each one. The name says only
what is known — that a routine lives at that offset — which is the whole point.
The address comment beside each call is left untouched, so the ledger's
citations are unaffected.

Usage:
    python fixrawcalls.py [--apply]
"""
import re, glob, sys, os
from codebreaker.core import layout

RAW = re.compile(r"(?<![\w.])0x([0-9a-fA-F]{2,4})\s*\(")
MARK = "/* ---- intra-module calls with no confirmed name (fixrawcalls.py)"


def main():
    total = 0
    for f in sorted(glob.glob(os.path.join(layout.recon_dir(), "*.c")) + glob.glob(os.path.join(layout.recon_sub("system"), "*.c"))):
        src = open(f, encoding="utf-8").read()
        if MARK in src:
            continue
        # only outside comments: strip comments for detection, then rewrite the
        # real text using the same positions
        stripped = re.sub(r"/\*.*?\*/", lambda m: " " * len(m.group(0)), src, flags=re.S)
        hits = [(m.start(), m.group(1)) for m in RAW.finditer(stripped)]
        if not hits:
            continue
        names = sorted({h[1].lower() for h in hits}, key=lambda x: int(x, 16))
        total += len(hits)
        print("%-42s %3d call sites, %2d distinct addresses" % (f, len(hits), len(names)))
        if "--apply" not in sys.argv:
            continue
        out, last = [], 0
        for pos, hexs in hits:
            m = RAW.match(stripped, pos)
            out.append(src[last:pos])
            out.append("Sub_0x%s(" % hexs.lower())
            last = m.end()
        out.append(src[last:])
        new = "".join(out)
        decl = ["", MARK,
                " * A routine is known to live at each of these offsets and nothing more,"
                , " * so the name says exactly that — the same policy as Call_MODULE_ord",
                " * for an unresolved import. See CONTEXT.md. */"]
        decl += ["extern long Sub_0x%s();" % n for n in names]
        decl.append("")
        last_inc = None
        for m in re.finditer(r"(?m)^\s*#\s*include[^\n]*\n", new):
            last_inc = m
        if last_inc is None:
            print("   (no #include — skipped)"); continue
        new = new[:last_inc.end()] + "\n".join(decl) + "\n" + new[last_inc.end():]
        open(f, "w", encoding="utf-8").write(new)
        back = open(f, encoding="utf-8").read()
        assert MARK in back
        for n in names:
            assert ("Sub_0x%s(" % n) in back, "missing Sub_0x%s in %s" % (n, f)
    print("\n%d raw-address call sites %s"
          % (total, "renamed" if "--apply" in sys.argv else "found"))
    if "--apply" not in sys.argv:
        print("(report only; pass --apply)")


if __name__ == "__main__":
    main()
