"""coff.py -- the functions of a COFF object or `!<arch>` archive: each one's name, bytes and mask.

    from codebreaker.symbols.libraries import coff
    coff.members(data)          (label, bytes) of each object in an archive
    coff.Coff(data)             one object: .machine .sections .symbols .relocations(section)
    coff.functions(data)        [Function] of one object (x86 or PowerPC), for the library matcher

The 32-bit tools' libraries -- Visual C++'s C runtime and MFC, the Xbox 360 XDK -- are archives
of COFF objects. What the linker fills in is listed by each code section's relocation records,
so nothing is guessed: on x86 the bytes of the field (two or four), on PowerPC the bits of the
instruction word the relocation's type fills (the 24-bit distance of a `bl`, a 16-bit half of
an address), the opcode and registers still compared.
"""
import struct

from codebreaker.compare import mask as masks

I386, POWERPC = 0x014C, 0x01F2
# IMAGE_REL_PPC_* (the low byte of a relocation's type)
REL_ADDR32, REL_ADDR24, REL_ADDR16, REL_ADDR14 = 0x2, 0x3, 0x4, 0x5
REL_REL24, REL_REL14, REL_ADDR32NB = 0x6, 0x7, 0xA
REL_SECREL, REL_REFHI, REL_REFLO, REL_PAIR = 0xB, 0x10, 0x11, 0x12
REL_SECRELLO, REL_SECRELHI, REL_GPREL = 0x13, 0x14, 0x15


def members(data):
    """(label, bytes) of each object in a COFF archive (`!<arch>`); the linker's own members left out."""
    at, long_names = 8, b""
    while at + 60 <= len(data):
        head = data[at:at + 60]
        try:                                                    # the size is the last number of the header
            size = int(head[16:58].replace(b"\0", b" ").split()[-1])    # (the first 32-bit tools misplaced it)
        except (ValueError, IndexError):
            return
        body = data[at + 60:at + 60 + size]
        at += 60 + size + (size & 1)
        label = head[:16].strip()
        if label == b"//":
            long_names = body
        elif label not in (b"/", b""):
            if label.startswith(b"/") and label[1:].isdigit():
                label = long_names[int(label[1:]):].split(b"\0")[0].split(b"\n")[0]
            yield label.rstrip(b"/").decode("latin1"), body


class Coff(object):
    """A COFF object: its headers, section table, symbols (None in the slots of auxiliary records)
    and relocations. The structures are little-endian whatever the machine (the Xbox 360's code is
    big-endian, its objects' headers are not)."""

    def __init__(self, data):
        self.data = data
        self.machine, nsec, _t, symptr, nsym, opt, _c = struct.unpack_from("<HHIIIHH", data, 0)
        self.symptr = symptr
        self.sections = []
        for i in range(nsec):
            o = 20 + opt + i * 40
            _vs, _va, rawsize, rawptr, relptr, _l, nrel, _n, chars = struct.unpack_from("<IIIIIIHHI", data, o + 8)
            self.sections.append(dict(size=rawsize, ptr=rawptr, relptr=relptr, nrel=nrel, chars=chars))
        strtab = symptr + nsym * 18
        self.symbols, i = [], 0
        while i < nsym and symptr:
            o = symptr + i * 18
            raw = data[o:o + 18]
            if len(raw) < 18:                                     # (a record cut short: the table ends)
                break
            if raw[:4] == b"\0\0\0\0":
                so = strtab + struct.unpack_from("<I", raw, 4)[0]
                name = data[so:data.find(b"\0", so)].decode("latin-1")
            else:
                name = raw[:8].rstrip(b"\0").decode("latin-1")
            value, secnum, typ, cls, naux = struct.unpack_from("<IhHBB", raw, 8)
            self.symbols.append(dict(name=name, value=value, sec=secnum, type=typ, cls=cls))
            self.symbols.extend([None] * naux)
            i += 1 + naux

    def relocations(self, sec):
        """[(offset in the section, symbol index, type)]"""
        return [struct.unpack_from("<IIH", self.data, sec["relptr"] + k * 10) for k in range(sec["nrel"])]


