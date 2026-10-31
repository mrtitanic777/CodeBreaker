// codebreaker/formats/ne.cpp -- C++ from codebreaker.formats.ne by CodeBreaker's Python front end.

#include "codebreaker/formats/ne.hpp"

namespace codebreaker::formats::ne {

const int64_t PRELOAD = 0x0040;

NEFile::NEFile(const py::str& path) {
    this->path = path;
    data = py::open_rb(path).read();
    py::bytes d = data;
    if (d.slice_to(2) != py::bytes("MZ")) {
        throw py::ValueError("not an MZ file");
    }
    ne_off = py::unpack_from1<int64_t>("<I", d, 0x3C);
    if (d.slice(ne_off, ne_off + 2) != py::bytes("NE")) {
        throw py::ValueError(py::str("no NE signature at 0x") + py::format(ne_off, "x") + " (got " + py::repr(d.slice(ne_off, ne_off + 2)) + ")");
    }
    _parse_header();
}

int64_t NEFile::u16(int64_t off) {
    return py::unpack_from1<int64_t>("<H", data, off);
}

int64_t NEFile::u32(int64_t off) {
    return py::unpack_from1<int64_t>("<I", data, off);
}

void NEFile::_parse_header() {
    int64_t o = ne_off;
    HeaderRecord h;
    h.linker_ver = data[o + 0x02];
    h.linker_rev = data[o + 0x03];
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
    h.align_shift = py::or_(u16(o + 0x32), 9);
    h.n_resources = u16(o + 0x34);
    h.exetype = data[o + 0x36];
    h.expect_winver = h.seg_tab >= 0x40 ? std::optional<int64_t>(u16(o + 0x3E)) : std::nullopt;
    this->h = h;
}

// Returns list of (name, ordinal). First entry = module/description.
py::list<std::tuple<py::str, int64_t>> NEFile::_read_name_table(int64_t file_off) {
    py::list<std::tuple<py::str, int64_t>> out;
    int64_t p = file_off;
    while (p < py::len(data)) {
        int64_t ln = data[p];
        p += 1;
        if (ln == 0) {
            break;
        }
        py::str name = data.slice(p, p + ln).decode("latin1", "strict");
        p += ln;
        int64_t ordv = u16(p);
        p += 2;
        out.append(std::make_tuple(name, ordv));
    }
    return out;
}

py::list<std::tuple<py::str, int64_t>> NEFile::resident_names() {
    return _read_name_table(ne_off + h.resname_tab);
}

py::list<std::tuple<py::str, int64_t>> NEFile::nonresident_names() {
    if (!(h.nonres_off != 0)) {
        return py::list<std::tuple<py::str, int64_t>>{};
    }
    return _read_name_table(h.nonres_off);
}

// This module's own name — the first resident-name-table entry.
py::str NEFile::module_name() {
    py::list<std::tuple<py::str, int64_t>> r = resident_names();
    return !r.empty() ? std::get<0>(r[0]) : "SELF";
}

// Imported module names, in module-reference-table order (1-based).
py::list<py::str> NEFile::module_refs() {
    py::list<py::str> names;
    int64_t base = ne_off + h.mod_tab;
    int64_t imp = ne_off + h.imp_tab;
    const int64_t stop = h.n_modrefs;
    for (int64_t i = 0; i < stop; ++i) {
        int64_t off = u16(base + i * 2);
        int64_t ln = data[imp + off];
        names.append(data.slice(imp + off + 1, imp + off + 1 + ln).decode("latin1", "strict"));
    }
    return names;
}

py::str NEFile::imported_name(int64_t off) {
    int64_t base = ne_off + h.imp_tab;
    if (!(0 <= base + off && base + off < py::len(data))) {
        return "?";
    }
    int64_t ln = data[base + off];
    return data.slice(base + off + 1, base + off + 1 + ln).decode("latin1", "replace");
}

// The ordinals the entry table actually exports, in order (unused gaps skipped).
// Exports by ordinal only have no resident name, so the name tables alone miss them.
py::list<int64_t> NEFile::export_ordinals() {
    py::list<int64_t> result;
    for (const Entry& e : entries()) {
        if (e.kind != "UNUSED") {
            result.append(e.ordinal.value());
        }
    }
    return result;
}

// Parse entry-table bundles -> list of dict(ordinal, seg, offset, flags).
py::list<Entry> NEFile::entries() {
    py::list<Entry> out;
    int64_t p = ne_off + h.entry_off;
    int64_t end = p + h.entry_len;
    int64_t ordinal = 1;
    while (p < end) {
        int64_t cnt = data[p];
        int64_t seg_ind = data[p + 1];
        p += 2;
        if (cnt == 0) {
            break;
        }
        if (seg_ind == 0x00) {
            for (int64_t _ = 0; _ < cnt; ++_) {
                out.append(Entry{ordinal, py::str("UNUSED"), {}, {}, {}});
                ordinal += 1;
            }
            continue;
        }
        int64_t flags = 0;
        int64_t off = 0;
        if (seg_ind == 0xFF) {
            for (int64_t _ = 0; _ < cnt; ++_) {
                flags = data[p];
                int64_t seg = data[p + 3];
                off = u16(p + 4);
                out.append(Entry{ordinal, py::str("MOVEABLE"), seg, off, flags});
                p += 6;
                ordinal += 1;
            }
        } else {
            for (int64_t _ = 0; _ < cnt; ++_) {
                flags = data[p];
                off = u16(p + 1);
                out.append(Entry{ordinal, py::str("FIXED"), seg_ind, off, flags});
                p += 3;
                ordinal += 1;
            }
        }
    }
    return out;
}

py::str NEFile::_rsrc_string(int64_t base, int64_t off) {
    int64_t p = base + off;
    int64_t ln = data[p];
    return data.slice(p + 1, p + 1 + ln).decode("latin1", "strict");
}

// [(offset, module name)] of every NE header inside a file that holds several modules.
py::list<std::tuple<int64_t, py::str>> embedded(const py::bytes& data, int64_t start) {
    py::list<std::tuple<int64_t, py::str>> out;
    int64_t i = data.find(py::bytes("NE"), start);
    while (i != -1 && i + 0x40 <= py::len(data)) {
        int64_t nseg = py::unpack_from1<int64_t>("<H", data, i + 0x1C);
        int64_t segtab = py::unpack_from1<int64_t>("<H", data, i + 0x22);
        int64_t resnm = py::unpack_from1<int64_t>("<H", data, i + 0x26);
        if (0 < nseg && nseg < 255 && (0x40 <= segtab && segtab < 0x100) && (segtab <= resnm && resnm < 0x1000) && i + resnm < py::len(data)) {
            int64_t ln = data[i + resnm];
            py::bytes name = data.slice(i + resnm + 1, i + resnm + 1 + ln);
            if (0 < ln && ln <= 8 && name.isalnum() && name.slice_to(1).isalpha() && name == name.upper()) {
                out.append(std::make_tuple(i, name.decode("ascii", "strict")));
            }
        }
        i = data.find(py::bytes("NE"), i + 2);
    }
    return out;
}

}  // namespace codebreaker::formats::ne
