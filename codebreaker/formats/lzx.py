"""lzx.py -- Microsoft LZX (LZXD), the codec of CAB, WIM, XEX and XNA's compressed content, in Python.

    from codebreaker.formats import lzx
    lzx.lzxd_decompress(data, window_bits, out_len)       a raw LZXD stream (XNB: window_bits 16)
    lzx.lzx_decompress_xex(buf, window_size, first_block, out_len)   a XEX's normal-compressed image:
                                                          its block chain (each block's SHA-1 checked)

An LZ77 window and canonical Huffman trees (main, length, aligned offset), the algorithm libmspack's
lzxd.c implements, written from the format (reset interval 0, no delta). Decompressing a retail
default.xex gives byte for byte the image an uncompressed copy holds. The native xex program has its
own decoder for whole modules; this one is for what Python reads directly (XNA content).
"""
import hashlib
import struct

# -- LZX constants ---------------------------------------------------------
MIN_MATCH = 2
NUM_CHARS = 256
BLOCKTYPE_VERBATIM = 1
BLOCKTYPE_ALIGNED = 2
BLOCKTYPE_UNCOMPRESSED = 3
PRETREE_NUM = 20
ALIGNED_NUM = 8
NUM_PRIMARY_LENGTHS = 7
NUM_SECONDARY_LENGTHS = 249


def _pos_tables():
    """extra_bits[] and position_base[] (LZX position-slot decode tables)."""
    extra = [0] * 51
    j = 0
    for i in range(0, 51, 2):
        extra[i] = j
        if i + 1 < 51:
            extra[i + 1] = j
        if i != 0 and j < 17:
            j += 1
    base = [0] * 51
    j = 0
    for i in range(51):
        base[i] = j
        j += 1 << extra[i]
    return extra, base


_EXTRA, _BASE = _pos_tables()


# -- bit reader: 16-bit little-endian words, bits consumed MSB-first.
# 32-bit MSB-packed accumulator, faithfully mirroring the reference XEX LZX
# BitReader (XexTool-RE src/compress/lzx.cpp) so bit accounting matches exactly,
# including the per-frame align16 and the raw-pointer path for uncompressed
# blocks. read()/peek() handle n in [0,17] (the accumulator is 32 bits).
class _Bits:
    def __init__(self, data):
        self.d = data
        self.ln = len(data)
        self.p = 0          # byte index of next unread word
        self.buf = 0        # 32-bit accumulator, bits packed at the top
        self.n = 0          # valid bits currently in buf

    def _word(self):
        p = self.p
        w = (self.d[p] | (self.d[p + 1] << 8)) if p + 2 <= self.ln else \
            (self.d[p] if p < self.ln else 0)
        self.p = p + 2
        return w

    def _ensure(self, need):
        while self.n < need:
            self.buf = (self.buf | (self._word() << (16 - self.n))) & 0xFFFFFFFF
            self.n += 16

    def read(self, n):
        if n == 0:
            return 0
        self._ensure(n)
        v = self.buf >> (32 - n)
        self.buf = (self.buf << n) & 0xFFFFFFFF
        self.n -= n
        return v

    def read_big(self, n):
        if n <= 16:
            return self.read(n)
        hi = self.read(16)
        return (hi << (n - 16)) | self.read(n - 16)

    def peek(self, n):
        self._ensure(n)
        return self.buf >> (32 - n)

    def remove(self, n):
        self.buf = (self.buf << n) & 0xFFFFFFFF
        self.n -= n

    def align16(self):
        rem = self.n & 15
        if rem:
            self.read(rem)

    def drain_to_byte(self):
        """Rewind buffered whole words so raw_ptr() is the next unread byte
        (call only when 16-bit aligned)."""
        self.p -= self.n // 8
        self.n = 0
        self.buf = 0

    def raw_u32_le(self):
        w = self.raw()
        self.p += 4
        return w[0] | (w[1] << 8) | (w[2] << 16) | (w[3] << 24)

    def raw(self):
        return self.d[self.p:self.p + 4]


# -- canonical Huffman: fast lookup table (peek maxlen, index) --------------
def _build(lengths):
    maxlen = max((l for l in lengths if l), default=0)
    if maxlen == 0:
        return None, 0
    bl_count = [0] * (maxlen + 1)
    for l in lengths:
        if l:
            bl_count[l] += 1
    code = 0
    next_code = [0] * (maxlen + 1)
    for bits in range(1, maxlen + 1):
        code = (code + bl_count[bits - 1]) << 1
        next_code[bits] = code
    table = [(-1, 0)] * (1 << maxlen)
    for sym, l in enumerate(lengths):
        if not l:
            continue
        c = next_code[l]
        next_code[l] += 1
        shift = maxlen - l
        start = c << shift
        entry = (sym, l)
        for k in range(1 << shift):
            table[start + k] = entry
    return table, maxlen


