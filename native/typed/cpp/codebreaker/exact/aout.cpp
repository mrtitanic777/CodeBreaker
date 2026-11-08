// codebreaker/exact/aout.cpp -- C++ port of codebreaker.exact.aout (see aout.hpp).

#include "codebreaker/exact/aout.hpp"

namespace codebreaker::exact::aout {

// (0o407, 0o410, 0o411, 0x0107, 0x0109)
const py::list<int64_t> MAGICS = {0407, 0410, 0411, 0x0107, 0x0109};

bool is_aout(const py::bytes& data) {
    if (!(py::len(data) >= 16)) {
        return false;
    }
    int64_t magic = py::unpack_from1<int64_t>("<H", data, 0);
    for (int64_t m : MAGICS) {
        if (m == magic) {
            return true;
        }
    }
    return false;
}

Obj read(const py::bytes& data) {
    auto [magic, tsz, dsz, bsz, ssz, entry, unused, norel] =
        py::unpack_from<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t>("<8H", data, 0);
    (void)magic;
    (void)entry;
    (void)unused;
    Obj o;
    py::bytes text = data.slice(16, 16 + tsz);
    py::bytes dat = data.slice(16 + tsz, 16 + tsz + dsz);
    o.seg = py::dict<py::str, py::bytes>{{py::str("_TEXT"), text}, {py::str("_DATA"), dat}, {py::str("_BSS"), py::bytes::zeros(bsz)}};
    o.filled = py::dict<py::str, int64_t>{{py::str("_TEXT"), tsz}, {py::str("_DATA"), dsz}};
    o.segs = py::list<SegInfo>{SegInfo(py::str("_TEXT"), py::str("CODE"), tsz, 1), SegInfo(py::str("_DATA"), py::str("DATA"), dsz, 2),
                               SegInfo(py::str("_BSS"), py::str("BSS"), bsz, 2)};
    // o.groups stays empty
    int64_t p = 16 + tsz + dsz;
    py::list<int64_t> rel_t;
    py::list<int64_t> rel_d;
    if (!(norel != 0)) {
        for (int64_t k = 0; k < tsz; k += 2) {
            rel_t.append(py::unpack_from1<int64_t>("<H", data, p + k));
        }
        for (int64_t k = 0; k < dsz; k += 2) {
            rel_d.append(py::unpack_from1<int64_t>("<H", data, p + tsz + k));
        }
        p += tsz + dsz;
    }
    py::list<Sym> syms;
    for (int64_t q = p; q < p + ssz; q += 12) {
        py::str nm = py::split(data.slice(q, q + 8), py::bytes(std::string(1, '\0')))[0].decode("latin1");
        auto [ty, val] = py::unpack_from<int64_t, int64_t>("<HH", data, q + 8);
        syms.append(Sym(nm, ty, val));
    }
    o.syms = syms;
    // (the segment a symbol's type names)
    auto seg_of = [](int64_t t) -> std::optional<py::str> {
        switch (t) {
            case 2: return py::str("_TEXT");
            case 3: return py::str("_DATA");
            case 4: return py::str("_BSS");
            default: return std::nullopt;
        }
    };
    for (const Sym& s : syms) {
        const py::str& nm = std::get<0>(s);
        int64_t ty = std::get<1>(s);
        int64_t val = std::get<2>(s);
        if ((ty & 0x20) != 0 && seg_of(ty & 7).has_value()) {
            // (a value counts from its own segment's start, as the relocated words do: XENIX C's objects keep I and D apart)
            o.pubs.append(Pub(nm, *seg_of(ty & 7), val));
        } else if ((ty & 0x20) != 0 && (ty & 7) == 0) {
            o.ext.append(nm);
            if (val != 0) {
                o.comm.set(nm, std::make_tuple(val, false));   // (an external with a size: a communal variable)
            }
        }
    }
    py::str names_seg[2] = {py::str("_TEXT"), py::str("_DATA")};
    const py::list<int64_t>* rels[2] = {&rel_t, &rel_d};
    for (int w = 0; w < 2; ++w) {
        const py::str& segname = names_seg[w];
        int64_t j = 0;
        for (int64_t v : *rels[w]) {
            int64_t jj = j++;
            if (v == 0) {
                continue;
            }
            int64_t k = 2 * jj + (v >> 15);
            int64_t kind = (v >> 1) & 7;
            int64_t pcrel = v & 1;
            Target tgt;
            if (kind == 4) {
                tgt = Target(py::str("ext"), std::get<0>(syms[(v & 0x7FFF) >> 4]));
            } else {
                py::str which = kind == 1 ? py::str("_TEXT") : kind == 2 ? py::str("_DATA") : kind == 3 ? py::str("_BSS") : py::str("?");
                tgt = Target(py::str("seg"), which);
            }
            int64_t disp = 0;   // (a word naming the data or the bss counts from that
            if (pcrel != 0) {   //  segment's start: the linker adds the piece's place)
                o.fix.append(Fixup(segname, k, py::str("rel"), tgt, k + 2));
            } else {
                o.fix.append(Fixup(segname, k, py::str("off"), tgt, disp));
            }
        }
    }
    o.raw = data;
    return o;
}

}  // namespace codebreaker::exact::aout
