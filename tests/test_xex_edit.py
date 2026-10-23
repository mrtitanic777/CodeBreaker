#!/usr/bin/env python3
"""Changing an Xbox 360 executable (formats/xex_edit.py) and the PowerPC assembler it uses
(build/ppc_asm.py): every instruction form assembles to what Capstone decodes back, privilege bits
are set and cleared in a header built here, and -- when this machine has Minecraft's recompiled
copy (Converted/Minecraft) -- code written at its entry point reads back from the new module's
image, as does a hook through a code cave.

    python tests/test_xex_edit.py"""
import os
import struct
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.build import ppc_asm            # noqa: E402
from codebreaker.formats import xex, xex_edit    # noqa: E402

GAME = os.path.join(ROOT, "Converted", "Minecraft", "original", "default.xex")
RESULTS = []

# (source, what Capstone prints: mnemonic and operands, spaces removed)
CASES = [
    ("nop", "nop"), ("blr", "blr"), ("bctr", "bctr"), ("li r3, 1", "lir3,1"), ("lis r4, 0x8200", "lisr4,-0x7e00"),
    ("mr r31, r3", "mrr31,r3"), ("mflr r12", "mflrr12"), ("mtlr r0", "mtlrr0"), ("mtctr r11", "mtctrr11"),
    ("addi r1, r1, -0x60", "addir1,r1,-0x60"), ("ori r3, r3, 0x1234", "orir3,r3,0x1234"),
    ("lwz r3, 0x10(r31)", "lwzr3,0x10(r31)"), ("stw r0, -8(r1)", "stwr0,-8(r1)"),
    ("stwu r1, -0x60(r1)", "stwur1,-0x60(r1)"), ("lbz r5, 0(r4)", "lbzr5,0(r4)"),
    ("add r3, r4, r5", "addr3,r4,r5"), ("and. r3, r3, r4", "and.r3,r3,r4"), ("cmpwi r3, 0", "cmpwir3,0"),
    ("cmplw r3, r4", "cmplwr3,r4"), ("neg r3, r4", "negr3,r4"), ("extsh r3, r4", "extshr3,r4"),
]


class Skip(Exception):
    pass


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "ok", ""))
    except Skip as s:
        RESULTS.append((fn.__name__, "SKIP", str(s)))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


def _decode(blob, address):
    try:
        import capstone
    except ImportError:
        raise Skip("capstone is not installed")
    md = capstone.Cs(capstone.CS_ARCH_PPC, capstone.CS_MODE_32 | capstone.CS_MODE_BIG_ENDIAN)
    return [(i.mnemonic + i.op_str).replace(" ", "") for i in md.disasm(blob, address)]


def test_assembler_against_capstone():
    for source, expect in CASES:
        got = _decode(ppc_asm.assemble(source, 0x82000000), 0x82000000)
        assert got == [expect], (source, got, expect)


def test_branches():
    blob = ppc_asm.assemble("b 0x82001000\nbl 0x82000000\nbeq 0x82000010\nbne 0x81FFFFF0", 0x82000000)
    assert _decode(blob, 0x82000000) == ["b0x82001000", "bl0x82000000", "beq0x82000010", "bne0x81fffff0"]
    labels = ppc_asm.assemble("top:\nnop\nb top", 0x82000000)
    assert labels[4:] == struct.pack(">I", 0x4BFFFFFC)


def _module(privileges):
    """A header with one privilege entry (key 0x30000)."""
    data = bytearray(0x40)
    data[0:4] = b"XEX2"
    struct.pack_into(">IIIII", data, 4, 0, 0x40, 0, 0, 1)
    struct.pack_into(">II", data, 0x18, 0x00030000, privileges)
    return bytes(data)


def test_privileges():
    data = _module(0x00000604)
    assert xex_edit.privileges(data)[0] == [2, 9, 10]
    cleared = xex_edit.set_privileges(data, [xex_edit._parse_bit("-pal50"), xex_edit._parse_bit("+0")])
    assert xex_edit.privileges(cleared)[0] == [0, 2, 9]
    assert struct.unpack_from(">I", cleared, 0x1C)[0] == 0x205
    try:
        xex_edit.set_privileges(data, [(40, True)])          # no 0x30100 entry
    except ValueError:
        pass
    else:
        raise AssertionError("a bit past the header's entries was accepted")


def test_patch_and_hook_real_module():
    if not os.path.isfile(GAME):
        raise Skip("no Converted/Minecraft here")
    image = xex_edit.Image(GAME)
    entry = xex.header(GAME).entry
    original = image.read(entry, 8)
    image.write(entry, ppc_asm.assemble("li r3, 1\nblr", entry))
    assert image.read(entry, 8) == bytes.fromhex("386000014e800020")
    image.write(entry, original)
    cave = entry + 0x40
    if xex_edit._relative_branch(struct.unpack(">I", original[:4])[0]):
        raise Skip("the entry point starts with a relative branch")
    xex_edit.hook(image, entry, cave, "li r4, 7")
    assert _decode(image.read(entry, 4), entry) == ["b0x%x" % cave]
    assert image.read(cave + 4, 4) == original[:4]
    assert _decode(image.read(cave + 8, 4), cave + 8) == ["b0x%x" % (entry + 4)]
    fd, out = tempfile.mkstemp(suffix=".xex")
    os.close(fd)
    try:
        image.save(out)
        with open(xex.basefile(out), "rb") as f:
            base = f.read()
        assert base[entry - image.base:entry - image.base + 4] == image.read(entry, 4)
    finally:
        os.remove(out)


def main():
    for fn in (test_assembler_against_capstone, test_branches, test_privileges, test_patch_and_hook_real_module):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d skipped, %d failed" % (sum(1 for r in RESULTS if r[1] == "ok"),
                                                sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