def _sym(bits, tbl, maxlen):
    v = bits.peek(maxlen)
    sym, l = tbl[v]
    bits.remove(l)
    return sym


# -- READ_LENGTHS: update code-length table via the pretree -----------------
def _read_lengths(bits, lens, first, last):
    pre = [bits.read(4) for _ in range(PRETREE_NUM)]
    ptbl, pmax = _build(pre)
    i = first
    while i < last:
        z = _sym(bits, ptbl, pmax)
        if z == 17:
            run = bits.read(4) + 4
            for _ in range(run):
                lens[i] = 0
                i += 1
        elif z == 18:
            run = bits.read(5) + 20
            for _ in range(run):
                lens[i] = 0
                i += 1
        elif z == 19:
            run = bits.read(1) + 4
            z = _sym(bits, ptbl, pmax)
            val = lens[i] - z
            if val < 0:
                val += 17
            for _ in range(run):
                lens[i] = val
                i += 1
        else:
            val = lens[i] - z
            if val < 0:
                val += 17
            lens[i] = val
            i += 1


FRAME = 32768


def _num_position_slots(window_size):
    slots = 0
    while slots < 51 and _BASE[slots] < window_size:
        slots += 1
    return slots


def lzxd_decompress(data, window_bits, out_len, ref_data=None):
    """Decode a raw (de-blocked) LZXD stream to `out_len` bytes.

    Output is produced in 32 KB frames; the bitstream is realigned to a 16-bit
    boundary at each frame boundary, while block state (type, trees, repeated
    offsets) persists across frames and blocks — mirroring the reference XEX LZX
    decoder exactly (that per-frame align16 is the crux the CAB idiom omits).

    `ref_data` enables the LZX-DELTA mode used by .xexp patches: the window is
    pre-seeded with [zero padding][ref_data] up to window_size, so matches may
    reference the old image; frame/size counting stays relative to the seed end
    and only the newly produced bytes are returned. ref_data=None is the plain
    XEX path and is byte-for-byte unchanged (base 0)."""
    window_size = 1 << window_bits
    pos_slots = _num_position_slots(window_size)
    main_elements = NUM_CHARS + (pos_slots << 3)

    bits = _Bits(data)
    out = bytearray()
    if ref_data:
        pad = window_size - len(ref_data) if window_size > len(ref_data) else 0
        out.extend(bytes(pad))
        out.extend(ref_data)
    base = len(out)
    R0 = R1 = R2 = 1

    main_lens = [0] * main_elements
    len_lens = [0] * (NUM_SECONDARY_LENGTHS + 1)
    main_tbl = main_max = len_tbl = len_max = None
    aligned_tbl = aligned_max = None

    # E8 (Intel x86 call) preprocessing header — off for PowerPC XEX, but honour.
    intel_filesize = 0
    if bits.read(1):
        intel_filesize = bits.read_big(32)

    block_type = 0
    block_remaining = 0

    while len(out) - base < out_len:
        rel = len(out) - base
        fe = rel - (rel % FRAME) + FRAME
        if fe > out_len:
            fe = out_len
        frame_end = base + fe

        while len(out) < frame_end:
            if block_remaining == 0:
                block_type = bits.read(3)
                block_remaining = bits.read_big(24)
                if block_type == BLOCKTYPE_ALIGNED:
                    al = [bits.read(3) for _ in range(ALIGNED_NUM)]
                    aligned_tbl, aligned_max = _build(al)
                if block_type in (BLOCKTYPE_VERBATIM, BLOCKTYPE_ALIGNED):
                    _read_lengths(bits, main_lens, 0, NUM_CHARS)
                    _read_lengths(bits, main_lens, NUM_CHARS, main_elements)
                    main_tbl, main_max = _build(main_lens)
                    _read_lengths(bits, len_lens, 0, NUM_SECONDARY_LENGTHS)
                    len_tbl, len_max = _build(len_lens)
                elif block_type == BLOCKTYPE_UNCOMPRESSED:
                    bits.align16()
                    bits.drain_to_byte()
                    R0 = bits.raw_u32_le()
                    R1 = bits.raw_u32_le()
                    R2 = bits.raw_u32_le()
                else:
                    raise ValueError(f"LZX: bad block type {block_type}")

            want = frame_end - len(out)
            if want > block_remaining:
                want = block_remaining
            target = len(out) + want

            if block_type == BLOCKTYPE_UNCOMPRESSED:
                out += bits.d[bits.p:bits.p + want]
                bits.p += want
                block_remaining -= want
                if block_remaining == 0 and (bits.p & 1):
                    bits.p += 1                       # even-byte pad at block end
                continue

            while len(out) < target:
                main = _sym(bits, main_tbl, main_max)
                if main < NUM_CHARS:
                    out.append(main)
                    continue
                main -= NUM_CHARS
                length_header = main & 7
                if length_header == NUM_PRIMARY_LENGTHS:
                    match_len = _sym(bits, len_tbl, len_max) + NUM_PRIMARY_LENGTHS + MIN_MATCH
                else:
                    match_len = length_header + MIN_MATCH

                slot = main >> 3
                if slot == 0:
                    offset = R0
                elif slot == 1:
                    offset = R1
                    R1 = R0
                    R0 = offset
                elif slot == 2:
                    offset = R2
                    R2 = R0
                    R0 = offset
                else:
                    extra = _EXTRA[slot]
                    if block_type == BLOCKTYPE_ALIGNED and extra >= 3:
                        verb = bits.read(extra - 3) << 3
                        aln = _sym(bits, aligned_tbl, aligned_max)
                        offset = _BASE[slot] - 2 + verb + aln
                    else:
                        verb = bits.read(extra) if extra else 0
                        offset = _BASE[slot] - 2 + verb
                    R2 = R1
                    R1 = R0
                    R0 = offset

                if offset == 0 or offset > len(out):
                    raise ValueError("LZX: invalid match offset")
                src = len(out) - offset
                for _ in range(match_len):
                    out.append(out[src])
                    src += 1
            block_remaining -= want

        if len(out) - base < out_len:
            bits.align16()

    produced = out[base:base + out_len]
    if intel_filesize:
        _e8_decode(produced, intel_filesize)
    return bytes(produced)


