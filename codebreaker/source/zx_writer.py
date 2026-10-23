"""zx_writer.py -- a ZX Spectrum program as assembly source that builds back into it.

    python -m codebreaker.source.zx_writer <game.z80|.sna|.tap> [<out folder>]
    from codebreaker.source import zx_writer
    r = zx_writer.reconstruct(path, folder)     # as source/z80_writer.py's write()

The program block (formats/zx.py: a snapshot's RAM above the screen, a tape's code block) is
written through source/z80_writer.py, with the Spectrum's names: its ULA port, the 48K ROM's
documented entry points and the system variables, as the Spectrum manual and The Complete
Spectrum ROM Disassembly name them.
"""
import os
import sys

from codebreaker.formats import zx
from codebreaker.source import z80_writer

PORTS = {0xFE: "ULA", 0x1F: "KEMPSTON"}
NAMES = {0x0010: "PRINT_A", 0x03B5: "BEEPER", 0x04C2: "SA_BYTES", 0x0556: "LD_BYTES", 0x0D6B: "CLS",
         0x0DAF: "CL_ALL", 0x1601: "CHAN_OPEN", 0x203C: "PR_STRING", 0x2D2B: "STACK_BC", 0x2DE3: "PRINT_FP",
         0x5C08: "LAST_K", 0x5C36: "CHARS", 0x5C3B: "FLAGS", 0x5C48: "BORDCR", 0x5C4B: "VARS", 0x5C4F: "CHANS",
         0x5C53: "PROG", 0x5C76: "SEED", 0x5C78: "FRAMES", 0x5C8D: "ATTR_P", 0x5CB2: "RAMTOP"}


def reconstruct(path, folder):
    p = zx.read(path)
    stem = os.path.splitext(os.path.basename(path))[0]
    header = ["ZX Spectrum %s %s; %d bytes at $%04X" % (p.kind, os.path.basename(path), len(p.image), p.origin)]
    if p.registers:
        header.append("registers at the snapshot: " + " ".join(
            "%s=%0*X" % (k.upper(), 4 if k in ("sp", "ix", "iy") else 2, v)
            for k, v in sorted(p.registers.items()) if not k.endswith("_")))
    return z80_writer.write(p.image, p.origin, p.entries, folder, stem, PORTS, NAMES, header)


def main(argv):
    if not argv:
        print(__doc__.split("\n\n")[0])
        return 2
    folder = argv[1] if len(argv) > 1 else os.path.splitext(argv[0])[0] + "_source"
    r = reconstruct(argv[0], folder)
    print("%s: %d functions, %d of %d bytes reached as code -> %s" % (
        os.path.basename(argv[0]), len(r["functions"]), r["code bytes"], r["bytes"], r["source"]))
    print("  rebuilt: %s" % ("identical to the original" if r["same"] else "; ".join(r["notes"]) or "differs"))
    return 0 if r["same"] else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
