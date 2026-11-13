"""Writes natalsu.golden.txt: what codebreaker/formats/powerpc_image.py says of natalsu.exe (the Xbox 360 dashboard's natalsu
image, unpacked from its .xex), as the reference cb/formats/powerpc_image is checked against. Run from a checkout that has the
native formats.dll built (a worktree gives the main checkout's folder as its argument):
    python native/cb/formats/testdata/gen_powerpc_image_golden.py [checkout]
"""
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, sys.argv[1] if len(sys.argv) > 1 else os.path.join(HERE, "..", "..", "..", ".."))   # (or the checkout that has formats.dll built)
from codebreaker.formats import powerpc_image  # noqa: E402

img = powerpc_image.read(os.path.join(HERE, "natalsu.exe"))
out = []
out.append("base %08x" % img.base)
out.append("memory %d" % img.memory_layout)
out.append("code %08x %08x" % img.code)
out.append("entry %08x" % img.entry)
for a, off, n in img.spans:
    out.append("span %08x %x %x" % (a, off, n))
for name, a, n, chars in img.sections:
    out.append("section %s %08x %x %08x" % (name, a, n, chars))
for a in img.functions:
    out.append("function %08x" % a)
for a, name in sorted(img.imports.items()):
    out.append("import %08x %s" % (a, name))
for a, name in sorted(img.inline.items()):
    out.append("inline %08x %s" % (a, name))
for a, name in sorted(img.run_names.items()):
    out.append("run %08x %s" % (a, name))
for a, (n, p, h) in sorted(img.extents.items()):
    out.append("extent %08x %x %x %d" % (a, n, p, h))
for a in (img.code[0], img.code[0] + 4, img.code[0] + 8, 0x1000):
    w = img.word(a)
    out.append("word %08x %s" % (a, "none" if w is None else "%08x" % w))
with open(os.path.join(HERE, "natalsu.golden.txt"), "w", newline="\n") as f:
    f.write("\n".join(out) + "\n")
