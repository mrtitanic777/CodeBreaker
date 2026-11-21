// formats/le.cpp -- the LE reader: header, objects, pages, names, entries, fix-ups, the device descriptor (le.hpp).
#include "formats/le.hpp"

#include <algorithm>
#include <unordered_map>

#include "core/fs.hpp"
#include "formats/format_builtin.hpp"
#include "formats/le_internal.hpp"
#include "formats/read.hpp"

namespace cb::formats::le {

using detail::kReader;
namespace eng = cb::formats::engine;

namespace {

constexpr std::uint64_t kMaxObjectBytes = std::uint64_t{1} << 30;  // an object past this is not a real driver

bool printable(ByteView b) {
    return std::all_of(b.begin(), b.end(), [](std::uint8_t c) { return c >= 32 && c < 127; });
}
bool is_alnum(std::uint8_t c) { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }

}  // namespace

namespace detail {
// A big-endian value of up to 3 bytes (what a page-map number is), tolerant of a table that is cut short.
std::uint32_t be24(ByteView d, std::uint64_t off) {
    std::uint32_t v = 0;
    for (std::uint8_t b : clamped(d, off, 3)) v = (v << 8) | b;
    return v;
}
ByteView rstrip_blanks(ByteView b) {  // bytes.rstrip(b" \x00")
    while (!b.empty() && (b.back() == ' ' || b.back() == 0)) b = b.first(b.size() - 1);
    return b;
}
}  // namespace detail

// ---- opening -----------------------------------------------------------------------------------------

namespace {

// The module read through le.fmt: the struct `Le` at the header's offset, and the tables of one part ("pages", "names",
// "entries", "fixups", "imports") when a reader asks for it.
eng::Tree read_le(ByteView data, std::uint64_t lfa, std::string_view part = {}) {
    eng::ReadOptions options;
    options.root = "Le";
    options.at = lfa;
    options.keep_source = false;   // only read from
    if (!part.empty()) options.parts.insert(std::string(part));
    return eng::builtin().read("le", data, options);
}

std::string latin1_of(const eng::Node* n) { return n ? text::from_latin1(as_bytes(n->text)) : std::string(); }

}  // namespace

Module Module::parse(std::shared_ptr<const Bytes> data, std::uint64_t lfa) {
    Module m;
    m.data_ = std::move(data);
    m.lfa_ = lfa;
    // The header and the object table, strict; the fields from the fix-up size on are 0 where the header is cut short
    // (le.fmt marks them `try`), and the page map is read tolerantly.
    const eng::Tree tree = read_le(*m.data_, lfa, "pages");
    const eng::Node& n = tree.root;
    m.signature_ = latin1_of(n.child("signature"));
    Header& h = m.header_;
    h.byte_order = static_cast<std::uint8_t>(n.u("byte_order"));
    h.word_order = static_cast<std::uint8_t>(n.u("word_order"));
    h.level = static_cast<std::uint32_t>(n.u("level"));
    h.cpu = static_cast<std::uint16_t>(n.u("cpu"));
    h.os = static_cast<std::uint16_t>(n.u("os"));
    h.version = static_cast<std::uint32_t>(n.u("version"));
    h.module_flags = static_cast<std::uint32_t>(n.u("module_flags"));
    h.pages = static_cast<std::uint32_t>(n.u("pages"));
    h.eip_object = static_cast<std::uint32_t>(n.u("eip_object"));
    h.eip = static_cast<std::uint32_t>(n.u("eip"));
    h.esp_object = static_cast<std::uint32_t>(n.u("esp_object"));
    h.esp = static_cast<std::uint32_t>(n.u("esp"));
    h.page_size = static_cast<std::uint32_t>(n.u("page_size"));
    h.last_page_bytes = static_cast<std::uint32_t>(n.u("last_page_bytes"));
    h.fixup_size = static_cast<std::uint32_t>(n.u("fixup_size"));
    h.loader_size = static_cast<std::uint32_t>(n.u("loader_size"));
    h.object_table = static_cast<std::uint32_t>(n.u("object_table"));
    h.objects = static_cast<std::uint32_t>(n.u("objects"));
    h.page_map = static_cast<std::uint32_t>(n.u("page_map"));
    h.resources = static_cast<std::uint32_t>(n.u("resources"));
    h.resident_names = static_cast<std::uint32_t>(n.u("resident_names"));
    h.entry_table = static_cast<std::uint32_t>(n.u("entry_table"));
    h.fixup_pages = static_cast<std::uint32_t>(n.u("fixup_pages"));
    h.fixup_records = static_cast<std::uint32_t>(n.u("fixup_records"));
    h.import_modules = static_cast<std::uint32_t>(n.u("import_modules"));
    h.import_module_count = static_cast<std::uint32_t>(n.u("import_module_count"));
    h.import_names = static_cast<std::uint32_t>(n.u("import_names"));
    h.data_pages = static_cast<std::uint32_t>(n.u("data_pages"));  // an offset from the top of the file
    h.preload_pages = static_cast<std::uint32_t>(n.u("preload_pages"));
    h.nonresident_names = static_cast<std::uint32_t>(n.u("nonresident_names"));
    h.nonresident_size = static_cast<std::uint32_t>(n.u("nonresident_size"));
    h.debug_info = static_cast<std::uint32_t>(n.u("debug_info"));
    h.debug_size = static_cast<std::uint32_t>(n.u("debug_size"));
    h.resource_data = static_cast<std::uint32_t>(n.u("resource_data"));
    h.resource_size = static_cast<std::uint32_t>(n.u("resource_size"));
    h.device_id = static_cast<std::uint16_t>(n.u("device_id"));
    h.kit_version = static_cast<std::uint16_t>(n.u("kit_version"));
    m.objtab_ = lfa + h.object_table;
    m.pagemap_ = lfa + h.page_map;
    m.resnames_ = lfa + h.resident_names;
    m.entrytab_ = lfa + h.entry_table;
    m.fixup_page_ = lfa + h.fixup_pages;
    m.fixup_rec_ = lfa + h.fixup_records;
    std::uint32_t index = 0;
    for (const eng::Node& e : n.get("object_entries").children) {
        Object o;
        o.index = ++index;
        o.vsize = static_cast<std::uint32_t>(e.u("vsize"));
        o.base = static_cast<std::uint32_t>(e.u("base"));
        o.flags = static_cast<std::uint32_t>(e.u("flags"));
        o.first_page = static_cast<std::uint32_t>(e.u("first_page"));
        o.page_count = static_cast<std::uint32_t>(e.u("page_count"));
        m.objects_.push_back(o);
    }
    if (const eng::Node* map = n.child("page_entries"))
        for (const eng::Node& e : map->children) m.pages_.push_back({static_cast<std::uint32_t>(e.u("number")), static_cast<std::uint8_t>(e.u("kind"))});
    m.index_pages();
    return m;
}

Module Module::from_buffer(Bytes data) {
    const std::uint64_t lfa = get<std::uint32_t>(data, 0x3c, kReader);
    if (!starts_with(data, lfa, "LE") && !starts_with(data, lfa, "LX")) throw FormatError(kReader, lfa, "not an LE/LX image");
    return parse(std::move(data), lfa);
}

Module Module::open(const std::filesystem::path& file) { return from_buffer(fs::read_file(file)); }

// The page map numbers each page of the image in order; a page's bytes and its row of the fix-up page table are
// found by that number (the first page carrying a number wins). Read once, with the header, tolerantly: the pages the
// file holds, then (when it ends early) one place with the number 0.
void Module::index_pages() {
    for (std::size_t i = 0; i < pages_.size(); ++i) page_order_.emplace(pages_[i].number, static_cast<std::uint32_t>(i + 1));
    if (pages_.size() < header_.pages) page_order_.emplace(0u, static_cast<std::uint32_t>(pages_.size() + 1));
}

bool Module::is_member() const { return get<std::uint32_t>(*data_, 0x3C, kReader) != lfa_; }

// ---- address space ----------------------------------------------------------------------------------------

std::optional<std::pair<const Object*, std::uint64_t>> Module::page_to_object(std::uint32_t page) const {
    auto it = page_order_.find(page);
    const std::uint64_t logical = it != page_order_.end() ? it->second : page;
    for (const Object& o : objects_)
        if (o.first_page <= logical && logical < std::uint64_t{o.first_page} + o.page_count)
            return std::pair{&o, (logical - o.first_page) * header_.page_size};
    return std::nullopt;
}

std::vector<PageEntry> Module::page_map() const {
    if (pages_.size() < header_.pages) throw TruncatedError(kReader, pagemap_ + 4 * std::uint64_t{pages_.size()}, 4, 0);
    return pages_;
}

// A page's bytes as stored (empty for an invalid, zero-filled or unnumbered page); the file may end early, in which
// case the page is what is there.
Bytes Module::page_body(const PageEntry& e, std::uint32_t last_number) const {
    if (e.kind == kPageInvalid || e.kind == kPageZeroFilled || e.number == 0) return {};
    const std::uint64_t off = header_.data_pages + std::uint64_t{e.number - 1} * header_.page_size;
    const std::uint64_t len = (e.number == last_number && header_.last_page_bytes) ? header_.last_page_bytes : header_.page_size;
    const ByteView b = clamped(*data_, off, len);
    return Bytes(b.begin(), b.end());
}

Bytes Module::object_data(const Object& obj) const {
    const auto pm = page_map();
    std::uint32_t last = 0;
    for (const PageEntry& e : pm) last = std::max(last, e.number);
    if (obj.page_count != 0 && obj.first_page == 0) throw FormatError(kReader, objtab_, "object starts at page 0");
    const std::uint64_t first = obj.page_count ? obj.first_page - 1 : 0;
    const std::uint64_t a = std::min<std::uint64_t>(first, pm.size());
    const std::uint64_t b = std::max(a, std::min<std::uint64_t>(first + obj.page_count, pm.size()));
    if ((b - a) * header_.page_size > kMaxObjectBytes || obj.vsize > kMaxObjectBytes)
        throw UnsupportedError(kReader, objtab_, "object is too large");
    Bytes out;
    for (std::uint64_t k = a; k < b; ++k) {
        Bytes page = page_body(pm[k], last);
        page.resize(std::max<std::size_t>(page.size(), header_.page_size), 0);  // each page is padded to the page size
        append(out, page);
    }
    out.resize(obj.vsize, 0);  // cut, or zero-extended, to the virtual size
    return out;
}

std::optional<std::uint64_t> Module::file_offset(std::uint32_t object, std::int64_t offset) const {
    if (object == 0 || object > objects_.size() || offset < 0 || header_.page_size == 0) return std::nullopt;
    const Object& obj = objects_[object - 1];
    if (obj.first_page == 0) return std::nullopt;
    const auto pm = page_map();
    const std::uint64_t i = obj.first_page - 1 + static_cast<std::uint64_t>(offset) / header_.page_size;
    if (static_cast<std::uint64_t>(offset) >= obj.vsize || i >= pm.size()) return std::nullopt;
    const PageEntry& e = pm[i];
    if (e.kind != kPagePresent || e.number == 0) return std::nullopt;
    return std::uint64_t{header_.data_pages} + std::uint64_t{e.number - 1} * header_.page_size +
           static_cast<std::uint64_t>(offset) % header_.page_size;
}

// ---- names, entries ---------------------------------------------------------------------------------------

std::string Module::resident_name() const {
    const ByteView d = *data_;
    if (resnames_ >= d.size()) throw TruncatedError(kReader, resnames_, 1, 0);
    const std::vector<NameOrdinal> names = names_at(d, resnames_);
    return names.empty() ? std::string() : names.front().name;
}

std::string Module::description() const {
    const ByteView d = *data_;
    const std::uint64_t off = header_.nonresident_names;
    if (!(0 < off && off < d.size())) return {};
    return text::from_latin1(clamped(d, off + 1, d[off]));
}

std::vector<std::string> Module::import_modules() const {
    const eng::Tree tree = read_le(*data_, lfa_, "imports");
    const eng::Node* names = tree.root.find("import_module_names");
    const std::size_t have = names ? names->children.size() : 0;
    if (have < header_.import_module_count) throw TruncatedError(kReader, lfa_ + header_.import_modules, 1, 0);
    std::vector<std::string> out;
    for (std::uint32_t k = 0; k < header_.import_module_count; ++k) out.push_back(latin1_of(names->children[k].child("text")));
    return out;
}

std::vector<NameOrdinal> names_at(ByteView d, std::uint64_t p, std::optional<std::uint64_t> end) {
    std::vector<NameOrdinal> out;
    if (!(0 < p && p < d.size()) || (end && *end == 0)) return out;
    eng::ReadOptions options;
    options.root = "NameTable";
    options.at = p;
    options.keep_source = false;
    options.vars["limit"] = static_cast<std::int64_t>(end.value_or(0));
    const eng::Tree tree = eng::builtin().read("le", d, options);
    for (const eng::Node& e : tree.root.get("names").children)
        if (e.u("length") != 0) out.push_back({latin1_of(e.child("name")), static_cast<std::uint16_t>(e.u("ordinal"))});
    return out;
}

std::pair<std::vector<NameOrdinal>, std::vector<NameOrdinal>> Module::names() const {
    std::vector<NameOrdinal> non;
    const std::uint64_t at = header_.nonresident_names, size = header_.nonresident_size;
    if (at && size && !is_member()) non = names_at(*data_, at, at + size);
    return {names_at(*data_, resnames_), std::move(non)};
}

std::vector<EntryPoint> Module::entries() const {
    std::vector<EntryPoint> out;
    const eng::Tree tree = read_le(*data_, lfa_, "entries");
    const eng::Node* table = tree.root.find("entries");
    if (!table) return out;
    std::uint32_t ordinal = 1;
    for (const eng::Node& bundle : table->children) {
        const std::uint64_t count = bundle.u("count");
        if (count == 0) break;
        const std::uint64_t kind = bundle.u("kind");
        if (kind == 0) {  // a run of unused ordinals
            ordinal += static_cast<std::uint32_t>(count);
            continue;
        }
        const std::uint32_t object = kind != 4 ? static_cast<std::uint32_t>(bundle.u("object")) : 0u;
        for (const eng::Node& e : bundle.get("entries").children)
            out.push_back({ordinal++, object, static_cast<std::uint32_t>(e.u("offset")), static_cast<std::uint8_t>(e.u("flags"))});
    }
    return out;
}

std::size_t Module::entry_table_size() const {
    const eng::Tree tree = read_le(*data_, lfa_, "entries");
    const eng::Node* table = tree.root.find("entries");
    if (!table) return 1;
    const bool ended = !table->children.empty() && table->children.back().u("count") == 0;   // the zero count that ends the table
    return table->size + (ended ? 0 : 1);
}

std::vector<Export> Module::exports() const {
    const auto [res, non] = names();
    std::unordered_map<std::uint32_t, std::string> by_ordinal;  // lookup only; a later name for an ordinal wins
    for (std::size_t k = 1; k < res.size(); ++k) by_ordinal[res[k].ordinal] = res[k].name;
    for (std::size_t k = 1; k < non.size(); ++k) by_ordinal[non[k].ordinal] = non[k].name;
    std::vector<Export> out;
    for (const EntryPoint& e : entries()) {
        Export x;
        if (auto it = by_ordinal.find(e.ordinal); it != by_ordinal.end()) x.name = it->second;
        x.ordinal = e.ordinal;
        x.object = e.object;
        x.offset = e.offset;
        out.push_back(std::move(x));
    }
    return out;
}

// ---- fix-ups ----------------------------------------------------------------------------------------------

std::vector<std::pair<Place, Fixup>> Module::fixups() const {
    std::vector<std::pair<Place, Fixup>> out;
    const eng::Tree tree = read_le(*data_, lfa_, "fixups");
    const eng::Node* offsets = tree.root.find("fixup_offsets");   // the fix-up page table
    if (!offsets || offsets->children.size() < std::size_t{header_.pages} + 1) throw TruncatedError(kReader, fixup_page_, 4 * (std::uint64_t{header_.pages} + 1), 0);
    const eng::Node* pages = tree.root.find("fixup_page_records");
    std::map<Place, std::size_t> where;
    for (std::uint32_t pg = 0; pages && pg < pages->children.size(); ++pg) {
        const auto owner = page_to_object(pg + 1);
        const eng::Node* records = pages->children[pg].child("records");
        for (std::size_t i = 0; records && i < records->children.size(); ++i) {
            const eng::Node& r = records->children[i];
            const std::uint64_t source = r.u("source"), flags = r.u("flags");
            if ((flags & 0x03) > 1) break;  // import by name / entry: the rest of this page's records is not read
            Fixup fx;
            if ((flags & 0x03) == 0) {  // an internal reference
                InternalFixup in;
                in.object = static_cast<std::uint32_t>(r.u("object"));
                in.source_type = static_cast<std::uint8_t>(source & 0x0f);
                if ((source & 0x0f) != 0x02) in.target = static_cast<std::uint32_t>(r.u("target"));  // (a 16-bit selector has no offset)
                fx = in;
            } else {  // an import by ordinal
                fx = ImportFixup{static_cast<std::uint32_t>(r.u("object")), static_cast<std::uint32_t>(r.u("ordinal"))};
            }
            std::vector<std::int16_t> places;
            if (source & 0x20) {
                if (const eng::Node* list = r.child("sources"))
                    for (const eng::Node& s : list->children) places.push_back(static_cast<std::int16_t>(s.as_signed()));
            } else {
                places.push_back(static_cast<std::int16_t>(r.i("first_source")));
            }
            if (!owner) continue;
            for (std::int16_t o : places) {
                const Place key{owner->first->index, static_cast<std::int64_t>(owner->second) + o};
                auto [it, fresh] = where.try_emplace(key, out.size());
                if (fresh) out.emplace_back(key, fx);
                else out[it->second].second = fx;
            }
        }
    }
    return out;
}

std::vector<FixupRecord> Module::fixup_records() const {
    std::vector<FixupRecord> out;
    const ByteView d = *data_;
    const eng::Tree tree = read_le(d, lfa_, "fixups");
    const eng::Node* offsets = tree.root.find("fixup_offsets");
    if (!offsets || offsets->children.size() < std::size_t{header_.pages} + 1) throw TruncatedError(kReader, fixup_page_, 4 * (std::uint64_t{header_.pages} + 1), 0);
    const eng::Node* pages = tree.root.find("fixup_page_records");
    for (std::uint32_t pg = 0; pages && pg < pages->children.size(); ++pg) {
        const eng::Node* records = pages->children[pg].child("records");
        for (std::size_t i = 0; records && i < records->children.size(); ++i) {
            const eng::Node& n = records->children[i];
            FixupRecord r;
            const std::uint64_t source = n.u("source"), flags = n.u("flags");
            r.page = pg + 1;
            r.source = static_cast<std::uint8_t>(source & 0x0F);
            r.source_flags = static_cast<std::uint8_t>(source & 0xF0);
            r.flags = static_cast<std::uint8_t>(flags);
            r.first = static_cast<std::uint32_t>(n.u("object"));
            switch (flags & 3) {
                case 0:
                    r.kind = FixupRecord::Kind::Internal;
                    if (const eng::Node* t = n.child("target")) r.target = static_cast<std::uint32_t>(t->value);
                    break;
                case 1:
                    r.kind = FixupRecord::Kind::Import;
                    r.ordinal = static_cast<std::uint32_t>(n.u("ordinal"));
                    break;
                case 2:
                    r.kind = FixupRecord::Kind::ImportName;
                    r.name = static_cast<std::uint32_t>(n.u("name_offset"));
                    break;
                default: r.kind = FixupRecord::Kind::Entry; break;
            }
            if (const eng::Node* a = n.child("addend")) r.addend = static_cast<std::uint32_t>(a->value);
            if (source & 0x20) {
                if (const eng::Node* list = n.child("sources"))
                    for (const eng::Node& s : list->children) r.sources.push_back(static_cast<std::int16_t>(s.as_signed()));
            } else {
                r.sources.push_back(static_cast<std::int16_t>(n.i("first_source")));
            }
            const ByteView raw = d.subspan(n.offset, n.size);   // the record's bytes as stored
            r.raw.assign(raw.begin(), raw.end());
            out.push_back(std::move(r));
        }
    }
    return out;
}

std::vector<Reference> Module::references() const {
    std::vector<Reference> out;
    for (const FixupRecord& r : fixup_records()) {
        if (r.kind != FixupRecord::Kind::Internal || !r.target) continue;
        const auto owner = page_to_object(r.page);
        if (!owner) continue;
        for (std::int16_t s : r.sources)
            out.push_back({{owner->first->index, static_cast<std::int64_t>(owner->second) + s}, r.source, r.first,
                           static_cast<std::int64_t>(*r.target) + static_cast<std::int64_t>(r.addend.value_or(0))});
    }
    return out;
}

std::map<Place, Place> Module::pointers() const {
    std::map<Place, Place> out;
    for (const Reference& r : references())
        if (r.source == 7) out[r.at] = Place{r.target_object, r.target_offset};  // 32-bit offset fix-ups
    return out;
}

// ---- the device descriptor ------------------------------------------------------------------------------------

std::optional<Ddb> Module::ddb() const {
    const ByteView d = *data_;
    auto decode = [](const Object& obj, ByteView b, std::uint64_t off) -> std::optional<Ddb> {
        if (off < 4) return std::nullopt;  // the stamp sits 4 bytes into the block
        const ByteView name = clamped(b, off + 8, 8);
        if (name.empty() || !printable(name)) return std::nullopt;
        if (!is_alnum(name[0])) return std::nullopt;
        Ddb r;
        r.object = obj.index;
        r.rva = static_cast<std::int64_t>(off) - 4;
        r.version = get<std::uint16_t>(b, off, kReader);
        r.device_id = get<std::uint16_t>(b, off + 2, kReader);
        r.major = b[off + 4];
        r.minor = b[off + 5];
        r.flags = get<std::uint16_t>(b, off + 6, kReader);
        r.name = text::from_latin1(detail::rstrip_blanks(name));
        r.init_order = get<std::uint32_t>(b, off + 16, kReader);
        r.control_proc = get<std::uint32_t>(b, off + 20, kReader);
        return r;
    };
    // each object's bytes, read when the scan reaches it (a damaged page map throws there, not before)
    std::vector<std::optional<Bytes>> loaded(objects_.size());
    auto bytes_of = [&](std::size_t k) -> const Bytes& {
        if (!loaded[k]) loaded[k] = object_data(objects_[k]);
        return *loaded[k];
    };
    // strict DDK version stamps -- the common case
    for (std::size_t k = 0; k < objects_.size(); ++k) {
        const Bytes& b = bytes_of(k);
        for (std::uint64_t off = 0; off + 26 < b.size(); off += 4) {
            const unsigned v = b[off] | (b[off + 1] << 8);
            if (v != 0x030a && v != 0x0400 && v != 0x040a) continue;
            if (auto r = decode(objects_[k], b, off)) return r;
        }
    }
    // any 0x03xx/0x04xx stamp whose device id is the header's (+0xC0)
    const std::uint16_t header_device = header_.device_id;
    if (header_device) {
        for (std::size_t k = 0; k < objects_.size(); ++k) {
            const Bytes& b = bytes_of(k);
            for (std::uint64_t off = 0; off + 26 < b.size(); off += 4) {
                const unsigned ver = b[off] | (b[off + 1] << 8);
                if ((ver >> 8) != 3 && (ver >> 8) != 4) continue;
                if ((b[off + 2] | (b[off + 3] << 8)) != header_device) continue;
                if (auto r = decode(objects_[k], b, off)) return r;
            }
        }
    }
    return std::nullopt;
}

bool plausible_descriptor(ByteView b, std::uint64_t at) {
    if (at > b.size() || b.size() - at < 0x38) return false;
    const ByteView name = b.subspan(at + 0x0C, 8);
    const unsigned version = b[at + 4] | (b[at + 5] << 8);
    return 0x0300 <= version && version <= 0x04FF && is_alnum(name[0]) && printable(name);
}

std::optional<Descriptor> Module::descriptor() const {
    std::vector<std::optional<Bytes>> loaded(objects_.size());
    auto bytes_of = [&](std::size_t k) -> const Bytes& {
        if (!loaded[k]) loaded[k] = object_data(objects_[k]);
        return *loaded[k];
    };
    std::optional<Place> where;
    for (const EntryPoint& e : entries()) {
        if (0 < e.object && e.object <= objects_.size() && plausible_descriptor(bytes_of(e.object - 1), e.offset)) {
            where = Place{e.object, e.offset};
            break;
        }
    }
    if (!where) {
        auto old = ddb();
        if (!old) return std::nullopt;
        where = Place{old->object, old->rva};
    }
    if (where->object == 0 || where->object > objects_.size() || where->offset < 0)
        throw FormatError(kReader, lfa_, "descriptor is outside the image");
    const Bytes& b = bytes_of(where->object - 1);
    const std::uint64_t at = static_cast<std::uint64_t>(where->offset);
    Descriptor out;
    out.where = *where;
    out.name = text::from_latin1(detail::rstrip_blanks(clamped(b, at + 0x0C, 8)));
    out.next = get<std::uint32_t>(b, at + 0x00, kReader);
    out.kit_version = get<std::uint16_t>(b, at + 0x04, kReader);
    out.device_id = get<std::uint16_t>(b, at + 0x06, kReader);
    out.major = get<std::uint8_t>(b, at + 0x08, kReader);
    out.minor = get<std::uint8_t>(b, at + 0x09, kReader);
    out.flags = get<std::uint16_t>(b, at + 0x0A, kReader);
    out.init_order = get<std::uint32_t>(b, at + 0x14, kReader);
    out.control = get<std::uint32_t>(b, at + 0x18, kReader);
    out.v86_api = get<std::uint32_t>(b, at + 0x1C, kReader);
    out.pm_api = get<std::uint32_t>(b, at + 0x20, kReader);
    out.v86_api_address = get<std::uint32_t>(b, at + 0x24, kReader);
    out.pm_api_address = get<std::uint32_t>(b, at + 0x28, kReader);
    out.reference_data = get<std::uint32_t>(b, at + 0x2C, kReader);
    out.service_table = get<std::uint32_t>(b, at + 0x30, kReader);
    out.service_count = get<std::uint32_t>(b, at + 0x34, kReader);
    const auto ptr = pointers();
    auto target_of = [&](std::uint64_t field) -> std::optional<Place> {
        auto it = ptr.find(Place{where->object, static_cast<std::int64_t>(at + field)});
        return it != ptr.end() ? std::optional<Place>(it->second) : std::nullopt;
    };
    out.at_control = target_of(0x18);
    out.at_v86_api = target_of(0x1C);
    out.at_pm_api = target_of(0x20);
    out.at_service_table = target_of(0x30);
    out.at_reference_data = target_of(0x2C);
    out.size = (starts_with(b, at + 0x3C, "verP") && at + 0x50 <= b.size()) ? 0x50 : 0x34;
    const ByteView block = clamped(b, at, out.size);
    out.bytes.assign(block.begin(), block.end());
    return out;
}

std::vector<std::optional<Place>> Module::service_table(const Descriptor& d) const {
    std::vector<std::optional<Place>> out;
    if (!d.at_service_table || !(0 < d.service_count && d.service_count < 0x1000)) return out;
    const auto ptr = pointers();
    for (std::uint32_t i = 0; i < d.service_count; ++i) {
        auto it = ptr.find(Place{d.at_service_table->object, d.at_service_table->offset + 4 * std::int64_t{i}});
        out.push_back(it != ptr.end() ? std::optional<Place>(it->second) : std::nullopt);
    }
    return out;
}

std::vector<std::optional<Place>> Module::service_table() const {
    auto d = descriptor();
    return d ? service_table(*d) : std::vector<std::optional<Place>>{};
}

std::vector<ServiceCall> Module::service_calls() const {
    std::vector<ServiceCall> out;
    for (const Object& o : objects_) {
        if (!o.executable()) continue;
        const Bytes img = object_data(o);
        auto i = find(img, "\xCD\x20");
        while (i && *i + 6 <= img.size()) {
            const std::uint16_t svc = get<std::uint16_t>(img, *i + 2, kReader), dev = get<std::uint16_t>(img, *i + 4, kReader);
            if (0 < dev && dev < 0x600 && (svc & 0x7FFF) < 0x400) {
                out.push_back({o.index, *i, dev, static_cast<std::uint32_t>(svc & 0x7FFF), (svc & 0x8000) != 0});
                i = find(img, "\xCD\x20", *i + 6);
            } else {
                i = find(img, "\xCD\x20", *i + 2);
            }
        }
    }
    return out;
}

}  // namespace cb::formats::le
