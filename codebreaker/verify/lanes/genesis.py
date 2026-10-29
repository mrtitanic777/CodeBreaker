"""genesis.py -- what is particular to reconstructing a Genesis / Mega Drive program.

One of the kinds reconstruct.py works with (see there for what a kind provides). The program's
source is assembly written from its cartridge (source/genesis_writer.py); it is assembled
(build/m68k_asm.py, or asm68k / vasm), and the cartridge built is compared with the original. One
that comes back identical IS the original, so each of its functions does what the original's
does. When it is not identical -- the source was edited -- each function is run in CodeBreaker's
68000 (emulate/m68k.py) in both cartridges, from the same registers, and what it did is
compared: how the run ended, D0-D7 A0-A6 and the condition codes, the 64 KB of work RAM, and
every access to the VDP, the I/O ports and the Z80's bus, in order.
"""
from codebreaker.verify.lanes.image_lane import Lane

_HARDWARE = [(0xA00000, 0xA1FFFF), (0xC00000, 0xC0001F)]
_STATES = [dict(d=[0] * 8, a=[0] * 7, fill=0),
           dict(d=[1, 2, 3, 4, 5, 6, 7, 8], a=[0xFF0100, 0xFF0200, 0xFF0300, 0xFF0400, 0xFF0500, 0xFF0600, 0xFF0700], fill=1),
           dict(d=[0x8000, 0xFFFF, 0x7F, 0x80000000, 0x10, 0x20, 0x30, 0x40],
                a=[0xFF8000, 0xFF9000, 0xFFA000, 0xFFB000, 0xFFC000, 0xFFD000, 0xFFE000], fill=2)]


def _effects(path, entry, state):
    from codebreaker import platforms
    from codebreaker.emulate import m68k
    from codebreaker.formats import genesis
    mem = genesis.read(path).memory()
    seed = state["fill"]
    for i in range(0x10000 if seed else 0):
        mem[0xFF0000 + i] = ((i * 7 + seed * 13) ^ (i >> 3)) & (0x0F if seed % 2 else 0xFF)
    bus = m68k.Bus(mem)
    for lo, hi in _HARDWARE:
        bus.map(lo, hi, read=lambda a, n: 0x0008 if a & ~1 == 0xC00004 else 0)   # (VDP status: in vblank)
    run = platforms.of("genesis").emulator.run(bus, entry, steps=20000, d=state["d"], a=state["a"] + [0])
    r = run.results
    return (run.exit, tuple(r["D%d" % i] for i in range(8)), tuple(r["A%d" % i] for i in range(7)), r["SR"] & 0x1F,
            bytes(mem[0xFF0000:0x1000000]), tuple(run.io[:4000]))


Lane("genesis", 1, _STATES, _effects,
     ("end", "D registers", "A registers", "condition codes", "RAM", "hardware accesses"),
     lambda state: "from D0=%08X A0=%06X" % (state["d"][0], state["a"][0]), key="%06x").export(globals())
