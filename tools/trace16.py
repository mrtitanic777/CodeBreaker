#!/usr/bin/env python3
"""trace16.py -- run one 16-bit function, original and rebuilt, and show both.

    python tools/trace16.py <module> <function> [--input a,b,c...] [--last N] [--first N]

For each side: how the run ended, the result, how far the stack moved, the Windows functions
called, and the instructions it ran (the last N by default -- where it went wrong is usually at
the end; --first N for the start). Uses the build left by the last `measure` of the module.
"""
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return 2
    import capstone
    from codebreaker.base import corpus
    from codebreaker.build import watcom
    from codebreaker.decompile import image16
    from codebreaker.emulate import ne_probe
    from codebreaker.formats.ne import NEFile
    from codebreaker.verify import reconstruct16

    def opt(flag, default):
        return argv[argv.index(flag) + 1] if flag in argv else default
    path = corpus.retail(argv[0]) or argv[0]
    fn = argv[1]
    work = watcom._scratch("reconstruct16", os.path.basename(path).lower())
    desc = open(os.path.join(work, "description.txt"), encoding="utf-8").read()
    m = re.search(r"(?m)^F\t([0-9a-f]+)\t%s\t(-?\d+)\t(\S+)" % re.escape(fn), desc)
    if not m:
        raise SystemExit("%s: not a function of %s (see %s)" % (fn, os.path.basename(path), work))
    seg, off = image16.where(int(m.group(1), 16))
    near = "near" in m.group(3)
    mp = open(os.path.join(work, "m.map"), errors="replace").read()
    mm = re.search(r"(?m)^([0-9a-f]{4}):([0-9a-f]{4,8})[+*]?\s+_?%s_?(?:@\d+)?\s*$" % re.escape(fn), mp)
    args = [int(x, 0) for x in opt("--input", ",".join(["0"] * reconstruct16.ARG_WORDS)).split(",")]
    nef = NEFile(path)
    auto = nef.h.get("autodata_seg")
    image = b""
    if auto:
        s = nef.segments()[auto - 1]
        image = bytes(nef.data[s["file_off"]:s["file_off"] + s["length"]]) if s["file_off"] else b""
    print("%s %s: %s, removes %s bytes, input %s" % (os.path.basename(path), fn, m.group(3), m.group(2), args))
    sides = [("original", path, seg, off, None)]
    if mm:
        sides.append(("rebuilt", os.path.join(work, "m.dll"), int(mm.group(1), 16), int(mm.group(2), 16), image))
    else:
        print("  (not in the rebuilt module: it did not compile)")
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    last, first = int(opt("--last", "18")), int(opt("--first", "0"))
    for label, p, s_, o_, img in sides:
        r = ne_probe.probe(p, seg=s_, offset=o_, args=args, near=near, data_image=img)
        print("== %s %d:%04x  ends: %s  AX=%04x DX=%04x  stack moved %s  instructions %d" % (
            label, s_, o_, r["exit"], r["ret"], (r["ret32"] >> 16) & 0xFFFF, r.get("sp_delta"), r["insn_count"]))
        if r.get("fault"):
            print("   fault at %s" % r["fault"])
        calls = [h.get("api") or "?" for h in r["imports_hit"]]
        if calls:
            print("   calls: " + ", ".join(calls[:16]))
        n = NEFile(p)
        bases = {b: i for i, b in (r["layout"]["seg_bases"] or {}).items()}
        codes = {x["idx"]: n.data[x["file_off"]:x["file_off"] + x["length"]] for x in n.segments()}
        path_ = r["path"][:first] if first else r["path"][-last:]
        for a in path_:
            b = a & ~0xFFFF
            if b in bases and (a - b) < len(codes.get(bases[b], b"")):
                o = a - b
                ins = next(md.disasm(codes[bases[b]][o:o + 10], o), None)
                print("   %d:%04x  %s %s" % (bases[b], o, ins.mnemonic if ins else "?", ins.op_str if ins else ""))
            else:
                print("   %08x  (outside the module)" % a)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
