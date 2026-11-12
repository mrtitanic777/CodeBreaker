"""Writes config_code.bin, config_switches.toml and config.golden.txt: what codebreaker/recompile/config.py makes of a small
made Xbox 360 program (config_code.bin: 0x800 bytes of PowerPC at CODE_VA, hand-assembled below), as the reference the C++
(cb/recompile/config.cpp) is checked against. The program has: a .pdata function that calls one function and tail-jumps to
two more, the register save/restore runs (by name), the vector runs (found by their `li r11,-0x120` and `stvx` / `lvx`),
a setjmp and a longjmp (and a lookalike of it that must not match), and two switches (one in a gap between .pdata functions,
one inside a function .pdata leaves out). Run from the checkout root:
    python native/cb/recompile/testdata/gen_config_golden.py
"""
import os
import struct
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, "..", "..", "..", ".."))
from codebreaker.recompile import config, discover  # noqa: E402

CODE_VA = 0x82100000
SIZE = 0x800
BLR, BCTR, NOP = 0x4E800020, 0x4E800420, 0x60000000
MFLR_R12 = 0x7D8802A6
MTCTR_R11 = 0x7D6903A6


def b(frm, to, link=False):
    return (18 << 26) | ((to - frm) & 0x03FFFFFC) | (1 if link else 0)


def bc(frm, to, bo, bi):
    return (16 << 26) | (bo << 21) | (bi << 16) | ((to - frm) & 0xFFFC)


def li(rd, imm):
    return (14 << 26) | (rd << 21) | (imm & 0xFFFF)


def xform(op, rt, ra, rb, xo):
    return (op << 26) | (rt << 21) | (ra << 16) | (rb << 11) | (xo << 1)


def lwz(rt, d, ra):
    return (32 << 26) | (rt << 21) | (ra << 16) | (d & 0xFFFF)


def std(rs, d, ra):
    return (62 << 26) | (rs << 21) | (ra << 16) | (d & 0xFFFC)


words = {}


def put(off, *ws):
    for i, w in enumerate(ws):
        words[off + 4 * i] = w & 0xFFFFFFFF


V = lambda off: CODE_VA + off  # noqa: E731

# F1 (.pdata, 0x40): calls X, tail-jumps to Y, conditionally to Z, then returns
put(0x00, MFLR_R12, 0x9181FFF8, b(V(0x08), V(0x100), True), bc(V(0x0C), V(0x30), 12, 2), b(V(0x10), V(0x180)),
    bc(V(0x14), V(0x1C0), 4, 2), NOP, NOP, NOP, NOP, NOP, NOP, BLR)
# F2 (.pdata, 0x20): calls the save run, tail-jumps into the restore run
put(0x40, 0x3821FFA0, b(V(0x44), V(0x200), True), b(V(0x48), V(0x240)), NOP, NOP, NOP, NOP, BLR)
# X (no .pdata): a leaf with a call to what will be a switch's case label; its switch jumps to labels in X and in Y
put(0x100, MFLR_R12, li(3, 1), bc(V(0x108), V(0x110), 12, 2), b(V(0x10C), V(0x520), True), MTCTR_R11, BCTR, NOP, NOP, NOP,
    NOP, NOP, NOP, NOP, NOP, BLR)
