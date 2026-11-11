// cb/formats/le.cpp -- the LE reader: header, objects, pages, names, entries, fix-ups, the device descriptor (le.hpp).
#include "cb/formats/le.hpp"

#include <algorithm>
#include <unordered_map>

#include "cb/core/fs.hpp"
#include "cb/formats/le_internal.hpp"
#include "cb/formats/read.hpp"

namespace cb::formats::le {

using detail::kReader;

namespace {

constexpr std::uint64_t kMaxObjectBytes = std::uint64_t{1} << 30;  // an object past this is not a real driver

bool printable(ByteView b) {
    return std::all_of(b.begin(), b.end(), [](std::uint8_t c) { return c >= 32 && c < 127; });
}
bool is_alnum(std::uint8_t c) { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }

// the entry-record size of an entry-table bundle kind, or 0 when the kind is unknown
std::size_t entry_size(int kind) {
    switch (kind) {
        case 1: return 3;
        case 2:
        case 3: return 5;
        case 4: return 7;
        default: return 0;
    }
}

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

Module Module::parse(std::shared_ptr<const Bytes> data, std::uint64_t lfa) {
    Module m;
    m.data_ = std::move(data);
    m.lfa_ = lfa;
    const ByteView d = *m.data_;
    m.signature_ = text::from_latin1(clamped(d, lfa, 2));
    auto req32 = [&](std::uint64_t o) { return get<std::uint32_t>(d, lfa + o, kReader); };
    auto req16 = [&](std::uint64_t o) { return get<std::uint16_t>(d, lfa + o, kReader); };
    auto opt32 = [&](std::uint64_t o) { return get_or_zero<std::uint32_t>(d, lfa + o); };
    auto opt16 = [&](std::uint64_t o) { return get_or_zero<std::uint16_t>(d, lfa + o); };
    Header& h = m.header_;
    h.byte_order = get_or_zero<std::uint8_t>(d, lfa + 0x02);
    h.word_order = get_or_zero<std::uint8_t>(d, lfa + 0x03);
    h.level = opt32(0x04);
    h.cpu = req16(0x08);
    h.os = req16(0x0a);
    h.version = opt32(0x0c);
    h.module_flags = opt32(0x10);
    h.pages = req32(0x14);
    h.eip_object = req32(0x18);
    h.eip = req32(0x1c);
    h.esp_object = opt32(0x20);
    h.esp = opt32(0x24);
    h.page_size = req32(0x28);
    h.last_page_bytes = req32(0x2c);
    h.fixup_size = opt32(0x30);
    h.loader_size = opt32(0x38);
    h.object_table = req32(0x40);
    h.objects = req32(0x44);
    h.page_map = req32(0x48);
    h.resources = opt32(0x50);
    h.resident_names = req32(0x58);
    h.entry_table = req32(0x5c);
    h.fixup_pages = req32(0x68);
    h.fixup_records = req32(0x6c);
    h.import_modules = opt32(0x70);
    h.import_module_count = opt32(0x74);
    h.import_names = opt32(0x78);
    h.data_pages = req32(0x80);  // an offset from the top of the file (not the one at 0x7c)
    h.preload_pages = opt32(0x84);
    h.nonresident_names = opt32(0x88);
    h.nonresident_size = opt32(0x8c);
    h.debug_info = opt32(0x98);
    h.debug_size = opt32(0x9c);
    h.resource_data = opt32(0xb8);
    h.resource_size = opt32(0xbc);
    h.device_id = opt16(0xc0);
    h.kit_version = opt16(0xc2);
    m.objtab_ = lfa + h.object_table;
    m.pagemap_ = lfa + h.page_map;
    m.resnames_ = lfa + h.resident_names;
    m.entrytab_ = lfa + h.entry_table;
    m.fixup_page_ = lfa + h.fixup_pages;
    m.fixup_rec_ = lfa + h.fixup_records;
    for (std::uint32_t i = 0; i < h.objects; ++i) {
        const std::uint64_t at = m.objtab_ + std::uint64_t{i} * 24;
        Object o;
        o.index = i + 1;
        o.vsize = get<std::uint32_t>(d, at, kReader);
        o.base = get<std::uint32_t>(d, at + 4, kReader);
        o.flags = get<std::uint32_t>(d, at + 8, kReader);
        o.first_page = get<std::uint32_t>(d, at + 12, kReader);
        o.page_count = get<std::uint32_t>(d, at + 16, kReader);
        m.objects_.push_back(o);
    }
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
// found by that number (the first page carrying a number wins). Read once, here, tolerantly: past the end of the
// file every number is 0 and nothing new follows.
void Module::index_pages() {
    const ByteView d = *data_;
    for (std::uint32_t i = 0; i < header_.pages; ++i) {
        const std::uint64_t at = pagemap_ + 4 * std::uint64_t{i};
        page_order_.emplace(detail::be24(d, at), i + 1);
        if (at >= d.size()) break;
    }
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
    std::vector<PageEntry> out;
    for (std::uint32_t i = 0; i < header_.pages; ++i) {
        const ByteView rec = slice(*data_, pagemap_ + 4 * std::uint64_t{i}, 4, kReader);
        out.push_back({detail::be24(rec, 0), rec[3]});
    }
    return out;
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
    const std::uint8_t n = get<std::uint8_t>(d, resnames_, kReader);
    return text::from_latin1(clamped(d, resnames_ + 1, n));
}

std::string Module::description() const {
    const ByteView d = *data_;
    const std::uint64_t off = lfa_ + 0x8c <= d.size() ? get<std::uint32_t>(d, lfa_ + 0x88, kReader) : 0;
    if (!(0 < off && off < d.size())) return {};
    return text::from_latin1(clamped(d, off + 1, d[off]));
}

std::vector<std::string> Module::import_modules() const {
    const ByteView d = *data_;
    const std::uint64_t off = lfa_ + get<std::uint32_t>(d, lfa_ + 0x70, kReader);
    const std::uint32_t count = get<std::uint32_t>(d, lfa_ + 0x74, kReader);
    std::vector<std::string> names;
    std::uint64_t p = off;
    for (std::uint32_t k = 0; k < count; ++k) {
        const std::uint8_t n = get<std::uint8_t>(d, p, kReader);
        names.push_back(text::from_latin1(clamped(d, p + 1, n)));
        p += 1 + std::uint64_t{n};
    }
    return names;
}

std::vector<NameOrdinal> names_at(ByteView d, std::uint64_t p, std::optional<std::uint64_t> end) {
    std::vector<NameOrdinal> out;
    while (0 < p && p < d.size() && d[p] && (!end || p < *end)) {
        const std::uint8_t n = d[p];
        if (p + n + 3 > d.size()) break;
        out.push_back({text::from_latin1(d.subspan(p + 1, n)), get<std::uint16_t>(d, p + 1 + n, kReader)});
        p += n + 3u;
    }
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
    const ByteView d = *data_;
    std::uint64_t p = entrytab_;
    std::uint32_t ordinal = 1;
    while (p + 2 <= d.size() && d[p]) {
        const int count = d[p], kind = d[p + 1] & 0x7F;
        p += 2;
        if (kind == 0) {  // a run of unused ordinals
            ordinal += count;
            continue;
        }
        const std::size_t size = entry_size(kind);
        if (size == 0 || p + 2 + std::uint64_t{static_cast<unsigned>(count)} * size > d.size()) break;
        const std::uint16_t obj = get<std::uint16_t>(d, p, kReader);
        p += 2;
        for (int k = 0; k < count; ++k, p += size) {
            const std::uint64_t at = p + (kind == 4 ? 3 : 1);
            const std::uint32_t off = (kind == 3 || kind == 4) ? get<std::uint32_t>(d, at, kReader) : get<std::uint16_t>(d, at, kReader);
            out.push_back({ordinal++, kind != 4 ? obj : 0u, off, d[p]});
        }
    }
    return out;
}

std::size_t Module::entry_table_size() const {
    const ByteView d = *data_;
    std::uint64_t p = entrytab_;
    while (p + 2 <= d.size() && d[p]) {
        const int count = d[p], kind = d[p + 1] & 0x7F;
        std::size_t size = 0;
        if (kind != 0) {
            size = entry_size(kind);
            if (size == 0) break;
        }
        p += 2 + (kind == 0 ? 0 : 2 + static_cast<std::uint64_t>(count) * size);
    }
    return p + 1 - entrytab_;
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
    const ByteView d = *data_;
    std::map<Place, std::size_t> where;
    std::vector<std::uint32_t> page_offsets;
    for (std::uint64_t i = 0; i <= header_.pages; ++i) page_offsets.push_back(get<std::uint32_t>(d, fixup_page_ + i * 4, kReader));
    for (std::uint32_t pg = 0; pg < header_.pages; ++pg) {
        const auto owner = page_to_object(pg + 1);
        std::uint64_t p = fixup_rec_ + page_offsets[pg];
        const std::uint64_t end = fixup_rec_ + page_offsets[pg + 1];
        while (p < end) {
            const std::uint8_t src = get<std::uint8_t>(d, p, kReader), flags = get<std::uint8_t>(d, p + 1, kReader);
            p += 2;
            const bool source_list = (src & 0x20) != 0;  // several sources, one target
            std::uint8_t count = 0;
            std::int16_t first_source = 0;
            if (source_list) {
                count = get<std::uint8_t>(d, p++, kReader);
            } else {
                first_source = get<std::int16_t>(d, p, kReader);
                p += 2;
            }
            Fixup fx;
            const int target_kind = flags & 0x03;
            if (target_kind == 0 || target_kind == 1) {
                const bool wide_object = (flags & 0x40) != 0;
                const std::uint32_t obj_or_module = wide_object ? get<std::uint16_t>(d, p, kReader) : get<std::uint8_t>(d, p, kReader);
                p += wide_object ? 2 : 1;
                if (target_kind == 0) {  // an internal reference
                    InternalFixup in;
                    in.object = obj_or_module;
                    in.source_type = src & 0x0f;
                    if ((src & 0x0f) == 0x02) {  // a 16-bit selector: no offset
                    } else if (flags & 0x10) {
                        in.target = get<std::uint32_t>(d, p, kReader);
                        p += 4;
                    } else {
                        in.target = get<std::uint16_t>(d, p, kReader);
                        p += 2;
                    }
                    fx = in;
                } else {  // an import by ordinal
                    ImportFixup im;
                    im.module = obj_or_module;
                    if (flags & 0x80) {
                        im.ordinal = get<std::uint8_t>(d, p++, kReader);
                    } else if (flags & 0x10) {
                        im.ordinal = get<std::uint32_t>(d, p, kReader);
                        p += 4;
                    } else {
                        im.ordinal = get<std::uint16_t>(d, p, kReader);
                        p += 2;
                    }
                    fx = im;
                }
            } else {
                break;  // import by name / entry: the rest of this page's records is not read
            }
            if (flags & 0x04) p += (flags & 0x20) ? 4 : 2;  // an additive value follows the target
            std::vector<std::int16_t> offsets;
            if (source_list) {
                for (std::uint8_t j = 0; j < count; ++j) offsets.push_back(get<std::int16_t>(d, p + std::uint64_t{j} * 2, kReader));
                p += std::uint64_t{count} * 2;
            } else {
                offsets.push_back(first_source);
            }
            if (owner) {
                for (std::int16_t o : offsets) {
                    const Place key{owner->first->index, static_cast<std::int64_t>(owner->second) + o};
                    auto [it, fresh] = where.try_emplace(key, out.size());
                    if (fresh) out.emplace_back(key, fx);
                    else out[it->second].second = fx;
                }
            }
        }
    }
    return out;
}

std::vector<FixupRecord> Module::fixup_records() const {
    std::vector<FixupRecord> out;
    const ByteView d = *data_;
    std::vector<std::uint32_t> offs;
    for (std::uint64_t i = 0; i <= header_.pages; ++i) offs.push_back(get<std::uint32_t>(d, fixup_page_ + 4 * i, kReader));
    // A fix-up value of 2 or 4 bytes, strict.
    auto value = [&](std::uint64_t at, int n) -> std::uint32_t {
        return n == 4 ? get<std::uint32_t>(d, at, kReader) : n == 2 ? get<std::uint16_t>(d, at, kReader) : get<std::uint8_t>(d, at, kReader);
    };
    for (std::uint32_t pg = 0; pg < header_.pages; ++pg) {
        std::uint64_t p = fixup_rec_ + offs[pg];
        const std::uint64_t end = fixup_rec_ + offs[pg + 1];
        while (p + 4 <= end) {
            FixupRecord r;
            const std::uint64_t p0 = p;
            const std::uint8_t src = get<std::uint8_t>(d, p, kReader), fl = get<std::uint8_t>(d, p + 1, kReader);
            p += 2;
            std::optional<std::uint8_t> count;
            if (src & 0x20) {
                count = get<std::uint8_t>(d, p, kReader);
                p += 1;
            } else {
                r.sources.push_back(get<std::int16_t>(d, p, kReader));
                p += 2;
            }
            const int wide = (fl & 0x40) ? 2 : 1;
            r.first = value(p, wide);
            p += wide;
            r.page = pg + 1;
            r.source = src & 0x0F;
            r.source_flags = src & 0xF0;
            r.flags = fl;
            switch (fl & 3) {
                case 0:
                    r.kind = FixupRecord::Kind::Internal;
                    if ((src & 0x0F) != 2) {
                        const int n = (fl & 0x10) ? 4 : 2;
                        r.target = value(p, n);
                        p += n;
                    }
                    break;
                case 1: {
                    const int n = (fl & 0x80) ? 1 : ((fl & 0x10) ? 4 : 2);
                    r.kind = FixupRecord::Kind::Import;
                    r.ordinal = value(p, n);
                    p += n;
                    break;
                }
                case 2: {
                    const int n = (fl & 0x10) ? 4 : 2;
                    r.kind = FixupRecord::Kind::ImportName;
                    r.name = value(p, n);
                    p += n;
                    break;
                }
                default: r.kind = FixupRecord::Kind::Entry; break;
            }
            if (fl & 0x04) {
                const int n = (fl & 0x20) ? 4 : 2;
                r.addend = value(p, n);
                p += n;
            }
            if (count) {
                for (std::uint8_t j = 0; j < *count; ++j) r.sources.push_back(get<std::int16_t>(d, p + 2 * std::uint64_t{j}, kReader));
                p += 2 * std::uint64_t{*count};
            }
            const ByteView raw = slice(d, p0, p - p0, kReader);
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
