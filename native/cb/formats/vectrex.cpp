// cb-unity: alone
#include "cb/formats/vectrex.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::vectrex {

namespace {

constexpr std::string_view kReader = "vectrex";

bool has_header(ByteView d) { return d.size() >= 8 && d[0] == 'g' && d[1] == ' ' && d[2] == 'G' && d[3] == 'C' && d[4] == 'E'; }

}  // namespace

Bytes Rom::memory() const {
    Bytes mem(0x10000, 0);
    std::copy(data.begin(), data.end(), mem.begin());
    return mem;
}

std::vector<std::uint32_t> Rom::functions(const Decoder& decode) const {
    return find_functions(memory(), {entry}, decode, {.begin = 0, .end = static_cast<std::uint32_t>(data.size())});
}

Rom parse(ByteView d) {
    if (d.empty() || d.size() > kMaxSize) throw FormatError(kReader, 0, std::format("{} bytes is not a Vectrex cartridge (1 byte to 32 KB)", d.size()));
    if (!has_header(d)) throw FormatError(kReader, 0, "the cartridge does not start with the copyright text `g GCE`");
    Rom r;
    r.data.assign(d.begin(), d.end());
    std::size_t p = 0;
    while (p < d.size() && d[p] != 0x80) ++p;
    if (p + 3 > d.size()) throw FormatError(kReader, p, "the copyright text does not end");
    r.copyright.assign(reinterpret_cast<const char*>(d.data()), p);
    ++p;
    r.music = static_cast<std::uint16_t>(d[p] << 8 | d[p + 1]);
    p += 2;
    while (p < d.size() && d[p] != 0) {   // title lines: height, width, y, x, text, $80
        if (p + 5 > d.size()) throw FormatError(kReader, p, "a title line is cut short");
        TitleLine t;
        t.height = static_cast<std::int8_t>(d[p]);
        t.width = static_cast<std::int8_t>(d[p + 1]);
        t.y = static_cast<std::int8_t>(d[p + 2]);
        t.x = static_cast<std::int8_t>(d[p + 3]);
        p += 4;
        while (p < d.size() && d[p] != 0x80) t.text.push_back(static_cast<char>(d[p++]));
        if (p >= d.size()) throw FormatError(kReader, p, "a title line does not end");
        ++p;
        r.title.push_back(std::move(t));
        if (r.title.size() > 16) throw FormatError(kReader, p, "too many title lines");
    }
    if (p >= d.size()) throw FormatError(kReader, p, "the header's zero byte is missing");
    r.entry = static_cast<std::uint32_t>(p + 1);
    if (r.entry >= d.size()) throw FormatError(kReader, p, "no program after the header");
    return r;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    if (!has_extension(path, {".vec", ".bin", ".gam"})) return false;
    const std::optional<std::uint64_t> size = size_of(path);
    if (!size || *size == 0 || *size > kMaxSize) return false;
    const std::optional<Bytes> head = read_prefix(path, 0, 8);
    return head && has_header(*head);
}

}  // namespace cb::formats::vectrex
