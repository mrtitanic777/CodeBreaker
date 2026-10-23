"""genesis.py -- Sega Genesis / Mega Drive cartridge images (`.md`, `.bin`, `.gen`, `.smd`).

    from codebreaker.formats import genesis
    rom = genesis.read(path)
    rom.data                        the ROM as the 68000 sees it from address 0 (an .smd dump de-interleaved)
    rom.header                      the header's fields at $100: system, copyright, names, serial, checksum,
                                    devices, ROM and RAM ranges, region
    rom.vectors                     {"ssp", "reset", and each exception vector that points into the ROM}
    rom.memory()                    the ROM placed in a 16 MB address space
    rom.functions()                 function starts: the reset vector, the exception and interrupt handlers,
                                    every JSR/BSR target reached from them
    rom.checksum()                  the checksum the header should hold (the sum of the words from $200)

The 68000 starts by reading two longs at address 0: the stack pointer and the program's start.
Then come the exception vectors (bus error, address error, illegal instruction, ...) and the
interrupt autovectors -- on the Genesis level 4 is the horizontal blank ($70) and level 6 the
vertical blank ($78). The header at $100 starts "SEGA" (the TMSS hardware of later consoles
refuses to start a cartridge without it). There is no bank switching below 4 MB: the ROM is
simply at $000000-$3FFFFF. An .smd dump (a copier's 512-byte header, then 16 KB blocks stored
odd bytes first) is turned into that plain image.
"""
import os
import struct

EXTENSIONS = (".md", ".bin", ".gen", ".smd")
VECTOR_NAMES = {2: "bus_error", 3: "address_error", 4: "illegal_instruction", 5: "zero_divide", 6: "chk",
                7: "trapv", 8: "privilege", 9: "trace", 10: "line_a", 11: "line_f", 24: "spurious",
                25: "level1", 26: "external", 27: "level3", 28: "hblank", 29: "level5", 30: "vblank", 31: "level7"}
for _n in range(16):
    VECTOR_NAMES[32 + _n] = "trap%d" % _n


class ROM(object):
    def memory(self):
        mem = bytearray(0x1000000)
        mem[:len(self.data)] = self.data[:0x400000]
        return mem

    def functions(self):
        return functions(self)

    def checksum(self):
        body = self.data[0x200:]
        if len(body) % 2:
            body += b"\0"
        return sum(struct.unpack(">%dH" % (len(body) // 2), body)) & 0xFFFF


def is_genesis(path):
    try:
        with open(path, "rb") as f:
            head = f.read(0x400)
    except OSError:
        return False
    if head[0x100:0x104] == b"SEGA" or head[0x101:0x105] == b"SEGA":
        return True
    return path.lower().endswith((".md", ".gen", ".smd")) and len(head) >= 0x200


def read(path):
    with open(path, "rb") as f:
        return parse(f.read(), path)


def _desmd(data):
    """An .smd image: a 512-byte header, then 16 KB blocks holding each block's odd bytes first."""
    body = data[512:]
    out = bytearray(len(body))
    for b in range(0, len(body), 0x4000):
        block = body[b:b + 0x4000]
        half = len(block) // 2
        out[b + 1:b + len(block):2] = block[:half]
        out[b:b + len(block):2] = block[half:]
    return bytes(out)


def parse(data, path=""):
    rom = ROM()
    rom.path = path
    rom.smd = len(data) % 0x4000 == 512 and data[0x100:0x104] != b"SEGA" and data[8:10] == b"\xAA\xBB"
    if rom.smd or (path.lower().endswith(".smd") and len(data) % 0x4000 == 512):
        data = _desmd(data)
        rom.smd = True
    if len(data) < 0x200:
        raise ValueError("%s: too short for a Genesis cartridge" % path)
    rom.data = bytes(data)
    h = rom.data

    def field(at, n):
        return h[at:at + n].decode("latin-1").strip()
    rom.header = {"system": field(0x100, 16), "copyright": field(0x110, 16), "domestic name": field(0x120, 48),
                  "overseas name": field(0x150, 48), "serial": field(0x180, 14),
                  "checksum": struct.unpack_from(">H", h, 0x18E)[0], "devices": field(0x190, 16),
                  "rom": struct.unpack_from(">II", h, 0x1A0), "ram": struct.unpack_from(">II", h, 0x1A8),
                  "region": field(0x1F0, 3)}
    longs = struct.unpack_from(">64I", h, 0)
    rom.vectors = {"ssp": longs[0], "reset": longs[1]}
    for n, name in VECTOR_NAMES.items():
        v = longs[n]
        if v < len(rom.data) and v % 2 == 0 and v >= 0x200:
            rom.vectors[name] = v
    return rom


def functions(rom):
    """Function starts: the reset vector and the handlers the vectors name, then every JSR and BSR
    target reached by following the code from them."""
    from codebreaker.decompile import m68k
    mem = rom.data
    limit = len(mem)
    starts = {v for k, v in rom.vectors.items() if k != "ssp" and v < limit and v % 2 == 0}
    todo, seen = list(starts), set()
    while todo:
        pc = todo.pop()
        while 0 <= pc < limit and pc not in seen:
            seen.add(pc)
            insn = m68k.decode(mem, pc, pc)
            if insn is None:
                break
            t = insn.target
            if t is not None and t < limit and t % 2 == 0:
                if insn.flow == "call" and insn.mnemonic != "TRAP" and t not in starts:
                    starts.add(t)
                if insn.flow in ("jump", "branch", "call"):
                    todo.append(t)
            if insn.flow in ("jump", "ret", "indirect", "halt"):
                break
            pc += insn.length
    return sorted(starts)
