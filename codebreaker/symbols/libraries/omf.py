"""omf.py -- the functions of a 16-bit OMF object module or library: each one's name, bytes and mask.

    from codebreaker.symbols.libraries import omf
    omf.functions(data)         [Function] of every object module in a .LIB (or one .OBJ)

The 16-bit tools' libraries (Microsoft C's and Visual C++ 1.5's run-time libraries) are object
modules laid end to end on pages. A function is a public name inside a code segment (one whose
class says CODE), or a piece the linker places on its own (a COMDAT: the C++ compiler writes one
per function). What the linker fills in is listed by the fix-up records after each data record:
the bytes of each fix-up's field (1 in the mask) -- and the opcode of a far call (2), which a
linker that translates far calls rewrites, with the address, as `push cs` / `call near` when the
callee ends up in the same segment. Any true value leaves a byte out; a matcher that knows its
program was linked without that translation compares a far call's opcode (`fields_only`).
"""
import struct

_WIDTH = {0: 1, 1: 2, 2: 2, 3: 4, 4: 1, 5: 2, 9: 4, 11: 6, 13: 4}       # bytes a 16-bit fix-up covers, by kind


def _number(b, i):
    return (((b[i] & 0x7F) << 8) | b[i + 1], i + 2) if b[i] & 0x80 else (b[i], i + 1)


def fields_only(mask):
    """The mask without the opcodes of far calls: only what a fix-up's field covers."""
    return bytes(1 if m == 1 else 0 for m in mask)


def _mask(masked, start, size):
    """The byte mask of `size` bytes from `start`, out of {offset: 1 | 2} for a whole buffer."""
    out = bytearray(size)
    for o, v in masked.items():
        if start <= o < start + size:
            out[o - start] = v
    return out


def functions(data):
    """[Function] for every function in a library (a dictionary-paged .LIB) or one object module.
    A module the reader cannot follow is left out."""
    out = []
    at, page, start = 0, 1, None
    if data[:1] == b"\xF0":
        page = struct.unpack_from("<H", data, 1)[0] + 3
        at = page
    while at + 3 <= len(data) and data[at] != 0xF1:
        kind, size = data[at], struct.unpack_from("<H", data, at + 1)[0]
        if start is None:
            start = at
        at += 3 + size
        if kind in (0x8A, 0x8B):                             # the end of one object module
            try:
                out += module_functions(data[start:at])
            except (struct.error, IndexError):
                pass
            start = None
            at += (-at) % page
    return out


def module_functions(module):
    """[Function] for the public functions of one object module (its bytes, MODEND included)."""
    from codebreaker.symbols.libraries import Function
    names, segments, publics, pieces = [""], [], [], {}
    last, at = None, 0                                       # last: (bytes, {masked offset: 1|2}, offset) fix-ups apply to
    while at + 3 <= len(module):
        kind, size = module[at], struct.unpack_from("<H", module, at + 1)[0]
        b = module[at + 3:at + 3 + size - 1]
        at += 3 + size
        wide = kind & 1                                      # the 32-bit form of a record
        if kind == 0x96:
            i = 0
            while i < len(b):
                names.append(b[i + 1:i + 1 + b[i]].decode("latin1"))
                i += 1 + b[i]
        elif kind in (0x98, 0x99):
            i = 1 + (3 if (b[0] >> 5) == 0 else 0) + (4 if wide else 2)
            _nm, i = _number(b, i)
            cls, i = _number(b, i)
            segments.append((names[cls] if cls < len(names) else "", bytearray(), {}))
        elif kind in (0xA0, 0xA1, 0xC2, 0xC3):
            if kind >= 0xC2:                                 # a piece of its own
                flags, how = b[0], b[1] & 0x0F
                off = struct.unpack_from("<I" if wide else "<H", b, 3)[0]
                _type, i = _number(b, 3 + (4 if wide else 2))
                code = how in (1, 3)
                if how == 0:
                    _grp, i = _number(b, i)
                    seg, i = _number(b, i)
                    i += 2 if seg == 0 else 0
                    code = 1 <= seg <= len(segments) and "CODE" in segments[seg - 1][0].upper()
                name, i = _number(b, i)
                where = pieces.setdefault(name, (code and not flags & 2, bytearray(), {}))
                buf, masked, chunk = where[1], where[2], b[i:]
            else:
                seg, i = _number(b, 0)
                off = struct.unpack_from("<I" if wide else "<H", b, i)[0]
                chunk = b[i + (4 if wide else 2):]
                if not 1 <= seg <= len(segments):
                    last = None
                    continue
                buf, masked = segments[seg - 1][1], segments[seg - 1][2]
            buf.extend(bytes(max(0, off + len(chunk) - len(buf))))
            buf[off:off + len(chunk)] = chunk
            last = (buf, masked, off)
        elif kind in (0x9C, 0x9D) and last:
            buf, masked, base = last
            i = 0
            while i < len(b):
                first = b[i]
                if not first & 0x80:                         # a thread: says how later fix-ups are framed
                    i += 1
                    if (first >> 2) & 7 < 3:
                        _x, i = _number(b, i)
                    continue
                where = ((first & 3) << 8) | b[i + 1]
                loc, fix = (first >> 2) & 0xF, b[i + 2]
                i += 3
                if not fix & 0x80 and (fix >> 4) & 7 < 3:
                    _x, i = _number(b, i)
                if not fix & 0x08:
                    _x, i = _number(b, i)
                if not fix & 0x04:
                    i += 4 if wide else 2
                start, n = base + where, _WIDTH.get(loc, 2)
                if loc == 3 and start and buf[start - 1] == 0x9A:
                    masked.setdefault(start - 1, 2)          # a far call: the linker may rewrite all of it
                for o in range(start, start + n):
                    masked[o] = 1
        elif kind in (0x90, 0x91):
            _grp, i = _number(b, 0)
            seg, i = _number(b, i)
            if seg == 0:
                i += 2
            while i < len(b):
                name = b[i + 1:i + 1 + b[i]].decode("latin1")
                i += 1 + b[i]
                off = struct.unpack_from("<I" if wide else "<H", b, i)[0]
                i += 4 if wide else 2
                _type, i = _number(b, i)
                publics.append((seg, off, name))
    out = [Function(names[n], bytes(buf), _mask(masked, 0, len(buf)), 0)
           for n, (code, buf, masked) in pieces.items() if code and n < len(names)]
    publics.sort()
    for k, (seg, start, name) in enumerate(publics):
        if not (1 <= seg <= len(segments)) or "CODE" not in segments[seg - 1][0].upper():
            continue
        code, masked = segments[seg - 1][1], segments[seg - 1][2]
        end = next((o for s, o, _n in publics[k + 1:] if s == seg and o > start), len(code))
        piece = bytes(code[start:end])
        out.append(Function(name, piece, _mask(masked, start, len(piece)), start))
    return out
