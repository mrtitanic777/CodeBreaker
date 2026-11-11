#include "cb/build/stubjoin.hpp"

#include <algorithm>
#include <format>
#include <vector>

#include "cb/build/fields.hpp"
#include "cb/core/io.hpp"

namespace cb::build::stubjoin {

namespace {

constexpr std::string_view kReader = "stubjoin";
constexpr std::uint32_t kStubAlign = 16;
constexpr std::uint64_t kNewHeaderField = 0x3C;   // e_lfanew

std::uint32_t u32(ByteView d, std::uint64_t at) { return u32_at(d, at, kReader); }
std::uint16_t u16(ByteView d, std::uint64_t at) { return u16_at(d, at, kReader); }

// Offsets here are differences of file offsets: `delta` and the `old + delta` they produce may be negative
// before put_u32_at()/put_u16_at() reject them, so they are signed.
using Offset = std::int64_t;

// A file offset field of `headers` (at `field`, relative to the file) moved by `delta`; zero means "none".
void move_u32(Bytes& headers, std::uint64_t field, std::uint64_t lfa, Offset delta) {
    const std::uint32_t v = u32(headers, field - lfa);
    if (v != 0) put_u32_at(headers, field - lfa, static_cast<Offset>(v) + delta, kReader);
}

Bytes join_pe(ByteView b, std::uint64_t lfa, std::uint32_t at) {
    constexpr std::uint64_t kSectionHeader = 40;
    constexpr std::uint64_t kDebugEntry = 28;
    constexpr std::uint16_t kPe32Plus = 0x20B;
    const std::uint64_t opt = lfa + 24;
    const std::uint16_t nsec = u16(b, lfa + 6);
    const std::uint16_t optsize = u16(b, lfa + 20);
    const std::uint32_t falign = u32(b, opt + 36);
    const std::uint32_t old_soh = u32(b, opt + 60);   // SizeOfHeaders
    const bool plus = u16(b, opt) == kPe32Plus;
    const std::uint64_t dirs = opt + (plus ? 112 : 96);
    const std::uint64_t sec = opt + optsize;

    const ByteView head_view = io::checked_range(b, lfa, sec + kSectionHeader * nsec - lfa);
    Bytes headers(head_view.begin(), head_view.end());
    const std::uint64_t new_soh = align_up(at + headers.size(), falign, kReader);
    const Offset delta = static_cast<Offset>(new_soh) - static_cast<Offset>(old_soh);

    put_u32_at(headers, opt + 60 - lfa, static_cast<Offset>(new_soh), kReader);
    move_u32(headers, lfa + 12, lfa, delta);   // PointerToSymbolTable
    struct Section {
        std::uint32_t rva, raw_size, raw_offset;
    };
    std::vector<Section> sections;
    for (std::uint64_t i = 0; i < nsec; ++i) {
        const std::uint64_t s = sec + kSectionHeader * i;
        sections.push_back({u32(b, s + 12), u32(b, s + 16), u32(b, s + 20)});
        for (const std::uint64_t field : {20, 24, 28}) move_u32(headers, s + field, lfa, delta);   // raw data, relocations, line numbers
    }
    // the certificates (data directory 4) are a file offset; the debug directory's entries hold one
    move_u32(headers, dirs + 4 * 8, lfa, delta);

    Bytes body(b.begin() + std::min<std::uint64_t>(old_soh, b.size()), b.end());
    const std::uint32_t debug_rva = u32(b, dirs + 6 * 8);
    const std::uint32_t debug_size = u32(b, dirs + 6 * 8 + 4);
    if (debug_rva != 0) {
        for (const Section& s : sections) {
            if (debug_rva < s.rva || debug_rva >= std::uint64_t{s.rva} + s.raw_size) continue;
            const std::uint64_t first = std::uint64_t{s.raw_offset} + debug_rva - s.rva - old_soh;   // in the body
            for (std::uint32_t k = 0; k < debug_size / kDebugEntry; ++k) {
                const std::uint64_t e = first + kDebugEntry * k + 24;   // PointerToRawData
                const std::uint32_t v = u32(body, e);
                if (v != 0) put_u32_at(body, e, static_cast<Offset>(v) + delta, kReader);
            }
            break;   // one section holds the directory (a PE whose sections overlap in RVA would be adjusted twice by the port)
        }
    }
    Bytes out = std::move(headers);
    out.resize(new_soh - at, 0);
    append(out, body);
    return out;
}

Bytes join_ne(ByteView b, std::uint64_t lfa, std::uint32_t at) {
    constexpr std::uint64_t kSegmentEntry = 8, kResourceEntry = 12;
    // an NE alignment shift count of 0 means 512-byte sectors; more than 16 is no module of this era
    unsigned shift = u16(b, lfa + 0x32);
    if (shift == 0) shift = 9;
    if (shift > 16) throw UnsupportedError(kReader, lfa + 0x32, std::format("an alignment shift of {}", shift));
    const std::uint16_t nseg = u16(b, lfa + 0x1C);
    const std::uint16_t segtab = u16(b, lfa + 0x22);
    const std::uint16_t restab = u16(b, lfa + 0x24);
    const std::uint16_t resname = u16(b, lfa + 0x26);
    // where the data starts: the first segment or resource (sector offsets from the file start)
    std::vector<std::uint16_t> sectors;
    for (std::uint64_t i = 0; i < nseg; ++i) sectors.push_back(u16(b, lfa + segtab + kSegmentEntry * i));
    std::vector<std::uint64_t> res;   // file offsets of the resource sector fields
    if (restab != resname) {
        std::uint64_t r = lfa + restab;
        unsigned rshift = u16(b, r);
        if (rshift == 0) rshift = 9;
        r += 2;
        while (u16(b, r) != 0) {
            const std::uint16_t count = u16(b, r + 2);
            for (std::uint64_t k = 0; k < count; ++k) res.push_back(r + 8 + kResourceEntry * k);
            r += 8 + kResourceEntry * count;
        }
        if (rshift != shift) throw UnsupportedError(kReader, restab + lfa, "resources aligned differently from segments");
    }
    const std::uint64_t unit = std::uint64_t{1} << shift;
    std::uint64_t first_sector = 0;
    bool any = false;
    auto note = [&](std::uint16_t sector) {
        if (sector == 0) return;
        first_sector = any ? std::min<std::uint64_t>(first_sector, sector) : sector;
        any = true;
    };
    for (const std::uint16_t s : sectors) note(s);
    for (const std::uint64_t o : res) note(u16(b, o));
    const std::uint64_t data_from = any ? first_sector * unit : b.size();
    const std::uint32_t nonres = u32(b, lfa + 0x2C);

    // the header and its tables -- and the non-resident names when they sit before the data
    const ByteView head_view = io::checked_range(b, lfa, data_from - lfa);
    Bytes header(head_view.begin(), head_view.end());
    const std::uint64_t new_data = align_up(at + header.size(), unit, kReader);
    const Offset delta = static_cast<Offset>(new_data - data_from) / static_cast<Offset>(unit);   // in sectors, exact
    for (std::size_t i = 0; i < sectors.size(); ++i) {
        if (sectors[i] != 0) put_u16_at(header, segtab + kSegmentEntry * i, Offset{sectors[i]} + delta, kReader);
    }
    for (const std::uint64_t o : res) {
        const std::uint16_t v = u16(b, o);
        if (v != 0) put_u16_at(header, o - lfa, Offset{v} + delta, kReader);
    }
    if (nonres != 0) {
        const Offset moved = nonres < data_from ? static_cast<Offset>(at) - static_cast<Offset>(lfa)
                                                : delta * static_cast<Offset>(unit);
        put_u32_at(header, 0x2C, Offset{nonres} + moved, kReader);
    }
    Bytes out = std::move(header);
    out.resize(new_data - at, 0);
    const ByteView data = b.subspan(std::min<std::uint64_t>(data_from, b.size()));
    append(out, data);
    return out;
}

}  // namespace

Bytes join(ByteView stub, ByteView built, std::optional<std::uint32_t> at) {
    const std::uint32_t lfa = u32(built, kNewHeaderField);
    std::uint64_t place = (at && *at != 0 && *at >= stub.size()) ? *at : align_up(stub.size(), kStubAlign, kReader);
    if (place < kNewHeaderField + 4) throw FormatError(kReader, 0, "the DOS program has no room for its new-header field");
    if (place > 0xFFFFFFFF) throw FormatError(kReader, place, "the module's header would start past 4 GB");
    const auto place32 = static_cast<std::uint32_t>(place);

    const ByteView sig = io::checked_range(built, lfa, 2);
    Bytes tail;
    if (sig[0] == 'P' && sig[1] == 'E') {
        tail = join_pe(built, lfa, place32);
    } else if (sig[0] == 'N' && sig[1] == 'E') {
        tail = join_ne(built, lfa, place32);
    } else {
        throw UnsupportedError(kReader, lfa, "not a PE or NE module");
    }
    Bytes out(stub.begin(), stub.end());
    out.resize(place, 0);
    put_u32_at(out, kNewHeaderField, static_cast<Offset>(place), kReader);
    append(out, tail);
    return out;
}

}  // namespace cb::build::stubjoin
