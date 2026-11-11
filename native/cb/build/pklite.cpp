#include "cb/build/pklite.hpp"

#include <algorithm>
#include <format>
#include <string>
#include <unordered_map>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"

namespace cb::build::pklite {

namespace {

constexpr std::uint32_t kLengthWithByte = 25;   // the length code that is followed by a byte: a copy of 25 + that byte
constexpr std::uint8_t kMarkByte = 0xFE;
constexpr std::uint8_t kEndByte = 0xFF;
constexpr std::uint32_t kFirstMark = 0x8000, kMarkEvery = 0xA000;
constexpr std::size_t kMaxCodeBits = 12;        // no prefix code of the decompressor is longer
constexpr std::size_t kMaxShortCopyDistance = 255;   // a length-2 copy comes from no farther
constexpr std::uint32_t kEntryMovAx = 1, kEntryMovDx = 4;   // where the entry code holds the memory it needs, and the stream's size
constexpr std::uint8_t kEntryCode[] = {0x8C, 0xDB, 0x53, 0x81, 0xC3};   // mov bx,ds / push bx / add bx,N
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

// The length code, as the decompressor reads it (`t`: its code from offset 0).
template <class Bits>
std::uint32_t read_length(Bits& in, ByteView t) {
    const auto more = [&in](std::uint32_t bx) { return (bx << 1) | in.bit(); };
    std::uint32_t bx = in.bit() << 1;
    bx |= in.bit();
    if (bx >= 2) return bx;
    bx = more(bx);
    if (bx == 0) return in.at(t, 0x209);
    bx = more(bx);
    if (bx < 5) return in.at(t, 0x209 + bx);
    bx = more(bx);
    if (bx <= 0xC) return in.at(t, 0x209 + bx);
    bx = ((bx & 3) << 1) | in.bit();
    if (bx >= 5) {
        bx = more(bx);
        if (bx > 0xC) {
            bx = ((bx & 3) << 1) | in.bit();
            if (bx >= 5) bx = more(bx);
            return in.at(t, 0x21F + bx);
        }
    }
    return in.at(t, 0x214 + bx);
}

// The code for a distance's high byte (after the bit that says it has one).
template <class Bits>
std::uint32_t read_high(Bits& in, ByteView t) {
    const auto more = [&in](std::uint32_t bx) { return (bx << 1) | in.bit(); };
    std::uint32_t bx = in.bit() << 2;
    bx |= in.bit() << 1;
    bx |= in.bit();
    if (bx < 2) return in.at(t, 0x22F + bx);
    bx = more(bx);
    if (bx < 8) return in.at(t, 0x22F + bx);
    bx = more(bx);
    if (bx < 0x17) return in.at(t, 0x22F + bx);
    bx = more(bx);
    return bx & 0xDF;
}

using CodeReader = std::uint32_t (*)(BitString&, ByteView);

// {value: bits} of a prefix code, by feeding the reader every string of bits: a string the reader runs out of is
// extended by a 0 and by a 1; one it reads to the end of is a value's code (the first string found for a value wins).
void collect_codes(CodeReader read, ByteView t, std::vector<std::uint8_t>& prefix, Codes& out) {
    BitString in(prefix);
    const std::uint32_t value = read(in, t);
    if (in.ran_out()) {
        if (prefix.size() < kMaxCodeBits) {
            for (int next = 0; next < 2; ++next) {
                prefix.push_back(static_cast<std::uint8_t>(next));
                collect_codes(read, t, prefix, out);
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

Codes codes_of(CodeReader read, ByteView t) {
    Codes out;
    std::vector<std::uint8_t> prefix;
    collect_codes(read, t, prefix, out);
    return out;
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
    while (true) {
        if (!in.bit()) {
            const std::uint8_t b = static_cast<std::uint8_t>(in.byte());
            out.image.push_back(b);
            out.tokens.push_back(Token::literal(b));
            continue;
        }
        std::uint32_t n = read_length(in, t);
        if (n == kLengthWithByte) {
            const std::uint32_t b = in.byte();
            if (b == kMarkByte) {
                out.tokens.push_back(Token::mark());
                continue;
            }
            if (b == kEndByte) {
                out.tokens.push_back(Token::end());
                out.end = in.position();
                return out;
            }
            n += b;
        }
        const std::uint32_t high = (n == 2 || in.bit()) ? 0 : read_high(in, t);
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
    for (const Token& t : tokens) {
        if (t.kind == Token::Kind::Literal) {
            w.bit(0);
            w.byte(t.byte);
            continue;
        }
        w.bit(1);
        if (t.kind == Token::Kind::Mark || t.kind == Token::Kind::End) {
            w.bits(code_for(stub.lengths, kLengthWithByte, "length"));
            w.byte(t.kind == Token::Kind::Mark ? kMarkByte : kEndByte);
            continue;
        }
        const std::uint32_t n = t.length, d = t.distance;
        if (n >= kLengthWithByte) {
            w.bits(code_for(stub.lengths, kLengthWithByte, "length"));
            w.byte(static_cast<std::uint8_t>(n - kLengthWithByte));
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
    explicit Matcher(ByteView data) : data_(data), previous_(data.size(), -1) {
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
        const std::size_t limit = std::min<std::size_t>(kMaxLength, n - pos);
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
    if (module.size() > kStubCode) stub.code.assign(module.begin() + kStubCode, module.end());
    stub.lengths = codes_of(read_length<BitString>, stub.code);
    stub.highs = codes_of(read_high<BitString>, stub.code);
    return stub;
}

std::size_t stream_start(ByteView module) {
    const auto at = std::search(module.begin(), module.end(), std::begin(kEntryCode), std::end(kEntryCode));
    if (at == module.end()) bad(0, "not PKLITE's decompressor (no start of the stream found)");
    io::ByteReader reader(module, "pklite");
    reader.seek(static_cast<std::uint64_t>(at - module.begin()) + sizeof kEntryCode);
    const std::int64_t start = static_cast<std::int64_t>(reader.u16()) * 16 - 0x100;
    if (start < 0) bad(reader.pos(), "the stream would start before the module");
    return static_cast<std::size_t>(start);
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
    while (const std::uint8_t count = reader.u8()) {
        Relocation run;
        run.segment = reader.u16();
        run.offsets.reserve(count);
        for (unsigned i = 0; i < count; ++i) run.offsets.push_back(reader.u16());
        s.relocs.push_back(std::move(run));
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

std::vector<Token> parse(ByteView image) {
    const Matcher matcher(image);
    std::vector<Token> tokens;
    std::size_t pos = 0;
    std::uint32_t mark = kFirstMark;
    Match cur = image.empty() ? Match{} : matcher.longest(0);
    while (pos < image.size()) {
        if (pos + kMaxLength >= mark) {
            tokens.push_back(Token::mark());
            mark += kMarkEvery;
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

Bytes relocation_table(std::span<const Relocated> relocs) {
    io::ByteWriter out;
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

Packed pack(ByteView image, std::span<const Relocated> relocs, Registers regs, ByteView head, std::uint32_t memory) {
    const std::vector<Token> tokens = parse(image);
    const Stub stub = Stub::of(head, head.size());
    io::ByteWriter body;
    {
        const Bytes stream = encode(tokens, stub);
        const Bytes table = relocation_table(relocs);
        body.bytes(stream);
        body.bytes(table);
        body.u16(regs.ss);
        body.u16(regs.sp);
        body.u16(regs.cs);
        body.u16(regs.ip);
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
    module[kEntryMovAx] = static_cast<std::uint8_t>(top & 0xFF);       // mov ax, top
    module[kEntryMovAx + 1] = static_cast<std::uint8_t>(top >> 8);
    module[kEntryMovDx] = static_cast<std::uint8_t>(paragraphs & 0xFF);   // mov dx, paras
    module[kEntryMovDx + 1] = static_cast<std::uint8_t>(paragraphs >> 8);
    Packed out;
    out.header.cs = 0xFFF0;
    out.header.ip = 0x100;
    out.header.ss = u16_field(module.size() / 16 + 9, "the stack segment");
    out.header.sp = 0x200;
    out.header.min_alloc = u16_field(static_cast<std::uint64_t>(top) + 0x10 - module.size() / 16, "the minimum allocation");
    out.header.relocs.push_back({7, 0});
    out.module = std::move(module);
    return out;
}

}  // namespace cb::build::pklite
