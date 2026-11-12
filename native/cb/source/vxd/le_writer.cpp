// cb-unity: alone (file-local helpers named for this module)
#include "cb/source/vxd/le_writer.hpp"

#include <algorithm>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"

namespace cb::source::vxd::le_writer {

namespace le = formats::le;

namespace {

constexpr const char* kReader = "LE writer";
constexpr std::uint32_t kHeaderAt = 0x80;      // where build_le puts the header, behind the DOS stub
constexpr std::uint32_t kPageAlignment = 0x200;
constexpr std::size_t kMaxSources = 255;       // the source count of a record is one byte

std::uint32_t u32_at(ByteView data, std::uint64_t at) {
    if (at + 4 > data.size()) throw TruncatedError(kReader, at, 4, at < data.size() ? data.size() - at : 0);
    return static_cast<std::uint32_t>(data[at] | (data[at + 1] << 8) | (data[at + 2] << 16) | (static_cast<std::uint32_t>(data[at + 3]) << 24));
}

// data[from:to] with the clamping of a slice: a range past the end is what is there.
ByteView clamped(ByteView data, std::uint64_t from, std::uint64_t to) {
    from = std::min<std::uint64_t>(from, data.size());
    to = std::min<std::uint64_t>(std::max(to, from), data.size());
    return data.subspan(from, to - from);
}

Bytes bytes_of(ByteView v) { return Bytes(v.begin(), v.end()); }

void put_u32(Bytes& out, std::size_t at, std::uint32_t v) {
    for (std::size_t k = 0; k < 4; ++k) out[at + k] = static_cast<std::uint8_t>(v >> (8 * k));
}

bool is_jcc(std::optional<std::uint8_t> b) { return b && *b >= 0x80 && *b <= 0x8F; }

const le::Object* object_numbered(const le::Module& driver, std::uint32_t index) {
    for (const le::Object& o : driver.objects()) {
        if (o.index == index) return &o;
    }
    return nullptr;
}

std::vector<le::ObjectEntry> object_entries(const le::Module& driver, std::uint64_t object_table_at) {
    std::vector<le::ObjectEntry> out;
    std::size_t i = 0;
    for (const le::Object& o : driver.objects()) {
        le::ObjectEntry e;
        e.vsize = o.vsize;
        e.base = o.base;
        e.flags = o.flags;
        e.first_page = o.first_page;
        e.page_count = o.page_count;
        std::array<std::uint8_t, 4> name{};   // the 4-character object name, recovered
        const ByteView stored = clamped(driver.data(), object_table_at + i * 24 + 20, object_table_at + i * 24 + 24);
        std::copy(stored.begin(), stored.end(), name.begin());
        e.reserved = name;
        out.push_back(e);
        ++i;
    }
    return out;
}

}  // namespace

std::uint8_t fixup_source_type(const masm::Emission& code, std::uint32_t object, std::int64_t offset, const std::set<std::int64_t>& ends,
                               ByteView object_bytes, std::optional<std::uint8_t> told) {
    if (told) return *told;
    const auto byte_at = [&](std::int64_t at) -> std::optional<std::uint8_t> {
        return at >= 0 && at < static_cast<std::int64_t>(object_bytes.size()) ? std::optional<std::uint8_t>(object_bytes[static_cast<std::size_t>(at)]) : std::nullopt;
    };
    const auto b1 = byte_at(offset - 1), b2 = byte_at(offset - 2);
    const Instruction* r1 = code.instruction_at({object, offset - 1});
    if (r1 != nullptr && r1->size == 5 && (b1 == 0xE8 || b1 == 0xE9)) return 0x08;
    const Instruction* r2 = code.instruction_at({object, offset - 2});
    if (r2 != nullptr && r2->size == 6 && b2 == 0x0F && is_jcc(b1)) return 0x08;
    if (!ends.contains(offset + 4)) {   // not a decoded instruction's tail: trust the opcode
        if (b1 == 0xE8 || b1 == 0xE9) return 0x08;
        if (b2 == 0x0F && is_jcc(b1)) return 0x08;
    }
    return 0x07;
}

OraclePages oracle_pages(const le::Module& driver, std::optional<std::uint32_t> first) {
    const ByteView data = driver.data();
    const std::uint32_t start = first ? *first : u32_at(data, driver.lfa() + 0x80);
    const std::uint32_t size = driver.header().page_size;
    OraclePages out;
    for (const le::PageEntry& e : driver.page_map()) {
        const std::uint64_t at = std::uint64_t{start} + std::uint64_t{e.number - 1} * size;
        out.pages.emplace_back(e.number, bytes_of(clamped(data, at, at + size)));
        out.top = std::max(out.top, e.number);
    }
    return out;
}

std::map<std::uint32_t, std::vector<Bytes>> emit_fixups(const masm::Emission& code, const le::Module& driver) {
    const ByteView data = driver.data();
    const le::Header& header = driver.header();
    const std::uint32_t page_size = header.page_size, page_count = header.pages;
    std::map<std::uint32_t, std::set<std::int64_t>> ends;   // per object: where its instructions end
    for (const auto& [at, insn] : code.instructions()) ends[at.object].insert(at.offset + insn.size);

    // The object bytes from its first page to the end of the module's data pages: that covers a fix-up whose offset overruns the object's own
    // pages into a later object. The end comes from the page layout (container-absolute), NOT from the size field: bundling in a W3 container
    // leaves that module-relative, which would make this empty for a bundled module and break every source-type read.
    const std::uint64_t data_pages = u32_at(data, driver.lfa() + 0x80);
    const std::uint64_t data_end = std::min<std::uint64_t>(data_pages + std::uint64_t{page_count - 1} * page_size + header.last_page_bytes, data.size());
    const OraclePages found = oracle_pages(driver);
    std::vector<Bytes> logical;
    for (const auto& [number, bytes] : found.pages) {
        if (number != found.top) {
            logical.push_back(bytes);
        } else {
            const std::int64_t keep = static_cast<std::int64_t>(data_end) - static_cast<std::int64_t>(data_pages + std::uint64_t{number - 1} * page_size);
            logical.push_back(Bytes(bytes.begin(), bytes.begin() + static_cast<std::ptrdiff_t>(std::clamp<std::int64_t>(keep, 0, static_cast<std::int64_t>(bytes.size())))));
        }
    }
    std::map<std::uint32_t, Bytes> code_of;
    for (const le::Object& o : driver.objects()) {
        Bytes joined;
        for (std::size_t k = o.first_page - 1; k < logical.size(); ++k) append(joined, logical[k]);
        code_of[o.index] = std::move(joined);
    }

    // a fix-up unit: what it points at, and the places on the page that hold it
    struct Key {
        std::uint8_t source_type, target_flags;
        bool internal;
        std::uint32_t first, second;   // object and offset, or module and ordinal
        bool operator==(const Key&) const = default;
    };
    struct Group {
        Key key;
        std::vector<std::int64_t> offsets;
    };
    std::map<std::uint32_t, std::vector<Group>> by_page;   // groups in the order first seen
    const auto add = [&](std::uint32_t page, const Key& key, std::int64_t offset) {
        std::vector<Group>& groups = by_page[page];
        auto group = std::find_if(groups.begin(), groups.end(), [&](const Group& g) { return g.key == key; });
        if (group == groups.end()) group = groups.insert(groups.end(), Group{key, {}});
        group->offsets.push_back(offset);
    };
    for (const auto& [at, fixup] : code.fixups()) {
        const auto* internal = std::get_if<le::InternalFixup>(&fixup);
        if (internal != nullptr && !internal->target) continue;   // a selector-only fix-up: no 32-bit field
        const le::Object* object = object_numbered(driver, at.object);
        if (object == nullptr || at.offset < 0) continue;
        const std::uint32_t page = object->first_page + static_cast<std::uint32_t>(at.offset / page_size);
        const std::int64_t in_page = at.offset % page_size;
        const auto bytes = code_of.find(at.object);
        const std::set<std::int64_t> none;
        const auto end_set = ends.find(at.object);
        const std::uint8_t source_type = fixup_source_type(code, at.object, at.offset, end_set != ends.end() ? end_set->second : none,
                                                           bytes != code_of.end() ? ByteView(bytes->second) : ByteView(),
                                                           internal != nullptr ? std::optional<std::uint8_t>(internal->source_type) : std::nullopt);
        Key key{};
        if (internal != nullptr) {
            key = {source_type, static_cast<std::uint8_t>(*internal->target > 0xFFFF ? 0x10 : 0x00), true, internal->object, *internal->target};
        } else {   // an import by ordinal: module, then the 8-bit ordinal
            const auto& imported = std::get<le::ImportFixup>(fixup);
            key = {source_type, static_cast<std::uint8_t>(0x01 | 0x80), false, imported.module, imported.ordinal & 0xFF};
        }
        add(page, key, in_page);
        if (in_page > static_cast<std::int64_t>(page_size) - 4 && page < page_count) add(page + 1, key, in_page - page_size);   // a source that crosses into the next page is listed there too
    }

    std::map<std::uint32_t, std::vector<Bytes>> records;
    for (std::uint32_t page = 1; page <= page_count; ++page) {
        std::vector<Bytes>& out = records[page];
        std::vector<Group>& groups = by_page[page];
        std::stable_sort(groups.begin(), groups.end(), [](const Group& a, const Group& b) {
            return *std::min_element(a.offsets.begin(), a.offsets.end()) < *std::min_element(b.offsets.begin(), b.offsets.end());
        });
        for (Group& g : groups) {
            std::sort(g.offsets.begin(), g.offsets.end());
            for (std::size_t i = 0; i < g.offsets.size(); i += kMaxSources) {
                const std::size_t n = std::min(kMaxSources, g.offsets.size() - i);
                le::FixupRecord record;
                record.source = g.key.source_type;
                record.source_flags = n > 1 ? 0x20 : 0;
                record.flags = g.key.target_flags;
                for (std::size_t k = 0; k < n; ++k) record.sources.push_back(static_cast<std::int16_t>(g.offsets[i + k]));
                if (g.key.internal) {   // the target: the object, then a 16 or 32-bit offset
                    record.kind = le::FixupRecord::Kind::Internal;
                    record.first = g.key.first;
                    record.target = g.key.second;
                } else {   // an import: the module, then the 8-bit ordinal
                    record.kind = le::FixupRecord::Kind::Import;
                    record.first = g.key.first;
                    record.ordinal = g.key.second;
                }
                out.push_back(le::encode_fixup(record));
            }
        }
    }
    return records;
}

Bytes build_le(const masm::Emission& code, const le::Module& driver, std::string_view name) {
    const ByteView data = driver.data();
    const le::Header& header = driver.header();
    const std::uint32_t page_size = header.page_size;
    const std::uint64_t lfa = kHeaderAt;
    // A header field of the original, read where the writer assumes the header is (0x80), NOT where the original keeps it.
    const auto at = [&](std::uint32_t offset) { return u32_at(data, lfa + offset); };
    const std::uint32_t data_pages = at(0x80);

    // Data pages: each recovered object's pages come from the recovery's code, laid out by logical page index. A logical page no object maps (a
    // gap) is not recovered code: LINK386 left stale buffer bytes there, copied from the original (the loader ignores them; they cannot be
    // derived). The last page holds only last_page_bytes (any tail is BSS, not stored). The original's own pages are those at each page's NUMBER in
    // its page map (LINK386 numbered some drivers' pages out of order); write_parts stores each page at its number again.
    std::map<std::uint32_t, Bytes> page_content;
    for (const le::Object& o : driver.objects()) {
        const ByteView object_code = code.code(o.index);
        for (std::uint32_t k = 0; k < o.page_count; ++k) page_content[o.first_page + k] = bytes_of(clamped(object_code, std::uint64_t{k} * page_size, std::uint64_t{k + 1} * page_size));
    }
    const OraclePages found = oracle_pages(driver, data_pages);
    std::vector<le::StoredPage> pages;
    std::uint32_t page_index = 0;
    std::uint64_t stored = 0;
    for (const auto& [number, own] : found.pages) {
        ++page_index;
        const auto content = page_content.find(page_index);
        Bytes b = content != page_content.end() ? content->second : own;   // a gap page: the linker's orphaned bytes
        if (b.size() < page_size && number != found.top && own.size() > b.size()) append(b, ByteView(own).subspan(b.size()));   // the object ends mid-page: the rest is the original's
        if (number == found.top) {
            b.resize(std::min<std::size_t>(b.size(), header.last_page_bytes));
        } else if (b.size() < page_size) {
            b.resize(page_size, 0);
        }
        stored += b.size();
        pages.push_back({0, std::move(b)});
    }
    // trailing bytes between the last object page and the non-resident name table (LINK386's zero padding or stale bytes): taken from the original so
    // the whole data region is byte exact
    const std::uint64_t names_end = at(0x88) ? at(0x88) : (at(0xB8) ? at(0xB8) : data.size());
    const Bytes names_gap = stored < names_end - std::min<std::uint64_t>(names_end, data_pages) ? bytes_of(clamped(data, data_pages + stored, names_end)) : Bytes();

    const std::uint64_t object_table = driver.lfa() + u32_at(data, driver.lfa() + 0x40);
    // the page map: taken verbatim from the original (each 4-byte entry's page number and flag byte are LINK386's; the numbers are a permutation for
    // some modules, and write_parts stores each page and its fix-up row at its number)
    const std::uint64_t page_map_at = driver.lfa() + u32_at(data, driver.lfa() + 0x48);
    // the resident-name table (the module's own name and any exported names): taken verbatim so it is byte exact whatever file name is given
    const std::uint32_t resident_at = u32_at(data, driver.lfa() + 0x58), entries_at = u32_at(data, driver.lfa() + 0x5C);
    le::NameTable resident = std::vector<le::NameOrdinal>{{std::string(name), 0}};
    if (entries_at > resident_at && resident_at > 0) resident = bytes_of(clamped(data, driver.lfa() + resident_at, driver.lfa() + entries_at));
    // the entry table (the export directory: DDB @1 and any further exports): taken verbatim -- the DDB's object and offset it points at are
    // independently verified by the byte-exact object bytes
    const std::uint32_t fixup_pages_at = u32_at(data, driver.lfa() + 0x68);
    Bytes entry_table;
    if (fixup_pages_at > entries_at && entries_at > 0) {
        entry_table = bytes_of(clamped(data, driver.lfa() + entries_at, driver.lfa() + fixup_pages_at));
    } else {   // one exported 32-bit entry: the DDB, flags 0x03
        const std::uint32_t ddb_object = code.ddb() ? code.ddb()->object : 1;
        const std::uint32_t ddb_rva = code.ddb() ? static_cast<std::uint32_t>(code.ddb()->rva) : 0;
        io::ByteWriter w;
        w.u8(1);
        w.u8(0x03);
        w.u16(static_cast<std::uint16_t>(ddb_object));
        w.u8(0x03);
        w.u32(ddb_rva);
        w.u8(0);
        entry_table = w.take();
    }
    // The loader section's tail after the fix-up records: the import module and procedure name tables (always empty in this corpus) and whatever dead
    // symbol strings LINK386 left in the gap up to the data pages, taken from the original as the tables' tail (the loader ignores it; it cannot be
    // derived), zero-padded up to the data pages. So the import tables are empty and the fix-up section's size counts the fix-up tables alone.
    const std::uint64_t import_modules_at = lfa + at(0x70);
    const Bytes tables_tail = data_pages > import_modules_at ? bytes_of(clamped(data, import_modules_at, data_pages)) : Bytes();

    Bytes head(le::kHeaderSize, 0);
    head[0] = 'L';
    head[1] = 'E';
    const auto put16 = [&](std::size_t where, std::uint16_t v) { head[where] = static_cast<std::uint8_t>(v); head[where + 1] = static_cast<std::uint8_t>(v >> 8); };
    put16(0x08, header.cpu);
    put16(0x0A, header.os);
    put_u32(head, 0x18, header.eip_object);
    put_u32(head, 0x1C, header.eip);
    put_u32(head, 0x28, page_size);
    // the fields the real Win95 loader reads, preserved from the original: the module's version and flags, the initial ESP, the preload count and
    // the VxD device id with the DDK version packed at 0xC0
    for (const std::uint32_t field : {0x0Cu, 0x10u, 0x20u, 0x24u, 0x84u, 0x94u, 0xC0u}) put_u32(head, field, at(field));
    // the non-resident name table (the module's description string), preserved and placed after the data pages
    const std::uint32_t nres_at = at(0x88), nres_size = at(0x8C);
    Bytes nonresident;
    if (nres_at != 0 && nres_size != 0 && std::uint64_t{nres_at} + nres_size <= data.size()) nonresident = bytes_of(clamped(data, nres_at, std::uint64_t{nres_at} + nres_size));
    if (!nonresident.empty()) {
        bool plausible = 0 < nonresident[0] && nonresident[0] < 64;   // a W3 module's container-relative offset is no description
        for (std::size_t i = 1; plausible && i < 1 + static_cast<std::size_t>(nonresident[0]) && i < nonresident.size(); ++i) {
            plausible = (nonresident[i] > 31 && nonresident[i] < 127) || nonresident[i] == 9;
        }
        if (!plausible) nonresident.clear();
    }
    // the Windows resource block (the VxD's version resource), recovered and re-pointed: 0xB8 and 0xBC locate it, at the end of the file just past the
    // non-resident table
    const std::uint32_t resource_at = at(0xB8), resource_size = at(0xBC);
    Bytes resources;
    if (resource_at != 0 && resource_size != 0 && std::uint64_t{resource_at} + resource_size <= data.size()) resources = bytes_of(clamped(data, resource_at, std::uint64_t{resource_at} + resource_size));
    if (!resources.empty()) put_u32(head, 0xBC, static_cast<std::uint32_t>(resources.size()));

    Bytes stub(lfa, 0);   // the DOS stub, preserved from the original
    if (data.size() >= lfa && data[0] == 'M' && data[1] == 'Z') std::copy(data.begin(), data.begin() + static_cast<std::ptrdiff_t>(lfa), stub.begin());
    stub[0] = 'M';
    stub[1] = 'Z';
    put_u32(stub, 0x3C, static_cast<std::uint32_t>(lfa));

    // The data pages start at the original's recorded offset (LINK386's own alignment varies with the page size), at a 0x200 alignment only if our
    // loader content would not fit under it; a file that comes out shorter than the original ends with the original's own end-of-file padding.
    le::Parts parts;
    parts.stub = std::move(stub);
    parts.header = std::move(head);
    parts.objects = object_entries(driver, object_table);
    parts.pages = std::move(pages);
    parts.page_map = bytes_of(clamped(data, page_map_at, page_map_at + std::uint64_t{header.pages} * 4));
    parts.last_page_bytes = header.last_page_bytes;
    parts.resident = std::move(resident);
    parts.nonresident = std::move(nonresident);
    parts.entry_table = std::move(entry_table);
    for (auto& [page, records] : emit_fixups(code, driver)) parts.fixups.insert_or_assign(page, std::move(records));
    parts.tables_tail = tables_tail;
    parts.align = kPageAlignment;
    parts.data_pages = data_pages;
    parts.gaps = std::make_pair(Bytes(), names_gap);
    parts.after = resources;
    parts.moved = {0, resources.empty() ? 0 : resource_at, resource_at};
    parts.fill_from = bytes_of(data);
    return le::write_parts(parts);
}

Bytes build_w3(const masm::Emission& code, const le::Module& driver) {
    const ByteView data = driver.data();
    const le::Header& header = driver.header();
    const std::uint64_t lfa = driver.lfa();
    const auto field = [&](std::uint32_t offset) { return u32_at(data, lfa + offset); };
    const std::uint32_t page_size = header.page_size;
    const std::uint32_t first = field(0x80);   // the data pages, container-absolute
    // The data pages: the container's own in page-map order (each found by its number), each recovered object's bytes laid over them at its first
    // page, in object order (an object whose recovered bytes run past its pages, its uninitialised tail, spills into the next page and the next
    // object is laid over that); write_parts stores each page at its number again.
    const OraclePages found = oracle_pages(driver);
    Bytes laid;
    for (const auto& [number, bytes] : found.pages) {
        Bytes page = bytes;
        page.resize(std::max<std::size_t>(page.size(), page_size), 0);
        append(laid, page);
    }
    for (const le::Object& o : driver.objects()) {
        const std::size_t at = static_cast<std::size_t>(o.first_page - 1) * page_size;
        const ByteView object_code = code.code(o.index);
        if (laid.size() < at + object_code.size()) laid.resize(at + object_code.size(), 0);
        std::copy(object_code.begin(), object_code.end(), laid.begin() + static_cast<std::ptrdiff_t>(at));
    }
    std::vector<le::StoredPage> pages;
    std::size_t index = 0;
    for (const auto& [number, bytes] : found.pages) {
        const ByteView page = clamped(laid, index * page_size, (index + 1) * std::size_t{page_size});
        pages.push_back({0, bytes_of(page.first(std::min<std::size_t>(page.size(), number == found.top ? header.last_page_bytes : page_size)))});
        ++index;
    }
    const std::uint64_t object_table = lfa + field(0x40);
    // the loader's tables and the names as the container stores them, but the fix-up tables rebuilt from the recovery; the container's own bytes behind
    // them up to the data pages are kept where they lie (the data pages move to the next 0x200 only when the rebuilt tables run into them). The
    // non-resident names and the version resource sit among the data pages and move with them.
    const std::map<std::uint32_t, std::vector<Bytes>> fixups = emit_fixups(code, driver);
    const auto [fix_pages, fix_records] = le::fixup_tables(fixups, header.pages);
    const std::uint64_t tables_end = lfa + field(0x68) + fix_pages.size() + fix_records.size();
    const std::uint32_t nres_at = field(0x88), nres_size = field(0x8C);
    le::Parts parts;
    parts.stub = bytes_of(clamped(data, 0, lfa));
    parts.header = bytes_of(clamped(data, lfa, lfa + le::kHeaderSize));
    parts.objects = object_entries(driver, object_table);
    parts.pages = std::move(pages);
    parts.page_map = bytes_of(clamped(data, lfa + field(0x48), lfa + field(0x48) + std::uint64_t{header.pages} * 4));
    parts.last_page_bytes = header.last_page_bytes;
    parts.resident = bytes_of(clamped(data, lfa + field(0x58), lfa + field(0x5C)));
    parts.entry_table = bytes_of(clamped(data, lfa + field(0x5C), lfa + field(0x68)));
    parts.nonresident = bytes_of(clamped(data, nres_at, std::uint64_t{nres_at} + nres_size));
    for (const auto& [page, records] : fixups) parts.fixups.insert_or_assign(page, records);
    parts.tables_tail = bytes_of(clamped(data, tables_end, first));
    parts.align = kPageAlignment;
    parts.data_pages = first;
    parts.gaps = std::make_pair(Bytes(), Bytes());
    OrderedMap<std::string, std::int64_t> in_pages;
    in_pages["nonresident_names"] = nres_at;
    in_pages["resource_data"] = field(0xB8);
    parts.in_pages = in_pages;
    parts.moved = {field(0x98), 0, static_cast<std::uint32_t>(data.size())};
    parts.fill_from = bytes_of(data);
    Bytes out = le::write_parts(parts);

    // the container's directory, each member's size being its header and loader tables as written
    std::optional<le::Container> container = le::container(data);
    if (!container) throw FormatError(kReader, lfa, "the module is not a member of a container");
    for (le::ContainerMember& member : container->members) {
        if (member.offset == lfa) member.size = u32_at(out, member.offset + 0x68) + u32_at(out, member.offset + 0x30);
    }
    const Bytes directory = le::pack_directory(*container);
    if (container->at + directory.size() > out.size()) throw FormatError(kReader, container->at, "the container's directory lies past the rebuilt file");
    std::copy(directory.begin(), directory.end(), out.begin() + static_cast<std::ptrdiff_t>(container->at));
    return out;
}

}  // namespace cb::source::vxd::le_writer
