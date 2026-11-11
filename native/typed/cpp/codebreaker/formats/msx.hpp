// codebreaker/formats/msx.hpp -- compat shim over cb/formats/msx.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module itself is native/cb/formats/msx.{hpp,cpp}; this header keeps the old py:: surface for the ports that
// have not been refactored yet and is deleted with the last of them. Conversion and forwarding only.
#pragma once

#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/msx.hpp"

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

inline const std::tuple<py::str, py::str, py::str> EXTENSIONS =
    std::tuple<py::str, py::str, py::str>{py::str(".rom"), py::str(".mx1"), py::str(".mx2")};

inline bool is_msx(const py::str& path) { return cb::formats::msx::probe(cb::fs::from_text(cb::compat::from_py(path))); }

inline py::ref<Program> read(const py::str& path) {
    return cb::compat::guard([&] {
        const cb::formats::msx::Program p = cb::formats::msx::read(cb::fs::from_text(cb::compat::from_py(path)));
        py::ref<Program> out = std::make_shared<Program>();
        out->path = path;
        out->origin = p.origin;
        out->image = cb::compat::to_py(cb::ByteView(p.image));
        out->header.init = p.header.init;
        out->header.statement = p.header.statement;
        out->header.device = p.header.device;
        out->header.text = p.header.text;
        for (const cb::formats::msx::Entry& e : p.entries) out->entries.set(e.address, cb::compat::to_py(e.name));
        return out;
    });
}

}  // namespace codebreaker::formats::msx
