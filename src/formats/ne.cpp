// formats/ne.cpp -- the NE reader (ne.hpp).
#include "formats/ne.hpp"

#include <algorithm>
#include <format>
#include <unordered_set>

#include "core/fs.hpp"
#include "core/io.hpp"
#include "formats/format_builtin.hpp"
#include "formats/read.hpp"

namespace cb::formats::ne {

namespace eng = cb::formats::engine;

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

namespace {

// The module read through ne.fmt: the struct `Ne` at the header's offset, and the tables of one part ("names",
// "resources", "relocs") when a reader asks for it. A module embedded in a larger file, or of a boot image, tells the
// description where its segments' data starts ($base) and where its overlay does ($overlay).
eng::Tree read_ne(ByteView data, std::uint64_t at, std::uint64_t base, std::optional<std::uint64_t> overlay, std::string_view part = {}) {
    eng::ReadOptions options;
    options.root = "Ne";
    options.at = at;
    options.keep_source = false;   // only read from
    options.vars["base"] = static_cast<std::int64_t>(base);
    options.vars["overlay"] = static_cast<std::int64_t>(overlay.value_or(0));
    if (!part.empty()) options.parts.insert(std::string(part));
    return eng::builtin().read("ne", data, options);
}

// A structure at any offset of the file, as its own root (a name table).
eng::Tree read_ne_struct(ByteView data, std::string_view root, std::uint64_t at) {
    eng::ReadOptions options;
    options.root = std::string(root);
    options.at = at;
    options.keep_source = false;
    return eng::builtin().read("ne", data, options);
}

std::string name_of(const eng::Node& entry, std::string_view child) {
    const eng::Node* n = entry.child(child);
    return n ? text::from_latin1(as_bytes(n->text)) : std::string();
}

}  // namespace

Module Module::parse(Bytes data, std::uint64_t ne_offset) {
    Module m;
    m.data_ = std::move(data);
    m.ne_offset_ = ne_offset;
    const eng::Tree tree = read_ne(m.data_, ne_offset, 0, std::nullopt);
    const eng::Node& n = tree.root;
    Header& h = m.header_;
    h.linker_version = static_cast<std::uint8_t>(n.u("linker_version"));
    h.linker_revision = static_cast<std::uint8_t>(n.u("linker_revision"));
    h.entry_table_offset = static_cast<std::uint16_t>(n.u("entry_table_offset"));
    h.entry_table_length = static_cast<std::uint16_t>(n.u("entry_table_length"));
    h.crc = static_cast<std::uint32_t>(n.u("crc"));
    h.flags = static_cast<std::uint16_t>(n.u("flags"));
    h.autodata_segment = static_cast<std::uint16_t>(n.u("autodata_segment"));
    h.heap_size = static_cast<std::uint16_t>(n.u("heap_size"));
    h.stack_size = static_cast<std::uint16_t>(n.u("stack_size"));
    h.cs_ip = static_cast<std::uint32_t>(n.u("cs_ip"));
    h.ss_sp = static_cast<std::uint32_t>(n.u("ss_sp"));
    h.segment_count = static_cast<std::uint16_t>(n.u("segment_count"));
    h.module_ref_count = static_cast<std::uint16_t>(n.u("module_ref_count"));
    h.nonresident_size = static_cast<std::uint16_t>(n.u("nonresident_size"));
    h.segment_table = static_cast<std::uint16_t>(n.u("segment_table_offset"));
    h.resource_table = static_cast<std::uint16_t>(n.u("resource_table_offset"));
    h.resident_names_table = static_cast<std::uint16_t>(n.u("resident_names_offset"));
    h.module_ref_table = static_cast<std::uint16_t>(n.u("module_ref_offset"));
    h.imported_names_table = static_cast<std::uint16_t>(n.u("imported_names_offset"));
    h.nonresident_offset = static_cast<std::uint32_t>(n.u("nonresident_offset"));
    h.movable_entries = static_cast<std::uint16_t>(n.u("movable_entries"));
    h.alignment_shift = static_cast<std::uint16_t>(n.u("shift"));   // a stored 0 means 9
    h.resource_segments = static_cast<std::uint16_t>(n.u("resource_segments"));
    h.target_os = static_cast<std::uint8_t>(n.u("target_os"));
    if (h.segment_table >= 0x40) {   // the field is there when the segment table leaves room for it
        const eng::Node& v = n.get("expected_windows_version");
        if (v.kind != eng::Kind::Int) throw TruncatedError(kReader, ne_offset + 0x3E, 2, 0);
        h.expected_windows_version = static_cast<std::uint16_t>(v.value);
    }
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
    if (p >= data_.size()) return out;
    const eng::Tree tree = read_ne_struct(data_, "NameTable", p);
    for (const eng::Node& e : tree.root.get("names").children)
        if (e.u("length") != 0) out.push_back({name_of(e, "name"), static_cast<std::uint16_t>(e.u("ordinal"))});
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
    const eng::Tree tree = read_ne(data_, ne_offset_, base_, overlay_at_, "names");
    const eng::Node* refs = tree.root.find("module_refs");
    const std::size_t have = refs ? refs->children.size() : 0;
    if (have < header_.module_ref_count) throw TruncatedError(kReader, table + 2 * std::uint64_t{have}, 2, 0);
    for (std::uint32_t i = 0; i < header_.module_ref_count; ++i) {
        const eng::Node& ref = refs->children[i];
        if (const eng::Node* name = ref.child("name")) names.push_back(name_of(*name, "text"));
        else names.push_back(pascal_string(data_, imported + ref.u("offset")));   // (a name cut short by the end of the file: what is there)
    }
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
    const eng::Tree tree = read_ne(data_, ne_offset_, base_, overlay_at_);
    const eng::Node* table = tree.root.find("segments");
    const std::size_t have = table ? table->children.size() : 0;
    if (have < header_.segment_count)
        throw TruncatedError(kReader, ne_offset_ + header_.segment_table + 8 * std::uint64_t{have}, 8, 0);
    for (std::uint32_t i = 0; i < header_.segment_count; ++i) {
        const eng::Node& n = table->children[i];
        if (n.u("sector")) (void)shifted(n.u("sector"));   // a damaged alignment shift
        Segment s;
        s.index = static_cast<std::uint16_t>(i + 1);
        s.file_offset = n.u("file_offset");
        s.length = static_cast<std::uint32_t>(n.u("byte_length"));
        s.flags = static_cast<std::uint16_t>(n.u("flags"));
        s.min_alloc = n.u("min_alloc") ? static_cast<std::uint32_t>(n.u("min_alloc")) : 0x10000u;
        segs.push_back(s);
    }
    return segs;
}

std::vector<Relocation> Module::relocations(const Segment& seg) const {
    std::vector<Relocation> recs;
    if (!(seg.flags & kRelocInfo)) return recs;
    // A relocated segment base does not relocate its reloc table, so the count can be garbage: then there is
    // nothing to read rather than an error.
    const std::uint64_t at = seg.file_offset + seg.length;
    if (at > data_.size()) return recs;
    eng::Tree tree;
    try {
        tree = read_ne_struct(data_, "SegmentRelocs", at);
    } catch (const FormatError&) {
        return recs;
    }
    for (const eng::Node& n : tree.root.get("records").children) {
        Relocation r;
        const std::uint8_t flags = static_cast<std::uint8_t>(n.u("flags"));
        const std::uint16_t b4 = static_cast<std::uint16_t>(n.u("b4")), b6 = static_cast<std::uint16_t>(n.u("b6"));
        switch (flags & 3) {
            case 0: r.target = InternalRef{static_cast<std::uint8_t>(b4 & 0xFF), b6}; break;
            case 1: r.target = ImportOrdinalRef{b4, b6}; break;
            case 2: r.target = ImportNameRef{b4, b6}; break;
            default: r.target = OsFixupRef{b4}; break;
        }
        r.offset = static_cast<std::uint16_t>(n.u("offset"));
        r.source = static_cast<std::uint8_t>(n.u("source") & 0x0F);
        r.flags = flags;
        recs.push_back(r);
    }
    return recs;
}

std::vector<Entry> Module::entries() const {
    std::vector<Entry> out;
    const eng::Tree tree = read_ne(data_, ne_offset_, base_, overlay_at_);
    const eng::Node* table = tree.root.find("entry_table");
    if (!table) return out;
    std::uint32_t ordinal = 1;
    for (const eng::Node& bundle : table->children) {
        const std::uint64_t count = bundle.u("count");
        if (count == 0) break;
        const std::uint64_t kind = bundle.u("kind");
        if (kind == 0) {  // a run of unused ordinals
            for (std::uint64_t k = 0; k < count; ++k) out.push_back({ordinal++, Entry::Kind::Unused, 0, 0, 0});
        } else if (kind == 0xFF) {  // moveable: flags, INT 3F, segment, offset
            for (const eng::Node& e : bundle.get("moveable").children)
                out.push_back({ordinal++, Entry::Kind::Moveable, static_cast<std::uint8_t>(e.u("segment")), static_cast<std::uint8_t>(e.u("flags")),
                               static_cast<std::uint16_t>(e.u("offset"))});
        } else {  // fixed, in segment `kind`
            for (const eng::Node& e : bundle.get("fixed").children)
                out.push_back({ordinal++, Entry::Kind::Fixed, static_cast<std::uint8_t>(kind), static_cast<std::uint8_t>(e.u("flags")),
                               static_cast<std::uint16_t>(e.u("offset"))});
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
    const std::uint64_t table_at = ne_offset_ + header_.resource_table;
    const eng::Tree tree = read_ne(data_, ne_offset_, base_, overlay_at_, "resources");
    const eng::Node* table = tree.root.find("resources");
    if (!table) throw TruncatedError(kReader, table_at, 2, table_at < data_.size() ? data_.size() - table_at : 0);
    const std::uint16_t shift = static_cast<std::uint16_t>(table->u("shift"));
    for (const eng::Node& type : table->get("types").children) {
        const std::uint16_t type_id = static_cast<std::uint16_t>(type.u("type_id"));
        if (type_id == 0) break;
        std::string type_name;
        std::optional<std::uint16_t> type_number;
        if (type_id & 0x8000) {
            type_number = static_cast<std::uint16_t>(type_id & 0x7fff);
            const char* n = resource_type_name(*type_number);
            type_name = n ? n : "#" + std::to_string(*type_number);
        } else if (const eng::Node* name = type.child("name")) {
            type_name = name_of(*name, "text");
        } else {
            type_name = resource_string(table_at, type_id);
        }
        for (const eng::Node& item : type.get("items").children) {
            Resource r;
            r.type = type_name;
            r.type_number = type_number;
            r.file_offset = shift_left(item.u("offset"), shift);
            r.length = shift_left(item.u("length"), shift);
            r.flags = static_cast<std::uint16_t>(item.u("flags"));
            const std::uint16_t rid = static_cast<std::uint16_t>(item.u("id"));
            if (rid & 0x8000) {
                r.id_number = static_cast<std::uint16_t>(rid & 0x7fff);
                r.id = "#" + std::to_string(*r.id_number);
            } else if (const eng::Node* name = item.child("name")) {
                r.id = name_of(*name, "text");
            } else {
                r.id = resource_string(table_at, rid);
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
    // the resident names, then (when there is one) the resource table and the names after its groups
    const eng::Tree names_tree = read_ne(d, o, base_, overlay_at_, "names");
    if (const eng::Node* names = names_tree.root.find("resident_names")) end = std::max(end, names->offset + names->size);
    if (h.resource_table != h.resident_names_table) {
        const eng::Tree res_tree = read_ne(d, o, base_, overlay_at_, "resources");
        if (const eng::Node* table = res_tree.root.find("resources"))
            end = std::max(end, table->offset + table->size);
    }
    const std::uint64_t imported = o + h.imported_names_table;  // imported names that are referred to
    std::vector<std::uint64_t> refs;
    if (const eng::Node* mod_refs = names_tree.root.find("module_refs"))
        for (const eng::Node& r : mod_refs->children) refs.push_back(r.u("offset"));
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
        const eng::Tree tree = read_ne(d, o, base_, overlay_at_, "resources");
        const eng::Node& table = tree.root.get("resources");
        const unsigned rshift = static_cast<unsigned>(table.u("shift"));
        std::size_t next = 0;
        for (const eng::Node& type : table.get("types").children) {
            if (type.u("type_id") == 0) break;
            for (const eng::Node& item : type.get("items").children) {   // the record's first field is the resource's place
                if (next >= res.size()) throw FormatError(kReader, item.offset, "resource table is inconsistent");
                const Resource& r = res[next++];
                out.patch_u16(0x40 + (item.offset - o), place(out, clamped(d, r.file_offset, r.length), rshift));
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
