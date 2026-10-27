#!/usr/bin/env python3
"""The 68000 and the Genesis / Mega Drive end to end: the instruction table (decompile/m68k.py)
round trips through its assembler, the interpreter (emulate/m68k.py) computes and sets flags as
the CPU does, source written from a cartridge builds back into it (source/genesis_writer.py,
build/m68k_asm.py), and the lane's judge (verify/lanes/genesis.py) tells an edited function from the
others.

    python tests/test_m68k.py

The cartridge is built here from tests/data/genesis/demo.asm, a small program written by hand."""
import os
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker import platforms                      # noqa: E402
from codebreaker.build import m68k_asm                 # noqa: E402
from codebreaker.decompile import m68k                 # noqa: E402
from codebreaker.emulate import m68k as cpu            # noqa: E402
from codebreaker.source import genesis_writer          # noqa: E402

DATA = os.path.join(ROOT, "tests", "data", "genesis")
RESULTS = []


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "ok", ""))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


_CART = []


def cartridge():
    if not _CART:
        path = os.path.join(tempfile.mkdtemp(prefix="cb_m68k_"), "demo.md")
        with open(path, "wb") as f:
            f.write(m68k_asm.build(open(os.path.join(DATA, "demo.asm")).read()))
        _CART.append(path)
    return _CART[0]


def test_table_round_trip():
    """Every opcode word that decodes, with two patterns of extension words: decoded, written,
    assembled back to the same bytes (every 16th word, to keep the test quick)."""
    bad, n = [], 0
    for w in range(0, 0x10000, 16):
        for ext in (bytes.fromhex("00121234567800ab"), bytes.fromhex("fffe8000000000ff")):
            code = bytes([w >> 8, w & 0xFF]) + ext
            insn = m68k.decode(code, 0, 0x1000)
            if insn is None:
                continue
            n += 1
            text = m68k.text(insn)
            built = m68k.assemble(text, 0x1000)
            if built != code[:insn.length]:
                again = m68k.decode(built + bytes(8), 0, 0x1000)
                if again is None or m68k.text(again) != text:
                    bad.append((code[:insn.length].hex(), text, built.hex()))
    assert n > 5000 and not bad, bad[:5]
    assert m68k.text(m68k.decode(bytes.fromhex("48e7fffe"), 0, 0)) == "MOVEM.L D0-D7/A0-A6,-(SP)"
    assert m68k.decode(bytes.fromhex("a000"), 0, 0) is None                  # line A is no instruction


def _run(lines, **kw):
    bus = cpu.Bus()
    pc = 0x200
    for line in lines + ["RTS"]:
        code = m68k.assemble(line, pc)
        bus.mem[pc:pc + len(code)] = code
        pc += len(code)
    return cpu.run(bus, 0x200, **kw), bus


def test_flags():
    X, N, Z, V, C = 0x10, 0x08, 0x04, 0x02, 0x01
    r, _ = _run(["ADDI.W #$1,D0"], d=[0x7FFF] + [0] * 7)
    assert r["d"][0] == 0x8000 and r["sr"] & 0x1F == N | V
    r, _ = _run(["SUBQ.B #1,D0"], d=[0] * 8)
    assert r["d"][0] == 0xFF and r["sr"] & 0x1F == X | N | C
    r, _ = _run(["CMP.L D1,D0"], d=[5, 5] + [0] * 6)
    assert r["sr"] & 0x1F == Z
    r, _ = _run(["LSL.W #1,D0"], d=[0x8001] + [0] * 7)
    assert r["d"][0] == 0x0002 and r["sr"] & 0x1F == X | C
    r, _ = _run(["DIVU.W D1,D0"], d=[45, 7] + [0] * 6)
    assert r["d"][0] == 0x00030006
    r, _ = _run(["MULS.W D1,D0"], d=[0xFFFF, 3] + [0] * 6)
    assert r["d"][0] == 0xFFFFFFFD and r["sr"] & N


def test_platform_recognises():
    assert platforms.identify(cartridge()).name == "genesis"


def test_source_rebuilds():
    r = genesis_writer.reconstruct(cartridge(), tempfile.mkdtemp(prefix="cb_gen_"))
    assert r["same"], r["notes"]
    text = open(r["source"]).read()
    assert "dc.l vblank" in text and "LEA (VDP_CTRL).L,A1" in text and 'dc.b "SEGA MEGA DRIVE "' in text


def test_lane_judges():
    lane = platforms.named("genesis").lane_module()
    work = tempfile.mkdtemp(prefix="cb_gen_")
    m = lane.prepare(cartridge(), work)
    ctx, items, missing = lane.build(m, cartridge(), work)
    assert not missing and ctx[0]
    text = open(m.written["source"]).read()
    edited = os.path.join(work, "edited.md")
    with open(edited, "wb") as f:
        f.write(m68k_asm.build(text.replace("        MOVEQ #0,D0\n", "        MOVEQ #1,D0\n", 1)))
    ctx2 = (False, cartridge(), edited)
    sum_ = next(n for n in items if n.startswith("sub_"))
    assert lane.judge(ctx2, items[sum_])[0] == "divergent"
    assert lane.judge(ctx2, items["vblank"])[0] == "equivalent"


def main():
    for fn in (test_table_round_trip, test_flags, test_platform_recognises, test_source_rebuilds, test_lane_judges):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d failed" % (sum(1 for r in RESULTS if r[1] == "ok"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
