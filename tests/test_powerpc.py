#!/usr/bin/env python3
"""The PowerPC interpreter, checked against C whose results are known.

    python tests/test_powerpc.py

Functions are compiled from C with the PowerPC compiler named in settings.json and run in
codebreaker/emulate/powerpc.py; what each returns is compared with the same computation done
here in Python. The interpreter is ours, with nothing to compare it against instruction by
instruction, so this is what stands between it and a wrong verdict. Skipped (not failed) when
no PowerPC compiler is set up."""
import math
import os
import random
import struct
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

HEADER = """typedef unsigned long long uint8; typedef long long int8; typedef unsigned int uint4; typedef int int4;
typedef unsigned short uint2; typedef short int2; typedef unsigned char uint1; typedef signed char int1;
"""
M32, M64 = 0xFFFFFFFF, 0xFFFFFFFFFFFFFFFF


def s32(v):
    v &= M32
    return v - (1 << 32) if v & 0x80000000 else v


def s64(v):
    v &= M64
    return v - (1 << 64) if v >> 63 else v


def cdiv(a, b):
    q = abs(a) // abs(b)
    return q if (a < 0) == (b < 0) else -q


# name: (C text, reference(a, b) -> expected r3 as an unsigned 32- or 64-bit value, result bits)
CASES = {
    "arith": ("int4 arith(int4 a, int4 b) { return a * b + a - b * 3; }",
              lambda a, b: s32(a) * s32(b) + s32(a) - s32(b) * 3, 32),
    "divide": ("int4 divide(int4 a, int4 b) { if (b == 0) return 7; return a / b + a % b; }",
               lambda a, b: 7 if s32(b) == 0 else (0 if (s32(a) == -2**31 and s32(b) == -1) else
                                                   cdiv(s32(a), s32(b)) + (s32(a) - cdiv(s32(a), s32(b)) * s32(b))), 32),
    "udivide": ("uint4 udivide(uint4 a, uint4 b) { if (b == 0) return 9; return a / b + a % b; }",
                lambda a, b: 9 if b & M32 == 0 else (a & M32) // (b & M32) + (a & M32) % (b & M32), 32),
    "divconst": ("int4 divconst(int4 a, int4 b) { return a / 7 + b / -13 + (a >> 3) + (int4)((uint4)b >> 5); }",
                 lambda a, b: cdiv(s32(a), 7) + cdiv(s32(b), -13) + (s32(a) >> 3) + ((b & M32) >> 5), 32),
    "shifts": ("uint4 shifts(uint4 a, uint4 b) { uint4 n = b & 31; return (a << n) ^ (a >> n) ^ ((a << 7) | (a >> 25)); }",
               lambda a, b: ((a << (b & 31)) ^ ((a & M32) >> (b & 31)) ^ ((a << 7) | ((a & M32) >> 25))), 32),
    "wide": ("int8 wide(int4 a, int4 b) { int8 x = (int8)a * b; return x * x + ((int8)a << 20) - (x >> 3); }",
             lambda a, b: (s32(a) * s32(b)) ** 2 + (s32(a) << 20) - ((s32(a) * s32(b)) >> 3), 64),
    "uwide": ("uint8 uwide(uint4 a, uint4 b) { uint8 x = ((uint8)a << 32) | b; if (b == 0) return x; return x / b + x % (b | 1) + (x >> 9); }",
              lambda a, b: (((a & M32) << 32) | (b & M32)) if b & M32 == 0 else
              ((((a & M32) << 32) | (b & M32)) // (b & M32) + (((a & M32) << 32) | (b & M32)) % ((b & M32) | 1)
               + ((((a & M32) << 32) | (b & M32)) >> 9)), 64),
    "compare": ("int4 compare(int4 a, int4 b) { int4 r = 0; if (a < b) r |= 1; if ((uint4)a < (uint4)b) r |= 2; "
                "if (a == b) r |= 4; if (a >= 100 && b <= -5) r |= 8; if ((a & 0xff) > 0x7f) r |= 16; return r; }",
                lambda a, b: (1 if s32(a) < s32(b) else 0) | (2 if a & M32 < b & M32 else 0) | (4 if s32(a) == s32(b) else 0)
                | (8 if s32(a) >= 100 and s32(b) <= -5 else 0) | (16 if (a & 0xFF) > 0x7F else 0), 32),
    "narrow": ("int4 narrow(int4 a, int4 b) { int1 c = (int1)a; int2 s = (int2)b; uint1 u = (uint1)b; uint2 w = (uint2)a; "
               "return c * s + u - w; }",
               lambda a, b: ((a & 0xFF) - 256 if a & 0x80 else a & 0xFF) * ((b & 0xFFFF) - 65536 if b & 0x8000 else b & 0xFFFF)
               + (b & 0xFF) - (a & 0xFFFF), 32),
    "loop": ("uint4 loop(uint4 a, uint4 b) { uint4 i, h = a; for (i = 0; i < (b & 63); i++) h = h * 31 + i ^ (h >> 5); return h; }",
             None, 32),
    "choose": ("int4 choose(int4 a, int4 b) { switch (a & 7) { case 0: return b + 1; case 1: return b * 2; case 2: return b - 9; "
               "case 3: return b ^ 0x55; case 4: return -b; case 5: return b >> 2; case 6: return ~b; } return 1234; }",
               lambda a, b: [s32(b) + 1, s32(b) * 2, s32(b) - 9, s32(b) ^ 0x55, -s32(b), s32(b) >> 2, ~s32(b), 1234][a & 7], 32),
    "fib": ("int4 fib(int4 a, int4 b) { int4 n = a & 15; if (n < 2) return n + b; return fib(n - 1, b) + fib(n - 2, b); }",
            None, 32),
    "memory": ("int4 memory(int4 a, int4 b) { int4 t[16]; int4 i, s = 0; for (i = 0; i < 16; i++) t[i] = a + i * b; "
               "for (i = 15; i >= 0; i -= 3) s += t[i] ^ i; return s; }",
               lambda a, b: sum((s32(s32(a) + i * s32(b)) ^ i) for i in range(15, -1, -3)), 32),
    "real": ("int4 real(int4 a, int4 b) { double x = a, y = b; double z = x * 1.5 + y / 4.0 - x * y * 0.001; "
             "if (z > 1e9) z = 1e9; if (z < -1e9) z = -1e9; return (int4)z; }",
             lambda a, b: int(max(-1e9, min(1e9, s32(a) * 1.5 + s32(b) / 4.0 - float(s32(a)) * float(s32(b)) * 0.001))), 32),
    "single": ("int4 single(int4 a, int4 b) { float x = (float)(a & 0xFFFF), y = (float)(b & 0xFF) + 1.0f; "
               "float q = x / y; return (int4)(q * 16.0f); }",
               lambda a, b: int(struct.unpack(">f", struct.pack(">f", struct.unpack(">f", struct.pack(">f", float(a & 0xFFFF) / (float(b & 0xFF) + 1.0)))[0] * 16.0))[0]), 32),
    "store": ("int4 store(int4 a, int4 b) { uint1 *p = (uint1 *)0x60000100; uint2 *q = (uint2 *)0x60000200; "
              "*(int4 *)0x60000300 = a; p[0] = (uint1)b; p[1] = (uint1)(b >> 8); q[1] = (uint2)a; "
              "return *(int4 *)0x60000300 + p[1] + q[1] + *(uint1 *)0x60000303; }",
              lambda a, b: s32(a) + ((b >> 8) & 0xFF) + (a & 0xFFFF) + (a & 0xFF), 32),
    "calls": ("uint8 outside(); int4 calls(int4 a, int4 b) { return (int4)outside(a + 1, b, 3) + (int4)outside(b) * 2; }",
              None, 32),
}


def _loop(a, b):
    h = a & M32
    for i in range(b & 63):
        h = ((h * 31 + i) ^ (h >> 5)) & M32
    return h


def _fib(a, b):
    def f(n):
        return n + s32(b) if n < 2 else f(n - 1) + f(n - 2)
    return f(a & 15)


CASES["loop"] = (CASES["loop"][0], _loop, 32)
CASES["fib"] = (CASES["fib"][0], _fib, 32)
CASES["calls"] = (CASES["calls"][0], lambda a, b: 0x1111 + 0x1111 * 2, 32)


def main():
    from codebreaker.build import functions
    from codebreaker.emulate import powerpc
    from codebreaker.formats import powerpc_image
    if not functions.powerpc_tools():
        print("  SKIP no PowerPC compiler in settings.json")
        print("0 passed, 1 skipped, 0 failed")
        return 0
    work = tempfile.mkdtemp(prefix="cb_powerpc_")
    order = list(CASES)
    dll, where, dropped = functions.build_powerpc(HEADER, {n: CASES[n][0] for n in order}, order, ["outside"],
                                                  work, 0x93000000)
    passed = failed = 0
    if not dll or dropped:
        print("  FAIL build: %s" % (dropped or "no image"))
        print("0 passed, 0 skipped, 1 failed")
        return 1
    regions = powerpc_image.regions(dll)
    externs = {where["outside"]: "outside"}
    rng = random.Random(7)
    inputs = [(0, 0), (1, 1), (5, 7), (-1, 1), (1, -1), (-7, -3), (0x7FFFFFFF, 2), (-0x80000000, -1), (123456789, 1000),
              (0x80, 0x8000), (0xFFFF, 0xFF), (100, -5), (-100, 13)] + \
             [(rng.randrange(-2**31, 2**31), rng.randrange(-2**31, 2**31)) for _ in range(40)] + \
             [(rng.randrange(-300, 300), rng.randrange(-300, 300)) for _ in range(20)]
    for name in order:
        _text, ref, bits = CASES[name]
        mask = M32 if bits == 32 else M64
        bad = None
        for a, b in inputs:
            if name == "divide" and (a, b) == (-0x80000000, -1):
                continue                                     # no defined result in C; the compiler traps
            r = powerpc.run(regions, where[name], [a & M64, b & M64], externs=externs, stub_ret=0x1111, steps=400000)
            want = ref(a, b) & mask
            got = r["ret"] & mask
            if r["exit"] != "ret" or got != want or not r["saved"]:
                bad = "inputs (%d, %d): %s, got %x, want %x%s" % (a, b, r["exit"], got, want,
                                                                 "" if r["saved"] else ", saved registers changed")
                break
        if name == "calls" and not bad:
            r = powerpc.run(regions, where[name], [4, 9], externs=externs, stub_ret=0x1111)
            got = [(n, [x & M32 for x in args[:3]]) for n, args in r["calls"]]
            if [g[0] for g in got] != ["outside", "outside"] or got[0][1] != [5, 9, 3] or got[1][1][0] != 9:
                bad = "calls recorded as %s" % got
        if name == "store" and not bad:
            r = powerpc.run(regions, where[name], [0x11223344, 0xAABB], externs=externs)
            w = r["writes"]
            if not (w.get(0x60000300, (0, 0))[1] == 0x11 and w.get(0x60000303, (0, 0))[1] == 0x44 and
                    w.get(0x60000100, (0, 0))[1] == 0xBB and w.get(0x60000202, (0, 0))[1] == 0x33):
                bad = "memory writes recorded wrongly"
        print("  %s %s%s" % ("FAIL" if bad else "ok  ", name, "  " + bad if bad else ""))
        failed += bool(bad)
        passed += not bad
    assert math.isfinite(1.0)
    print("%d passed, 0 skipped, %d failed" % (passed, failed))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
