"""Writes powerpc_insns.golden.txt, powerpc_slice.golden.txt and powerpc_image.golden.txt: what codebreaker/recompile/powerpc.py
makes of single instructions (written out below), of the 16 KB of the Minecraft Xbox 360 Edition's code in ppc_slice.bin, and
of the small made program of config_code.bin (see gen_config_golden.py), as the reference the C++ (cb/recompile/powerpc.cpp)
is checked against. Run from the checkout root:
    python native/cb/recompile/testdata/gen_powerpc_golden.py

An instruction the Python cannot make anything of (an exception) is written "!EXC": the C++ makes a PPC_UNIMPLEMENTED trap of it
(the Python also wrote "None" into a statement for some of those, which its recompile_function turned into the same trap: here
that safety net is applied to each instruction too).
"""
import hashlib
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, "..", "..", "..", ".."))
from codebreaker.recompile import disassemble, discover, powerpc  # noqa: E402

LO, HI = 0x82100000, 0x82100080
TABLES = {0x82100020: (11, [0x82100030, 0x82100040, 0x82100090, 0x82100050])}


def twi(to, ra, imm):
    return (3 << 26) | (to << 21) | (ra << 16) | (imm & 0xFFFF)


# (address, raw word, mnemonic, operands): written the way Capstone writes them
INSNS = []


def add(va_off, mn, ops="", raw=0):
    INSNS.append((LO + va_off, raw, mn, ops))


# control flow
add(0x00, "b", "0x82100030")
add(0x04, "b", "0x82200000")
add(0x08, "ba", "0x82100010")
add(0x0C, "bl", "0x82300000")
add(0x10, "bla", "0x82100000")
add(0x14, "blr")
add(0x18, "blrl")
add(0x1C, "bctrl")
add(0x20, "bctr")
add(0x24, "bctr")
for i, mn in enumerate(["beq", "bne", "blt", "bge", "bgt", "ble", "bso", "bns", "bun", "bnu", "beq+", "bne-", "blt+"]):
    add(0x30 + 4 * i, mn, "0x82100040")
    add(0x30 + 4 * i, mn, "cr6, 0x82100040")
    add(0x30 + 4 * i, mn, "cr2, 0x82200040")
for mn in ["beqlr", "bnelr", "bltlr", "bgelr", "bgtlr", "blelr", "beqlr-", "bgtlr+"]:
    add(0x40, mn)
    add(0x40, mn, "cr3")
for mn in ["beqctr", "bnectr", "bltctr", "bgectr", "bgtctr", "blectr", "beqctr+"]:
    add(0x44, mn)
    add(0x44, mn, "cr5")
for mn in ["bdnz", "bdz", "bdnz+", "bdz-"]:
    add(0x48, mn, "0x82100020")
    add(0x48, mn, "0x82110020")
for mn in ["bdzlr", "bdnzlr", "bdzlr+"]:
    add(0x4C, mn)
for mn in ["bdnzf", "bdnzt", "bdzf", "bdzt", "bdnzf+"]:
    add(0x50, mn, "eq, 0x82100020")
    add(0x50, mn, "4*cr1+lt, 0x82200020")
    add(0x50, mn, "4*cr7+un, 0x82100020")
    add(0x50, mn, "lt gt, 0x82100020")
