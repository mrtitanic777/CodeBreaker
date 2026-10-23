"""zx.py -- ZX Spectrum programs: snapshots (`.sna`, `.z80`) and tapes (`.tap`).

    from codebreaker.formats import zx
    p = zx.read(path)
    p.origin, p.image           the program's block of memory and the address it starts at
    p.entries                   {address: name}: where it starts (a snapshot's PC, a tape's USR calls,
                                an IM 2 interrupt handler)
    p.registers                 a snapshot's registers (a f b c d e h l ix iy sp i im), else {}
    p.kind                      "snapshot" / "tape"

A snapshot is the machine stopped: the registers, and RAM from $4000 (16 KB of it the screen and
the system variables, the program above). .sna (48K) is a 27-byte header and the 48 KB, with the
program counter on the stack; .z80 holds the registers and the memory, compressed (ED ED n b runs)
in its first version and in 16 KB pages from the second on (48K: page 8 at $4000, 4 at $8000, 5 at
$C000; 128K: pages 3-10 are banks 0-7: bank 5 at $4000, 2 at $8000, the one paged in at $C000).
The program block taken from a snapshot is RAM from $5B00 (after the screen and the printer
buffer) to its end.

A tape is a list of blocks: a 17-byte header (type: 0 a BASIC program, 3 code; name; length; the
load address or the BASIC autostart line) then the data. The program is the code block; its
entry is the address BASIC's USR calls (`RANDOMIZE USR 32768`), read from the tokenised BASIC.
"""
import os
import struct

EXTENSIONS = (".sna", ".z80", ".tap")
PROGRAM_START = 0x5B00          # past the screen ($4000-$5AFF)


class Program(object):
    pass


def is_zx(path):
    if not path.lower().endswith(EXTENSIONS):
        return False
    try:
        return read(path) is not None
    except (OSError, ValueError, struct.error, IndexError):
        return False


def read(path):
    with open(path, "rb") as f:
        data = f.read()
    ext = os.path.splitext(path)[1].lower()
    if ext == ".sna":
        return _sna(data, path)
    if ext == ".z80":
        return _z80(data, path)
    if ext == ".tap":
        return _tap(data, path)
    raise ValueError("%s: not a Spectrum file" % path)


def _program(path, kind, memory, registers, pc, extra_entries=()):
    p = Program()
    p.path, p.kind, p.registers = path, kind, registers
    p.memory = memory
    p.origin = PROGRAM_START
    p.image = bytes(memory[PROGRAM_START:0x10000])
    p.entries = {}
    if PROGRAM_START <= pc <= 0xFFFF:
        p.entries[pc] = "start"
    for a, n in extra_entries:
        if PROGRAM_START <= a <= 0xFFFF:
            p.entries.setdefault(a, n)
    if registers.get("im") == 2:                      # IM 2: the vector table at I*256, read at its last entry
        at = (registers["i"] << 8) | 0xFF
        handler = memory[at] | memory[(at + 1) & 0xFFFF] << 8
        if PROGRAM_START <= handler <= 0xFFFF:
            p.entries.setdefault(handler, "interrupt")
    return p


def _sna(data, path):
    if len(data) < 27 + 0xC000:
        raise ValueError("%s: too short for a .sna" % path)
    h = data[:27]
    regs = {"i": h[0], "l_": h[1], "h_": h[2], "e_": h[3], "d_": h[4], "c_": h[5], "b_": h[6], "f_": h[7],
            "a_": h[8], "l": h[9], "h": h[10], "e": h[11], "d": h[12], "c": h[13], "b": h[14],
            "iy": struct.unpack_from("<H", h, 15)[0], "ix": struct.unpack_from("<H", h, 17)[0],
            "r": h[20], "f": h[21], "a": h[22], "sp": struct.unpack_from("<H", h, 23)[0], "im": h[25]}
    mem = bytearray(0x10000)
    mem[0x4000:0x10000] = data[27:27 + 0xC000]
    if len(data) >= 27 + 0xC000 + 4:                  # 128K: the PC follows the 48K block
        pc = struct.unpack_from("<H", data, 27 + 0xC000)[0]
    else:                                             # 48K: RETN pops the PC
        sp = regs["sp"]
        pc = mem[sp] | mem[(sp + 1) & 0xFFFF] << 8
        regs["sp"] = (sp + 2) & 0xFFFF
    return _program(path, "snapshot", mem, regs, pc)


