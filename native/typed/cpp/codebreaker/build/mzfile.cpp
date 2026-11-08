// codebreaker/build/mzfile.cpp -- C++ from codebreaker.build.mzfile.

#include "codebreaker/build/mzfile.hpp"

namespace codebreaker::build::mzfile {

const py::list<std::tuple<int64_t, int64_t>> BUILT_ = py::list<std::tuple<int64_t, int64_t>>{
    {0x02, 2}, {0x04, 2}, {0x06, 2}, {0x0A, 2}, {0x0E, 2}, {0x10, 2}, {0x14, 2}, {0x16, 2}};

py::bytes template_(const py::bytes& header) {
    py::bytearray t(header);
    int64_t nreloc = py::unpack_from1<int64_t>("<H", t, 6);
    int64_t reloc_off = py::unpack_from1<int64_t>("<H", t, 0x18);
    for (const auto& [at, n] : BUILT_) {
        t.set_slice(at, at + n, py::bytes::zeros(n));
    }
    t.set_slice(reloc_off, reloc_off + 4 * nreloc, py::bytes::zeros(4 * nreloc));
    return py::bytes(t.view());
}

py::bytes compose(const py::bytes& tpl, const py::bytes& module, const py::list<std::tuple<int64_t, int64_t>>& relocs,
                  int64_t cs, int64_t ip, int64_t ss, int64_t sp, int64_t min_alloc, bool full_last) {
    py::bytearray head(tpl);
    int64_t reloc_off = py::unpack_from1<int64_t>("<H", head, 0x18);
    py::bytearray room = head.slice(reloc_off, reloc_off + 4 * py::len(relocs));
    bool occupied = false;                                  // any(room)
    for (int64_t c : room) {
        if (c != 0) {
            occupied = true;
            break;
        }
    }
    if (reloc_off + 4 * py::len(relocs) > head.size() || occupied) {
        throw py::ValueError(std::to_string(py::len(relocs)) + " relocations do not fit in the header's room for them");
    }
    int64_t i = 0;
    for (const auto& [off, seg] : relocs) {
        py::pack_into("<HH", head, reloc_off + 4 * i, off, seg);
        ++i;
    }
    int64_t total = head.size() + module.size();
    // (a last page that is full: 0, or 512 as some linkers wrote it -- `full_last`)
    int64_t last = total % 512;
    if (last == 0) {
        last = full_last ? 512 : 0;
    }
    py::pack_into("<HHH", head, 2, last, (total + 511) / 512, py::len(relocs));
    py::pack_into("<H", head, 0x0A, min_alloc);
    py::pack_into("<HH", head, 0x0E, ss, sp);
    py::pack_into("<HH", head, 0x14, ip, cs);
    return py::bytes(head.view()) + module;
}

}  // namespace codebreaker::build::mzfile
