#include "cb/build/vxdfile.hpp"

#include <format>
#include <vector>

#include "cb/build/fields.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"

namespace cb::build::vxdfile {

namespace {

constexpr std::string_view kReader = "vxdfile";

// Fields of the LE header, from its start.
constexpr std::uint64_t kPagesOffset = 0x80, kNonResidentOffset = 0x88, kNonResidentLength = 0x8C, kResourceOffset = 0xB8,
                        kResourceLength = 0xBC;
constexpr std::uint64_t kDosHeaderLfa = 0x3C;

}  // namespace

std::string definitions(ByteView original, std::uint32_t lfa) {
    const std::uint32_t at = u32_at(original, std::uint64_t{lfa} + kNonResidentOffset, kReader);
    const std::uint32_t n = u32_at(original, std::uint64_t{lfa} + kNonResidentLength, kReader);
    const ByteView table = io::checked_range(original, at, n);

    // the non-resident name table: length byte, name, ordinal word; a zero length ends it
    struct Entry {
        std::string name;
        std::uint16_t ordinal;
    };
    std::vector<Entry> names;
    io::ByteReader r(table, "vxdfile.non-resident names");
    while (!r.eof()) {
        const std::uint8_t length = r.u8();
        if (length == 0) break;
        const std::string name = text::from_latin1(r.bytes(length));
        names.push_back({name, r.u16()});
    }

    std::vector<std::string> lines;
    for (const Entry& e : names) {
        if (e.ordinal == 0) lines.push_back(std::format("DESCRIPTION '{}'", text::replace_all(e.name, "'", "''")));
    }
    bool any_export = false;
    for (const Entry& e : names) {
        if (e.ordinal == 0) continue;
        if (!any_export) lines.push_back("EXPORTS");
        any_export = true;
        lines.push_back(std::format("    {} @{}", e.name, e.ordinal));
    }
    return text::join(lines, "\n") + "\n";
}

Bytes relay(ByteView built, std::uint32_t at, ByteView resource) {
    const std::uint32_t lfa = u32_at(built, kDosHeaderLfa, kReader);
    const std::uint32_t pages_from = u32_at(built, std::uint64_t{lfa} + kPagesOffset, kReader);   // file offsets in the built file
    const std::uint32_t nonres_at = u32_at(built, std::uint64_t{lfa} + kNonResidentOffset, kReader);
    const std::uint32_t nonres_len = u32_at(built, std::uint64_t{lfa} + kNonResidentLength, kReader);
    if (pages_from < lfa) throw FormatError(kReader, lfa, "the pages start before the LE header");
    if (nonres_at != 0 && nonres_at < pages_from) throw FormatError(kReader, nonres_at, "the name table is before the pages");

    const ByteView tables = io::checked_range(built, lfa, pages_from - lfa);   // the LE header and its tables
    Bytes head(tables.begin(), tables.end());
    const ByteView pages = nonres_at != 0 ? io::checked_range(built, pages_from, nonres_at - pages_from)
                                          : io::checked_range(built, pages_from, built.size() - pages_from);
    const ByteView names = nonres_at != 0 ? io::checked_range(built, nonres_at, nonres_len) : ByteView{};

    // pages begin on the next 512-byte boundary of the file
    const std::uint64_t start = align_up(std::uint64_t{at} + head.size(), kPageAlign, kReader);
    head.resize(start - at, 0);
    const auto after_pages = static_cast<std::int64_t>(start + pages.size());
    put_u32_at(head, kPagesOffset, static_cast<std::int64_t>(start), kReader);
    if (!names.empty()) put_u32_at(head, kNonResidentOffset, after_pages, kReader);
    if (!resource.empty()) {
        put_u32_at(head, kResourceOffset, after_pages + static_cast<std::int64_t>(names.size()), kReader);
        put_u32_at(head, kResourceLength, static_cast<std::int64_t>(resource.size()), kReader);
    }
    append(head, pages);
    append(head, names);
    append(head, resource);
    return head;
}

}  // namespace cb::build::vxdfile
