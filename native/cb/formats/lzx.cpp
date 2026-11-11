#include "cb/formats/lzx.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <format>
#include <vector>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/io.hpp"

namespace cb::formats::lzx {

namespace {

constexpr std::string_view kReader = "lzx";

constexpr unsigned kMinMatch = 2;
constexpr unsigned kNumChars = 256;
constexpr unsigned kVerbatim = 1;
constexpr unsigned kAligned = 2;
constexpr unsigned kUncompressed = 3;
constexpr unsigned kPretreeNum = 20;
constexpr unsigned kAlignedNum = 8;
constexpr unsigned kPrimaryLengths = 7;
constexpr unsigned kSecondaryLengths = 249;
constexpr std::size_t kFrame = 32768;
constexpr std::size_t kMaxE8Frames = 32768;
constexpr std::size_t kPositionSlots = 51;
constexpr unsigned kMaxCodeLength = 16;

[[noreturn]] void damaged(std::string what) { throw FormatError(kReader, 0, std::move(what)); }

// Extra bits and base distance of each position slot.
struct PositionTables {
    std::array<std::uint8_t, kPositionSlots> extra{};
    std::array<std::uint32_t, kPositionSlots> base{};
    PositionTables() {
        unsigned j = 0;
        for (std::size_t i = 0; i < kPositionSlots; i += 2) {
            extra[i] = static_cast<std::uint8_t>(j);
            if (i + 1 < kPositionSlots) extra[i + 1] = static_cast<std::uint8_t>(j);
            if (i != 0 && j < 17) ++j;
        }
        std::uint32_t b = 0;
        for (std::size_t i = 0; i < kPositionSlots; ++i) {
            base[i] = b;
            b += std::uint32_t{1} << extra[i];
        }
    }
};
const PositionTables& position_tables() {
    static const PositionTables t;
    return t;
}

// The reference decoder's bit reader: 16-bit little-endian words, most significant bit first, into a 32-bit
// accumulator. Past the end it reads zeros, but only a little way (the last refill legitimately looks ahead);
// a stream that keeps asking is cut off.
class BitReader {
public:
    explicit BitReader(ByteView data) : data_(data) {}

    std::uint32_t read(unsigned k) {
        if (k == 0) return 0;
        ensure(k);
        const std::uint32_t v = buf_ >> (32 - k);
        buf_ <<= k;
        bits_ -= k;
        return v;
    }
    // up to 32 bits (two reads of at most 16)
    std::uint32_t read_wide(unsigned k) {
        if (k <= 16) return read(k);
        const std::uint32_t hi = read(16);
        return (hi << (k - 16)) | read(k - 16);
    }
    std::uint32_t peek(unsigned k) {
        ensure(k);
        return buf_ >> (32 - k);
    }
    void remove(unsigned k) {
        buf_ <<= k;
        bits_ -= k;
    }
    void align16() {
        if (const unsigned rem = bits_ & 15) read(rem);
    }
    // Give back the whole bytes held in the accumulator, so that raw bytes can be read from `pos()`.
    void drain_to_byte() {
        pos_ -= bits_ / 8;
        bits_ = 0;
        buf_ = 0;
    }
    std::uint32_t raw_u32() {
        io::ByteReader r(data_, kReader);
        r.seek(pos_);
        const std::uint32_t v = r.u32();
        pos_ += 4;
        return v;
    }
    ByteView raw_bytes(std::size_t n) {
        const ByteView v = io::checked_range(data_, pos_, n);
        pos_ += n;
        return v;
    }
    void skip_pad_byte() { pos_ += 1; }
    std::size_t pos() const { return pos_; }

private:
    void ensure(unsigned need) {
        while (bits_ < need) {
            buf_ |= word() << (16 - bits_);
            bits_ += 16;
        }
    }
    std::uint32_t word() {
        if (pos_ > data_.size() + 16) throw TruncatedError(kReader, pos_, 2, 0);
        std::uint32_t w = 0;
        if (pos_ + 2 <= data_.size()) w = data_[pos_] | (data_[pos_ + 1] << 8);
        else if (pos_ < data_.size()) w = data_[pos_];
        pos_ += 2;
        return w;
    }

