#include "cb/build/mzfile.hpp"

#include <algorithm>
#include <format>

#include "cb/build/fields.hpp"
#include "cb/core/io.hpp"

namespace cb::build::mzfile {

namespace {

constexpr std::string_view kReader = "mzfile";

// The header fields the build decides (offset of each word).
constexpr std::uint64_t kLastPage = 0x02, kPages = 0x04, kRelocCount = 0x06, kMinAlloc = 0x0A, kSs = 0x0E, kSp = 0x10,
                        kIp = 0x14, kCs = 0x16, kRelocOffset = 0x18;
constexpr std::uint64_t kFixedPart = 0x1A;
constexpr std::size_t kPage = 512;

}  // namespace

Bytes make_template(ByteView header) {
    if (header.size() < kFixedPart) throw TruncatedError(kReader, 0, kFixedPart, header.size());
    Bytes out(header.begin(), header.end());
    const std::uint16_t count = u16_at(out, kRelocCount, kReader);
    const std::uint16_t table = u16_at(out, kRelocOffset, kReader);
    for (const std::uint64_t at : {kLastPage, kPages, kRelocCount, kMinAlloc, kSs, kSp, kIp, kCs}) {
        out[at] = out[at + 1] = 0;
    }
    const ByteView room = io::checked_range(out, table, 4u * count);   // FormatError if the table lies
    std::fill_n(out.begin() + static_cast<std::ptrdiff_t>(table), room.size(), std::uint8_t{0});
    return out;
}

Bytes compose(ByteView tpl, ByteView module, std::span<const Relocation> relocs, const StartState& start,
              std::uint16_t min_alloc, bool full_last) {
    if (tpl.size() < kFixedPart) throw TruncatedError(kReader, 0, kFixedPart, tpl.size());
    Bytes file(tpl.begin(), tpl.end());
    const std::uint64_t table = u16_at(file, kRelocOffset, kReader);
    const std::uint64_t wanted = 4ull * relocs.size();
    const auto room = io::try_range(file, table, wanted);
    if (!room || std::any_of(room->begin(), room->end(), [](std::uint8_t b) { return b != 0; })) {
        throw FormatError(kReader, table, std::format("{} relocations do not fit in the header's room for them", relocs.size()));
    }
    std::uint64_t at = table;
    for (const Relocation& r : relocs) {
        put_u16_at(file, at, r.offset, kReader);
        put_u16_at(file, at + 2, r.segment, kReader);
        at += 4;
    }
    const std::uint64_t total = file.size() + module.size();
    // a last page that is exactly full: 0, or 512 as some linkers wrote it
    std::uint64_t last = total % kPage;
    if (last == 0 && full_last) last = kPage;
    put_u16_at(file, kLastPage, static_cast<std::int64_t>(last), kReader);
    put_u16_at(file, kPages, static_cast<std::int64_t>((total + kPage - 1) / kPage), kReader);
    put_u16_at(file, kRelocCount, static_cast<std::int64_t>(relocs.size()), kReader);
    put_u16_at(file, kMinAlloc, min_alloc, kReader);
    put_u16_at(file, kSs, start.ss, kReader);
    put_u16_at(file, kSp, start.sp, kReader);
    put_u16_at(file, kIp, start.ip, kReader);
    put_u16_at(file, kCs, start.cs, kReader);
    append(file, module);
    return file;
}

}  // namespace cb::build::mzfile
