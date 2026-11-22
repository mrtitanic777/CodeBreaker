#include "build/pklite.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>

#include "core/error.hpp"
#include "core/io.hpp"
#include "formats/dos.hpp"

namespace cb::build::pklite {

namespace {

constexpr std::uint32_t kLengthWithByte = 25;   // the length code that is followed by a byte: a copy of 25 + that byte
constexpr std::uint32_t kSmallLengthWithByte = 10;   // the same in the small model and 1.20
constexpr std::uint32_t kSmallMaxLength = 10 + 0xFE;
constexpr std::uint16_t kGroupStep = 0xFFF;     // the paragraphs between one group of relocations and the next
constexpr std::uint8_t kMarkByte = 0xFE;
constexpr std::uint8_t kEndByte = 0xFF;
constexpr std::uint32_t kFirstMark = 0x8000, kMarkEvery = 0xA000;
constexpr std::size_t kMaxCodeBits = 12;        // no prefix code of the decompressor is longer
constexpr std::size_t kMaxShortCopyDistance = 255;   // a length-2 copy comes from no farther
constexpr std::uint32_t kEntryMovAx = 1, kEntryMovDx = 4;   // where the entry code holds the memory it needs, and the stream's size
constexpr std::uint8_t kEntryCode[] = {0x8C, 0xDB, 0x53};   // mov bx,ds / push bx, then add bx,N
constexpr std::size_t kHeadBytes = 6;

[[noreturn]] void bad(std::uint64_t offset, const std::string& what) { throw FormatError("pklite", offset, what); }

[[noreturn]] void too_big(const std::string& what) {
    throw UnsupportedError("pklite", 0, what + " does not fit PKLITE's 16-bit field");
}

std::uint16_t u16_field(std::uint64_t value, const char* what) {
    if (value > 0xFFFF) too_big(what);
    return static_cast<std::uint16_t>(value);
}

// A byte of the decompressor's code, by its offset in the code (the tables are there).
std::uint8_t table(ByteView code, std::size_t index) {
    if (index >= code.size()) bad(kStubCode + index, "the decompressor's code is too short for its tables");
    return code[index];
}

// ---- the decompressor's reading ------------------------------------------------------------------------------------
// Both readers below give the prefix-code functions the same three things: bit(), and at() to look a value up in the
// decompressor's tables.

// The stream: words of bits and single bytes through one pointer.
class StreamBits {
public:
    StreamBits(ByteView data, std::size_t at) : data_(data), si_(at) {
        bp_ = word();
        dx_ = 16;
    }
    std::size_t position() const { return si_; }
    std::uint32_t byte() {
        if (si_ >= data_.size()) throw TruncatedError("pklite", si_, 1, 0);
        return data_[si_++];
    }
    std::uint32_t bit() {
        const std::uint32_t carry = bp_ & 1;
        bp_ >>= 1;
        if (--dx_ == 0) {
            bp_ = word();
            dx_ = 16;
        }
        return carry;
    }
    std::uint8_t at(ByteView code, std::size_t index) const { return table(code, index); }
    std::uint32_t left() const { return dx_; }

private:
    std::uint32_t word() {
        if (si_ + 2 > data_.size()) throw TruncatedError("pklite", si_, 2, data_.size() > si_ ? data_.size() - si_ : 0);
        const std::uint32_t w = data_[si_] | (static_cast<std::uint32_t>(data_[si_ + 1]) << 8);
        si_ += 2;
        return w;
    }
    ByteView data_;
    std::size_t si_;
    std::uint32_t bp_ = 0;
    std::uint32_t dx_ = 0;
};

// A bit string being tried: bit() gives its bits, then zeros and a note that it ran out. A value computed from a
// string that ran out is meaningless, so at() answers 0 for it instead of looking anything up.
class BitString {
public:
    explicit BitString(const std::vector<std::uint8_t>& bits) : bits_(bits) {}
    std::uint32_t bit() {
        if (next_ < bits_.size()) return bits_[next_++];
        ran_out_ = true;
        return 0;
    }
    bool ran_out() const { return ran_out_; }
    std::uint8_t at(ByteView code, std::size_t index) const { return ran_out_ ? 0 : table(code, index); }

private:
    const std::vector<std::uint8_t>& bits_;
    std::size_t next_ = 0;
    bool ran_out_ = false;
};

// The length code, as the decompressor reads it (`t`: its code from offset 0; `l`: where its table is).
template <class Bits>
std::uint32_t read_length(Bits& in, ByteView t, std::size_t l) {
    const auto more = [&in](std::uint32_t bx) { return (bx << 1) | in.bit(); };
    std::uint32_t bx = in.bit() << 1;
    bx |= in.bit();
    if (bx >= 2) return bx;
    bx = more(bx);
    if (bx == 0) return in.at(t, l);
    bx = more(bx);
    if (bx < 5) return in.at(t, l + bx);
    bx = more(bx);
    if (bx <= 0xC) return in.at(t, l + bx);
    bx = ((bx & 3) << 1) | in.bit();
    if (bx >= 5) {
        bx = more(bx);
        if (bx > 0xC) {
            bx = ((bx & 3) << 1) | in.bit();
            if (bx >= 5) bx = more(bx);
            return in.at(t, l + 0x16 + bx);
        }
    }
    return in.at(t, l + 0xB + bx);
}

// The small model's length code.
template <class Bits>
std::uint32_t read_small_length(Bits& in, ByteView t, std::size_t l) {
    std::uint32_t bx = in.bit() << 1;
    bx |= in.bit();
    if (bx != 0) {
        bx = (bx << 1) | in.bit();
        if (bx >= 6) bx = (bx << 1) | in.bit();
    }
    return in.at(t, l + bx);
}

// The code for a distance's high byte (after the bit that says it has one).
template <class Bits>
std::uint32_t read_high(Bits& in, ByteView t, std::size_t h) {
    const auto more = [&in](std::uint32_t bx) { return (bx << 1) | in.bit(); };
    std::uint32_t bx = in.bit() << 2;
    bx |= in.bit() << 1;
    bx |= in.bit();
    if (bx < 2) return in.at(t, h + bx);
    bx = more(bx);
    if (bx < 8) return in.at(t, h + bx);
    bx = more(bx);
    if (bx < 0x17) return in.at(t, h + bx);
    bx = more(bx);
    return bx & 0xDF;
}

using CodeReader = std::uint32_t (*)(BitString&, ByteView, std::size_t);

// {value: bits} of a prefix code, by feeding the reader every string of bits: a string the reader runs out of is
// extended by a 0 and by a 1; one it reads to the end of is a value's code (the first string found for a value wins).
void collect_codes(CodeReader read, ByteView t, std::size_t base, std::vector<std::uint8_t>& prefix, Codes& out) {
    BitString in(prefix);
    const std::uint32_t value = read(in, t, base);
    if (in.ran_out()) {
        if (prefix.size() < kMaxCodeBits) {
            for (int next = 0; next < 2; ++next) {
                prefix.push_back(static_cast<std::uint8_t>(next));
                collect_codes(read, t, base, prefix, out);
                prefix.pop_back();
            }
        }
        return;
    }
    Code code;
    code.length = static_cast<std::uint8_t>(prefix.size());
    for (std::uint8_t b : prefix) code.bits = static_cast<std::uint16_t>((code.bits << 1) | b);
    out.try_emplace(value, code);
}

Codes codes_of(CodeReader read, ByteView t, std::size_t base) {
    Codes out;
    std::vector<std::uint8_t> prefix;
    collect_codes(read, t, base, prefix, out);
    return out;
}

// ---- which form a decompressor is ----------------------------------------------------------------------------------

std::uint16_t word_in(ByteView d, std::size_t at) { return static_cast<std::uint16_t>(d[at] | (d[at + 1] << 8)); }

template <std::size_t N>
std::size_t find_bytes(ByteView d, const std::uint8_t (&what)[N], std::size_t from = 0) {
    if (from >= d.size()) return std::string::npos;
    const auto it = std::search(d.begin() + static_cast<std::ptrdiff_t>(from), d.end(), std::begin(what), std::end(what));
    return it == d.end() ? std::string::npos : static_cast<std::size_t>(it - d.begin());
}

// The module with a keyed decompressor put back: mov cx,N / mov si,P / mov di,si / std / (nop) / dec cx / jz / lodsw / xchg dx,ax / add|xor ax,dx / stosw.
Bytes unkeyed(ByteView module) {
    Bytes d(module.begin(), module.end());
    const std::size_t scan = std::min<std::size_t>(d.size(), 0x80);
    for (std::size_t i = 0; i + 20 < scan; ++i) {
        if (d[i] != 0xB9 || d[i + 3] != 0xBE || d[i + 6] != 0x8B || d[i + 7] != 0xFE || d[i + 8] != 0xFD) continue;
        std::size_t j = i + 9;
        if (d[j] == 0x90) ++j;
        if (d[j] != 0x49 || d[j + 1] != 0x74 || d[j + 3] != 0xAD || d[j + 4] != 0x92 || d[j + 7] != 0xAB || (d[j + 5] != 0x03 && d[j + 5] != 0x33)) continue;
        const bool add = d[j + 5] == 0x03;
        std::uint16_t key = word_in(d, 4), count = word_in(d, i + 1);
        std::int64_t si = static_cast<std::int64_t>(word_in(d, i + 4)) - 0x100;
        while (--count != 0) {
            if (si < 0 || static_cast<std::size_t>(si) + 2 > d.size()) bad(static_cast<std::uint64_t>(std::max<std::int64_t>(si, 0)), "the keyed decompressor runs outside the module");
            const std::uint16_t stored = word_in(d, static_cast<std::size_t>(si));
            const std::uint16_t plain = add ? static_cast<std::uint16_t>(key + stored) : static_cast<std::uint16_t>(key ^ stored);
            d[static_cast<std::size_t>(si)] = static_cast<std::uint8_t>(plain);
            d[static_cast<std::size_t>(si) + 1] = static_cast<std::uint8_t>(plain >> 8);
            key = stored;
            si -= 2;
        }
        break;
    }
    return d;
}

// Where the decompressor's code starts: what `mov si,N / cld / rep movsw` moves down to offset 0.
std::size_t code_start(ByteView plain) {
    const std::size_t scan = std::min<std::size_t>(plain.size(), 0x90);
    for (std::size_t i = 0; i + 6 <= scan; ++i)
        if (plain[i] == 0xBE && plain[i + 2] == 0x01 && plain[i + 3] == 0xFC && plain[i + 4] == 0xF3 && plain[i + 5] == 0xA5) return static_cast<std::size_t>(word_in(plain, i + 1)) - 0x100;
    return kStubCode;
}

// The 1.20 code, as its instructions read it. The two values beyond any length stand for its special codes.
constexpr std::uint32_t kV120Escape = 10;

Codes v120_lengths() {
    const auto code = [](std::uint16_t bits, std::uint8_t length) { return Code{bits, length}; };
    return {{2, code(0b10, 2)},     {3, code(0b11, 2)},       {4, code(0b000, 3)},     {5, code(0b0100, 4)},   {6, code(0b0101, 4)},
            {7, code(0b01110, 5)},  {8, code(0b011110, 6)},   {9, code(0b011111, 6)},  {kV120Escape, code(0b0110, 4)}};
}
constexpr Code kV120Zero{0b0010, 4};      // a literal zero, with no byte
constexpr Code kV120FarPair{0b0011, 4};   // a copy of 2 from the page above: its low byte follows

Codes v120_highs() {
    Codes out;
    out[1] = Code{0, 2};
    for (std::uint32_t hi = 2; hi <= 5; ++hi) out[hi] = Code{static_cast<std::uint16_t>(hi + 2), 4};
    for (std::uint32_t hi = 6; hi <= 11; ++hi) out[hi] = Code{static_cast<std::uint16_t>(hi + 10), 5};
    for (std::uint32_t hi = 12; hi <= 31; ++hi) out[hi] = Code{static_cast<std::uint16_t>(hi + 0x20), 6};
    return out;
}

std::uint32_t read_v120_high(StreamBits& in) {
    std::uint32_t x = in.bit() << 1;
    x |= in.bit();
    if (x == 0) return 1;
    x = (x << 1) | in.bit();
    x = (x << 1) | in.bit();
    if (x <= 7) return x - 2;
    x = (x << 1) | in.bit();
    if (x <= 0x15) return x - 10;
    x = (x << 1) | in.bit();
    return x - 0x20;
}

// ---- decoding ------------------------------------------------------------------------------------------------------

struct Decoded {
    Bytes image;
    std::vector<Token> tokens;
    std::size_t end;   // where the stream ends
};

Decoded decode(const Stub& stub, ByteView data, std::size_t at) {
    StreamBits in(data, at);
    const ByteView t = stub.code;
    Decoded out;
    const std::uint32_t with_byte = stub.tree == Tree::Large ? kLengthWithByte : kSmallLengthWithByte;
    const auto finish = [&] {
        out.tokens.push_back(Token::end());
        out.end = in.position();
    };
    while (true) {
        if (!in.bit()) {
            std::uint8_t b = static_cast<std::uint8_t>(in.byte());
            if (stub.keyed_literals) b = static_cast<std::uint8_t>(b ^ in.left());
            out.image.push_back(b);
            out.tokens.push_back(Token::literal(b));
            continue;
        }
        std::uint32_t n = 0;
        std::optional<std::uint32_t> known_high;
        if (stub.tree == Tree::V120) {
            if (in.bit()) {
                n = 2 + in.bit();
                if (n == 2) known_high = 0;
            } else if (in.bit()) {
                n = 5 + (in.bit() << 1);
                n += in.bit();
                if (n == 7) {
                    n = with_byte;
                } else if (n == 8) {
                    n = in.bit() ? 8 + in.bit() : 7;
                }
            } else if (in.bit()) {
                if (!in.bit()) {
                    out.image.push_back(0);
                    out.tokens.push_back(Token::literal(0));
                    continue;
                }
                n = 2;
                known_high = 1;
            } else {
                n = 4;
            }
        } else {
            n = stub.tree == Tree::Large ? read_length(in, t, stub.length_table) : read_small_length(in, t, stub.length_table);
        }
        if (n == with_byte) {
            const std::uint32_t b = in.byte();
            if (stub.tree == Tree::Large && b == kMarkByte) {
                out.tokens.push_back(Token::mark());
                continue;
            }
            if (b == kEndByte) {
                finish();
                return out;
            }
            n += b;
        }
        std::uint32_t high = 0;
        if (known_high) {
            high = *known_high;
        } else if (stub.tree == Tree::V120) {
            high = in.bit() ? 0 : read_v120_high(in);
        } else {
            high = (n == 2 || in.bit()) ? 0 : read_high(in, t, stub.high_table);
        }
        const std::uint32_t distance = (high << 8) | in.byte();
        if (distance == 0 || distance > out.image.size())
            bad(in.position(), std::format("a copy of {} bytes from {} back, with {} bytes expanded", n, distance, out.image.size()));
        for (std::uint32_t i = 0; i < n; ++i) out.image.push_back(out.image[out.image.size() - distance]);
        out.tokens.push_back(Token::copy(static_cast<std::uint16_t>(n), static_cast<std::uint16_t>(distance)));
    }
}

// ---- encoding ------------------------------------------------------------------------------------------------------

// The stream as the decompressor will read it: a word is reserved where the reading will fetch it -- at the start, and
// right after the byte that follows the 16th bit of the last.
class Writer {
public:
    Writer() : out_(2, 0) {}
    std::size_t size() const { return out_.size(); }
    void bit(std::uint32_t b) {
        word_ |= b << n_;
        if (++n_ == 16) {
            put_word();
            slot_ = out_.size();
            n_ = 0;
            word_ = 0;
            out_.push_back(0);
            out_.push_back(0);
        }
    }
    void bits(const Code& code) {
        for (int i = code.length - 1; i >= 0; --i) bit((code.bits >> i) & 1u);
    }
    void byte(std::uint8_t b) { out_.push_back(b); }
    std::uint32_t left() const { return 16 - n_; }   // (as the decompressor's count stands after the bit just written)
    Bytes done() {
        put_word();
        return std::move(out_);
    }

private:
    void put_word() {
        out_[slot_] = static_cast<std::uint8_t>(word_ & 0xFF);
        out_[slot_ + 1] = static_cast<std::uint8_t>(word_ >> 8);
    }
    Bytes out_;
    std::size_t slot_ = 0;
    std::uint32_t n_ = 0;
    std::uint32_t word_ = 0;
};

const Code& code_for(const Codes& codes, std::uint32_t value, const char* what) {
    const auto it = codes.find(value);
    if (it == codes.end()) throw UnsupportedError("pklite", 0, std::format("this decompressor has no code for the {} {}", what, value));
    return it->second;
}

void encode_into(Writer& w, std::span<const Token> tokens, const Stub& stub) {
    const std::uint32_t with_byte = stub.tree == Tree::Large ? kLengthWithByte : kSmallLengthWithByte;
    for (const Token& t : tokens) {
        if (t.kind == Token::Kind::Literal) {
            if (stub.tree == Tree::V120 && t.byte == 0) {
                w.bit(1);
                w.bits(kV120Zero);
                continue;
            }
            w.bit(0);
            w.byte(stub.keyed_literals ? static_cast<std::uint8_t>(t.byte ^ w.left()) : t.byte);
            continue;
        }
        w.bit(1);
        if (t.kind == Token::Kind::Mark || t.kind == Token::Kind::End) {
            w.bits(code_for(stub.lengths, with_byte, "length"));
            w.byte(t.kind == Token::Kind::Mark ? kMarkByte : kEndByte);
            continue;
        }
        const std::uint32_t n = t.length, d = t.distance;
        if (stub.tree == Tree::V120 && n == 2 && d >= 0x100) {
            if (d >> 8 != 1) throw UnsupportedError("pklite", 0, "PKLITE 1.20 has no code for a copy of 2 from that far");
            w.bits(kV120FarPair);
            w.byte(static_cast<std::uint8_t>(d & 0xFF));
            continue;
        }
        if (n >= with_byte) {
            w.bits(code_for(stub.lengths, with_byte, "length"));
            w.byte(static_cast<std::uint8_t>(n - with_byte));
        } else {
            w.bits(code_for(stub.lengths, n, "length"));
        }
        if (n != 2) {
            if (d < 0x100) {
                w.bit(1);
            } else {
                w.bit(0);
                w.bits(code_for(stub.highs, d >> 8, "distance high byte"));
            }
        }
        w.byte(static_cast<std::uint8_t>(d & 0xFF));
    }
}

// ---- finding what PKLITE finds -------------------------------------------------------------------------------------

struct Match {
    std::uint32_t length = 0;
    std::uint32_t distance = 0;
};

// longest(pos): the longest match PKLITE finds at pos: every earlier place with the same three bytes is a candidate,
// nearest first, and of equal lengths the nearest -- except at the full length a copy can have there (277, or what is
// left), where it is the FARTHEST (so inside a long run of one byte the copy comes from where the run began); a
// candidate farther than kMaxDistance is taken as kMaxDistance back. Length 2 only within 255. {0, 0}: none.
class Matcher {
public:
    explicit Matcher(ByteView data, std::uint32_t most = kMaxLength) : data_(data), most_(most), previous_(data.size(), -1) {
        // previous_[p]: the last q < p with the same three bytes as p
        std::unordered_map<std::uint32_t, std::int32_t> latest;   // lookup only
        latest.reserve(data.size() / 2 + 16);
        for (std::size_t p = 0; p + 2 < data.size(); ++p) {
            const std::uint32_t key = (static_cast<std::uint32_t>(data[p]) << 16) | (static_cast<std::uint32_t>(data[p + 1]) << 8) | data[p + 2];
            const auto [it, fresh] = latest.try_emplace(key, static_cast<std::int32_t>(p));
            if (!fresh) {
                previous_[p] = it->second;
                it->second = static_cast<std::int32_t>(p);
            }
        }
    }

