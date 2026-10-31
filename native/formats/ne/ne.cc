// ne.cc -- the NE reader (ne.h).
#include "ne.h"

#include <algorithm>

#include "../core/text.h"

namespace cbf::ne {

const char *resource_type_name(uint16_t id) {
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

Module::Module(Bytes d, int64_t off) : data(d), ne_off(off) { parse_header(); }

Module Module::open(Bytes d) {
    if (!d.slice_is(0, 2, "MZ", 2)) raise("ValueError", "not an MZ file");
    int64_t off = d.u32(0x3C);       // e_lfanew is 4 bytes: a stub of 64 KB or more needs all of them
    if (!d.slice_is(off, off + 2, "NE", 2))
        raise("ValueError", "no NE signature at 0x" + hex(uint64_t(off)));
    return Module(d, off);
}

void Module::parse_header() {
    const int64_t o = ne_off;
    h.linker_ver = data.at(o + 0x02);
    h.linker_rev = data.at(o + 0x03);
    h.entry_off = u16(o + 0x04);
    h.entry_len = u16(o + 0x06);
    h.crc = u32(o + 0x08);
    h.flags = u16(o + 0x0C);
    h.autodata_seg = u16(o + 0x0E);
    h.heap = u16(o + 0x10);
    h.stack = u16(o + 0x12);
    h.csip = u32(o + 0x14);
    h.sssp = u32(o + 0x18);
    h.n_segs = u16(o + 0x1C);
    h.n_modrefs = u16(o + 0x1E);
    h.nonres_size = u16(o + 0x20);
    h.seg_tab = u16(o + 0x22);
    h.rsrc_tab = u16(o + 0x24);
    h.resname_tab = u16(o + 0x26);
    h.mod_tab = u16(o + 0x28);
    h.imp_tab = u16(o + 0x2A);
    h.nonres_off = u32(o + 0x2C);
    h.n_movable = u16(o + 0x30);
    h.align_shift = u16(o + 0x32);
    if (!h.align_shift) h.align_shift = 9;
    h.n_resources = u16(o + 0x34);
    h.exetype = data.at(o + 0x36);
    if (h.seg_tab >= 0x40) h.expect_winver = u16(o + 0x3E);
}

std::vector<Name> Module::read_name_table(int64_t p) const {
    std::vector<Name> out;
    while (p < data.size()) {
        uint8_t ln = data.at(p);
        p += 1;
        if (ln == 0) break;
        Bytes name = data.slice(p, p + ln);
        p += ln;
        uint16_t ord = u16(p);
        p += 2;
        out.push_back({name, ord});
    }
    return out;
}

std::vector<Name> Module::resident_names() const { return read_name_table(ne_off + h.resname_tab); }

std::vector<Name> Module::nonresident_names() const {
    if (boot) {                       // in the overlay, at the header's offset in alignment units
        if (!h.nonres_off || overlay_at >= data.size()) return {};
        return read_name_table(overlay_at + py_shl(h.nonres_off, h.align_shift));
    }
    if (!h.nonres_off) return {};
    return read_name_table(h.nonres_off);
}

std::string Module::module_name() const {
    auto r = resident_names();
    return r.empty() ? "SELF" : latin1(r[0].name);
}

std::vector<std::string> Module::module_refs() const {
    std::vector<std::string> names;
    const int64_t base = ne_off + h.mod_tab, imp = ne_off + h.imp_tab;
    for (int64_t i = 0; i < h.n_modrefs; ++i) {
        int64_t off = u16(base + i * 2);
        uint8_t ln = data.at(imp + off);
        names.push_back(latin1(data.slice(imp + off + 1, imp + off + 1 + ln)));
    }
    return names;
}

std::string Module::imported_name(int64_t off) const {
    const int64_t base = ne_off + h.imp_tab;
    if (!(0 <= base + off && base + off < data.size())) return "?";   // out-of-range import-name offset
    uint8_t ln = data.at(base + off);
    return latin1(data.slice(base + off + 1, base + off + 1 + ln));
}

std::vector<Segment> Module::segments() const {
    std::vector<Segment> segs;
    const int64_t base = ne_off + h.seg_tab;
    for (int64_t i = 0; i < h.n_segs; ++i) {
        const int64_t o = base + i * 8;
        uint16_t sector = u16(o), length = u16(o + 2), flags = u16(o + 4), minalloc = u16(o + 6);
        Segment s;
        s.idx = i + 1;
        s.file_off = py_shl(sector, h.align_shift) + (sector ? this->base : 0);
        s.length = length ? length : (sector ? 0x10000 : 0);
        s.flags = flags;
        s.minalloc = minalloc ? minalloc : 0x10000;
        if (boot && s.file_off && !(flags & kPreload)) s.file_off += overlay_at;
        segs.push_back(s);
    }
    return segs;
}

std::vector<Relocation> Module::relocations(uint16_t flags, int64_t file_off, int64_t length) const {
    std::vector<Relocation> recs;
    if (!(flags & kRelocInfo)) return recs;
    const int64_t end = file_off + length;
    if (end + 2 > data.size()) return recs;
    const int64_t n = u16(end);
    int64_t p = end + 2;
    // A relocated segment base (segfix.py) does not relocate its reloc table, so the count can be
    // garbage: then there is nothing to read rather than an error.
    if (p + 8 * n > data.size()) return recs;
    for (int64_t k = 0; k < n; ++k, p += 8) {
        Relocation r;
        uint8_t src_type = data.at(p), rel_type = data.at(p + 1);
        r.offset = u16(p + 2);
        uint16_t b4 = u16(p + 4), b6 = u16(p + 6);
        switch (rel_type & 3) {
        case 0:
            r.kind = Relocation::Internal;
            r.seg = data.at(p + 4);
            r.entry = b6;
            break;
        case 1:
            r.kind = Relocation::ImportOrdinal;
            r.module = b4;
            r.ordinal = b6;
            break;
        case 2:
            r.kind = Relocation::ImportName;
            r.module = b4;
            r.name_off = b6;
            break;
        default:
            r.kind = Relocation::OsFixup;
            r.ftype = b4;
        }
        r.src = src_type & 0x0f;
        r.rel_type = rel_type;
        recs.push_back(r);
    }
    return recs;
}

std::vector<Entry> Module::entries() const {
    std::vector<Entry> out;
    int64_t p = ne_off + h.entry_off;
    const int64_t end = p + h.entry_len;
    int64_t ordinal = 1;
    while (p < end) {
        uint8_t cnt = data.at(p), seg_ind = data.at(p + 1);
        p += 2;
        if (cnt == 0) break;
        if (seg_ind == 0x00) {                             // a run of unused ordinals
            for (int k = 0; k < cnt; ++k) out.push_back({Entry::Unused, ordinal++});
        } else if (seg_ind == 0xFF) {                      // moveable: flags, INT 3F, segment, offset
            for (int k = 0; k < cnt; ++k, p += 6) {
                Entry e{Entry::Moveable, ordinal++};
                e.flags = data.at(p);
                e.seg = data.at(p + 3);
                e.offset = u16(p + 4);
                out.push_back(e);
            }
        } else {                                           // fixed, in segment seg_ind
            for (int k = 0; k < cnt; ++k, p += 3) {
                Entry e{Entry::Fixed, ordinal++};
                e.flags = data.at(p);
                e.offset = u16(p + 1);
                e.seg = seg_ind;
                out.push_back(e);
            }
        }
    }
    return out;
}

std::vector<int64_t> Module::export_ordinals() const {
    std::vector<int64_t> out;
    for (const Entry &e : entries())
        if (e.kind != Entry::Unused) out.push_back(e.ordinal);
    return out;
}

std::string Module::rsrc_string(int64_t b, int64_t off) const {
    const int64_t p = b + off;
    uint8_t ln = data.at(p);
    return latin1(data.slice(p + 1, p + 1 + ln));
}

std::vector<Resource> Module::resources() const {
    std::vector<Resource> out;
    const int64_t base = ne_off + h.rsrc_tab;
    if (h.rsrc_tab == h.resname_tab) return out;           // no resource table
    int64_t p = base;
    const uint16_t shift = u16(p);
    p += 2;
    for (;;) {
        uint16_t type_id = u16(p);
        if (type_id == 0) break;
        uint16_t count = u16(p + 2);
        p += 8;
        std::string res_type;
        if (type_id & 0x8000) {
            const char *n = resource_type_name(type_id & 0x7fff);
            res_type = n ? n : "#" + dec(type_id & 0x7fff);
        } else {
            res_type = rsrc_string(base, type_id);
        }
        for (int k = 0; k < count; ++k, p += 12) {
            Resource r;
            r.type = res_type;
            r.off = py_shl(u16(p), shift);
            r.length = py_shl(u16(p + 2), shift);
            r.flags = u16(p + 4);
            uint16_t rid = u16(p + 6);
            r.id = (rid & 0x8000) ? "#" + dec(rid & 0x7fff) : rsrc_string(base, rid);
            if (boot && !(r.flags & kPreload)) r.off += overlay_at;
            out.push_back(std::move(r));
        }
    }
    return out;
}

std::vector<std::pair<int64_t, std::string>> embedded(Bytes data, int64_t start) {
    std::vector<std::pair<int64_t, std::string>> out;
    int64_t i = data.find("NE", 2, start);
    while (i != -1 && i + 0x40 <= data.size()) {
        uint16_t nseg = data.u16(i + 0x1C), segtab = data.u16(i + 0x22), resnm = data.u16(i + 0x26);
        if (0 < nseg && nseg < 255 && 0x40 <= segtab && segtab < 0x100 && segtab <= resnm && resnm < 0x1000 &&
            i + resnm < data.size()) {
            uint8_t ln = data.at(i + resnm);
            Bytes name = data.slice(i + resnm + 1, i + resnm + 1 + ln);
            if (0 < ln && ln <= 8 && b_isalnum(name) && b_isalpha(name.slice(0, 1)) && b_is_upper_form(name))
                out.emplace_back(i, ascii(name));
        }
        i = data.find("NE", 2, i + 2);
    }
    return out;
}

int64_t tables_end(const Module &m) {
    const Header &h = m.h;
    const Bytes d = m.data;
    const int64_t o = m.ne_off;
    int64_t end = std::max({o + 0x40, o + h.seg_tab + 8 * int64_t(h.n_segs), o + h.entry_off + int64_t(h.entry_len),
                            o + h.mod_tab + 2 * int64_t(h.n_modrefs)});
    int64_t p = o + h.resname_tab;                         // resident names
    while (p < d.size() && d.at(p)) p += d.at(p) + 3;
    end = std::max(end, p + 1);
    if (h.rsrc_tab != h.resname_tab) {                     // resource table, then its names
        p = o + h.rsrc_tab + 2;
        while (m.u16(p)) p += 8 + 12 * int64_t(m.u16(p + 2));
        p += 2;
        while (p < d.size() && d.at(p)) p += d.at(p) + 1;
        end = std::max(end, p + 1);
    }
    const int64_t imp = o + h.imp_tab;                     // imported names that are referred to
    std::vector<int64_t> refs;
    for (int64_t i = 0; i < h.n_modrefs; ++i) refs.push_back(m.u16(o + h.mod_tab + 2 * i));
    for (const Segment &s : m.segments())
        for (const Relocation &r : m.relocations(s.flags, s.file_off, s.length))
            if (r.kind == Relocation::ImportName) refs.push_back(r.name_off);
    for (int64_t off : refs)
        if (imp + off < d.size()) end = std::max(end, imp + off + 1 + d.at(imp + off));
    return end;
}

namespace {
// struct.pack_into("<H" / "<I", out, at, v), with its two errors
void pack_into(std::string &out, int64_t at, uint64_t v, int width) {
    const uint64_t top = width == 2 ? 0xFFFF : 0xFFFFFFFF;
    if (v > top)
        raise("struct.error", std::string("'") + (width == 2 ? "H" : "I") + "' format requires 0 <= number <= " + dec(int64_t(top)));
    if (at < 0 || at + width > int64_t(out.size()))
        raise("struct.error", "pack_into requires a buffer of at least " + dec(at + width) + " bytes for packing " +
                                  dec(width) + " bytes at offset " + dec(at) + " (actual buffer size is " + dec(int64_t(out.size())) + ")");
    for (int k = 0; k < width; ++k) out[size_t(at + k)] = char(v >> (8 * k));
}

// place(blob, sh): the blob appended at the next multiple of 1 << sh -> where, in those units
int64_t place(std::string &out, Bytes blob, int64_t sh) {
    if (sh > 40) fallback("an alignment past memory");
    const int64_t unit = int64_t(1) << sh;
    const int64_t pad = (unit - int64_t(out.size()) % unit) % unit;
    out.append(size_t(pad), '\0');
    const int64_t at = int64_t(out.size());
    out.append(reinterpret_cast<const char *>(blob.data()), size_t(blob.size()));
    return at >> sh;
}
}  // namespace

std::string standalone(const Module &m) {
    const Header &h = m.h;
    const Bytes d = m.data;
    const int64_t o = m.ne_off;
    const int64_t shift = h.align_shift;
    std::string out(0x40, '\0');
    out[0] = 'M';
    out[1] = 'Z';
    pack_into(out, 0x18, 0x40, 2);
    pack_into(out, 0x3C, 0x40, 4);
    Bytes tables = d.slice(o, tables_end(m));
    out.append(reinterpret_cast<const char *>(tables.data()), size_t(tables.size()));
    const auto segs = m.segments();
    for (size_t i = 0; i < segs.size(); ++i) {
        const Segment &s = segs[i];
        if (!s.file_off) continue;
        int64_t end = s.file_off + s.length;
        if ((s.flags & kRelocInfo) && end + 2 <= d.size()) end += 2 + 8 * int64_t(m.u16(end));
        pack_into(out, 0x40 + h.seg_tab + 8 * int64_t(i), uint64_t(place(out, d.slice(s.file_off, end), shift)), 2);
    }
    if (h.rsrc_tab != h.resname_tab) {
        const auto res = m.resources();
        size_t next = 0;
        int64_t p = o + h.rsrc_tab;
        const int64_t rshift = m.u16(p);
        p += 2;
        while (m.u16(p)) {
            const int64_t count = m.u16(p + 2);
            p += 8;
            for (int64_t k = 0; k < count; ++k, p += 12) {
                if (next >= res.size()) raise("StopIteration", "");
                const Resource &r = res[next++];
                pack_into(out, 0x40 + p - o, uint64_t(place(out, d.slice(r.off, r.off + r.length), rshift)), 2);
            }
        }
    }
    const auto names = m.nonresident_names();
    std::string table;
    for (const Name &n : names) {
        table += char(n.name.size());
        table.append(reinterpret_cast<const char *>(n.name.data()), size_t(n.name.size()));
        table += char(n.ordinal);
        table += char(n.ordinal >> 8);
    }
    table += '\0';
    pack_into(out, 0x40 + 0x2C, names.empty() ? 0 : out.size(), 4);
    pack_into(out, 0x40 + 0x20, names.empty() ? 0 : table.size(), 2);
    if (!names.empty()) out += table;
    return out;
}

}  // namespace cbf::ne
