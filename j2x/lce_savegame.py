#!/usr/bin/env python3
"""
Xbox 360 Minecraft: Legacy Console Edition savegame unpacker.

Container:  [u32 version=0][u32 BE decompressed_size] then XMemCompress LZX chunks.
Chunk framing (per chunk):
    if first byte == 0xFF:  0xFF, u16 BE raw_size, u16 BE comp_size
    else:                   u16 BE comp_size,  raw_size defaults to 0x8000
Each chunk is an INDEPENDENT LZX stream that inflates to raw_size bytes.

LZX core reimplemented from the libmspack lzxd algorithm (MS-LZX / WIM flavour).
"""
import sys, struct

# ---- LZX constants -----------------------------------------------------------
MIN_MATCH = 2
NUM_CHARS = 256
BLOCKTYPE_VERBATIM   = 1
BLOCKTYPE_ALIGNED    = 2
BLOCKTYPE_UNCOMPRESSED = 3
PRETREE_NUM_ELEMENTS = 20
ALIGNED_NUM_ELEMENTS = 8
NUM_PRIMARY_LENGTHS  = 7
NUM_SECONDARY_LENGTHS = 249

# position-slot extra-bits / base tables ---------------------------------------
def _build_pos_tables(num_slots):
    extra = []
    for i in range(num_slots + 1):
        if i < 4:
            extra.append(0)
        else:
            extra.append(min((i >> 1) - 1, 17))
    base = [0] * (num_slots + 1)
    for i in range(1, num_slots + 1):
        base[i] = base[i - 1] + (1 << extra[i - 1])
    return extra, base

def _num_position_slots(window):
    extra = [0, 0, 0, 0]
    base = [0, 1, 2, 3]
    i = 4
    while base[-1] < window:
        e = min((i >> 1) - 1, 17)
        extra.append(e)
        base.append(base[-1] + (1 << extra[i - 1]))
        i += 1
    return len(base) - 1   # slots 0..(len-2)

# ---- LZX bit reader: 16-bit little-endian words, consumed MSB-first ----------
class BitReader:
    __slots__ = ("d", "p", "n", "acc", "nbits")
    def __init__(self, data):
        self.d = data
        self.n = len(data)
        self.p = 0
        self.acc = 0
        self.nbits = 0

    def _ensure(self, k):
        while self.nbits < k:
            p = self.p
            if p + 1 < self.n:
                w = self.d[p] | (self.d[p + 1] << 8)
            elif p < self.n:
                w = self.d[p]
            else:
                w = 0
            self.p = p + 2
            self.acc = (self.acc << 16) | w
            self.nbits += 16

    def bits(self, k):
        if k == 0:
            return 0
        if self.nbits < k:
            self._ensure(k)
        self.nbits -= k
        v = (self.acc >> self.nbits) & ((1 << k) - 1)
        self.acc &= (1 << self.nbits) - 1
        return v

    def peek(self, k):
        if self.nbits < k:
            self._ensure(k)
        return (self.acc >> (self.nbits - k)) & ((1 << k) - 1)

    def remove(self, k):
        self.nbits -= k
        self.acc &= (1 << self.nbits) - 1

    def align_to_16(self):
        # drop remaining bits in the current 16-bit word
        r = self.nbits & 15
        if r:
            self.remove(r)

    def read_raw_byte(self):
        # after 16-bit alignment, pull bytes straight from the stream buffer
        if self.nbits >= 8:
            self.nbits -= 8
            v = (self.acc >> self.nbits) & 0xFF
            self.acc &= (1 << self.nbits) - 1
            return v
        p = self.p
        b = self.d[p] if p < self.n else 0
        self.p = p + 1
        return b

# ---- canonical Huffman table (MSB-first) ------------------------------------
class Huffman:
    __slots__ = ("maxlen", "table", "lens")
    def __init__(self, lens):
        self.lens = lens
        maxlen = 0
        for L in lens:
            if L > maxlen:
                maxlen = L
        self.maxlen = maxlen
        if maxlen == 0:
            self.table = None
            return
        size = 1 << maxlen
        table = [0] * size            # packs (sym<<5)|len ; len<=16 fits in 5 bits
        code = 0
        # assign canonical codes ordered by (length, symbol)
        by_len = [[] for _ in range(maxlen + 1)]
        for sym, L in enumerate(lens):
            if L:
                by_len[L].append(sym)
        for L in range(1, maxlen + 1):
            for sym in by_len[L]:
                start = code << (maxlen - L)
                end = start + (1 << (maxlen - L))
                entry = (sym << 5) | L
                for j in range(start, end):
                    table[j] = entry
                code += 1
            code <<= 1
        self.table = table

    def decode(self, br):
        if br.nbits < self.maxlen:
            br._ensure(self.maxlen)
        peek = (br.acc >> (br.nbits - self.maxlen)) & ((1 << self.maxlen) - 1)
        entry = self.table[peek]
        L = entry & 31
        br.nbits -= L
        br.acc &= (1 << br.nbits) - 1
        return entry >> 5

