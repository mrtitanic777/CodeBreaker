#!/usr/bin/env python3
"""The platform descriptions (codebreaker/platforms): every stage's table comes from them, and
they say what the tables said before they existed.

    python tests/test_platforms.py
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from codebreaker import platforms         # noqa: E402


def main():
    from codebreaker import emulate
    from codebreaker.base import corpus
    from codebreaker.decompile import describe
    from codebreaker.verify import reconstruct
    checks = [
        ("every lane", reconstruct.KINDS == {
            "pe": "codebreaker.verify.win32", "ne": "codebreaker.verify.win16",
            "powerpc": "codebreaker.verify.powerpc", "le": "codebreaker.verify.driver",
            "dos": "codebreaker.verify.dos", "java": "codebreaker.verify.java", "nes": "codebreaker.verify.nes",
            "sms": "codebreaker.verify.sms", "gb": "codebreaker.verify.gb", "genesis": "codebreaker.verify.genesis",
            "zx": "codebreaker.verify.zx", "msx": "codebreaker.verify.msx"}),
        ("every runner", emulate.RUNNERS == {
            "pe": ("codebreaker.emulate.emulator", "trace_function"), "ne": ("codebreaker.emulate.windows16", "probe"),
            "le": ("codebreaker.emulate.driver", "run"), "powerpc": ("codebreaker.emulate.powerpc", "run"),
            "nes": ("codebreaker.emulate.m6502", "run"), "sms": ("codebreaker.emulate.z80", "run"),
            "gb": ("codebreaker.emulate.sm83", "run"), "genesis": ("codebreaker.emulate.m68k", "run"),
            "zx": ("codebreaker.emulate.z80", "run"), "msx": ("codebreaker.emulate.z80", "run")}),
        ("languages", (describe._arch(0x14C), describe._arch(0x1F2), describe._arch(0x8664), describe.ARCH,
                       describe.LE_ARCH, describe.DOS_ARCH) == (
            "x86:LE:32:default:windows", "PowerPC:BE:64:Xenon:homed", None, "x86:LE:16:Protected Mode:default",
            "x86:LE:32:default:windows", "x86:LE:16:Real Mode:default")),
        ("names are unique", len({p.name for p in platforms.all()}) == len(platforms.all())),
        ("a file nothing recognises", platforms.identify(__file__) is None),
        ("a plain file reads as itself", corpus._readable(__file__) == __file__),
    ]
    for name, ok in checks:
        print("  %-4s %s" % ("ok" if ok else "FAIL", name))
    fails = sum(1 for _n, ok in checks if not ok)
    print("%d passed, 0 skipped, %d failed" % (len(checks) - fails, fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
