"""In-app viewers — hex dump, hex diff, and disassembly views for any file or
region. Pure formatting (no state), so the same functions back both the CLI
`view` command and the GUI's inspector panes.

  view hex    <file> [--offset O] [--len N]        classic hex + ASCII dump
  view diff   <a> <b> [--offset O] [--len N]       byte diff (only changed rows)
  view disasm <file> [--vaddr V] [--offset O] [--len N]   PowerPC listing

The hex dump collapses runs of identical rows to a single ``*`` (like xxd), and
the diff shows only the 16-byte rows that differ, marking changed columns. The
disassembly view reuses our PowerPC disassembler so any code region reads as a
listing without extracting it first.
"""

import sys

DESCRIPTION = "in-app viewers: hex dump, byte diff, PowerPC disassembly + decompile of any file/region"

_WIDTH = 16


def _printable(b):
    return chr(b) if 32 <= b < 127 else "."


def hexdump(data, base=0, width=_WIDTH, collapse=True):
    """Classic ``OFFSET  hex bytes  |ascii|`` dump. Runs of identical rows are
    collapsed to a single ``*`` line. Returns a string."""
    lines = []
    n = len(data)
    prev = None
    starred = False
    for off in range(0, n, width):
        row = data[off:off + width]
        if collapse and row == prev and off + width < n:
            if not starred:
                lines.append("*")
                starred = True
            continue
        starred = False
        prev = row
        hexs = []
        for i in range(width):
            if i < len(row):
                hexs.append("%02X" % row[i])
            else:
                hexs.append("  ")
            if i == width // 2 - 1:
                hexs.append("")                       # gap after 8 bytes
        ascii_ = "".join(_printable(b) for b in row)
        lines.append("%08X  %s  |%s|" % (base + off, " ".join(hexs).rstrip(), ascii_))
    return "\n".join(lines)


def hexdiff(a, b, base=0, width=_WIDTH):
    """Show only the `width`-byte rows where a and b differ. Each differing row
    prints both, with changed byte columns marked by '^' underneath. Returns
    (text, n_diff_rows, first_diff_offset|None)."""
    n = max(len(a), len(b))
    out = []
    ndiff = 0
    first = None
    for off in range(0, n, width):
        ra = a[off:off + width]
        rb = b[off:off + width]
        if ra == rb:
            continue
        ndiff += 1
        if first is None:
            first = off + next(i for i in range(width)
                               if a[off + i:off + i + 1] != b[off + i:off + i + 1])

        def row(r):
            return " ".join("%02X" % r[i] if i < len(r) else "  " for i in range(width))
        mark = " ".join("^^" if (a[off + i:off + i + 1] != b[off + i:off + i + 1])
                        else "  " for i in range(width))
        out.append("%08X A %s" % (base + off, row(ra)))
        out.append("%08X B %s" % (base + off, row(rb)))
        out.append("%8s   %s" % ("", mark))
    return "\n".join(out), ndiff, first


def disasm_view(data, base=0, vaddr=0):
    """PowerPC listing of a code blob. Reuses disasm_ppc. Returns a string."""
    import importlib
    try:
        disasm_ppc = importlib.import_module(".disasm_ppc", "xbt.ppc")
    except Exception:
        disasm_ppc = importlib.import_module("disasm_ppc")
    rows = []
    for va, raw, mn, op in disasm_ppc.listing(data, vaddr or base):
        rows.append("%08X  %s  %-8s %s" % (va, raw.hex(), mn, op))
    return "\n".join(rows)


def decomp_view(data, base=0, vaddr=0):
    """Pseudo-C decompilation of a code blob. Reuses ppcdecomp. Returns a string."""
    import importlib
    try:
        ppcdecomp = importlib.import_module(".ppcdecomp", "xbt.ppc")
    except Exception:
        ppcdecomp = importlib.import_module("ppcdecomp")
    va = vaddr or base
    return ppcdecomp.decompile_code(data, va, "sub_%08X" % va)


