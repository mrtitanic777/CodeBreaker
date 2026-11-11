// cb/formats/le_parts.cpp -- an LE image taken apart (Module::parts) and put back together (write_parts), with the
// small encoders it uses (le.hpp).
#include <algorithm>
#include <map>

#include "cb/core/io.hpp"
#include "cb/formats/le.hpp"
#include "cb/formats/le_internal.hpp"
#include "cb/formats/read.hpp"

namespace cb::formats::le {

using detail::kReader;

namespace {

[[noreturn]] void does_not_fit(std::string_view what) {
    throw FormatError(kReader, 0, std::string(what) + " does not fit its field");
}

// Narrowing with a range check: the value goes into a field of `width` bytes or the image cannot be written.
std::uint32_t fit(std::int64_t v, int width, std::string_view what) {
    if (v < 0 || (width < 4 && v >= (std::int64_t{1} << (8 * width))) || v > 0xFFFFFFFFLL) does_not_fit(what);
    return static_cast<std::uint32_t>(v);
}

void put_le(Bytes& out, std::uint64_t v, int width) {
    for (int k = 0; k < width; ++k) out.push_back(static_cast<std::uint8_t>(v >> (8 * k)));
}

void put_signed16(Bytes& out, std::int16_t v) { put_le(out, static_cast<std::uint16_t>(v), 2); }

// A header field written in place: 32 bits, range checked.
void put_into(Bytes& hdr, std::size_t at, std::int64_t v) {
    const std::uint32_t u = fit(v, 4, "header field");
    for (int k = 0; k < 4; ++k) hdr[at + k] = static_cast<std::uint8_t>(u >> (8 * k));
}

std::uint8_t name_length(std::size_t n) {
    if (n > 255) does_not_fit("name length");
    return static_cast<std::uint8_t>(n);
}

bool is_empty_table(const NameTable& t) {
    return std::visit([](const auto& v) { return v.empty(); }, t);
}

// Header field name -> its offset, for Parts::in_pages.
std::size_t header_field_offset(const std::string& name) {
    static const std::map<std::string, std::size_t> kOffsets = {
        {"byte_order", 0x02}, {"word_order", 0x03}, {"level", 0x04}, {"cpu", 0x08}, {"os", 0x0A}, {"version", 0x0C},
        {"module_flags", 0x10}, {"pages", 0x14}, {"eip_object", 0x18}, {"eip", 0x1C}, {"esp_object", 0x20},
        {"esp", 0x24}, {"page_size", 0x28}, {"last_page_bytes", 0x2C}, {"fixup_size", 0x30}, {"loader_size", 0x38},
        {"object_table", 0x40}, {"objects", 0x44}, {"page_map", 0x48}, {"resources", 0x50}, {"resident_names", 0x58},
        {"entry_table", 0x5C}, {"fixup_pages", 0x68}, {"fixup_records", 0x6C}, {"import_modules", 0x70},
        {"import_module_count", 0x74}, {"import_names", 0x78}, {"data_pages", 0x80}, {"preload_pages", 0x84},
        {"nonresident_names", 0x88}, {"nonresident_size", 0x8C}, {"debug_info", 0x98}, {"debug_size", 0x9C},
        {"resource_data", 0xB8}, {"resource_size", 0xBC}, {"device_id", 0xC0}, {"kit_version", 0xC2}};
    auto it = kOffsets.find(name);
    if (it == kOffsets.end()) throw UsageError("unknown LE header field '" + name + "'");
    return it->second;
}

}  // namespace

// ---- encoders --------------------------------------------------------------------------------------------

Bytes encode_fixup(const FixupRecord& rec) {
    const int src = rec.source | rec.source_flags;
    const bool many = (src & 0x20) != 0;
    const int fl = rec.flags;
    Bytes out{static_cast<std::uint8_t>(src), static_cast<std::uint8_t>(fl)};
    if (many) {
        out.push_back(name_length(rec.sources.size()));
    } else {
        if (rec.sources.empty()) throw UsageError("a fix-up record without a source offset");
        put_signed16(out, rec.sources.front());
    }
    const int wide = (fl & 0x40) ? 2 : 1;
    put_le(out, fit(rec.first, wide, "fix-up object"), wide);
    switch (rec.kind) {
        case FixupRecord::Kind::Internal:
            if (rec.source != 2) {
                if (!rec.target) throw UsageError("an internal fix-up record without a target");
                const int n = (fl & 0x10) ? 4 : 2;
                put_le(out, fit(*rec.target, n, "fix-up target"), n);
            }
            break;
        case FixupRecord::Kind::Import: {
            const int n = (fl & 0x80) ? 1 : ((fl & 0x10) ? 4 : 2);
            put_le(out, fit(rec.ordinal, n, "import ordinal"), n);
            break;
        }
        case FixupRecord::Kind::ImportName: {
            const int n = (fl & 0x10) ? 4 : 2;
            put_le(out, fit(rec.name, n, "import name offset"), n);
            break;
        }
        case FixupRecord::Kind::Entry: break;
    }
    if (fl & 0x04) {
        if (!rec.addend) throw UsageError("a fix-up record flagged additive without an addend");
        const int n = (fl & 0x20) ? 4 : 2;
        put_le(out, fit(*rec.addend, n, "fix-up addend"), n);
    }
    if (many)
        for (std::int16_t s : rec.sources) put_signed16(out, s);
    return out;
}

Bytes object_table(const std::vector<ObjectEntry>& objects) {
    Bytes out;
    for (const ObjectEntry& o : objects) {
        for (std::uint32_t v : {o.vsize, o.base, o.flags, o.first_page, o.page_count}) put_le(out, v, 4);
        if (o.reserved) out.insert(out.end(), o.reserved->begin(), o.reserved->end());
        else out.insert(out.end(), 4, std::uint8_t{0});
    }
    return out;
}

Bytes name_table(const NameTable& names) {
    if (const Bytes* stored = std::get_if<Bytes>(&names)) return *stored;
    Bytes out;
    for (const NameOrdinal& n : std::get<std::vector<NameOrdinal>>(names)) {
        const Bytes raw = text::to_latin1(n.name);
        out.push_back(name_length(raw.size()));
        append(out, raw);
        put_le(out, n.ordinal, 2);
    }
    out.push_back(0);
    return out;
}

std::pair<Bytes, Bytes> fixup_tables(const std::map<std::uint32_t, std::vector<Bytes>>& fixups, std::uint32_t pages) {
    Bytes table, records;
    for (std::uint32_t pg = 1; pg <= pages; ++pg) {
        put_le(table, fit(static_cast<std::int64_t>(records.size()), 4, "fix-up record offset"), 4);
        if (auto it = fixups.find(pg); it != fixups.end())
            for (const Bytes& r : it->second) append(records, r);
    }
    put_le(table, fit(static_cast<std::int64_t>(records.size()), 4, "fix-up record offset"), 4);
    return {std::move(table), std::move(records)};
}

// ---- taking an image apart -----------------------------------------------------------------------------------

Parts Module::parts() const {
    const ByteView d = *data_;
    const Header& h = header_;
    Parts out;
    const bool member = is_member();
    const auto pm = page_map();
    std::uint32_t last = 0;
    for (const PageEntry& e : pm) last = std::max(last, e.number);
    std::vector<Bytes> bodies;
    for (const PageEntry& e : pm) bodies.push_back(page_body(e, last));
    // Was the last stored page a whole one? (Then an unset last-page size is the page size, written as 0.)
    bool any_kept = false;
    std::size_t kept_last_size = 0;
    for (std::size_t k = 0; k < pm.size(); ++k)
        if (pm[k].kind != kPageInvalid && pm[k].kind != kPageZeroFilled && !bodies[k].empty()) {
            any_kept = true;
            kept_last_size = bodies[k].size();
        }
    const bool full_last = any_kept && kept_last_size == h.page_size;
    std::map<std::uint32_t, std::uint32_t> page_of;  // a page's number -> its first place in the map (1-based)
    for (std::size_t i = pm.size(); i-- > 0;)
        if (pm[i].number) page_of[pm[i].number] = static_cast<std::uint32_t>(i + 1);
    for (const FixupRecord& r : fixup_records()) {  // first seen first
        auto it = page_of.find(r.page);
        out.fixups.try_emplace(it != page_of.end() ? it->second : r.page).first->second.push_back(r.raw);
    }
    const auto [res, non] = names();
    const std::int64_t names_end = !non.empty() ? std::int64_t{h.nonresident_names} + h.nonresident_size : 0;
    const std::int64_t pages_end =
        std::int64_t{h.data_pages} + (last ? std::int64_t{last - 1} * h.page_size + (h.last_page_bytes ? h.last_page_bytes : h.page_size) : 0);
    std::uint32_t align = 1;
    while (align < 0x1000 && h.data_pages % (align * 2) == 0) align *= 2;
    const std::int64_t import_at = static_cast<std::int64_t>(lfa_) + h.import_names;
    const std::int64_t import_end = static_cast<std::int64_t>(lfa_) + h.fixup_size + h.fixup_pages;
    const std::int64_t size = static_cast<std::int64_t>(d.size());

    auto range = [&](std::int64_t a, std::int64_t b) {
        const ByteView v = clamped(d, static_cast<std::uint64_t>(std::max<std::int64_t>(a, 0)), static_cast<std::uint64_t>(std::max<std::int64_t>(b - a, 0)));
        return Bytes(v.begin(), v.end());
    };
    if (!member) {
        out.stub = range(0, static_cast<std::int64_t>(lfa_));
    } else {
        out.stub = {'M', 'Z'};
        out.stub.resize(0x3C, 0);
        put_le(out.stub, 0x40, 4);
    }
    out.header = range(static_cast<std::int64_t>(lfa_), static_cast<std::int64_t>(lfa_ + kHeaderSize));
    for (std::size_t i = 0; i < objects_.size(); ++i) {
        const Object& ob = objects_[i];
        ObjectEntry e{ob.vsize, ob.base, ob.flags, ob.first_page, ob.page_count, std::nullopt};
        const ByteView res4 = clamped(d, objtab_ + 24 * i + 20, 4);
        if (res4.size() == 4) e.reserved = std::array<std::uint8_t, 4>{res4[0], res4[1], res4[2], res4[3]};
        out.objects.push_back(e);
    }
    for (std::size_t k = 0; k < pm.size(); ++k) out.pages.push_back({pm[k].kind, bodies[k]});
    out.resident = res;
    out.nonresident = non;
    out.entry_table = range(static_cast<std::int64_t>(entrytab_), static_cast<std::int64_t>(entrytab_ + entry_table_size()));
    out.import_modules = import_modules();
    if (h.import_names && static_cast<std::int64_t>(lfa_) < import_at && import_at < import_end && import_end <= size)
        out.import_names = range(import_at, import_end);
    out.align = align;
    if (!h.last_page_bytes && full_last) out.last_page_bytes = 0;
    const std::int64_t after_from = std::max(pages_end, names_end);
    const std::int64_t gap_start = import_end;
    if (static_cast<std::int64_t>(lfa_) < gap_start && gap_start <= std::int64_t{h.data_pages} && !member)
        out.gaps.emplace(range(gap_start, h.data_pages), Bytes());
    else
        out.gaps.emplace(Bytes(), Bytes());
    const std::int64_t nr = h.nonresident_names;
    if (!non.empty() && pages_end <= nr && nr < pages_end + 16) out.gaps->second = range(pages_end, nr);
    if (!member) out.after = range(after_from, size);
    out.moved = {h.debug_info, h.resource_data, static_cast<std::uint32_t>(after_from)};
    return out;
}

// ---- putting it back -----------------------------------------------------------------------------------------

Bytes write_parts(const Parts& image) {
    Bytes hdr = image.header;
    if (hdr.size() < kHeaderSize) hdr.resize(kHeaderSize, 0);
    const std::int64_t page_size = get<std::uint32_t>(hdr, 0x28, kReader);
    const Bytes table = object_table(image.objects);
    const std::int64_t npages = static_cast<std::int64_t>(image.pages.size());
    auto stored_page = [&](std::size_t i) {
        const StoredPage& p = image.pages[i];
        return p.kind != kPageInvalid && p.kind != kPageZeroFilled && !p.body.empty();
    };
    std::vector<std::size_t> stored;  // places in the map of the pages that carry bytes
    for (std::size_t i = 0; i < image.pages.size(); ++i)
        if (stored_page(i)) stored.push_back(i);

    // {place in the map: page number}, in stored order; and the page map itself
    std::vector<std::pair<std::size_t, std::uint32_t>> number;
    Bytes pmap;
    if (image.page_map) {
        for (std::size_t i : stored) number.emplace_back(i, detail::be24(*image.page_map, 4 * i));
        pmap = *image.page_map;
    } else {
        std::map<std::size_t, std::uint32_t> assigned;
        for (std::size_t n = 0; n < stored.size(); ++n) {
            number.emplace_back(stored[n], static_cast<std::uint32_t>(n + 1));
            assigned[stored[n]] = static_cast<std::uint32_t>(n + 1);
        }
        for (std::size_t i = 0; i < image.pages.size(); ++i) {
            auto it = assigned.find(i);
            const std::uint32_t num = it != assigned.end() ? it->second : 0;
            for (int k = 2; k >= 0; --k) pmap.push_back(static_cast<std::uint8_t>(num >> (8 * k)));  // 3 bytes, big-endian
            pmap.push_back(image.pages[i].kind);
        }
    }
    std::map<std::uint32_t, std::size_t> at;  // {page number: place in the map}, the last place wins
    for (const auto& [place, num] : number) at[num] = place;
    const std::uint32_t top = at.empty() ? 0 : at.rbegin()->first;
    if (top > 0 && page_size * top > (std::int64_t{1} << 31)) throw UnsupportedError(kReader, 0, "image is too large");
    Bytes data;
    for (std::uint32_t n = 1; n <= top; ++n) {
        auto it = at.find(n);
        if (it == at.end()) {
            data.insert(data.end(), static_cast<std::size_t>(page_size), std::uint8_t{0});
            continue;
        }
        Bytes b = image.pages[it->second].body;
        if (n != top && static_cast<std::int64_t>(b.size()) < page_size) b.resize(static_cast<std::size_t>(page_size), 0);
        append(data, b);
    }
    const std::int64_t last_len = top ? static_cast<std::int64_t>(image.pages[at[top]].body.size()) : 0;
    const Bytes resident = name_table(image.resident);
    std::map<std::uint32_t, std::uint32_t> num_of;  // place in the map -> page number
    for (const auto& [place, num] : number) num_of[static_cast<std::uint32_t>(place)] = num;
    std::map<std::uint32_t, std::vector<Bytes>> fix;
    for (const auto& [pg, recs] : image.fixups) {
        auto it = num_of.find(pg - 1);
        fix[it != num_of.end() ? it->second : pg] = recs;  // (a later page with the same number wins)
    }
    const auto [fix_pages, fix_records] = fixup_tables(fix, static_cast<std::uint32_t>(npages));
    Bytes modules;
    for (const std::string& m : image.import_modules) {
        const Bytes n = text::to_latin1(m);
        modules.push_back(name_length(n.size()));
        append(modules, n);
    }
    const Bytes tail = image.tables_tail.value_or(Bytes());

    // where each table lands, in order, right after the header
    std::int64_t pos = kHeaderSize;
    std::int64_t where_object_table = pos;
    pos += static_cast<std::int64_t>(table.size());
    const std::int64_t where_page_map = pos;
    pos += static_cast<std::int64_t>(pmap.size());
    const std::int64_t where_resident = pos;
    pos += static_cast<std::int64_t>(resident.size());
    const std::int64_t where_entry = pos;
    pos += static_cast<std::int64_t>(image.entry_table.size());
    const std::int64_t where_fix_pages = pos;
    pos += static_cast<std::int64_t>(fix_pages.size());
    const std::int64_t where_fix_records = pos;
    pos += static_cast<std::int64_t>(fix_records.size());
    const std::int64_t where_modules = pos;
    pos += static_cast<std::int64_t>(modules.size());
    const std::int64_t where_import_names = pos;
    pos += static_cast<std::int64_t>(image.import_names.size());
    pos += static_cast<std::int64_t>(tail.size());

    const std::int64_t base = static_cast<std::int64_t>(image.stub.size());
    std::int64_t pages_at = image.data_pages.value_or(0);
    if (pages_at < base + pos) {
        if (!image.align || *image.align == 0) throw UsageError("an LE image to write needs a nonzero alignment");
        const std::int64_t al = *image.align;
        pages_at = ((base + pos + al - 1) / al) * al;
    }
    put_into(hdr, 0x14, npages);
    put_into(hdr, 0x2C, image.last_page_bytes ? std::int64_t{*image.last_page_bytes} : last_len);
    put_into(hdr, 0x30, static_cast<std::int64_t>(fix_pages.size() + fix_records.size() + modules.size() + image.import_names.size()));
    put_into(hdr, 0x38, where_fix_pages - static_cast<std::int64_t>(kHeaderSize));
    put_into(hdr, 0x40, where_object_table);
    put_into(hdr, 0x44, static_cast<std::int64_t>(image.objects.size()));
    put_into(hdr, 0x48, where_page_map);
    put_into(hdr, 0x58, where_resident);
    put_into(hdr, 0x5C, where_entry);
    put_into(hdr, 0x68, where_fix_pages);
    put_into(hdr, 0x6C, where_fix_records);
    put_into(hdr, 0x70, where_modules);
    put_into(hdr, 0x74, static_cast<std::int64_t>(image.import_modules.size()));
    put_into(hdr, 0x78, where_import_names);
    put_into(hdr, 0x80, pages_at);

    Bytes out = image.stub;
    const Bytes* const consecutive[] = {&hdr,       &table,        &pmap,    &resident,           &image.entry_table,
                                        &fix_pages, &fix_records, &modules, &image.import_names, &tail};
    for (const Bytes* part : consecutive) append(out, *part);
    Bytes gap, names_gap;
    if (image.gaps) {
        gap = image.gaps->first;
        names_gap = image.gaps->second;
    }
    if (static_cast<std::int64_t>(gap.size()) == pages_at - static_cast<std::int64_t>(out.size())) append(out, gap);
    else if (pages_at < static_cast<std::int64_t>(out.size())) throw UsageError("the data pages start inside the tables");
    else out.insert(out.end(), static_cast<std::size_t>(pages_at - static_cast<std::int64_t>(out.size())), std::uint8_t{0});
    append(out, data);
    const Bytes non = is_empty_table(image.nonresident) ? Bytes() : name_table(image.nonresident);
    append(out, names_gap);
    const bool nonres_in_pages = image.in_pages && image.in_pages->contains("nonresident_names");
    put_into(hdr, 0x8C, static_cast<std::int64_t>(non.size()));
    if (!nonres_in_pages) {  // else it is already among the data pages
        put_into(hdr, 0x88, non.empty() ? 0 : static_cast<std::int64_t>(out.size()));
        append(out, non);
    }
    const std::int64_t shift = static_cast<std::int64_t>(out.size()) - image.moved.from;
    for (const auto& [field, v] : {std::pair<std::size_t, std::int64_t>{0x98, image.moved.debug_info},
                                   std::pair<std::size_t, std::int64_t>{0xB8, image.moved.resource_data}})
        put_into(hdr, field, v && !image.after.empty() ? v + shift : v);
    if (image.in_pages) {
        if (!image.data_pages) throw UsageError("an image with in_pages fields needs data_pages");
        for (const auto& [name, v] : *image.in_pages)
            put_into(hdr, header_field_offset(name), v + pages_at - std::int64_t{*image.data_pages});
    }
    append(out, image.after);
    if (image.fill_from && image.fill_from->size() > out.size())
        append(out, ByteView(*image.fill_from).subspan(out.size()));
    // out[base : base + header size] = hdr -- a slice assignment: the stretch is replaced, and past the end the header is appended
    const std::size_t at_header = std::min<std::size_t>(static_cast<std::size_t>(base), out.size());
    const std::size_t stop = std::min<std::size_t>(at_header + kHeaderSize, out.size());
    out.erase(out.begin() + static_cast<std::ptrdiff_t>(at_header), out.begin() + static_cast<std::ptrdiff_t>(stop));
    out.insert(out.begin() + static_cast<std::ptrdiff_t>(at_header), hdr.begin(), hdr.end());
    return out;
}

}  // namespace cb::formats::le
