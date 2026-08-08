#!/usr/bin/env python3
"""
argcheck.py — explain every arity mismatch against the real 1.01 prototype.

THE ROOT CAUSE, ESTABLISHED FROM THE BINARY
    Far PASCAL pushes arguments left to right, one WORD at a time. A parameter
    that is 32 bits — LPSTR, LPRECT, LONG, DWORD, FARPROC, any LP* — occupies
    TWO pushed words. Reading the disassembly and writing one argument per push
    therefore produces a call with one argument too many for every 32-bit
    parameter it has.

    Proven at CLOCK seg1:0x0c63, where ChangeMenu's six pushes are
        hMenu, idItem, lpNewItem(segment), lpNewItem(offset), idNewItem, wChange
    — six words, five arguments.

    That single mistake accounts for every "too many arguments" diagnostic in
    the tree, and the counts confirm it: the excess always equals the number of
    32-bit parameters in the prototype.

WHAT THIS PRINTS
    For each mismatching call: Microsoft's prototype, which parameter positions
    are 32-bit (so how many words each takes), and the call as written. That
    turns each fix from an investigation into a lookup.

    "Too few" is a DIFFERENT problem and is labelled as such — those calls are
    genuinely under-specified and need the binary read, not a merge.

Usage:
    python argcheck.py
    python argcheck.py --file notepad.c
"""
import re, sys, subprocess, glob, os
from engine.foundation import layout

HDR = "refs/sdk101/d06/WINDOWS.H"
WIDE = re.compile(r"^\s*(LP\w+|LONG|DWORD|FARPROC|long|unsigned long)\s*$", re.I)


def prototypes():
    src = open(HDR, encoding="cp437", errors="replace").read()
    src = re.sub(r"/\*.*?\*/", " ", src, flags=re.S)
    out = {}
    for m in re.finditer(r"\b(\w+)\s*\(([^;{)]*)\)\s*;", src):
        name, args = m.group(1), m.group(2).strip()
        if not args or args.lower() == "void":
            out[name] = []
            continue
        out[name] = [a.strip() for a in args.split(",")]
    return out


def main():
    protos = prototypes()
    files = layout.written_c_files()
    if "--file" in sys.argv:
        f = sys.argv[sys.argv.index("--file") + 1]
        files = [x for x in files if x.endswith(f)]

    # include the recon root + every component subdir so cross-file headers resolve
    inc = ["-Ibuild/include", "-I" + layout.recon_dir(), "-I" + layout.recon_sub("system")]
    inc += ["-I" + d for d in sorted({os.path.dirname(f) for f in files})]
    n_many = n_few = 0
    for f in files:
        p = subprocess.run(["gcc", "-fsyntax-only", "-std=gnu89", "-w"] + inc + [f],
                           capture_output=True, text=True)
        src = open(f, encoding="utf-8", errors="replace").read().split("\n")
        for line in p.stderr.splitlines():
            m = re.search(r":(\d+):\d+: error: too (many|few) arguments to function '(\w+)'"
                          r"; expected (\d+), have (\d+)", line)
            if not m:
                continue
            ln, kind, fn, exp, have = int(m.group(1)), m.group(2), m.group(3), \
                int(m.group(4)), int(m.group(5))
            args = protos.get(fn, [])
            wide = [i for i, a in enumerate(args) if WIDE.match(a)]
            if kind == "many":
                n_many += 1
            else:
                n_few += 1
            print("\n%s:%d  %s  — too %s (expected %d, have %d)" % (f, ln, fn, kind, exp, have))
            print("   prototype : %s(%s)" % (fn, ", ".join(args)))
            if wide:
                print("   32-bit at : %s  -> %d words pushed for %d arguments"
                      % (", ".join("#%d %s" % (i + 1, args[i]) for i in wide),
                         len(args) + len(wide), len(args)))
                if kind == "many" and have - exp == len(wide):
                    print("   VERDICT   : push-count error. Merge the value pair at"
                          " each 32-bit position.")
            if kind == "few":
                print("   VERDICT   : under-specified — read the binary; not a merge.")
            print("   as written: %s" % src[ln - 1].strip()[:110])
    print("\n%d push-count candidates, %d under-specified" % (n_many, n_few))


if __name__ == "__main__":
    main()