# ── self-verification ──────────────────────────────────────────────────────
def _selfcheck():
    # hex dump
    data = bytes(range(16)) + b"A" * 16
    d = hexdump(data)
    if not d.splitlines()[0].startswith("00000000  00 01 02 03 04 05 06 07"):
        return False, "hexdump first row wrong:\n" + d
    if "|" not in d or d.splitlines()[0][-2] not in ".\x01":
        return False, "hexdump ascii gutter missing"
    # collapse: 64 identical rows -> one '*' between first and last
    big = b"\xAA" * (16 * 5)
    if "*" not in hexdump(big):
        return False, "hexdump did not collapse identical rows"
    # diff
    a = bytes(32)
    b = bytearray(32); b[5] = 0xFF; b[20] = 0x11
    text, nrows, first = hexdiff(a, bytes(b))
    if first != 5 or nrows != 2:
        return False, "hexdiff wrong: first=%r nrows=%r" % (first, nrows)
    if "^^" not in text:
        return False, "hexdiff change marker missing"
    # disasm view
    import importlib
    try:
        ppcasm = importlib.import_module(".ppcasm", "xbt.ppc")
    except Exception:
        ppcasm = importlib.import_module("ppcasm")
    code = ppcasm.assemble("li r3, 1\nblr\n", 0x82000000)
    dv = disasm_view(code, vaddr=0x82000000)
    if "li" not in dv or "blr" not in dv:
        return False, "disasm view missing expected mnemonics:\n" + dv
    # decomp view
    cv = decomp_view(ppcasm.assemble("add r3, r3, r4\nblr\n", 0x82000000), vaddr=0x82000000)
    if "r3 = r3 + r4;" not in cv or "return r3;" not in cv:
        return False, "decomp view wrong:\n" + cv
    return True, "hex dump (+collapse) + byte diff (first@5, 2 rows) + PPC disasm + decomp view"


def _read_region(path, a):
    def opt(flag, default):
        return a[a.index(flag) + 1] if flag in a else default
    off = int(opt("--offset", "0"), 0)
    ln = opt("--len", None)
    with open(path, "rb") as f:
        f.seek(off)
        data = f.read(int(ln, 0)) if ln else f.read()
    return data, off


def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage:\n"
              "  view hex    <file> [--offset O] [--len N]\n"
              "  view diff   <a> <b> [--offset O] [--len N]\n"
              "  view disasm <file> [--vaddr V] [--offset O] [--len N]\n"
              "  view decomp <file> [--vaddr V] [--offset O] [--len N]\n"
              "  view --selftest")
        return
    if a[0] == "--selftest":
        ok, msg = _selfcheck()
        print(("PASS: " if ok else "FAIL: ") + msg)
        sys.exit(0 if ok else 1)
    op = a[0]
    if op == "hex":
        data, off = _read_region(a[1], a)
        print(hexdump(data, base=off))
    elif op == "diff":
        def opt(flag, default):
            return a[a.index(flag) + 1] if flag in a else default
        off = int(opt("--offset", "0"), 0)
        ln = opt("--len", None)
        da, db = open(a[1], "rb").read(), open(a[2], "rb").read()
        if ln:
            da, db = da[off:off + int(ln, 0)], db[off:off + int(ln, 0)]
        text, nrows, first = hexdiff(da, db, base=off)
        if nrows == 0:
            print("identical (%d bytes)" % max(len(da), len(db)))
        else:
            print(text)
            print("\n%d differing row(s); first difference at 0x%08X" % (nrows, first))
    elif op in ("disasm", "decomp"):
        def opt(flag, default):
            return a[a.index(flag) + 1] if flag in a else default
        va = int(opt("--vaddr", "0"), 0)
        data, off = _read_region(a[1], a)
        view_fn = disasm_view if op == "disasm" else decomp_view
        print(view_fn(data, vaddr=va or off))
    else:
        print("unknown op: %s (hex/diff/disasm/decomp)" % op)
        sys.exit(2)


if __name__ == "__main__":
    main()
