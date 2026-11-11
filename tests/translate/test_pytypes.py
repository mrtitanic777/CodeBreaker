"""The Python front end's interval types and struct-format reader.

pytypes: every integer operation's interval holds every concrete result (the property the inference leans on: an
interval that is too narrow lets a division by zero or an int64 overflow through as "proved safe"). structfmt: the formats
the front end accepts are the ones CPython's struct accepts.
"""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, ROOT)

from codebreaker.translate.frontends.python import pytypes as P        # noqa: E402
from codebreaker.translate.frontends.python import structfmt           # noqa: E402

RANGE = range(-4, 5)
INTERVALS = [(lo, hi) for lo in RANGE for hi in RANGE if lo <= hi]


def _holds(t, v):
    return (t.lo is None or t.lo <= v) and (t.hi is None or v <= t.hi)


def test_xor_interval_holds_every_result():
    # `ixor` was `ior`: for two non-negative operands its lower bound max(a.lo, b.lo) is wrong (5 ^ 5 is 0)
    for alo, ahi in INTERVALS:
        for blo, bhi in INTERVALS:
            r = P.ixor(P.INT(alo, ahi), P.INT(blo, bhi))
            for x in range(alo, ahi + 1):
                for y in range(blo, bhi + 1):
                    assert _holds(r, x ^ y), (alo, ahi, blo, bhi, x, y, r)


def test_xor_of_equal_values_may_be_zero():
    same = P.ixor(P.INT(5, 5), P.INT(5, 5))
    assert same.has_zero()          # a division by it is not proved safe
    assert P.ixor(P.INT(5, 7), P.INT(5, 7)) == P.INT(0, 7)
    assert P.ixor(P.INT(0, None), P.INT(0, 3)) == P.INT(0, None)
    assert P.ixor(P.INT(-4, 3), P.INT(0, 8)) == P.ior(P.INT(-4, 3), P.INT(0, 8))


def test_other_operations_hold_their_results():
    ops = [(P.iadd, lambda x, y: x + y), (P.isub, lambda x, y: x - y), (P.imul, lambda x, y: x * y),
           (P.iand, lambda x, y: x & y), (P.ior, lambda x, y: x | y),
           (P.ifloordiv, lambda x, y: x // y if y else None), (P.imod, lambda x, y: x % y if y else None)]
    for f, g in ops:
        for alo, ahi in INTERVALS:
            for blo, bhi in INTERVALS:
                r = f(P.INT(alo, ahi), P.INT(blo, bhi))
                for x in range(alo, ahi + 1):
                    for y in range(blo, bhi + 1):
                        v = g(x, y)
                        assert v is None or _holds(r, v), (f.__name__, alo, ahi, blo, bhi, x, y, r)


def test_structfmt_accepts_what_struct_accepts():
    assert structfmt.parse('') == []                       # was an IndexError
    assert structfmt.parse('<') == []
    assert structfmt.size(structfmt.parse('<HHI')) == 8
    assert len(structfmt.parse('<12H')) == 12
    assert len(structfmt.parse('<2H  3B')) == 5
    assert structfmt.parse('<4s') == [('s', 4, 'bytes', 4, 4)]
    assert structfmt.parse('<P') is None


def test_structfmt_refuses_what_struct_refuses():
    assert structfmt.parse('<4') is None                   # a count with no code was accepted
    assert structfmt.parse('<H3') is None
    assert structfmt.parse('<4 H') is None                 # CPython: no white space between a count and its code
    assert structfmt.parse('<H,H') is None
    assert structfmt.parse('HI') is None                   # native alignment pads between sized numbers
    assert structfmt.parse('=HI') is not None
