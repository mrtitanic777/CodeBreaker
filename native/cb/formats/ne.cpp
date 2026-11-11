// cb/formats/ne.cpp -- the NE reader (ne.hpp).
#include "cb/formats/ne.hpp"

#include <algorithm>
#include <format>
#include <unordered_set>

#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/formats/read.hpp"

namespace cb::formats::ne {

namespace {

constexpr std::string_view kReader = "ne";

std::uint16_t u16(ByteView d, std::uint64_t off) { return get<std::uint16_t>(d, off, kReader); }
std::uint32_t u32(ByteView d, std::uint64_t off) { return get<std::uint32_t>(d, off, kReader); }
std::uint8_t u8(ByteView d, std::uint64_t off) { return get<std::uint8_t>(d, off, kReader); }

// value << shift for a stored offset counted in alignment units; FormatError when the result would not fit 63 bits
// (a damaged shift).
std::uint64_t shift_left(std::uint64_t value, std::uint64_t shift) {
    if (value == 0) return 0;
    if (shift >= 63 || value > (std::uint64_t{INT64_MAX} >> shift)) throw FormatError(kReader, 0, "alignment shift overflows");
    return value << shift;
}

// A length-prefixed string at `off` (a byte count, then the bytes), latin1 decoded; strict.
std::string pascal_string(ByteView d, std::uint64_t off) {
    const std::uint8_t len = u8(d, off);
    return text::from_latin1(clamped(d, off + 1, len));
}

}  // namespace

const char* resource_type_name(std::uint16_t id) {
    switch (id) {
        case 1: return "CURSOR";
        case 2: return "BITMAP";
        case 3: return "ICON";
        case 4: return "MENU";
        case 5: return "DIALOG";
        case 6: return "STRING";
        case 7: return "FONTDIR";
        case 8: return "FONT";
        case 9: return "ACCELERATOR";
        case 10: return "RCDATA";
        case 11: return "MESSAGETABLE";
        case 12: return "GROUP_CURSOR";
        case 14: return "GROUP_ICON";
        case 16: return "VERSION";
        default: return nullptr;
    }
}

// ---- opening ---------------------------------------------------------------------------------------------

Module Module::parse(Bytes data, std::uint64_t ne_offset) {
    Module m;
    m.data_ = std::move(data);
    m.ne_offset_ = ne_offset;
    const ByteView d = m.data_;
    const std::uint64_t o = ne_offset;
    if (!starts_with(d, o, "NE")) throw FormatError(kReader, o, "no NE signature");
    Header& h = m.header_;
    h.linker_version = u8(d, o + 0x02);
    h.linker_revision = u8(d, o + 0x03);
    h.entry_table_offset = u16(d, o + 0x04);
    h.entry_table_length = u16(d, o + 0x06);
    h.crc = u32(d, o + 0x08);
    h.flags = u16(d, o + 0x0C);
    h.autodata_segment = u16(d, o + 0x0E);
    h.heap_size = u16(d, o + 0x10);
    h.stack_size = u16(d, o + 0x12);
    h.cs_ip = u32(d, o + 0x14);
    h.ss_sp = u32(d, o + 0x18);
    h.segment_count = u16(d, o + 0x1C);
    h.module_ref_count = u16(d, o + 0x1E);
    h.nonresident_size = u16(d, o + 0x20);
    h.segment_table = u16(d, o + 0x22);
    h.resource_table = u16(d, o + 0x24);
    h.resident_names_table = u16(d, o + 0x26);
    h.module_ref_table = u16(d, o + 0x28);
    h.imported_names_table = u16(d, o + 0x2A);
    h.nonresident_offset = u32(d, o + 0x2C);
    h.movable_entries = u16(d, o + 0x30);
    h.alignment_shift = u16(d, o + 0x32);
    if (!h.alignment_shift) h.alignment_shift = 9;
    h.resource_segments = u16(d, o + 0x34);
    h.target_os = u8(d, o + 0x36);
    if (h.segment_table >= 0x40) h.expected_windows_version = u16(d, o + 0x3E);
    return m;
}

Module Module::open(Bytes data) {
    if (!starts_with(data, 0, "MZ")) throw FormatError(kReader, 0, "not an MZ file");
    const std::uint64_t off = u32(data, 0x3C);  // e_lfanew is 4 bytes: a stub of 64 KB or more needs all of them
    return parse(std::move(data), off);
}

Module Module::open_file(const std::filesystem::path& file) { return open(fs::read_file(file)); }

Module Module::parse_boot_module(Bytes data, std::uint64_t ne_offset, std::uint64_t overlay_at) {
    Module m = parse(std::move(data), ne_offset);
    m.overlay_at_ = overlay_at;
    return m;
}

std::optional<Module> Module::try_open(ByteView data) {
    try {
        return open(Bytes(data.begin(), data.end()));
    } catch (const FormatError&) {  // probing: not an NE file, or its header is cut short
        return std::nullopt;
    }
}

// value << alignment shift (a stored offset counted in alignment units)
std::uint64_t Module::shifted(std::uint64_t value) const { return shift_left(value, header_.alignment_shift); }

// ---- names ---------------------------------------------------------------------------------------------

std::vector<Name> Module::read_name_table(std::uint64_t p) const {
    std::vector<Name> out;
    const ByteView d = data_;
    while (p < d.size()) {
        const std::uint8_t len = d[p];
        ++p;
        if (len == 0) break;
        Name n;
        n.name = text::from_latin1(slice(d, p, len, kReader));
        p += len;
        n.ordinal = u16(d, p);
        p += 2;
        out.push_back(std::move(n));
    }
    return out;
}

std::vector<Name> Module::resident_names() const {
    return read_name_table(ne_offset_ + header_.resident_names_table);
}

std::vector<Name> Module::nonresident_names() const {
    if (overlay_at_) {  // in the overlay, at the header's offset counted in alignment units
        if (!header_.nonresident_offset || *overlay_at_ >= data_.size()) return {};
        return read_name_table(*overlay_at_ + shifted(header_.nonresident_offset));
    }
    if (!header_.nonresident_offset) return {};
    return read_name_table(header_.nonresident_offset);
}

std::string Module::module_name() const {
    auto names = resident_names();
    return names.empty() ? "SELF" : names.front().name;
}

std::vector<std::string> Module::module_refs() const {
    std::vector<std::string> names;
    const std::uint64_t table = ne_offset_ + header_.module_ref_table;
    const std::uint64_t imported = ne_offset_ + header_.imported_names_table;
    for (std::uint32_t i = 0; i < header_.module_ref_count; ++i)
        names.push_back(pascal_string(data_, imported + u16(data_, table + std::uint64_t{i} * 2)));
    return names;
}

std::string Module::imported_name(std::uint64_t offset) const {
    const std::uint64_t at = ne_offset_ + header_.imported_names_table + offset;
    if (at >= data_.size()) return "?";  // out-of-range import-name offset
    return pascal_string(data_, at);
}

// ---- segments, relocations, entries, resources -----------------------------------------------------------

std::vector<Segment> Module::segments() const {
    std::vector<Segment> segs;
    const std::uint64_t table = ne_offset_ + header_.segment_table;
    for (std::uint32_t i = 0; i < header_.segment_count; ++i) {
        const std::uint64_t o = table + std::uint64_t{i} * 8;
        const std::uint16_t sector = u16(data_, o), length = u16(data_, o + 2), flags = u16(data_, o + 4),
                            min_alloc = u16(data_, o + 6);
        Segment s;
        s.index = static_cast<std::uint16_t>(i + 1);
        s.file_offset = shifted(sector) + (sector ? base_ : 0);
        s.length = length ? length : (sector ? 0x10000u : 0u);
        s.flags = flags;
        s.min_alloc = min_alloc ? min_alloc : 0x10000u;
        if (overlay_at_ && s.file_offset && !(flags & kPreload)) s.file_offset += *overlay_at_;
        segs.push_back(s);
    }
    return segs;
}

std::vector<Relocation> Module::relocations(const Segment& seg) const {
    std::vector<Relocation> recs;
    if (!(seg.flags & kRelocInfo)) return recs;
    const std::uint64_t end = seg.file_offset + seg.length;
    // A relocated segment base does not relocate its reloc table, so the count can be garbage: then there is
    // nothing to read rather than an error.
    auto count = try_get<std::uint16_t>(data_, end);
    if (!count) return recs;
    std::uint64_t p = end + 2;
    if (p + 8 * std::uint64_t{*count} > data_.size()) return recs;
    for (std::uint32_t k = 0; k < *count; ++k, p += 8) {
        Relocation r;
        const std::uint8_t src_type = u8(data_, p), flags = u8(data_, p + 1);
        r.offset = u16(data_, p + 2);
        const std::uint16_t b4 = u16(data_, p + 4), b6 = u16(data_, p + 6);
        switch (flags & 3) {
            case 0: r.target = InternalRef{u8(data_, p + 4), b6}; break;
            case 1: r.target = ImportOrdinalRef{b4, b6}; break;
            case 2: r.target = ImportNameRef{b4, b6}; break;
            default: r.target = OsFixupRef{b4}; break;
        }
        r.source = src_type & 0x0f;
        r.flags = flags;
        recs.push_back(r);
    }
    return recs;
}

std::vector<Entry> Module::entries() const {
    std::vector<Entry> out;
    std::uint64_t p = ne_offset_ + header_.entry_table_offset;
    const std::uint64_t end = p + header_.entry_table_length;
    std::uint32_t ordinal = 1;
    while (p < end) {
        const std::uint8_t count = u8(data_, p), seg_indicator = u8(data_, p + 1);
        p += 2;
        if (count == 0) break;
        if (seg_indicator == 0x00) {  // a run of unused ordinals
            for (int k = 0; k < count; ++k) out.push_back({ordinal++, Entry::Kind::Unused, 0, 0, 0});
        } else if (seg_indicator == 0xFF) {  // moveable: flags, INT 3F, segment, offset
            for (int k = 0; k < count; ++k, p += 6)
                out.push_back({ordinal++, Entry::Kind::Moveable, u8(data_, p + 3), u8(data_, p), u16(data_, p + 4)});
        } else {  // fixed, in segment seg_indicator
            for (int k = 0; k < count; ++k, p += 3)
                out.push_back({ordinal++, Entry::Kind::Fixed, seg_indicator, u8(data_, p), u16(data_, p + 1)});
        }
    }
    return out;
}

std::vector<std::uint32_t> Module::export_ordinals() const {
    std::vector<std::uint32_t> out;
    for (const Entry& e : entries())
        if (e.kind != Entry::Kind::Unused) out.push_back(e.ordinal);
    return out;
}

std::string Module::resource_string(std::uint64_t table, std::uint64_t offset) const {
    return pascal_string(data_, table + offset);
}

std::vector<Resource> Module::resources() const {
    std::vector<Resource> out;
    if (header_.resource_table == header_.resident_names_table) return out;  // no resource table
    const std::uint64_t table = ne_offset_ + header_.resource_table;
    std::uint64_t p = table;
    const std::uint16_t shift = u16(data_, p);
    p += 2;
    for (;;) {
        const std::uint16_t type_id = u16(data_, p);
        if (type_id == 0) break;
        const std::uint16_t count = u16(data_, p + 2);
        p += 8;
        std::string type_name;
        std::optional<std::uint16_t> type_number;
        if (type_id & 0x8000) {
            type_number = static_cast<std::uint16_t>(type_id & 0x7fff);
            const char* n = resource_type_name(*type_number);
            type_name = n ? n : "#" + std::to_string(*type_number);
        } else {
            type_name = resource_string(table, type_id);
        }
        for (int k = 0; k < count; ++k, p += 12) {
            Resource r;
            r.type = type_name;
            r.type_number = type_number;
            r.file_offset = shift_left(u16(data_, p), shift);
            r.length = shift_left(u16(data_, p + 2), shift);
            r.flags = u16(data_, p + 4);
            const std::uint16_t rid = u16(data_, p + 6);
            if (rid & 0x8000) {
                r.id_number = static_cast<std::uint16_t>(rid & 0x7fff);
                r.id = "#" + std::to_string(*r.id_number);
            } else {
                r.id = resource_string(table, rid);
            }
            if (overlay_at_ && !(r.flags & kPreload)) r.file_offset += *overlay_at_;
            out.push_back(std::move(r));
        }
    }
    return out;
}

// ---- modules inside larger files -----------------------------------------------------------------------

std::vector<std::pair<std::uint64_t, std::string>> embedded(ByteView data, std::uint64_t start) {
    std::vector<std::pair<std::uint64_t, std::string>> out;
    auto is_alpha = [](std::uint8_t c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); };
    auto is_digit = [](std::uint8_t c) { return c >= '0' && c <= '9'; };
    auto at = find(data, "NE", start);
    while (at && *at + 0x40 <= data.size()) {
        const std::uint64_t i = *at;
        const std::uint16_t nseg = u16(data, i + 0x1C), segtab = u16(data, i + 0x22), resnm = u16(data, i + 0x26);
        if (0 < nseg && nseg < 255 && 0x40 <= segtab && segtab < 0x100 && segtab <= resnm && resnm < 0x1000 &&
            i + resnm < data.size()) {
            const std::uint8_t len = data[i + resnm];
            const ByteView name = clamped(data, i + resnm + 1, len);
            const bool plausible =
                0 < len && len <= 8 && !name.empty() && is_alpha(name[0]) &&
                std::all_of(name.begin(), name.end(), [&](std::uint8_t c) { return (is_alpha(c) || is_digit(c)) && !(c >= 'a' && c <= 'z'); });
            if (plausible) out.emplace_back(i, text::from_latin1(name));
        }
        at = find(data, "NE", i + 2);
    }
    return out;
}

