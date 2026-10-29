"""gb.py -- what is particular to reconstructing a Game Boy program.

One of the kinds reconstruct.py works with (see there for what a kind provides). The program's
source is assembly written from its cartridge (source/gb_writer.py); it is assembled
(build/sm83_asm.py, or RGBDS), and the cartridge built is compared with the original. One that
comes back identical IS the original, so each of its functions does what the original's does.
When it is not identical -- the source was edited -- each function is run in CodeBreaker's
SM83 (emulate/sm83.py) in both cartridges, from the same registers, and what it did is compared:
how the run ended, the registers and flags, work RAM and high RAM as it left them, and every
access to the I/O registers and the bank controller, in order.
"""
from codebreaker.verify.lanes.image_lane import Lane

_IO = (0xFF00, 0xFF7F)
_STATES = [dict(a=0, b=0, c=0, d=0, e=0, h=0, l=0, fill=0), dict(a=1, b=2, c=3, d=0xC1, e=0x00, h=0xC2, l=0x10, fill=1),
           dict(a=0x80, b=0xFF, c=0x7F, d=0xC0, e=0x80, h=0xC1, l=0x00, fill=2),
           dict(a=0x55, b=0x10, c=0xAA, d=0xD0, e=0x00, h=0xC8, l=0x40, fill=3)]


def _effects(path, entry, state):
    from codebreaker import platforms
    from codebreaker.emulate import sm83
    from codebreaker.formats import gb
    mem = gb.read(path).memory()
    seed = state["fill"]
    for i in range(0x2000 if seed else 0):
        mem[0xC000 + i] = ((i * 7 + seed * 13) ^ (i >> 3)) & (0x0F if seed % 2 else 0xFF)
    bus = sm83.Bus(mem)
    bus.map(*_IO, read=lambda a: 0x90 if a == 0xFF44 else 0)      # (LY in vblank)
    bus.map(0xFFFF, 0xFFFF)
    bus.map(0x2000, 0x3FFF, read=lambda a: mem[a])                 # bank select writes, logged
    regs = {k: v for k, v in state.items() if k != "fill"}
    run = platforms.of("gb").emulator.run(bus, entry, steps=20000, **regs)
    ram = bytes(bus.mem[0xC000:0xE000]) + bytes(bus.mem[0xFF80:0xFFFF])
    io = tuple(x for x in run.io if not (x[0] == "r" and 0x2000 <= x[1] <= 0x3FFF))[:4000]
    return (run.exit,) + tuple(run.results[k] for k in ("A", "F", "BC", "DE", "HL")) + (ram, io)


Lane("gb", 1, _STATES, _effects,
     ("end", "A", "flags", "BC", "DE", "HL", "RAM", "I/O and bank accesses"),
     lambda state: "from A=%(a)02X BC=%(b)02X%(c)02X HL=%(h)02X%(l)02X" % state).export(globals())
