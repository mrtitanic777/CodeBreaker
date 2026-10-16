"""exepack.py -- what a file packed by EXEPACK (or LINK /EXEPACK) keeps of the program it packed.

The packed file's code segment begins with EXEPACK's header: the program's own entry and stack (real
CS:IP, SS:SP), the length it unpacks to, the signature "RB". After the unpacker's code and its message
("Packed file is corrupt") come the program's relocations: sixteen groups, one for each 64K of the image,
a count and that many offsets - in the order the linker listed them.

    exepack.read(file bytes) -> Packed or None (not packed)
        .ip .cs .sp .ss      the program's entry and stack (paragraphs from the image's start)
        .dest                the unpacked length in paragraphs
        .relocs              [image address] of each relocation, in the table's order
"""
import struct

SIG = b"RB"
MSG = b"Packed file is corrupt"


class Packed(object):
    pass


def read(b):
    if b[:2] not in (b"MZ", b"ZM") or len(b) < 0x20:
        return None
    h = struct.unpack_from("<14H", b, 0)
    hdr = h[4] * 16
    at = hdr + h[11] * 16                                   # (the packed file's CS:0 - EXEPACK's header)
    if at + 18 > len(b) or b[at + 14:at + 16] != SIG and b[at + 16:at + 18] != SIG:
        return None
    p = Packed()
    p.ip, p.cs, p.mem, p.size, p.sp, p.ss, p.dest = struct.unpack_from("<7H", b, at)
    p.relocs = []
    q = b.find(MSG, at)
    if q >= 0:
        q += len(MSG)
        for g in range(16):
            if q + 2 > len(b):
                break
            n = struct.unpack_from("<H", b, q)[0]
            q += 2
            p.relocs += [g * 0x10000 + struct.unpack_from("<H", b, q + 2 * k)[0] for k in range(n)]
            q += 2 * n
    return p
