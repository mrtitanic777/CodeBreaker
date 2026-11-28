#include "formats/xbox/pck.hpp"

#include <algorithm>
#include <format>

#include "core/error.hpp"
#include "core/io.hpp"
#include "core/text.hpp"
#include "formats/format_builtin.hpp"

namespace cb::formats::xbox::pck {

namespace eng = cb::formats::engine;

namespace {

constexpr std::size_t kFirstPayloadGap = 12;     // bytes between the table and the first payload
constexpr std::size_t kMaxPadding = 512;         // zero padding skipped before a payload
constexpr std::uint8_t kPng[] = {0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A};

// UTF-16 big-endian text, decoded
std::string utf16be(ByteView raw) {
    Bytes le(raw.begin(), raw.end());
    for (std::size_t i = 0; i + 1 < le.size(); i += 2) std::swap(le[i], le[i + 1]);
    return text::from_utf16le(le);
}

bool ends_with_png(const std::string& name) { return text::lower(name).ends_with(".png"); }

}  // namespace

Archive parse(ByteView file) {
    // The layout is pck.fmt's.
    eng::ReadOptions options;
    options.keep_source = false;   // only read from
    const eng::Tree tree = eng::builtin().read("pck", file, options);
    auto text_of = [&](const eng::Node& n) { return utf16be(file.subspan(n.offset, n.size)); };
    Archive a;
    a.version = static_cast<std::uint32_t>(tree.root.u("version"));
    for (const eng::Node& p : tree.root.get("properties").children) a.properties.emplace_back(static_cast<std::uint32_t>(p.u("id")), text_of(p.get("name")));
    for (const eng::Node& n : tree.root.get("entries").children)
        a.entries.push_back({static_cast<std::uint32_t>(n.u("size")), static_cast<std::uint32_t>(n.u("type")), text_of(n.get("name"))});
    a.table_end = static_cast<std::size_t>(tree.root.u("table_end"));
    return a;
}

Payloads payloads(ByteView file) {
    const Archive a = parse(file);
    Payloads out;
    out.file_size = file.size();
    std::size_t p = a.table_end + kFirstPayloadGap;
    for (const Entry& e : a.entries) {
        if (e.size == 0) continue;
        std::size_t at = p;
        if (ends_with_png(e.name)) {   // exact: a PNG is self-delimiting, so it is found by its signature
            const auto from = p <= file.size() ? file.begin() + static_cast<std::ptrdiff_t>(p) : file.end();
            const auto hit = std::search(from, file.end(), std::begin(kPng), std::end(kPng));
            if (hit == file.end()) continue;
            at = static_cast<std::size_t>(hit - file.begin());
        } else {
            while (at < file.size() && file[at] == 0 && at - p < kMaxPadding && p != a.table_end + kFirstPayloadGap) ++at;
        }
        const std::size_t available = at < file.size() ? file.size() - at : 0;
        Payload payload;
        payload.offset = std::min(at, file.size());
        payload.size = std::min<std::size_t>(e.size, available);
        p = at + e.size;
        std::string safe = text::replace_all(text::replace_all(e.name, "/", "_"), "\\", "_");
        while (!safe.empty() && safe.front() == '\0') safe.erase(safe.begin());
        while (!safe.empty() && safe.back() == '\0') safe.pop_back();
        payload.name = safe.empty() ? std::format("entry{}", out.found.size()) : safe;
        out.found.push_back(std::move(payload));
    }
    out.end = p;
    return out;
}

}  // namespace cb::formats::xbox::pck
