#!/usr/bin/env python3
r"""CLI for the toolchain backbone:  python -m engine.backbone <cmd>

  list                      every registered adapter and whether it can run here
  detect <role> [k=v ...]   the adapter detect() picks for the given hints
  compile <file.c> [flags]  compile C via the default (or $BB_COMPILER) compiler
  disasm  <file.bin> [bits] decode bytes via the capstone disassembler role
"""
import sys, os
from engine.backbone import BACKBONE as bb


def main(argv):
    if not argv or argv[0] in ("-h", "--help", "help"):
        print(__doc__); return 0
    cmd, rest = argv[0], argv[1:]
    if cmd == "list":
        for d in bb.catalog():
            print("%-13s %-18s %s  %s" % (
                d["kind"], d["name"], "OK " if d["available"] else "GAP", d["version"]))
        errs = getattr(bb, "_errors", [])
        for mod, e in errs:
            print("!! adapter %s failed: %s" % (mod, e))
        return 0
    if cmd == "detect":
        role = rest[0]; hints = {}
        for kv in rest[1:]:
            k, _, v = kv.partition("=")
            hints[k] = int(v) if v.isdigit() else v
        a = bb.detect(role, **hints)
        print(a.name if a else "(no available adapter)"); return 0
    if cmd == "compile":
        src = open(rest[0]).read(); flags = " ".join(rest[1:]) or None
        c = bb.compiler(os.environ.get("BB_COMPILER"))
        r = c.compile(src, flags=flags)
        print("%s via %s: %s" % ("OK" if r.ok else "FAIL", c.name, r.stderr or (r.text.hex())))
        return 0 if r.ok else 1
    if cmd == "disasm":
        data = open(rest[0], "rb").read()
        bits = int(rest[1]) if len(rest) > 1 else 16
        d = bb.disassembler("capstone-x86-%d" % bits)
        for ins in d.disasm(data):
            print(ins)
        return 0
    print("unknown command %r (try: list, detect, compile, disasm)" % cmd); return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
