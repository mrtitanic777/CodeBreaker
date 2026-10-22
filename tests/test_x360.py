"""The Xbox 360 project lane: XenonRecomp configured by CodeBreaker, on the native/x360 runtime.

    python -m pytest tests/test_x360.py
    CB_SLOW_TESTS=1 python -m pytest tests/test_x360.py      # also Xenia's instruction tests (~15 min)

Uses Tetris's image from the absorbed projects' folder when it is there (skipped otherwise).
"""
import os
import struct
import sys
import tempfile

import pytest

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.recompile import config, discover, switches, xenon    # noqa: E402

XEX = os.path.join(ROOT, "Projects", "TetrisPC", "work", "port", "private", "default.xex")


def _need_game():
    if not os.path.isfile(XEX):
        pytest.skip("no Tetris XEX here")


def test_helpers_and_jump_buffers():
    """The save/restore runs (the eight XenonRecomp takes) and the CRT's setjmp/longjmp."""
    _need_game()
    found = config.helpers(discover._image(XEX))
    assert found == {
        "savegprlr_14_address": 0x82162820, "restgprlr_14_address": 0x82162870,
        "savefpr_14_address": 0x82162F40, "restfpr_14_address": 0x82162F8C,
        "savevmx_14_address": 0x821667E0, "restvmx_14_address": 0x82166A78,
        "savevmx_64_address": 0x82166874, "restvmx_64_address": 0x82166B0C,
        "setjmp_address": 0x821690E0, "longjmp_address": 0x82168FC0}, found


def test_switch_tables():
    """switches.py finds the halfword table XenonAnalyse misses (no bounds check right before it,
    the index shifted into r0), and no table's labels leave their function."""
    _need_game()
    found, report = switches.find(XEX)
    tables = {t["bctr"]: t for t in found}
    assert tables[0x8215C910]["kind"] == "short" and len(tables[0x8215C910]["labels"]) == 30
    assert not report["outside"]


def test_extra_functions_respect_switches():
    """A switch's labels are never function starts, and nothing inside a save/restore run is one."""
    _need_game()
    img = discover._image(XEX)
    found, _ = switches.find(XEX)
    tables = {t["bctr"]: t["labels"] for t in found}
    extra = dict(config.extra_functions(XEX, img, tables))
    labels = {l for ls in tables.values() for l in ls}
    assert not labels & set(extra)
    assert 0x82166A54 not in extra and 0x82166CEC not in extra     # entries of the vector runs


def test_tail_jump_targets_are_functions():
    """A branch out of a function is a tail call, so its target is declared a function: XACT's handler at
    0x823699C8 ends `b 0x823710C8`, which XenonRecomp otherwise had nothing to call for (and dropped)."""
    _need_game()
    img = discover._image(XEX)
    found, _ = switches.find(XEX)
    extra = dict(config.extra_functions(XEX, img, {t["bctr"]: t["labels"] for t in found}))
    assert 0x823710C8 in extra and 0x823735D0 in extra or 0x823735D0 in img.extents


def test_basic_xex():
    """basic(): the header kept, one data block holding the whole image."""
    _need_game()
    from codebreaker.formats import xex
    with tempfile.TemporaryDirectory() as d:
        out = xex.basic(XEX, os.path.join(d, "basic.xex"))
        data = open(out, "rb").read()
        original = open(XEX, "rb").read()
        pe_offset, = struct.unpack(">I", data[8:12])
        assert data[:4] == b"XEX2" and data[pe_offset:pe_offset + 2] == b"MZ"
        count, = struct.unpack(">I", data[0x14:0x18])
        info = next(v for k, v in (struct.unpack(">II", data[0x18 + i * 8:0x20 + i * 8]) for i in range(count))
                    if k == 0x3FF)
        _size, encryption, compression, data_size, zeros = struct.unpack(">IHHII", data[info:info + 16])
        assert (encryption, compression, zeros) == (0, 1, 0) and data_size == len(data) - pe_offset
        assert data[0x18:info] == original[0x18:info]


@pytest.mark.skipif(not os.environ.get("CB_SLOW_TESTS"), reason="slow: set CB_SLOW_TESTS=1")
def test_instruction_tests():
    """Xenia's PowerPC instruction tests through XenonRecomp: no mismatch."""
    try:
        result = xenon.instruction_tests()
    except RuntimeError as e:
        pytest.skip(str(e))
    assert result["tested"] >= 150 and not result["mismatches"], result["mismatches"][:10]
