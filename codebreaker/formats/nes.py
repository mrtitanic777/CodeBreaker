"""nes.py -- NES cartridge images (iNES and NES 2.0, `.nes`).

    from codebreaker.formats import nes
    rom = nes.read(path)
    rom.prg, rom.chr                    the program ROM and the character (tile) ROM
    rom.mapper, rom.submapper           the cartridge board's number (0 = NROM: no bank switching)
    rom.mirroring, rom.battery          how the screen's name tables mirror; whether it saves
    rom.memory()                        the 64 KB the CPU sees at reset (RAM zero, PRG mapped)
    rom.vectors                         {"nmi", "reset", "irq"}: the three addresses at $FFFA
    rom.code_at                         the address PRG's first byte sits at
    rom.functions()                     function starts: the vectors, and every JSR target

The file: a 16-byte header ("NES" 1A, PRG size in 16 KB units, CHR size in 8 KB units, two
flag bytes holding the mapper number's nibbles, mirroring, battery and trainer), an optional
512-byte trainer, the PRG, then the CHR. NES 2.0 marks itself in byte 7 (bits 2-3 = 2) and
widens the mapper number and the sizes.

Where the program sits. On NROM (mapper 0) 32 KB of PRG fill $8000-$FFFF, and 16 KB sit at
$C000 and repeat at $8000. Other boards switch banks; as most of them do at power-on, the last
16 KB bank is taken to be fixed at $C000 and the first at $8000 -- the program's start and its
vectors are there -- and the other banks are listed by number, their addresses not known
without running the board.
"""
import struct

MAGIC = b"NES\x1a"
MIRRORING = {0: "horizontal", 1: "vertical"}


class ROM(object):
    def memory(self):
        return _memory(self)

    def functions(self):
        return functions(self)

    def mapped(self, address):
        return _mapped(self, address)


def is_nes(path):
    try:
        with open(path, "rb") as f:
            return f.read(4) == MAGIC
    except OSError:
        return False


def read(path):
    with open(path, "rb") as f:
        data = f.read()
    return parse(data, path)


def parse(data, path=""):
    if data[:4] != MAGIC or len(data) < 16:
        raise ValueError("%s: not an iNES image" % path)
    rom = ROM()
    rom.path = path
    prg_units, chr_units, f6, f7 = data[4], data[5], data[6], data[7]
    rom.nes2 = (f7 & 0x0C) == 0x08
    rom.mapper = (f6 >> 4) | (f7 & 0xF0)
    rom.submapper = 0
    if rom.nes2:
        rom.mapper |= (data[8] & 0x0F) << 8
        rom.submapper = data[8] >> 4
        prg_units |= (data[9] & 0x0F) << 8
        chr_units |= (data[9] & 0xF0) << 4
    rom.four_screen = bool(f6 & 0x08)
    rom.mirroring = "four-screen" if rom.four_screen else MIRRORING[f6 & 1]
    rom.battery = bool(f6 & 0x02)
    at = 16
    rom.trainer = data[at:at + 512] if f6 & 0x04 else b""
    at += len(rom.trainer)
    rom.header = data[:16]
    rom.prg = data[at:at + prg_units * 0x4000]
    at += len(rom.prg)
    rom.chr = data[at:at + chr_units * 0x2000]
    at += len(rom.chr)
    rom.extra = data[at:]                       # (anything after: some dumps carry a title)
    if len(rom.prg) != prg_units * 0x4000:
        raise ValueError("%s: the PRG is cut short (%d of %d bytes)" % (path, len(rom.prg), prg_units * 0x4000))
    rom.banks = [rom.prg[i:i + 0x4000] for i in range(0, len(rom.prg), 0x4000)]
    rom.code_at = 0xC000 if len(rom.prg) == 0x4000 else 0x8000
    mem = rom.memory()
    rom.vectors = {name: struct.unpack_from("<H", mem, at)[0]
                   for name, at in (("nmi", 0xFFFA), ("reset", 0xFFFC), ("irq", 0xFFFE))}
    return rom


def _memory(rom):
    """The CPU's 64 KB at power-on: PRG mapped (see the module's notes), the rest zero."""
    mem = bytearray(0x10000)
    if rom.mapper == 0 or len(rom.prg) <= 0x8000:
        if len(rom.prg) == 0x4000:
            mem[0x8000:0xC000] = mem[0xC000:0x10000] = rom.prg
        else:
            mem[0x8000:0x8000 + len(rom.prg)] = rom.prg[:0x8000]
    else:
        mem[0x8000:0xC000] = rom.banks[0]
        mem[0xC000:0x10000] = rom.banks[-1]
    return mem


def _mapped(rom, address):
    """Where in the PRG the byte the CPU reads at `address` is: an offset, or None."""
    if address < 0x8000:
        return None
    if len(rom.prg) == 0x4000:
        return (address - 0x8000) & 0x3FFF
    if rom.mapper == 0 or len(rom.prg) <= 0x8000:
        return address - 0x8000
    return (address - 0x8000) if address < 0xC000 else len(rom.prg) - 0x4000 + (address - 0xC000)


def functions(rom):
    """Function starts in the fixed address space: the three vectors, then every JSR target
    reached by following the code from them (fall through, branches, JMP, JSR)."""
    from codebreaker.decompile import m6502
    mem = rom.memory()
    starts = {v for v in rom.vectors.values() if v >= 0x8000}
    todo, seen = list(starts), set()
    while todo:
        pc = todo.pop()
        while 0x8000 <= pc <= 0xFFFF and pc not in seen:
            seen.add(pc)
            insn = m6502.decode(mem, pc, pc)
            if insn is None:
                break
            if insn.mnemonic == "JSR" and insn.target >= 0x8000 and insn.target not in starts:
                starts.add(insn.target)
                todo.append(insn.target)
            if insn.target is not None and insn.target >= 0x8000:
                todo.append(insn.target)
            if insn.mnemonic in m6502.ENDS:
                break
            pc += insn.size
    return sorted(starts)
