"""pklite.py -- read and write a program the way PKLITE 1.15 (its "large" model) stores it.

    from codebreaker.build import pklite
    s = pklite.read(stored_module)        .image .relocs .regs (cs, ip, ss, sp) .tokens .stub .start .end
    tokens = pklite.parse(image, stub)    the choices PKLITE makes for a program (see parse)
    stream = pklite.encode(tokens, stub)  the compressed stream of those tokens
    module = pklite.pack(image, relocs, regs, stub, tail)    the stored load module

The format is read off PKLITE's own decompressor, which is the first thing in every file it
packed (the entry point): it moves itself and the stream to the top of memory and expands the
program down to where DOS loaded it, then relocates it and jumps to it. The stream is read
through one pointer that serves two things: a bit stream of 16-bit words, lowest bit first,
the next word fetched AS SOON AS the 16th bit of one is taken; and single bytes, read in between.

    bit 0             a literal: the next byte
    bit 1             a copy: a length code; for length 2 one byte of distance, else a bit -- 1:
                      the distance is one byte; 0: a code for its high byte, then its low byte
    length 25 + n     the length code for 25 reads a byte n: a copy of 25 + n (n < FE); FE: the
                      pointers are renormalised (output past ~32K of a segment), nothing copied;
                      FF: the end
    after the end     the relocation table -- runs of a count byte, a segment word and that many
                      offset words, a zero count ending it -- then SS, SP, CS, IP (relative)

The two prefix codes are the decompressor's: it reads bits into BX and looks values up in
tables inside its own code (the lookups may run from one table into the next, as the CPU's
do). They are derived here by walking the decompressor's decision tree, with the tables taken
from the stub being written -- the stub is PKLITE's code, carried as the program's (see
dos_writer: it is written as source like every other part of the file).

PKLITE's choices (parse): every copy is the longest match within 8191 bytes, the nearest of
equals; a length-2 copy only within 255 bytes; a copy is put off by one byte (a literal
instead) when the next byte starts a longer one; a renormalisation mark comes before the first
token at or past each 0x8000 + k * 0xA000 bytes of output less 277 (the longest copy).
"""
import struct

STUB_CODE = 0x44            # the decompressor's code in the load module (copied to offset 0 to run)
MAX_DISTANCE = 0x1FFF
MAX_LENGTH = 25 + 0xFC                # (the code allows FD; PKLITE never makes one longer than 277)
_FIRST_MARK, _MARK_EVERY = 0x8000, 0xA000


class _Out(Exception):
    pass


class _Bits(object):
    """The decompressor's reading: words of bits and single bytes through one pointer."""
    def __init__(self, data, at):
        self.d, self.si = data, at
        self.bp, self.dx = self.word(), 16

    def word(self):
        if self.si + 2 > len(self.d):
            raise _Out()
        w = struct.unpack_from("<H", self.d, self.si)[0]
        self.si += 2
        return w

    def byte(self):
        if self.si >= len(self.d):
            raise _Out()
        self.si += 1
        return self.d[self.si - 1]

    def bit(self):
        cf = self.bp & 1
        self.bp >>= 1
        self.dx -= 1
        if self.dx == 0:
            self.bp, self.dx = self.word(), 16
        return cf


def _length(bit, t):
    """The length code, as the decompressor reads it (t: its code from offset 0)."""
    bx = bit() << 1 | bit()
    if bx >= 2:
        return bx
    bx = bx << 1 | bit()
    if bx == 0:
        return t[0x209]
    bx = bx << 1 | bit()
    if bx < 5:
        return t[0x209 + bx]
    bx = bx << 1 | bit()
    if bx <= 0xC:
        return t[0x209 + bx]
    bx = (bx & 3) << 1 | bit()
    if bx >= 5:
        bx = bx << 1 | bit()
        if bx > 0xC:
            bx = (bx & 3) << 1 | bit()
            if bx >= 5:
                bx = bx << 1 | bit()
            return t[0x21F + bx]
    return t[0x214 + bx]


def _high(bit, t):
    """The code for a distance's high byte (after the bit that says it has one)."""
    bx = bit() << 2 | bit() << 1 | bit()
    if bx < 2:
        return t[0x22F + bx]
    bx = bx << 1 | bit()
    if bx < 8:
        return t[0x22F + bx]
    bx = bx << 1 | bit()
    if bx < 0x17:
        return t[0x22F + bx]
    bx = bx << 1 | bit()
    return bx & 0xDF


