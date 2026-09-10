#!/usr/bin/env python3
"""
fixcomments.py — find and repair block comments that close themselves early.

C block comments do not nest. A reconstruction header that reads

    /* ...
     * /* inferred */ note about the likely SDK function
     * ... */

ends at the FIRST `*/`, and every line after it is parsed as code. The file
still looks perfectly reasonable to a human reader, which is why this survived
until something actually compiled it.

The repair rewrites the INNER delimiters only — `/*` -> `[` and `*/` -> `]`
inside a comment — so the prose is unchanged in meaning and the outer comment
regains its true extent.

Reports before it edits, and re-reads and asserts after. Never uses a blind
replace: the scan is a state machine over the real comment structure, so it
cannot touch a `/*` that is inside a string literal or already code.

Usage:
    python fixcomments.py            # report only
    python fixcomments.py --apply
"""
import glob, sys, re, os
from codebreaker.foundation import layout


def scan(src):
    """-> list of (index, kind) for delimiters that are INSIDE a block comment."""
    hits, i, n = [], 0, len(src)
    in_block = in_line = in_str = in_chr = False
    depth_start = None
    while i < n - 1:
        c, d = src[i], src[i + 1]
        if in_line:
            if c == "\n":
                in_line = False
            i += 1; continue
        if in_str:
            if c == "\\":
                i += 2; continue
            if c == '"':
                in_str = False
            i += 1; continue
        if in_chr:
            if c == "\\":
                i += 2; continue
            if c == "'":
                in_chr = False
            i += 1; continue
        if in_block:
            if c == "/" and d == "*":
                hits.append((i, "open"))     # a nested opener: harmless but noted
                i += 2; continue
            if c == "*" and d == "/":
                in_block = False
                i += 2; continue
            i += 1; continue
        # not in any comment or string
        if c == "/" and d == "*":
            in_block = True; depth_start = i; i += 2; continue
        if c == "/" and d == "/":
            in_line = True; i += 2; continue
        if c == '"':
            in_str = True; i += 1; continue
        if c == "'":
            in_chr = True; i += 1; continue
        i += 1
    return hits


def repair(src):
    """Rewrite inner `/* ... */` pairs as `[ ... ]` within a block comment."""
    out, i, n = [], 0, len(src)
    in_block = in_line = in_str = in_chr = False
    changed = 0
    while i < n:
        c = src[i]
        d = src[i + 1] if i + 1 < n else ""
        if in_line:
            out.append(c)
            if c == "\n":
                in_line = False
            i += 1; continue
        if in_str or in_chr:
            out.append(c)
            if c == "\\" and i + 1 < n:
                out.append(src[i + 1]); i += 2; continue
            if in_str and c == '"':
                in_str = False
            elif in_chr and c == "'":
                in_chr = False
            i += 1; continue
        if in_block:
            if c == "/" and d == "*":
                # A nested opener inside a block comment. There are TWO shapes
                # and they need OPPOSITE treatment — an earlier version of this
                # function treated them alike and turned
                #     static char g_szDisplay[/*~14*/];
                # into
                #     static char g_szDisplay[[~14]];
                # which is not C. Brackets are never used here now.
                #
                #   A)  /* ds:0x602 ...  /* inferred */
                #       The `*/` IS the intended closer. Neutralise the inner
                #       opener only, and the comment keeps its extent.
                #
                #   B)  /* header ...
                #        * /* inferred */ more prose
                #        * ... */
                #       The `*/` closes the header EARLY. A later `*/` is the
                #       real closer, so this one is redundant and both must go.
                #
                # Test: after the candidate closer, is there another `*/` with
                # no `/*` between? If so this closer is redundant (case B).
                j = src.find("*/", i + 2)
                redundant = False
                if j != -1:
                    nxt_close = src.find("*/", j + 2)
                    nxt_open = src.find("/*", j + 2)
                    redundant = nxt_close != -1 and (nxt_open == -1 or nxt_close < nxt_open)
                if redundant:
                    out.append("(*" + src[i + 2:j] + "*)")
                    changed += 1
                    i = j + 2
                    continue
                out.append("(*"); changed += 1; i += 2; continue
            if c == "*" and d == "/":
                out.append("*/"); in_block = False; i += 2; continue
            out.append(c); i += 1; continue
        if c == "/" and d == "*":
            out.append("/*"); in_block = True; i += 2; continue
        if c == "/" and d == "/":
            out.append("//"); in_line = True; i += 2; continue
        if c == '"':
            out.append(c); in_str = True; i += 1; continue
        if c == "'":
            out.append(c); in_chr = True; i += 1; continue
        out.append(c); i += 1
    return "".join(out), changed


def main():
    files = sorted(glob.glob(os.path.join(layout.recon_dir(), "*.c")) + glob.glob(os.path.join(layout.recon_sub("system"), "*.c"))
                   + glob.glob(os.path.join(layout.recon_dir(), "*.h")))
    total = 0
    for f in files:
        src = open(f, encoding="utf-8").read()
        hits = scan(src)
        if not hits:
            continue
        total += len(hits)
        print("%-44s %3d nested comment opener(s)" % (f, len(hits)))
        if "--apply" in sys.argv:
            new, changed = repair(src)
            assert len(new) >= len(src) - 4 * changed, "repair shrank the file oddly"
            open(f, "w", encoding="utf-8").write(new)
            back = open(f, encoding="utf-8").read()
            assert not scan(back), "still nested after repair: " + f
    print("\n%d nested openers across %d files" % (total, len(files)))
    if "--apply" not in sys.argv:
        print("(report only; pass --apply to repair)")


if __name__ == "__main__":
    main()
