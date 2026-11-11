"""Writes ppc_slice.bin, ppc_slice.listing.txt and ppc_slice.reach.txt: 16 KB of the Minecraft Xbox 360 Edition's code (from
0x822b6e00, around the entry point 0x822b6ea8: ordinary code, 734 VMX128 words, tables, padding) and what the reference
(codebreaker/recompile/disassemble.py, Capstone 5.0.7) answers for it, as the reference the C++
(cb/recompile/disassemble.cpp) is checked against. Run from the checkout root:
    python native/cb/recompile/testdata/gen_ppc_slice.py ["path to default.xex"]

ppc_slice.listing.txt: one line per word, "<address> <word hex> <V if VMX128 decoded it, else -> <mnemonic>|<operands>".
ppc_slice.reach.txt: the base, the entries, and the byte runs flow reaches ("covered") and does not ("data"), as begin-end.
"""
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, "..", "..", "..", ".."))
from codebreaker.decompile import vmx128
from codebreaker.recompile import disassemble as d

XEX = sys.argv[1] if len(sys.argv) > 1 else "C:/Nexia360/Games/Minecraft Xbox 360 Edition/default.xex"
SLICE_VA, SLICE_LEN = 0x822B6E00, 16384

body, base, entry = d.code_section(XEX)
start = SLICE_VA - base
sl = body[start:start + SLICE_LEN]
with open(os.path.join(HERE, "ppc_slice.bin"), "wb") as f:
    f.write(sl)

lines = []
for va, raw, mn, ops in d.listing(sl, SLICE_VA):
    vector = 1 if vmx128.decode(int.from_bytes(raw, "big")) else 0
    lines.append("%08X %s %s %s|%s" % (va, raw.hex(), "V" if vector else "-", mn, ops))
with open(os.path.join(HERE, "ppc_slice.listing.txt"), "w", newline="\n") as f:
    f.write("\n".join(lines) + "\n")


def runs(offsets):
    out, cur = [], None
    for i in sorted(offsets):
        if cur and i == cur[1]:
            cur[1] = i + 1
        else:
            if cur:
                out.append(tuple(cur))
            cur = [i, i + 1]
    if cur:
        out.append(tuple(cur))
    return out


covered = d.classify(sl, SLICE_VA, [entry, SLICE_VA])
data = d.data_spans(sl, SLICE_VA, [entry, SLICE_VA])
with open(os.path.join(HERE, "ppc_slice.reach.txt"), "w", newline="\n") as f:
    f.write("base %08X\nentries %08X %08X\ncovered %s\ndata %s\n" % (
        SLICE_VA, entry, SLICE_VA, " ".join("%d-%d" % r for r in runs(covered)), " ".join("%d-%d" % r for r in data)))
print(len(lines), "words")
