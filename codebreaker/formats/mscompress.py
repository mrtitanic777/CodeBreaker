"""mscompress.py -- files stored by Microsoft's COMPRESS (the setup disks' NAME.EX_ / NAME.EX$), expanded.

    python -m codebreaker.formats.mscompress FILE... [--out DIR]
    from codebreaker.formats import mscompress
    data, name = mscompress.expand(packed)          name: the original's name where the file says it

Three formats, by their first bytes:
    SZDD   "SZDD\\x88\\xf0\\x27\\x33"  (COMPRESS 2, Windows 3.1 and Visual C++ 1.x disks): LZSS in a 4 KB
           window of spaces, written from 4096-16; a control byte's bits (lowest first) say literal (1)
           or a match of two bytes: position 12 bits, length 4 bits + 3
    SZ     "SZ \\x88\\xf0\\x27\\x33\\xd1" (MS-DOS 5 era, Microsoft C 6.0 disks): the same, written from 4096-18
    KWAJ   "KWAJ\\x88\\xf0\\x27\\xd1"    (Microsoft C/C++ 7.0 disks): a header of optional fields (flags), then
           method 0 stored, 1 XOR FF, 2 the LZSS above, 3 LZ with five Huffman tables (match lengths after a
           match / after literals, literal run lengths, offset high 6 bits, literals), bits highest first
"""
import os
import struct
import sys


def _lzss(data, pos):
    win = bytearray(b" " * 4096)
    out = bytearray()
    i, n = 0, len(data)
    while i < n:
        ctl = data[i]
        i += 1
        for bit in range(8):
            if i >= n:
                break
            if ctl >> bit & 1:
                b = data[i]
                i += 1
                out.append(b)
                win[pos] = b
                pos = (pos + 1) & 4095
            else:
                if i + 1 >= n:
                    i = n
                    break
                lo, hi = data[i], data[i + 1]
                i += 2
                at, length = lo | (hi & 0xF0) << 4, (hi & 0x0F) + 3
                for _ in range(length):
                    b = win[at]
                    out.append(b)
                    win[pos] = b
                    pos = (pos + 1) & 4095
                    at = (at + 1) & 4095
    return bytes(out)


class _Bits(object):
    def __init__(self, data):
        self.d, self.p, self.buf, self.n = data, 0, 0, 0

    def get(self, k):
        while self.n < k:
            if self.p >= len(self.d):
                raise EOFError
            self.buf = self.buf << 8 | self.d[self.p]
            self.p += 1
            self.n += 8
        self.n -= k
        return self.buf >> self.n & ((1 << k) - 1)


def _lens(bits, kind, count):
    if kind == 0:
        return [{16: 4, 32: 5, 64: 6, 256: 8}[count]] * count
    lens = []
    if kind == 1:
        c = bits.get(4)
        lens.append(c)
        while len(lens) < count:
            if bits.get(1) == 0:
                pass
            elif bits.get(1) == 0:
                c += 1
            else:
                c = bits.get(4)
            lens.append(c)
    elif kind == 2:
        c = bits.get(4)
        lens.append(c)
        while len(lens) < count:
            sel = bits.get(2)
            c = bits.get(4) if sel == 3 else c + sel - 1
            lens.append(c)
    elif kind == 3:
        lens = [bits.get(4) for _ in range(count)]
    else:
        raise ValueError("KWAJ table type %d" % kind)
    return lens


def _huffman(lens):
    """{(length, code): symbol}, codes given in order of length then symbol (canonical)."""
    table, code = {}, 0
    for length in range(1, 17):
        for sym, l in enumerate(lens):
            if l == length:
                table[(length, code)] = sym
                code += 1
        code <<= 1
    return table


def _sym(bits, table):
    code = 0
    for length in range(1, 17):
        code = code << 1 | bits.get(1)
        if (length, code) in table:
            return table[(length, code)]
    raise ValueError("KWAJ: no such Huffman code")


def _kwaj_lzh(data):
    bits = _Bits(data)
    types = [bits.get(4) for _ in range(6)]
    t = [_huffman(_lens(bits, types[k], n)) for k, n in enumerate((16, 16, 32, 64, 256))]
    matchlen1, matchlen2, litlen, offsets, literals = t
    win, pos, out, lit_run = bytearray(b" " * 4096), 0, bytearray(), False
    try:
        while True:
            length = _sym(bits, matchlen2 if lit_run else matchlen1)
            if length:
                length += 2
                lit_run = False
                off = _sym(bits, offsets) << 6 | bits.get(6)
                for _ in range(length):
                    b = win[(pos - off) & 4095]
                    win[pos] = b
                    out.append(b)
                    pos = (pos + 1) & 4095
            else:
                length = _sym(bits, litlen) + 1
                lit_run = length != 32
                for _ in range(length):
                    b = _sym(bits, literals)
                    win[pos] = b
                    out.append(b)
                    pos = (pos + 1) & 4095
    except EOFError:
        pass
    return bytes(out)


def expand(packed):
    """-> (the original bytes, its name from the file's own header or None)."""
    if packed[:8] == b"SZDD\x88\xf0\x27\x33":
        size = struct.unpack_from("<I", packed, 10)[0]
        return _lzss(packed[14:], 4096 - 16)[:size], None
    if packed[:8] == b"SZ \x88\xf0\x27\x33\xd1":
        size = struct.unpack_from("<I", packed, 8)[0]
        return _lzss(packed[12:], 4096 - 18)[:size], None
    if packed[:8] == b"KWAJ\x88\xf0\x27\xd1":
        method, start, flags = struct.unpack_from("<HHH", packed, 8)
        p, size, name = 14, None, None
        if flags & 1:
            size = struct.unpack_from("<I", packed, p)[0]
            p += 4
        if flags & 2:
            p += 2
        if flags & 4:
            p += 2 + struct.unpack_from("<H", packed, p)[0]
        if flags & 0x18:
            base = ext = b""
            if flags & 8:
                e = packed.index(b"\0", p)
                base, p = packed[p:e], e + 1
            if flags & 0x10:
                e = packed.index(b"\0", p)
                ext, p = packed[p:e], e + 1
            name = (base + (b"." + ext if ext else b"")).decode("latin1")
        body = packed[start:]
        if method == 0:
            data = body
        elif method == 1:
            data = bytes(b ^ 0xFF for b in body)
        elif method == 2:
            data = _lzss(body, 4096 - 16)
        elif method == 3:
            data = _kwaj_lzh(body)
        else:
            raise ValueError("KWAJ method %d" % method)
        return (data[:size] if size is not None else data), name
    raise ValueError("not a COMPRESS file")


def main(argv):
    out = argv[argv.index("--out") + 1] if "--out" in argv else None
    files = [a for a in argv if not a.startswith("--") and a != out]
    for f in files:
        with open(f, "rb") as h:
            data, name = expand(h.read())
        stem = os.path.basename(f)
        target = name.upper() if name and "." in name else stem[:-1] + "?"
        if target.endswith("?"):
            target = stem
        dest = os.path.join(out or os.path.dirname(f), target)
        with open(dest, "wb") as h:
            h.write(data)
        print("%s -> %s (%d bytes)" % (f, dest, len(data)))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