std::vector<std::pair<std::string, Module>> open_boot_image(ByteView resident, ByteView overlay) {
    std::vector<std::pair<std::string, Module>> out;
    Bytes both(resident.begin(), resident.end());
    append(both, overlay);
    std::unordered_set<std::string> seen;  // membership only
    for (const auto& [offset, name] : embedded(resident)) {
        if (!seen.insert(name).second) continue;
        out.emplace_back(name, Module::parse_boot_module(both, offset, resident.size()));
    }
    return out;
}

std::vector<std::pair<std::string, Module>> open_boot_image_files(const std::filesystem::path& resident,
                                                                  const std::optional<std::filesystem::path>& overlay) {
    std::optional<std::filesystem::path> ovl = overlay;
    if (!ovl) {
        const std::string stem = text::lower(resident.stem().string());
        std::error_code ec;
        for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::absolute(resident).parent_path(), ec)) {
            if (text::lower(entry.path().filename().string()) == stem + ".ovl") {
                ovl = entry.path();
                break;
            }
        }
    }
    const Bytes res = fs::read_file(resident);
    const Bytes extra = ovl ? fs::read_file(*ovl) : Bytes();
    return open_boot_image(res, extra);
}

// ---- tables_end and standalone ---------------------------------------------------------------------------

std::uint64_t Module::tables_end() const {
    const Header& h = header_;
    const ByteView d = data_;
    const std::uint64_t o = ne_offset_;
    std::uint64_t end = std::max({o + 0x40, o + h.segment_table + 8 * std::uint64_t{h.segment_count},
                                  o + h.entry_table_offset + std::uint64_t{h.entry_table_length},
                                  o + h.module_ref_table + 2 * std::uint64_t{h.module_ref_count}});
    std::uint64_t p = o + h.resident_names_table;  // resident names
    while (p < d.size() && d[p]) p += d[p] + 3u;
    end = std::max(end, p + 1);
    if (h.resource_table != h.resident_names_table) {  // the resource table, then its names
        p = o + h.resource_table + 2;
        while (u16(d, p)) p += 8 + 12 * std::uint64_t{u16(d, p + 2)};
        p += 2;
        while (p < d.size() && d[p]) p += d[p] + 1u;
        end = std::max(end, p + 1);
    }
    const std::uint64_t imported = o + h.imported_names_table;  // imported names that are referred to
    std::vector<std::uint64_t> refs;
    for (std::uint32_t i = 0; i < h.module_ref_count; ++i) refs.push_back(u16(d, o + h.module_ref_table + 2 * std::uint64_t{i}));
    for (const Segment& s : segments())
        for (const Relocation& r : relocations(s))
            if (const auto* by_name = std::get_if<ImportNameRef>(&r.target)) refs.push_back(by_name->name_offset);
    for (std::uint64_t off : refs)
        if (imported + off < d.size()) end = std::max(end, imported + off + 1 + d[imported + off]);
    return end;
}

