#!/usr/bin/env python3
"""
dropshims.py — delete the placeholder externs that Microsoft's header supersedes.

Before this project had the 1.01 header, files declared what they called:

    extern long MessageBox();  extern long InitApp();  extern long SetTimer();

Those were honest at the time — a non-committal shape, asserting no signature.
Now `build/include/windows.h` supplies Microsoft's real prototypes, and every
such line CONTRADICTS one: `long MessageBox()` against
`int FAR PASCAL MessageBox(HWND, LPSTR, LPSTR, WORD)`. That is the single
largest class of diagnostic in the tree.

ONLY the ones the header actually declares are removed. Several of these names
are NOT in WINDOWS.H — `InitApp`, `InitTask`, `WaitEvent`, `GetInstanceData`
are KERNEL entry points the SDK header does not expose — and deleting those
would turn a wrong declaration into no declaration, trading 121 conflicts for
as many undeclared identifiers. Those stay, and are reported so it is visible
which names the SDK never published.

Usage:
    python dropshims.py [--apply]
"""
import re, glob, sys, collections
from engine.foundation import layout

HDR = "refs/sdk101/d06/WINDOWS.H"

# A declaration line of the shape `extern <type> <name>();` — empty parens, so
# it asserts no signature. Several may share one physical line.
DECL = re.compile(r"extern\s+[A-Za-z_][\w \t\*]*?\b(\w+)\s*\(\s*\)\s*;")


def header_names():
    src = open(HDR, encoding="cp437", errors="replace").read()
    src = re.sub(r"/\*.*?\*/", " ", src, flags=re.S)
    return set(re.findall(r"\b(\w+)\s*\([^;{)]*\)\s*;", src))


def main():
    known = header_names()
    print("WINDOWS.H declares %d names" % len(known))
    kept = collections.Counter()
    removed_total = 0
    for f in sorted(glob.glob(layout.recon_dir() + "/*.c") + glob.glob(layout.recon_sub("system") + "/*.c")):
        src = open(f, encoding="utf-8").read()
        hits = [(m.start(), m.end(), m.group(1)) for m in DECL.finditer(src)]
        if not hits:
            continue
        drop = [h for h in hits if h[2] in known]
        keep = [h for h in hits if h[2] not in known]
        for _, _, n in keep:
            kept[n] += 1
        if not drop:
            continue
        removed_total += len(drop)
        print("%-40s remove %3d, keep %2d" % (f, len(drop), len(keep)))
        if "--apply" in sys.argv:
            out, last = [], 0
            for s, e, n in sorted(drop):
                out.append(src[last:s]); last = e
            out.append(src[last:])
            new = "".join(out)
            # tidy lines that are now only whitespace between decls
            new = re.sub(r"[ \t]+\n", "\n", new)
            open(f, "w", encoding="utf-8").write(new)
            back = open(f, encoding="utf-8").read()
            for _, _, n in drop:
                assert not re.search(r"extern\s+[\w \t\*]*\b%s\s*\(\s*\)\s*;" % re.escape(n), back), \
                    "still present: %s in %s" % (n, f)

    print("\n%d placeholder externs superseded by the real header" % removed_total)
    if kept:
        print("\nKEPT — these names are NOT in the SDK's public header, so the")
        print("placeholder is the only declaration there is:")
        for n, c in kept.most_common(30):
            print("   %-24s x%d" % (n, c))
    if "--apply" not in sys.argv:
        print("\n(report only; pass --apply)")


if __name__ == "__main__":
    main()
