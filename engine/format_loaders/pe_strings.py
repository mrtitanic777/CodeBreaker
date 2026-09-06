#!/usr/bin/env python3
"""
pe_strings.py — VA-mapped, categorized string extraction for a PE image.

`build_engine/datacode.find_strings` scans a single carved segment; this scans
the whole image and, crucially, maps every hit to its **virtual address** so a
data cross-reference (pe_trace.PeCarve.data_refs) can link code to the literal
it uses. Both ASCII and UTF-16LE are recovered.

Categories are generic and content-derived only — no knowledge of any specific
program or SDK. Tab/CR/LF are part of the string class so a printf format's
leading "\\n" is captured and the string's start VA lines up with the
instruction that pushes it.

    python pe_strings.py foo.exe [--min N] [--cat CATEGORY] [--ascii]
    python pe_strings.py --selftest
"""
import re
import sys

from engine.format_loaders.pe import open_pe

_ASCII = rb"[\t\r\n\x20-\x7e]{%d,}"
_UTF16 = rb"(?:[\t\r\n\x20-\x7e]\x00){%d,}"
_FMT = re.compile(r"%[-+ 0-9.*#hlLIw]*[sdiouxXeEfgGcpn%]")
_MODULE_EXT = (".dll", ".exe", ".sys", ".drv", ".pdb", ".so", ".ocx", ".cpl", ".scr")


def categorize(s):
    """A generic, content-only category for a recovered string."""
    t = s.strip()
    if _FMT.search(t):
        return "format"
    low = t.lower()
    if low.endswith(_MODULE_EXT):
        return "module"
    if "\\" in t or "/" in t:
        return "path"
    if re.fullmatch(r"[A-Z][A-Z0-9]*(?:_[A-Z0-9]+)+", t):    # FOO_BAR_BAZ constant
        return "constant"
    if re.search(r"(?i)error|fail|invalid|cannot|unexpected|exception", low):
        return "error"
    return "other"


def _va_of(pe, off):
    for s in pe.sections:
        if s["rawptr"] <= off < s["rawptr"] + s["rawsize"]:
            return pe.image_base + s["vaddr"] + (off - s["rawptr"])
    return None


def extract(pe, min_len=4, want_utf16=True):
    """[{va, off, kind, cat, text}] for every ASCII (and optional UTF-16) string."""
    d = pe.data
    out = []
    for m in re.finditer(_ASCII % min_len, d):
        t = m.group().decode("latin1")
        out.append({"va": _va_of(pe, m.start()), "off": m.start(),
                    "kind": "ascii", "cat": categorize(t), "text": t})
    if want_utf16:
        for m in re.finditer(_UTF16 % min_len, d):
            t = m.group().decode("utf-16le", "replace")
            out.append({"va": _va_of(pe, m.start()), "off": m.start(),
                        "kind": "utf16", "cat": categorize(t), "text": t})
    out.sort(key=lambda r: r["off"])
    return out


def by_va(pe, min_len=4, want_utf16=True):
    """{va: text} for strings that resolved to a VA (feeds PeCarve.string_refs)."""
    return {r["va"]: r["text"] for r in extract(pe, min_len, want_utf16) if r["va"]}


def _selftest():
    class _PE:
        image_base = 0x400000
        sections = [{"name": ".rdata", "rawptr": 0, "rawsize": 0x60, "vaddr": 0x1000}]
        # \xff separators keep an ASCII NUL from bleeding into the UTF-16 scan.
        data = (b"\x00\x00hello world\x00\xff" + b"\n   %08X value\x00\xff"
                + b"F\x00O\x00O\x00_\x00B\x00A\x00R\x00\x00\x00\xff"
                + b"a\\b\\c.dll\x00")
    r = extract(_PE(), 4)
    cats = {x["text"].strip(): x["cat"] for x in r}
    assert cats.get("hello world") == "other", cats
    assert cats.get("%08X value") == "format", cats
    assert cats.get("a\\b\\c.dll") == "module", cats
    assert any(x["kind"] == "utf16" and x["cat"] == "constant" for x in r), r
    print("[OK] pe_strings extract + generic categorize (%d hits)" % len(r))
    return True


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        sys.exit(__doc__)
    if argv[0] == "--selftest":
        sys.exit(0 if _selftest() else 1)
    path = argv[0]
    min_len = int(argv[argv.index("--min") + 1]) if "--min" in argv else 4
    want_utf16 = "--ascii" not in argv
    only = argv[argv.index("--cat") + 1] if "--cat" in argv else None
    rows = extract(open_pe(path), min_len, want_utf16)
    from collections import Counter
    for r in rows:
        if only and r["cat"] != only:
            continue
        va = ("%#010x" % r["va"]) if r["va"] else "     ----- "
        print("%s  %-6s %-9s %s" % (va, r["kind"], r["cat"], r["text"].strip()[:100]))
    print("# %d strings  %s" % (len(rows), dict(Counter(r["cat"] for r in rows))),
          file=sys.stderr)


if __name__ == "__main__":
    main()
