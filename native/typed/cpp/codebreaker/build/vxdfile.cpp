// codebreaker/build/vxdfile.cpp -- C++17 port of codebreaker.build.vxdfile (see vxdfile.hpp).

#include "codebreaker/build/vxdfile.hpp"

namespace codebreaker::build::vxdfile {

const int64_t PAGE_ALIGN = 0x200;

int64_t _u(const py::bytes& d, int64_t at) {
    return py::unpack_from1<int64_t>("<I", d, at);
}

// The .def text the original's non-resident name table was made from. -> str
py::str definitions(const py::bytes& original, int64_t lfa) {
    int64_t at = _u(original, lfa + 0x88);
    int64_t n = _u(original, lfa + 0x8C);
    py::bytes table = original.slice(at, at + n);
    py::list<std::tuple<py::str, int64_t>> names;
    int64_t i = 0;
    while (i < py::len(table) && table[i] != 0) {
        int64_t k = table[i];
        names.append(std::make_tuple(table.slice(i + 1, i + 1 + k).decode("latin1"),
                                     py::unpack_from1<int64_t>("<H", table, i + 1 + k)));
        i += 3 + k;
    }
    py::list<py::str> lines;
    for (const auto& [name, ordinal] : names) {
        if (ordinal == 0) {
            lines.append(py::percent("DESCRIPTION '%s'", name.replace(py::str("'"), py::str("''"))));
        }
    }
    py::list<std::tuple<py::str, int64_t>> exports;
    for (const auto& [name, ordinal] : names) {
        if (ordinal != 0) {
            exports.append(std::make_tuple(name, ordinal));
        }
    }
    if (!exports.empty()) {
        lines.append(py::str("EXPORTS"));
        for (const auto& [name, ordinal] : exports) {
            lines.append(py::percent("    %s @%d", name, ordinal));
        }
    }
    return py::join(py::str("\n"), lines) + py::str("\n");
}

// The LE of `built` (a whole VxD file) laid out to start at file offset `at` (a multiple of
// 16) as Microsoft's LINK lays it out: tables, pages from the next 512-byte boundary, the
// non-resident name table as linked, then `resource`. -> the bytes from `at` on.
py::bytes relay(const py::bytes& built, int64_t at, const py::bytes& resource) {
    int64_t lfa = _u(built, 0x3C);
    int64_t pages_from = _u(built, lfa + 0x80);   // file offsets in the built file
    int64_t nonres_at = _u(built, lfa + 0x88);
    int64_t nonres_len = _u(built, lfa + 0x8C);
    py::bytearray head = py::bytearray(built.slice(lfa, pages_from));
    py::bytes pages = nonres_at != 0 ? built.slice(pages_from, nonres_at) : built.slice_from(pages_from);
    py::bytes names = nonres_at != 0 ? built.slice(nonres_at, nonres_at + nonres_len) : py::bytes();
    int64_t start = py::floordiv(at + py::len(head) + PAGE_ALIGN - 1, PAGE_ALIGN) * PAGE_ALIGN;
    head += py::bytes::zeros(start - at - py::len(head));
    py::pack_into("<I", head, 0x80, start);
    if (!names.empty()) {
        py::pack_into("<I", head, 0x88, start + py::len(pages));
    }
    if (!resource.empty()) {
        py::pack_into("<II", head, 0xB8, start + py::len(pages) + py::len(names), py::len(resource));
    }
    return head.view() + pages + names + resource;
}

}  // namespace codebreaker::build::vxdfile
