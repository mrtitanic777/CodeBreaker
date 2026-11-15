// cb-unity: alone
#include "cb/formats/intv.hpp"

#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::intv {

namespace {

constexpr std::string_view kReader = "intv";

std::uint16_t be16(ByteView d, std::size_t at) { return static_cast<std::uint16_t>(d[at] << 8 | d[at + 1]); }

// The header's pointer number `k` (two words, low byte then high byte), when both are bytes.
std::optional<std::uint32_t> pointer(const Rom& r, unsigned k) {
    const auto lo = r.word_at(kHeaderAddress + 2 * k), hi = r.word_at(kHeaderAddress + 2 * k + 1);
    if (!lo || !hi || *lo > 0xFF || *hi > 0xFF) return std::nullopt;
    return *lo | (*hi << 8);
}

}  // namespace

std::uint16_t crc16(ByteView data) {
    std::uint16_t crc = 0xFFFF;
    for (const std::uint8_t b : data) {
        crc = static_cast<std::uint16_t>(crc ^ (b << 8));
        for (int k = 0; k < 8; ++k) crc = static_cast<std::uint16_t>((crc & 0x8000) ? (crc << 1) ^ 0x1021 : crc << 1);
    }
    return crc;
}

const Segment* Rom::segment_at(std::uint32_t address) const {
    for (const Segment& s : segments) {
        if (address >= s.start && address < s.end()) return &s;
    }
    return nullptr;
}

std::optional<std::uint16_t> Rom::word_at(std::uint32_t address) const {
    const Segment* s = segment_at(address);
    if (s == nullptr) return std::nullopt;
    return s->words[address - s->start];
}

std::size_t Rom::word_count() const {
    std::size_t n = 0;
    for (const Segment& s : segments) n += s.words.size();
    return n;
}

std::optional<std::uint32_t> Rom::entry() const {
    const std::optional<std::uint32_t> p = pointer(*this, 2);
    if (p && segment_at(*p) != nullptr) return p;
    return std::nullopt;
}

std::string Rom::title() const {
    const std::optional<std::uint32_t> p = pointer(*this, 5);
    if (!p) return {};
    std::string out;
    for (std::uint32_t a = *p + 1; out.size() < 40; ++a) {
        const auto w = word_at(a);
        if (!w || *w == 0) break;
        if (*w < 32 || *w > 126) return {};
        out.push_back(static_cast<char>(*w));
    }
    return out;
}

Bytes Rom::serialize(bool recompute_crc) const {
    Bytes out;
    if (!raw) out = {0xA8, static_cast<std::uint8_t>(segments.size()), static_cast<std::uint8_t>(~segments.size())};
    for (const Segment& s : segments) {
        const std::size_t begin = out.size();
        if (!raw) {
            out.push_back(static_cast<std::uint8_t>(s.start >> 8));
            out.push_back(static_cast<std::uint8_t>((s.end() - 1) >> 8));
        }
        for (const std::uint16_t w : s.words) {
            out.push_back(static_cast<std::uint8_t>(w >> 8));
            out.push_back(static_cast<std::uint8_t>(w));
        }
        if (!raw) {
            const std::uint16_t crc = recompute_crc ? crc16(ByteView(out).subspan(begin)) : s.crc;
            out.push_back(static_cast<std::uint8_t>(crc >> 8));
            out.push_back(static_cast<std::uint8_t>(crc));
        }
    }
    out.insert(out.end(), tail.begin(), tail.end());
    return out;
}

Rom parse(ByteView d, bool raw) {
    Rom r;
    if (raw) {
        if (d.size() < 64 || d.size() % 2 != 0 || d.size() > 0x20000) throw FormatError(kReader, 0, std::format("{} bytes is not a raw Intellivision ROM", d.size()));
        r.raw = true;
        Segment s;
        s.start = kHeaderAddress;
        for (std::size_t i = 0; i < d.size(); i += 2) s.words.push_back(be16(d, i));
        r.segments.push_back(std::move(s));
        if (!r.entry()) throw FormatError(kReader, 8, "the header at $5000 does not point at a program");
        return r;
    }
    if (d.size() < 3 + 2 + 512 + 2 || d[0] != 0xA8 || d[1] == 0 || static_cast<std::uint8_t>(~d[1]) != d[2]) throw FormatError(kReader, 0, "not an Intellicart .rom file (the header is $A8, N, ~N)");
    std::size_t p = 3;
    for (unsigned k = 0; k < d[1]; ++k) {
        if (p + 2 > d.size() || d[p + 1] < d[p]) throw FormatError(kReader, p, "a segment's page range is wrong");
        Segment s;
        s.start = static_cast<std::uint32_t>(d[p]) << 8;
        const std::size_t words = (static_cast<std::size_t>(d[p + 1]) - d[p] + 1) * 256;
        p += 2;
        if (p + words * 2 + 2 > d.size()) throw FormatError(kReader, p, "a segment is cut short");
        s.words.reserve(words);
        for (std::size_t i = 0; i < words; ++i) s.words.push_back(be16(d, p + 2 * i));
        p += words * 2;
        s.crc = be16(d, p);
        p += 2;
        r.segments.push_back(std::move(s));
    }
    r.tail.assign(d.begin() + static_cast<std::ptrdiff_t>(p), d.end());
    return r;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view(), extension_of(path) != ".rom");
}

bool probe(const std::filesystem::path& path) {
    const std::optional<std::uint64_t> size = size_of(path);
    if (!size) return false;
    try {
        if (has_extension(path, {".rom"})) {
            if (*size < 519 || *size > 0x400000) return false;
            const fs::MappedFile file(path);
            (void)parse(file.view(), false);
            return true;
        }
        if (has_extension(path, {".int"})) {
            if (*size > 0x20000) return false;
            const fs::MappedFile file(path);
            (void)parse(file.view(), true);
            return true;
        }
    } catch (const Error&) {
    }
    return false;
}

}  // namespace cb::formats::intv
