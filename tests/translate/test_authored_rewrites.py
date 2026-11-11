"""The rewrites of verify/authored (search16, shape) on C text that has an octal constant (a leading 0).

int(text, 0) does not read "012", and these functions called it on whatever constant a comparison or a switch chain held, so one
octal constant in a function ended the whole search (or repair) with a ValueError instead of leaving that place alone.
"""
import os
import random
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, ROOT)

from codebreaker.verify.authored import search16, shape       # noqa: E402


def test_a_comparison_with_an_octal_constant_is_left_alone():
    assert search16.rewrite("f()\n{\n    if (x <= 010) {\n    }\n}", random.Random(1)) is None
    assert search16.rewrite("f()\n{\n    if ((010 < x)) {\n    }\n}", random.Random(1)) is None
    # the other constants of the function can still be rewritten, and zero written 00 is zero
    out = search16.rewrite("f()\n{\n    if (x <= 010) {\n    }\n    if (y <= 0x19) {\n    }\n}", random.Random(1))
    assert out == "f()\n{\n    if (x <= 010) {\n    }\n    if (y < 0x1a) {\n    }\n}"
    assert search16.rewrite("f()\n{\n    if (x <= 00) {\n    }\n}", random.Random(1)) == "f()\n{\n    if (x < 0x1) {\n    }\n}"


def test_a_switch_chain_with_an_octal_constant_is_left_alone():
    body = "\n    if (x == 012) { a(); } else if (x == 2) { b(); }\n}"
    assert list(shape.switches(body, body)) == []
    both = body + "\n    if (v == 5) { a(); } else if (v == 6) { b(); }"
    assert len(list(shape.switches(both, both))) == 1