add(0x50, "bdnzt", "cr2eq, 0x82100020")
add(0x54, "b", "")
add(0x54, "bl", "")
add(0x54, "bctr+")
# atomics, and the record forms
add(0x58, "lwarx", "r3, 0, r4")
add(0x58, "lwarx", "r3, r5, r4")
add(0x58, "ldarx", "r3, r5, r4")
add(0x58, "stwcx.", "r3, 0, r4")
add(0x58, "stwcx.", "r3, r6, r4")
add(0x58, "stdcx.", "r3, r6, r4")
add(0x58, "lwarx", "r3, r5")
for mn, ops in [("add.", "r3, r4, r5"), ("slwi.", "r3, r4, 2"), ("andi.", "r3, r4, 5"), ("rlwinm.", "r3, r4, 2, 0, 29"),
                ("subf.", "r3, r4, r5"), ("srwi.", "r3, r4, 3"), ("or.", "r3, r4, r5"), ("fadd.", "f1, f2, f3"),
                ("vcmpequw.", "v1, v2, v3"), ("frobnicate.", "r3, r4"), ("addic.", "r3, r4, 1"), ("extsb.", "r3, r4"),
                ("neg.", "r3, r4"), ("sub.", "r3, r4, r5"), ("add.", "")]:
    add(0x5C, mn, ops)
# compares (Capstone leaves out an implicit cr0)
for mn, ops in [("cmplwi", "r10, 2"), ("cmpwi", "cr6, r3, -1"), ("cmpd", "r3, r4"), ("cmpldi", "cr1, r3, 0x10"), ("cmpw", "cr7, r3, r4"),
                ("cmplw", "r3, r4"), ("cmpwi", "")]:
    add(0x60, mn, ops)
# the module's own: hints, MSR, traps, shifts, a few conversions
for mn, ops in [("dcbi", "r0, r3"), ("icbi", "r0, r3"), ("isync", ""), ("ptesync", ""), ("tlbsync", ""), ("dcbz128", "r0, r3"),
                ("mtmsr", "r3, 1"), ("mtmsree", "r3"), ("trap", ""), ("twui", "r3, 5"), ("fctiw", "f1, f2"), ("fctiw", "f1"),
                ("vmr", "v1, v2"), ("vmr", "v1"), ("slwi", "r3, r4, 2"), ("srwi", "r3, r4, 3"), ("sldi", "r3, r4, 33"), ("srdi", "r3, r4, 5"),
                ("sub", "r3, r4, r5"), ("sub", "r3, r4"), ("slwi", "r3, r4, x"), ("lvxl", "v1, r3, r4"), ("lvxl", "v1, 0, r4"),
                ("lvxl", "v1, r0, r4"), ("stvxl", "v1, r3, r4"), ("stvxl", "v200, r3, r4")]:
    add(0x64, mn, ops)
# what the builders make
for mn, ops in [("add", "r3, r4, r5"), ("addi", "r3, r4, 5"), ("addi", "r3, r0, -16"), ("addis", "r3, r4, 0x8210"), ("lwz", "r3, 8(r1)"),
                ("lwz", "r3, -0x70(r1)"), ("lwz", "r3, 0(0)"), ("lwz", "r3, r4"), ("stw", "r3, 8(r1)"), ("stwu", "r1, -0x60(r1)"),
                ("lbz", "r3, 0(r4)"), ("lwzx", "r3, r4, r5"), ("lwzx", "r3, 0, r5"), ("mflr", "r12"), ("mtlr", "r12"), ("mfctr", "r3"),
                ("mtctr", "r3"), ("li", "r3, -1"), ("lis", "r3, 0x8210"), ("mr", "r3, r4"), ("nop", ""), ("or", "r3, r4, r5"),
                ("ori", "r3, r4, 0x10"), ("rlwinm", "r3, r4, 2, 0, 29"), ("clrlwi", "r3, r4, 16"), ("extsw", "r3, r4"), ("cntlzw", "r3, r4"),
                ("fadd", "f1, f2, f3"), ("fmr", "f1, f2"), ("lfs", "f1, 0x10(r3)"), ("stfd", "f1, 8(r1)"), ("fcmpu", "cr6, f1, f2"),
                ("fcmpu", "f1, f2"), ("vaddubm", "v1, v2, v3"), ("vperm", "v1, v2, v3, v4"), ("vspltisw", "v1, -1"), ("lvx", "v1, r3, r4"),
                ("stvx", "v1, r3, r4"), ("vsel128", "v1, v2, v3"), ("sync", ""), ("eieio", ""), ("lwsync", ""), ("mfmsr", "r3"),
                ("subf", "r3, r4, r5"), ("mulli", "r3, r4, 10"), ("divw", "r3, r4, r5"), ("srawi", "r3, r4, 2"), ("rldicl", "r3, r4, 0, 32"),
                ("add", "r3, r4"), ("add", ""), ("add", "r3, r4, x9"), ("frobnicate", "r3, r4"), (".long", "0x00000000"), ("mfspr", "r3, 8"),
                ("dcbt", "r0, r3"), ("dcbz", "r0, r3"), ("stfiwx", "f0, 0, r11"), ("lwax", "r3, 0, r4"), ("addze", "r3, r4"), ("mfcr", "r3"),
                ("mtcrf", "0xff, r3"), ("crxor", "cr6lt, cr6lt, cr6lt"), ("fctiwz", "f1, f2"), ("fsel", "f1, f2, f3, f4"), ("fmadds", "f1, f2, f3, f4")]:
    add(0x68, mn, ops)
