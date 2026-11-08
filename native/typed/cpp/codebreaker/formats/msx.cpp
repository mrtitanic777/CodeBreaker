// codebreaker/formats/msx.cpp -- C++17 port of codebreaker.formats.msx (see msx.hpp).

#include "codebreaker/formats/msx.hpp"

#include "py_os.hpp"

namespace codebreaker::formats::msx {

const std::tuple<py::str, py::str, py::str> EXTENSIONS =
    std::tuple<py::str, py::str, py::str>{py::str(".rom"), py::str(".mx1"), py::str(".mx2")};

bool is_msx(const py::str& path) {
    if (!path.lower().endswith(EXTENSIONS)) {
        return false;
    }
    py::bytes head;
    try {
        py::file f = py::open_rb(path);
        head = f.read(16);
    } catch (const py::OSError&) {
        return false;
    }
    if (head.slice_to(2) == py::bytes("AB")) {
        return true;
    }
    try {
        py::file f = py::open_rb(path);
        f.seek(0x4000);
        return f.read(2) == py::bytes("AB");
    } catch (const py::OSError&) {
        return false;
    }
}

py::ref<Program> read(const py::str& path) {
    py::bytes data;
    {
        py::file f = py::open_rb(path);
        data = f.read();
    }
    if (py::len(data) > 0xC000) {
        throw py::ValueError(py::percent("%s: a %d KB cartridge switches banks (a mapper), not read yet", path,
                                         py::floordiv(py::len(data), 1024)));
    }
    // The header is at the start of page 1 ($4000): the image's first byte when the cartridge starts there, or its
    // byte $4000 when it fills pages 0-2 from $0000 (48 KB: msx-wrally).
    int64_t at = 0;
    int64_t origin = 0;
    if (data.slice_to(2) == py::bytes("AB")) {
        at = 0;
        origin = 0x4000;
    } else if (py::len(data) > 0x4000 && data.slice(0x4000, 0x4002) == py::bytes("AB")) {
        at = 0x4000;
        origin = 0x0000;
    } else {
        throw py::ValueError(py::percent("%s: no MSX cartridge header", path));
    }
    auto [init, statement, device, text] = py::unpack_from<int64_t, int64_t, int64_t, int64_t>("<4H", data, at + 2);
    if (at == 0 && init != 0 && init < 0x4000) {
        origin = 0x0000;
    } else if (at == 0 && init >= 0x8000 && py::len(data) <= 0x4000) {
        origin = 0x8000;
    }
    py::ref<Program> p = std::make_shared<Program>();
    p->path = path;
    p->origin = origin;
    p->image = data;
    p->header = ReadHeader{init, statement, device, text};
    p->entries = py::dict<int64_t, py::str>{};
    const std::tuple<const char*, int64_t> found[3] = {{"init", init}, {"statement", statement}, {"device", device}};
    for (const auto& [name, a] : found) {
        if (a != 0 && (origin <= a && a < origin + py::len(data))) {
            p->entries.setdefault(a, py::str(name));
        }
    }
    return p;
}

}  // namespace codebreaker::formats::msx
