"""msx.py -- MSX cartridge images (`.rom`, `.mx1`, `.mx2`).

    from codebreaker.formats import msx
    p = msx.read(path)
    p.origin, p.image           where the cartridge sits and its bytes
    p.entries                   {address: name}: the header's INIT, STATEMENT, DEVICE and TEXT entries

A cartridge starts "AB", then four addresses: INIT (called at power-on), STATEMENT (the BASIC
CALL extension), DEVICE, and TEXT (a BASIC program in the ROM); zero where unused. A cartridge sits
in page 1 ($4000) -- 32 KB ones fill pages 1 and 2 -- or, rarely, at $0000 or $8000; where is told
by its INIT address. Cartridges over 48 KB switch banks through a mapper (Konami, ASCII ...), which
this lane does not follow yet: only the part seen at power-on would come back.
"""
import struct

EXTENSIONS = (".rom", ".mx1", ".mx2")


class Program(object):
    pass


def is_msx(path):
    if not path.lower().endswith(EXTENSIONS):
        return False
    try:
        with open(path, "rb") as f:
            head = f.read(16)
    except OSError:
        return False
    if head[:2] == b"AB":
        return True
    try:
        with open(path, "rb") as f:
            f.seek(0x4000)
            return f.read(2) == b"AB"
    except OSError:
        return False


def read(path):
    with open(path, "rb") as f:
        data = f.read()
    if len(data) > 0xC000:
        raise ValueError("%s: a %d KB cartridge switches banks (a mapper), not read yet" % (path, len(data) // 1024))
    # The header is at the start of page 1 ($4000): the image's first byte when the cartridge starts there, or its
    # byte $4000 when it fills pages 0-2 from $0000 (48 KB: msx-wrally).
    if data[:2] == b"AB":
        at, origin = 0, 0x4000
    elif len(data) > 0x4000 and data[0x4000:0x4002] == b"AB":
        at, origin = 0x4000, 0x0000
    else:
        raise ValueError("%s: no MSX cartridge header" % path)
    init, statement, device, text = struct.unpack_from("<4H", data, at + 2)
    if at == 0 and init and init < 0x4000:
        origin = 0x0000
    elif at == 0 and init >= 0x8000 and len(data) <= 0x4000:
        origin = 0x8000
    p = Program()
    p.path, p.origin, p.image = path, origin, data
    p.header = {"init": init, "statement": statement, "device": device, "text": text}
    p.entries = {}
    for name, a in (("init", init), ("statement", statement), ("device", device)):
        if a and origin <= a < origin + len(data):
            p.entries.setdefault(a, name)
    return p