namespace {

// The blob appended at the next multiple of 1 << shift; where it went, in those units (must fit 16 bits).
std::uint16_t place(io::ByteWriter& out, ByteView blob, unsigned shift) {
    if (shift > 31) throw FormatError(kReader, out.size(), "alignment shift is too large to lay the image out");
    out.align(std::size_t{1} << shift);
    const std::uint64_t at = out.size() >> shift;
    if (at > 0xFFFF) throw FormatError(kReader, out.size(), "standalone image is too large for 16-bit sector numbers");
    out.bytes(blob);
    return static_cast<std::uint16_t>(at);
}

}  // namespace

Bytes Module::standalone() const {
    const Header& h = header_;
    const ByteView d = data_;
    const std::uint64_t o = ne_offset_;
    const unsigned shift = h.alignment_shift;
    io::ByteWriter out;
    out.zeros(0x40);
    out.patch_u16(0, 0x5A4D);  // "MZ"
    out.patch_u16(0x18, 0x40);
    out.patch_u32(0x3C, 0x40);
    out.bytes(clamped(d, o, tables_end() - o));
    const auto segs = segments();
    for (std::size_t i = 0; i < segs.size(); ++i) {
        const Segment& s = segs[i];
        if (!s.file_offset) continue;
        std::uint64_t end = s.file_offset + s.length;
        if (auto count = (s.flags & kRelocInfo) ? try_get<std::uint16_t>(d, end) : std::nullopt) end += 2 + 8 * std::uint64_t{*count};
        const ByteView blob = clamped(d, s.file_offset, end - s.file_offset);
        out.patch_u16(0x40 + h.segment_table + 8 * i, place(out, blob, shift));
    }
    if (h.resource_table != h.resident_names_table) {
        const auto res = resources();
        std::size_t next = 0;
        std::uint64_t p = o + h.resource_table;
        const unsigned rshift = u16(d, p);
        p += 2;
        while (u16(d, p)) {
            const std::uint32_t count = u16(d, p + 2);
            p += 8;
            for (std::uint32_t k = 0; k < count; ++k, p += 12) {
                if (next >= res.size()) throw FormatError(kReader, p, "resource table is inconsistent");
                const Resource& r = res[next++];
                out.patch_u16(0x40 + (p - o), place(out, clamped(d, r.file_offset, r.length), rshift));
            }
        }
    }
    const auto names = nonresident_names();
    io::ByteWriter table;
    for (const Name& n : names) {
        const Bytes raw = text::to_latin1(n.name);
        table.u8(static_cast<std::uint8_t>(raw.size()));
        table.bytes(raw);
        table.u16(n.ordinal);
    }
    table.u8(0);
    out.patch_u32(0x40 + 0x2C, names.empty() ? 0 : static_cast<std::uint32_t>(out.size()));
    out.patch_u16(0x40 + 0x20, names.empty() ? 0 : static_cast<std::uint16_t>(table.size()));
    if (!names.empty()) out.bytes(table.data());
    return out.take();
}

}  // namespace cb::formats::ne
