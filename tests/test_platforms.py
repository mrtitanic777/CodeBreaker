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


def _slots():
    """The one lane interface (docs/ARCHITECTURE.md part 4): every platform has the six slots;
    every one with a lane fills its loader and its packager, and its processor unless its code is
    not machine code; every Ref a slot names resolves to something that exists."""
    bad = []
    for p in platforms.all():
        if list(p.slots()) != list(platforms.SLOTS):
            bad.append((p.name, "slots"))
        if p.lane and not (p.loader.container and p.packager):
            bad.append((p.name, "loader or packager"))
        if p.lane and p.source != "java -> c++" and not str(p.cpu):
            bad.append((p.name, "cpu"))
        for slot, s in p.slots().items():
            for field, v in s.given().items():
                if isinstance(v, platforms.Ref):
                    try:
                        v.target()
                    except Exception as e:
                        bad.append((p.name, slot, field, repr(e)))
    return not bad or print(bad)


def _the_old_tables():
    """What the lanes and module.build had written for themselves is what the slots give."""
    from codebreaker.build import functions, module
    from codebreaker.decompile import c_text
    from codebreaker.emulate import emulator, windows16
    w16, w32 = platforms.of("ne"), platforms.of("pe")
    return (w16.compiler.options() == functions.CFLAGS16 and w32.compiler.options() == c_text.CFLAGS
            and w16.convention.arguments == 8 and w32.convention.arguments == 4
            and w16.packager.link.target() is module._build16 and w32.packager.link.target() is module._build32
            and w16.emulator.run.target() is windows16.recorded and w32.emulator.run.target() is emulator.recorded
            and platforms.of("dos").emulator.run is None
            and platforms.step_modules() == {"win16": "codebreaker.engine.win16"}
            and platforms.named("win16") is w16 and str(w16.cpu) == "x86:LE:16:Protected Mode:default")


def main():
    from codebreaker import emulate
    from codebreaker.base import corpus
    from codebreaker.decompile import describe
    from codebreaker.verify import reconstruct
    checks = [
        ("every lane", reconstruct.KINDS == {
            "pe": "codebreaker.verify.lanes.win32", "ne": "codebreaker.verify.lanes.win16",
            "powerpc": "codebreaker.verify.lanes.powerpc", "le": "codebreaker.verify.lanes.driver",
            "dos": "codebreaker.verify.lanes.dos", "java": "codebreaker.verify.lanes.java", "nes": "codebreaker.verify.lanes.nes",
            "sms": "codebreaker.verify.lanes.sms", "gb": "codebreaker.verify.lanes.gb", "genesis": "codebreaker.verify.lanes.genesis",
            "zx": "codebreaker.verify.lanes.zx", "msx": "codebreaker.verify.lanes.msx"}),
        ("every runner", emulate.RUNNERS == {
            "pe": ("codebreaker.emulate.emulator", "recorded"), "ne": ("codebreaker.emulate.windows16", "recorded"),
            "le": ("codebreaker.emulate.driver", "recorded"), "powerpc": ("codebreaker.emulate.powerpc", "recorded"),
            "nes": ("codebreaker.emulate.m6502", "recorded"), "sms": ("codebreaker.emulate.z80", "recorded"),
            "gb": ("codebreaker.emulate.sm83", "recorded"), "genesis": ("codebreaker.emulate.m68k", "recorded"),
            "zx": ("codebreaker.emulate.z80", "recorded"), "msx": ("codebreaker.emulate.z80", "recorded")}),
        ("languages", (describe._arch(0x14C), describe._arch(0x1F2), describe._arch(0x8664), describe.ARCH,
                       describe.LE_ARCH, describe.DOS_ARCH) == (
            "x86:LE:32:default:windows", "PowerPC:BE:64:Xenon:homed", None, "x86:LE:16:Protected Mode:default",
            "x86:LE:32:default:windows", "x86:LE:16:Real Mode:default")),
        ("names are unique", len({p.name for p in platforms.all()}) == len(platforms.all())),
        ("a file nothing recognises", platforms.identify(__file__) is None),
        ("a plain file reads as itself", corpus._readable(__file__) == __file__),
        ("reach: platforms, gaps and data told apart", _reach_kinds()),
        ("every platform fills the six slots", _slots()),
        ("the slots say what the lanes' own tables said", _the_old_tables()),
    ]
    for name, ok in checks:
        print("  %-4s %s" % ("ok" if ok else "FAIL", name))
    fails = sum(1 for _n, ok in checks if not ok)
    print("%d passed, 0 skipped, %d failed" % (len(checks) - fails, fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
