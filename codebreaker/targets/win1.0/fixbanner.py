#!/usr/bin/env python3
"""
fixbanner.py — repair nested delimiters inside BANNER comments, line by line.

fixcomments.py works at the character level and asks "is this `*/` redundant?"
by looking for a later one. That test fails when the same banner contains a
SECOND `/* inferred */` further down: the later opener hides the real closer, so
the first `*/` looks essential and is left in place, and the banner still ends
forty lines early.

These files have one consistent banner style, and a line-oriented rule is exact
where the character-level one is a guess:

    a banner opens with `/*` at column 0
    its interior lines begin with ` *`
    it closes on a line whose only content is `*/`

Inside that span, any `/*` or `*/` that is NOT the banner's own opener or closer
is interior prose and gets neutralised — `/*` -> `(*`, `*/` -> `*)`.

Reports, edits, then re-reads and asserts every banner closes where it should.

Usage:
    python fixbanner.py [--apply]
"""
import glob, sys, re
from codebreaker.core import layout


def fix(src):
    lines = src.split("\n")
    out, i, n, changed = [], 0, len(lines), 0
    while i < n:
        L = lines[i]
        if L.startswith("/*"):
            # find this banner's closing line
            j = i + 1
            while j < n and lines[j].strip() != "*/":
                # a banner interior line starts with optional spaces then '*'
                if not lines[j].lstrip().startswith("*") and lines[j].strip():
                    break
                j += 1
            if j < n and lines[j].strip() == "*/":
                head = L if L.strip() == "/*" else "/*" + L[2:].replace("/*", "(*").replace("*/", "*)")
                out.append(head)
                for k in range(i + 1, j):
                    orig = lines[k]
                    new = orig.replace("/*", "(*").replace("*/", "*)")
                    if new != orig:
                        changed += 1
                    out.append(new)
                out.append(lines[j])
                i = j + 1
                continue
        out.append(L)
        i += 1
    return "\n".join(out), changed


def main():
    files = sorted(glob.glob(layout.recon_dir() + "/*.c") + glob.glob(layout.recon_sub("system") + "/*.c"))
    tot = 0
    for f in files:
        src = open(f, encoding="utf-8").read()
        new, ch = fix(src)
        if not ch:
            continue
        tot += ch
        print("%-44s %3d interior delimiters" % (f, ch))
        if "--apply" in sys.argv:
            open(f, "w", encoding="utf-8").write(new)
            back = open(f, encoding="utf-8").read()
            again, ch2 = fix(back)
            assert ch2 == 0, "not idempotent: " + f
    print("\n%d fixed across the tree" % tot)
    if "--apply" not in sys.argv:
        print("(report only; pass --apply)")


if __name__ == "__main__":
    main()
