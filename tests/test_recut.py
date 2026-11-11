"""exact/recut.py and the FIXUPP reader it shares with exact/standin.py (the C++ twin is native/cb/exact/recut.cpp,
which is held to the same files: native/cb/exact/testdata/recut)."""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.exact import recut, standin    # noqa: E402

DATA = os.path.join(ROOT, "native", "cb", "exact", "testdata", "recut")
BASE = 0x100


def test_a_fixups_location_is_four_bits():
    """The FIXUPP location field is bits 2-5: a location of 8 or more (the 32-bit forms) was read as its low three
    bits, so a record rewritten from what was read named another kind of word."""
    # M=1, location 9 (a 32-bit offset), offset 0; frame method 0 index 1, target method 0 index 1, P set
    f, = standin.fixups(bytes([0x80 | 0x40 | (9 << 2), 0x00, 0x04, 0x01, 0x01]), {})
    assert f["loc"] == 9, f["loc"]
    f, = standin.fixups(bytes([0x80 | 0x40 | (2 << 2) | 1, 0x05, 0x04, 0x01, 0x01]), {})
    assert f["loc"] == 2 and f["off"] == 0x105


class _Project:
    header = None
    file = b""

    def __init__(self, relocs):
        self.packed = type("Packed", (), {"relocs": relocs})()

    def symbols(self):
        return {"_main": ("C", BASE)}


def _case(name):
    raw = open(os.path.join(DATA, name + ".obj"), "rb").read()
    relocs = [int(x, 16) for x in open(os.path.join(DATA, name + ".relocs")).read().split()]
    golden = open(os.path.join(DATA, name + ".golden.obj"), "rb").read()
    return raw, relocs, golden


def test_recut_cuts_where_the_relocations_run_up_again():
    raw, relocs, golden = _case("cut")
    got = recut.recut(_Project(relocs), raw)
    assert got == golden and got != raw


def test_recut_leaves_a_module_whose_relocations_are_not_the_programs():
    raw, relocs, golden = _case("foreign")
    assert recut.recut(_Project(relocs), raw) == raw == golden


def test_order_is_the_packed_ones_or_the_headers_table():
    class Headed:
        packed = None
        header = (0,) * 3 + (2,) + (0,) * 8 + (4,)
        file = bytes(4) + bytes([0x0C, 0, 0x10, 0, 0x0E, 0, 0x11, 0])

    assert recut.order(Headed()) == [0x10C, 0x11E]
    assert recut.order(_Project([5, 4])) == [5, 4]