put(0x150, NOP, BLR)
put(0x160, NOP, BLR)
# Y (no .pdata): a function reached only by a tail jump
put(0x180, NOP, NOP, BLR)
put(0x1A0, NOP, NOP, NOP, NOP, NOP, NOP, NOP, BLR)
# Z (no .pdata)
put(0x1C0, MFLR_R12, BLR)
# the save and restore runs, an entry per register
put(0x200, *[0xF9C1FF70 + 8 * i for i in range(8)], BLR)
put(0x240, *[0xE9C1FF70 + 8 * i for i in range(8)], BLR)
# the vector runs and two lookalikes
put(0x300, li(11, -0x120), xform(31, 14, 11, 12, 231), BLR)            # stvx v14,r11,r12: savevmx_14
put(0x320, li(11, -0x120), xform(31, 14, 11, 12, 103), BLR)            # lvx v14,r11,r12: restvmx_14
put(0x340, li(11, -0x400), xform(31, 14, 11, 12, 231), BLR)            # -0x400 with v14: not the _64 run
put(0x360, li(11, -0x120), xform(31, 3, 4, 5, 266), BLR)               # add: not a vector store or load
# a lookalike of longjmp (the base register differs) before the real ones
put(0x3C0, lwz(0, 0x134, 4), NOP, lwz(1, 0x90, 3), BLR)
# setjmp (no .pdata): stores r1 at 0x90 of the buffer
put(0x400, std(1, 0x90, 3), li(3, 0), BLR)
# F3 (.pdata, 0x40): longjmp -- loads LR from 0x134(r3) and r1 from 0x90(r3)
put(0x430, MFLR_R12, NOP, NOP, NOP, lwz(0, 0x134, 3), NOP, lwz(1, 0x90, 3), BLR)
# F5 (.pdata, 0x20)
put(0x4C0, NOP, NOP, NOP, BLR)
# a switch between .pdata functions: mtctr, bctr, and three cases
put(0x500, MTCTR_R11, BCTR)
put(0x510, NOP, BLR)
put(0x520, NOP, BLR)
put(0x530, NOP, BLR)
# F6 (.pdata, 0x20)
put(0x600, NOP, NOP, BLR)

code = b"".join(struct.pack(">I", words.get(o, 0)) for o in range(0, SIZE, 4))
with open(os.path.join(HERE, "config_code.bin"), "wb") as f:
    f.write(code)

EXTENTS = {V(0x00): 0x40, V(0x40): 0x20, V(0x430): 0x40, V(0x4C0): 0x20, V(0x600): 0x20}
RUN_NAMES = {V(0x200): "__savegprlr_14", V(0x204): "__savegprlr_15", V(0x240): "__restgprlr_14", V(0x244): "__restgprlr_15",
             V(0x280): "__savefpr_14"}
IMPORTS = {V(0x700)}
ENTRY = V(0)


class FakeImage:
    code = (CODE_VA, CODE_VA + SIZE)
    entry = ENTRY
    extents = {a: (n, 0, False) for a, n in EXTENTS.items()}
    imports = {a: "imp" for a in IMPORTS}
    run_names = RUN_NAMES
    sections = [(".text", CODE_VA, SIZE, 0x60000020)]

    def bytes_at(self, va, size):
        return code[va - CODE_VA: va - CODE_VA + size] if CODE_VA <= va and va + size <= CODE_VA + SIZE else b""


img = FakeImage()
discover._image = lambda path: img

SWITCHES_GAP = {V(0x504): [V(0x510), V(0x520), V(0x530)]}
SWITCHES_BOTH = {V(0x504): [V(0x510), V(0x520), V(0x530)], V(0x114): [V(0x150), V(0x160), V(0x1AC)]}
SWITCHES_EMPTY_FIRST = {V(0x114): [V(0x150), V(0x160), V(0x1AC)], V(0x504): [V(0x510), V(0x520), V(0x530)]}

out = ["# made by gen_config_golden.py; see there"]
out += ["extent %08X %X" % kv for kv in sorted(EXTENTS.items())]
out += ["import %08X" % a for a in sorted(IMPORTS)]
out += ["run %08X %s" % kv for kv in sorted(RUN_NAMES.items())]
out += ["entry %08X" % ENTRY]
out.append("[helpers]")
out += ["%s %08X" % kv for kv in sorted(config.helpers(img).items())]
out.append("[jump_buffers]")
out += ["%s %08X" % kv for kv in sorted(config.jump_buffers(img, code).items())]
out.append("[start_before]")
for a in range(CODE_VA, CODE_VA + SIZE, 4):
    out.append("%08X %08X" % (a, config._start_before(code, CODE_VA, a)))
out.append("[end_of_run]")
for a in range(CODE_VA, CODE_VA + SIZE, 4):
    out.append("%08X %08X" % (a, config._end_of_run(code, CODE_VA, a)))


def extras(name, switches):
    out.append("[extra_%s]" % name)
    out.extend("%08X %X" % kv for kv in config.extra_functions("x.xex", img, switches))


extras("none", None)
extras("gap", SWITCHES_GAP)
extras("both", SWITCHES_BOTH)
extras("both_reversed", SWITCHES_EMPTY_FIRST)

