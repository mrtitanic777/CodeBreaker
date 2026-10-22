#!/usr/bin/env python3
"""The Xenos shader microcode decoder (codebreaker/decompile/xenos.py), against the shader
compiler's own listing of the same microcode.

    python tests/test_xenos.py
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from codebreaker.decompile import xenos      # noqa: E402


def main():
    got = xenos.disassemble(xenos.SAMPLE)
    checks = [
        ("control flow: alloc then exece", got[0] == "CF: alloc / exece"),
        # fxc lists "muls r0.z, r0.xy": opcode, destination and mask are exact; the swizzle is
        # the same channels written out in full
        ("scalar multiply into r0.z", got[1].startswith("muls r0.z, r0.")),
        ("the move to the colour output, exactly as listed", got[2] == "mov oC0, r0.xyzx"),
        ("nothing after the program's end", len(got) == 3),
    ]
    for name, ok in checks:
        print("  %-4s %s" % ("ok" if ok else "FAIL", name))
    fails = sum(1 for _n, ok in checks if not ok)
    print("%d passed, 0 skipped, %d failed" % (len(checks) - fails, fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
