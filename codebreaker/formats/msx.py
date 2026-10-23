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
import os
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
    return head[:2] == b"AB"


def read(path):
    with open(path, "rb") as f:
        data = f.read()
    if data[:2] != b"AB":
        raise ValueError("%s: no MSX cartridge header" % path)
    if len(data) > 0xC000:
        raise ValueError("%s: a %d KB cartridge switches banks (a mapper), not read yet" % (path, len(data) // 1024))
    init, statement, device, text = struct.unpack_from("<4H", data, 2)
    origin = 0x4000
    if init and init < 0x4000:
        origin = 0x0000
    elif init >= 0x8000 and len(data) <= 0x4000:
        origin = 0x8000
    p = Program()
    p.path, p.origin, p.image = path, origin, data
    p.header = {"init": init, "statement": statement, "device": device, "text": text}
    p.entries = {}
    for name, a in (("init", init), ("statement", statement), ("device", device)):
        if a and origin <= a < origin + len(data):
            p.entries.setdefault(a, name)
    return p
