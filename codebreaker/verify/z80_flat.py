"""z80_flat.py -- the lane of a flat Z80 program (one block at one address): the ZX Spectrum's and
the MSX's (verify/zx.py, verify/msx.py), each giving its reader, writer and machine.

The program's source is written (source/z80_writer.py through the machine's writer) and built
(build/z80_asm.py build_flat); a block that comes back identical IS the original. When it is not
-- the source was edited -- each function is run in CodeBreaker's Z80 (emulate/z80.py) in the
original's memory and in the rebuilt one's, from the same registers, and what it did is compared:
how the run ended, the registers and flags, the memory it left, and every port access, in order.
The machine's ROM is not run: each of its bytes reads as RET, so a call into the ROM returns at
once, the same way in both runs.
"""
from codebreaker.verify.image_lane import Lane

STATES = [dict(a=0, b=0, c=0, d=0, e=0, h=0, l=0, fill=0), dict(a=1, b=2, c=3, d=0xC1, e=0x00, h=0xC2, l=0x10, fill=1),
          dict(a=0x80, b=0xFF, c=0x7F, d=0xD0, e=0x80, h=0xE1, l=0x00, fill=2)]
WHAT = ("end", "A", "flags", "BC", "DE", "HL", "IX", "IY", "memory", "port accesses")


def lane(kind, rev, read, writer, rom):
    """kind: the name results are filed under; read(path) -> a Program (formats/zx.py, msx.py);
    writer: the machine's writer module; rom: the addresses the machine's ROM occupies."""
    def subjects(module_path, written):
        p = read(module_path)
        return (p, p.image), (p, written["built image"])
    return Lane(kind, rev, writer, STATES, lambda subject, entry, state: _effects(rom, subject, entry, state), WHAT,
                lambda state: "from A=%(a)02X BC=%(b)02X%(c)02X HL=%(h)02X%(l)02X" % state, subjects=subjects,
                same="the program built from this source is the original, byte for byte")


def _effects(rom, subject, entry, state):
    from codebreaker.emulate import z80
    program, image = subject
    mem = bytearray(program.memory) if hasattr(program, "memory") else bytearray(0x10000)
    lo, hi = rom
    mem[lo:hi] = b"\xC9" * (hi - lo)                       # the ROM: every byte RET
    mem[program.origin:program.origin + len(image)] = image
    seed = state["fill"]
    end = program.origin + len(image)
    for i in range(end, 0x10000 if seed else end):         # memory past the program
        mem[i] = ((i * 7 + seed * 13) ^ (i >> 3)) & (0x0F if seed % 2 else 0xFF)
    bus = z80.Bus(mem, inp=lambda port: 0xFF)
    regs = {k: v for k, v in state.items() if k != "fill"}
    before = bytes(mem)
    run = z80.run(bus, entry, steps=20000, sp=0xFFF0, **regs)
    # what the run changed (the program lives in this memory: an edited byte the function does not
    # write is the source's difference, not the function's)
    changed = tuple((i, v) for i, (u, v) in enumerate(zip(before, bus.mem)) if u != v)
    return (run["end"], run["a"], run["f"], run["bc"], run["de"], run["hl"], run["ix"], run["iy"], changed,
            tuple(bus.log[:4000]))
