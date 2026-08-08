#!/usr/bin/env python3
"""
declglobals.py — declare the `ds_*` module globals the reconstruction refers to.

Across the tree, several hundred module globals are written as `ds_0x96e`,
`ds_word_0x264`, `ds_byte_0xee` — named for the DS offset they were recovered
at — and simply never declared. The convention is already in use elsewhere in
these files (`static WORD gFoo; /* ds:0x00a4 */`); these are the ones that
never got a declaration to go with the reference.

The name carries everything needed: `ds_byte_` -> BYTE, `ds_word_` -> WORD,
plain `ds_` -> WORD (the default width in this binary's data segment), and the
hex tail is the offset, which goes in the comment where every other global in
the project records it.

This declares storage. It does NOT claim to know what any of them MEAN — a
global named for its address is exactly as informative as its address, which is
the point of the naming convention. `globals.py` is the tool that identifies
one, by finding who writes it.

Inserts after the last #include of each file, and asserts the file still holds
every declaration afterwards.

Usage:
    python declglobals.py            # report
    python declglobals.py --apply
"""
import re, subprocess, glob, sys, collections
from engine.foundation import layout

INC = ["-Ibuild/include", "-I" + layout.recon_dir(), "-I" + layout.recon_sub("system")]
CC = ["gcc", "-fsyntax-only", "-std=gnu89", "-w"]
DS = re.compile(r"^ds_(?:(word|byte|dword)_)?0x([0-9a-fA-F]+)$")
MARK = "/* ---- module globals referenced above, declared by declglobals.py"


def undeclared(path):
    p = subprocess.run(CC + INC + [path], capture_output=True, text=True)
    names = set()
    for line in p.stderr.splitlines():
        m = re.search(r"error: '([^']+)' undeclared", line)
        if m:
            names.add(m.group(1))
    return names


def main():
    total = 0
    for f in sorted(glob.glob(layout.recon_dir() + "/*.c") + glob.glob(layout.recon_sub("system") + "/*.c")):
        src = open(f, encoding="utf-8").read()
        if MARK in src:
            continue
        names = {n for n in undeclared(f) if DS.match(n)}
        if not names:
            continue
        rows = []
        for n in sorted(names, key=lambda x: int(DS.match(x).group(2), 16)):
            m = DS.match(n)
            kind = (m.group(1) or "word").lower()
            typ = {"byte": "BYTE", "word": "WORD", "dword": "DWORD"}[kind]
            off = int(m.group(2), 16)
            rows.append("static %-5s %-22s /* ds:0x%04x */" % (typ, n + ";", off))
        total += len(rows)
        print("%-42s %3d globals" % (f, len(rows)))
        if "--apply" not in sys.argv:
            continue

        block = ["", MARK,
                 " * Named for the DS offset they were recovered at, which is all that is",
                 " * known about them: a global named for its address is exactly as",
                 " * informative as its address. Use globals.py to identify one — it",
                 " * reports who WRITES it, which is what says what it is. */"]
        block += rows
        block.append("")
        # insert after the last #include
        last = None
        for m in re.finditer(r"(?m)^\s*#\s*include[^\n]*\n", src):
            last = m
        if last is None:
            print("   (no #include — skipped)"); continue
        new = src[:last.end()] + "\n".join(block) + "\n" + src[last.end():]
        open(f, "w", encoding="utf-8").write(new)
        back = open(f, encoding="utf-8").read()
        assert MARK in back
        for n in names:
            assert re.search(r"\b%s;" % re.escape(n), back), "missing %s in %s" % (n, f)
    print("\n%d globals %s" % (total, "declared" if "--apply" in sys.argv else "would be declared"))
    if "--apply" not in sys.argv:
        print("(report only; pass --apply)")


if __name__ == "__main__":
    main()
