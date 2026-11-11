// codebreaker/invert/quad_align.hpp -- SHIM over cb/invert/quad_align.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer (invert/early_c_forward).
#pragma once

#include <cstdint>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/invert/quad_align.hpp"

namespace codebreaker::invert::quad_align {

// one quad record: (opcode, payload bytes)
using QuadRecord = std::tuple<int64_t, py::bytes>;

// Return list of functions, each = list of (opcode, payload_bytes).
inline py::list<py::list<QuadRecord>> parse_quads(const py::str& qpath) {
    return cb::compat::guard([&] {
        py::list<py::list<QuadRecord>> out;
        for (const auto& function : cb::invert::quad_align::parse_quads(cb::fs::read_file(cb::fs::from_text(cb::compat::from_py(qpath))))) {
            py::list<QuadRecord> records;
            for (const auto& q : function) records.append(QuadRecord(q.opcode, cb::compat::to_py(cb::ByteView(q.payload))));
            out.append(records);
        }
        return out;
    });
}

// Return [(name, off, bytes)] for PROG functions, in address order.
inline py::list<std::tuple<py::str, int64_t, py::bytes>> parse_obj_funcs(const py::str& objpath) {
    return cb::compat::guard([&] {
        py::list<std::tuple<py::str, int64_t, py::bytes>> out;
        for (const auto& f : cb::invert::quad_align::parse_obj_funcs(cb::fs::read_file(cb::fs::from_text(cb::compat::from_py(objpath))))) {
            out.append(std::make_tuple(cb::compat::to_py(f.name), static_cast<int64_t>(f.offset), cb::compat::to_py(cb::ByteView(f.code))));
        }
        return out;
    });
}

// The script: `python quad_align.py <quads> <obj>`; `argv` is sys.argv (the program name first). Prints the alignment.
inline void main(const py::list<py::str>& argv) {
    const py::str quads = argv[1], object = argv[2];   // (IndexError for a missing argument)
    cb::compat::guard([&] {
        std::string text = cb::invert::quad_align::report(cb::fs::read_file(cb::fs::from_text(cb::compat::from_py(quads))),
                                                          cb::fs::read_file(cb::fs::from_text(cb::compat::from_py(object))));
        text.pop_back();   // (print adds the last newline)
        py::print(cb::compat::to_py(text));
    });
}

}  // namespace codebreaker::invert::quad_align