def functions(data):
    """[Function] of one object: each external function of its code sections (x86), or each
    function a symbol starts (PowerPC); [] for an object of another machine."""
    from codebreaker.symbols.libraries import Function
    if len(data) < 20:
        return []
    obj = Coff(data)
    if obj.machine == I386 and obj.symptr:
        return [Function(*f) for f in _x86(obj)]
    if obj.machine == POWERPC and obj.symbols:
        return [Function(*f) for f in _powerpc(obj)]
    return []


def _x86(obj):
    """(name, code, byte mask, offset) for the external functions of a 32-bit x86 object."""
    sections = {}
    for i, sec in enumerate(obj.sections):
        if sec["chars"] & 0x20 and sec["size"] and sec["ptr"]:  # holds code
            masked = set()
            for at, _sym, kind in obj.relocations(sec):
                masked.update(range(at, at + (2 if kind in (1, 2) else 4)))
            sections[i + 1] = (obj.data[sec["ptr"]:sec["ptr"] + sec["size"]], masked, [])
    for s in obj.symbols:
        # a function: external, or static and typed as a function (jump-table labels are neither)
        if s and s["sec"] in sections and (s["cls"] == 2 or (s["cls"] == 3 and s["type"] == 0x20)):
            sections[s["sec"]][2].append((s["value"], s["name"], s["cls"]))
    for code, masked, symbols in sections.values():
        symbols.sort()
        for k, (start, name, storage) in enumerate(symbols):
            end = next((s for s, _n, _c in symbols[k + 1:] if s > start), len(code))
            if storage == 2:
                piece = code[start:end]
                yield name, piece, masks.of((o - start for o in masked if start <= o < end), len(piece)), start


def _field(typ):
    """The bits of an instruction word a relocation of this type fills (None: the whole word)."""
    t = typ & 0xFF
    if t in (REL_REL24, REL_ADDR24):
        return 0x03FFFFFC
    if t in (REL_REL14, REL_ADDR14):
        return 0x0000FFFC
    if t in (REL_REFHI, REL_REFLO, REL_ADDR16, REL_SECRELLO, REL_SECRELHI, REL_GPREL):
        return 0x0000FFFF
    return None


def _powerpc(obj):
    """(name, code, [bits left out of each word], offset in its section) for every function in a
    PowerPC object's code sections."""
    for si, sec in enumerate(obj.sections, start=1):
        if not sec["chars"] & 0x20 or sec["size"] < 8:            # IMAGE_SCN_CNT_CODE
            continue
        code = obj.data[sec["ptr"]:sec["ptr"] + sec["size"]]
        n = len(code) // 4
        ignore = [0] * n
        skip = False
        for va, _sym, typ in obj.relocations(sec):
            if skip:                                               # a PAIR carries data, not a place
                skip = False
                continue
            if typ & 0xFF == REL_PAIR:
                continue
            if typ & 0xFF == REL_REFHI:
                skip = True
            m = _field(typ)
            w = (va & ~3) // 4
            if w >= n:
                continue
            if m is None and va & 3:                               # an unaligned 32-bit field: two words
                ignore[w] = 0xFFFFFFFF
                if w + 1 < n:
                    ignore[w + 1] = 0xFFFFFFFF
            else:
                ignore[w] |= m if m is not None else 0xFFFFFFFF
        syms = sorted((s for s in obj.symbols if s and s["sec"] == si and s["cls"] in (2, 3)
                       and not s["name"].startswith((".", "$")) and (s["type"] & 0x20 or s["cls"] == 2)),
                      key=lambda s: s["value"])
        for k, s in enumerate(syms):
            start = s["value"]
            end = syms[k + 1]["value"] if k + 1 < len(syms) else len(code)
            if end - start >= 8 and not start & 3:
                yield s["name"], code[start:end], ignore[start // 4:end // 4], start
