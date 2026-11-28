#include "formats/xbox/xzp.hpp"

#include <algorithm>
#include <filesystem>
#include <utility>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "formats/format_builtin.hpp"
#include "core/text.hpp"

namespace cb::formats::xbox::xzp {

namespace eng = cb::formats::engine;

namespace {

constexpr std::string_view kReader = "xzp";
constexpr std::size_t kBaseRelative = 0x16;   // the payload base is counted from here
constexpr std::size_t kV1Table = 0x17;
constexpr std::uint32_t kMaxV1Name = 512;   // UTF-16 units

}  // namespace

// The layout is xzp.fmt's; where a v1 table ends and what a name holds are this reader's.
Pack parse(ByteView d) {
    if (d.size() < 4 || as_text(d.subspan(0, 4)) != "XUIZ") throw FormatError(kReader, 0, "not a XUIZ container");
    eng::ReadOptions options;
    options.keep_source = false;   // only read from
    const eng::Tree tree = [&] {
        try {
            return eng::builtin().read("xzp", d, options);
        } catch (const TruncatedError&) {   // (the table is read tolerantly: only the header's words can be cut short)
            throw FormatError(kReader, 0, "the XUIZ header is cut short");
        }
    }();
    const eng::Node& h = tree.root;
    Pack pack;
    const std::uint64_t version = h.u("version");
    pack.file_size = static_cast<std::uint32_t>(h.u("file_size"));
    pack.base = h.u("base");
    const std::vector<eng::Node>& entries = h.get("entries").children;
    if (version >= 3) {
        pack.version = 3;
        std::size_t p = kBaseRelative;
        for (const eng::Node& n : entries) {
            Entry e;
            e.size = static_cast<std::uint32_t>(n.u("size"));
            e.offset = static_cast<std::uint32_t>(n.u("offset"));
            e.name = text::from_latin1(as_bytes(n.get("name").text));   // (a name the file cuts short is what is there)
            pack.entries.push_back(std::move(e));
            p = n.offset + 9 + n.u("length");
        }
        pack.table_end = p;
        return pack;
    }
    pack.version = 1;
    std::size_t p = kV1Table;
    for (const eng::Node& n : entries) {
        p = n.offset;
        const std::uint64_t size = n.u("size"), offset = n.u("offset"), units = n.u("units");
        if (units == 0 || units > kMaxV1Name || p + 9 + units * 2 > d.size()) break;
        const eng::Node& name_node = n.get("name");
        const ByteView raw = d.subspan(name_node.offset, name_node.size);
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
        pack.entries.push_back({static_cast<std::uint32_t>(size), static_cast<std::uint32_t>(offset), std::move(name)});
        p = n.u("name_end");
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
