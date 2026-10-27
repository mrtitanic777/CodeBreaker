#!/usr/bin/env python3
"""The Z80 family end to end: the Master System / Game Gear (Z80) and the Game Boy (SM83). For
each: the instruction table (decompile/z80.py, decompile/sm83.py) round trips through its
assembler, the interpreter (emulate/z80.py, emulate/sm83.py) sets the flags the CPU sets, source
written from a cartridge builds back into it (source/sms_writer.py, source/gb_writer.py with
build/z80_asm.py, build/sm83_asm.py), and the lane's judge (verify/sms.py, verify/gb.py) tells an
edited function from the others.

    python tests/test_z80.py

The cartridges are built here from tests/data/sms/demo.asm and tests/data/gb/demo.asm, small
programs written by hand."""
import os
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker import platforms                      # noqa: E402
from codebreaker.build import sm83_asm, z80_asm        # noqa: E402
from codebreaker.decompile import sm83, z80            # noqa: E402
from codebreaker.emulate import sm83 as gbcpu          # noqa: E402
from codebreaker.emulate import z80 as zcpu            # noqa: E402
from codebreaker.source import gb_writer, sms_writer   # noqa: E402

DATA = os.path.join(ROOT, "tests", "data")
RESULTS = []


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "ok", ""))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


_CARTS = {}


def cartridge(kind):
    """The demo cartridge built from its source, written once to a temporary file."""
    if kind not in _CARTS:
        work = tempfile.mkdtemp(prefix="cb_z80_")
        if kind == "sms":
            image = z80_asm.build_file(os.path.join(DATA, "sms", "demo.asm"))
        else:
            image = sm83_asm.build(open(os.path.join(DATA, "gb", "demo.asm")).read(), 0x10000)
        path = os.path.join(work, "demo." + kind)
        with open(path, "wb") as f:
            f.write(image)
        _CARTS[kind] = path
    return _CARTS[kind]


def _round_trip(module, pages):
    bad = []
    for page in pages:
        for op in range(256):
            code = page(op)
            insn = module.decode(code, 0, 0x1000)
            if insn is None or insn.pattern == "NOP" and code[:1] in (b"\xDD", b"\xFD") or insn.pattern.startswith("."):
                continue
            text = module.text(insn)
            if text.startswith(("DB ", ".db ")):        # bytes written as data (a mirror encoding): the bytes themselves
                listed = bytes(int(v.strip()[1:], 16) for v in text.split(";")[0].split(" ", 1)[1].split(","))
                assert listed == bytes(code[:insn.size]), (text, code[:insn.size].hex())
                continue
            built = module.assemble(text, 0x1000)
            if built != bytes(code[:insn.size]):
                again = module.decode(built + bytes(4), 0, 0x1000)
                if again is None or module.text(again) != text:
                    bad.append((code[:insn.size].hex(), text, built.hex()))
    assert not bad, bad[:5]


def test_z80_table_round_trip():
    _round_trip(z80, [lambda op: bytes([op, 0x34, 0x12, 0x56]), lambda op: bytes([0xCB, op, 0, 0]),
                      lambda op: bytes([0xED, op, 0x34, 0x12]), lambda op: bytes([0xDD, op, 0x05, 0x12, 0x56]),
                      lambda op: bytes([0xFD, op, 0xFB, 0x12, 0x56]), lambda op: bytes([0xDD, 0xCB, 0x05, op]),
                      lambda op: bytes([0xFD, 0xCB, 0xFB, op])])
    assert z80.text(z80.decode(bytes.fromhex("dde9"), 0, 0)) == "JP (IX)"
    assert z80.text(z80.decode(bytes.fromhex("ddcb05c7"), 0, 0)) == "SET 0,(IX+$05),A"


def test_sm83_table_round_trip():
    _round_trip(sm83, [lambda op: bytes([op, 0x34, 0x12, 0x56]), lambda op: bytes([0xCB, op, 0, 0])])
    assert sm83.text(sm83.decode(bytes.fromhex("e044"), 0, 0)) == "LDH ($FF44),A"
    assert sm83.decode(bytes([0xD3]), 0, 0) is None              # one of the eleven missing opcodes


def _z80_flags(src, **regs):
    bus = zcpu.Bus()
    pc = 0x100
    for line in src:
        code = z80.assemble(line, pc)
        bus.mem[pc:pc + len(code)] = code
        pc += len(code)
    bus.mem[pc] = 0xC9                                          # RET
    r = zcpu.run(bus, 0x100, **regs)
    return r["a"], r["f"]


