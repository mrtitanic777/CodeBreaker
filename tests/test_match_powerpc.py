#!/usr/bin/env python3
"""`match` and `repair` on PowerPC (codebreaker/verify/authored/match_powerpc.py): the object reader, its
bit-level masks, the comparison.

    python tests/test_match_powerpc.py

Compiles small functions with the Xbox 360 compiler settings.json names (powerpc_tools); SKIPPED
when there is none."""
import os
import struct
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.verify.authored import match_powerpc as M      # noqa: E402

RESULTS = []
SOURCE = """
extern int counter;
int helper(int);
int caller(int a, int b) {
    int i, s = 0;
    for (i = 0; i < a; i++) {
        if (i & 1) s += helper(i); else s -= b;
    }
    counter = s;
    return s * 3;
}
"""


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


def _built():
    from codebreaker.build import functions
    tools = functions.powerpc_tools()
    if not tools:
        raise Skip("no PowerPC compiler set up")
    work = tempfile.mkdtemp(prefix="cb_ppcmatch_")
    return M._compile(tools[0], "", SOURCE, "caller", work, "/O2")


def test_one_function_whole():
    code, mask = _built()
    words = [struct.unpack_from(">I", code, i)[0] for i in range(0, len(code), 4)]
    # it ends returning: blr, or a jump into the register-restore helper (the compiler's epilogue)
    assert words[-1] == 0x4E800020 or words[-1] >> 26 == 18 and not words[-1] & 1, hex(words[-1])
    assert len(mask) == len(words)
    assert len(words) > 10, "a label inside the function must not cut it (%d words)" % len(words)


def test_masks_are_fields():
    code, mask = _built()
    words = [struct.unpack_from(">I", code, i)[0] for i in range(0, len(code), 4)]
    calls = [i for i, w in enumerate(words) if w >> 26 == 18 and w & 1]
    assert calls and all(mask[i] == M._LI for i in calls), "a bl's distance, and only it"
    lis = [i for i, w in enumerate(words) if w >> 26 == 15 and (w >> 16) & 31 == 0]
    assert lis and all(mask[i] == M._HALF for i in lis), "the high half of the global's address"
    assert all(mask[i] == 0 for i, w in enumerate(words) if w >> 26 not in (15, 18) and not mask[i] in (M._HALF,)), \
        "nothing else is masked"


def test_distance():
    built = _built()
    code, mask = built
    assert M.distance(built, (code, [0] * len(mask)))[:2] == ("identical", 0)
    w = bytearray(code)
    calls = [i for i in range(0, len(code), 4) if struct.unpack_from(">I", code, i)[0] >> 26 == 18]
    w[calls[0] + 2] ^= 0x40                                  # another callee address: masked
    assert M.distance(built, (bytes(w), [0] * len(mask)))[0] == "identical"
    w[0] ^= 0x01                                             # another register: not masked
    assert M.distance(built, (bytes(w), [0] * len(mask)))[:2] == ("same size", 1)
    assert M.distance(built, (code[:-4], [0] * (len(mask) - 1)))[0] == "different"


def test_original_stops_at_padding():
    class Img:
        functions, imports, inline, code = [0x1000], {}, {}, (0x1000, 0x1040)
        extents = {0x1000: (0x40, 8, False)}

        def bytes_at(self, va, n):
            body = struct.pack(">4I", 0x7D8802A6, 0x48000011, 0x4E800020, 0)
            return (body + struct.pack(">4I", 0x38600000, 0x4E800020, 0, 0) + bytes(32))[:n]
    code, mask = M.original_functions(Img(), [0x1000])[0x1000]
    assert len(code) == 12 and mask[1] == M._LI                  # bl out of the function: masked


def main():
    for fn in (test_one_function_whole, test_masks_are_fields, test_distance, test_original_stops_at_padding):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "ok"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
