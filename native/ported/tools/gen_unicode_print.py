"""native/ported/tools/gen_unicode_print.py <out>: generate native/ported/include/py_unicode_print.hpp: the code points str.isprintable() refuses (Cc Cf Cs Co Cn Zl Zp Zs but the space),
as sorted ranges, from CPython's own unicodedata (Unicode %s)."""
import sys
import unicodedata

NON = {"Cc", "Cf", "Cs", "Co", "Cn", "Zl", "Zp", "Zs"}
ranges = []
start = None
for cp in range(0x110000):
    ch = chr(cp)
    bad = cp != 0x20 and unicodedata.category(ch) in NON
    assert bad == (not ch.isprintable()), hex(cp)
    if bad and start is None:
        start = cp
    if not bad and start is not None:
        ranges.append((start, cp - 1))
        start = None
if start is not None:
    ranges.append((start, 0x10FFFF))
out = ["// py_unicode_print.hpp -- GENERATED from CPython's unicodedata (Unicode %s, Python %s) by runtime.unicode/gen.py." % (unicodedata.unidata_version, sys.version.split()[0]),
       "// The code points str.isprintable() is False for, as sorted inclusive ranges (Cc Cf Cs Co Cn Zl Zp Zs, the space excepted).",
       "#pragma once", "#include <cstddef>", "", "namespace py {", "namespace unicode_ {", "inline const char32_t nonprintable_ranges[][2] = {"]
line = "   "
for a, b in ranges:
    item = " {0x%X, 0x%X}," % (a, b)
    if len(line) + len(item) > 150:
        out.append(line)
        line = "   "
    line += item
out.append(line)
out += ["};", "inline bool isprintable(char32_t c) { return !in_(nonprintable_ranges, c); }", "}  // namespace unicode_", "}  // namespace py", ""]
open(sys.argv[1], "w", newline="\n").write("\n".join(out))
print(len(ranges), "ranges")