def _unrle(block, size=None):
    out = bytearray()
    i = 0
    while i < len(block):
        if block[i] == 0xED and i + 1 < len(block) and block[i + 1] == 0xED:
            out += bytes([block[i + 3]]) * block[i + 2]
            i += 4
        else:
            out.append(block[i])
            i += 1
        if size and len(out) >= size:
            break
    return out


def _z80(data, path):
    h = data[:30]
    regs = {"a": h[0], "f": h[1], "c": h[2], "b": h[3], "l": h[4], "h": h[5], "sp": struct.unpack_from("<H", h, 8)[0],
            "i": h[10], "r": (h[11] & 0x7F) | ((h[12] & 1) << 7), "e": h[13], "d": h[14],
            "iy": struct.unpack_from("<H", h, 23)[0], "ix": struct.unpack_from("<H", h, 25)[0], "im": h[29] & 3}
    pc = struct.unpack_from("<H", h, 6)[0]
    mem = bytearray(0x10000)
    if pc:                                            # version 1: 48K, compressed if bit 5 of byte 12
        body = data[30:]
        ram = _unrle(body[:-4] if body.endswith(b"\x00\xED\xED\x00") else body, 0xC000) if h[12] & 0x20 else body
        mem[0x4000:0x4000 + min(len(ram), 0xC000)] = ram[:0xC000]
        return _program(path, "snapshot", mem, regs, pc)
    extra = struct.unpack_from("<H", data, 30)[0]
    pc = struct.unpack_from("<H", data, 32)[0]
    hw = data[34]
    v2 = extra == 23
    is128 = hw >= (3 if v2 else 4) and hw not in (64,)
    paged = data[35] & 7 if is128 else 0
    at = 32 + extra
    banks = {}
    while at + 3 <= len(data):
        length, page = struct.unpack_from("<HB", data, at)
        at += 3
        raw = data[at:at + (0x4000 if length == 0xFFFF else length)]
        at += 0x4000 if length == 0xFFFF else length
        banks[page] = raw if length == 0xFFFF else _unrle(raw, 0x4000)
    where = {3 + 5: 0x4000, 3 + 2: 0x8000, 3 + paged: 0xC000} if is128 else {8: 0x4000, 4: 0x8000, 5: 0xC000}
    for page, addr in where.items():
        if page in banks:
            mem[addr:addr + 0x4000] = banks[page][:0x4000]
    return _program(path, "snapshot", mem, regs, pc)


def _tap(data, path):
    blocks, at = [], 0
    while at + 2 <= len(data):
        n = struct.unpack_from("<H", data, at)[0]
        blocks.append(data[at + 2:at + 2 + n])
        at += 2 + n
    header, code, usr = None, None, []
    basic = b""
    for b in blocks:
        if not b:
            continue
        if b[0] == 0x00 and len(b) == 19:
            header = b
            continue
        if b[0] == 0xFF and header is not None:
            kind, length, p1 = header[1], struct.unpack_from("<H", header, 12)[0], struct.unpack_from("<H", header, 14)[0]
            payload = b[1:1 + length]
            if kind == 0:
                basic += payload
            elif kind == 3 and (code is None or len(payload) > len(code[1])):
                code = (p1, payload)
            header = None
    if code is None:
        raise ValueError("%s: no code block on the tape" % path)
    usr = _usr_calls(basic)
    mem = bytearray(0x10000)
    origin, payload = code
    mem[origin:origin + len(payload)] = payload[:0x10000 - origin]
    p = Program()
    p.path, p.kind, p.registers, p.memory = path, "tape", {}, mem
    p.origin, p.image = origin, bytes(payload[:0x10000 - origin])
    p.entries = {a: "start" if i == 0 else "usr_%04X" % a for i, a in enumerate(usr) if origin <= a < origin + len(payload)}
    if not p.entries:
        p.entries = {origin: "start"}            # (no USR found: the block's first byte, as most loaders call it)
    return p


def _usr_calls(basic):
    """Addresses BASIC passes to USR: the token $C0 followed by a number (its text, then $0E and the
    five-byte form, read as the small-integer form 00 sign lo hi 00)."""
    out = []
    i = 0
    while i < len(basic):
        if basic[i] == 0xC0:
            j = basic.find(b"\x0E", i, i + 16)
            if j >= 0 and j + 6 <= len(basic) and basic[j + 1] == 0:
                out.append(basic[j + 3] | basic[j + 4] << 8)
                i = j + 6
                continue
        i += 1
    return out
