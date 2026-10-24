#!/usr/bin/env python3
"""The platform descriptions (codebreaker/platforms): every stage's table comes from them, and
they say what the tables said before they existed.

    python tests/test_platforms.py
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from codebreaker import platforms         # noqa: E402


def _reach_kinds():
    """`reach` on a folder of an NES cartridge, a Game Boy Advance and a Nintendo 64 header (no
    lane yet: named as gaps) and a text file: each said for what it is, nothing judged."""
    import tempfile
    from codebreaker.coverage import reach
    with tempfile.TemporaryDirectory() as d:
        files = {"game.nes": b"NES\x1a\x01\x00" + b"\0" * 10 + b"\xea" * 0x4000,
                 "game.gba": b"\0\0\0\xea" + b"\0" * 0xAE + b"\x96" + b"\0" * 0x4D,
                 "game.z64": b"\x80\x37\x12\x40" + b"\0" * 0x3C,
                 "notes.txt": b"hello"}
        for name, data in files.items():
            with open(os.path.join(d, name), "wb") as f:
                f.write(data)
        got = {os.path.basename(r["path"]): (r["platform"], r["lane"]) for r in
               [dict(reach.describe(os.path.join(d, n)), path=n) for n in files]}
    return got == {"game.nes": ("NES", "nes"), "game.gba": ("Game Boy Advance (ARM7TDMI)", None),
                   "game.z64": ("Nintendo 64 (MIPS R4300)", None), "notes.txt": ("", None)}


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
        ("reach: platforms, gaps and data told apart", _reach_kinds()),
    ]
    for name, ok in checks:
        print("  %-4s %s" % ("ok" if ok else "FAIL", name))
    fails = sum(1 for _n, ok in checks if not ok)
    print("%d passed, 0 skipped, %d failed" % (len(checks) - fails, fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
