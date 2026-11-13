#include "cb/formats/xbox/xzp.hpp"

#include <algorithm>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"

namespace cb::formats::xbox::xzp {

namespace {

constexpr std::string_view kReader = "xzp";
constexpr std::size_t kBaseRelative = 0x16;   // the payload base is counted from here
constexpr std::size_t kV1Table = 0x17;
constexpr std::uint32_t kMaxV1Name = 512;   // UTF-16 units

std::uint32_t be32_at(ByteView d, std::size_t at) {
    io::ByteReader r(io::checked_range(d, at, 4), kReader);
    return r.u32_be();
}

}  // namespace

Pack parse(ByteView d) {
    if (d.size() < 4 || as_text(d.subspan(0, 4)) != "XUIZ") throw FormatError(kReader, 0, "not a XUIZ container");
    Pack pack;
    const std::uint32_t version = be32_at(d, 4);
    pack.file_size = be32_at(d, 8);
    pack.base = kBaseRelative + std::uint64_t{be32_at(d, 0x10)};
    if (version >= 3) {
        pack.version = 3;
        io::ByteReader count_reader(io::checked_range(d, 0x14, 2), kReader);
        const std::uint32_t count = count_reader.u16_be();
        std::size_t p = kBaseRelative;
        for (std::uint32_t k = 0; k < count; ++k) {
            if (p + 9 > d.size()) break;
            Entry e;
            e.size = be32_at(d, p);
            e.offset = be32_at(d, p + 4);
            const std::size_t length = d[p + 8];
            const std::size_t end = std::min(d.size(), p + 9 + length);   // (a name the file cuts short is what is there)
            e.name = text::from_latin1(d.subspan(p + 9, end - (p + 9)));
            pack.entries.push_back(std::move(e));
            p += 9 + length;
        }
        pack.table_end = p;
        return pack;
    }
    pack.version = 1;
    std::size_t p = kV1Table;
    while (p + 9 <= d.size()) {
        const std::uint32_t size = (std::uint32_t{d[p]} << 16) | (std::uint32_t{d[p + 1]} << 8) | d[p + 2];
        const std::uint32_t offset = be32_at(d, p + 3);
        const std::uint32_t units = d[p + 7] | (std::uint32_t{d[p + 8]} << 8);
        if (units == 0 || units > kMaxV1Name || p + 9 + std::size_t{units} * 2 > d.size()) break;
        const std::size_t name_end = static_cast<std::size_t>(std::min<std::uint64_t>(p + 9 + std::uint64_t{units} * 2, pack.base));
        const ByteView raw = d.subspan(std::min(p + 9, name_end), name_end > p + 9 ? name_end - (p + 9) : 0);
        std::string decoded;
        if (raw.size() % 2 != 0) {   // a name the payload base cuts in half: the whole units, then the lone byte as a character
            decoded = text::from_utf16le(raw.subspan(0, raw.size() - 1));
            decoded += text::from_latin1(raw.subspan(raw.size() - 1));
        } else {
            decoded = text::from_utf16le(raw);
        }
        std::string name;   // printable ASCII only
        for (const char c : decoded) {
            if (c >= 32 && c < 127) name.push_back(c);
        }
        if (name.empty() || pack.base + offset + size > d.size()) break;
        pack.entries.push_back({size, offset, std::move(name)});
        p = static_cast<std::size_t>(name_end);
    }
    pack.table_end = p;
    return pack;
}

std::vector<Payload> payloads(ByteView d) {
    const Pack pack = parse(d);
    std::vector<Payload> out;
    for (const Entry& e : pack.entries) {
        std::string relative;
        const std::string slashed = text::replace_all(e.name, "\\", "/");   // (a named local: the parts view into it)
        for (const std::string_view part : text::split(slashed, '/')) {
            if (part.empty() || part == "." || part == ".." || part.find(':') != std::string_view::npos) continue;
            if (!relative.empty()) relative += '/';
            relative += part;
        }
        if (relative.empty()) continue;
        const std::uint64_t at = pack.base + e.offset;
        Payload p;
        p.path = std::move(relative);
        p.offset = static_cast<std::size_t>(std::min<std::uint64_t>(at, d.size()));
        p.size = static_cast<std::size_t>(std::min<std::uint64_t>(e.size, d.size() - p.offset));
        out.push_back(std::move(p));
    }
    return out;
}

}  // namespace cb::formats::xbox::xzp