def _e8_decode(out, filesize):
    """Undo LZX Intel-E8 call-target translation, in 32 KB frames (mirrors the
    reference undo_e8). PowerPC XEX images leave this off, but honour the flag."""
    if filesize == 0:
        return
    n = len(out)
    frame_start = 0
    frames = 0
    while frame_start < n and frames < 32768:
        frame_size = min(FRAME, n - frame_start)
        if frame_size > 10:
            end = frame_start + frame_size - 10
            i = frame_start
            while i < end:
                if out[i] != 0xE8:
                    i += 1
                    continue
                pos = struct.unpack("<i", struct.pack("<I", frame_start + (i - frame_start)))[0]
                v = struct.unpack_from("<i", out, i + 1)[0]
                if -pos <= v < filesize:
                    nv = (v - pos) if v >= 0 else (v + filesize)
                    struct.pack_into("<I", out, i + 1, nv & 0xFFFFFFFF)
                i += 5
        frame_start += FRAME
        frames += 1


# -- XEX block de-framing ---------------------------------------------------
def _deblock(buf, first_block, verify=True):
    """Strip the XEX per-block framing, returning the concatenated LZX stream.
    `first_block` is (block_size, block_hash20) from the file-format header."""
    stream = bytearray()
    p = 0
    cur_size, cur_hash = first_block
    while cur_size:
        block = buf[p:p + cur_size]
        if len(block) < cur_size:
            raise ValueError("LZX: truncated block")
        if verify:
            got = hashlib.sha1(block).digest()
            if got != cur_hash:
                raise ValueError("LZX: block SHA-1 mismatch (wrong decrypt key "
                                 "or corrupt image)")
        # first 24 bytes describe the NEXT block
        next_size = struct.unpack_from(">I", block, 0)[0]
        next_hash = bytes(block[4:24])
        q = 24
        while True:
            if q + 2 > len(block):
                break
            chunk = (block[q] << 8) | block[q + 1]
            q += 2
            if chunk == 0:
                break
            stream += block[q:q + chunk]
            q += chunk
        p += cur_size
        cur_size, cur_hash = next_size, next_hash
    return bytes(stream)


def lzx_decompress_xex(buf, window_size, first_block, out_len):
    """Full XEX 'normal compression' decode: de-block then LZXD."""
    if not first_block:
        raise ValueError("LZX: missing first-block descriptor")
    stream = _deblock(buf, first_block, verify=True)
    window_bits = window_size.bit_length() - 1  # log2 of a power-of-two window
    return lzxd_decompress(stream, window_bits, out_len)
