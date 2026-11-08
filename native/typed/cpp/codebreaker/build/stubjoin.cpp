// codebreaker/build/stubjoin.cpp -- C++ port of codebreaker.build.stubjoin (see the header).

#include "codebreaker/build/stubjoin.hpp"

namespace codebreaker::build::stubjoin {

int64_t _u(const py::bytes& d, int64_t at, const char* f) {
    return py::unpack_from1<int64_t>(f, d, at);
}

int64_t _align(int64_t n, int64_t a) {
    return py::checked_mul(py::floordiv(py::checked_sub(py::checked_add(n, a), 1), a), a);
}

py::bytes join(const py::bytes& stub, const py::bytes& built, std::optional<int64_t> at) {
    int64_t lfa = _u(built, 0x3C);
    int64_t place = (at && *at != 0 && *at >= py::len(stub)) ? *at : _align(py::len(stub), 16);
    py::bytes tail;
    if (built.slice(lfa, lfa + 2) == py::bytes("PE")) {
        tail = _pe(built, lfa, place);
    } else if (built.slice(lfa, lfa + 2) == py::bytes("NE")) {
        tail = _ne(built, lfa, place);
    } else {
        throw py::ValueError("not a PE or NE module");
    }
    py::bytearray head = py::bytearray(stub) + py::bytes::zeros(place - py::len(stub));
    py::pack_into("<I", head, 0x3C, place);
    return py::bytes(head.view()) + tail;
}

py::bytes _pe(const py::bytes& b, int64_t lfa, int64_t at) {
    int64_t opt = lfa + 24;
    int64_t nsec = _u(b, lfa + 6, "<H");
    int64_t optsize = _u(b, lfa + 20, "<H");
    int64_t falign = _u(b, opt + 36);
    int64_t old_soh = _u(b, opt + 60);
    bool plus = _u(b, opt, "<H") == 0x20B;
    int64_t dirs = opt + (plus ? 112 : 96);
    int64_t sec = opt + optsize;
    py::bytearray headers(b.slice(lfa, sec + 40 * nsec));
    int64_t new_soh = _align(at + py::len(headers), falign);
    int64_t delta = new_soh - old_soh;

    auto move = [&](int64_t off) {                         // a field of the headers holding a file offset
        int64_t v = _u(headers, off - lfa);
        if (v) {
            py::pack_into("<I", headers, off - lfa, v + delta);
        }
    };
    py::pack_into("<I", headers, opt + 60 - lfa, new_soh);
    move(lfa + 12);                                        // PointerToSymbolTable
    py::bytearray body(b.slice_from(old_soh));
    py::list<std::tuple<int64_t, int64_t, int64_t>> sections;
    for (int64_t i = 0; i < nsec; ++i) {
        int64_t s = sec + 40 * i;
        sections.append({_u(b, s + 12), _u(b, s + 16), _u(b, s + 20)});     // rva, raw size, raw offset
        for (int64_t f : {20, 24, 28}) {                   // raw data, relocations, line numbers
            move(s + f);
        }
    }
    // the certificates (data directory 4) are a file offset; the debug directory's entries hold one
    if (_u(b, dirs + 4 * 8)) {
        move(dirs + 4 * 8);
    }
    int64_t drva = _u(b, dirs + 6 * 8);
    int64_t dsize = _u(b, dirs + 6 * 8 + 4);
    for (const auto& [rva, rsize, raw] : sections) {
        if (drva && rva <= drva && drva < rva + rsize) {
            int64_t at_debug = raw + drva - rva - old_soh;
            for (int64_t k = 0; k < py::floordiv(dsize, 28); ++k) {
                int64_t e = at_debug + 28 * k + 24;
                if (_u(body, e)) {
                    py::pack_into("<I", body, e, _u(body, e) + delta);
                }
            }
        }
    }
    return py::bytes(headers.view()) + py::bytes::zeros(new_soh - at - py::len(headers)) + py::bytes(body.view());
}

py::bytes _ne(const py::bytes& b, int64_t lfa, int64_t at) {
    int64_t shift = _u(b, lfa + 0x32, "<H");
    int64_t nseg = _u(b, lfa + 0x1C, "<H");
    int64_t segtab = _u(b, lfa + 0x22, "<H");
    int64_t restab = _u(b, lfa + 0x24, "<H");
    int64_t resname = _u(b, lfa + 0x26, "<H");
    // where the data starts: the first segment or resource (sector offsets from the file start)
    py::list<int64_t> sectors;
    for (int64_t i = 0; i < nseg; ++i) {
        sectors.append(_u(b, lfa + segtab + 8 * i, "<H"));
    }
    py::list<int64_t> res;                                 // offsets (in the header) of resource sectors
    if (restab != resname) {
        int64_t r = lfa + restab;
        int64_t rshift = _u(b, r, "<H");
        r += 2;
        while (_u(b, r, "<H")) {
            int64_t count = _u(b, r + 2, "<H");
            for (int64_t k = 0; k < count; ++k) {
                res.append(r + 8 + 12 * k);
            }
            r += 8 + 12 * count;
        }
        if (rshift != shift) {
            throw py::ValueError("resources aligned differently from segments");
        }
    }
    int64_t unit = py::shl(1, shift);                      // (Python's int is unbounded: a shift of 63+ raises OverflowError here)
    py::list<int64_t> firsts;
    for (int64_t s : sectors) {
        if (s) {
            firsts.append(s);
        }
    }
    for (int64_t o : res) {
        if (_u(b, o, "<H")) {
            firsts.append(_u(b, o, "<H"));
        }
    }
    int64_t data_from = py::truthy(firsts) ? py::checked_mul(py::min_of(firsts), unit) : py::len(b);
    int64_t nonres = _u(b, lfa + 0x2C);
    py::bytearray header(b.slice(lfa, data_from));         // the header, its tables -- and the
    int64_t new_data = _align(at + py::len(header), unit); // non-resident names when before the data
    int64_t delta = py::floordiv(new_data - data_from, unit);
    for (int64_t i = 0; i < py::len(sectors); ++i) {
        int64_t s = sectors[i];
        if (s) {
            py::pack_into("<H", header, segtab + 8 * i, s + delta);
        }
    }
    for (int64_t o : res) {
        int64_t v = _u(b, o, "<H");
        if (v) {
            py::pack_into("<H", header, o - lfa, v + delta);
        }
    }
    if (nonres) {
        int64_t moved = nonres < data_from ? at - lfa : py::checked_mul(delta, unit);
        py::pack_into("<I", header, 0x2C, nonres + moved);
    }
    return py::bytes(header.view()) + py::bytes::zeros(new_data - at - py::len(header)) + b.slice_from(data_from);
}

}  // namespace codebreaker::build::stubjoin