def test_z80_flags():
    S, Z, Y, H, X, P, N, C = 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
    assert _z80_flags(["ADD A,$01"], a=0x7F) == (0x80, S | H | P)               # signed overflow
    assert _z80_flags(["ADD A,$01"], a=0xFF) == (0x00, Z | H | C)
    assert _z80_flags(["SUB $01"], a=0x00) == (0xFF, S | Y | H | X | N | C)
    assert _z80_flags(["CP $28"], a=0x10) == (0x10, S | Y | H | X | N | C)       # Y and X from the operand
    assert _z80_flags(["XOR A"], a=0x55) == (0x00, Z | P)
    assert _z80_flags(["ADD A,$15", "DAA"], a=0x27)[0] == 0x42                   # decimal 27 + 15
    assert _z80_flags(["SCF", "CCF"], a=0)[1] & C == 0


def test_sm83_flags():
    Z, N, H, C = 0x80, 0x40, 0x20, 0x10
    bus = gbcpu.Bus()
    prog = ["ADD A,$01", "RET"]
    pc = 0x150
    for line in prog:
        code = sm83.assemble(line, pc)
        bus.mem[pc:pc + len(code)] = code
        pc += len(code)
    r = gbcpu.run(bus, 0x150, a=0xFF)
    assert (r["a"], r["f"]) == (0x00, Z | H | C)
    bus.mem[0x150:0x152] = sm83.assemble("SWAP A") + sm83.assemble("RET")
    assert gbcpu.run(bus, 0x150, a=0xF1)["a"] == 0x1F


def test_platforms_recognise():
    assert platforms.identify(cartridge("sms")).name == "sms"
    assert platforms.identify(cartridge("gb")).name == "gb"


def test_sms_source_rebuilds():
    r = sms_writer.reconstruct(cartridge("sms"), tempfile.mkdtemp(prefix="cb_sms_"))
    assert r["same"], r["notes"]
    text = open(r["source"]).read()
    assert "OUT (VDP_DATA),A" in text and "LD (MAPPER_SLOT2),A" in text and "LD HL,D_0139" in text


def test_gb_source_rebuilds():
    r = gb_writer.reconstruct(cartridge("gb"), tempfile.mkdtemp(prefix="cb_gb_"))
    assert r["same"], r["notes"]
    text = open(r["source"]).read()
    assert "LDH (rLCDC),A" in text and "LD (rROMB0),A" in text and "; title" in text


def _judges(kind, writer, assemble, before, after, function, other):
    lane = platforms.named(kind).lane_module()
    work = tempfile.mkdtemp(prefix="cb_%s_" % kind)
    m = lane.prepare(cartridge(kind), work)
    ctx, items, missing = lane.build(m, cartridge(kind), work)
    assert not missing and ctx[0], "the rebuilt cartridge should be the original"
    text = open(m.written["source"]).read()
    assert before in text
    edited = os.path.join(work, "edited." + kind)
    with open(edited, "wb") as f:
        f.write(assemble(text.replace(before, after, 1)))
    ctx2 = (False, cartridge(kind), edited)
    assert lane.judge(ctx2, items[function])[0] == "divergent"
    assert lane.judge(ctx2, items[other])[0] == "equivalent"


def test_sms_lane_judges():
    _judges("sms", sms_writer, z80_asm.build, "        INC A\n", "        DEC A\n", "sub_0117", "sub_011F")


def test_gb_lane_judges():
    _judges("gb", gb_writer, lambda text: sm83_asm.build(text, 0x10000), "        SWAP A\n", "        CPL\n",
            "sub_017A", "vblank")     # (entry calls sub_017A: it differs too)


def _tap_block(flag, payload):
    body = bytes([flag]) + payload
    x = 0
    for b in body:
        x ^= b
    body += bytes([x])
    return len(body).to_bytes(2, "little") + body