# ---- length-table reader (pretree RLE, delta mod 17) ------------------------
def read_lengths(lens, first, last, br):
    pre_lens = [br.bits(4) for _ in range(PRETREE_NUM_ELEMENTS)]
    pre = Huffman(pre_lens)
    i = first
    while i < last:
        sym = pre.decode(br)
        if sym == 17:
            run = br.bits(4) + 4
            while run and i < last:
                lens[i] = 0; i += 1; run -= 1
        elif sym == 18:
            run = br.bits(5) + 20
            while run and i < last:
                lens[i] = 0; i += 1; run -= 1
        elif sym == 19:
            run = br.bits(1) + 4
            sym2 = pre.decode(br)
            val = (lens[i] - sym2) % 17
            while run and i < last:
                lens[i] = val; i += 1; run -= 1
        else:
            lens[i] = (lens[i] - sym) % 17
            i += 1

# ---- LZX decode: frame-based (32 KB output frames, 16-bit realign per frame) -
FRAME = 0x8000

def lzx_decompress_chunk(data, out_len, num_slots, extra, base):
    br = BitReader(data)
    out = bytearray()
    R0 = R1 = R2 = 1
    main_elems = NUM_CHARS + (num_slots << 3)
    main_lens = [0] * main_elems
    length_lens = [0] * (NUM_SECONDARY_LENGTHS + 1)
    main = length = aligned = None
    block_type = 0
    block_length = 0
    block_remaining = 0
    header_read = False
    e8 = 0
    e8_size = 0

    while len(out) < out_len:
        # ---- frame start ----
        if not header_read:
            e8 = br.bits(1)
            if e8:
                hi = br.bits(16); lo = br.bits(16)
                e8_size = (hi << 16) | lo
            header_read = True

        frame_size = min(FRAME, out_len - len(out))
        target = len(out) + frame_size

        while len(out) < target:
            if block_remaining == 0:
                # realign after an odd-length uncompressed block
                if block_type == BLOCKTYPE_UNCOMPRESSED and (block_length & 1):
                    br.read_raw_byte()
                block_type = br.bits(3)
                block_length = (br.bits(8) << 16) | (br.bits(8) << 8) | br.bits(8)
                block_remaining = block_length

                if block_type == BLOCKTYPE_ALIGNED:
                    aln_lens = [br.bits(3) for _ in range(ALIGNED_NUM_ELEMENTS)]
                    aligned = Huffman(aln_lens)
                    read_lengths(main_lens, 0, NUM_CHARS, br)
                    read_lengths(main_lens, NUM_CHARS, main_elems, br)
                    main = Huffman(main_lens)
                    read_lengths(length_lens, 0, NUM_SECONDARY_LENGTHS, br)
                    length = Huffman(length_lens)
                elif block_type == BLOCKTYPE_VERBATIM:
                    aligned = None
                    read_lengths(main_lens, 0, NUM_CHARS, br)
                    read_lengths(main_lens, NUM_CHARS, main_elems, br)
                    main = Huffman(main_lens)
                    read_lengths(length_lens, 0, NUM_SECONDARY_LENGTHS, br)
                    length = Huffman(length_lens)
                elif block_type == BLOCKTYPE_UNCOMPRESSED:
                    br.align_to_16()
                    R0 = (br.read_raw_byte() | (br.read_raw_byte() << 8)
                          | (br.read_raw_byte() << 16) | (br.read_raw_byte() << 24))
                    R1 = (br.read_raw_byte() | (br.read_raw_byte() << 8)
                          | (br.read_raw_byte() << 16) | (br.read_raw_byte() << 24))
                    R2 = (br.read_raw_byte() | (br.read_raw_byte() << 8)
                          | (br.read_raw_byte() << 16) | (br.read_raw_byte() << 24))
                else:
                    raise ValueError("bad block type %d at out=%d" % (block_type, len(out)))

            this_run = block_remaining
            if this_run > target - len(out):
                this_run = target - len(out)
            block_remaining -= this_run

            if block_type == BLOCKTYPE_UNCOMPRESSED:
                for _ in range(this_run):
                    out.append(br.read_raw_byte())
                continue

            produced = 0
            while produced < this_run:
                sym = main.decode(br)
                if sym < NUM_CHARS:
                    out.append(sym)
                    produced += 1
                    continue
                sym -= NUM_CHARS
                len_head = sym & 7
                pos_slot = sym >> 3
                if len_head == NUM_PRIMARY_LENGTHS:
                    match_len = length.decode(br) + NUM_PRIMARY_LENGTHS + MIN_MATCH
                else:
                    match_len = len_head + MIN_MATCH

                if pos_slot == 0:
                    offset = R0
                elif pos_slot == 1:
                    offset = R1; R1 = R0; R0 = offset
                elif pos_slot == 2:
                    offset = R2; R2 = R0; R0 = offset
                else:
                    eb = extra[pos_slot]
                    if aligned is not None and eb >= 3:
                        verb = br.bits(eb - 3) << 3
                        aln = aligned.decode(br)
                        offset = base[pos_slot] + verb + aln - 2
                    else:
                        verb = br.bits(eb)
                        offset = base[pos_slot] + verb - 2
                    R2 = R1; R1 = R0; R0 = offset

                src = len(out) - offset
                for _ in range(match_len):
                    out.append(out[src])
                    src += 1
                produced += match_len
            # a match may cross the frame target; account for the overshoot
            if produced > this_run:
                block_remaining -= (produced - this_run)

        # ---- end of frame: realign input bitstream to a 16-bit boundary ----
        if br.nbits > 0:
            br._ensure(16)
        rem = br.nbits & 15
        if rem:
            br.remove(rem)

    del out[out_len:]
    if e8 and out_len > 10:
        _undo_e8(out, out_len, e8_size)
    return bytes(out)

