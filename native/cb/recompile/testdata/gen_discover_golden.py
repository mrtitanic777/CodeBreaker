"""Writes discover_data.bin and discover.golden.txt: what codebreaker/recompile/discover.py finds in 16 KB of the Minecraft
Xbox 360 Edition's code (testdata/ppc_slice.bin, from 0x822B6E00) plus a made data section whose words are pointers (real
function starts the slice's `blr`s end, and the kinds of word the scan must refuse), as the reference the C++
(cb/recompile/discover.cpp) is checked against. Run from the checkout root:
    python native/cb/recompile/testdata/gen_discover_golden.py

discover_data.bin is the data section (big-endian words), loaded at DATA_VA. The golden file gives the image's other facts
(entry, .pdata extents, imports) and the answers: "[name]" sections of lines."""
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, "..", "..", "..", ".."))
from codebreaker.recompile import discover

CODE_VA = 0x822B6E00
DATA_VA = 0x82900000
ENTRY = 0x822B6EA8
EXTENTS = {0x822B6EA8: 0x100, 0x822B7000: 0x80}
IMPORTS = {0x822B7400}
SEEDS = [0x822B7800, 0x822B6E00 + 0x1234 * 0 + 0x3000, 0x10000000, 0x822B7000]   # in the code, in the code, outside, already an extent

code = open(os.path.join(HERE, "ppc_slice.bin"), "rb").read()
CODE_END = CODE_VA + len(code)

# data words: the first instruction after each blr of the slice (real function starts, up to 40 of them), then refusals
words = []
for off in range(0, len(code) - 4, 4):
    if struct.unpack_from(">I", code, off)[0] == 0x4E800020 and len(words) < 40:
        words.append(CODE_VA + off + 4)
words += [
    0, 0xFFFFFFFF, CODE_VA + 1,                    # null, all ones, unaligned
    0x82000000, CODE_END, CODE_END + 4,            # outside the code (the end is outside)
    0x822B6EA8 + 0x10, 0x822B7000 + 4,             # inside an extent
    0x822B6EA8, 0x822B7000,                        # a known function's start
    CODE_VA, CODE_VA + 8,                          # the first words of the code
]
words += words[:5]                                 # a repeated pointer is found once
data = b"".join(struct.pack(">I", w) for w in words)
with open(os.path.join(HERE, "discover_data.bin"), "wb") as f:
    f.write(data)


class FakeImage:
    code = (CODE_VA, CODE_END)
    entry = ENTRY
    extents = {a: (s, 0, False) for a, s in EXTENTS.items()}
    imports = {a: "imp" for a in IMPORTS}
    sections = [(".text", CODE_VA, len(code), 0x60000020), (".rdata", DATA_VA, len(data), 0x40000040)]

    def bytes_at(self, va, size):
        if CODE_VA <= va and va + size <= CODE_END:
            return code[va - CODE_VA: va - CODE_VA + size]
        if va == DATA_VA:
            return data[:size]
        return b""


img = FakeImage()
discover._image = lambda path: img

out = ["# made by gen_discover_golden.py; see there", "entry %08X" % ENTRY]
out += ["extent %08X %X" % kv for kv in sorted(EXTENTS.items())]
out += ["import %08X" % a for a in sorted(IMPORTS)]
known = {a: s for a, s in EXTENTS.items()}

out.append("[from_pointers]")
out += ["%08X" % a for a in sorted(discover.from_pointers(img, known))]
out.append("[from_constants]")
out += ["%08X" % a for a in sorted(discover.from_constants(img, known))]
out.append("[plausible]")
for a in range(CODE_VA, CODE_VA + 4 * 512, 4):
    out.append("%08X %d" % (a, 1 if discover._plausible_start(code, CODE_VA, a) else 0))


def run(name, **kw):
    funcs, sources, _img = discover.discover("unused", **kw)
    out.append("[%s]" % name)
    out.append("sources " + " ".join("%s=%d" % kv for kv in sources.items()))
    out.extend("%08X %d" % (a, funcs[a]) for a in sorted(funcs))


run("discover_default")
run("discover_seeds", extra=SEEDS)
run("discover_no_scan", pointer_scan=False)
run("discover_no_scan_seeds", pointer_scan=False, extra=SEEDS)

with open(os.path.join(HERE, "discover.golden.txt"), "w", newline="\n") as f:
    f.write("\n".join(out) + "\n")
print(len(words), "data words;", len(out), "lines")
