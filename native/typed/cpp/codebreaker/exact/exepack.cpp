// codebreaker/exact/exepack.cpp -- C++ from codebreaker.exact.exepack (the body is the judge-proven translation).

#include "codebreaker/exact/exepack.hpp"

namespace codebreaker::exact::exepack {

const py::bytes SIG = py::bytes("RB");
const py::bytes MSG = py::bytes("Packed file is corrupt");

py::ref<Packed> read(const py::bytes& b) {
    if (!py::in(b.slice_to(2), std::tuple<py::bytes, py::bytes>{py::bytes("MZ"), py::bytes("ZM")}) || py::len(b) < 0x20) {
        return nullptr;
    }
    std::tuple<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t> h = py::unpack_from<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t>("<14H", b, 0);
    int64_t hdr = std::get<4>(h) * 16;
    int64_t at = hdr + std::get<11>(h) * 16;                // (the packed file's CS:0 - EXEPACK's header)
    if (at + 18 > py::len(b) || (b.slice(at + 14, at + 16) != SIG && b.slice(at + 16, at + 18) != SIG)) {
        return nullptr;
    }
    py::ref<Packed> p = std::make_shared<Packed>();
    std::tuple<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t> parts = py::unpack_from<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t>("<7H", b, at);
    p->ip = std::get<0>(parts);
    p->cs = std::get<1>(parts);
    p->mem = std::get<2>(parts);
    p->size = std::get<3>(parts);
    p->sp = std::get<4>(parts);
    p->ss = std::get<5>(parts);
    p->dest = std::get<6>(parts);
    p->relocs = py::list<int64_t>{};
    int64_t q = b.find(MSG, at);
    if (q >= 0) {
        q += py::len(MSG);
        for (int64_t g = 0; g < 16; ++g) {
            if (q + 2 > py::len(b)) {
                break;
            }
            int64_t n = py::unpack_from1<int64_t>("<H", b, q);
            q += 2;
            py::list<int64_t> items_2;
            for (int64_t k = 0; k < n; ++k) {
                items_2.append(g * 0x10000 + py::unpack_from1<int64_t>("<H", b, q + 2 * k));
            }
            p->relocs = p->relocs + items_2;
            q += 2 * n;
        }
    }
    return p;
}

}  // namespace codebreaker::exact::exepack
