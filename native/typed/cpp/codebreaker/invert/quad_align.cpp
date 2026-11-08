// codebreaker/invert/quad_align.cpp -- C++17 port of codebreaker.invert.quad_align (see quad_align.hpp).

#include "codebreaker/invert/quad_align.hpp"

#include <optional>

#include "codebreaker/formats/omf.hpp"
#include "py_os.hpp"

namespace codebreaker::invert::quad_align {

namespace omf = codebreaker::formats::omf;

py::list<py::list<QuadRecord>> parse_quads(const py::str& qpath) {
    py::bytes d;
    {
        py::file f = py::open_rb(qpath);
        d = f.read();
    }
    // name pool at tail
    int64_t i = py::len(d);
    while (i > 0 && ((32 <= d[i - 1] && d[i - 1] < 127) || d[i - 1] == 0)) {
        i -= 1;
    }
    int64_t end = i;
    py::list<py::list<QuadRecord>> funcs;
    std::optional<py::list<QuadRecord>> cur;   // None until the first FUNC-BEGIN (a list is a shared handle)
    int64_t o = 0x5c;
    while (o + 2 <= end) {
        int64_t op = d[o];
        int64_t ln = d[o + 1];
        if (op == 0xff) {
            break;
        }
        QuadRecord rec = std::make_tuple(op, d.slice(o + 2, o + 2 + ln));
        if (op == 0x23) {   // FUNC-BEGIN
            cur = py::list<QuadRecord>();
            funcs.append(*cur);
        }
        if (cur) {
            cur->append(rec);
        }
        o += 2 + ln;
    }
    return funcs;
}

py::list<std::tuple<py::str, int64_t, py::bytes>> parse_obj_funcs(const py::str& objpath) {
    py::bytes data;
    {
        py::file f = py::open_rb(objpath);
        data = f.read();
    }
    py::list<py::str> lnames;
    lnames.append(py::str(""));
    py::list<py::str> segs;
    py::bytes code;
    py::list<std::tuple<py::str, int64_t>> pubs;
    omf::Walk w = omf::walk(data);
    while (auto item = w.next()) {
        auto [off, rt, body] = *item;
        (void)off;
        if (rt == 0x96) {
            int64_t j = 0;
            while (j < py::len(body)) {
                int64_t n = body[j];
                lnames.append(body.slice(j + 1, j + 1 + n).decode("latin1"));
                j += 1 + n;
            }
        } else if (rt == 0x98 || rt == 0x99) {
            auto [si, j] = omf::index(body, 3);
            (void)j;
            segs.append(si < py::len(lnames) ? lnames[si] : py::to_str(si));
        } else if (rt == 0xa0 || rt == 0xa1) {
            auto [seg, j0] = omf::index(body, 0);
            int64_t j = j0;
            int64_t do_ = py::unpack_from1<int64_t>("<H", body, j);   // (`do` in the Python)
            j += 2;
            py::bytes pl = body.slice_from(j);
            py::str sn = (0 < seg && seg <= py::len(segs)) ? segs[seg - 1] : py::str("");
            if (sn == py::str("PROG")) {
                if (do_ + py::len(pl) > py::len(code)) {
                    code = code + py::bytes::zeros(do_ + py::len(pl) - py::len(code));
                }
                code = code.slice_to(do_) + pl + code.slice_from(do_ + py::len(pl));
            }
        } else if (rt == 0x90 || rt == 0x91) {
            auto [g, j0] = omf::index(body, 0);
            (void)g;
            auto [s, j1] = omf::index(body, j0);
            int64_t j = j1;
            if (s == 0) {
                j += 2;
            }
            while (j < py::len(body)) {
                int64_t n = body[j];
                py::str nm = body.slice(j + 1, j + 1 + n).decode("latin1");
                j += 1 + n;
                int64_t po = py::unpack_from1<int64_t>("<H", body, j);
                j += 2;
                auto [ignored, j2] = omf::index(body, j);
                (void)ignored;
                j = j2;
                pubs.append(std::make_tuple(nm, po));
            }
        }
    }
    // (the Python filters with `not p[0].isupper() or True`, which keeps every public)
    py::list<std::tuple<py::str, int64_t>> kept;
    for (const auto& p : pubs) {
        kept.append(p);
    }
    pubs = kept;
    pubs.sort([](const std::tuple<py::str, int64_t>& x) { return std::get<1>(x); });
    py::list<std::tuple<py::str, int64_t, py::bytes>> out;
    for (int64_t i = 0; i < py::len(pubs); ++i) {
        const auto& [nm, po] = pubs[i];
        int64_t end = i + 1 < py::len(pubs) ? std::get<1>(pubs[i + 1]) : py::len(code);
        out.append(std::make_tuple(nm, po, code.slice(po, end)));
    }
    return out;
}

// bytes.hex(' '): two hex digits per byte, separated by a space
static py::str hex_spaced(const py::bytes& b) {
    py::list<py::str> items;
    for (int64_t x : b) {
        items.append(py::percent("%02x", x));
    }
    return py::join(py::str(" "), items);
}

void main(const py::list<py::str>& argv) {
    py::list<py::list<QuadRecord>> quads = parse_quads(argv[1]);
    py::list<std::tuple<py::str, int64_t, py::bytes>> objf_all = parse_obj_funcs(argv[2]);
    // obj pubs include data symbols; keep those whose bytes look like a function (start 55/53/8b)
    py::list<std::tuple<py::str, int64_t, py::bytes>> objf;
    for (const auto& f : objf_all) {
        py::bytes first = std::get<2>(f).slice(0, 1);
        if (first == py::bytes("\x55") || first == py::bytes("\x53") || first == py::bytes("\x8b") || first == py::bytes("\xb8")) {
            objf.append(f);
        }
    }
    py::print(py::percent("quad funcs: %d | obj funcs: %d", py::len(quads), py::len(objf)));
    for (int64_t i = 0; i < py::len(objf); ++i) {
        const auto& [nm, po, code] = objf[i];
        (void)po;
        py::list<QuadRecord> q = i < py::len(quads) ? quads[i] : py::list<QuadRecord>();
        py::list<py::str> opnames;
        for (const auto& rec : q) {
            opnames.append(py::percent("%02x", std::get<0>(rec)));
        }
        py::str ops = py::join(py::str(" "), opnames);
        py::print(py::percent("\n=== %s  (%d bytes) ===", nm, py::len(code)));
        py::print(py::str("  quads: ") + ops);
        py::print(py::str("  code : ") + hex_spaced(code));
    }
}

}  // namespace codebreaker::invert::quad_align
