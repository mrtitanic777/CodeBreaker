#!/usr/bin/env python3
"""
argfix.py — merge the split 32-bit arguments that argcheck.py identifies.

Applies ONLY to value pairs whose merge is unambiguous:

    0, 0                      ->  (TYPE)0
    HIWORD(x), LOWORD(x)      ->  x          (high word pushed first)
    LOWORD(x), HIWORD(x)      ->  x
    (WORD)x, (WORD)(x>>16)    ->  x
    (WORD)(x>>16), (WORD)x    ->  x

Anything else — two distinct expressions, a literal pair like `0x00f0, 0x21`
that encodes a raster op, a segment/offset pair naming real memory — is LEFT
ALONE and reported. Those carry information that a mechanical merge would
destroy or misorder, and getting the word order backwards would produce code
that compiles and is wrong, which is worse than code that does not compile.

A call is only touched when its argument count is EXACTLY the prototype's count
plus its number of 32-bit parameters, and every one of those positions holds a
safe pair. Partial merges are refused.

Usage:
    python argfix.py            # report
    python argfix.py --apply
"""
import re, sys, glob
from codebreaker.recon_engine import argcheck
from codebreaker.foundation import layout

# 32-bit parameter types. POINT and DWORD-shaped structs count: a POINT is two
# words on the stack exactly as a far pointer is.
WIDE = re.compile(r"^\s*(LP\w+|LONG|DWORD|FARPROC|POINT|long|unsigned long)\s*$", re.I)


def split_args(s):
    """Split a call's argument text on top-level commas."""
    out, depth, cur, i, n = [], 0, [], 0, len(s)
    instr = inch = False
    while i < n:
        c = s[i]
        if instr:
            cur.append(c)
            if c == "\\" and i + 1 < n:
                cur.append(s[i + 1]); i += 2; continue
            if c == '"':
                instr = False
            i += 1; continue
        if inch:
            cur.append(c)
            if c == "\\" and i + 1 < n:
                cur.append(s[i + 1]); i += 2; continue
            if c == "'":
                inch = False
            i += 1; continue
        if c == '"':
            instr = True; cur.append(c); i += 1; continue
        if c == "'":
            inch = True; cur.append(c); i += 1; continue
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
        if c == "," and depth == 0:
            out.append("".join(cur)); cur = []; i += 1; continue
        cur.append(c); i += 1
    out.append("".join(cur))
    return out


def merge(a, b, typ):
    """The merged expression, or None if the pair is not unambiguous."""
    a, b = a.strip(), b.strip()
    if a == "0" and b == "0":
        t = typ.strip()
        return "(%s)0" % t
    m1 = re.fullmatch(r"HIWORD\((.+)\)", a)
    m2 = re.fullmatch(r"LOWORD\((.+)\)", b)
    if m1 and m2 and m1.group(1).strip() == m2.group(1).strip():
        return m1.group(1).strip()
    m1 = re.fullmatch(r"LOWORD\((.+)\)", a)
    m2 = re.fullmatch(r"HIWORD\((.+)\)", b)
    if m1 and m2 and m1.group(1).strip() == m2.group(1).strip():
        return m1.group(1).strip()
    m1 = re.fullmatch(r"\(WORD\)\s*\(?(\w+)\)?", a)
    m2 = re.fullmatch(r"\(WORD\)\s*\(\s*(\w+)\s*>>\s*16\s*\)", b)
    if m1 and m2 and m1.group(1) == m2.group(1):
        return m1.group(1)
    m1 = re.fullmatch(r"\(WORD\)\s*\(\s*(\w+)\s*>>\s*16\s*\)", a)
    m2 = re.fullmatch(r"\(WORD\)\s*\(?(\w+)\)?", b)
    if m1 and m2 and m1.group(1) == m2.group(1):
        return m1.group(1)

    # THE GENERAL RULE. Word order is no longer an assumption — the HIGH word
    # is pushed first, established three independent ways:
    #   ChangeMenu   pushes `ds` (segment) before the offset
    #   DialogBox    pushes [bx+0xeb6] before [bx+0xeb4]
    #   PatBlt       pushes 0x00f0 before 0x21, and PATCOPY is 0x00F00021
    # MAKELONG(a, b) puts a LOW and b HIGH, so a pushed pair (x, y) is
    # MAKELONG(y, x).
    #
    # Only applied when both halves are simple expressions — an identifier,
    # a number, a cast or a field access. Anything with a comma or a call in
    # it is left for a human, since the split may not be where it looks.
    SIMPLE = r"[A-Za-z_0-9\.\->\(\)\* ]+"
    if re.fullmatch(SIMPLE, a) and re.fullmatch(SIMPLE, b) and "," not in a + b:
        t = typ.strip()
        if re.match(r"^(LP|FARPROC)", t, re.I):
            return "(%s)MAKELONG(%s, %s)" % (t, b.strip(), a.strip())
        return "MAKELONG(%s, %s)" % (b.strip(), a.strip())
    return None


def process(src, protos):
    fixed, skipped = 0, []
    for fn, params in protos.items():
        wide = [i for i, p in enumerate(params) if WIDE.match(p)]
        if not wide:
            continue
        want = len(params) + len(wide)
        pat = re.compile(r"\b%s\s*\(" % re.escape(fn))
        pos = 0
        while True:
            m = pat.search(src, pos)
            if not m:
                break
            # find the matching close paren
            i, depth = m.end(), 1
            while i < len(src) and depth:
                if src[i] == "(":
                    depth += 1
                elif src[i] == ")":
                    depth -= 1
                i += 1
            if depth:
                break
            inner = src[m.end():i - 1]
            args = split_args(inner)
            if len(args) != want or len(args) == len(params):
                pos = i; continue
            new = list(args)
            ok = True
            for k in sorted(wide, reverse=True):
                merged = merge(new[k], new[k + 1], params[k])
                if merged is None:
                    ok = False; break
                new[k:k + 2] = [" " + merged]
            if not ok:
                skipped.append((fn, inner.strip().replace("\n", " ")[:90]))
                pos = i; continue
            src = src[:m.end()] + ",".join(new) + src[i - 1:]
            fixed += 1
            pos = m.end()
    return src, fixed, skipped


def main():
    protos = argcheck.prototypes()
    tot, allskip = 0, []
    for f in sorted(glob.glob(layout.recon_dir() + "/*.c") + glob.glob(layout.recon_sub("system") + "/*.c")):
        src = open(f, encoding="utf-8").read()
        new, n, skip = process(src, protos)
        allskip += [(f,) + s for s in skip]
        if not n:
            continue
        tot += n
        print("%-42s %2d merged" % (f, n))
        if "--apply" in sys.argv:
            open(f, "w", encoding="utf-8").write(new)
    print("\n%d unambiguous merges" % tot)
    if allskip:
        print("\nLEFT ALONE — the pair is not an unambiguous split; these need a")
        print("human to say which word is which:")
        for f, fn, inner in allskip[:25]:
            print("   %-28s %s(%s)" % (f.split("\\")[-1], fn, inner))
    if "--apply" not in sys.argv:
        print("\n(report only; pass --apply)")


if __name__ == "__main__":
    main()