    Match longest(std::size_t pos) const {
        const std::size_t n = data_.size();
        const std::size_t limit = std::min<std::size_t>(most_, n - pos);
        std::uint32_t best = 0, best_distance = 0;
        std::int64_t q = limit >= 3 ? previous_[pos] : -1;
        while (q >= 0) {
            if (pos - static_cast<std::size_t>(q) > kMaxDistance) q = static_cast<std::int64_t>(pos - kMaxDistance);
            std::size_t l = 0;
            while (l < limit && data_[static_cast<std::size_t>(q) + l] == data_[pos + l]) ++l;
            if (l > best || l == limit) {
                best = static_cast<std::uint32_t>(l);
                best_distance = static_cast<std::uint32_t>(pos - static_cast<std::size_t>(q));
            }
            if (pos - static_cast<std::size_t>(q) >= kMaxDistance) break;
            q = previous_[static_cast<std::size_t>(q)];
        }
        if (best >= 3) return {best, best_distance};
        if (limit >= 2) {
            for (std::size_t d = 1; d <= std::min(pos, kMaxShortCopyDistance); ++d)
                if (data_[pos - d] == data_[pos] && data_[pos - d + 1] == data_[pos + 1])
                    return {2, static_cast<std::uint32_t>(d)};
        }
        return {0, 0};
    }

private:
    ByteView data_;
    std::uint32_t most_;
    std::vector<std::int32_t> previous_;
};

// Whether PKLITE puts a copy off by a byte: the next one is two or more longer, or one longer and this one comes from
// more than 128 back.
bool put_off(const Match& cur, const Match& next) {
    return next.length >= cur.length + 2 || (next.length == cur.length + 1 && cur.distance > 128);
}

}  // namespace

// ---- the public functions ------------------------------------------------------------------------------------------

Stub Stub::of(ByteView module, std::size_t start) {
    if (start > module.size()) bad(start, "the stream starts past the end of the module");
    Stub stub;
    stub.head.assign(module.begin(), module.begin() + static_cast<std::ptrdiff_t>(start));
    const Bytes plain = unkeyed(module);
    const std::size_t base = code_start(plain);
    if (plain.size() > base) stub.code.assign(plain.begin() + static_cast<std::ptrdiff_t>(base), plain.end());
    const ByteView code = stub.code;
    static constexpr std::uint8_t kLargeTable[] = {0x2E, 0x8A, 0x9F}, kSmallTable[] = {0x2E, 0x8A, 0x8F}, kHighTable[] = {0x2E, 0x8A, 0xBF};
    static constexpr std::uint8_t kKeyedLiteral[] = {0xAC, 0x32, 0xC2, 0xAA}, kV120Mark[] = {0x80, 0xFB, 0x15}, kRuns[] = {0xAC, 0x91, 0xE3}, kNextGroup[] = {0x81, 0xC2, 0xFF, 0x0F};
    static constexpr std::uint8_t kSwapA[] = {0xAD, 0x86, 0xC4}, kSwapB[] = {0xAD, 0x86, 0xE0};
    const std::size_t large = find_bytes(code, kLargeTable), small = find_bytes(code, kSmallTable), high = find_bytes(code, kHighTable);
    stub.keyed_literals = find_bytes(code, kKeyedLiteral) != std::string::npos;
    if (find_bytes(code, kRuns) == std::string::npos && find_bytes(code, kNextGroup) != std::string::npos)
        stub.relocations = find_bytes(code, kSwapA) != std::string::npos || find_bytes(code, kSwapB) != std::string::npos ? RelocationForm::SwappedGroups : RelocationForm::Groups;
    if (small != std::string::npos && (large == std::string::npos || small < large) && small + 5 <= code.size()) {
        stub.tree = Tree::Small;
        stub.length_table = word_in(code, small + 3);
    } else if (large != std::string::npos && large + 5 <= code.size()) {
        stub.length_table = word_in(code, large + 3);
    } else if (find_bytes(code, kV120Mark) != std::string::npos) {
        stub.tree = Tree::V120;
    }
    if (stub.tree != Tree::V120 && high != std::string::npos && high + 5 <= code.size()) stub.high_table = word_in(code, high + 3);
    if (stub.tree == Tree::V120) {
        stub.lengths = v120_lengths();
        stub.highs = v120_highs();
    } else {
        stub.lengths = stub.tree == Tree::Small ? codes_of(read_small_length<BitString>, stub.code, stub.length_table) : codes_of(read_length<BitString>, stub.code, stub.length_table);
        stub.highs = codes_of(read_high<BitString>, stub.code, stub.high_table);
    }
    return stub;
}

std::size_t stream_start(ByteView module) {
    const Bytes plain = unkeyed(module);
    const ByteView view = plain;
    const std::size_t scan = std::min<std::size_t>(view.size(), 0x400);
    for (std::size_t at = find_bytes(view.first(scan), kEntryCode); at != std::string::npos; at = find_bytes(view.first(scan), kEntryCode, at + 1)) {
        const std::size_t next = at + sizeof kEntryCode;
        if (next + 4 > view.size() || view[next + 1] != 0xC3 || (view[next] != 0x81 && view[next] != 0x83)) continue;
        const std::int64_t paragraphs = view[next] == 0x81 ? word_in(view, next + 2) : view[next + 2];
        const std::int64_t start = paragraphs * 16 - 0x100;
        if (start < 0) bad(next, "the stream would start before the module");
        return static_cast<std::size_t>(start);
    }
    // (1.20: mov si,N / add si,dx, the last word it moves up lying just above N)
    for (std::size_t i = 0x40; i + 5 <= std::min<std::size_t>(view.size(), 0xC0); ++i) {
        if (view[i] == 0xBE && ((view[i + 3] == 0x03 && view[i + 4] == 0xF2) || (view[i + 3] == 0x01 && view[i + 4] == 0xD6))) {
            const std::int64_t start = static_cast<std::int64_t>(word_in(view, i + 1)) + 2 - 0x100;
            if (start < 0) bad(i, "the stream would start before the module");
            return static_cast<std::size_t>(start);
        }
    }
    bad(0, "not PKLITE's decompressor (no start of the stream found)");
}

Stored read(ByteView module) {
    Stored s;
    s.start = stream_start(module);
    s.stub = Stub::of(module, s.start);
    Decoded d = decode(s.stub, module, s.start);
    s.image = std::move(d.image);
    s.tokens = std::move(d.tokens);
    s.end = d.end;
    io::ByteReader reader(module, "pklite");
    reader.seek(s.end);
    if (s.stub.relocations == RelocationForm::Runs) {
        while (const std::uint8_t count = reader.u8()) {
            Relocation run;
            run.segment = reader.u16();
            run.offsets.reserve(count);
            for (unsigned i = 0; i < count; ++i) run.offsets.push_back(reader.u16());
            s.relocs.push_back(std::move(run));
        }
    } else {
        std::uint32_t segment = 0;
        for (std::uint16_t count = reader.u16(); count != 0xFFFF; count = reader.u16(), segment += kGroupStep) {
            if (segment > 0xFFFF) bad(reader.pos(), "too many groups of relocations");
            if (count == 0) continue;
            Relocation run;
            run.segment = static_cast<std::uint16_t>(segment);
            run.offsets.reserve(count);
            for (unsigned i = 0; i < count; ++i) {
                const std::uint16_t offset = reader.u16();
                run.offsets.push_back(s.stub.relocations == RelocationForm::SwappedGroups ? static_cast<std::uint16_t>((offset >> 8) | (offset << 8)) : offset);
            }
            s.relocs.push_back(std::move(run));
        }
    }
    s.regs.ss = reader.u16();
    s.regs.sp = reader.u16();
    s.regs.cs = reader.u16();
    s.regs.ip = reader.u16();
    const ByteView tail = reader.bytes(reader.remaining());
    s.tail.assign(tail.begin(), tail.end());
    return s;
}

Bytes encode(std::span<const Token> tokens, const Stub& stub) {
    Writer w;
    encode_into(w, tokens, stub);
    return w.done();
}

namespace {

std::vector<Token> parse_with(ByteView image, bool marks, std::uint32_t most, std::span<const Choice> choices) {
    const Matcher matcher(image, most);
    std::map<std::uint32_t, Token> chosen;
    for (const Choice& c : choices) chosen.emplace(c.at, c.token);
    std::vector<Token> tokens;
    std::size_t pos = 0;
    std::uint32_t mark = kFirstMark;
    Match cur = image.empty() ? Match{} : matcher.longest(0);
    while (pos < image.size()) {
        if (marks && pos + kMaxLength >= mark) {
            tokens.push_back(Token::mark());
            mark += kMarkEvery;
        }
        if (const auto it = chosen.find(static_cast<std::uint32_t>(pos)); it != chosen.end()) {
            tokens.push_back(it->second);
            pos += it->second.kind == Token::Kind::Copy ? it->second.length : 1;
            cur = pos < image.size() ? matcher.longest(pos) : Match{};
            continue;
        }
        const Match next = pos + 1 < image.size() ? matcher.longest(pos + 1) : Match{};
        if (cur.length >= 2 && !put_off(cur, next)) {
            tokens.push_back(Token::copy(static_cast<std::uint16_t>(cur.length), static_cast<std::uint16_t>(cur.distance)));
            pos += cur.length;
            cur = pos < image.size() ? matcher.longest(pos) : Match{};
        } else {
            tokens.push_back(Token::literal(image[pos]));
            ++pos;
            cur = next;
        }
    }
    tokens.push_back(Token::end());
    return tokens;
}

}  // namespace

std::vector<Token> parse(ByteView image) { return parse_with(image, true, kMaxLength, {}); }

std::vector<Token> parse(ByteView image, const Stub& stub, std::span<const Choice> choices) {
    const bool large = stub.tree == Tree::Large;
    return parse_with(image, large, large ? kMaxLength : kSmallMaxLength, choices);
}

std::vector<Choice> choices_of(const Stored& stored) {
    const bool large = stored.stub.tree == Tree::Large;
    const Matcher matcher(stored.image, large ? kMaxLength : kSmallMaxLength);
    const ByteView image = stored.image;
    std::vector<Choice> out;
    std::size_t pos = 0;
    for (const Token& t : stored.tokens) {
        if (t.kind != Token::Kind::Literal && t.kind != Token::Kind::Copy) continue;
        const Match cur = matcher.longest(pos);
        const Match next = pos + 1 < image.size() ? matcher.longest(pos + 1) : Match{};
        const Token rule = cur.length >= 2 && !put_off(cur, next) ? Token::copy(static_cast<std::uint16_t>(cur.length), static_cast<std::uint16_t>(cur.distance)) : Token::literal(image[pos]);
        if (!(rule == t)) out.push_back({static_cast<std::uint32_t>(pos), t});
        pos += t.kind == Token::Kind::Copy ? t.length : 1;
    }
    return out;
}

Bytes relocation_table(std::span<const Relocated> relocs, RelocationForm form) {
    io::ByteWriter out;
    if (form != RelocationForm::Runs) {
        std::map<std::uint32_t, std::vector<std::uint16_t>> groups;
        for (const Relocated& r : relocs) {
            if (r.segment % kGroupStep != 0) throw UnsupportedError("pklite", 0, "a relocation outside this form's groups");
            groups[r.segment / kGroupStep].push_back(r.offset);
        }
        const std::uint32_t last = groups.empty() ? 0 : groups.rbegin()->first + 1;
        for (std::uint32_t g = 0; g < last; ++g) {
            const auto it = groups.find(g);
            out.u16(it == groups.end() ? std::uint16_t{0} : u16_field(it->second.size(), "a group of relocations"));
            if (it == groups.end()) continue;
            for (const std::uint16_t o : it->second) out.u16(form == RelocationForm::SwappedGroups ? static_cast<std::uint16_t>((o >> 8) | (o << 8)) : o);
        }
        out.u16(0xFFFF);
        return out.take();
    }
    for (std::size_t i = 0; i < relocs.size();) {
        const std::uint16_t segment = relocs[i].segment;
        std::size_t j = i;
        while (j < relocs.size() && relocs[j].segment == segment && j - i < 255) ++j;
        out.u8(static_cast<std::uint8_t>(j - i));
        out.u16(segment);
        for (std::size_t k = i; k < j; ++k) out.u16(relocs[k].offset);
        i = j;
    }
    out.u8(0);
    return out.take();
}

Packed pack(ByteView image, std::span<const Relocated> relocs, Registers regs, ByteView head, std::uint32_t memory, const PackDetails& details) {
    const Stub stub = Stub::of(head, head.size());
    const std::vector<Token> tokens = parse(image, stub, details.choices);
    io::ByteWriter body;
    {
        const Bytes stream = encode(tokens, stub);
        const Bytes table = relocation_table(relocs, stub.relocations);
        body.bytes(stream);
        body.bytes(table);
        body.u16(regs.ss);
        body.u16(regs.sp);
        body.u16(regs.cs);
        body.u16(regs.ip);
        body.bytes(details.tail);
    }
    Bytes module(head.begin(), head.end());
    const Bytes stored = body.take();
    module.insert(module.end(), stored.begin(), stored.end());
    if (module.size() < kHeadBytes) bad(0, "the head is too short to hold PKLITE's entry code");
    const std::uint64_t paras = (module.size() + 15) / 16;
    // the memory it needs, relative to where it is loaded: the program and what it asks for beyond itself, less the
    // program prefix -- and never so little that the expanding program would overrun the stream before it is read (it
    // is moved to just below the decompressor)
    const std::int64_t need = static_cast<std::int64_t>((image.size() + 15) / 16) + memory - 0x10;
    std::int64_t written = 0, reach = 0;   // how far the output runs ahead of the input
    Writer w;
    for (const Token& t : tokens) {
        reach = std::max(reach, written - static_cast<std::int64_t>(w.size()));
        written += t.kind == Token::Kind::Literal ? 1 : t.kind == Token::Kind::Copy ? t.length : 0;
        encode_into(w, std::span<const Token>(&t, 1), stub);
    }
    const std::int64_t safe = static_cast<std::int64_t>(paras) + (std::max<std::int64_t>(reach, 0) + 15) / 16 + 0x25;
    const std::uint16_t top = u16_field(static_cast<std::uint64_t>(std::max(need, safe)), "the memory the program needs");
    const std::uint16_t paragraphs = u16_field(paras, "the size of the stored program");
    if (!details.keep_entry) {
        module[kEntryMovAx] = static_cast<std::uint8_t>(top & 0xFF);       // mov ax, top
        module[kEntryMovAx + 1] = static_cast<std::uint8_t>(top >> 8);
        module[kEntryMovDx] = static_cast<std::uint8_t>(paragraphs & 0xFF);   // mov dx, paras
        module[kEntryMovDx + 1] = static_cast<std::uint8_t>(paragraphs >> 8);
    }
    Packed out;
    out.header.cs = 0xFFF0;
    out.header.ip = 0x100;
    out.header.ss = u16_field(module.size() / 16 + 9, "the stack segment");
    out.header.sp = 0x200;
    out.header.min_alloc = u16_field(static_cast<std::uint64_t>(top) + 0x10 - module.size() / 16, "the minimum allocation");
    out.header.relocs.push_back({.segment = 0, .offset = 7});   // (the word at 0000:0007, the one its entry code adds the memory to)
    out.module = std::move(module);
    return out;
}

void install() {
    formats::dos::set_pklite_unpacker([](ByteView module) -> std::optional<formats::dos::Unpack> {
        try {
            Stored s = read(module);
            formats::dos::Unpack u;
            for (const Relocation& run : s.relocs)
                for (const std::uint16_t offset : run.offsets)
                    if (const std::uint32_t at = static_cast<std::uint32_t>(run.segment) * 16 + offset; at + 2 <= s.image.size()) u.relocations.push_back(at);
            u.module = std::move(s.image);
            u.cs = s.regs.cs, u.ip = s.regs.ip, u.ss = s.regs.ss, u.sp = s.regs.sp;
            return u;
        } catch (const Error&) {
            return std::nullopt;
        }
    });
}

}  // namespace cb::build::pklite
