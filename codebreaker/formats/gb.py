"""gb.py -- Game Boy and Game Boy Color cartridge images (`.gb`, `.gbc`).

    from codebreaker.formats import gb
    rom = gb.read(path)
    rom.data, rom.banks             the ROM and its 16 KB banks
    rom.title, rom.cgb, rom.sgb     the header's title; Color (only / also) and Super Game Boy support
    rom.mbc                         the cartridge type's memory bank controller ("ROM", "MBC1", "MBC3", "MBC5", ...)
    rom.memory()                    the 64 KB the CPU sees at power-on: bank 0 at $0000, bank 1 at $4000
    rom.vectors                     {"entry": $0100, "vblank": $40, "stat": $48, "timer": $50, "serial": $58, "joypad": $60}
    rom.functions()                 function starts: the entry, the interrupt vectors in use, every CALL/RST target

The cartridge header is at $0100-$014F: the entry (usually NOP / JP), Nintendo's logo (the boot
ROM checks it), the title, the Color and Super Game Boy flags, the cartridge type (which bank
controller, RAM, battery), the ROM and RAM sizes, the header checksum (the boot ROM checks it)
and the global checksum (nothing checks it). Bank 0 is always at $0000-$3FFF; the controller
switches the bank at $4000-$7FFF, which holds bank 1 at power-on -- other banks are listed by
number, their addresses known only by running the program.
"""
import os

BANK = 0x4000
EXTENSIONS = (".gb", ".gbc")
LOGO = bytes.fromhex("CEED6666CC0D000B03730083000C000D0008111F8889000EDCCC6EE6DDDDD999BBBB67636E0EECCCDDDC999FBBB9333E")
MBC = {0x00: "ROM", 0x01: "MBC1", 0x02: "MBC1+RAM", 0x03: "MBC1+RAM+BATTERY", 0x05: "MBC2", 0x06: "MBC2+BATTERY",
       0x08: "ROM+RAM", 0x09: "ROM+RAM+BATTERY", 0x0F: "MBC3+TIMER+BATTERY", 0x10: "MBC3+TIMER+RAM+BATTERY",
       0x11: "MBC3", 0x12: "MBC3+RAM", 0x13: "MBC3+RAM+BATTERY", 0x19: "MBC5", 0x1A: "MBC5+RAM",
       0x1B: "MBC5+RAM+BATTERY", 0x1C: "MBC5+RUMBLE", 0x1D: "MBC5+RUMBLE+RAM", 0x1E: "MBC5+RUMBLE+RAM+BATTERY",
       0x20: "MBC6", 0x22: "MBC7", 0xFC: "POCKET CAMERA", 0xFE: "HuC3", 0xFF: "HuC1"}


class ROM(object):
    def memory(self):
        mem = bytearray(0x10000)
        n = min(len(self.data), 0x8000)
        mem[:n] = self.data[:n]
        return mem

    def functions(self):
        return functions(self)


def is_gb(path):
    if not path.lower().endswith(EXTENSIONS):
        try:
            with open(path, "rb") as f:
                f.seek(0x104)
                return f.read(len(LOGO)) == LOGO
        except OSError:
            return False
    try:
        return os.path.getsize(path) >= 0x8000
    except OSError:
        return False


def read(path):
    with open(path, "rb") as f:
        return parse(f.read(), path)


def parse(data, path=""):
    if len(data) < 0x150:
        raise ValueError("%s: too short for a Game Boy cartridge" % path)
    rom = ROM()
    rom.path = path
    rom.data = bytes(data)
    h = rom.data
    rom.logo_ok = h[0x104:0x104 + len(LOGO)] == LOGO
    rom.cgb = {0x80: "also", 0xC0: "only"}.get(h[0x143], "")
    rom.title = h[0x134:0x143 if not rom.cgb else 0x13F].split(b"\0")[0].decode("latin-1").strip()
    rom.sgb = h[0x146] == 0x03
    rom.mbc = MBC.get(h[0x147], "type $%02X" % h[0x147])
    rom.header_checksum = h[0x14D]
    x = 0
    for b in h[0x134:0x14D]:
        x = (x - b - 1) & 0xFF
    rom.header_checksum_ok = x == h[0x14D]
    rom.banks = [rom.data[i:i + BANK] for i in range(0, len(rom.data), BANK)]
    rom.vectors = {"entry": 0x0100, "vblank": 0x40, "stat": 0x48, "timer": 0x50, "serial": 0x58, "joypad": 0x60}
    return rom


def functions(rom):
    """Function starts in the power-on address space ($0000-$7FFF): the entry at $0100, each
    interrupt vector that holds code (unused ones are commonly $FF or a lone RETI), then every
    CALL and RST target reached by following the code from them."""
    from codebreaker.decompile import sm83
    mem = rom.memory()
    limit = min(len(rom.data), 0x8000)
    starts = {0x0100}
    for name, at in rom.vectors.items():
        if name != "entry" and mem[at] not in (0xFF, 0x00) and sm83.decode(mem, at, at):
            starts.add(at)
    todo, seen = list(starts), set()
    while todo:
        pc = todo.pop()
        while 0 <= pc < limit and pc not in seen:
            if 0x104 <= pc < 0x150:                   # the header is data
                break
            seen.add(pc)
            insn = sm83.decode(mem, pc, pc)
            if insn is None:
                break
            t = insn.target
            if insn.flow in ("call", "ccall") and t is not None and t < limit and t not in starts:
                starts.add(t)
                todo.append(t)
            if t is not None and t < limit and insn.flow in ("jump", "branch", "call", "ccall"):
                todo.append(t)
            if insn.flow in ("jump", "ret", "indirect") or insn.pattern == "STOP":
                break
            pc += insn.size
    return sorted(starts)
