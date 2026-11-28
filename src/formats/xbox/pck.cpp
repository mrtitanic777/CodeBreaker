#include "formats/xbox/pck.hpp"

#include <algorithm>
#include <format>

#include "core/error.hpp"
#include "core/text.hpp"
#include "formats/gen/pck.hpp"

namespace cb::formats::xbox::pck {

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

// The layout is pck.fmt's (its generated reader, formats/gen/pck).
Archive parse(ByteView file) {
    const gen::pck::File h = gen::pck::read(file);
    Archive a;
    a.version = h.version;
    a.properties.reserve(h.properties.size());
    for (const gen::pck::Property& p : h.properties) a.properties.emplace_back(p.id, utf16be(p.name));
    a.entries.reserve(h.entries.size());
    for (const gen::pck::PckEntry& n : h.entries) a.entries.push_back({n.size, n.type, utf16be(n.name)});
    a.table_end = static_cast<std::size_t>(h.table_end);
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
