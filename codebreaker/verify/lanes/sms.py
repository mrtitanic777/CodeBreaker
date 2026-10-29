"""sms.py -- what is particular to reconstructing a Master System / Game Gear program.

One of the kinds reconstruct.py works with (see there for what a kind provides). The program's
source is assembly written from its cartridge (source/sms_writer.py); it is assembled
(build/z80_asm.py, or WLA-DX), and the cartridge built is compared with the original. One that
comes back identical IS the original, so each of its functions does what the original's does.
When it is not identical -- the source was edited -- each function is run in CodeBreaker's Z80
(emulate/z80.py) in both cartridges, from the same registers, and what it did is compared: how
the run ended, the registers and flags, the 8 KB of RAM it left, and every port access and
mapper write, in order.
"""
from codebreaker.verify.lanes.image_lane import Lane

_MAPPER = (0xFFFC, 0xFFFF)
# the registers a function starts from, and how its RAM is filled (seed 0: all zero)
_STATES = [dict(a=0, b=0, c=0, d=0, e=0, h=0, l=0, fill=0), dict(a=1, b=2, c=3, d=0xC1, e=0x00, h=0xC2, l=0x10, fill=1),
           dict(a=0x80, b=0xFF, c=0x7F, d=0xC0, e=0x80, h=0xC1, l=0x00, fill=2),
           dict(a=0x55, b=0x10, c=0xAA, d=0xD0, e=0x00, h=0xC8, l=0x40, fill=3)]


def _effects(path, entry, state):
    from codebreaker import platforms
    from codebreaker.emulate import z80
    from codebreaker.formats import sms
    mem = sms.read(path).memory()
    seed = state["fill"]
    for i in range(0x2000 if seed else 0):            # small numbers, as counters and indexes hold
        mem[0xC000 + i] = ((i * 7 + seed * 13) ^ (i >> 3)) & (0x0F if seed % 2 else 0xFF)
    bus = z80.Bus(mem, inp=lambda port: 0x80 if port & 0xFF == 0xBF else 0xFF)   # (the VDP always in vblank)
    bus.map(*_MAPPER)
    regs = {k: v for k, v in state.items() if k != "fill"}
    run = platforms.of("sms").emulator.run(bus, entry, steps=20000, **regs)
    ram = bytes(bus.mem[0xC000:0xE000])
    return (run.exit,) + tuple(run.results[k] for k in ("A", "F", "BC", "DE", "HL", "IX", "IY")) + (ram, tuple(run.io[:4000]))


Lane("sms", 1, _STATES, _effects,
     ("end", "A", "flags", "BC", "DE", "HL", "IX", "IY", "RAM", "port and mapper accesses"),
     lambda state: "from A=%(a)02X BC=%(b)02X%(c)02X HL=%(h)02X%(l)02X" % state).export(globals())