add(0x6C, "twi", "31, r3, 5", twi(31, 3, 5))
add(0x6C, "tw", "4, r3, r4", (31 << 26) | (4 << 21) | (3 << 16) | (4 << 11) | (4 << 1))


def make_text(rows, jump_tables):
    out = []
    for va, raw, mn, ops in rows:
        out.append("@ %08X %08X %s | %s" % (va, raw, mn, ops))
        try:
            stmts = powerpc.emit_insn(va, mn, ops, set(), LO, HI, raw, jump_tables)
        except Exception:
            out.append("> !EXC")
            continue
        if any("None" in s for s in stmts):
            stmts = ['PPC_UNIMPLEMENTED("%s %s");' % (mn, ops)]
        out.extend("> " + s for s in stmts)
    return out


lines = ["# made by gen_powerpc_golden.py; see there"]
lines += ["lo %08X" % LO, "hi %08X" % HI]
lines += ["table %08X %d %s" % (a, r, " ".join("%08X" % t for t in ts)) for a, (r, ts) in TABLES.items()]
lines += make_text(INSNS, TABLES)
with open(os.path.join(HERE, "powerpc_insns.golden.txt"), "w", newline="\n") as f:
    f.write("\n".join(lines) + "\n")
print(len(INSNS), "instructions")

# ---- the slice -----------------------------------------------------------------------------------------------------
SLICE_VA = 0x822B6E00
slice_code = open(os.path.join(HERE, "ppc_slice.bin"), "rb").read()
SLICE_HI = SLICE_VA + len(slice_code)
is_exec = lambda a: a & 3 == 0 and SLICE_VA <= a < SLICE_HI  # noqa: E731


def function_text(code, va, size, **kw):
    cpp, called, stats = powerpc.recompile_function(code, va, va, size, **kw)
    return cpp, called, stats


out = ["# made by gen_powerpc_golden.py; see there"]
cpp, called, stats = function_text(slice_code, SLICE_VA, len(slice_code), img=slice_code, img_base=SLICE_VA, is_exec=is_exec)
out.append("[whole_slice]")
out.append("sha1 " + hashlib.sha1(cpp.encode()).hexdigest())
out.append("lines %d" % cpp.count("\n"))
out.append("stats total=%d unimpl=%d translated=%d labels=%d" % (stats["total"], stats["unimpl"], stats["translated"], stats["labels"]))
out.append("called %d first=%s last=%s" % (len(called), "%08X" % min(called), "%08X" % max(called)))
cpp_nt, called_nt, stats_nt = function_text(slice_code, SLICE_VA, len(slice_code))     # no image: no jump tables
out.append("[whole_slice_no_image]")
out.append("sha1 " + hashlib.sha1(cpp_nt.encode()).hexdigest())
out.append("stats total=%d unimpl=%d translated=%d labels=%d" % (stats_nt["total"], stats_nt["unimpl"], stats_nt["translated"], stats_nt["labels"]))
# the instructions the slice has, by mnemonic, that the module translated by hand or not at all
rows = list(disassemble.listing(slice_code, SLICE_VA))
unimpl = {}
for line in cpp.split("\n"):
    if "PPC_UNIMPLEMENTED" in line:
        unimpl[line.strip()] = unimpl.get(line.strip(), 0) + 1