    ByteView data_;
    std::size_t pos_ = 0;
    std::uint32_t buf_ = 0;
    unsigned bits_ = 0;
};

// A canonical Huffman decoding table: peek `max_length` bits, index -> (symbol, length).
class HuffmanTable {
public:
    HuffmanTable() = default;
    explicit HuffmanTable(const std::vector<std::uint8_t>& lengths) {
        for (std::uint8_t l : lengths) max_length_ = std::max<unsigned>(max_length_, l);
        if (max_length_ == 0) return;   // every length is 0: the table may exist but is never used
        CB_ASSERT(max_length_ <= kMaxCodeLength, "LZX code length beyond the format's 16");
        std::vector<std::uint32_t> count(max_length_ + 1, 0);
        for (std::uint8_t l : lengths) {
            if (l) ++count[l];
        }
        std::vector<std::uint32_t> next_code(max_length_ + 1, 0);
        std::uint32_t code = 0;
        for (unsigned bits = 1; bits <= max_length_; ++bits) {
            code = (code + count[bits - 1]) << 1;
            next_code[bits] = code;
        }
        entries_.assign(std::size_t{1} << max_length_, kUnassigned);
        for (std::size_t symbol = 0; symbol < lengths.size(); ++symbol) {
            const unsigned l = lengths[symbol];
            if (l == 0) continue;
            const std::uint32_t c = next_code[l]++;
            const unsigned shift = max_length_ - l;
            const std::size_t start = std::size_t{c} << shift;
            if (start + (std::size_t{1} << shift) > entries_.size()) damaged("over-subscribed Huffman code");
            std::fill_n(entries_.begin() + static_cast<std::ptrdiff_t>(start), std::size_t{1} << shift,
                        static_cast<std::uint32_t>(symbol << 8 | l));
        }
    }