with open(os.path.join(HERE, "config_switches.toml"), "w", newline="\n") as f:
    f.write("# Switch tables, found by CodeBreaker (codebreaker/recompile/switches.py)\n\n")
    for bctr, labels in SWITCHES_BOTH.items():
        f.write("# absolute table, guard at 0x0\n[[switch]]\nbase = 0x%X\nr = 11\ndefault = 0x0\nlabels = [\n" % bctr)
        f.writelines("    0x%X,\n" % lab for lab in labels)
        f.write("]\n\n")

xex = os.path.join(HERE, "title.xex")


def make(name, **kw):
    out.append("[make_%s]" % name)
    text = config.make(xex, file_path="game.xex", **kw)
    text = text.replace("(codebreaker/recompile/config.py)", "(recompile/config)")   # (the C++ names itself)
    out.extend("| " + line for line in text.rstrip("\n").split("\n"))


make("beside", switch_tables="config_switches.toml")
make("no_file", switch_tables="no_such_switches.toml", out_dir="out\\ppc")
make("hooks", switch_tables="config_switches.toml", hooks="\n[[midasm_hook]]\nname = \"x\"\naddress = 0x82100000\n\n")

# XenonAnalyse's tables merged into the finder's (the finder is replaced by one table it "found")
from codebreaker.recompile import switches as sw  # noqa: E402

ANALYSED = ("[[switch]]\nbase = 0x%X\nr = 11\ndefault = 0x%X\nlabels = [0x%X, 0x%X]\n\n"      # (the bctr the finder has: skipped)
            "[[switch]]\nbase = 0x%X\nr = 12\nlabels = [0x%X, 0x%X, 0x%X]\n\n"                # (its bctr is the next one: added)
            "[[switch]]\nbase = 0x%X\nr = 3\nlabels = [0x%X]\n\n"                              # (no bctr after it: skipped)
            "[[switch]]\nbase = 0x%X\nr = 3\nlabels = [0x%X]\n\n"                              # (the same sequence again: its bctr is taken)
            % (V(0x500), V(0x540), V(0x510), V(0x520), V(0x110), V(0x150), V(0x160), V(0x1AC), V(0x7F0), V(0x7F8), V(0x108), V(0x150)))
with open(os.path.join(HERE, "config_analysed.toml"), "w", newline="\n") as f:
    f.write(ANALYSED)
found_tables = [{"bctr": V(0x504), "r": 11, "default": 0, "labels": [V(0x510), V(0x520), V(0x530)], "kind": "absolute", "table": 0,
                 "guard": V(0x500), "inline": False}]
sw.find = lambda path: (list(found_tables), None)
target = tempfile.mkdtemp(prefix="cbcfg")
counts = config.write_switch_tables(os.path.join(target, "s.toml"), "x.xex", ANALYSED)
out.append("[write_switch_tables]")
out.append("counts %d %d" % counts)
out.extend("| " + line for line in open(os.path.join(target, "s.toml"), newline="").read().rstrip("\n").split("\n"))
os.remove(os.path.join(target, "s.toml"))
os.rmdir(target)

with open(os.path.join(HERE, "config.golden.txt"), "w", newline="\n") as f:
    f.write("\n".join(out) + "\n")

# the switch-table readers: what tomllib reads
import tomllib  # noqa: E402

samples = {
    "finder": open(os.path.join(HERE, "config_switches.toml")).read(),
    "analyse": "[[switch]]\nbase = 0x82100504\nr = 11\ndefault = 0x82100540\nlabels = [0x82100510, 0x82100520]\n\n"
               "[[switch]]\nbase = 1_000\nr = 3\nlabels = [ 0x10 , 0b101,0o17,\n  42 # a comment\n , ]\n",
    "other": "title = \"x\"\n[main]\nfile = 'y'\nok = true\nf = 1.5\n[[switch]]\nbase = 16\nlabels = []\n[tail]\nz = [1, 2]\n",
}
for name, text in samples.items():
    t = tomllib.loads(text)
    print(name, [(s["base"], s.get("r"), s.get("default"), s["labels"]) for s in t.get("switch", [])])
print("golden written;", len(out), "lines")
