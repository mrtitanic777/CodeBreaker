#!/usr/bin/env python3
r"""show.py -- `wt behavior show`: pretty-print a recorded trace from the corpus.

    wt behavior show <module> [func] [--path N]

    <module>     the traced binary, e.g. gdi32.dll
    [func]       VA or section offset (hex/decimal); omitted -> the module's lowest-VA trace.
    --path N     show the first N executed instructions with disassembly (default 24).

Reads only stored evidence (corpus.py). Run `wt behavior trace` first to produce it.
"""
import sys

from engine.behavior_engine import corpus


def _opt(args, name, default=None):
    return args[args.index(name) + 1] if name in args and args.index(name) + 1 < len(args) else default


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    module = argv[0]
    func = None
    for a in argv[1:]:
        if not a.startswith("-"):
            func = a; break
    npath = int(_opt(argv, "--path", 24))

    rec = corpus.load_by_selector(module, func)
    if rec is None:
        print("no stored trace for %s %s -- run `wt behavior trace %s %s` first"
              % (module, func or "", module, func or "")); sys.exit(1)

    print("=== %s  func VA 0x%x  (off 0x%x, section %s) ===" %
          (rec["module"], rec["func_va"], rec["func_off"], rec["section"]))
    print("binary sha256 : %s" % rec.get("target_sha256", "?"))
    print("recorded      : %s" % rec.get("recorded_at", "?"))
    print("input args    : %s" % (", ".join("0x%x" % a for a in rec["input"]["args"]) or "(none)"))
    print("exit          : %s" % rec["exit"])
    print("ret (eax)     : %s" % ("0x%x" % rec["ret"] if rec["ret"] is not None else "(none)"))
    print("instructions  : %d%s" % (rec["insn_count"], "  [TRUNCATED]" if rec["truncated"] else ""))
    print("mem reads=%d  writes=%d  lazy-maps=%d  calls=%d" %
          (len(rec["mem_reads"]), len(rec["mem_writes"]), len(rec["lazy_maps"]), len(rec["calls"])))

    if rec["calls"]:
        print("\n-- calls --")
        for c in rec["calls"][:40]:
            tgt = ("0x%x" % c["target"]) if c["target"] is not None else "(indirect)"
            print("  at 0x%-8x -> %s%s" % (c["pc"], tgt, "  [into self]" if c["into_func"] else ""))

    if rec["lazy_maps"]:
        print("\n-- lazy-mapped faults (pointer targets reached during the run) --")
        for m in rec["lazy_maps"][:40]:
            print("  pc 0x%-8x  %-5s  addr 0x%-8x  page 0x%x" %
                  (m["pc"], m["access"], m["addr"], m["page"]))

    if rec["mem_writes"]:
        print("\n-- memory writes --")
        for w in rec["mem_writes"][:40]:
            v = ("0x%x" % w["value"]) if w["value"] is not None else "?"
            print("  pc 0x%-8x  [0x%-8x] <- %s  (%d bytes)" % (w["pc"], w["addr"], v, w["size"]))

    if npath and rec["path"]:
        print("\n-- path (first %d instructions) --" % npath)
        mnem = rec.get("mnemonics", {})
        for pc in rec["path"][:npath]:
            print("  0x%-8x  %s" % (pc, mnem.get("0x%x" % pc, "")))


if __name__ == "__main__":
    main()