out.append("[unimplemented_in_slice]")
out.extend("%d %s" % (n, s) for s, n in sorted(unimpl.items()))
# a few whole functions, as text: the code between the blr that ends one and the next
WINDOWS = [(0x0000, 0x0060), (0x0200, 0x0290), (0x0980, 0x0A20)]
for lo, hi in WINDOWS:
    c, cl, st = function_text(slice_code[lo:hi], SLICE_VA + lo, hi - lo, img=slice_code, img_base=SLICE_VA, is_exec=is_exec)
    out.append("[window_%04X_%04X]" % (lo, hi))
    out.append("calls " + " ".join("%08X" % a for a in sorted(cl)))
    out.append("stats total=%d unimpl=%d translated=%d labels=%d" % (st["total"], st["unimpl"], st["translated"], st["labels"]))
    out.extend("| " + l for l in c.rstrip("\n").split("\n"))
with open(os.path.join(HERE, "powerpc_slice.golden.txt"), "w", newline="\n") as f:
    f.write("\n".join(out) + "\n")

# ---- an image: the made program of gen_config_golden.py ------------------------------------------------------------
CODE_VA = 0x82100000
prog = open(os.path.join(HERE, "config_code.bin"), "rb").read()
EXTENTS = {0x82100000: 0x40, 0x82100040: 0x20, 0x82100430: 0x40, 0x821004C0: 0x20, 0x82100600: 0x20}
IMPORTS = {0x82100700}


class FakeImage:
    code = (CODE_VA, CODE_VA + len(prog))
    entry = CODE_VA
    extents = {a: (n, 0, False) for a, n in EXTENTS.items()}
    imports = {a: "imp" for a in IMPORTS}
    sections = [(".text", CODE_VA, len(prog), 0x60000020)]

    def bytes_at(self, va, size):
        return prog[va - CODE_VA: va - CODE_VA + size] if CODE_VA <= va and va + size <= CODE_VA + len(prog) else b""


discover._image = lambda path: FakeImage()
disassemble.image = lambda path: (prog, CODE_VA, CODE_VA, [])

out = ["# made by gen_powerpc_golden.py; see there"]


def image_case(name, **kw):
    body, mapping, agg = powerpc.recompile_image("fake", jobs=1, **kw)
    out.append("[%s]" % name)
    out.append("agg funcs=%d total=%d translated=%d unimpl=%d discovered=%d tail=%d lo=%08X hi=%08X" % (
        agg["funcs"], agg["total"], agg["translated"], agg["unimpl"], agg["discovered"], agg["tail entries"], agg["lo"], agg["hi"]))
    out.append("decls " + " ".join("%08X" % a for a in agg["decls"]))
    out.append("chosen " + " ".join("%08X" % a for a in agg["chosen"]))
    out.append("skipped " + " ".join("%08X" % a for a in agg["skipped"]))
    out.append("[%s.body]" % name)
    out.extend("| " + l for l in body.split("\n"))
    out.append("[%s.mapping]" % name)
    out.extend("| " + l for l in mapping.split("\n"))


image_case("image_default")
image_case("image_skip", skip={0x82100700, 0x82100100})
image_case("image_limit", limit=2)
image_case("image_seeds", extra=[0x82100300, 0x82100320, 0x82100000])
with open(os.path.join(HERE, "powerpc_image.golden.txt"), "w", newline="\n") as f:
    f.write("\n".join(out) + "\n")
print("done")