def _codes(read, t):
    """{value: bit string} of a prefix code, by feeding the reader every string of bits."""
    out = {}

    def walk(prefix):
        it = iter(prefix)

        def bit():
            try:
                return next(it)
            except StopIteration:
                raise _Out()
        try:
            v = read(bit, t)
        except _Out:
            if len(prefix) < 12:
                walk(prefix + [0])
                walk(prefix + [1])
            return
        out.setdefault(v, tuple(prefix))
    walk([])
    return out


class Stub(object):
    """PKLITE's decompressor as it is in one file: its code and the codes its tables make."""
    def __init__(self, module, start):
        self.head = bytes(module[:start])              # entry code, decompressor, up to the stream
        code = module[STUB_CODE:]
        self.lengths = _codes(_length, code)
        self.highs = _codes(_high, code)
        self.code = code


class Stored(object):
    """A load module PKLITE stored, read: see read()."""


def _decode(stub, data, at):
    """-> (image, tokens, end of stream). Tokens: ("lit", b) ("copy", n, d) ("seg",) ("end",)"""
    r, t = _Bits(data, at), stub.code
    out, tokens = bytearray(), []
    while True:
        if not r.bit():
            out.append(r.byte())
            tokens.append(("lit", out[-1]))
            continue
        n = _length(r.bit, t)
        if n == 25:
            b = r.byte()
            if b == 0xFE:
                tokens.append(("seg",))
                continue
            if b == 0xFF:
                tokens.append(("end",))
                return bytes(out), tokens, r.si
            n += b
        hi = 0 if n == 2 or r.bit() else _high(r.bit, t)
        d = hi << 8 | r.byte()
        for _ in range(n):
            out.append(out[len(out) - d])
        tokens.append(("copy", n, d))


def stream_start(module):
    """Where the stream starts: the paragraph the entry code puts DS at (mov bx,ds / add bx,N)."""
    at = module.find(b"\x8c\xdb\x53\x81\xc3")
    if at < 0:
        raise ValueError("not PKLITE's decompressor (no start of the stream found)")
    return struct.unpack_from("<H", module, at + 5)[0] * 16 - 0x100


def read(module):
    """A load module stored by PKLITE 1.15 large. -> Stored"""
    start = stream_start(module)
    stub = Stub(module, start)
    image, tokens, end = _decode(stub, module, start)
    s = Stored()
    s.stub, s.start, s.end, s.image, s.tokens = stub, start, end, image, tokens
    relocs, at = [], end
    while True:
        n = module[at]
        at += 1
        if not n:
            break
        seg = struct.unpack_from("<H", module, at)[0]
        offs = struct.unpack_from("<%dH" % n, module, at + 2)
        relocs.append((seg, list(offs)))
        at += 2 + 2 * n
    ss, sp, cs, ip = struct.unpack_from("<4H", module, at)
    s.relocs, s.regs, s.tail = relocs, (cs, ip, ss, sp), bytes(module[at + 8:])
    return s


class _Writer(object):
    """The stream as the decompressor will read it: a word is reserved where the reading will
    fetch it -- at the start, and right after the byte that follows the 16th bit of the last."""
    def __init__(self):
        self.out = bytearray(2)
        self.slot, self.n, self.word = 0, 0, 0

    def bit(self, b):
        self.word |= b << self.n
        self.n += 1
        if self.n == 16:
            struct.pack_into("<H", self.out, self.slot, self.word)
            self.slot, self.n, self.word = len(self.out), 0, 0
            self.out += b"\0\0"

    def bits(self, code):
        for b in code:
            self.bit(b)

    def byte(self, b):
        self.out.append(b)

    def done(self):
        struct.pack_into("<H", self.out, self.slot, self.word)
        return bytes(self.out)


def encode(tokens, stub):
    w = _Writer()
    encode_into(w, tokens, stub)
    return w.done()


def encode_into(w, tokens, stub):
    for t in tokens:
        if t[0] == "lit":
            w.bit(0)
            w.byte(t[1])
            continue
        w.bit(1)
        if t[0] in ("seg", "end"):
            w.bits(stub.lengths[25])
            w.byte(0xFE if t[0] == "seg" else 0xFF)
            continue
        n, d = t[1], t[2]
        if n >= 25:
            w.bits(stub.lengths[25])
            w.byte(n - 25)
        else:
            w.bits(stub.lengths[n])
        if n != 2:
            if d < 0x100:
                w.bit(1)
            else:
                w.bit(0)
                w.bits(stub.highs[d >> 8])
        w.byte(d & 0xFF)


