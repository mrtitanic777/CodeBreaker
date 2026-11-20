#include "formats/xbox/pck.hpp"

#include <algorithm>
#include <format>

#include "core/error.hpp"
#include "core/io.hpp"
#include "core/text.hpp"

namespace cb::formats::xbox::pck {

namespace {

constexpr std::string_view kReader = "pck";
constexpr std::uint32_t kMaxNameLength = 8192;   // in UTF-16 units
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
    io::ByteReader r(file, kReader);
    Archive a;
    a.version = r.u32_be();
    const std::uint32_t property_count = r.u32_be();
    for (std::uint32_t i = 0; i < property_count; ++i) {
        const std::uint32_t id = r.u32_be(), length = r.u32_be();
        if (length > kMaxNameLength) throw FormatError(kReader, r.pos(), std::format("bad name length {}", length));
        const std::string name = utf16be(r.bytes(std::uint64_t{length} * 2));
        r.skip(4);
        a.properties.emplace_back(id, name);
    }
    const std::uint32_t count = r.u32_be();
    for (std::uint32_t i = 0; i < count; ++i) {
        Entry e;
        e.size = r.u32_be();
        e.type = r.u32_be();
        const std::uint32_t length = r.u32_be();
        e.name = utf16be(r.bytes(std::uint64_t{length} * 2));
        r.skip(4);
        a.entries.push_back(std::move(e));
    }
    a.table_end = r.pos();
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
