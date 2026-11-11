// cb/formats/le_w4.cpp -- LE containers: the W3 directory, the W4 page compression, and the modules a file holds
// (le.hpp).
#include <algorithm>

#include "cb/core/fs.hpp"
#include "cb/formats/le.hpp"
#include "cb/formats/le_internal.hpp"
#include "cb/formats/read.hpp"

namespace cb::formats::le {

using detail::kReader;

namespace {

// The members of the W3 directory at `at`: name (eight bytes, blank/zero padded), offset, size.
std::vector<ContainerMember> read_w3_directory(ByteView d, std::uint64_t at, std::uint16_t count) {
    std::vector<ContainerMember> members;
    for (std::uint32_t i = 0; i < count; ++i) {
        const std::uint64_t p = at + 0x10 + 16 * std::uint64_t{i};
        ContainerMember m;
        m.name = text::from_latin1(detail::rstrip_blanks(clamped(d, p, 8)));
        m.offset = get<std::uint32_t>(d, p + 8, kReader);
        m.size = get<std::uint32_t>(d, p + 12, kReader);
        members.push_back(std::move(m));
    }
    return members;
}

bool holds_le(ByteView d, std::uint64_t at) { return starts_with(d, at, "LE") || starts_with(d, at, "LX"); }

// Bits are read low first.
class BitReader {
public:
    explicit BitReader(ByteView buf) : buf_(buf) {}
    std::uint64_t take(int n = 1) {
        std::uint64_t v = 0;
        for (int i = 0; i < n; ++i) {
            v |= std::uint64_t{(get<std::uint8_t>(buf_, pos_ >> 3, kReader) >> (pos_ & 7)) & 1u} << i;
            ++pos_;
        }
        return v;
    }

private:
    ByteView buf_;
    std::uint64_t pos_ = 0;
};

}  // namespace

// ---- containers --------------------------------------------------------------------------------------------

std::optional<Container> container(ByteView data) {
    if (!starts_with(data, 0, "MZ") || data.size() < 0x40) return std::nullopt;
    const std::uint64_t at = get<std::uint32_t>(data, 0x3C, kReader);
    Container c;
    c.at = at;
    if (starts_with(data, at, "W4")) {
        c.kind = Container::Kind::W4;
        c.version = get<std::uint16_t>(data, at + 2, kReader);
        return c;
    }
    if (!starts_with(data, at, "W3")) return std::nullopt;
    c.kind = Container::Kind::W3;
    const std::uint16_t count = get<std::uint16_t>(data, at + 4, kReader);
    c.members = read_w3_directory(data, at, count);
    c.version = get<std::uint16_t>(data, at + 2, kReader);
    const ByteView reserved = clamped(data, at + 6, 10);
    c.reserved.assign(reserved.begin(), reserved.end());
    return c;
}

Bytes pack_directory(const Container& c) {
    Bytes out{'W', '3'};
    auto put16 = [&](std::uint64_t v) {
        if (v > 0xFFFF) throw FormatError(kReader, 0, "W3 directory field does not fit 16 bits");
        out.push_back(static_cast<std::uint8_t>(v));
        out.push_back(static_cast<std::uint8_t>(v >> 8));
    };
    auto put32 = [&](std::uint32_t v) {
        for (int k = 0; k < 4; ++k) out.push_back(static_cast<std::uint8_t>(v >> (8 * k)));
    };
    put16(c.version);
    put16(c.members.size());
    append(out, c.reserved);
    for (const ContainerMember& m : c.members) {
        Bytes n = text::to_latin1(m.name);
        if (n.size() < 8) n.resize(8, ' ');
        append(out, n);
        put32(m.offset);
        put32(m.size);
    }
    return out;
}

std::vector<std::pair<std::string, Module>> modules(Bytes data, std::string_view name) {
    std::vector<std::pair<std::string, Module>> out;
    if (container(data)) {
        // a container: its members, from the expanded image (the file's own bytes when expanding changes nothing)
        Bytes expanded = expand(data);
        const std::shared_ptr<const Bytes> shared =
            std::make_shared<const Bytes>(equal_bytes(expanded, data) ? std::move(data) : std::move(expanded));
        const auto c = container(*shared);
        if (!c) throw FormatError(kReader, 0, "the expanded image is not a container");
        for (const ContainerMember& m : c->members)
            if (holds_le(*shared, m.offset)) out.emplace_back(m.name, Module::parse(shared, m.offset));
        return out;
    }
    const std::uint64_t lfa = get<std::uint32_t>(data, 0x3C, kReader);
    if (!holds_le(data, lfa)) throw FormatError(kReader, lfa, "not an LE/LX/W3 image");
    out.emplace_back(std::string(name), Module::parse(std::move(data), lfa));
    return out;
}

std::vector<std::pair<std::string, Module>> open_modules(const std::filesystem::path& file) {
    return modules(fs::read_file(file), file.filename().string());
}

// ---- W4 page compression -----------------------------------------------------------------------------------

Bytes expand_page(ByteView compressed, std::size_t size) {
    BitReader bits(compressed);
    Bytes out;
    while (out.size() < size) {
        const std::uint64_t b0 = bits.take(), b1 = bits.take();
        if (b0 != b1) {  // a literal byte
            out.push_back(static_cast<std::uint8_t>((b0 << 7) | bits.take(7)));
            continue;
        }
        std::uint64_t distance;
        if (b0 == 0) {
            distance = bits.take(6);
        } else if (bits.take() == 0) {
            distance = bits.take(8) + 64;
        } else {
            distance = bits.take(12);
            if (distance == 0xFFF) continue;  // the end of a 512-byte stretch
            distance += 320;
        }
        int k = 0;
        while (bits.take() == 0)
            if (++k > 30) throw FormatError(kReader, 0, "W4 run length is too long");
        const std::uint64_t n = (std::uint64_t{1} << k) + bits.take(k) + 1;
        if (distance == 0 || distance > out.size()) throw FormatError(kReader, 0, "W4 copy reaches before the page");
        for (std::uint64_t j = 0; j < n; ++j) out.push_back(out[out.size() - distance]);
    }
    return out;
}

Bytes compress_page(ByteView page) {
    std::vector<std::uint8_t> bits;
    auto put = [&bits](std::uint64_t v, int n) {
        for (int i = 0; i < n; ++i) bits.push_back(static_cast<std::uint8_t>((v >> i) & 1));
    };
    const std::int64_t n = static_cast<std::int64_t>(page.size());
    std::vector<std::int64_t> heads(0x10000, -1), prev(page.size(), -1);
    auto pair_at = [&page](std::int64_t j) { return (page[static_cast<std::size_t>(j)] << 8) | page[static_cast<std::size_t>(j) + 1]; };
    std::int64_t i = 0;
    while (i < n) {
        const std::int64_t room = (i / 512 + 1) * 512 - i;
        std::int64_t best = 1, distance = 0;
        if (room >= 2 && i + 1 < n) {
            std::int64_t p = heads[static_cast<std::size_t>(pair_at(i))];
            while (p >= std::max<std::int64_t>(0, i - 4414)) {
                std::int64_t length = 0;
                while (length < room && i + length < n && page[static_cast<std::size_t>(p + length)] == page[static_cast<std::size_t>(i + length)])
                    ++length;
                if (length > best) {
                    best = length;
                    distance = i - p;
                    if (length == room) break;
                }
                p = prev[static_cast<std::size_t>(p)];
            }
        }
        if (best >= 2) {
            if (distance <= 63) {
                put(0, 2);
                put(static_cast<std::uint64_t>(distance), 6);
            } else if (distance <= 319) {
                put(3, 3);
                put(static_cast<std::uint64_t>(distance - 64), 8);
            } else {
                put(7, 3);
                put(static_cast<std::uint64_t>(distance - 320), 12);
            }
            int k = 0;
            while ((std::int64_t{1} << (k + 1)) <= best - 1) ++k;  // (best - 1).bit_length() - 1
            put(0, k);
            put(1, 1);
            put(static_cast<std::uint64_t>(best - 1 - (std::int64_t{1} << k)), k);
        } else {
            const int top = page[static_cast<std::size_t>(i)] >> 7;
            put(static_cast<std::uint64_t>(top | ((1 - top) << 1)), 2);
            put(page[static_cast<std::size_t>(i)] & 0x7F, 7);
        }
        for (std::int64_t j = i; j < i + best; ++j) {
            if (j + 1 < n) {
                prev[static_cast<std::size_t>(j)] = heads[static_cast<std::size_t>(pair_at(j))];
                heads[static_cast<std::size_t>(pair_at(j))] = j;
            }
        }
        i += best;
        if (i % 512 == 0) {
            put(7, 3);
            put(0xFFF, 12);
        }
    }
    if (n % 512) {
        put(7, 3);
        put(0xFFF, 12);
    }
    while (bits.size() % 8) bits.push_back(0);
    Bytes out;
    for (std::size_t k = 0; k < bits.size(); k += 8) {
        unsigned v = 0;
        for (int j = 0; j < 8; ++j) v |= static_cast<unsigned>(bits[k + static_cast<std::size_t>(j)]) << j;
        out.push_back(static_cast<std::uint8_t>(v));
    }
    return out;
}

Bytes expand(ByteView data) {
    auto c = container(data);
    if (!c || c->kind != Container::Kind::W4) return Bytes(data.begin(), data.end());
    const std::uint64_t at = c->at;
    const std::uint16_t size = get<std::uint16_t>(data, at + 4, kReader), count = get<std::uint16_t>(data, at + 6, kReader);
    std::vector<std::uint64_t> offs;
    for (std::uint32_t i = 0; i < count; ++i) offs.push_back(get<std::uint32_t>(data, at + 0x10 + 4 * std::uint64_t{i}, kReader));
    offs.push_back(data.size());
    const ByteView head = clamped(data, 0, at);
    Bytes out(head.begin(), head.end());
    for (std::uint32_t i = 0; i < count; ++i) {
        const ByteView part = offs[i + 1] > offs[i] ? clamped(data, offs[i], offs[i + 1] - offs[i]) : ByteView();
        if (offs[i + 1] >= offs[i] && offs[i + 1] - offs[i] == size) append(out, part);
        else append(out, expand_page(part, size));
    }
    return out;
}

Bytes compress(ByteView data, ByteView like) {
    auto c = container(like);
    if (!c) throw FormatError(kReader, 0, "the model image is not a container");
    const std::uint64_t at = c->at;
    const std::uint16_t size = get<std::uint16_t>(like, at + 4, kReader);
    if (size == 0) throw FormatError(kReader, at + 4, "the model container has a zero page size");
    const ByteView body = at < data.size() ? data.subspan(at) : ByteView();
    std::vector<Bytes> pages;
    for (std::size_t i = 0; i < body.size(); i += size) {
        const ByteView piece = body.subspan(i, std::min<std::size_t>(size, body.size() - i));
        Bytes page(piece.begin(), piece.end());
        page.resize(size, 0);
        pages.push_back(compress_page(page));
    }
    if (pages.size() > 0xFFFF) throw FormatError(kReader, at + 6, "too many pages for a W4 container");
    Bytes out(like.begin(), like.begin() + static_cast<std::ptrdiff_t>(std::min<std::uint64_t>(at + 0x10, like.size())));
    if (at + 8 > out.size()) throw FormatError(kReader, at + 6, "the model container header is cut short");
    out[at + 6] = static_cast<std::uint8_t>(pages.size());
    out[at + 7] = static_cast<std::uint8_t>(pages.size() >> 8);
    std::uint64_t pos = at + 0x10 + 4 * std::uint64_t{pages.size()};
    for (const Bytes& pg : pages) {
        if (pos > 0xFFFFFFFFull) throw FormatError(kReader, pos, "W4 page offset does not fit 32 bits");
        for (int k = 0; k < 4; ++k) out.push_back(static_cast<std::uint8_t>(pos >> (8 * k)));
        pos += pg.size();
    }
    for (const Bytes& pg : pages) append(out, pg);
    return out;
}

}  // namespace cb::formats::le
