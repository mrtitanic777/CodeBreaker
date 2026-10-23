"""sms.py -- Sega Master System, Game Gear and SG-1000 cartridge images (`.sms`, `.gg`, `.sg`).

    from codebreaker.formats import sms
    rom = sms.read(path)
    rom.data                    the ROM (a copier's 512-byte header, when there is one, left out)
    rom.system                  "Master System" / "Game Gear" / "SG-1000"
    rom.header                  the "TMR SEGA" header's fields, or None (SG-1000 and some homebrew have none)
    rom.pages                   the 16 KB pages
    rom.memory()                the 64 KB the Z80 sees at power-on: pages 0-2 in slots 0-2, RAM zero
    rom.vectors                 {"reset": 0x0000, "irq": 0x0038, "nmi": 0x0066}
    rom.functions()             function starts: the vectors and every CALL/RST target reached from them

A cartridge image is the ROM as it is, nothing in front (a 512-byte copier header is recognised
by the size and dropped). The Master System BIOS checks a header at $7FF0 (or $3FF0 / $1FF0 for
small ROMs): "TMR SEGA", two reserved bytes, the checksum, the product code and version, the
region and the ROM size. The Game Gear's is the same with regions 5-7.

The Z80 sees the ROM through Sega's mapper: three 16 KB slots at $0000, $4000 and $8000, each
showing the page written to $FFFD / $FFFE / $FFFF (the first 1 KB always page 0, where the
vectors are); RAM is 8 KB at $C000, repeated at $E000. At power-on the slots hold pages 0, 1
and 2, which is what memory() maps; other pages are listed by number, their addresses known only
by running the program (it writes the mapper registers itself). SG-1000 cartridges have no mapper.
"""
import os
import struct

PAGE = 0x4000
_HEADER_AT = (0x7FF0, 0x3FF0, 0x1FF0)
REGIONS = {3: ("Master System", "Japan"), 4: ("Master System", "export"), 5: ("Game Gear", "Japan"),
           6: ("Game Gear", "export"), 7: ("Game Gear", "international")}
_SIZES = {0xA: 0x2000, 0xB: 0x4000, 0xC: 0x8000, 0xD: 0xC000, 0xE: 0x10000, 0xF: 0x20000, 0x0: 0x40000,
          0x1: 0x80000, 0x2: 0x100000}
EXTENSIONS = (".sms", ".gg", ".sg")


class ROM(object):
    def memory(self):
        return _memory(self)

    def functions(self):
        return functions(self)

    def mapped(self, address):
        """Where in the ROM the byte the Z80 reads at `address` at power-on is: an offset, or None."""
        if address >= 0xC000 or address >= len(self.data):
            return None
        return address


def is_sms(path):
    if not path.lower().endswith(EXTENSIONS):
        return False
    try:
        return os.path.getsize(path) >= 0x2000
    except OSError:
        return False


def read(path):
    with open(path, "rb") as f:
        return parse(f.read(), path)


def parse(data, path=""):
    rom = ROM()
    rom.path = path
    if len(data) % PAGE == 512:                     # a copier's header in front
        data = data[512:]
    rom.data = bytes(data)
    rom.header = None
    for at in _HEADER_AT:
        if len(data) >= at + 16 and data[at:at + 8] == b"TMR SEGA":
            h = data[at:at + 16]
            region, size = h[15] >> 4, h[15] & 0x0F
            # the product code: BCD, low byte first, its top digit in the high nibble of byte 14
            rom.header = {"at": at, "checksum": struct.unpack_from("<H", h, 10)[0],
                          "product": ("%X%02X%02X" % (h[14] >> 4, h[13], h[12])).lstrip("0") or "0",
                          "version": h[14] & 0x0F, "region": REGIONS.get(region, ("?", "?"))[1],
                          "size": _SIZES.get(size)}
            break
    ext = os.path.splitext(path)[1].lower()
    if rom.header:
        rom.system = REGIONS.get(rom.data[rom.header["at"] + 15] >> 4, ("Master System",))[0]
    else:
        rom.system = {".gg": "Game Gear", ".sg": "SG-1000"}.get(ext, "Master System")
    rom.mapper = rom.system != "SG-1000" and len(rom.data) > 0xC000
    rom.pages = [rom.data[i:i + PAGE] for i in range(0, len(rom.data), PAGE)]
    rom.vectors = {"reset": 0x0000, "irq": 0x0038, "nmi": 0x0066}
    return rom


def _memory(rom):
    mem = bytearray(0x10000)
    n = min(len(rom.data), 0xC000)
    mem[:n] = rom.data[:n]
    return mem


def functions(rom):
    """Function starts in the power-on address space ($0000-$BFFF): the reset, interrupt and NMI
    vectors, then every CALL and RST target reached by following the code from them."""
    from codebreaker.decompile import z80
    mem = rom.memory()
    limit = min(len(rom.data), 0xC000)
    starts = {v for v in rom.vectors.values() if v < limit and _looks_like_code(mem, v)}
    starts.add(0)
    todo, seen = list(starts), set()
    while todo:
        pc = todo.pop()
        while 0 <= pc < limit and pc not in seen:
            seen.add(pc)
            insn = z80.decode(mem, pc, pc)
            if insn is None:
                break
            t = insn.target
            if insn.flow in ("call", "ccall") and t is not None and t < limit and t not in starts:
                starts.add(t)
                todo.append(t)
            if t is not None and t < limit and insn.flow in ("jump", "branch", "call", "ccall"):
                todo.append(t)
            if insn.flow in ("jump", "ret", "indirect"):     # (HALT resumes after an interrupt)
                break
            pc += insn.size
    return sorted(starts)


def _looks_like_code(mem, at):
    """An interrupt vector the program does not use is often filled with $FF (RST $38) or zero."""
    return mem[at] not in (0xFF,)
