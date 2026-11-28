#include "formats/xbox/xzp.hpp"

#include <algorithm>
#include <filesystem>
#include <utility>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "formats/gen/xzp.hpp"

namespace cb::formats::xbox::xzp {

namespace {

constexpr std::string_view kReader = "xzp";
constexpr std::size_t kBaseRelative = 0x16;   // the payload base is counted from here
constexpr std::size_t kV1Table = 0x17;
constexpr std::uint32_t kMaxV1Name = 512;   // UTF-16 units

}  // namespace

// The layout is xzp.fmt's (its generated reader, formats/gen/xzp); where a v1 table ends and what a name holds are this
// reader's.
Pack parse(ByteView d) {
    if (d.size() < 4 || as_text(d.subspan(0, 4)) != "XUIZ") throw FormatError(kReader, 0, "not a XUIZ container");
    const gen::xzp::File h = [&] {
        try {
            return gen::xzp::read(d);
        } catch (const TruncatedError&) {   // (the table is read tolerantly: only the header's words can be cut short)
            throw FormatError(kReader, 0, "the XUIZ header is cut short");
        }
    }();
    Pack pack;
    pack.file_size = h.file_size;
    pack.base = static_cast<std::uint64_t>(h.base);
    if (h.version >= 3) {
        pack.version = 3;
        std::uint64_t p = kBaseRelative;
        if (!h.entries_V3Entry) throw FormatError(kReader, 0x16, "the XUIZ table is not there");
        for (const gen::xzp::V3Entry& n : *h.entries_V3Entry) {
            Entry e;
            e.size = n.size;
            e.offset = n.offset;
            e.name = text::from_latin1(as_bytes(n.name));   // (a name the file cuts short is what is there)
            pack.entries.push_back(std::move(e));
            p = n._offset + 9 + n.length;
        }
        pack.table_end = static_cast<std::size_t>(p);
        return pack;
    }
    pack.version = 1;
    std::uint64_t p = kV1Table;
    if (!h.entries_V1Entry) throw FormatError(kReader, kV1Table, "the XUIZ table is not there");
    for (const gen::xzp::V1Entry& n : *h.entries_V1Entry) {
        p = n._offset;
        if (n.units == 0 || n.units > kMaxV1Name || p + 9 + std::uint64_t{n.units} * 2 > d.size()) break;
        const ByteView raw = n.name;
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
        if (name.empty() || pack.base + n.offset + n.size > d.size()) break;
        pack.entries.push_back({n.size, n.offset, std::move(name)});
        p = static_cast<std::uint64_t>(n.name_end);
    }
    pack.table_end = static_cast<std::size_t>(p);
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

Bytes build(const std::filesystem::path& folder, ByteView template_pack) {
    if (template_pack.size() < kV1Table || as_text(template_pack.subspan(0, 4)) != "XUIZ") {
        throw FormatError(kReader, 0, "the template is not a XUIZ container");
    }
    struct File {
        std::string name;
        std::filesystem::path path;
    };
    std::vector<File> files;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(folder)) {
        if (!entry.is_regular_file()) continue;
        files.push_back({text::replace_all(fs::to_text(entry.path().lexically_relative(folder)), "/", "\\"), entry.path()});
    }
    std::stable_sort(files.begin(), files.end(), [](const File& a, const File& b) { return text::lower(a.name) < text::lower(b.name); });

    Bytes table, payload;
    for (const File& f : files) {
        const Bytes data = fs::read_file(f.path);
        const Bytes name = text::to_utf16le(f.name);
        const std::size_t units = name.size() / 2;
        if (data.size() >= (std::size_t{1} << 24) || units > 0xFFFF) throw UnsupportedError(kReader, 0, "an entry too large for the v1 table: " + f.name);
        const std::size_t offset = payload.size();
        for (int shift = 16; shift >= 0; shift -= 8) table.push_back(static_cast<std::uint8_t>(data.size() >> shift));   // u24be size
        for (int shift = 24; shift >= 0; shift -= 8) table.push_back(static_cast<std::uint8_t>(offset >> shift));       // u32be offset
        table.push_back(static_cast<std::uint8_t>(units));                                                              // u16le name length
        table.push_back(static_cast<std::uint8_t>(units >> 8));
        table.insert(table.end(), name.begin(), name.end());
        payload.insert(payload.end(), data.begin(), data.end());
    }
    Bytes out(template_pack.begin(), template_pack.begin() + kV1Table);
    const std::uint32_t base_relative = static_cast<std::uint32_t>(kV1Table + table.size() - kBaseRelative);
    for (int i = 0; i < 4; ++i) out[0x10 + i] = static_cast<std::uint8_t>(base_relative >> (24 - 8 * i));
    out.insert(out.end(), table.begin(), table.end());
    out.insert(out.end(), payload.begin(), payload.end());
    for (int i = 0; i < 4; ++i) out[0x08 + i] = static_cast<std::uint8_t>(out.size() >> (24 - 8 * i));
    return out;
}

}  // namespace cb::formats::xbox::xzp
