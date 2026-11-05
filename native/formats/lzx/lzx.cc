// lzx.cc -- Microsoft LZX (LZXD): codebreaker/formats/lzx.py, line for line.
//
// An LZ77 window and canonical Huffman trees (main, length, aligned offset), the bit reader the
// reference XEX decoder uses (16-bit little-endian words read MSB-first into a 32-bit accumulator,
// a 16-bit realign at every 32 KB frame), the Intel E8 translation undone, and the XEX block chain
// (each block's SHA-1 checked).  A damaged stream raises what the Python raised.
#include "lzx.h"

#include <string>
#include <vector>

#include "../core/questions.h"
#include "crypto/sha1.h"

namespace cbf::lzx {
namespace {

constexpr int kMinMatch = 2, kNumChars = 256, kVerbatim = 1, kAligned = 2, kUncompressed = 3;
constexpr int kPretreeNum = 20, kAlignedNum = 8, kPrimaryLengths = 7, kSecondaryLengths = 249;
constexpr int64_t kFrame = 32768;

struct Tables {
    int extra[51];
    int64_t base[51];
    Tables() {
        int j = 0;
        for (int i = 0; i < 51; i += 2) {
            extra[i] = j;
            if (i + 1 < 51) extra[i + 1] = j;
            if (i != 0 && j < 17) ++j;
        }
        int64_t b = 0;
        for (int i = 0; i < 51; ++i) {
            base[i] = b;
            b += int64_t(1) << extra[i];
        }
    }
};
const Tables &tables() {
    static const Tables t;
    return t;
}

struct BitReader {
    Bytes d;
    int64_t ln, p = 0;
    uint32_t buf = 0;
    int n = 0;
    explicit BitReader(Bytes data) : d(data), ln(data.size()) {}