def spectrum(kind):
    """The Spectrum demo as a tape (a BASIC loader: 10 RANDOMIZE USR 32768, then the code block) or
    as a 48K .sna stopped at its start."""
    key = "zx" + kind
    if key not in _CARTS:
        at, code = z80_asm.build_flat(open(os.path.join(DATA, "zx", "demo.asm")).read())
        path = os.path.join(tempfile.mkdtemp(prefix="cb_zx_"), "demo." + kind)
        if kind == "tap":
            line = b"\xF9\xC0" + b"32768" + b"\x0E\x00\x00" + at.to_bytes(2, "little") + b"\x00" + b"\x0D"
            basic = (10).to_bytes(2, "big") + len(line).to_bytes(2, "little") + line
            head = lambda t, name, n, p1, p2: bytes([t]) + name.ljust(10).encode() + n.to_bytes(2, "little") + \
                p1.to_bytes(2, "little") + p2.to_bytes(2, "little")
            data = _tap_block(0, head(0, "demo", len(basic), 10, len(basic))) + _tap_block(0xFF, basic) + \
                _tap_block(0, head(3, "democode", len(code), at, 32768)) + _tap_block(0xFF, code)
        else:
            mem = bytearray(0x10000)
            mem[at:at + len(code)] = code
            sp = 0xFF00
            mem[sp:sp + 2] = at.to_bytes(2, "little")             # the PC, which RETN pops
            header = bytes([0x3F] + [0] * 14) + (0).to_bytes(2, "little") * 2 + bytes([0, 0, 0, 0]) + \
                sp.to_bytes(2, "little") + bytes([1, 7])
            data = header + bytes(mem[0x4000:])
        with open(path, "wb") as f:
            f.write(data)
        _CARTS[key] = path
    return _CARTS[key]


def msx_cartridge():
    if "msx" not in _CARTS:
        _at, rom = z80_asm.build_flat(open(os.path.join(DATA, "msx", "demo.asm")).read())
        path = os.path.join(tempfile.mkdtemp(prefix="cb_msx_"), "demo.rom")
        with open(path, "wb") as f:
            f.write(rom)
        _CARTS["msx"] = path
    return _CARTS["msx"]


def test_spectrum_lane():
    from codebreaker.formats import zx
    from codebreaker.source import zx_writer
    tape, snap = zx.read(spectrum("tap")), zx.read(spectrum("sna"))
    assert tape.origin == 0x8000 and tape.entries == {0x8000: "start"}
    assert snap.entries.get(0x8000) == "start" and snap.registers["sp"] == 0xFF02
    assert platforms.identify(spectrum("tap")).name == "zx"
    r = zx_writer.reconstruct(spectrum("tap"), tempfile.mkdtemp(prefix="cb_zx_"))
    assert r["same"], r["notes"]
    text = open(r["source"]).read()
    assert "CALL CHAN_OPEN" in text and "OUT (ULA),A" in text and "LD HL,FRAMES" in text and "RST $10" in text
    lane = platforms.named("zx").lane_module()
    work = tempfile.mkdtemp(prefix="cb_zx_")
    m = lane.prepare(spectrum("tap"), work)
    ctx, items, missing = lane.build(m, spectrum("tap"), work)
    assert ctx[0] and not missing
    _at, edited = z80_asm.build_flat(open(m.written["source"]).read().replace("LD A,$04", "LD A,$05", 1))
    ctx2 = (False, ctx[1], (ctx[1][0], edited))     # (each side: the program, and its bytes)
    assert lane.judge(ctx2, items["start"])[0] == "divergent"
    assert lane.judge(ctx2, items["sub_8017"])[0] == "equivalent"
    assert zx_writer.reconstruct(spectrum("sna"), tempfile.mkdtemp(prefix="cb_zx_"))["same"]


def test_msx_lane():
    from codebreaker.source import msx_writer
    assert platforms.identify(msx_cartridge()).name == "msx"
    r = msx_writer.reconstruct(msx_cartridge(), tempfile.mkdtemp(prefix="cb_msx_"))
    assert r["same"], r["notes"]
    text = open(r["source"]).read()
    assert "CALL INIT32" in text and "CALL LDIRVM" in text and "OUT (VDP_CTRL),A" in text and "LD HL,(JIFFY)" in text
    lane = platforms.named("msx").lane_module()
    work = tempfile.mkdtemp(prefix="cb_msx_")
    m = lane.prepare(msx_cartridge(), work)
    ctx, items, _missing = lane.build(m, msx_cartridge(), work)
    _at, edited = z80_asm.build_flat(open(m.written["source"]).read().replace("        INC HL\n", "        DEC HL\n", 1))
    ctx2 = (False, ctx[1], (ctx[1][0], edited))     # (each side: the program, and its bytes)
    tick = next(n for n in items if n.startswith("sub_"))
    assert lane.judge(ctx2, items[tick])[0] == "divergent"


def main():
    for fn in (test_z80_table_round_trip, test_sm83_table_round_trip, test_z80_flags, test_sm83_flags,
               test_platforms_recognise, test_sms_source_rebuilds, test_gb_source_rebuilds, test_sms_lane_judges,
               test_gb_lane_judges, test_spectrum_lane, test_msx_lane):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d failed" % (sum(1 for r in RESULTS if r[1] == "ok"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
