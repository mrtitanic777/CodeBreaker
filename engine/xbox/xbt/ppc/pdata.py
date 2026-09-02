"""Xbox 360 .pdata / exception-unwind parser.

The PE `.pdata` section on Xenon holds a RUNTIME_FUNCTION table — one 8-byte
entry per function giving its start, length, prologue size, and whether it has a
language exception handler. This is the ground truth for function extents (more
reliable than heuristic discovery) and tells you which functions carry unwind
info. Reimplemented clean-room from the documented Xenon RUNTIME_FUNCTION layout.

Entry: u32 BeginAddress; u32 packed = PrologLength[7:0] | FunctionLength[29:8]
       | Is32Bit[30] | HasExceptionHandler[31]  (all big-endian).
"""

import struct
import sys

DESCRIPTION = "parse the Xbox 360 .pdata exception-unwind table (function extents + prolog/EH flags)"


class RuntimeFunction:
    def __init__(self, begin, packed):
        self.begin = begin
        self.prolog_len = packed & 0xFF
        self.func_words = (packed >> 8) & 0x3FFFFF
        self.func_bytes = self.func_words * 4
        self.is_32bit = bool((packed >> 30) & 1)
        self.has_handler = bool((packed >> 31) & 1)

    def __repr__(self):
        return "0x%08X  len=%-6d prolog=%-4d %s%s" % (
            self.begin, self.func_bytes, self.prolog_len * 4,
            "EH " if self.has_handler else "   ",
            "32" if self.is_32bit else "64")


def parse(data):
    """Parse a raw .pdata blob into a list of RuntimeFunction."""
    out = []
    for o in range(0, len(data) - 7, 8):
        begin, packed = struct.unpack_from(">II", data, o)
        if begin == 0 and packed == 0:
            continue
        out.append(RuntimeFunction(begin, packed))
    return out


def from_xex(path):
    import xex
    x = xex.open_xex(path)
    img = x.basefile()
    secs = xex._pe_sections(img)
    pd = next((s for s in secs if s["name"].lower().startswith(".pdata")), None)
    if pd is None:
        raise ValueError("no .pdata section in %s" % path)
    return parse(img[pd["vaddr"]:pd["vaddr"] + pd["raw_size"]])


def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: pdata <xex> [--limit N] [--handlers]\n"
              "  --handlers  list only functions with an exception handler")
        return
    fns = from_xex(a[0])
    handlers = [f for f in fns if f.has_handler]
    if "--handlers" in a:
        for f in handlers:
            print("  %r" % f)
        print("(%d of %d functions carry an exception handler)" % (len(handlers), len(fns)))
        return
    limit = int(a[a.index("--limit") + 1]) if "--limit" in a else 40
    total = sum(f.func_bytes for f in fns)
    print("%s: %d functions in .pdata, %d KB of code, %d with EH" % (
        a[0], len(fns), total // 1024, len(handlers)))
    for f in fns[:limit]:
        print("  %r" % f)
    if len(fns) > limit:
        print("  ... (%d more)" % (len(fns) - limit))


if __name__ == "__main__":
    main()
