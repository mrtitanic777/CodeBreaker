"""exact/habits/cmpp.py: the instruction-level view the comparers share (the C++ twin is native/cb/exact/habits/cmpp.cpp)."""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.exact.habits import cmpp    # noqa: E402


def test_the_entry_datas_first_byte_is_no_opcode():
    """A procedure's call of the entry helper is followed by a byte and a word of frame data. loose_bytes read that byte as
    an opcode: with it 0xE8 the two bytes after it counted as a call's displacement and were left out of the comparison,
    which hid a difference in the frame size."""
    cmpp.HDRLEN = 3
    proc = bytes([0xE8, 0x00, 0x00, 0xE8, 0x34, 0x12, 0xC3])
    assert cmpp.loose_bytes(proc) == set()
    assert [r[3] for r in cmpp.walk(proc)] == ["call", "hdr", "ret"]
    # a real call to outside the procedure is still loose
    assert cmpp.loose_bytes(bytes([0xE8, 0x00, 0x00, 0x01, 0x02, 0x03, 0xE8, 0x40, 0x00, 0xC3])) == {7, 8}