    uint32_t word() {
        uint32_t w = p + 2 <= ln ? (d.data()[p] | (d.data()[p + 1] << 8)) : (p < ln ? d.data()[p] : 0);
        p += 2;
        return w;
    }
    void ensure(int need) {
        while (n < need) {
            if (16 - n < 0) raise("ValueError", "negative shift count");
            buf = static_cast<uint32_t>((static_cast<uint64_t>(buf) | (static_cast<uint64_t>(word()) << (16 - n))) & 0xFFFFFFFFu);
            n += 16;
        }
    }
    uint32_t read(int k) {
        if (k == 0) return 0;
        ensure(k);
        uint32_t v = static_cast<uint32_t>(static_cast<uint64_t>(buf) >> (32 - k));
        buf = static_cast<uint32_t>((static_cast<uint64_t>(buf) << k) & 0xFFFFFFFFu);
        n -= k;
        return v;
    }
    uint64_t read_big(int k) {
        if (k <= 16) return read(k);
        uint64_t hi = read(16);
        return (hi << (k - 16)) | read(k - 16);
    }
    uint32_t peek(int k) {
        ensure(k);
        return static_cast<uint32_t>(static_cast<uint64_t>(buf) >> (32 - k));
    }
    void remove(int k) {
        buf = static_cast<uint32_t>((static_cast<uint64_t>(buf) << k) & 0xFFFFFFFFu);
        n -= k;
    }
    void align16() {
        int rem = n & 15;
        if (rem) read(rem);
    }
    void drain_to_byte() {
        p -= n / 8;           // n is never negative here
        n = 0;
        buf = 0;
    }
    uint32_t raw_u32_le() {
        Bytes w = d.slice(p, p + 4);
        p += 4;
        return w.at(0) | (w.at(1) << 8) | (w.at(2) << 16) | (static_cast<uint32_t>(w.at(3)) << 24);
    }
};

// a canonical Huffman table: peek maxlen bits, index -> (symbol, length); none when every length is 0
struct Table {
    bool none = true;
    int maxlen = 0;
    std::vector<std::pair<int, int>> t;
};

Table build(const std::vector<int64_t> &lengths) {
    Table out;
    int64_t maxlen = 0;
    for (int64_t l : lengths)
        if (l && l > maxlen) maxlen = l;
    if (maxlen == 0) return out;
    if (maxlen > 30) raise("MemoryError", "");
    std::vector<int64_t> bl_count(static_cast<size_t>(maxlen + 1), 0);
    for (int64_t l : lengths)
        if (l) {
            if (l < 0) raise("IndexError", "list index out of range");
            bl_count[static_cast<size_t>(l)] += 1;
        }
    int64_t code = 0;
    std::vector<int64_t> next_code(static_cast<size_t>(maxlen + 1), 0);
    for (int64_t bits = 1; bits <= maxlen; ++bits) {
        code = (code + bl_count[static_cast<size_t>(bits - 1)]) << 1;
        next_code[static_cast<size_t>(bits)] = code;
    }
    out.t.assign(size_t(1) << maxlen, {-1, 0});
    for (size_t sym = 0; sym < lengths.size(); ++sym) {
        int64_t l = lengths[sym];
        if (!l) continue;
        int64_t c = next_code[static_cast<size_t>(l)]++;
        int64_t shift = maxlen - l;
        int64_t start = c << shift;
        for (int64_t k = 0; k < (int64_t(1) << shift); ++k) {
            int64_t at = start + k;
            if (at >= static_cast<int64_t>(out.t.size())) raise("IndexError", "list assignment index out of range");
            out.t[static_cast<size_t>(at)] = {static_cast<int>(sym), static_cast<int>(l)};
        }
    }
    out.none = false;
    out.maxlen = static_cast<int>(maxlen);
    return out;
}

int sym(BitReader &bits, const Table &tbl) {
    uint32_t v = bits.peek(tbl.maxlen);
    if (tbl.none) raise("TypeError", "'NoneType' object is not subscriptable");
    auto e = tbl.t[v];
    bits.remove(e.second);
    return e.first;
}

int64_t &lat(std::vector<int64_t> &v, int64_t i) {
    if (i < 0) i += static_cast<int64_t>(v.size());
    if (i < 0 || i >= static_cast<int64_t>(v.size())) raise("IndexError", "list index out of range");
    return v[static_cast<size_t>(i)];
}

void read_lengths(BitReader &bits, std::vector<int64_t> &lens, int64_t first, int64_t last) {
    std::vector<int64_t> pre(kPretreeNum);
    for (auto &p : pre) p = bits.read(4);
    Table ptbl = build(pre);
    int64_t i = first;
    while (i < last) {
        int z = sym(bits, ptbl);
        if (z == 17) {
            int64_t run = bits.read(4) + 4;
            for (int64_t k = 0; k < run; ++k) lat(lens, i++) = 0;
        } else if (z == 18) {
            int64_t run = bits.read(5) + 20;
            for (int64_t k = 0; k < run; ++k) lat(lens, i++) = 0;
        } else if (z == 19) {
            int64_t run = bits.read(1) + 4;
            z = sym(bits, ptbl);
            int64_t val = lat(lens, i) - z;
            if (val < 0) val += 17;
            for (int64_t k = 0; k < run; ++k) lat(lens, i++) = val;
        } else {
            int64_t val = lat(lens, i) - z;
            if (val < 0) val += 17;
            lat(lens, i) = val;
            i += 1;
        }
    }
}

void e8_decode(std::vector<uint8_t> &out, int64_t filesize) {
    if (filesize == 0) return;
    int64_t n = static_cast<int64_t>(out.size());
    int64_t frame_start = 0, frames = 0;
    while (frame_start < n && frames < 32768) {
        int64_t frame_size = kFrame < n - frame_start ? kFrame : n - frame_start;
        if (frame_size > 10) {
            int64_t end = frame_start + frame_size - 10;
            int64_t i = frame_start;
            while (i < end) {
                if (out[static_cast<size_t>(i)] != 0xE8) {
                    ++i;
                    continue;
                }
                int64_t pos = static_cast<int32_t>(static_cast<uint32_t>(i));
                Bytes view(out);
                int64_t v = view.i32(i + 1);
                if (-pos <= v && v < filesize) {
                    int64_t nv = v >= 0 ? v - pos : v + filesize;
                    uint32_t w = static_cast<uint32_t>(nv);
                    for (int k = 0; k < 4; ++k) out[static_cast<size_t>(i + 1 + k)] = static_cast<uint8_t>(w >> (8 * k));
                }
                i += 5;
            }
        }
        frame_start += kFrame;
        frames += 1;
    }
}

}  // namespace

std::vector<uint8_t> lzxd_decompress(Bytes data, int64_t window_bits, int64_t out_len, const Bytes *ref_data) {
    const Tables &T = tables();
    if (window_bits < 0 || window_bits > 40) raise("OverflowError", "an int past 64 bits");
    int64_t window_size = int64_t(1) << window_bits;
    int pos_slots = 0;
    while (pos_slots < 51 && T.base[pos_slots] < window_size) ++pos_slots;
    int64_t main_elements = kNumChars + (static_cast<int64_t>(pos_slots) << 3);

    BitReader bits(data);
    std::vector<uint8_t> out;
    if (ref_data && !ref_data->empty()) {
        int64_t pad = window_size > ref_data->size() ? window_size - ref_data->size() : 0;
        out.assign(static_cast<size_t>(pad), 0);
        out.insert(out.end(), ref_data->begin(), ref_data->end());
    }
    const int64_t base = static_cast<int64_t>(out.size());
    int64_t R0 = 1, R1 = 1, R2 = 1;
    std::vector<int64_t> main_lens(static_cast<size_t>(main_elements), 0), len_lens(kSecondaryLengths + 1, 0);
    Table main_tbl, len_tbl, aligned_tbl;

    int64_t intel_filesize = 0;
    if (bits.read(1)) intel_filesize = static_cast<int64_t>(bits.read_big(32));
    int64_t block_type = 0, block_remaining = 0;
    auto size = [&]() { return static_cast<int64_t>(out.size()); };

    while (size() - base < out_len) {
        int64_t rel = size() - base;
        int64_t fe = rel - (rel % kFrame) + kFrame;
        if (fe > out_len) fe = out_len;
        int64_t frame_end = base + fe;
        while (size() < frame_end) {
            if (block_remaining == 0) {
                block_type = bits.read(3);
                block_remaining = static_cast<int64_t>(bits.read_big(24));
                if (block_type == kAligned) {
                    std::vector<int64_t> al(kAlignedNum);
                    for (auto &a : al) a = bits.read(3);
                    aligned_tbl = build(al);
                }
                if (block_type == kVerbatim || block_type == kAligned) {
                    read_lengths(bits, main_lens, 0, kNumChars);
                    read_lengths(bits, main_lens, kNumChars, main_elements);
                    main_tbl = build(main_lens);
                    read_lengths(bits, len_lens, 0, kSecondaryLengths);
                    len_tbl = build(len_lens);
                } else if (block_type == kUncompressed) {
                    bits.align16();
                    bits.drain_to_byte();
                    R0 = bits.raw_u32_le();
                    R1 = bits.raw_u32_le();
                    R2 = bits.raw_u32_le();
                } else {
                    raise("ValueError", "LZX: bad block type " + std::to_string(block_type));
                }
            }
            int64_t want = frame_end - size();
            if (want > block_remaining) want = block_remaining;
            int64_t target = size() + want;
            if (block_type == kUncompressed) {
                Bytes piece = bits.d.slice(bits.p, bits.p + want);
                out.insert(out.end(), piece.begin(), piece.end());
                bits.p += want;
                block_remaining -= want;
                if (block_remaining == 0 && (bits.p & 1)) bits.p += 1;
                continue;
            }
            while (size() < target) {
                int64_t main = sym(bits, main_tbl);
                if (main < kNumChars) {
                    if (main < 0) raise("ValueError", "byte must be in range(0, 256)");
                    out.push_back(static_cast<uint8_t>(main));
                    continue;
                }
                main -= kNumChars;
                int64_t length_header = main & 7, match_len;
                if (length_header == kPrimaryLengths) match_len = sym(bits, len_tbl) + kPrimaryLengths + kMinMatch;
                else match_len = length_header + kMinMatch;
                int64_t slot = main >> 3, offset;
                if (slot == 0) {
                    offset = R0;
                } else if (slot == 1) {
                    offset = R1;
                    R1 = R0;
                    R0 = offset;
                } else if (slot == 2) {
                    offset = R2;
                    R2 = R0;
                    R0 = offset;
                } else {
                    if (slot >= 51) raise("IndexError", "list index out of range");
                    int extra = T.extra[slot];
                    if (block_type == kAligned && extra >= 3) {
                        int64_t verb = static_cast<int64_t>(bits.read(extra - 3)) << 3;
                        int64_t aln = sym(bits, aligned_tbl);
                        offset = T.base[slot] - 2 + verb + aln;
                    } else {
                        int64_t verb = extra ? bits.read(extra) : 0;
                        offset = T.base[slot] - 2 + verb;
                    }
                    R2 = R1;
                    R1 = R0;
                    R0 = offset;
                }
                if (offset == 0 || offset > size()) raise("ValueError", "LZX: invalid match offset");
                int64_t src = size() - offset;
                for (int64_t k = 0; k < match_len; ++k) {
                    if (src >= size()) raise("IndexError", "bytearray index out of range");   // a negative offset
                    out.push_back(out[static_cast<size_t>(src)]);
                    ++src;
                }
            }
            block_remaining -= want;
        }
        if (size() - base < out_len) bits.align16();
    }
    std::vector<uint8_t> produced(out.begin() + base, out.begin() + (base + out_len < size() ? base + out_len : size()));
    if (intel_filesize) e8_decode(produced, intel_filesize);
    return produced;
}

std::vector<uint8_t> deblock(Bytes buf, int64_t first_size, Bytes first_hash, bool verify) {
    std::vector<uint8_t> stream;
    int64_t p = 0, cur_size = first_size;
    std::string cur_hash = first_hash.str();
    while (cur_size) {
        Bytes block = buf.slice(p, p + cur_size);
        if (block.size() < cur_size) raise("ValueError", "LZX: truncated block");
        if (verify) {
            auto got = xex::crypto::Sha1::hash(block.data(), static_cast<size_t>(block.size()));
            if (cur_hash.size() != 20 || std::memcmp(got.data(), cur_hash.data(), 20) != 0)
                raise("ValueError", "LZX: block SHA-1 mismatch (wrong decrypt key or corrupt image)");
        }
        int64_t next_size = block.be32(0);
        std::string next_hash = block.slice(4, 24).str();
        int64_t q = 24;
        while (true) {
            if (q + 2 > block.size()) break;
            int64_t chunk = (block.at(q) << 8) | block.at(q + 1);
            q += 2;
            if (chunk == 0) break;
            Bytes piece = block.slice(q, q + chunk);
            stream.insert(stream.end(), piece.begin(), piece.end());
            q += chunk;
        }
        p += cur_size;
        cur_size = next_size;
        cur_hash = next_hash;
    }
    return stream;
}

namespace {

void lzxd_q(Call &c) {   // (window_bits, out_len, ref_data)
    const Value &ref = c.arg(2);
    Bytes r = ref.is_none() ? Bytes() : ref.as_bytes();
    auto out = lzxd_decompress(c.data, c.int_arg(0), c.int_arg(1), ref.is_none() ? nullptr : &r);
    c.out.bytes(Bytes(out));
}

void xex_q(Call &c) {    // (window_size, (block_size, block_hash), out_len)
    const Value &fb = c.arg(1);
    if (!fb.truthy()) raise("ValueError", "LZX: missing first-block descriptor");
    if (fb.items.size() != 2) raise("ValueError", "too many values to unpack (expected 2)");
    auto stream = deblock(c.data, fb[0].as_int(), fb[1].as_bytes(), true);
    int64_t ws = c.int_arg(0);
    int64_t bits = 0;
    for (uint64_t v = static_cast<uint64_t>(ws); v; v >>= 1) ++bits;
    auto out = lzxd_decompress(Bytes(stream), bits - 1, c.int_arg(2), nullptr);
    c.out.bytes(Bytes(out));
}

}  // namespace

CBF_QUESTIONS(lzx) {
    add("lzx.lzxd_decompress", lzxd_q);
    add("lzx.lzx_decompress_xex", xex_q);
}

}  // namespace cbf::lzx
