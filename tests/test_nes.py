#!/usr/bin/env python3
"""The NES platform end to end: the cartridge (formats/nes.py), the 6502 (decompile/m6502.py,
emulate/m6502.py, the SLEIGH language), source that builds back (source/nes_writer.py,
build/m6502_asm.py) and the lane's judge (verify/lanes/nes.py).

    python tests/test_nes.py

The cartridge is built here from tests/data/nes/demo.s, a small program written by hand. The
checks against the SLEIGH language need native/decompiler/build/bin (pcodecheck, decompiler);
without them those are SKIPPED."""
import os
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker import platforms                      # noqa: E402
from codebreaker.build import m6502_asm                # noqa: E402
from codebreaker.decompile import m6502                # noqa: E402
from codebreaker.emulate import m6502 as cpu           # noqa: E402
from codebreaker.formats import nes                    # noqa: E402
from codebreaker.source import nes_writer              # noqa: E402

DATA = os.path.join(ROOT, "tests", "data", "nes")
BIN = os.path.join(ROOT, "native", "decompiler", "build", "bin")
RESULTS = []


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


_CART = []


def cartridge():
    if not _CART:
        image = m6502_asm.build(open(os.path.join(DATA, "demo.s")).read(),
                                open(os.path.join(DATA, "demo.cfg")).read(), DATA)
        path = os.path.join(tempfile.mkdtemp(prefix="cb_nes_"), "demo.nes")
        open(path, "wb").write(image)
        _CART.append(path)
    return _CART[0]


def test_cartridge():
    rom = nes.read(cartridge())
    assert (rom.mapper, rom.mirroring, len(rom.prg), len(rom.chr)) == (0, "vertical", 0x4000, 0x2000)
    assert rom.vectors == {"nmi": 0xC05C, "reset": 0xC000, "irq": 0xC076}
    assert rom.functions() == [0xC000, 0xC04F, 0xC05C, 0xC076]
    mem = rom.memory()
    assert mem[0x8000:0xC000] == mem[0xC000:0x10000] == rom.prg          # 16 KB: mirrored
    assert platforms.identify(cartridge()).name == "nes"


def test_table_round_trip():
    assert len(m6502.OPCODES) == 151
    for op, (m, mode) in m6502.OPCODES.items():
        code = bytes([op, 0x34, 0x92])
        i = m6502.decode(code, 0, 0x8000)
        assert m6502.assemble(m6502.text(i), 0x8000) == code[:i.size], (m, mode)


def test_source_rebuilds():
    folder = tempfile.mkdtemp(prefix="cb_nes_src_")
    r = nes_writer.reconstruct(cartridge(), folder)
    assert r["same"], r["notes"]
    text = open(r["source"]).read()
    for want in ("reset:", "nmi:", "JSR sub_C04F", "BIT PPUSTATUS", "STA PPUADDR", "LDA D_C077,x",
                 ".word nmi, reset, irq"):
        assert want in text, want


def test_interpreter():
    rom = nes.read(cartridge())
    mem = rom.memory()
    mem[0x10], mem[0x11] = 6, 7
    run = cpu.run(mem, 0xC04F)                                    # multiply
    assert run["end"] == "return" and run["a"] == 42
    bus = cpu.Bus(rom.memory())
    bus.map(0x2000, 0x401F, read=lambda a: 0x80)                    # in vblank at once
    run = cpu.run(bus, 0xC000, steps=4000)                         # reset: ends in its loop
    writes = [(a, v) for k, a, v in bus.log if k == "w"]
    assert writes[:2] == [(0x2000, 0), (0x2001, 0)]
    assert (0x2006, 0x3F) in writes and [v for a, v in writes if a == 0x2007] == [0x0F, 0x16, 0x27, 0x30]
    assert bus.mem[0x12] == 42 and run["end"] == "limit"


def test_flags_documented():
    """Documented cases a 6502 must get right (the ones the stock SLEIGH got wrong)."""
    def one(code, **st):
        mem = bytearray(0x10000)
        mem[0x400:0x400 + len(code)] = code
        c = cpu.CPU(mem)
        c.pc = 0x400
        for k, v in st.items():
            setattr(c, k, v)
        c.step()
        return c, mem
    c, _ = one(bytes([0x69, 0x01]), a=0x7F, p=0x20)               # ADC: signed overflow
    assert c.a == 0x80 and c.flags["V"] == 1 and c.flags["C"] == 0
    c, _ = one(bytes([0x69, 0x00]), a=0xFF, p=0x21)               # ADC: the carry in carries out
    assert c.a == 0 and c.flags["C"] == 1 and c.flags["Z"] == 1
    c, _ = one(bytes([0xE9, 0x03]), a=0x05, p=0x21)               # SBC: C is "no borrow"
    assert c.a == 2 and c.flags["C"] == 1
    c, mem = one(bytes([0x20, 0x00, 0x90]), s=0xFF)               # JSR pushes its last byte's address
    assert c.pc == 0x9000 and (mem[0x1FF], mem[0x1FE]) == (0x04, 0x02)
    c, _ = one(bytes([0x6C, 0xFF, 0x30]))                          # JMP ($30FF): the page wraps
    assert c.pc == 0x0000


def _have(tool):
    path = os.path.join(BIN, tool + (".exe" if os.name == "nt" else ""))
    if not os.path.isfile(path):
        raise Skip("%s not built" % tool)
    return path


def test_sleigh_agrees():
    _have("pcodecheck")
    r = subprocess.run([sys.executable, os.path.join(ROOT, "tools", "sleigh_verify_6502.py"), "--states", "8"],
                       capture_output=True, text=True)
    assert r.returncode == 0, r.stdout[-600:]


def test_decompiles():
    _have("decompiler")
    from codebreaker.decompile import decompiler
    text, done, total = decompiler.as_c(cartridge())
    assert (done, total) == (4, 4)
    assert "RAM2006 = 0x3f;" in text and "RAM2006 = 0;" in text          # both writes: volatile
    assert "cVar1 = cVar1 + cRAM0010;" in text or "+ cRAM0010" in text      # the multiply's loop


def test_lane_judges():
    from codebreaker.verify.lanes import nes as lane
    work = tempfile.mkdtemp(prefix="cb_nes_lane_")
    m = lane.prepare(cartridge(), work)
    ctx, items, dropped = lane.build(m, cartridge(), work)
    assert not dropped and set(items) == {"reset", "sub_C04F", "nmi", "irq"}
    assert all(lane.judge(ctx, items[n])[0] == "equivalent" for n in items)
    # an edit that changes what multiply does: ADC becomes EOR (the same length)
    text = open(m.written["source"]).read().replace("ADC $10", "EOR $10")
    built = m6502_asm.build(text, open(m.written["config"]).read(), os.path.dirname(m.written["source"]))
    edited = os.path.join(work, "edited.nes")
    open(edited, "wb").write(built)
    ctx2 = (False, cartridge(), edited)
    assert lane.judge(ctx2, items["sub_C04F"])[0] == "divergent"
    assert lane.judge(ctx2, items["irq"])[0] == "equivalent"


def main():
    for fn in (test_cartridge, test_table_round_trip, test_source_rebuilds, test_interpreter,
               test_flags_documented, test_sleigh_agrees, test_decompiles, test_lane_judges):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "ok"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
