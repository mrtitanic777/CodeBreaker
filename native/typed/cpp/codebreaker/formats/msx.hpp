// codebreaker/formats/msx.hpp -- C++17 port of codebreaker.formats.msx.
//
// msx.py -- MSX cartridge images (`.rom`, `.mx1`, `.mx2`).
//
//     py::ref<Program> p = msx::read(path);
//     p->origin, p->image         where the cartridge sits and its bytes
//     p->entries                  {address: name}: the header's INIT, STATEMENT and DEVICE entries
//
// A cartridge starts "AB", then four addresses: INIT (called at power-on), STATEMENT (the BASIC
// CALL extension), DEVICE, and TEXT (a BASIC program in the ROM); zero where unused. A cartridge sits
// in page 1 ($4000) -- 32 KB ones fill pages 1 and 2 -- or, rarely, at $0000 or $8000; where is told
// by its INIT address. Cartridges over 48 KB switch banks through a mapper (Konami, ASCII ...), which
// this lane does not follow yet: only the part seen at power-on would come back.
#pragma once

#include "py.hpp"

namespace codebreaker::formats::msx {

// Program.header: a dict with the four fixed keys of the cartridge header (a record)
struct ReadHeader {
    int64_t init = 0;
    int64_t statement = 0;
    int64_t device = 0;
    int64_t text = 0;
};
template <class F> void for_each_field(const ReadHeader& r, F&& f) {
    f("init", r.init);
    f("statement", r.statement);
    f("device", r.device);
    f("text", r.text);
}
template <class F> void for_each_field_mut(ReadHeader& r, F&& f) {
    f("init", r.init, true);
    f("statement", r.statement, true);
    f("device", r.device, true);
    f("text", r.text, true);
}

// the plain attribute holder the module's `Program` is
class Program : public py::object {
public:
    int64_t origin = 0;
    py::bytes image;
    ReadHeader header;
    py::dict<int64_t, py::str> entries;
    py::str path;
};

extern const std::tuple<py::str, py::str, py::str> EXTENSIONS;

bool is_msx(const py::str& path);
py::ref<Program> read(const py::str& path);

}  // namespace codebreaker::formats::msx
