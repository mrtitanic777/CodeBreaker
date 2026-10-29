"""nes.py -- what is particular to reconstructing a NES program.

One of the kinds reconstruct.py works with (see there for what a kind provides). The program's
source is assembly written from its cartridge (source/nes_writer.py); it is assembled and linked
(build/m6502_asm.py, or the cc65 suite), and the cartridge built is compared with the original.
One that comes back identical IS the original, so each of its functions does what the
original's does. When it is not identical -- the source was edited -- each function is run in
CodeBreaker's 6502 (emulate/m6502.py) in both cartridges, from the same registers, and what it
did is compared: how the run ended, A X Y and the flags, the RAM it left, and every access to
the picture and sound hardware, in order.
"""
from codebreaker.verify.lanes.image_lane import Lane

_HARDWARE = (0x2000, 0x401F)
# the registers a function starts from, and how its 2 KB of RAM is filled (seed 0: all zero)
_STATES = [dict(a=0, x=0, y=0, fill=0), dict(a=1, x=2, y=3, fill=1), dict(a=0x80, x=0xFF, y=0x7F, fill=2),
           dict(a=0x55, x=0x10, y=0xAA, fill=3), dict(a=0, x=0, y=0, fill=4), dict(a=0xFF, x=1, y=0, fill=5)]


def _effects(path, entry, state):
    from codebreaker import platforms
    from codebreaker.emulate import m6502
    from codebreaker.formats import nes
    mem = nes.read(path).memory()
    seed = state["fill"]
    for i in range(0x800 if seed else 0):              # small numbers, as counters and indexes hold
        mem[i] = ((i * 7 + seed * 13) ^ (i >> 3)) & (0x0F if seed % 2 else 0xFF)
    bus = m6502.Bus(mem)
    bus.map(*_HARDWARE, read=lambda a: 0x80 if a == 0x2002 else 0)    # (the picture unit always in vblank)
    run = platforms.of("nes").emulator.run(bus, entry, steps=20000, a=state["a"], x=state["x"], y=state["y"])
    ram = bytes(bus.mem[:0x800])
    return (run.exit, run.results["A"], run.results["X"], run.results["Y"], run.results["P"], ram,
            tuple(run.io[:4000]))


Lane("nes", 2, _STATES, _effects,
     ("end", "A", "X", "Y", "flags", "RAM", "hardware accesses"),
     lambda state: "from A=%(a)02X X=%(x)02X Y=%(y)02X" % state).export(globals())