def _matcher(data):
    """longest(pos) -> (length, distance), as PKLITE finds it: every earlier place with the same
    three bytes is a candidate, nearest first, and of equal lengths the nearest -- except at the
    full length a copy can have there (277, or what is left), where it is the FARTHEST (so inside
    a long run of one byte the copy comes from where the run began); a candidate farther than
    MAX_DISTANCE is taken as MAX_DISTANCE back. Length 2 only within 255. (0, 0): none."""
    n = len(data)
    head, chain, latest = [None] * n, [None] * n, {}   # head[p]: the last q < p with p's 3 bytes
    for p in range(n - 2):
        k = data[p:p + 3]
        head[p] = chain[p] = latest.get(k)
        latest[k] = p

    def longest(pos):
        best, bd = 0, 0
        lim = min(MAX_LENGTH, n - pos)
        q = head[pos] if lim >= 3 else None
        while q is not None:
            if pos - q > MAX_DISTANCE:
                q = pos - MAX_DISTANCE
            l = 0
            while l < lim and data[q + l] == data[pos + l]:
                l += 1
            if l > best or l == lim:
                best, bd = l, pos - q
            if pos - q >= MAX_DISTANCE:
                break
            q = chain[q]
        if best >= 3:
            return best, bd
        if lim >= 2:
            for d in range(1, min(pos, 255) + 1):
                if data[pos - d] == data[pos] and data[pos - d + 1] == data[pos + 1]:
                    return 2, d
        return 0, 0
    return longest


def _put_off(cur, nxt):
    """Whether PKLITE puts a copy off by a byte: the next one is two or more longer, or one
    longer and this one comes from more than 128 back."""
    return nxt[0] >= cur[0] + 2 or (nxt[0] == cur[0] + 1 and cur[1] > 128)


def parse(image):
    """The tokens PKLITE makes for a program (see the top)."""
    data = bytes(image)
    longest = _matcher(data)
    tokens, pos, mark = [], 0, _FIRST_MARK
    cur = longest(0)
    while pos < len(data):
        if pos >= mark - MAX_LENGTH:
            tokens.append(("seg",))
            mark += _MARK_EVERY
        n, d = cur
        nxt = longest(pos + 1) if pos + 1 < len(data) else (0, 0)
        if n >= 2 and not _put_off(cur, nxt):
            tokens.append(("copy", n, d))
            pos += n
            cur = longest(pos) if pos < len(data) else (0, 0)
        else:
            tokens.append(("lit", data[pos]))
            pos += 1
            cur = nxt
    tokens.append(("end",))
    return tokens


def _relocation_table(relocs):
    """The table: the relocations in the order given, (segment, offset) pairs, a run of the same
    segment as one entry of at most 255 offsets."""
    out, i = bytearray(), 0
    while i < len(relocs):
        seg = relocs[i][0]
        j = i
        while j < len(relocs) and relocs[j][0] == seg and j - i < 255:
            j += 1
        out += struct.pack("<BH", j - i, seg) + b"".join(struct.pack("<H", off) for _s, off in relocs[i:j])
        i = j
    return bytes(out) + b"\0"


def pack(image, relocs, regs, head, memory):
    """The program `image` with its relocations [(segment, offset)] in the order its table lists
    them, registers (cs, ip, ss, sp), and `memory` -- the paragraphs it asks for beyond itself
    (the min-alloc of its own header) -- stored as PKLITE stores it, behind `head` (PKLITE's entry
    code and decompressor, STREAM_START bytes). -> (load module, header) where header is
    {cs, ip, ss, sp, min_alloc, relocs}: the file's entry is PKLITE's, with one relocation (the
    segment its entry code adds to the memory it needs)."""
    tokens = parse(image)
    stub = Stub(bytes(head), len(head))
    cs, ip, ss, sp = regs
    body = encode(tokens, stub) + _relocation_table(relocs) + struct.pack("<4H", ss, sp, cs, ip)
    module = bytearray(head) + body
    paras = (len(module) + 15) // 16
    # the memory it needs, relative to where it is loaded: the program and what it asks for
    # beyond itself, less the program prefix -- and never so little that the expanding program
    # would overrun the stream before it is read (it is moved to just below the decompressor)
    need = (len(image) + 15) // 16 + memory - 0x10
    out = reach = 0
    w = _Writer()
    for t in tokens:                                   # how far the output runs ahead of the input
        reach = max(reach, out - len(w.out))
        out += 1 if t[0] == "lit" else t[1] if t[0] == "copy" else 0
        encode_into(w, [t], stub)
    safe = paras + (max(reach, 0) + 15) // 16 + 0x25
    top = max(need, safe)
    struct.pack_into("<H", module, 1, top)               # mov ax, top
    struct.pack_into("<H", module, 4, paras)             # mov dx, paras
    header = dict(cs=0xFFF0, ip=0x100, ss=len(module) // 16 + 9, sp=0x200,
                  min_alloc=top + 0x10 - len(module) // 16, relocs=[(7, 0)])
    return bytes(module), header