def _undo_e8(out, size, e8_size):
    # LZX intel-call translation, 32KB frames, first e8_size bytes only
    i = 0
    limit = min(size - 10, 0x8000 - 10) if size >= 10 else 0
    while i < limit:
        if out[i] == 0xE8:
            rel = struct.unpack_from("<i", out, i + 1)[0]
            if -i <= rel < e8_size:
                if rel >= 0:
                    abs_ = rel - i
                else:
                    abs_ = rel + e8_size
                struct.pack_into("<i", out, i + 1, abs_ & 0xFFFFFFFF)
            i += 5
        else:
            i += 1

# ---- container --------------------------------------------------------------
def unpack(path):
    d = open(path, "rb").read()
    ver = struct.unpack_from(">I", d, 0)[0]
    dec_size = struct.unpack_from(">I", d, 4)[0]
    window = 0x20000                      # XMemCompress LZX window = 128 KB
    num_slots = _num_position_slots(window)
    extra, base = _build_pos_tables(num_slots)

    # Concatenate every chunk's compressed payload into one continuous LZX
    # bitstream (framing is 32 KB output frames; LZX blocks span frames).
    payload = bytearray()
    off = 8
    n = len(d)
    ci = 0
    while off < n:
        b = d[off]
        if b == 0xFF:
            comp = struct.unpack_from(">H", d, off + 3)[0]
            off += 5
        else:
            comp = struct.unpack_from(">H", d, off)[0]
            off += 2
        if comp == 0:
            break
        payload += d[off:off + comp]
        off += comp
        ci += 1

    sys.stderr.write("version=%d  decompressed_size=%d  window=%#x  slots=%d  chunks=%d  comp_payload=%d\n"
                     % (ver, dec_size, window, num_slots, ci, len(payload)))

    out = lzx_decompress_chunk(bytes(payload), dec_size, num_slots, extra, base)
    sys.stderr.write("produced=%d / %d\n" % (len(out), dec_size))
    return out

if __name__ == "__main__":
    src = sys.argv[1]
    dst = sys.argv[2] if len(sys.argv) > 2 else src + ".decompressed"
    data = unpack(src)
    open(dst, "wb").write(data)
    sys.stderr.write("wrote %s (%d bytes)\n" % (dst, len(data)))