    unsigned decode(BitReader& bits) const {
        if (max_length_ == 0) damaged("a Huffman table with no codes was used");
        const std::uint32_t entry = entries_[bits.peek(max_length_)];
        if (entry == kUnassigned) damaged("invalid Huffman code");
        bits.remove(entry & 0xFF);
        return entry >> 8;
    }

private:
    static constexpr std::uint32_t kUnassigned = 0xFFFFFFFFu;
    unsigned max_length_ = 0;
    std::vector<std::uint32_t> entries_;
};

// Read the code lengths of lens[first, last): a pre-tree gives run-length codes (17, 18: zero runs; 19: a run of one
// repeated difference) and plain differences from the previous block's length.
void read_lengths(BitReader& bits, std::vector<std::uint8_t>& lens, std::size_t first, std::size_t last) {
    std::vector<std::uint8_t> pre(kPretreeNum);
    for (std::uint8_t& p : pre) p = static_cast<std::uint8_t>(bits.read(4));
    const HuffmanTable pretree(pre);
    auto at = [&](std::size_t i) -> std::uint8_t& {
        if (i >= lens.size()) damaged("code length run past the end of the tree");
        return lens[i];
    };
    auto next_length = [&](std::size_t i, unsigned z) {
        int v = at(i) - static_cast<int>(z);
        if (v < 0) v += 17;
        return static_cast<std::uint8_t>(v);
    };
    std::size_t i = first;
    while (i < last) {
        unsigned z = pretree.decode(bits);
        if (z == 17) {
            for (unsigned k = bits.read(4) + 4; k > 0; --k) at(i++) = 0;
        } else if (z == 18) {
            for (unsigned k = bits.read(5) + 20; k > 0; --k) at(i++) = 0;
        } else if (z == 19) {
            const unsigned run = bits.read(1) + 4;
            z = pretree.decode(bits);
            const std::uint8_t v = next_length(i, z);
            for (unsigned k = 0; k < run; ++k) at(i++) = v;
        } else {
            at(i) = next_length(i, z);
            ++i;
        }
    }
}

// Undo the encoder's translation of the relative addresses in x86 CALL (E8) operands, frame by frame.
void e8_decode(Bytes& out, std::uint32_t file_size) {
    if (file_size == 0) return;
    const std::size_t n = out.size();
    std::size_t frame_start = 0;
    for (std::size_t frames = 0; frame_start < n && frames < kMaxE8Frames; ++frames, frame_start += kFrame) {
        const std::size_t frame_size = std::min(kFrame, n - frame_start);
        if (frame_size <= 10) continue;
        const std::size_t end = frame_start + frame_size - 10;
        std::size_t i = frame_start;
        while (i < end) {
            if (out[i] != 0xE8) {
                ++i;
                continue;
            }
            const std::int32_t pos = static_cast<std::int32_t>(i);
            const std::int32_t v = static_cast<std::int32_t>(out[i + 1] | (out[i + 2] << 8) | (out[i + 3] << 16) |
                                                             (static_cast<std::uint32_t>(out[i + 4]) << 24));
            if (-static_cast<std::int64_t>(pos) <= v && v < static_cast<std::int64_t>(file_size)) {
                const std::uint32_t nv = static_cast<std::uint32_t>(v >= 0 ? v - pos : v + static_cast<std::int64_t>(file_size));
                for (unsigned k = 0; k < 4; ++k) out[i + 1 + k] = static_cast<std::uint8_t>(nv >> (8 * k));
            }
            i += 5;
        }
    }
}

}  // namespace

Bytes decompress(ByteView data, unsigned window_bits, std::size_t out_len, ByteView reference) {
    if (window_bits < 15 || window_bits > 21) {
        throw UnsupportedError(kReader, 0, std::format("a window of 2^{} bytes", window_bits));
    }
    const PositionTables& T = position_tables();
    const std::size_t window_size = std::size_t{1} << window_bits;
    std::size_t position_slots = 0;
    while (position_slots < kPositionSlots && T.base[position_slots] < window_size) ++position_slots;
    const std::size_t main_elements = kNumChars + (position_slots << 3);

    BitReader bits(data);
    Bytes out;
    if (!reference.empty()) {
        // The window starts full: zeros, then the reference data at its end.
        out.assign(window_size > reference.size() ? window_size - reference.size() : 0, 0);
        append(out, reference);
    }
    const std::size_t base = out.size();
    out.reserve(base + out_len + 512);

    std::uint32_t r0 = 1, r1 = 1, r2 = 1;   // the three most recent match offsets
    std::vector<std::uint8_t> main_lens(main_elements, 0), len_lens(kSecondaryLengths + 1, 0);
    HuffmanTable main_table, len_table, aligned_table;

    std::uint32_t intel_filesize = 0;
    if (bits.read(1)) intel_filesize = bits.read_wide(32);
    unsigned block_type = 0;
    std::size_t block_remaining = 0;

    while (out.size() - base < out_len) {
        const std::size_t done = out.size() - base;
        const std::size_t frame_end = base + std::min(done - done % kFrame + kFrame, out_len);
        while (out.size() < frame_end) {
            if (block_remaining == 0) {
                block_type = bits.read(3);
                block_remaining = bits.read_wide(24);
                if (block_type == kAligned) {
                    std::vector<std::uint8_t> aligned(kAlignedNum);
                    for (std::uint8_t& a : aligned) a = static_cast<std::uint8_t>(bits.read(3));
                    aligned_table = HuffmanTable(aligned);
                }
                if (block_type == kVerbatim || block_type == kAligned) {
                    read_lengths(bits, main_lens, 0, kNumChars);
                    read_lengths(bits, main_lens, kNumChars, main_elements);
                    main_table = HuffmanTable(main_lens);
                    read_lengths(bits, len_lens, 0, kSecondaryLengths);
                    len_table = HuffmanTable(len_lens);
                } else if (block_type == kUncompressed) {
                    bits.align16();
                    bits.drain_to_byte();
                    r0 = bits.raw_u32();
                    r1 = bits.raw_u32();
                    r2 = bits.raw_u32();
                } else {
                    damaged(std::format("bad block type {}", block_type));
                }
            }
            const std::size_t want = std::min(frame_end - out.size(), block_remaining);
            const std::size_t target = out.size() + want;
            if (block_type == kUncompressed) {
                append(out, bits.raw_bytes(want));
                block_remaining -= want;
                if (block_remaining == 0 && (bits.pos() & 1)) bits.skip_pad_byte();   // blocks end on a word
                continue;
            }
            while (out.size() < target) {
                unsigned main = main_table.decode(bits);
                if (main < kNumChars) {
                    out.push_back(static_cast<std::uint8_t>(main));
                    continue;
                }
                main -= kNumChars;
                const unsigned length_header = main & 7;
                const unsigned match_len =
                    (length_header == kPrimaryLengths ? len_table.decode(bits) + kPrimaryLengths : length_header) + kMinMatch;
                const unsigned slot = main >> 3;
                std::uint32_t offset;
                if (slot == 0) {
                    offset = r0;
                } else if (slot == 1) {
                    offset = r1;
                    r1 = r0;
                    r0 = offset;
                } else if (slot == 2) {
                    offset = r2;
                    r2 = r0;
                    r0 = offset;
                } else {
                    if (slot >= kPositionSlots) damaged("match position slot out of range");
                    const unsigned extra = T.extra[slot];
                    if (block_type == kAligned && extra >= 3) {
                        const std::uint32_t verbatim = bits.read(extra - 3) << 3;
                        offset = T.base[slot] - 2 + verbatim + aligned_table.decode(bits);
                    } else {
                        offset = T.base[slot] - 2 + (extra ? bits.read(extra) : 0);
                    }
                    r2 = r1;
                    r1 = r0;
                    r0 = offset;
                }
                if (offset == 0 || offset > out.size()) damaged("invalid match offset");
                std::size_t src = out.size() - offset;
                for (unsigned k = 0; k < match_len; ++k) out.push_back(out[src++]);   // may overlap its own output
            }
            block_remaining -= want;
        }
        if (out.size() - base < out_len) bits.align16();
    }
    Bytes produced(out.begin() + static_cast<std::ptrdiff_t>(base),
                   out.begin() + static_cast<std::ptrdiff_t>(std::min(base + out_len, out.size())));
    e8_decode(produced, intel_filesize);
    return produced;
}

Bytes deblock(ByteView image, std::uint32_t first_size, ByteView first_hash, bool verify) {
    Bytes stream;
    std::uint64_t at = 0;
    std::uint32_t size = first_size;
    crypto::Sha1Digest hash{};
    if (first_hash.size() == hash.size()) std::copy(first_hash.begin(), first_hash.end(), hash.begin());
    bool have_hash = first_hash.size() == hash.size();
    while (size != 0) {
        const ByteView block = io::checked_range(image, at, size);   // FormatError: a truncated block
        if (verify) {
            if (!have_hash || crypto::sha1(block) != hash) {
                damaged("block SHA-1 mismatch (wrong decrypt key or corrupt image)");
            }
        }
        io::ByteReader r(block, "lzx block");
        const std::uint32_t next_size = r.u32_be();
        const ByteView next_hash = r.bytes(20);
        while (r.remaining() >= 2) {
            const std::uint16_t chunk = r.u16_be();
            if (chunk == 0) break;
            append(stream, r.bytes(chunk));
        }
        at += size;
        size = next_size;
        std::copy(next_hash.begin(), next_hash.end(), hash.begin());
        have_hash = true;
    }
    return stream;
}

Bytes decompress_xex(ByteView image, std::uint32_t window_size, std::uint32_t first_size, ByteView first_hash,
                     std::size_t out_len) {
    if (!std::has_single_bit(window_size)) damaged(std::format("window size {} is not a power of two", window_size));
    const Bytes stream = deblock(image, first_size, first_hash, true);
    return decompress(stream, static_cast<unsigned>(std::bit_width(window_size) - 1), out_len);
}

}  // namespace cb::formats::lzx
