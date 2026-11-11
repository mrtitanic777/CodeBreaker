"""Provenance of data_*.golden.* (the cb::build::data tests): what the Python build/data.py wrote for the real modules in
native/cb/formats/testdata, with the stand-ins for the reconstructed functions that data_test.cpp uses. Run from the
checkout root while the Python is still the live implementation (it needs formats.dll). The outputs were reviewed by hand
(see data_test.cpp) before being checked in; this script is only how they were made.
"""
import os
import sys

sys.path.insert(0, os.getcwd())
os.environ.setdefault("CB_FORMATS_BIN", r"C:\Users\mrtit\Documents\GitHub\CodeBreaker\native\formats\build\bin")
from codebreaker.build import data  # noqa: E402
from codebreaker.formats.ne import NEFile  # noqa: E402
from codebreaker.formats.pe import open_pe  # noqa: E402

HERE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.abspath(os.path.join(HERE, "..", "..", "formats", "testdata"))


def write(name, text):
    with open(os.path.join(HERE, name), "w", newline="\n") as f:
        f.write(text)


def summary(d):
    out = ["pointers=%d size=%d movable=%s" % (d.pointers, d.size, d.movable)]
    out += ["section %d %s %x %d" % s for s in d.sections]
    out += ["part %s %x %d %d" % p for p in d.parts]
    out += ["unresolved %s | %s" % u for u in d.unresolved]
    for sym in sorted(d.imports):
        mod, by = d.imports[sym]
        out.append("import %s %s %s" % (sym, mod, ("#%d" % by) if isinstance(by, int) else by))
    for (seg, off), name in sorted(d.exported.items()):
        out.append("exported %d %x %s" % (seg, off, name))
    for off, name in sorted(d.cuts.items()):
        out.append("cut %x %s" % (off, name))
    return "\n".join(out) + "\n"


def text_range(path):
    pe = open_pe(path)
    t = [s for s in pe.sections if s["name"] == ".text"][0]
    return pe.image_base + t["vaddr"], pe.image_base + t["vaddr"] + t["vsize"]


def fn32(path):
    lo, hi = text_range(path)
    return lambda va: ("fn_%x" % va) if lo <= va < hi else None


for name in ("lz32.dll", "desk.cpl", "comcat.dll"):
    path = os.path.join(SRC, name)
    d = data.data32(path, fn32(path))
    stem = "data_" + name.replace(".", "_")
    write(stem + ".golden.c", d.source)
    write(stem + ".golden.decl", d.declarations)
    write(stem + ".golden.txt", summary(d))

# exported data: cut lz32's .data in three
path = os.path.join(SRC, "lz32.dll")
pe = open_pe(path)
base = pe.image_base
named = {base + 0x2010: "g_exported", base + 0x2040: "g_second"}
d = data.data32(path, fn32(path), named)
write("data_lz32_named.golden.c", d.source)
write("data_lz32_named.golden.decl", d.declarations)
write("data_lz32_named.golden.txt", summary(d))

# the words the relocation table marks, and the addresses each function's code refers to
words = data.relocated_words(pe)
lo, hi = text_range(path)
starts = [lo, lo + 0x100, lo + 0x200, lo + 0x300]
consts = data.address_constants(path, starts)
lines = ["word %x %x" % (va, words[va]) for va in sorted(words)]
for s in starts:
    lines.append("constants %x %s" % (s, " ".join("%x" % a for a in sorted(consts[s]))))
write("data_lz32_words.golden.txt", "\n".join(lines) + "\n")

# the 32-bit text and header rewrites
dd = data.data32(path, fn32(path), named)
p0 = dd.parts[0]
va0 = p0[1]
header = ("#define g_a (*(int *)0x%x)\n#define g_b (*(char *)0x%x)\nint helper(void);\n"
          "#define g_far (*(int *)0x%x)\n#define g_upper (*(int *)0x%X)\n" % (va0 + 4, va0 + 0x44, 0x7fff0000, va0))
h, lost = data.relocate_header(header, dd)
write("data_lz32_header.golden.txt", h + "LOST " + " ".join("%x" % x for x in lost) + "\n")
body = ("int f(void) { int a = *(int *)0x%x; int b = x-0x%x; int c = -0x%x; int d = 0x%x; int e = 0x%xU; "
        "int g = 0x%x; return v.0x%x + 0x%x; }\n" % (va0 + 4, va0 + 8, (1 << 32) - (lo + 8), lo + 0x10, va0 + 0x44, 0x12345,
                                                      va0, lo + 0x20))
text = "static int pre = 0x%x;\n" % (va0 + 4) + body
addrs = {va0 + 4, va0 + 8, va0 + 0x44, lo + 8, lo + 0x10, lo + 0x20, 0x12345, 0x55550000}
t, lost = data.relocate_text(text, addrs, dd, fn32(path))
write("data_lz32_text.golden.txt", "INPUT\n" + text + "OUTPUT\n" + t + "LOST " + " ".join("%x" % x for x in lost) + "\n")


# ---- 16-bit
def fn16(nef):
    code = {s["idx"] for s in nef.segments() if s["kind"] == "CODE"}
    return lambda place: ("fn_%d_%04x" % place) if place[0] in code else None


def seg_symbol(seg):
    return "seg_%d" % seg if seg % 2 == 0 else None


for name in ("lzexpand.dll", "charmap.exe"):
    path = os.path.join(SRC, name)
    nef = NEFile(path)
    d = data.data16(path, fn16(nef), (), seg_symbol)
    stem = "data_" + name.replace(".", "_")
    write(stem + ".golden.c", d.source)
    write(stem + ".golden.txt", summary(d))

# with the other segments carried: pointers into them become far pointers
path = os.path.join(SRC, "charmap.exe")
nef = NEFile(path)
d = data.data16(path, fn16(nef), {1}, seg_symbol)
write("data_charmap_exe_far.golden.c", d.source)
write("data_charmap_exe_far.golden.txt", summary(d))

path = os.path.join(SRC, "lzexpand.dll")
nef = NEFile(path)
segs = {s["idx"]: bytes(nef.data[s["file_off"]:s["file_off"] + s["length"]]) for s in nef.segments()
        if s["idx"] in (2, 4)}
write("data_lzexpand_dll_segments.golden.c", data.far_segments(segs, path, fn16(nef), seg_symbol))
write("data_lzexpand_dll_segments_bare.golden.c", data.far_segments(segs))

# a code segment of charmap with its internal fix-ups, the other segments carried
path = os.path.join(SRC, "charmap.exe")
nef = NEFile(path)
segs = {s["idx"]: bytes(nef.data[s["file_off"]:s["file_off"] + s["length"]]) for s in nef.segments()}
write("data_charmap_exe_segments.golden.c", data.far_segments(segs, path, fn16(nef), seg_symbol))
